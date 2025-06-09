#include "FASTAAlgorithm.h"






FASTAAlgorithm::FASTAAlgorithm() : nw_d(-1), nw_score(0) ,sw_d( - 1), sw_score(0){
	
	const char* s = "ATGC";
	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 4;j++) {
			if (i != j) sw_score_mat[base_hash(s[i]) * 8 + base_hash(s[j])] = nw_score_mat[base_hash(s[i]) * 8 + base_hash(s[j])] = -1;
			else sw_score_mat[base_hash(s[i]) * 8 + base_hash(s[j])] = nw_score_mat[base_hash(s[i]) * 8 + base_hash(s[j])] = 1;
		}
	}
	
}

FASTAAlgorithm::~FASTAAlgorithm() {

}


void FASTAAlgorithm::set_nw_score_mat() {
	std::cout << "Please input the score mat like this mat : \n   A  T  G  C\nA  1 -1 -1 -1\nT -1  1 -1 -1\nG -1 -1  1 -1\nC -1 -1 -1  1\nYou dont need to input the letters ! " << std::endl;
	const char* s = "ATGC";
	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 4;j++) std::cin >> nw_score_mat[base_hash(s[i]) * 8 + base_hash(s[j])];
		//for (int j = 0;j < 4;j++) std::cin >> nw_score_mat[i * 4 + j];
	}
}

void FASTAAlgorithm::set_nw_d() {
	std::cout << "Please input the score of a gap : ";
	std::cin >> nw_d;
}

inline int FASTAAlgorithm::base_hash(const char& s) const {
	return s % 8;
}

inline long long FASTAAlgorithm::nw_base_score(const char& a, const char& b) const {
	return nw_score_mat[base_hash(a) * 8 + base_hash(b)];
	
}

//bool FASTAAlgorithm::nw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file, const std::string &out_filename1, const std::string &out_filename2) {
//	if (interpretor1.sum * interpretor2.sum >= max_matrix || interpretor1.sum * interpretor2.sum==0) return false;//too large, cannot calculate
//	std::string bases1 = interpretor1.get_bases();
//	std::string bases2 = interpretor2.get_bases();
//
//	std::string fixed_bases1 = "";
//	std::string fixed_bases2 = "";
//
//	size_t size1 = bases1.size();
//	size_t size2 = bases2.size();
//
//	int bases_sum = 0;
//
//	for (size_t i = 0;i < 4;i++) {
//		bases_sum += interpretor1.total_inform[i];
//	}
//	if (bases_sum != size1) return false;
//
//	bases_sum = 0;
//	for (size_t i = 0;i < 4;i++) {
//		bases_sum += interpretor2.total_inform[i];
//	}
//	if (bases_sum != size2) return false;
//
//	long long** dp = new long long* [size1 + 1];
//	for (size_t i = 0;i <= size1;i++) {
//		dp[i] = new long long[size2 + 1];
//	}
//
//
//	for (int i = 0;i <= size2;i++) {
//		dp[0][i] = nw_d*i;
//	}
//
//	for (int i = 0;i <= size1;i++) {
//		dp[i][0] = nw_d*i;
//	}
//
//	long long current_score;
//
//	//get score dp mat
//	for (size_t bases1_index = 1;bases1_index <= size1;bases1_index++) {
//		for (size_t bases2_index = 1;bases2_index <= size2;bases2_index++) {
//			current_score = dp[bases1_index - 1][bases2_index - 1] + nw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);//directly judge whether is the same
//			current_score = max(dp[bases1_index - 1][bases2_index] + nw_d, current_score);//is disloacted
//			current_score = max(dp[bases1_index][bases2_index - 1] + nw_d, current_score) ;//is disloacted
//			dp[bases1_index][bases2_index] = current_score;
//		}
//	}
//
//
//
//	//search route
//	int reverse_bases1_index = size1;
//	int reverse_bases2_index = size2;
//	current_score = dp[reverse_bases1_index][reverse_bases2_index];
//	while (reverse_bases1_index > 0  && reverse_bases2_index > 0) {
//		if (current_score == dp[reverse_bases1_index - 1][reverse_bases2_index - 1] + nw_base_score(bases1[reverse_bases1_index - 1], bases2[reverse_bases2_index - 1] )) {
//			fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
//			fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
//			reverse_bases1_index--;
//			reverse_bases2_index--;
//		}
//		else if (current_score == dp[reverse_bases1_index - 1][reverse_bases2_index] + nw_d) {//(dp[i][j]=dp[i-1][j]+nw_d) means bases1[i] match nothing in bases2, which is equal to add - in bases2
//			fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
//			fixed_bases2 = '-'  +fixed_bases2;
//			reverse_bases1_index--;
//		}
//		else{//(dp[i][j]=dp[i][j-1]+nw_d) means bases2[j] match nothing in bases1, which is equal to add - in bases1
//			fixed_bases1 = '-' +fixed_bases1;
//			fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
//			reverse_bases2_index--;
//		}
//
//		current_score = dp[reverse_bases1_index][reverse_bases2_index];
//
//	}
//
//	while (reverse_bases1_index > 0) {
//		fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
//		fixed_bases2 = '-' + fixed_bases2;
//		reverse_bases1_index--;
//	}
//
//	while (reverse_bases2_index > 0) {
//		fixed_bases1 = '-' + fixed_bases1;
//		fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
//		reverse_bases2_index--;
//	}
//
//
//	nw_score = dp[size1][size2];
//
//	for (size_t i = 0;i <= size1;i++) {
//		delete[] dp[i];
//	}
//	delete[] dp;
//
//	//out to file
//	if (!out_to_file) return true;
//
//	if (!fixed_bases_write_outfile(interpretor1, fixed_bases1, out_filename1)) return false;
//	if (!fixed_bases_write_outfile(interpretor2, fixed_bases2, out_filename2)) return false;
//	
//	
//	return true;
//}

