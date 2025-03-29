#include "FASTAInterpretor.h"

static LPCTSTR DNACodeNmae[19] = {TEXT("腺苷（adenosine）"),TEXT("胞嘧啶（cytidine）"),TEXT("鸟嘌呤（guanine）"),TEXT("胸腺嘧啶（thymidine）"),TEXT("尿嘧啶（uridine）"),TEXT("G或A (puRine)"),
	TEXT("T或C (嘧啶pyrimidine)"),	TEXT("G或T (酮化keto)"),TEXT("A或C (胺化amino)"),TEXT("G或C (强作用strong)"),TEXT("A或T (弱作用weak)"),TEXT("G或T或C"),TEXT("G或A或T"),TEXT("A或C或T"),
	TEXT("G或C或A"),TEXT("A或G或C或T"),TEXT("未知的"),TEXT("片段长度不定"),TEXT("错误的符号") };

static WCHAR str_number[10] = { TEXT('0'),TEXT('1'),TEXT('2'),TEXT('3'),TEXT('4'),TEXT('5'),TEXT('6'),TEXT('7'),TEXT('8'),TEXT('9') };

std::wstring number_to_lpctstr(unsigned long long i) {

	std::wstring s;
	if (i == 0ll) s = TEXT("0");
	WCHAR num;
	unsigned long long j = i;
	while (j) {
		num = TEXT('0') + j % 10;
		s = num + s;
		j /= 10;
	}
	return s;
}

std::wstring number_precentage(unsigned long long i, unsigned long long sum) {

	std::wstring s;
	if (i == 0ll) {
		s = TEXT("0(0%)");
		return s;
	}

	unsigned long long percentage = 10000.0 * i / sum;
	WCHAR num;
	while (i) {
		num = TEXT('0') + i % 10;
		s = num + s;
		i /= 10;
	}

	if (percentage == 0) {
		s += TEXT("(<0.01%)");
		return s;
	}
	s += '(';
	if (percentage >= 1000) s += TEXT('0') + percentage / 1000;
	if (percentage >= 100) s += TEXT('0') + (percentage / 100) % 10;
	if (percentage < 100) s += TEXT('0');
	s += TEXT('.');
	if (percentage >= 10) s += TEXT('0') + (percentage / 10) % 10;
	if (percentage > 0) s += TEXT('0') + percentage % 10;
	s += TEXT("%)");
	
	return s;
}

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

bool FASTASegment::is_annotation() const {
	return segment_type == annotation;
}

bool FASTASegment::is_sequence() const {
	return segment_type == sequence;
}

std::string FASTASegment::get_content() const {
	return content;
}



//FASTAInterpretor
FASTAInterpretor::FASTAInterpretor() : interpreted(false) {
	for (int i = 0;i < 20;i++) total_inform[i] = 0;
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
	FASTASegment fsegment;

	bool process_good = true;

	while (!fastafile.eof()) {

		fastafile >> wordline;

		if (wordline[0] == ';' || wordline[0] == '>') {
			fsegment.set(annotation,wordline.substr(1));
		}
		else {
			fsegment.set(sequence, wordline);
			if (!compile(fsegment)) process_good=true;
		}
		fasta_segment.push_back(fsegment);
	}



	return process_good;
}

void FASTAInterpretor::show(unsigned int width,unsigned int height) const {
	
	//gray background

	unsigned long long sum = 0;
	unsigned long long max_num = 0;
	for (int i = 0;i < 20;i++) {
		if(i<=UNCERTAIN) sum += total_inform[i];
		max_num = (max_num > total_inform[i]) ? max_num : total_inform[i];
	}

	if (width <= 400 || height <= 300) {
		width = 800;
		height = 600;
	}

	unsigned int line_height = height / 31;
	unsigned int half_line_height = line_height / 2;
	unsigned int begin_x = 0.03 * width;
	unsigned int font_height = (15 < 0.03 * width) ? 15 : 0.03 * width;

	initgraph(width, height);
	setbkcolor(RGB(220,220,220));
	cleardevice();

	//font
	LOGFONT f;
	gettextstyle(&f);// 获取当前字体设置
	f.lfHeight = font_height;// 设置字体高度
	wcscpy_s(f.lfFaceName, _T("黑体"));// 设置字体为“黑体”(高版本 VC 推荐使用 wcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;// 设置输出效果为抗锯齿  
	settextstyle(&f);// 设置字体样式
	settextcolor(BLACK);

	setfillcolor(BLUE);

	RECT textbox = { 0,1,width,line_height };
	drawtext(TEXT("序列信息 Sequence Information"), &textbox, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	for (unsigned int i = 0;i < 20;i++) {
		if (i == 19) {
			outtextxy(begin_x, (i + 1) * (line_height + half_line_height), TEXT("总碱基数（未知长度不计）"));
			outtextxy(begin_x + 20 * font_height, (i + 1) * (line_height + half_line_height), (LPCTSTR)number_to_lpctstr(sum).c_str());
		}
		else {
			outtextxy(begin_x, (i+1) * (line_height + half_line_height), DNACodeNmae[i]);
			solidrectangle(begin_x + 12 * font_height, (i + 1) * (line_height + half_line_height), begin_x + 12 * font_height + width * 0.5 * total_inform[i] / max_num, (i + 2) * line_height + (i + 1) * half_line_height);
			outtextxy(begin_x + 12 * font_height + width * 0.5 * total_inform[i] / max_num + 2, (i + 1) * (line_height + half_line_height), (LPCTSTR)number_precentage(total_inform[i],sum).c_str());
		}
	}

}



bool FASTAInterpretor::compile(const FASTASegment& segment) {
	if (segment.is_annotation()) return true;

	bool flag = true;

	std::string seg_content = segment.get_content();
	for (int i = 0;i < seg_content.size();i++) {
		DNACode res_code = encode(seg_content[i]);
		if (res_code == UNSIGNEDCODE || flag) flag = false;
		total_inform[res_code]++;
	}

	return flag;
}

DNACode FASTAInterpretor::encode(const char& s) const {
	switch (s) {
		case 'A':
		case 'a':
			return A;

		case 'T':
		case 't':
			return T;

		case 'G':
		case 'g':
			return G;

		case 'C':
		case 'c':
			return C;

		case 'U':
		case 'u':
			return U;

		case 'R':
		case 'r':
			return G_A;

		case 'Y':
		case 'y':
			return T_C;

		case 'K':
		case'k':
			return G_T;

		case 'M':
		case 'm':
			return A_C;

		case 'S':
		case 's':
			return G_C;

		case 'W':
		case 'w':
			return A_T;

		case 'B':
		case 'b':
			return G_T_C;

		case 'D':
		case 'd':
			return G_A_T;

		case 'H':
		case 'h':
			return A_C_T;

		case 'V':
		case 'v':
			return G_C_A;

		case 'N':
		case 'n':
			return A_G_C_T;

		case 'X':
		case 'x':
			return UNCERTAIN;

		case '-':
			return UNCERTAIN_LENGTH;

		default:
			return UNSIGNEDCODE;
	}
}