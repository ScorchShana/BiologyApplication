#include "GENEAlgorihtm.h"

return_type GENEAlgorithm::independent_t_test(const GENEFile& gf1, const GENEFile& gf2, const int& a_level, std::vector<bool> &res,std::vector<double> &t_res)const {
	if (gf1.data.size() != gf2.data.size()) return size_error;

	res.clear();

	bool is_few = false;
	bool is_many = false;

	#pragma omp parallel for
	for (int row = 0;row < gf1.data.size();row++) {

		if (gf1.data[row].size() < 2 || gf2.data[row].size() < 2) is_few = true;
		if (is_few) continue;

		double n1 = gf1.data[row].size();
		double n2 = gf2.data[row].size();
		int df = gf1.data[row].size() + gf2.data[row].size() - 2;

		if (df > 40) is_many = true;
		if (is_many) continue;

		df--;

		double sum1 = 0;
		double square_sum1 = 0;
		double sum2 = 0;
		double square_sum2 = 0;
		double S1;
		double S2;

		for (size_t i = 0;i < gf1.data[row].size();i++) {
			sum1 += gf1.data[row][i];
			square_sum1 += gf1.data[row][i] * gf1.data[row][i];
		}
		S1 = (square_sum1 - 2.0 * sum1 / n1 * sum1 + sum1 / n1 * sum1) / n1;

		for (size_t i = 0;i < gf2.data[row].size();i++) {
			sum2 += gf2.data[row][i];
			square_sum2 += gf2.data[row][i] * gf2.data[row][i];
		}
		S2 = (square_sum2 - 2.0 * sum2 / n2 * sum2 + sum2 / n2 * sum2) / n2;


		double t = (sum1 / n1 - sum2 / n2) / std::sqrt(((n1 - 1) * S1 + (n2 - 1) * S2) / (n1 + n2 - 2) * (1 / n1 + 1 / n2));
		res.push_back(abs(t)>t_table[df][a_level]);//true is significantly different
		t_res.push_back(t);
	}

	if (is_few)return too_few;
	else if (is_many) return too_many;
	return success;
}

void GENEAlgorithm::show_independent_t_test(const GENEFile& gf1, const int& a_level, const std::vector<bool>& res, const std::vector<double>& t_res) const {
	std::cout << "The result of t-test with the alpha of " << alpha[a_level] << " :" << std::endl;
	for (size_t i = 0;i < gf1.genename.size();i++) {
		if (res[i]) std::cout << "Gene " << gf1.genename[i] << " indicate the significant difference ! ";
		else std::cout << "Gene " << gf1.genename[i] << " doesn't indicate the significant difference ! ";
		std::cout << "And the value t of it is " << t_res[i] << '\n';
	}
	std::cout << std::endl;
}

