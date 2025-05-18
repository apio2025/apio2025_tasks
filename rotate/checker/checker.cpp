#include "testlib.h"
using namespace std;

const string output_secret = "Kk1WryViMzuiNVd5LmhgkRlqQl5ntK4r";

int main(int argc, char* argv[]) {
	registerChecker("rotate", argc, argv);
	readBothSecrets(output_secret);
	readBothGraderResults();
	compareRemainingLines(3);
}
