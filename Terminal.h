#ifndef TERMINAL_H_
#define TERMINAL_H_
#include "ports_database.h"
/*******************************************/
class ports_terminal	{

	ports_database data;
	string outfile;

public:
	/**exceptions**/
	class invalid_input_exception : public exception	{
	public : virtual const char *what() const noexcept	{ return "USAGE:\t‘load’,<file> *or*\n\t<node>,'inbound' *or*\n\t<node>,’outbound’ *or*\n\t<node>,’balance’,dd/mm HH:mm *or*\n\t‘print’ *or*\n\t‘exit’ *to terminate*";}
	};
	class file_use_exception: public exception	{
	public :
		virtual const char *what() const noexcept	{ return "ERROR opening/reading the specified file.";}
	};
	class file_content_exception : public exception	{
		int line;
	public:
		file_content_exception(int line):line(line)	{}
		~file_content_exception()	{}
		virtual const char *what() const noexcept	{
			string msg = "";
			stringstream convert;
			convert << line;
			msg += "Invalid input at line ";
			msg += convert.str();
			return msg.c_str();
		}
	};

	/**c'tors & d'tors**/
	ports_terminal();
	~ports_terminal();

	/**class functions**/
	void run(); // project's main program
	void initialize(int num_files, char **files); // database initializer.
};
/*******************************************/
#endif
