#include "Terminal.h"
/*****************************/
ports_terminal::ports_terminal():data(ports_database{}),outfile(""){}
/*****************************/
ports_terminal::~ports_terminal()	{

}
/*****************************/
bool start_with(string s, string sub)	{

	if( s.size() < sub.size() )	{ return false; }

	for(unsigned int i = 0 ; i < sub.size(); ++i)	{
		if( sub[i] != s[i] )	{ return false; }
	}

	return true;
}
/*****************************/
string parse_filename(string line)	{

	unsigned int i = 0;
	string filename = "";

	while( line[i] != ' ' )	{++i;}
	i++;
	while( i < line.size() )	{ filename += line[i]; ++i; }

	return filename;
}
/*****************************/
void ports_terminal::run()	{

	stringstream parser;
	string line;
	string garbage;
	string operand;
	string filename;
	string port_name;
	bool out = false;;

	while( !out )	{

		try	{
			// clear parser for new input
			parser.clear();
			parser.str("");
			cin.clear();

			// get a line from user
			getline(cin,line);
			parser << line;


			/**PARSE GIVEN LINE AND DISTINGUISH DIFFERENT COMMAND FORMATS:**/
			getline(parser,operand,',');
			if( parser.fail() )	{ throw(ports_terminal::invalid_input_exception()); }


			//1. single operand (exit, print)
			else if( operand == "exit" )	{ out = true; continue; }
			else if( operand == "print" )	{
				std::fstream out(outfile,ios::out);
				out << data.to_string();
				out.close();
				continue;
			}

			//2. double operand (load, inbound, outbound)
			else if( start_with(operand,"load") )	{
				filename = parse_filename(operand);
				if( filename == "" )	{ throw(ports_terminal::invalid_input_exception()); }
        try  {data.load(filename.c_str());}
        catch( file_content_exception& e )  { throw file_use_exception(); }
				cout << "Update was successful." << endl;
				continue;
			}

			//*operand might be a port's name, therefore will be saved for this case*
			port_name = operand;
			getline(parser,operand,',');
			if( parser.fail())	{ throw(ports_terminal::invalid_input_exception()); }

			if( operand == "inbound" )	{
				data.print_inbound(port_name);
				continue;
			}

			else if( operand == "outbound" )	{
				data.print_outbound(port_name);
				continue;
			}

			else if( operand != "balance" )	{ throw(ports_terminal::invalid_input_exception()); }

			getline(parser,operand,',');
			if( parser.fail())	{ throw(ports_terminal::invalid_input_exception()); }

			//triple operand command (balance)
			date checkpoint;
			try	{checkpoint = date(operand);}
			catch( exception& e )	{throw ports_terminal::invalid_input_exception();}
			cout << data.get_cargo_balance(port_name,checkpoint) << endl;
		}
		catch( exception& e )	{cerr << e.what() << endl;}
	}

}
/*****************************/
void ports_terminal::initialize(int num_files, char **files)	{

	// if no arguments given for program execution (wrong format)
	if( num_files == 1 )	{ throw std::runtime_error("ERROR: no input file given."); }

	 // if no input flag appears (wrong format)
	if( string(*(files + 1)) != "-i" )  { throw std::runtime_error("ERROR: Usage: <exe_file> -i <input_file1> ... <input_filen> -o (optional) <output_file>");}
	int i;

	// iterate through program arguments and initialize database with input files.
	for(i = 2; i < num_files && string(*(files + i)) != "-o"; i++)	{
		data.load(*(files + i));
	}

	// in case that -o flage was given
	if( i < num_files )	{
		outfile = string(*(files + (++i)));
		std::fstream f(outfile.c_str(),ios::out | ios::app);
		f.close();
	}

	// default defining of output file
	else	{
		outfile = "output.dat";
		std::fstream f(outfile,ios::app | ios::out);
		f.close();
	}
}
/*****************************/