bool FASTAAlgorithm::better_nw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file, const std::string& out_filename1, const std::string& out_filename2) {
	if (interpretor1.sum * interpretor2.sum == 0) return false;

	if (out_to_file && interpretor1.sum * interpretor2.sum >= max_matrix) {
		return large_nw(interpretor1, interpretor2, true, out_filename1, out_filename2);
	}


	std::string bases1 = interpretor1.get_bases();
	std::string bases2 = interpretor2.get_bases();

	std::string fixed_bases1 = "";
	std::string fixed_bases2 = "";

	size_t size1 = bases1.size();
	size_t size2 = bases2.size();

	
	int bases_sum = 0;

	for (size_t i = 0; i < 4; i++) {
		bases_sum += interpretor1.total_inform[i];
	}
	if (bases_sum != size1) return false;

	bases_sum = 0;
	for (size_t i = 0; i < 4; i++) {
		bases_sum += interpretor2.total_inform[i];
	}
	if (bases_sum != size2) return false;

	if (out_to_file) {
		long long* dp = new long long[(size1 + 1) * (size2 + 1)]();
		long long* pre_row = new long long[size2 + 1]();

		for (int i = 0; i <= size2; i++) {
			dp[0 * (size2 + 1) + i] = nw_d * i;
			pre_row[i] = nw_d * i;
		}

		for (int i = 0; i <= size1; i++) {
			dp[i * (size2 + 1) + 0] = nw_d * i;

		}

		long long current_score;
		long long diagonal = 0;
		long long* dp_ptr = dp + size2 + 1;


		//get score dp mat
		for (size_t bases1_index = 1; bases1_index <= size1; bases1_index++) {
			dp_ptr++;
			diagonal = nw_d * (bases1_index - 1);
			for (size_t bases2_index = 1; bases2_index <= size2; bases2_index++, dp_ptr++) {

				current_score = diagonal + nw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);
				current_score = max(pre_row[bases2_index] + nw_d, current_score);
				current_score = max(*(dp_ptr - 1) + nw_d, current_score);

				*dp_ptr = current_score;

				diagonal = pre_row[bases2_index];
				pre_row[bases2_index] = current_score;
			}
		}

		delete[] pre_row;
		pre_row = nullptr;
		dp_ptr = nullptr;


		//search route
		//search route
		int reverse_bases1_index = size1;
		int reverse_bases2_index = size2;
		current_score = dp[reverse_bases1_index * (size2 + 1) + reverse_bases2_index];
		while (reverse_bases1_index > 0 && reverse_bases2_index > 0) {
			if (current_score == dp[(reverse_bases1_index - 1) * (size2 + 1) + reverse_bases2_index - 1] + nw_base_score(bases1[reverse_bases1_index - 1], bases2[reverse_bases2_index - 1])) {
				fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
				fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
				reverse_bases1_index--;
				reverse_bases2_index--;
			}
			else if (current_score == dp[(reverse_bases1_index - 1) * (size2 + 1) + reverse_bases2_index] + nw_d) {//(dp[i][j]=dp[i-1][j]+nw_d) means bases1[i] match nothing in bases2, which is equal to add - in bases2
				fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
				fixed_bases2 = '-' + fixed_bases2;
				reverse_bases1_index--;
			}
			else {//(dp[i][j]=dp[i][j-1]+nw_d) means bases2[j] match nothing in bases1, which is equal to add - in bases1
				fixed_bases1 = '-' + fixed_bases1;
				fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
				reverse_bases2_index--;
			}

			current_score = dp[reverse_bases1_index * (size2 + 1) + reverse_bases2_index];

		}

		while (reverse_bases1_index > 0) {
			fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
			fixed_bases2 = '-' + fixed_bases2;
			reverse_bases1_index--;
		}

		while (reverse_bases2_index > 0) {
			fixed_bases1 = '-' + fixed_bases1;
			fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
			reverse_bases2_index--;
		}


		nw_score = dp[size1 * (size2 + 1) + size2];


		delete[] dp;

		if (!fixed_bases_write_outfile(interpretor1, fixed_bases1, out_filename1)) return false;
		if (!fixed_bases_write_outfile(interpretor2, fixed_bases2, out_filename2)) return false;

		return true;
	}

	else {
		long long* dp = new long long[2 * (size2 + 1)]();
		int flag = 0;

		long long current_score;
		for (size_t i = 0; i <= size2; i++) dp[(1 - flag) * (size2 + 1) + i] = nw_d * i;
		for (size_t bases1_index = 1; bases1_index <= size1; bases1_index++) {
			dp[flag * (size2 + 1) + 0] = nw_d * bases1_index;
			for (size_t bases2_index = 1; bases2_index <= size2; bases2_index++) {

				current_score = dp[(1 - flag) * (size2 + 1) + bases2_index - 1] + nw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);
				current_score = max(dp[(1 - flag) * (size2 + 1) + bases2_index] + nw_d, current_score);
				current_score = max(dp[flag * (size2 + 1) + bases2_index - 1] + nw_d, current_score);
				
				dp[flag * (size2 + 1) + bases2_index] = current_score;
			}
			flag = 1 - flag;
		}

		nw_score = dp[(1 - flag) * (size2 + 1) + size2];

		delete[] dp;

		return true;
	}
	return true;
}

