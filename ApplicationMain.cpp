#include "FASTA.h"
#include <iostream>
#include <string>
#include <locale.h> 




using namespace std;
FASTAInterpretor interpretor;

int main() {

	setlocale(LC_ALL, "zh_CN.UTF-8");//use utf8 in Chinese to contact with Concsole with WinAPI
	system("chcp 65001");//print words in utf8 with console
	system("cls");

	ios::sync_with_stdio(false);

	string name;
	cout << "你好Please enter the name of the file (including the suffix)!" << endl;
	cin >> name;
	if (!interpretor.load_file(name)) {
		cout << "Error" << endl;
		system("pause");
		return 0;
	}
	interpretor.interpret();
	interpretor.show(450,350);

	system("pause");
	return 0;
}
