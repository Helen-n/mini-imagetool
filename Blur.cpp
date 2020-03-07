#include "Blur.h"

namespace wrap
{
	namespace ImageBlur
	{
		Blur::Blur()
		{
			m_flw = new FlwCanvas();
		}

		Blur::~Blur()
		{}

		void Blur::doBlur(Mat & srcIamge, Mat dstImage, Rect & roi, int r, BlurType blurType)
		{
			cv::Mat element = getStructuringElement(MORPH_RECT, Size(r, r));

			switch (blurType)
			{
			case BLUR_GUASS:
				GaussianBlur(srcIamge(roi), dstImage, Size(r, r), 5);
				break;
			case BLUR_BOX:
				blur(srcIamge(roi), dstImage, Size(r, r));
				break;
			case BLUR_MIDDLE:
				medianBlur(srcIamge(roi), dstImage, r);
				break;
			case BLUR_DOUBLE:
				bilateralFilter(srcIamge(roi), dstImage, r, 20, 30);
				break;
			default:
				break;
			}

			Mat tmpImage = srcIamge.clone();
			dstImage.copyTo(tmpImage(roi));
			dstImage = tmpImage.clone();
		}

		void Blur::doFlw(Mat & srcImage, Mat dstImage, Rect & roi)
		{
			m_flw->doFlwBlur(srcImage, dstImage, roi);
		}
	}
}
