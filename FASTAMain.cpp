#include "FASTA.h"

#include <iostream>
#include <string>


using namespace std;
FASTAInterpretor interpretor;

int main() {
	ios::sync_with_stdio(false);

	string name;
	cout << "Please enter the name of the file (including the suffix)!" << endl;
	cin >> name;
	interpretor.load_file(name);

	return 0;
}
