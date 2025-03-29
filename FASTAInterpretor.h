#pragma once


#ifndef FASTAINTERPRETOR_HEADER

#include "FASTAFile.h"
#include <vector>
#include <easyx.h>

enum SegmentType{annotation,sequence};
enum DNACode { A, C, G, T, U, G_A, T_C, G_T, A_C, G_C, A_T, G_T_C, G_A_T, A_C_T, G_C_A, A_G_C_T, UNCERTAIN, UNCERTAIN_LENGTH, UNSIGNEDCODE };
//enum start from 0!!!!

class FASTASegment {
public:
	
		FASTASegment();
		FASTASegment(const FASTASegment& s);
		FASTASegment &operator=(const FASTASegment& s);
		void set(const SegmentType& in_type, const std::string &in_content);
		bool is_annotation() const;
		bool is_sequence() const;
		std::string get_content() const;


	private:
		SegmentType segment_type;
		std::string content;
};

class FASTAInterpretor {
	
	public:
		FASTAInterpretor();
		bool load_file(const std::string &filename);
		bool interpret();
		void show(unsigned int width = 800, unsigned int height = 600) const;



	private:
		unsigned long long total_inform[20];
		std::vector<FASTASegment>fasta_segment;
		bool interpreted;
		FASTAFile fastafile;

		bool compile(const FASTASegment &segment);
		DNACode encode(const char &s) const;


};


#endif // !FASTAINTERPRETOR_HEADER
