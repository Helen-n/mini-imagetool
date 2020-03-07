#ifndef BLUR_MANAGE_BLUR
#define BLUR_MANAGE_BLUR

#include "Blur.h"

namespace wrap
{
	namespace ImageBlur
	{
		
			/*=================================================================================*/
			// common variable
			/*=================================================================================*/
			static Blur     *m_blur;
			static int       m_channels;
			static Mat      *m_srcImage;
			static Mat      *m_dstImage;
			static Landmark *m_landmark;

			/*=================================================================================*/
			// common part interface
			/*=================================================================================*/
			extern "C" DLLEXPORTED bool blurInit(int width, int height, int scanWidth, int bpp, unsigned char* rawData,
				int nCount, const double* doubleArray[]);

			extern "C" DLLEXPORTED unsigned char* blurImage(int *width, int *height, int *scanWidth, int *bpp);

			extern "C" DLLEXPORTED void blurRelease();

			/*=================================================================================*/
			// special usage interface 
			/*=================================================================================*/
			extern "C" DLLEXPORTED void blurFlw(int brushSize, int strength);

			/*=================================================================================*/
			// common usage interface
			/*=================================================================================*/
			extern "C" DLLEXPORTED void blurCommon(int x, int y, int w, int h, int r, BlurType blurType);
	
	}	
}


#endif /* BLUR_MANAGE_BLUR */

