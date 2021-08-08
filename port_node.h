#ifndef PORT_NODE_H_
#define PORT_NODE_H_
#include "date.h"
/***********************************/
class port_node {

private:

	string port_name;
	unsigned int cargo;
	unsigned int travel_time;
	std::unique_ptr<port_node> next;
	bool origin;
	bool weight_type; //  cargo = 0, shipping time = 1

public:
	/**c'tors & d'tors**/
	port_node();
	port_node(string name, port_node * next ,unsigned int delivered_containers, unsigned int travel_time, bool origin, bool type);
	port_node(const port_node& other) = delete;
	port_node(port_node&& other) = delete;
	~port_node();

	/**class operators**/
	port_node& operator=(const port_node& other) = delete;
	port_node& operator=(port_node&& other) = delete;

	/**class functions**/
	std::unique_ptr<port_node>& get_next();
	void set_next(port_node *new_next);
	string get_name() const;
	unsigned int get_cargo() const;
	unsigned int get_travel_time() const;
	void set_travel_time(unsigned int new_time);

	string to_string() const	{

		string out = "";
		stringstream convert;

		if( origin )	{
			out += "(port name (head): ";
			out += port_name;
			out += ")";
			return out;
		}

		// if weight type is shipping time
		if( !weight_type )	{
			out += "(port name: ";
			out += port_name;
			out += ", shipping time (minutes): ";
			convert << travel_time;
			out += convert.str();
			out += ")";
			return out;
		}

		// if weight type is cargo
		out += "(port name: ";
		out += port_name;
		out += ", cargo (containers): ";
		convert << cargo;
		out += convert.str();
		out += ")";
		return out;

	}
};
/***********************************/
#endif
