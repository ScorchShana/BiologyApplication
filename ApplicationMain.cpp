#include "FASTA.h"
#include "GENE.h"
#include <locale.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <regex>
#include <windows.h>

static std::string GBKToUTF8(const char* str_GBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str_GBK, -1, NULL, 0);//count the len of the str from local code (GBK) to utf16
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, str_GBK, -1, wstr, len);//local code (GBK) to utf16
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);//count the len of the str from utf16 to utf8
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);//utf16 to utf8
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

std::wstring UTF8ToUnicode(const char* str_TUF8) {
	int len = MultiByteToWideChar(CP_UTF8, 0, str_TUF8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, str_TUF8, -1, NULL, 0);
	std::wstring s = wstr;
	return s;
}

static std::string UTF8ToGBK(const char* str_UTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, str_UTF8, -1, NULL, 0);//count the len of the str from local code (GBK) to utf16
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, str_UTF8, -1, wstr, len);//local code (GBK) to utf16
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);//count the len of the str from utf16 to utf8
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);//utf16 to utf8
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

static bool check_filename(const std::string& filename) {
	static std::regex regExpress("[/:*?\"<>|\]");
	bool bValid = !std::regex_search(filename, regExpress);
	return bValid;
}

static inline bool check_file_exist(const std::string& filename) {
	std::fstream f(filename);
	return f.good();
}

static inline void print_single_help(const std::string& arg, const std::string& des) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
	std::cout << arg;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
	std::cout << " : ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	std::cout << des << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);

}

static inline void print_error(const std::string& error) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
	std::cout << error << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
}

static std::vector<std::string>cmd_args;

std::string outfile1, outfile2;
std::string infile1, infile2;
typedef enum func_type { none, show, nw, sw, z_score, t_test }func_type;
FASTAAlgorithm FA;
GENEAlgorithm GA;
GENEPreprocessing GP;
func_type ftype = none;