//omp abandoned
//bool FASTAAlgorithm::omp_nw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file, const std::string& out_filename1, const std::string& out_filename2) {
//	if (interpretor1.sum * interpretor2.sum >= max_matrix || interpretor1.sum * interpretor2.sum == 0) return false;//too large, cannot calculate
//	std::string bases1 = interpretor1.get_bases();
//	std::string bases2 = interpretor2.get_bases();
//
//	std::string fixed_bases1 = "";
//	std::string fixed_bases2 = "";
//
//	size_t size1 = bases1.size();
//	size_t size2 = bases2.size();
//
//
//	int bases_sum = 0;
//
//	for (size_t i = 0;i <4;i++) {
//		bases_sum += interpretor1.total_inform[i];
//	}
//	if (bases_sum != size1) return false;
//
//	bases_sum = 0;
//	for (size_t i = 0;i < 4;i++) {
//		bases_sum += interpretor2.total_inform[i];
//	}
//	if (bases_sum != size2) return false;
//
//
//	long long* dp = new long long[(size1 + 1) * (size2 + 1)]();
//
//
//	for (int i = 0;i <= size2;i++) {
//		dp[0 * (size2 + 1) + i] = nw_d * i;
//	}
//
//	for (int i = 0;i <= size1;i++) {
//		dp[i * (size2 + 1)] = nw_d * i;
//	}
//
//
//	//get score dp mat
//	for (int sum = 2;sum <= size1 + size2;sum++) {
//		int bases1_index, bases2_index;
//		#pragma omp parallel for private(bases2_index)
//		for (bases1_index = 1; bases1_index <= size1; bases1_index++) {
//			bases2_index = sum - bases1_index;
//
//			if (bases2_index > size2) continue;
//			if (bases2_index < 1) continue;
//
//			long long current_score = dp[(bases1_index - 1) * (size2 + 1) + bases2_index - 1] + nw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);
//			current_score = max(dp[(bases1_index - 1) * (size2 + 1) + bases2_index] + nw_d, current_score);
//			current_score = max(dp[bases1_index * (size2 + 1) + bases2_index - 1] + nw_d, current_score);
//
//			dp[bases1_index * (size2 + 1) + bases2_index] = current_score;
//		}
//	}
//
//
//
//	//search route
//	int reverse_bases1_index = size1;
//	int reverse_bases2_index = size2;
//	long long current_score = dp[reverse_bases1_index * (size2 + 1) + reverse_bases2_index];
//	while (reverse_bases1_index > 0 && reverse_bases2_index > 0) {
//		if (current_score == dp[(reverse_bases1_index - 1) * (size2 + 1) + reverse_bases2_index - 1] + nw_base_score(bases1[reverse_bases1_index - 1], bases2[reverse_bases2_index - 1])) {
//			fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
//			fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
//			reverse_bases1_index--;
//			reverse_bases2_index--;
//		}
//		else if (current_score == dp[(reverse_bases1_index - 1) * (size2 + 1) + reverse_bases2_index] + nw_d) {//(dp[i][j]=dp[i-1][j]+nw_d) means bases1[i] match nothing in bases2, which is equal to add - in bases2
//			fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
//			fixed_bases2 = '-' + fixed_bases2;
//			reverse_bases1_index--;
//		}
//		else {//(dp[i][j]=dp[i][j-1]+nw_d) means bases2[j] match nothing in bases1, which is equal to add - in bases1
//			fixed_bases1 = '-' + fixed_bases1;
//			fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
//			reverse_bases2_index--;
//		}
//
//		current_score = dp[reverse_bases1_index * (size2 + 1) + reverse_bases2_index];
//
//	}
//
//	while (reverse_bases1_index > 0) {
//		fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
//		fixed_bases2 = '-' + fixed_bases2;
//		reverse_bases1_index--;
//	}
//
//	while (reverse_bases2_index > 0) {
//		fixed_bases1 = '-' + fixed_bases1;
//		fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
//		reverse_bases2_index--;
//	}
//
//
//	nw_score = dp[size1 * (size2 + 1) + size2];
//
//	delete[] dp;
//
//	//out to file
//	if (!out_to_file) return true;
//
//	if (!fixed_bases_write_outfile(interpretor1, fixed_bases1, out_filename1)) return false;
//	if (!fixed_bases_write_outfile(interpretor2, fixed_bases2, out_filename2)) return false;
//
//
//	return true;
//}

