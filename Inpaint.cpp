#include "Inpaint.h"

namespace wrap
{
	namespace ImageInpaint
	{
		Inpaint:: Inpaint()
		{
			m_canvas = new lbpCanvas();
		}

		Inpaint::~Inpaint()
		{
			if (m_canvas)
				delete m_canvas;
		}

		void Inpaint::doInpaint(Mat &srcImage, Mat &dstImage, Point point, int r)
		{
			dstImage  = srcImage.clone();
			int nRows = srcImage.rows;
			int nCols = srcImage.cols;

			if ((0 <= point.x && point.x < nCols) && (0 <= point.y && point.y < nRows))
			{
				Mat mask(dstImage.size(), CV_8UC1, Scalar(0));
				Rect rect(max(0,point.x-r/2), max(0,point.y-r/2),r,r);
				mask(rect) = Scalar(255);
				inpaint(dstImage, mask, dstImage, r, 0);
			}
		}

		void Inpaint::doInapintLbp(Mat &srcImage, Mat &dstImage, vector<Point> &curve, int r, LbpType lbpType, int getMask=-1)
		{
			m_canvas->doInpaint(srcImage, dstImage, curve, r);
		}
	}
}

