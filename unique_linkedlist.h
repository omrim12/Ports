#ifndef UNIQUE_LINKEDLIST_H_
#define UNIQUE_LINKEDLIST_H_
#include "port_node.h"
/**************************************/
template<class T>
class linkedlist	{ // port nodes copying is forbidden, therefore linkedlists copying methods are deleted.

	T *head;
	T *tail;
	T *cursor;
	unsigned int _size;

public:
	/**c'tors & d'tors**/
	linkedlist();
	~linkedlist();
	linkedlist(const linkedlist<T>& other) = delete;
	linkedlist(linkedlist<T>&& other) = delete;

	/**operators**/
	linkedlist<T>& operator=(const linkedlist<T>& other) = delete;
	linkedlist<T>& operator=(linkedlist<T>&& other) = delete;

	/**interface**/
	T *get_cursor() const;
	void goto_next();
	void reset_cursor();
	void push(T *new_node);
	void insert(T *new_node);
	unsigned int get_size() const;
};
/**************************************/
#include "unique_linkedlist_imp.h"
#endif
