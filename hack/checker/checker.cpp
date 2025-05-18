#include "testlib.h"
using namespace std;

const string output_secret = "1eJ4ArSX1VUyR4oqcU3vVcJ6xoN8jIYs";

int main(int argc, char* argv[]) {
	registerChecker("hack", argc, argv);
	readBothSecrets(output_secret);

	double points = ouf.readDouble();
	
	ouf.readEoln();
	ouf.readLine();
	ouf.readEoln();
	ouf.readEof();
	
	if(points + 0.00001 >= 1) {
		quit(_ok);
	} else if(points - 0.00001 <= 0){
		quit(_wa);
	} else {
		quitp(points);
	}
}