int main(int argc, char *args[]) {

	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, "zh_CN.UTF-8");//use utf8 in Chinese to contact with Concsole with WinAPI

	std::ios::sync_with_stdio(false);

	for (int i = 0;i < argc;i++) {
		std::string s = GBKToUTF8(args[i]);
		cmd_args.push_back(s);
		//std::cout << s << std::endl;
	}

	int maintype = 0;
	for (size_t i = 1;i < cmd_args.size();i++) {
		if (cmd_args[i] == "-help") {
			maintype = 1;
			break;
		}
		if (cmd_args[i] == "-gene") {
			maintype = 3;
			break;
		}
		else if (cmd_args[i] == "-fasta") {
			maintype = 2;
			break;
		}
	}
	if (maintype == 0) {
		print_error("Invalid command! Lack of key arugment like \"-help\" , \"-fasta\" or \"-gene\" !");
		return 0;
	}
	else if (maintype == 1) {
		std::cout << "Attention : only utf-8 and ascii encoding are supported !\n";
		std::cout << "Commandline arguments : \n";
		print_single_help("-help   ", "print the help of the commandline argument");
		print_single_help("-fasta  ", "indecate you want to analyze the fasta file , only input this can you using the following argument ( from -nw to -set_mat)");
		print_single_help("-nw     ", "perform Smith-Waterman Algorithm on the two input file with argument \"-i\" and you can choose whether to output 1 files of the best matched squence or not with argument \"-o\"");
		print_single_help("-sw     ", "perform Smith-Waterman Algorithm on the two input file with argument \"-i\" and you can choose whether to output 1 files of the best matched squence or not with argument \"-o\"");
		print_single_help("-show   ", "show the DNA information of a dna sequence file with argument \"-i\"");
		print_single_help("-i      ", "the name(s) of input file(s) should be followed");
		print_single_help("-o      ", "the name(s) of output file(s) should be followed , default name(s) may be out.fasta , out1.fasta or out.fasta");
		print_single_help("-set_d  ", "later set the score of a DNA base gap int the Needleman-Wunsch Algorithm or Smith-Waterman Algorithm following the tips , default is -1");
		print_single_help("-set_mat", "later set the score matrix of the DNA base int the Needleman-Wunsch Algorithm or Smith-Waterman Algorithm following the tips , default is mathed 1 and unmatched -1");
		print_single_help("-gene   ", "indecate you want to analyze the csv file , only input this can you using the following argument");
		print_single_help("-t_test ", "perform t-test between two csv files and two tables of files must be tha same form");
		print_single_help("-a      ", "perform t-test with alpha , alpha could only be one of 0.5 , 0.2 , 0.1 , 0.05 , 0.02 , 0.01 , 0.005 , 0.002 , 0.001 . Recommended : 0.05");
		print_single_help("-z_score", "perform normalization on csv file");
		print_single_help("-i      ", "the name(s) of input file(s) should be followed");
		print_single_help("-o      ", "the name(s) of output file(s) should be followed , default name may be out.csv");
		
		
	}
	else if (maintype == 2) {
		for (size_t i = 1;i < cmd_args.size();i++) {
			if (cmd_args[i] == "-show") {
				ftype = show;
				break;
			}
			else if (cmd_args[i] == "-nw") {
				ftype = nw;
				break;
			}
			else if (cmd_args[i] == "-sw") {
				ftype = sw;
				break;
			}
		}

		if (ftype == none) {
			print_error("Input invalid command !\nIf you need help, please input argument -help !");
			return 0;
		}
		
		else if (ftype == show) {
			for (size_t i = 1;i < cmd_args.size();i++) {
				if (cmd_args[i] == "-i") {
					i++;
					if (i < cmd_args.size()) {
						infile1 = cmd_args[i];
					}
				}
			}

			if (infile1.empty()) {
				print_error("No input file!");
				return 0;
			}
			else {
				if (!check_file_exist(infile1)) {
					print_error("Not founnd file ： " + infile1 + "!");
					return 0;
				}
			}


			//function
			FASTAInterpretor fi;
			if (!fi.load_file(infile1)) {
				print_error("Load file  " + infile1 + " error !");
				return 0;
			}

			if (!fi.interpret()) {
				print_error("Read file  " + infile1 + " error ! Please check whether your file is only included base and annotation !");
				return 0;
			}

			fi.showCMD();

		}
		else if (ftype == nw) {
			bool set_d = false;
			bool set_mat = false;
			bool out_to_file = false;
			for (size_t i = 1;i < cmd_args.size();i++) {
				if (cmd_args[i] == "-i") {
					i++;
					if (i < cmd_args.size()) {
						infile1 = cmd_args[i];
					}
					i++;
					if (i < cmd_args.size()) {
						infile2 = cmd_args[i];
					}
				}
				else if (cmd_args[i] == "-o") {
					out_to_file = true;
					i++;
					if (i < cmd_args.size()) {
						outfile1 = cmd_args[i];
					}
					i++;
					if (i < cmd_args.size()) {
						outfile2 = cmd_args[i];
					}
				}
				else if (cmd_args[i] == "-set_d") {
					set_d = true;
				}
				else if (cmd_args[i] == "-set_mat") {
					set_mat = true;
				}
			}

			if (infile1.empty()) {
				print_error("No input file!");
				return 0;
			}
			else {
				if (!check_file_exist(infile1)) {
					print_error("Not found file " + infile1 + "!");
					return 0;
				}
			}

			if (infile2.empty()) {
				print_error("No input file!");
				return 0;
			}
			else {
				if (!check_file_exist(infile2)) {
					print_error("Not found file " + infile2 + "!");
					return 0;
				}
			}

			if (out_to_file) {
				if (outfile1.empty()) {
					outfile1 = "out1.fasta";
				}
				else {
					if (!check_filename(outfile1)) {
						print_error("Illeagal output file name : " + outfile1);
						return 0;
					}
				}

				if (outfile2.empty()) {
					outfile2 = "out2.fasta";
				}
				else {
					if (!check_filename(outfile2)) {
						print_error("Illeagal output file name : " + outfile2);
						return 0;
					}
				}
			}


			//function

			if (set_d) FA.set_nw_d();
			if (set_mat) FA.set_nw_score_mat();

			FASTAInterpretor fi1, fi2;
			if (!fi1.load_file(infile1)) {
				print_error("Load file  " + infile1 + " error !");
				return 0;
			}

			if (!fi1.interpret()) {
				print_error("Read file  " + infile1 + " error ! Please check whether your file is only included base and annotation !");
				return 0;
			}

			if (!fi2.load_file(infile2)) {
				print_error("Load file  " + infile2 + " error !");
				return 0;
			}

			if (!fi2.interpret()) {
				print_error("Read file  " + infile2 + " error ! Please check whether your file is only included base and annotation !");
				return 0;
			}

			if (fi1.sum * fi2.sum >= max_matrix) {
				print_error("The bases are too many ！Please ensure the product of the bases is fewer than 5e8 !");
				return 0;
			}

			if (!FA.better_nw(fi1, fi2, out_to_file, outfile1, outfile2)) {
				print_error("Runtime error ! Please ensure the DNA Sequence only include A, T, G, C, a, t, g, c ! ");
				return 0;
			}
			std::cout << "Successful perform Needleman-Wunsch Algorithm !" << std::endl;
			std::cout << "The score is : " << FA.nw_score << std::endl;
			if (out_to_file) std::cout << "The output is in " << outfile1 << " and " << outfile2 << " !" << std::endl;

		}
		else if (ftype == sw) {
			bool set_d = false;
			bool set_mat = false;
			bool out_to_file = false;

			for (size_t i = 1;i < cmd_args.size();i++) {
				if (cmd_args[i] == "-i") {
					i++;
					if (i < cmd_args.size()) {
						infile1 = cmd_args[i];
					}
					i++;
					if (i < cmd_args.size()) {
						infile2 = cmd_args[i];
					}
				}
				else if (cmd_args[i] == "-o") {
					out_to_file = true;
					i++;
					if (i < cmd_args.size()) {
						outfile1 = cmd_args[i];
					}
				}
				else if (cmd_args[i] == "-set_d") {
					set_d = true;
				}
				else if (cmd_args[i] == "-set_mat") {
					set_mat = true;
				}
			}

			if (infile1.empty()) {
				print_error("No input file!");
				return 0;
			}
			else {
				if (!check_file_exist(infile1)) {
					print_error("Not find file " + infile1 + "!");
					return 0;
				}
			}

			if (infile2.empty()) {
				print_error("No input file!");
				return 0;
			}
			else {
				if (!check_file_exist(infile2)) {
					print_error("Not find file " + infile2 + "!");
					return 0;
				}
			}

			if (out_to_file) {
				if (outfile1.empty()) {
					outfile1 = "out.fasta";
				}
				else {
					if (!check_filename(outfile1)) {
						print_error("Illeagal output file name : " + outfile1);
						return 0;
					}
				}
			}


			//function
			if (set_d) FA.set_sw_d();
			if (set_mat) FA.set_sw_score_mat();

			FASTAInterpretor fi1, fi2;
			if (!fi1.load_file(infile1)) {
				print_error("Load file  " + infile1 + " error !");
				return 0;
			}

			if (!fi1.interpret()) {
				print_error("Read file  " + infile1 + " error ! Please check whether your file is only included base and annotation !");
				return 0;
			}

			if (!fi2.load_file(infile2)) {
				print_error("Load file  " + infile2 + " error !");
				return 0;
			}

			if (!fi2.interpret()) {
				print_error("Read file  " + infile2 + " error ! Please check whether your file is only included base and annotation !");
				return 0;
			}

			if (fi1.sum * fi2.sum >= max_matrix) {
				print_error("The bases are too many ！Please ensure the product of the bases is fewer than 5e8 !");
				return 0;
			}

			if (!FA.better_sw(fi1, fi2, out_to_file, outfile1)) {
				print_error("Runtime error ! Please ensure the DNA Sequence only include A, T, G, C, a, t, g, c ! ");
				return 0;
			}
			std::cout << "Successful perform Smith-Waterman Algorithm !" << std::endl;
			std::cout << "The score is : " << FA.sw_score << std::endl;
			if (out_to_file) std::cout << "The output is in " << outfile1 << " !" << std::endl;


		}
	}
	else if(maintype == 3){
		for (size_t i = 1;i < cmd_args.size();i++) {
			if (cmd_args[i] == "-t_test") {
				ftype = t_test;
				break;
			}
			else if (cmd_args[i] == "-z_score") {
				ftype = z_score;
				break;
			}
		}

		if (ftype == none) {
			print_error("Input invalid command !\nIf you need help, please input argument -help !");
			return 0;
		}
		
		else if (ftype == z_score) {

			for (size_t i = 1;i < cmd_args.size();i++) {
				if (cmd_args[i] == "-i") {
					i++;
					if (i < cmd_args.size()) {
						infile1 = cmd_args[i];
					}
				}
				else if (cmd_args[i] == "-o") {
					i++;
					if (i < cmd_args.size()) {
						outfile1 = cmd_args[i];
					}
				}
			}

			if (infile1.empty()) {
				print_error("No input file!");
				return 0;
			}
			else {
				if (!check_file_exist(infile1)) {
					print_error("Not find file " + infile1 + "!");
					return 0;
				}
			}

			if (outfile1.empty()) {
				outfile1 = "out.csv";
			}
			else {
				if (!check_filename(outfile1)) {
					print_error("Illeagal output file name : " + outfile1);
					return 0;
				}
			}

			GENEFile gf;
			if (!gf.load_file(infile1)) {
				print_error("Load file  " + infile1 + " error !");
				return 0;
			}
			if (!gf.resolve()) {
				print_error("Read file  " + infile1 + " error ! Please check whether your file is a good csv file !");
				return 0;
			}
			GP.z_score(gf);
			if (!gf.to_file(outfile1)) {
				print_error("Erite file  " + outfile1 + " error ! Please check whether the name of file is legal !");
				return 0;
			}
		}

		else if (ftype == t_test) {
			double a = -1;
			for (size_t i = 1;i < cmd_args.size();i++) {
				if (cmd_args[i] == "-i") {
					i++;
					if (i < cmd_args.size()) {
						infile1 = cmd_args[i];
					}
					i++;
					if (i < cmd_args.size()) {
						infile2 = cmd_args[i];
					}
				}
				if (cmd_args[i] == "-a") {
					i++;
					if (i < cmd_args.size()) {
						a = stod(cmd_args[i]);
					}
				}
			}

			if (infile1.empty()) {
				print_error("No input file!");
				return 0;
			}
			else {
				if (!check_file_exist(infile1)) {
					print_error("Not find file " + infile1 + "!");
					return 0;
				}
			}

			if (infile2.empty()) {
				print_error("No input file!");
				return 0;
			}
			else {
				if (!check_file_exist(infile2)) {
					print_error("Not find file " + infile2 + "!");
					return 0;
				}
			}

			GENEFile gf1,gf2;
			if (!gf1.load_file(infile1)) {
				print_error("Load file  " + infile1 + " error !");
				return 0;
			}
			if (!gf1.resolve()) {
				print_error("Read file  " + infile1 + " error ! Please check whether your file is a good csv file !");
				return 0;
			}
			if (!gf2.load_file(infile2)) {
				print_error("Load file  " + infile2 + " error !");
				return 0;
			}
			if (!gf2.resolve()) {
				print_error("Read file  " + infile2 + " error ! Please check whether your file is a good csv file !");
				return 0;
			}

			int a_level = -1;
			for (int i = 0;i < 9;i++) {
				if (abs(a - alpha[i]) < 1e-6) {
					a_level = i;
					break;
				}
			}

			if (0 <= a_level) {
				std::vector<bool>res;
				std::vector<double>t_res;
				return_type fres = GA.independent_t_test(gf1, gf2, a_level, res, t_res);
				switch (fres) {
				case size_error:
					print_error("Size error ! The size of two table should be the same !");
					return 0;
				case too_few:
					print_error("Algorithm error ! There are too few samples. The samples must be more than 2 !");
					return 0;
				case too_many:
					print_error("Algorithm error ! There are too many samples. The samples must be fewer than 40 !");
					return 0;
				case success:
					GA.show_independent_t_test(gf1, a_level, res, t_res);
				}

			}
			else {
				print_error("Bad alpha of t-test ! Recommended alpha : 0.05 !");
				return 0;
			}
		}
	}

	
	std::cout << "Successfully finished command !" << std::endl;
	system("pause");
	return 0;
}