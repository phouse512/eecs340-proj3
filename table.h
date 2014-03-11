#ifndef _table
#define _table


#include <iostream>

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)

#include <deque>

struct Row {
  unsigned dest_node;
  unsigned next_node;
  double cost;
  ostream &Print(ostream &os) const;
  
  Row(unsigned dest, unsigned next, double cost);
};

inline ostream & operator<<(ostream &os, const Row &r) { return r.Print(os);}

class Table {
  deque<Row> m;
 public:
  deque<Row>::iterator FindMatching(const unsigned dest);
  Row *GetNext(const unsigned dest);
  void SetNext(const unsigned dest, const Row &r);
  ostream & Print(ostream &os) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
