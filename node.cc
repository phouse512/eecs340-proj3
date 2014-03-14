#include "node.h"
#include "context.h"
#include "error.h"


Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) 
{}

Node::Node() 
{ throw GeneralException(); }

Node::Node(const Node &rhs) : 
  number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat) {}

Node & Node::operator=(const Node &rhs) 
{
  return *(new(this)Node(rhs));
}

void Node::SetNumber(const unsigned n) 
{ number=n;}

unsigned Node::GetNumber() const 
{ return number;}

void Node::SetLatency(const double l) 
{ lat=l;}

double Node::GetLatency() const 
{ return lat;}

void Node::SetBW(const double b) 
{ bw=b;}

double Node::GetBW() const 
{ return bw;}

Node::~Node()
{}

void Node::SendToNeighbors(const RoutingMessage *m)
{
  context->SendToNeighbors(this,m);
}

void Node::SendToNeighbor(const Node *n, const RoutingMessage *m)
{
  context->SendToNeighbor(this,n,m);
}

deque<Node*> *Node::GetNeighbors()
{
  return context->GetNeighbors(this);
}

void Node::SetTimeOut(const double timefromnow)
{
  context->TimeOut(this,timefromnow);
}


bool Node::Matches(const Node &rhs) const
{
  return number==rhs.number;
}


#if defined(GENERIC)
void Node::LinkUpdate(const Link *l)
{
  cerr << *this << " got a link update: "<<*l<<endl;
  //Do Something generic:
  SendToNeighbors(new RoutingMessage);
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << *this << " got a routing messagee: "<<*m<<" Ignored "<<endl;
}


void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  return 0;
}

Table *Node::GetRoutingTable() const
{
  return new Table;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}

#endif

#if defined(LINKSTATE)

const unsigned Node::maxttl=5;

void Node::LinkUpdate(const Link *l)
{
  // Add To table and update graph
  // Now flood to rest of network
  cerr << *this<<": Link Update: "<<*l<<endl;
  RoutingMessage *m = new RoutingMessage(*this,*l,maxttl,seqno++);
  SendToNeighbors(m);
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << *this << " Routing Message: "<<*m;
  if (m->srcnode.Matches(*this)) {
    // skip, one of ours
    cerr << " IGNORED"<<endl;
  } else {
    // update our table
    if (m->ttl==0) { 
      cerr << " DROPPED, TTL=0"<<endl;
    } else {
      // Forward to neighbors
      RoutingMessage *out=new RoutingMessage(*m);
      out->ttl--;
      cerr << " FORWARDED"<<endl;
      SendToNeighbors(out);
    }
  }
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  // WRITE
  return 0;
}

Table *Node::GetRoutingTable() const
{
  // WRITE
  return 0;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}
#endif


#if defined(DISTANCEVECTOR)