bool FASTAAlgorithm::large_nw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file, const std::string& out_filename1, const std::string& out_filename2) {
	
	std::fstream temp_f("nw.temp",std::ios::out | std::ios::binary | std::ios::trunc);
	
	if (!temp_f.good()) return false;
	std::string bases1 = interpretor1.get_bases();
	std::string bases2 = interpretor2.get_bases();

	std::string fixed_bases1 = "";
	std::string fixed_bases2 = "";

	size_t size1 = bases1.size();
	size_t size2 = bases2.size();


	int bases_sum = 0;

	for (size_t i = 0; i < 4; i++) {
		bases_sum += interpretor1.total_inform[i];
	}
	if (bases_sum != size1) return false;

	bases_sum = 0;
	for (size_t i = 0; i < 4; i++) {
		bases_sum += interpretor2.total_inform[i];
	}
	if (bases_sum != size2) return false;

	long long* pre_row = new long long[size2 + 1];
	long long current_score;
	long long diagonal,front;
	
	for (int i = 0; i <= size2; i++) {
		current_score = i * nw_d;
		temp_f.write((char*)(&current_score), sizeof(long long));
		pre_row[i] = current_score;
	}

	for (size_t bases1_index = 1; bases1_index <= size1; bases1_index++) {
		current_score = bases1_index * nw_d;
		pre_row[0] = bases1_index * nw_d;
		//temp_f.write((char*)(&current_score), sizeof(long long));
		diagonal = (bases1_index - 1) * nw_d;
		front= bases1_index * nw_d;
		for (size_t bases2_index = 1; bases2_index <= size2; bases2_index++) {

			current_score = diagonal + nw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);

			/*temp_f.seekg(-(int(size2) + 2) * sizeof(long long), std::ios::cur);
			temp_f.read((char*)(&diagonal), sizeof(long long));
			temp_f.read((char*)(&above), sizeof(long long));*/

			current_score = max(pre_row[bases2_index] + nw_d, current_score);

			/*temp_f.seekg((size2 - 1) * sizeof(long long),std::ios::cur);
			temp_f.read((char*)(&front), sizeof(long long));*/

			current_score = max(front + nw_d, current_score);
			diagonal = pre_row[bases2_index];
			pre_row[bases2_index] = current_score;
			//temp_f.write((char*)(&current_score), sizeof(long long));
			front = current_score;
		}
		temp_f.write((char*)pre_row, (1 + size2) * sizeof(long long));
	}
	/*std::streampos fileSize = temp_f.tellg();
	long long file_size = fileSize / sizeof(long long);
	std::cout << "right size : " << (1 + size1) * (1 + size2) << "\nfile_size : " << file_size << std::endl;*/

	nw_score = current_score;

	delete[] pre_row;

	temp_f.close();

	temp_f.open("nw.temp", std::ios::in | std::ios::binary);
	if (!temp_f.good()) return false;
	temp_f.seekg(0, std::ios::end);

	int reverse_bases1_index = size1;
	int reverse_bases2_index = size2;
	int goal_score;

	/*temp_f.seekg(-1 * sizeof(long long), std::ios::cur);
	temp_f.read((char*)(&current_score), sizeof(long long));
	std::cout << "last score: " << pre_row[size2] << "\nfile last score :" << current_score << std::endl;*/

	while (reverse_bases1_index > 0 && reverse_bases2_index > 0) {
		temp_f.seekg(-2 * sizeof(long long), std::ios::cur);
		temp_f.read((char*)(&goal_score), sizeof(long long));
		if (current_score == goal_score + nw_d) {//dp[i][j-1]
			fixed_bases1 = '-' + fixed_bases1;
			fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
			reverse_bases2_index--;
			current_score = goal_score;
		}
		else {
			temp_f.seekg(-1 * (size2 + 1) * sizeof(long long), std::ios::cur);
			temp_f.read((char*)(&goal_score), sizeof(long long));
			if (current_score == goal_score + nw_d) {//dp[i-1][j]
				fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
				fixed_bases2 = '-' + fixed_bases2;
				reverse_bases1_index--;
				current_score = goal_score;

			}
			else {//(dp[i][j]=dp[i-1][j-1]+score) 
				fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
				fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
				reverse_bases1_index--;
				reverse_bases2_index--;
				temp_f.seekg(-2 * sizeof(long long), std::ios::cur);
				temp_f.read((char*)(&goal_score), sizeof(long long));
				current_score = goal_score;
			}
		}

	}

	while (reverse_bases1_index > 0) {
		fixed_bases1 = bases1[reverse_bases1_index - 1] + fixed_bases1;
		fixed_bases2 = '-' + fixed_bases2;
		reverse_bases1_index--;
	}

	while (reverse_bases2_index > 0) {
		fixed_bases1 = '-' + fixed_bases1;
		fixed_bases2 = bases2[reverse_bases2_index - 1] + fixed_bases2;
		reverse_bases2_index--;
	}

	temp_f.close();
	remove("nw.temp");

	if (!fixed_bases_write_outfile(interpretor1, fixed_bases1, out_filename1)) return false;
	if (!fixed_bases_write_outfile(interpretor2, fixed_bases2, out_filename2)) return false;

	return true;
}


