#pragma once


#ifndef FASTAINTERPRETOR_HEADER
#define FASTAINTERPRETOR_HEADER


#include "FASTAFile.h"
#include <vector>
#include <unordered_map>


enum SegmentType{annotation,sequence};
const int A = 0, C = 1, G = 2, T = 3, U = 4, G_A = 5, T_C = 6, G_T = 7, A_C = 8, G_C = 9, A_T = 10, G_T_C = 11, G_A_T = 12, A_C_T = 13, G_C_A = 14, A_G_C_T = 15, 
		UNCERTAIN = 16, UNCERTAIN_LENGTH = 17, UNSIGNEDCODE = 18;
//typedef enum DNACode { A, C, G, T, U, G_A, T_C, G_T, A_C, G_C, A_T, G_T_C, G_A_T, A_C_T, G_C_A, A_G_C_T, UNCERTAIN, UNCERTAIN_LENGTH, UNSIGNEDCODE }DNACode;
//enum start from 0!!!!

int encode(const char& s);

class FASTASegment {
	public:
	
		FASTASegment();
		~FASTASegment();
		FASTASegment(const FASTASegment& s);
		FASTASegment &operator=(const FASTASegment& s);
		void set(const SegmentType& in_type, const std::string &in_content);
		bool is_annotation() const;
		bool is_sequence() const;

		friend class FASTAInterpretor;
		friend class FASTAAlgorithm;

	private:
		SegmentType segment_type;
		std::string content;
};

class FASTAInterpretor {
	
	public:
		FASTAInterpretor();
		~FASTAInterpretor();
		bool load_file(const std::string &filename);
		bool interpret();
		void showCMD() const;
		std::string get_bases() const;
		
		friend class FASTAAlgorithm;

		bool interpreted;
		unsigned long long sum;//not include unsigned code



	private:
		unsigned long long total_inform[20];//index 19 for unsigned code
		std::vector<FASTASegment>fasta_segment;
		FASTAFile fastafile;

		bool compile(const FASTASegment &segment);

};


#endif // !FASTAINTERPRETOR_HEADER
