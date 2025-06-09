#pragma once

#ifndef GENEPREPORCESSING_HEADER

#define GENEPREPORCESSING_HEADER

#include <vector>
#include "GENEFile.h"
#include <cmath>

class GENEPreprocessing {
	public:
		GENEPreprocessing();
		~GENEPreprocessing();

		void z_score(GENEFile &gf) const;

};
#endif // !GENEPREPORCESSING_HEADER
