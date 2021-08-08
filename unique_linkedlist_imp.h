#ifndef UNIQUE_LINKEDLIST_IMP_H_
#define UNIQUE_LINKEDLIST_IMP_H_
/*****************************/
template<class T>
linkedlist<T>::linkedlist():head(nullptr),tail(head),cursor(tail),_size(0)	{}
/*****************************/
template<class T>
linkedlist<T>::~linkedlist()	{ delete head; } // assuming all nodes are chained so that first node's deletion will cause a recursive destruction.
/*****************************/
template<class T>
unsigned int linkedlist<T>::get_size() const	{ return _size; }
/*****************************/
template<class T>
void linkedlist<T>::push(T * new_node)	{

	// although shown as compilation error, it compiles perfectly.
	if( _size == 0 )	{
		head = std::move(new_node);
		tail = std::move(head);
		cursor = std::move(head);
		++_size;
		return;
	}

	tail->set_next(new_node);
	tail = std::move(new_node);
	++_size;
}
/*****************************/
template<class T>
void linkedlist<T>::insert(T * new_node)	{

	if( _size == 0 )	{ push(new_node); return; }

	// insert given node in current cursor position ( cursor --> new_node --> old_next )
	T * old_next = cursor->get_next().get();
	cursor->set_next(new_node);
	new_node->set_next(old_next);

	++_size;
}
/*****************************/
template<class T>
T *linkedlist<T>::get_cursor() const	{return cursor;}
/*****************************/
template<class T>
void linkedlist<T>::goto_next()	{ cursor = cursor->get_next().get(); }
/*****************************/
template<class T>
void linkedlist<T>::reset_cursor()	{ cursor = head; }
/*****************************/
#endif