void Node::LinkUpdate(const Link *l)
{
  //first update the table entry for the affected neighbor
  Row* neighbor = table.GetNext(l->GetDest());

  
  cerr << *this << ": Linkupdate " << *l << endl;

  cerr << "Neighbor: " << neighbor << endl;

  if (neighbor == NULL){

    const Row new_neighbor(l->GetDest(), l->GetDest(), l->GetLatency());
    table.SetNext(l->GetDest(), new_neighbor);
    cerr << "neighbor is equal to null" << endl;
    //send change to neighbors
    Node dest_node = Node(l->GetDest(), context, 0, 0);
    cerr << "is it after dest_node?" << endl;
    SendToNeighbors(new RoutingMessage(*this, dest_node, l->GetLatency()));
    cerr << "is it after sendToNeighbors" << endl;

    /* LINK UPDATE COMPLETE. NOW UPDATE TABLE */
    UpdateTable();
    cerr << "end of neihgbor null" << endl;
  }
  else {
    if((neighbor->cost > l->GetLatency() && neighbor!=NULL) //update if new latency is less
        || (neighbor->dest_node == neighbor->next_node)) //update if new latency is greater but prev was a direct link
      { //update if link is new
        //WHAT ABOUT ZOMBIE ENTRIES OR DELETIONS?
    
  //if((neighbor->next_node == l->GetDest() || neighbor->cost < 1 || neighbor->cost < l->GetLatency()){

      const Row new_neighbor(l->GetDest(), l->GetDest(), l->GetLatency());
      table.SetNext(neighbor->dest_node, new_neighbor);
      cerr << "neighbor is theoretically not null" << endl;
      //send change to neighbors
      Node dest_node = Node(neighbor->dest_node, context, 0, 0);
      SendToNeighbors(new RoutingMessage(*this, dest_node, l->GetLatency()));


      /* LINK UPDATE COMPLETE. NOW UPDATE TABLE */
      UpdateTable();

    }//if update
    
  }
    //update if the cost changes, 

    // update our table
      //if 

  // send out routing mesages to neighbors


  //update algorithm:
  // loop through every y in N
  //    iterate through deque<Row> starting at m.begin() to m.end()

  cerr << *this<<": Link Update: "<<*l<<endl;
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << "Node "<<GetNumber()<<": "<<m->srcnode.GetNumber()<<" has new cost "<<m->cost
       <<" path to "<<m->dest.GetNumber()<<" Action: ";

  if (m->dest.GetNumber()==GetNumber()) { 
    cerr << " ourself - ignored\n";
    return;
  }

  //check if new path + cost to that neighbor is better than current path
  //check if new path would help you get somewhere faster

  //grab row to destination
  unsigned dest_node = m->dest.GetNumber();
  Row* dest_row = table.GetNext(dest_node);

  //grab row to source
  unsigned src_node = m->srcnode.GetNumber();
  Row* src_row = table.GetNext(src_node);

  if(src_row == NULL) {
    cerr << "src is null" << endl;
  }
  if(dest_row == NULL) {
    cerr << "dest is null" << endl;
  }

  if(dest_row == NULL){
    if(dest_node != number){
      const Row new_row(dest_node, src_node, src_row->cost + m->cost);
      table.SetNext(dest_node, new_row);

      //send change to neighbors
      Node update_node = Node(dest_node, context, 0, 0);
      SendToNeighbors(new RoutingMessage(*this, update_node, src_row->cost + m->cost));
    }
    else{ //if dest is ourselves...basically a link update
      const Row new_row(src_node, src_node, m->cost);
      table.SetNext(src_node, new_row);

      //send change to neighbors
      Node update_node = Node(src_node, context, 0, 0);
      SendToNeighbors(new RoutingMessage(*this, update_node, m->cost));
    }
    UpdateTable();
  } else {
    if((dest_row->cost > src_row->cost + m->cost) //update if new path plus cost to neighbor better
        || (dest_node == number)){ //update if destination prev not accessible
        //WHAT ABOUT ZOMBIE ENTRIES OR DELETIONS? MAKE SURE NO SEGFAULT!
      if(dest_node != number){
        const Row new_row(dest_node, src_node, src_row->cost + m->cost);
        table.SetNext(dest_node, new_row);

        //send change to neighbors
        Node update_node = Node(dest_node, context, 0, 0);
        SendToNeighbors(new RoutingMessage(*this, update_node, src_row->cost + m->cost));
      }
      else{ //if dest is ourselves...basically a link update
        const Row new_row(src_node, src_node, m->cost);
        table.SetNext(src_node, new_row);

        //send change to neighbors
        Node update_node = Node(src_node, context, 0, 0);
        SendToNeighbors(new RoutingMessage(*this, update_node, m->cost));
      }
      UpdateTable();
    }
  } 
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}


Node *Node::GetNextHop(const Node *destination) const
{
  //get the row that has the destination you want
  Row* dest_row = GetRoutingTable()->GetNext(destination->GetNumber());

  //return new node, everything but number in constructor is junk
  return new Node(dest_row->next_node, NULL, 0, 0);
}

Table *Node::GetRoutingTable() const
{
  return new Table(table);
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw;
  os << ", table="<<table<<")";
  return os;
}
#endif


void Node::UpdateTable(){
    //grab the table's rows
    cerr << "inside update table" << endl;
    deque<Row> rows = table.GetRows();
    //grab neighbors
    cerr << "after getRows" << endl;
    deque<Node*>* neighbors = GetNeighbors();
    cerr << "after getNeighbors" << endl;

    //loop through each destination in table
    for(deque<Row>::iterator d = rows.begin(); d != rows.end(); ++d){
      cerr << "loop iteration" << endl;
      //initialize best cost to initial cost
      double lowest_cost = d->cost;
      unsigned new_next_node = d->next_node;

      //loop through neighbors
      for(deque<Node*>::iterator n = neighbors->begin(); n != neighbors->end(); ++n){
        cerr << "inside neighbor loop" << endl;
        //calculate cost through each neighbor
        //first get cost to neighbor
        double cost_to_neighbor = table.GetNext((*n)->GetNumber())->cost;
        
        if(cost_to_neighbor == NULL){
          cerr << "cost_to_neighbor is NULL :(" << endl;
        }
        //now get cost to destination d
        //get the table
        Row* n_row = (*n)->GetRoutingTable()->GetNext(d->dest_node);

        //check if n_row is null, if neighbor doesn't know about destination
        if(n_row != NULL){
          //get the cost to destination
          double cost_to_d = n_row->cost;
          //total cost
          double total_cost = cost_to_neighbor + cost_to_d;

          if(total_cost < lowest_cost){
            lowest_cost = total_cost;
            new_next_node = (*n)->GetNumber();
          }
        }//null check

      }//for neighbors

      //if placeholders changed, update
      if(new_next_node != d->next_node || lowest_cost != d->cost){
        const Row new_update(d->dest_node, new_next_node, lowest_cost);
        table.SetNext(d->dest_node, new_update);

        //send change to neighbors
        Node changed_node = Node(d->dest_node, context, 0, 0);
        SendToNeighbors(new RoutingMessage(*this, changed_node, lowest_cost));
      }

    }//for rows
}