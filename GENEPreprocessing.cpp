#include "GENEPreprocessing.h"

GENEPreprocessing::GENEPreprocessing() {

}
GENEPreprocessing::~GENEPreprocessing() {

}

void GENEPreprocessing::z_score(GENEFile &gf) const {
	double sum_x;
	double aver_x;
	double square_sum;
	double std_deviation,inv_std_deviation;

	#pragma omp parallel for
	for (int i = 0;i < gf.data.size();i++) {
		sum_x = 0;
		square_sum = 0;

		for (size_t j = 0;j < gf.data[i].size();j++) {
			sum_x += gf.data[i][j];
			square_sum += gf.data[i][j] * gf.data[i][j];
		}

		aver_x = sum_x / gf.data[i].size();
		std_deviation = sqrt((square_sum - 2.0 * sum_x * aver_x) / gf.data[i].size() + aver_x * aver_x);
		if (abs(std_deviation) <= 1e-6) continue;

		inv_std_deviation = 1.0 / std_deviation;

		for (size_t j = 0;j < gf.data[i].size();j++) {
			gf.data[i][j] = (gf.data[i][j] - aver_x) * inv_std_deviation;
		}
	}
}

