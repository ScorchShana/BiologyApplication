//#include "FASTA.h"
//#include <iostream>
//#include <string>
//#include <locale.h> 
//
//
//
//
//using namespace std;
//FASTAInterpretor interpretor1,interpretor2;
//FASTAAlgorithm algotirhm;
//
//int main() {
//	SetConsoleOutputCP(CP_UTF8);
//	setlocale(LC_ALL, "zh_CN.UTF-8");//use utf8 in Chinese to contact with Concsole with WinAPI
//	system("chcp 65001");//print words in utf8 with console
//	system("cls");
//
//	ios::sync_with_stdio(false);
//
//	string name1,name2;
//	cout << "你好Please enter the name of the file (including the suffix)!" << endl;
//	name1 = "D:\\Study\\SCUT\\c++实训\\大一下\\BiologyApplication\\BiologyApplication\\BiologyApplication\\sw_test1.fasta";
//	if (!interpretor1.load_file(name1)) {
//		cout << "Error" << endl;
//		system("pause");\
//		return 0;
//	}
//	if (!interpretor1.interpret()) {
//		cout << "Interpret Error" << endl;
//		system("pause");
//		return 0;
//	}
//	/*interpretor1.showGUI();*/
//	/*interpretor1.showCMD();*/
//
//	name2 = "D:\\Study\\SCUT\\c++实训\\大一下\\BiologyApplication\\BiologyApplication\\BiologyApplication\\sw_test2.fasta";
//	if (!interpretor2.load_file(name2)) {
//		cout << "Error" << endl;
//		system("pause");
//		return 0;
//	}
//
//	if (!interpretor2.interpret()) {
//		cout << "Interpret Error" << endl;
//		system("pause");
//		return 0;
//	}
//	//interpretor2.showGUI();
//	//interpretor2.showCMD();
//
//	if (!algotirhm.nw(interpretor1, interpretor2)) {
//		cout << "Algorithm Error" << endl;
//		system("pause");
//		return 0;
//	}
//	cout << "socre:" << algotirhm.nw_score << endl;
//
//	if (!algotirhm.omp_nw(interpretor1, interpretor2)) {
//		cout << "Algorithm Error" << endl;
//		system("pause");
//		return 0;
//	}
//	cout << "socre:" << algotirhm.nw_score << endl;
//
//	if (!algotirhm.better_nw(interpretor1, interpretor2)) {
//		cout << "Algorithm Error" << endl;
//		system("pause");
//		return 0;
//	}
//	cout << "socre:" << algotirhm.nw_score << endl;
//	system("pause");
//
//	return 0;
//}