bool FASTAAlgorithm::fixed_bases_write_outfile(const FASTAInterpretor& interpretor1, const std::string& fixed_bases1, const std::string out_filename1) const {
	std::fstream out_file1(out_filename1, std::ios::out);
	if (!out_file1.good()) return false;
	size_t segment_index = 0, segment_content_index = 0;
	size_t i = 0;
	for (i = 0;i < fixed_bases1.size() && segment_index < interpretor1.fasta_segment.size();i++) {

		if (segment_content_index >= interpretor1.fasta_segment[segment_index].content.size()) {
			segment_content_index = 0;
			segment_index++;
			out_file1 << '\n';
		}
		if (segment_index >= interpretor1.fasta_segment.size()) break;
		for (;segment_index < interpretor1.fasta_segment.size() && interpretor1.fasta_segment[segment_index].is_annotation();segment_index++) {
			out_file1 << '>' << interpretor1.fasta_segment[segment_index].content << '\n';
			segment_content_index = 0;
		}
		out_file1 << fixed_bases1[i];
		if (fixed_bases1[i] != interpretor1.fasta_segment[segment_index].content[segment_content_index]) continue;

		segment_content_index++;
	}

	for(;i < fixed_bases1.size();i++) out_file1 << fixed_bases1[i];

	out_file1.close();

	return true;
}

void FASTAAlgorithm::set_sw_score_mat() {
	std::cout << "Please input the score mat like this mat : \n   A  T  G  C\nA  1 -1 -1 -1\nT -1  1 -1 -1\nG -1 -1  1 -1\nC -1 -1 -1  1\nYou dont need to input the letters ! " << std::endl;
	const char* s = "ATGC";
	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 4;j++) std::cin >> sw_score_mat[base_hash(s[i]) * 8 + base_hash(s[j])];
		//for (int j = 0;j < 4;j++) std::cin >> nw_score_mat[i * 4 + j];
	}
}

void FASTAAlgorithm::set_sw_d() {
	std::cout << "Please input the score of a gap : ";
	std::cin >> sw_d;
}

inline long long FASTAAlgorithm::sw_base_score(const char& a, const char& b) const {
	return sw_score_mat[base_hash(a) * 8 + base_hash(b)];
}

//bool FASTAAlgorithm::sw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file, const std::string& out_filename1) {
//	if (interpretor1.sum * interpretor2.sum >= max_matrix || interpretor1.sum * interpretor2.sum == 0) return false;//too large, cannot calculate
//	std::string bases1 = interpretor1.get_bases();
//	std::string bases2 = interpretor2.get_bases();
//
//	//bases1 is the short one
//
//	std::string fixed_bases1 = "";
//
//	size_t size1 = bases1.size();
//	size_t size2 = bases2.size();
//
//	int bases_sum = 0;
//
//	for (size_t i = 0;i < 4;i++) {
//		bases_sum += interpretor1.total_inform[i];
//	}
//	if (bases_sum != size1) return false;
//
//	bases_sum = 0;
//	for (size_t i = 0;i < 4;i++) {
//		bases_sum += interpretor2.total_inform[i];
//	}
//	if (bases_sum != size2) return false;
//
//
//	long long** dp = new long long* [size1 + 1];
//
//	for (size_t i = 0;i <= size1;i++) {
//		dp[i] = new long long[size2 + 1];
//	}
//
//
//	for (int i = 0;i <= size2;i++) {
//		dp[0][i] = 0;
//	}
//
//	for (int i = 0;i <= size1;i++) {
//		dp[i][0] = 0;
//	}
//
//
//	long long current_max_score = 0;
//	int current_max_index1 = 0, current_max_index2 = 0;
//	
//
//
//	//get score dp mat
//	for (size_t bases1_index = 1;bases1_index <= size1;bases1_index++) {
//		for (size_t bases2_index = 1;bases2_index <= size2;bases2_index++) {
//			dp[bases1_index][bases2_index] = dp[bases1_index - 1][bases2_index - 1] + sw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);//directly judge whether is the same
//			dp[bases1_index][bases2_index] = max(dp[bases1_index - 1][bases2_index] + sw_d, dp[bases1_index][bases2_index]);//is disloacted
//			dp[bases1_index][bases2_index] = max(dp[bases1_index][bases2_index - 1] + sw_d, dp[bases1_index][bases2_index]);//is disloacted
//			dp[bases1_index][bases2_index] = max(0ll, dp[bases1_index][bases2_index]);
//			if (current_max_score < dp[bases1_index][bases2_index]) {
//				current_max_index1 = bases1_index;
//				current_max_index2 = bases2_index;
//				current_max_score = dp[bases1_index][bases2_index];
//			}
//		}
//	}
//
//
//
//	sw_score = current_max_score;
//
//	//search route
//	long long current_score = current_max_score;
//
//
//	while (current_max_index1 > 0 && current_max_index2 > 0 && current_score >0 ) {
//		if (current_score == dp[current_max_index1 - 1][current_max_index2 - 1] + sw_base_score(bases1[current_max_index1 - 1], bases2[current_max_index2 - 1])) {
//			fixed_bases1 = bases1[current_max_index1 - 1] + fixed_bases1;
//			current_max_index1--;
//			current_max_index2--;
//		}
//		else if (current_score == dp[current_max_index1 - 1][current_max_index2] + sw_d) {//(dp[i][j]=dp[i-1][j]+nw_d) means bases1[i] match nothing in bases2, which is equal to add - in bases2
//			fixed_bases1 = bases1[current_max_index1 - 1] + fixed_bases1;
//			current_max_index1--;
//		}
//		else {//(dp[i][j]=dp[i][j-1]+nw_d) means bases2[j] match nothing in bases1, which is equal to add - in bases1
//			fixed_bases1 = '-' + fixed_bases1;
//			current_max_index2--;
//		}
//
//		current_score = dp[current_max_index1][current_max_index2];
//	}
//
//	
//
//
//	for (size_t i = 0;i <= size1;i++) {
//		delete[] dp[i];
//	}
//	delete[] dp;
//
//	//out to file
//	if (!out_to_file) return true;
//
//	std::fstream out_file1(out_filename1, std::ios::out);
//	if (!out_file1.good()) return false;
//	int newline_len = fixed_bases1.size() / 80;
//	for (int i = 0;i < newline_len; i++) {
//		fixed_bases1.insert((i + 1) * 80 + i, "\n");
//	}
//	out_file1 << fixed_bases1;
//	out_file1.close();
//
//	return true;
//}


