#pragma once

#ifndef FASTAALGORITHM_HEADER
#define FASTAALGORITHM_HEADER

#include "FASTAInterpretor.h"
#include <algorithm>
#include <windows.h>
//#include <omp.h>
#include <cstdio>

const unsigned long long max_matrix = 5e8;

class FASTAAlgorithm {
	public:

		long long nw_score;
		long long sw_score;

		FASTAAlgorithm();
		~FASTAAlgorithm();

		void set_nw_score_mat();
		void set_nw_d();
		inline long long nw_base_score(const char& a, const char& b) const;
		//bool nw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file = false, const std::string &out_filename1 = "", const std::string &out_filename2 = "");
		bool better_nw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file = false, const std::string& out_filename1 = "", const std::string& out_filename2 = "");
		//bool omp_nw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file = false, const std::string& out_filename1 = "", const std::string& out_filename2 = "");
		bool large_nw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file = false, const std::string& out_filename1 = "", const std::string& out_filename2 = "");
		bool fixed_bases_write_outfile(const FASTAInterpretor& interpretor1, const std::string& fixed_bases1, const std::string out_filename1) const;

		void set_sw_score_mat();
		void set_sw_d();
		inline long long sw_base_score(const char& a, const char& b)const;
		inline int base_hash(const char &s) const;
		//bool sw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file = false, const std::string& out_filename1 = "");
		bool better_sw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file = false, const std::string& out_filename1 = "");
		bool large_sw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file = false, const std::string& out_filename1 = "");
		//bool omp_sw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file = false, const std::string& out_filename1 = "");

	private:
		//long long nw_score_mat[4 * 4];//column A G C T
		long long nw_d;
		long long sw_score_mat[8 * 8];
		long long nw_score_mat[8 * 8];
		long long sw_d;
};



#endif // !FASTAALGORITHM_HEADER
