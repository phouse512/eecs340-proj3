#include "table.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const 
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

#endif

#if defined(DISTANCEVECTOR)

//so we can access the table's rows for updating in node.cc
deque<Row> Table::GetRows(){
  return m;
}

deque<Row>::iterator Table::FindMatching(const unsigned dest) 
{
  // find and return any matching rows with a destination of input dest
  for(deque<Row>::iterator i = m.begin(); i != m.end(); i++){
    if(i->dest_node == dest){
      return i;
    } 
  }
  return m.end();
}

Row *Table::GetNext(const unsigned dest) 
{
  // return a row that matches the destination 
  deque<Row>::iterator i = FindMatching(dest);

  if(i != m.end()){
    return new Row(i->dest_node, i->next_node, i->cost);
  }
  else{
    //return new Row(-1, -1, 0);
    return NULL;
  }

  // use Table::FindMatching to find the next row that matches the input dest
  // return a new row that has its attributes set by the the deque<Row> returned by FindMatching  
}

void Table::SetNext(const unsigned dest, const Row &r)
{
  deque<Row>::iterator i = FindMatching(dest);

  if(i != m.end()){
    i->next_node = r.next_node;
    i->cost = r.cost; 
  }
  else{
    //add row to back of deque
    m.push_back(r);
  }
}

Row::Row(const unsigned dest, const unsigned next, const double c) :
  dest_node(dest), next_node(next), cost(c)
{}

ostream & Row::Print(ostream &os) const 
{
  os <<"Row(dest="<<dest_node<<", next="<<next_node<<", cost="<<cost<<")";
  return os;
}

ostream & Table::Print(ostream &os) const 
{
  os<<"Table(rows={";
  for (deque<Row>::const_iterator i=m.begin();i!=m.end();++i) { 
    os <<(*i)<<", ";
  }
  os<<"})";
  return os;
}
    
#endif
