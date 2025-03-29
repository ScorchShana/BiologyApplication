#pragma once

#ifndef FASTAFILE_HEADER

#include <fstream>
#include <string>
#include <iostream>

class FASTAFile {
	
	public:

		FASTAFile();
		~FASTAFile();

		bool open(const std::string &in_name);
		bool good() const;
		bool eof() const;
		friend FASTAFile& operator>>(FASTAFile& fastafile, std::string& s);

	private:
		std::string filename;
		std::fstream file;

};


#endif // !READFILE_HEADER
