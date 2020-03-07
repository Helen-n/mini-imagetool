#ifndef WAHITEN_SKINDETECTOR
#define WAHITEN_SKINDETECTOR

#include "core.hpp"
#include "Histogram.h"

namespace wrap
{
	namespace ImageWhiten
	{
		class DLLEXPORTED SkinDetector
		{
			Mat image;

			int _hueLower;
			int _hueUpper;

			Scalar lower;
			Scalar higher;

			Mat hist;

			float _mergeFactor;

			vector<int> histSize;
			vector<float> ranges;
			vector<int> channels;

			Histogram h;

			Mat p1;

		public:
			SkinDetector();
			void skinMask(Mat image, Mat &mask);
		};
	}
}

#endif /* WAHITEN_SKINDETECTOR */

