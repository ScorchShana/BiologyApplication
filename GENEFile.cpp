#include "GENEFile.h"



GENEFile::GENEFile() : resolved(false) {
	
}

GENEFile::~GENEFile() {
	if (genefile.good()) genefile.close();
	filename.clear();
}

bool GENEFile::load_file(const std::string& fn) {
	genefile.open(fn, std::ios::in);
	if (genefile.good()) {
		filename = fn;
		return true;
	}
	return false;
}

bool GENEFile::good() const {
	return genefile.good();
}

bool GENEFile::resolve() {
	if (resolved) return true;
	if (filename.empty() || (!good())) return false;
	std::string line;
	std::string idata;

	if (!genefile.eof()) {
		std::getline(genefile, line);
		for (size_t i = 0;i < line.size();i++) {
			if (line[i] == ',') {
				if (i > 0) {
					cellname.push_back(idata);
				}
				idata.clear();
			}
			else {
				idata += line[i];
			}
		}
		cellname.push_back(idata);
	}

	std::vector<double>line_data;
	while (!genefile.eof()) {

		line_data.clear();
		idata.clear();

		std::getline(genefile,line);

		bool started = true;
		for (size_t i = 0;i < line.size();i++) {
			if (line[i] == ',') {
				if (started) {
					started = false;
					genename.push_back(idata);
				}
				else {
					line_data.push_back(std::stod(idata));
				}
				idata.clear();
			}
			else {
				idata += line[i];
			}
		}
		line_data.push_back(std::stod(idata));
		if (line_data.size() != cellname.size()) return false;
		data.push_back(line_data);
	}

	resolved = true;
	return true;

}

bool GENEFile::to_file(const std::string& fn) const {
	std::fstream outfile(fn, std::ios::out);
	if (!outfile.good()) return false;

	outfile << ',';
	for (size_t i = 0;i < cellname.size();i++) {
		if (i + 1 < cellname.size()) outfile << cellname[i] << ',';
		else outfile << cellname[i] << '\n';
	}
	for (size_t i = 0;i < genename.size();i++) {
		outfile << genename[i] << ',';
		for (size_t j = 0;j < cellname.size();j++) {
			if (j + 1 < cellname.size()) outfile << data[i][j] << ',';
			else outfile << data[i][j] << '\n';
		}
	}

	outfile.close();
	return true;
}