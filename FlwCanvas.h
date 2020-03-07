#ifndef BLUR_FLW
#define BULR_FLW

#include "core.hpp"

namespace wrap
{
	namespace ImageBlur
	{
		class DLLEXPORTED FlwCanvas
		{
		public:
			FlwCanvas();
			~FlwCanvas();
			void doFlwBlur(const Mat &srcImage, Mat &dstImage, Rect &roi);
		};
	}
}


#endif /* BULR_FLW */