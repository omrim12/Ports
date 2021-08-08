#include "Terminal.h"
/*********************************/
// TODO: Implement / fix the followings :
//1. change adjacency lists to vector of unique pointers to linkedlists
//	 and update project to interface.
/*********************************/
int main(int argc, char **argv) {
	try	{
		ports_terminal prog;
		prog.initialize(argc,argv);
		prog.run();
	}
	catch( exception& e )	{cerr << e.what() << endl;}
	return 0;
}
