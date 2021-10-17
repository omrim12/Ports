Student name: Omri bar-haim

Few important details of project implementing decisions:
----------------------------------------------

1. Port node objects copy is forbidden intentionally.
   one of data members is a unique pointer to next node, therefore 
   the copy assignment and copy constructor methods of a port_node, 
   linkedlist and ports_database are defined as deleted (database contains graphs
   implemented as an adjacency list which is a vector of linkedlists which contain port nodes).

2. Linkedlist traverse decided to be implemented using raw pointers for defending all port nodes
   data member 'next' explicity (which is a unique pointer).

3. Query functions in ports database are defined as non-const because of adjacency list's 
   linkedlists cursor data member, which it's address changes every single traverse.

4. The classes ports_database and ports_terminal construct single-tones, 
   therefore i've decided not to implement all big-five (the decision for ports_database 
   was made because of explicity issues as well).
