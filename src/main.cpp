#include <iostream>
using namespace std;

#include "EdgeMap.h"
int main(int argc, char **argv)
{

	EdgeMap process;
	process.open("akiyo_qcif.y4m");
	process.process();
	return 0;

	
}