bool FASTAAlgorithm::better_sw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file, const std::string& out_filename1) {
	if (interpretor1.sum * interpretor2.sum == 0) return false;

	if (out_to_file && interpretor1.sum * interpretor2.sum >= max_matrix) {
		return large_sw(interpretor1, interpretor2, true, out_filename1);
	}


	std::string bases1 = interpretor1.get_bases();
	std::string bases2 = interpretor2.get_bases();

	//bases1 is the short one

	std::string fixed_bases1 = "";

	size_t size1 = bases1.size();
	size_t size2 = bases2.size();

	int bases_sum = 0;

	for (size_t i = 0;i < 4;i++) {
		bases_sum += interpretor1.total_inform[i];
	}
	if (bases_sum != size1) return false;

	bases_sum = 0;
	for (size_t i = 0;i < 4;i++) {
		bases_sum += interpretor2.total_inform[i];
	}
	if (bases_sum != size2) return false;

	if (out_to_file) {
		long long* dp = new long long[(size1 + 1) * (size2 + 1)]();
		long long* pre_row = new long long[size2 + 1]();
		long long* dp_ptr = dp + size2 + 1;

		long long current_max_score = 0;
		int current_max_index1 = 0, current_max_index2 = 0;
		long long current_score = 0;
		long long diagonal = 0;


		//get score dp mat
		for (size_t bases1_index = 1; bases1_index <= size1; bases1_index++) {
			dp_ptr++;
			diagonal = 0;
			for (size_t bases2_index = 1; bases2_index <= size2; bases2_index++, dp_ptr++) {

				current_score = diagonal + sw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);
				current_score = max(pre_row[bases2_index] + sw_d, current_score);
				current_score = max(*(dp_ptr - 1) + sw_d, current_score);
				current_score = max(0ll, current_score);

				*dp_ptr = current_score;

				if (current_max_score < current_score) {
					current_max_index1 = bases1_index;
					current_max_index2 = bases2_index;
					current_max_score = current_score;
				}

				diagonal = pre_row[bases2_index];
				pre_row[bases2_index] = current_score;
			}
		}

		delete[] pre_row;
		pre_row = nullptr;
		dp_ptr = nullptr;


		sw_score = current_max_score;

		//search route
		current_score = current_max_score;

		while (current_max_index1 > 0 && current_max_index2 > 0 && current_score > 0) {
			if (current_score == dp[(current_max_index1 - 1) * (size2 + 1) + current_max_index2 - 1] + sw_base_score(bases1[current_max_index1 - 1], bases2[current_max_index2 - 1])) {
				fixed_bases1 = bases1[current_max_index1 - 1] + fixed_bases1;
				current_max_index1--;
				current_max_index2--;
			}
			else if (current_score == dp[(current_max_index1 - 1) * (size2 + 1) + current_max_index2] + sw_d) {//(dp[i][j]=dp[i-1][j]+nw_d) means bases1[i] match nothing in bases2, which is equal to add - in bases2
				fixed_bases1 = bases1[current_max_index1 - 1] + fixed_bases1;
				current_max_index1--;
			}
			else {//(dp[i][j]=dp[i][j-1]+nw_d) means bases2[j] match nothing in bases1, which is equal to add - in bases1
				fixed_bases1 = '-' + fixed_bases1;
				current_max_index2--;
			}

			current_score = dp[current_max_index1 * (size2 + 1) + current_max_index2];
		}


		delete[] dp;

		//out_to_file
		std::fstream out_file1(out_filename1, std::ios::out);
		if (!out_file1.good()) return false;
		int newline_len = fixed_bases1.size() / 80;
		for (int i = 0; i < newline_len; i++) {
			fixed_bases1.insert((i + 1) * 80 + i, "\n");
		}
		out_file1 << fixed_bases1;
		out_file1.close();

		return true;
	}
	else {

		long long* dp = new long long[2 * (size2 + 1)]();
		int flag = 0;

		long long current_score, max_current_score = 0;
		for (size_t bases1_index = 1; bases1_index <= size1; bases1_index++) {
			dp[flag * (size2 + 1) + 0] = 0;
			for (size_t bases2_index = 1; bases2_index <= size2; bases2_index++) {

				current_score = dp[(1 - flag) * (size2 + 1) + bases2_index - 1] + sw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);
				current_score = max(dp[(1 - flag) * (size2 + 1) + bases2_index] + sw_d, current_score);
				current_score = max(dp[flag * (size2 + 1) + bases2_index - 1] + sw_d, current_score);


				max_current_score = max(current_score, max_current_score);
				dp[flag * (size2 + 1) + bases2_index] = current_score;
			}
			flag = 1 - flag;
		}

		sw_score = max_current_score;

		delete[] dp;

		return true;
	}
	return true;
}

