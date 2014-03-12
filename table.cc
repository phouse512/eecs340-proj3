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

deque<Row>::iterator Table::FindMatching(const unsigned dest) 
{
  // find and return any matching rows with a destination of input dest
}

Row *Table::GetNext(const unsigned dest) 
{
  // return a row that matches the destination 

  // use Table::FindMatching to find the next row that matches the input dest
  // return a new row that has its attributes set by the the deque<Row> returned by FindMatching  
}

void Table::SetNext(const unsigned dest, const Row &r)
{
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
