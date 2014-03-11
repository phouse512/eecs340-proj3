#ifndef _messages
#define _messages

#include <iostream>

#include "node.h"
#include "link.h"

#if defined(GENERIC)
struct RoutingMessage {
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(LINKSTATE)
struct RoutingMessage {
  Node srcnode;
  Link link;
  unsigned ttl;
  unsigned seqno;

  RoutingMessage();
  RoutingMessage(const Node &src, const Link &l, const unsigned ttl, const unsigned seqno);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)
struct RoutingMessage {
  Node srcnode;
  Node dest;
  double cost;

  RoutingMessage();
  RoutingMessage(const Node &src, const Node &dest, const double cost);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  ostream & Print(ostream &os) const;
};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