//omp abandoned
//bool FASTAAlgorithm::omp_sw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file, const std::string& out_filename1) {
//	if (interpretor1.sum * interpretor2.sum >= max_matrix || interpretor1.sum * interpretor2.sum == 0) return false;//too large, cannot calculate
//	std::string bases1 = interpretor1.get_bases();
//	std::string bases2 = interpretor2.get_bases();
//
//	//bases1 is the short one
//
//	std::string fixed_bases1 = "";
//
//	size_t size1 = bases1.size();
//	size_t size2 = bases2.size();
//
//	int bases_sum = 0;
//
//	for (size_t i = 0;i < 4;i++) {
//		bases_sum += interpretor1.total_inform[i];
//	}
//	if (bases_sum != size1) return false;
//
//	bases_sum = 0;
//	for (size_t i = 0;i < 4;i++) {
//		bases_sum += interpretor2.total_inform[i];
//	}
//	if (bases_sum != size2) return false;
//
//	long long* dp = new long long[(size1 + 1) * (size2 + 1)]();
//
//	long long current_max_score = 0;
//	int current_max_index1 = 0, current_max_index2 = 0;
//
//
//	//get score dp mat
//	for (size_t sum = 2;sum <= size1 + size2;sum++) {
//		int bases1_index, bases2_index;
//		#pragma omp parallel for private(bases2_index)
//		for (bases1_index = 1; bases1_index <= size1; bases1_index++) {
//			bases2_index = sum - bases1_index;
//
//			if (bases2_index > size2) continue;
//			if (bases2_index < 1) continue;
//
//			long long current_score = dp[(bases1_index - 1) * (size2 + 1) + bases2_index - 1] + sw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);
//			current_score = max(dp[(bases1_index - 1) * (size2 + 1) + bases2_index] + sw_d, current_score);
//			current_score = max(dp[bases1_index * (size2 + 1) + bases2_index - 1] + sw_d, current_score);
//			current_score = max(0ll, current_score);
//
//			dp[bases1_index * (size2 + 1) + bases2_index] = current_score;
//
//			#pragma omp critical
//			{
//				if (current_max_score < current_score) {
//					current_max_index1 = bases1_index;
//					current_max_index2 = bases2_index;
//					current_max_score = current_score;
//				}
//			}
//		}
//	}
//
//
//
//	sw_score = current_max_score;
//
//	//search route
//	long long current_score = current_max_score;
//
//	while (current_max_index1 > 0 && current_max_index2 > 0 && current_score > 0) {
//		if (current_score == dp[(current_max_index1 - 1) * (size2 + 1) + current_max_index2 - 1] + sw_base_score(bases1[current_max_index1 - 1], bases2[current_max_index2 - 1])) {
//			fixed_bases1 = bases1[current_max_index1 - 1] + fixed_bases1;
//			current_max_index1--;
//			current_max_index2--;
//		}
//		else if (current_score == dp[(current_max_index1 - 1) * (size2 + 1) + current_max_index2] + sw_d) {//(dp[i][j]=dp[i-1][j]+nw_d) means bases1[i] match nothing in bases2, which is equal to add - in bases2
//			fixed_bases1 = bases1[current_max_index1 - 1] + fixed_bases1;
//			current_max_index1--;
//		}
//		else {//(dp[i][j]=dp[i][j-1]+nw_d) means bases2[j] match nothing in bases1, which is equal to add - in bases1
//			fixed_bases1 = '-' + fixed_bases1;
//			current_max_index2--;
//		}
//
//		current_score = dp[current_max_index1 * (size2 + 1) + current_max_index2];
//	}
//
//
//	delete[] dp;
//
//	//out to file
//	if (!out_to_file) return true;
//
//	std::fstream out_file1(out_filename1, std::ios::out);
//	if (!out_file1.good()) return false;
//	int newline_len = fixed_bases1.size() / 80;
//	for (int i = 0;i < newline_len; i++) {
//		fixed_bases1.insert((i + 1) * 80 + i, "\n");
//	}
//	out_file1 << fixed_bases1;
//	out_file1.close();
//
//	return true;
//}

