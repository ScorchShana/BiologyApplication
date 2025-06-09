#pragma once

#ifndef GENEFILE_HEADER

#define GENEFILE_HEADER

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class GENEFile {
	public:
		GENEFile();
		~GENEFile();

		bool load_file(const std::string& fn);
		bool good()const;
		bool resolve();
		bool to_file(const std::string& fn) const;

		std::string filename;
		std::vector<std::vector<double>>data;
		std::vector<std::string>genename;
		std::vector<std::string>cellname;

	private:
		bool resolved;
		std::fstream genefile;
		
		
};


#endif // !GENEFILE_HEADER
