/////////////////////////////////////////////////////////////////////////////////////////////////
///      THIS FILE CONTAINS THE MAIN FUNCTION, WHICH IS DESIGNATED START OF THE PROGRAM       /// 
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "examples/BVHExample.h"

using namespace std;

int main(int argc, char **argv) {
	
	BVHExample window = BVHExample();

	window.show(argc, argv);

	glutMainLoop();
	return 0;
}