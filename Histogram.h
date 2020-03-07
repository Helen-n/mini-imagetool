#ifndef WHITEN_HISTOGRAM
#define WHITEN_HISTOGRAM

#include "core.hpp"

namespace wrap
{
	namespace ImageWhiten
	{

		class DLLEXPORTED Histogram
		{
		public:

			Histogram();

			Mat getHist();

			Mat BuildHistogram(Mat srcImage, bool accumulate = false);

			vector<int> getThresh(Mat srcImage, float s1, float s2);

			Mat likeyhoodImage(Mat image);

			void setHistSize(vector<int> size);

			void setRange(vector<float>);

			void setChannel(vector<int>);

			Mat applyKernel(int, int);

			float compareHist(Histogram hist, int method);

			Mat drawHist();

			void setMask(Mat mask);

			void mergeHistogram(Mat hist, float factor);

			void setHist(Mat hist){ hist.copyTo(_histMat); }

			vector<int> getThreshHist(Mat histMat, float s1, float s2);

			Mat drawHist(MatND);
		private:

			Mat _mask;

			MatND _histMat;

			vector<int> _histSize;  // Size of the histogram

			vector<float> _histRange;  //Range for all the matrices

			vector<int>_channels;

			Mat _tmpHist;

		};
	}
}
	
#endif /* WHITEN_HISTOGRAM */
