#ifndef BLUR_BLUR
#define BLUR_BLUR

#include "core.hpp"
#include "FlwCanvas.h"

namespace wrap
{
	namespace ImageBlur
	{
		class DLLEXPORTED Blur
		{
			FlwCanvas *m_flw;
		public:
			Blur();
			virtual ~Blur();
			void doBlur(Mat &srcIamge, Mat dstImage, Rect &roi, int r, BlurType  blurType);
			void doFlw(Mat &srcImage, Mat dstImage, Rect &roi);
		};
	}
}


#endif /* BLUR_BLUR */
