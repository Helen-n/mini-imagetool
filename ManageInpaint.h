#ifndef INPAINT_MANAGE_INPAINT
#define INPAINT_MANAGE_INPAINT

#include "Inpaint.h"

namespace wrap
{
	namespace ImageInpaint
	{

		/*=================================================================================*/
		// common variable
		/*=================================================================================*/
		static int       m_channels;
		static Mat      *m_srcImage;
		static Mat      *m_dstImage;
		static Inpaint  *m_inpaint;
		static Landmark *m_landmark;

		/*=================================================================================*/
		// common part interface
		/*=================================================================================*/
		extern "C" DLLEXPORTED bool inpaintInit(int width, int height, int scanWidth, int bpp, unsigned char* rawData,
			int nCount, const double* doubleArray[]);

		extern "C" DLLEXPORTED unsigned char* inpaintImage(int *width, int *height, int *scanWidth, int *bpp);

		extern "C" DLLEXPORTED void inpaintRelease();

		/*=================================================================================*/
		// special usage interface 
		/*=================================================================================*/
		extern "C" DLLEXPORTED void inpaintBrow(int r, LbpType lbpType);

		/*=================================================================================*/
		// common usage interface
		/*=================================================================================*/
		extern "C" DLLEXPORTED void inpaintRect(int x, int y, int w, int h, int r, LbpType lbpType);

		extern "C" DLLEXPORTED void inpaintPoint(int x, int y, int r);
	}
}

#endif /* INPAINT_MANAGE_INPAINT */

