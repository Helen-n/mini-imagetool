#ifndef INPAINT_LBP_CANVAS
#define INPAINT_LBP_CANVAS

#include "core.hpp"

namespace wrap
{
	namespace ImageInpaint
	{
		class DLLEXPORTED lbpCanvas
		{
		private:
			void computeLBPImage_Gray(const Mat &srcImage, Mat &lbpImage);

			void computeLBPImage_Uniform(const Mat &srcImage, Mat &lbpImage);

			void getMask(Mat &srcMat, Mat &mask);

		public:
			lbpCanvas();

			~lbpCanvas();

			void doInpaint(Mat &srcImage, Mat &dstImage, vector<Point> &curve, int r, LbpType lbpType=LBP_GRAY, int getMask = -1);

		};
	}
}

#endif /* INPAINT_LBP_CANVAS */
