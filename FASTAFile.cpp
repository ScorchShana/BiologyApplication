#include "FASTAFile.h"

FASTAFile::FASTAFile() : filename("") {
	return;
}




FASTAFile::~FASTAFile() {
	if (file.good()) file.close();
}

bool FASTAFile::open(const std::string &in_name) {
	filename = in_name;
	file.open(filename, std::ios::in);
	return file.good();
}

bool FASTAFile::good() const {
	return file.good();
}

bool FASTAFile::eof() const {
	return file.eof();
}

FASTAFile& operator>>(FASTAFile& fastafile, std::string& s) {
	fastafile.file >> s;
	return fastafile;
}