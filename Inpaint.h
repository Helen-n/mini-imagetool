#ifndef INPAINT_INPAINT
#define INPAINT_INPAINT

#include "core.hpp"
#include "lbpCanvas.h"

namespace wrap
{
	namespace ImageInpaint
	{
		class DLLEXPORTED Inpaint
		{
		private:
			lbpCanvas *m_canvas;

		public:
			Inpaint();
			virtual ~Inpaint();
			void doInpaint(Mat &srcImage, Mat &dstImage, Point point, int r);
			void doInapintLbp(Mat &srcImage, Mat &dstImage, vector<Point> &curve, int r, LbpType lbpType, int getMask);
		};
	}
}

#endif /* INPAINT_INPAINT */