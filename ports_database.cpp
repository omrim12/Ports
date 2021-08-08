#ifndef PORTS_DATABASE_CPP_
#define PORTS_DATABASE_CPP_
#include "ports_database.h"
#include "Terminal.h"
/**************************/
ports_database::ports_database():travel_time_graph(std::vector<unique_ptr<linkedlist<port_node> > >()),
								cargo_graph(std::vector<unique_ptr<linkedlist<port_node> > > ()),
								ports_balances(std::vector<port_balance>())	{}
/**************************/
ports_database::~ports_database()	{
	for( auto& cargo_list_ptr : cargo_graph )	{
		delete cargo_list_ptr.release();
	}
	for( auto& time_list_ptr : travel_time_graph )	{
		delete time_list_ptr.release();
	}
}
/**************************/
ports_database::ports_database(ports_database&& other):travel_time_graph(0),cargo_graph(0),ports_balances(0)	{} // this function intended only
																												   // for initializing database.
/**************************/
void ports_database::load(const char *filename)	{

	stringstream ss; // for parsing lines of input file
	stringstream convert; // to convert integers to string and the opposite way

	string line_buffer;
	string data_buffer; // all parsed elements will be saved here

	string out_port_name;
	string in_port_name;

	date arrival_time;
	date shipping_time;
	date next_shipping_time;

	int index;
	int time_differ;
	unsigned int cargo;
	unsigned int total_cargo = 0;
	bool file_start = true;
	bool first_edge = true;
	bool first_balance = true;
	bool invalid_input = false;
	unsigned int line = 1;

	// open file for receiving data
	std::fstream file(filename, ios::in);
	if( !file )	{throw ports_terminal::file_content_exception(0); }

	// reading data line by line
	while( getline(file,line_buffer) )	{

		out_port_name = in_port_name;
		if( !first_edge )	{ shipping_time = next_shipping_time; }

		ss.clear();
		convert.clear();
		ss.str(line_buffer);

		// get port name
		getline(ss, data_buffer, ',');
		if( ss.fail() )	{ invalid_input = true; }
		in_port_name = data_buffer;

		// first line requires different parsing because of different template.
		if( file_start )	{
			getline(ss, data_buffer);
			if( ss.fail() )	{ invalid_input = true; }
			try	{shipping_time = date(data_buffer);}
			catch( exception& e )	{ invalid_input = true; }
			put_vertex(in_port_name,true);
			file_start = false;
			++line;
			continue;
		}

		// get arrival time
		getline(ss, data_buffer, ',');
		if( ss.fail() )	{ invalid_input = true; }
		try	{ arrival_time = date(data_buffer); }
		catch( exception& e )	{ invalid_input = true; }

		// update time difference
		time_differ = arrival_time.time_differ(shipping_time);
		if( time_differ < 0 ) 	{ invalid_input = true; }

		// get cargo
		getline(ss, data_buffer, ',');
		if( ss.fail() )	{ invalid_input = true; }
		convert.str(data_buffer);
		convert >> cargo;
		total_cargo += cargo;
		if( ss.fail() )	{ invalid_input = true; }

		// get shipping time
		getline(ss, data_buffer,',');
		if( ss.fail() )	{ invalid_input = true; }
		try	{ next_shipping_time = date(data_buffer); }
		catch( exception& e )	{ invalid_input = true; }

		// in case of invalid input detected during line parsing process
		if( invalid_input )	{
			cerr << "Invalid input in file " << filename << " in line " << line << endl;
			exit(-1);
		}

		// insert parsed edge data to database.
		put_edge(out_port_name,in_port_name,cargo,time_differ);
		first_edge = false;
		++line;
	}

	// opening file once again for updating ports balances at given arrival/shipping time.
	// (assuming all input is valid at this point).
	file.close();
	file.open(filename,ios::in);
	file_start = true;

	while( getline(file,line_buffer) )	{

		ss.clear();
		convert.clear();
		ss.str(line_buffer);

		if( !first_balance )	{ shipping_time = next_shipping_time; }

		getline(ss,data_buffer,',');

		if( file_start )	{
			out_port_name = data_buffer;
			getline(ss,data_buffer,',');
			shipping_time = date(data_buffer);
			file_start = false;
			continue;
		}

		in_port_name = data_buffer;

		getline(ss,data_buffer,',');
		arrival_time = date(data_buffer);

		getline(ss,data_buffer,',');
		convert.str(data_buffer);
		convert >> cargo;

		getline(ss,data_buffer,',');
		next_shipping_time = date(data_buffer);

		// insert new ports to balances vector if needed and update balance checkpoints for each port.
		index = existing_port_balance(out_port_name);
		if( index < 0 )	{
			ports_balances.push_back(port_balance(out_port_name));
			index = existing_port_balance(out_port_name);
		}
		// origin port sends a ship with total cargo at shipping start.
		if( first_balance )	{ports_balances[index].balances.push_back(std::pair<date,int>(shipping_time,int(-1 * total_cargo)));}
		else	{ports_balances[index].balances.push_back(std::pair<date,int>(shipping_time,int(-1 * cargo)));}

		index = existing_port_balance(in_port_name);
		if( index < 0 )	{
			ports_balances.push_back(port_balance(in_port_name));
			index = existing_port_balance(in_port_name);
		}
		ports_balances[index].balances.push_back(std::pair<date,int>(arrival_time,cargo));

		first_balance = false;
		out_port_name = in_port_name;
	}
}
/**************************/
void ports_database::print_inbound(string port_name) throw(no_such_port_exception)	{

	// cannot outbound from a non existing port (implemented as a vertex).
	if( !existing_vertex(port_name) )	{throw( no_such_port_exception(port_name) );}

	unsigned int i;
	unsigned int saved_travel_time;
	bool inbound_exist = false;

	// iterate through travel time adjacency list to find a linked list which its head node
	// is named after port name. the continuous chained nodes are ones that can be reached
	// within a single edge movement.
	for(i = 0; i < travel_time_graph.size(); i++)	{
		travel_time_graph[i]->reset_cursor();
		travel_time_graph[i]->goto_next();
		while( travel_time_graph[i]->get_cursor() != nullptr )	{
			if( travel_time_graph[i]->get_cursor()->get_name() == port_name )	{
				inbound_exist = true;
				saved_travel_time = travel_time_graph[i]->get_cursor()->get_travel_time();
				travel_time_graph[i]->reset_cursor();
				cout << travel_time_graph[i]->get_cursor()->get_name() << "," << saved_travel_time << endl;
				break;
			}
			travel_time_graph[i]->goto_next();
		}
	}

	if( !inbound_exist )	{ cout << port_name << ": no inbound ports" << endl; }

}
/**************************/
void ports_database::print_outbound(string port_name) throw(no_such_port_exception)	{

	if( !existing_vertex(port_name) )	{throw( no_such_port_exception(port_name) );}
	unsigned int i;

	// iterate through travel_time adjacency list and print out all linked lists heads that are chained with a port node named after port_name.
	// (which means there is a directed edge from current port to a port named after port_name).
	for(i = 0; i < travel_time_graph.size(); i++)	{
		travel_time_graph[i]->reset_cursor();
		if( travel_time_graph[i]->get_cursor()->get_name() == port_name )	{
			if( travel_time_graph[i]->get_size() == 1 )	{ cout << port_name << ": no outbound ports" << endl; return; }
			travel_time_graph[i]->goto_next();
			while( travel_time_graph[i]->get_cursor() != nullptr )	{
				cout << travel_time_graph[i]->get_cursor()->get_name() << "," << travel_time_graph[i]->get_cursor()->get_travel_time() << endl;
				travel_time_graph[i]->goto_next();
			}
			return;
		}
	}
}
/**************************/
void ports_database::put_edge(string vert_name, string adj_name, unsigned int cargo, unsigned int travel_time)	{

	// initialize vertices of the edge if needed.
	if( !existing_vertex(vert_name) )	{ put_vertex(vert_name,true); }
	if( !existing_vertex(adj_name) )	{ put_vertex(adj_name,true); }

	unsigned int i;
	bool exist = existing_edge(vert_name,adj_name); // to indicate if a specific calculation on an existing edge
													// should be preformed

	for(i = 0; i < cargo_graph.size(); ++i)	{
		// reset cursors of both lists to start of structure.
		cargo_graph[i]->reset_cursor();
		travel_time_graph[i]->reset_cursor();

		if( cargo_graph[i]->get_cursor()->get_name() == vert_name )	{
			// if there is already an edge (vert_name,adj_name) , update the edge weight of time to be the average of new one and existing.
			if( exist )	{
				travel_time_graph[i]->goto_next();
				while( travel_time_graph[i]->get_cursor()->get_name() != adj_name )	{cargo_graph[i]->goto_next();}
				travel_time_graph[i]->get_cursor()->set_travel_time( (travel_time_graph[i]->get_cursor()->get_travel_time() + travel_time ) / 2 );
				return;
			}

			// insert new edges to both graphs based on the given parameters.
			cargo_graph[i]->insert(new port_node(adj_name,nullptr,cargo,0,false,true));
			travel_time_graph[i]->insert(new port_node(adj_name,nullptr,0,travel_time,false,false));
			return;
		}
	}
}
/**************************/
void ports_database::put_vertex(string vert_name, bool is_origin)	{

	// initialize linked lists for new vertex (one for each graph)
	linkedlist<port_node> *new_cargo_ll = new linkedlist<port_node>();
	linkedlist<port_node> *new_time_ll = new linkedlist<port_node>();

	// insert new vertex to be the head of both linked lists
	new_cargo_ll->insert(new port_node(vert_name,nullptr,0,0,is_origin,true));
	new_time_ll->insert(new port_node(vert_name,nullptr,0,0,is_origin,false));

	// insert linked lists to adjacency lists using
	// move semantics for unique_ptr ownership moving properly (cargo_ll_ptr and time_ll_ptr
	// will be destroyed at function close).
	std::unique_ptr<linkedlist<port_node> > cargo_ll_ptr(new_cargo_ll);
	std::unique_ptr<linkedlist<port_node> > time_ll_ptr(new_time_ll);

	// although shown as compilation error, it compiles perfectly.
	cargo_graph.push_back(std::move(cargo_ll_ptr));
	travel_time_graph.push_back(std::move(time_ll_ptr));
}
/**************************/
int ports_database::existing_port_balance(string port_name) const	{

	for(unsigned int i = 0; i < ports_balances.size(); ++i)	{
		if( ports_balances[i].port_name == port_name )	{return i;}
	}

	return -1;
}
/**************************/
bool ports_database::existing_edge(string src_port, string dest_port)	{

	unsigned int i;

	//iterate through one of both graphs to find wished edge (both graphs are holding same structure except the weights theys represent).
	for(i = 0; i < cargo_graph.size(); ++i)	{
		cargo_graph[i]->reset_cursor();
		if( cargo_graph[i]->get_cursor()->get_name() == src_port )	{
			cargo_graph[i]->goto_next();
			while( cargo_graph[i]->get_cursor() != nullptr && cargo_graph[i]->get_cursor()->get_name() != dest_port )	{cargo_graph[i]->goto_next();}
			if( cargo_graph[i]->get_cursor() != nullptr )	{return true;}
		}
	}

	return false;
}
/**************************/
bool ports_database::existing_vertex(string vert_name)	{

	unsigned int i;

	// iterate through one of graphs to seek for a port node named after vert_name (both graphs are holding same structure except the weights theys represent).
	for( i = 0; i < cargo_graph.size(); ++i)	{
		if( cargo_graph[i]->get_size() == 0 )	{ continue; }
		cargo_graph[i]->reset_cursor();
		if( cargo_graph[i]->get_cursor()->get_name() == vert_name )	{return true;}
	}

	return false;
}
/**************************/
int ports_database::get_cargo_balance(string vert_name, date checkpoint)	{

	// cannot calculate cargo balance for a non existing vertex.
	if( !existing_vertex(vert_name) )	{ throw no_such_port_exception(vert_name); }

	int i = existing_port_balance(vert_name);
	int balance = 0;

	// sort all checkpoints of a given port by dates
	std::sort(ports_balances[i].balances.begin(),ports_balances[i].balances.end(),
			[](std::pair<date,int> pair1, std::pair<date,int> pair2)->bool{ return pair1.first < pair2.first; });

	// iterate through checkpoints map and stop after reaching the wished date to
	// get balance.
	// while iterating, sum all balances at all checkpoints so far.
	for(auto it1 = ports_balances[i].balances.begin(); it1 != ports_balances[i].balances.end(); ++it1)	{
		if( (*it1).first <= checkpoint )	{balance += (*it1).second; continue;}
		break;
	}

	return balance;
}
/**************************/
#endif
