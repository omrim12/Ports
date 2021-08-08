#ifndef PORTS_DATABASE_H_
#define PORTS_DATABASE_H_
#include "unique_linkedlist.h"
using namespace std;
/**************************************/
class ports_terminal;
/**************************************/
class ports_database	{

private :
	struct port_balance	{ // made for implementing get_cargo_balance function.
		string port_name;
		std::vector<std::pair<date,int> > balances;
		port_balance():port_name(""),balances(std::vector<std::pair<date,int> >{})	{}
		~port_balance()	{}
		port_balance(string port):port_name(port),balances(std::vector<std::pair<date,int> >{})	{}
		port_balance& operator=(const port_balance& other)	{
			balances = other.balances;
			port_name = other.port_name;
			return *this;
		}
		port_balance(const port_balance& other):port_name(other.port_name),balances(other.balances)	{}
	};

	std::vector<unique_ptr<linkedlist<port_node> > > travel_time_graph;
	std::vector<unique_ptr<linkedlist<port_node> > > cargo_graph;
	std::vector<port_balance> ports_balances;

	/**helper functions**/
	void put_edge(string vert_name, string adj_name, unsigned int cargo, unsigned int travel_time);
	bool existing_edge(string src_port, string dest_port);
	int existing_port_balance(string port_name) const;
	void put_vertex(string vert_name, bool is_origin);
	bool existing_vertex(string vert_name);

public:
	/**graph exceptions**/
	class no_such_port_exception : public exception	{
		string port_name;
	public :
		no_such_port_exception(string port_name):port_name(port_name)	{}
		~no_such_port_exception(){}
		virtual const char *what() const noexcept	{
			string out = "";
			out += port_name;
			out += " does not exist in the database.\n";
			return out.c_str();
		}
	};

	/**c'tors & d'tors**/
	ports_database();
	~ports_database();
	ports_database(const ports_database&) = delete;
	ports_database(ports_database&& other);

	/**operators**/
	ports_database& operator=(const ports_database&) = delete;
	ports_database& operator=(ports_database&&) = delete;

	/**class functions**/
	void load(const char *filename);
	void print_inbound(string port_name) throw(no_such_port_exception);
	void print_outbound(string port_name) throw(no_such_port_exception);
	int get_cargo_balance(string vert_name, date checkpoint);

	/**class operators**/
	string to_string() const	{ // Assuming that T has operator << well-defined.

		unsigned int i,j;
		string out = "";

		out += "Shipping time list (represented as adjacency list):\n";
		out += "-------------------------------------------------\n";
		for(i = 0; i < travel_time_graph.size(); ++i)	{
			travel_time_graph[i]->reset_cursor();
			for(j = 0; j < travel_time_graph[i]->get_size(); ++j)	{
				out += (*(travel_time_graph[i]->get_cursor())).to_string();
				out += " --> ";
				travel_time_graph[i]->goto_next();
			}
			out += '\n';
		}

		out += "\nCargo data list (represented as adjacency list):\n";
		out += 	"-------------------------------------------\n";
		for(i = 0; i < cargo_graph.size(); ++i)	{
			cargo_graph[i]->reset_cursor();
			for(j = 0; j < cargo_graph[i]->get_size(); ++j)	{
				out += (*(cargo_graph[i]->get_cursor())).to_string();
				out += " --> ";
				cargo_graph[i]->goto_next();
			}
			out += '\n';
		}

		return out;
	}
};
/**************************************/
#endif
