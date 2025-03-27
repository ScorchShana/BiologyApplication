#pragma once


#ifndef FASTAINTERPRETOR_HEADER

#include "FASTAFile.h"
#include <vector>

enum SegmentType{annotation,sequence};

class FASTASegment {
public:
	
		FASTASegment();
		FASTASegment(const FASTASegment& s);
		FASTASegment &operator=(const FASTASegment& s);
		void set(const SegmentType& in_type, const std::string &in_content);

	private:
		SegmentType segment_type;
		std::string content;
};

class FASTAInterpretor {
	
	public:
		FASTAInterpretor();
		bool load_file(const std::string &filename);
		bool interpret();



	private:
		std::vector<FASTASegment>fasta_segment;
		bool interpreted;
		FASTAFile fastafile;


};


#endif // !FASTAINTERPRETOR_HEADER
