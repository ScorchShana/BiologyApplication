#include "FASTAInterpretor.h"


static std::string DNACodeNmae[19] = { "       腺苷(adenosine)","      胞嘧啶(cytidine)","       鸟嘌呤(guanine)","   胸腺嘧啶(thymidine)","       尿嘧啶(uridine)",
	"         G或A (puRine)",	" T或C (嘧啶pyrimidine)","       G或T (酮化keto)","      A或C (胺化amino)","   G或C (强作用strong)",
	"     A或T (弱作用weak)","               G或T或C","               G或A或T","               A或C或T","               G或C或A",
	"            A或G或C或T","            未知的碱基","  片段长度不定(或空缺)","            错误的符号" };

int basecode[255] = { UNSIGNEDCODE };


int encode(const char& s) {
	static const std::unordered_map<char, int> encoding_map = {
		{'A', A}, {'a', A}, {'T', T}, {'t', T}, {'G', G}, {'g', G},
		{'C', C}, {'c', C}, {'U', U}, {'u', U}, {'R', G_A}, {'r', G_A},
		{'Y', T_C}, {'y', T_C}, {'K', G_T}, {'k', G_T}, {'M', A_C}, {'m', A_C},
		{'S', G_C}, {'s', G_C}, {'W', A_T}, {'w', A_T}, {'B', G_T_C}, {'b', G_T_C},
		{'D', G_A_T}, {'d', G_A_T}, {'H', A_C_T}, {'h', A_C_T}, {'V', G_C_A}, {'v', G_C_A},
		{'N', A_G_C_T}, {'n', A_G_C_T}, {'X', UNCERTAIN}, {'x', UNCERTAIN}, {'-', UNCERTAIN_LENGTH}
	};

	auto it = encoding_map.find(s);
	return (it != encoding_map.end()) ? it->second : UNSIGNEDCODE;
}

std::string number_to_str(unsigned long long i) {
	return std::to_string(i);
}

std::string number_precentage_str(unsigned long long i, unsigned long long sum) {

	std::string s;
	if (i == 0ll) {
		s = "0(0%)";
		return s;
	}

	unsigned long long percentage = 10000.0 * i / sum;
	char num;
	while (i) {
		num = '0' + i % 10;
		s = num + s;
		i /= 10;
	}

	if (percentage == 0) {
		s += "(<0.01%)";
		return s;
	}
	s += '(';
	if (percentage >= 1000) s += '0' + percentage / 1000;
	if (percentage >= 100) s += '0' + (percentage / 100) % 10;
	if (percentage < 100) s += '0';
	s += '.';
	if (percentage >= 10) s += '0' + (percentage / 10) % 10;
	if (percentage > 0) s += '0' + percentage % 10;
	s += "%)";

	return s;
}


//FASTASegment
FASTASegment::FASTASegment() : segment_type(annotation),content("") {
	return;
}

FASTASegment::~FASTASegment() {
	content.clear();
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

bool FASTASegment::is_annotation() const {
	return segment_type == annotation;
}

bool FASTASegment::is_sequence() const {
	return segment_type == sequence;
}





//FASTAInterpretor
FASTAInterpretor::FASTAInterpretor() : sum(0ll),interpreted(false) {
	for (int i = 0;i < 20;i++) total_inform[i] = 0;
}

FASTAInterpretor::~FASTAInterpretor() {
	fasta_segment.clear();
}

bool FASTAInterpretor::load_file(const std::string& filename) {
	return fastafile.open(filename);
}


//return if there is sth. wrong while interpretor, but not to stop the process
bool FASTAInterpretor::interpret() {

	if (interpreted) return false;
	if (!fastafile.good()) return false;

	//init
	fasta_segment.clear();
	for (int i = 0;i < 20;i++) total_inform[i] = 0;
	
	std::string wordline;

	bool process_good = true;

	while (!fastafile.eof()) {
		FASTASegment fsegment;
		fastafile >> wordline;
		if (wordline.empty()) continue;

		if (wordline[0] == ':' || wordline[0] == '>') {
			fsegment.set(annotation,wordline.substr(1));
		}
		else {
			fsegment.set(sequence, wordline);
			if (!compile(fsegment)) process_good=true;
		}
		fasta_segment.push_back(fsegment);
	}

	for (int i = 0;i < 20;i++) {
		if (i <= UNCERTAIN) sum += total_inform[i];
	}

	return process_good;
}


void FASTAInterpretor::showCMD() const {

	unsigned long long sum = 0;
	unsigned long long max_num = 0;
	for (int i = 0;i < 20;i++) {
		if (i <= UNCERTAIN) sum += total_inform[i];
		max_num = (max_num > total_inform[i]) ? max_num : total_inform[i];
	}

	for (size_t i = 0;i < 19;i++) {
		unsigned int length = 70.0 * total_inform[i] / max_num;
		std::cout << DNACodeNmae[i] << ' ';
		for (size_t j = 0;j < length;j++) std::cout << '-';
		std::cout << ' ' << number_precentage_str(total_inform[i], sum);
		std::cout << std::endl;
	}

	std::cout << "总碱基数(未知长度不计)" << "  " << number_to_str(sum) << std::endl;;

}

//only bases
std::string FASTAInterpretor::get_bases() const {
	std::string bases;
	for (size_t i = 0;i < fasta_segment.size();i++) {
		if (fasta_segment[i].is_sequence()) {
			bases += fasta_segment[i].content;
		}
	}
	return bases;
}

//compile each FASTASemgent
bool FASTAInterpretor::compile(const FASTASegment& segment) {
	if (segment.is_annotation()) return true;

	bool flag = true;
	int res_code;
	for (int i = 0;i < segment.content.size();i++) {
		res_code = encode(segment.content[i]);
		if (res_code == UNSIGNEDCODE && flag) flag = false;
		total_inform[res_code]++;
	}

	return flag;
}

