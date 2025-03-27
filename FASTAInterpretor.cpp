#include "FASTAInterpretor.h"


//FASTASegment
FASTASegment::FASTASegment() : segment_type(annotation),content("") {
	return;
}

FASTASegment::FASTASegment(const FASTASegment& s) {
	segment_type = s.segment_type;
	content = s.content;
}

FASTASegment &FASTASegment::operator=(const FASTASegment& s) {
	segment_type = s.segment_type;
	content = s.content;
	return (*this);
}

void FASTASegment::set(const SegmentType& in_type, const std::string &in_content) {
	segment_type = in_type;
	content = in_content;
}




//FASTAInterpretor
FASTAInterpretor::FASTAInterpretor() : interpreted(false) {
	return;
}

bool FASTAInterpretor::load_file(const std::string& filename) {
	return fastafile.open(filename);
}

bool FASTAInterpretor::interpret() {
	if (interpreted) return false;
	
	std::string wordline;
	FASTASegment fsegment;
	while (!fastafile.eof()) {
		fastafile >> wordline;
		if (wordline[0] == ';' || wordline[0] == '>') {
			fsegment.set(annotation,wordline.substr(1));
		}
		else {
			fsegment.set(sequence, wordline);
		}
		fasta_segment.push_back(fsegment);
	}

	return true;
}