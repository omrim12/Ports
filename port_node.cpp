#include "port_node.h"
/**************************/
port_node::port_node():port_name(""),cargo(0),travel_time(0),origin(false), weight_type(0)	{}
/**************************/
port_node::port_node(string name, port_node * next, unsigned int cont,unsigned int travel_t, bool is_origin, bool type):
					port_name(name),cargo(cont),travel_time(travel_t), next(next),origin(is_origin), weight_type(type) {}
/**************************/
port_node::~port_node()	{ delete next.release(); } // this call recursively deletes all nodes chained in a linkedlist.
/**************************/
std::unique_ptr<port_node>& port_node::get_next()	{ return next; }
/**************************/
void port_node::set_next(port_node * new_next)	{
	next.release(); // this call doesn't delete raw pointer on purpose.
	next.reset(new_next);
}
/**************************/
string port_node::get_name() const	{return port_name;}
/**************************/
unsigned int port_node::get_cargo() const	{return cargo;}
/**************************/
unsigned int port_node::get_travel_time() const	{return travel_time;}
/**************************/
void port_node::set_travel_time(unsigned int new_time)	{ travel_time = new_time; }
/**************************/