bool FASTAAlgorithm::large_sw(const FASTAInterpretor& interpretor1, const FASTAInterpretor& interpretor2, bool out_to_file, const std::string& out_filename1) {

	std::fstream temp_f("sw.temp", std::ios::out | std::ios::binary | std::ios::trunc);

	if (!temp_f.good()) return false;
	std::string bases1 = interpretor1.get_bases();
	std::string bases2 = interpretor2.get_bases();

	std::string fixed_bases1 = "";
	std::string fixed_bases2 = "";

	size_t size1 = bases1.size();
	size_t size2 = bases2.size();


	int bases_sum = 0;

	for (size_t i = 0; i < 4; i++) {
		bases_sum += interpretor1.total_inform[i];
	}
	if (bases_sum != size1) return false;

	bases_sum = 0;
	for (size_t i = 0; i < 4; i++) {
		bases_sum += interpretor2.total_inform[i];
	}
	if (bases_sum != size2) return false;

	long long* pre_row = new long long[size2 + 1]();
	long long current_score;
	long long current_max_score = 0;
	int current_max_index1 = 0, current_max_index2 = 0;
	long long diagonal, front;

	temp_f.write((char*)pre_row, (1 + size2) * sizeof(long long));

	for (size_t bases1_index = 1; bases1_index <= size1; bases1_index++) {
		current_score = 0;
		//temp_f.write((char*)(&current_score), sizeof(long long));
		diagonal = 0;
		front = 0;
		for (size_t bases2_index = 1; bases2_index <= size2; bases2_index++) {

			current_score = diagonal + sw_base_score(bases1[bases1_index - 1], bases2[bases2_index - 1]);

			/*temp_f.seekg(-(int(size2) + 2) * sizeof(long long), std::ios::cur);
			temp_f.read((char*)(&diagonal), sizeof(long long));
			temp_f.read((char*)(&above), sizeof(long long));*/

			current_score = max(pre_row[bases2_index] + sw_d, current_score);

			/*temp_f.seekg((size2 - 1) * sizeof(long long),std::ios::cur);
			temp_f.read((char*)(&front), sizeof(long long));*/

			current_score = max(front + sw_d, current_score);

			if (current_max_score < current_score) {
				current_max_index1 = bases1_index;
				current_max_index2 = bases2_index;
				current_max_score = current_score;
			}

			diagonal = pre_row[bases2_index];
			pre_row[bases2_index] = current_score;
			front = current_score;
		}
		temp_f.write((char*)pre_row, (1 + size2) * sizeof(long long));
	}

	sw_score = current_max_score;

	delete[] pre_row;

	temp_f.close();

	temp_f.open("sw.temp", std::ios::in | std::ios::binary);
	if (!temp_f.good()) return false;
	temp_f.seekg((current_max_index1 * (size2 + 1) + current_max_index2 + 1) * sizeof(long long), std::ios::beg);

	current_score = current_max_score;
	int goal_score;
	


	while (current_max_index1 > 0 && current_max_index2 > 0 && current_score > 0) {
		temp_f.seekg(-2 * sizeof(long long), std::ios::cur);
		temp_f.read((char*)(&goal_score), sizeof(long long));
		if (current_score == goal_score + sw_d) {//dp[i][j-1]
			fixed_bases1 = '-' + fixed_bases1;
			fixed_bases2 = bases2[current_max_index2 - 1] + fixed_bases2;
			current_max_index2--;
			current_score = goal_score;
		}
		else {
			temp_f.seekg(-1 * (size2 + 1) * sizeof(long long), std::ios::cur);
			temp_f.read((char*)(&goal_score), sizeof(long long));
			if (current_score == goal_score + sw_d) {//dp[i-1][j]
				fixed_bases1 = bases1[current_max_index1 - 1] + fixed_bases1;
				fixed_bases2 = '-' + fixed_bases2;
				current_max_index1--;
				current_score = goal_score;

			}
			else {//(dp[i][j]=dp[i-1][j-1]+score) 
				fixed_bases1 = bases1[current_max_index1 - 1] + fixed_bases1;
				fixed_bases2 = bases2[current_max_index2 - 1] + fixed_bases2;
				current_max_index1--;
				current_max_index2--;
				temp_f.seekg(-2 * sizeof(long long), std::ios::cur);
				temp_f.read((char*)(&goal_score), sizeof(long long));
				current_score = goal_score;
			}
		}

	}

	

	temp_f.close();
	remove("sw.temp");

	if (!out_to_file) return false;
	std::fstream out_file1(out_filename1, std::ios::out);
	if (!out_file1.good()) return false;
	int newline_len = fixed_bases1.size() / 80;
	for (int i = 0; i < newline_len; i++) {
		fixed_bases1.insert((i + 1) * 80 + i, "\n");
	}
	out_file1 << fixed_bases1;
	out_file1.close();

	return true;
}