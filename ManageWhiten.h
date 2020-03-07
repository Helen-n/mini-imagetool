#ifndef WHITEN_MANAGE_WHITEN
#define WHITEN_MANAGE_WHITEN

#include "core.hpp"
#include "Whiten.h"

namespace wrap
{
	namespace ImageWhiten
	{
		/*=================================================================================*/
		// common variable
		/*=================================================================================*/
		static int       m_channels;
		static Mat      *m_srcImage;
		static Mat      *m_dstImage;
		static Whiten   *m_whiten;
		static Landmark *m_landmark;

		/*=================================================================================*/
		// common part interface
		/*=================================================================================*/
		extern "C" DLLEXPORTED bool whitenInit(int width, int height, int scanWidth, int bpp, unsigned char* rawData,
			int nCount, const double* intArray[]);

		extern "C" DLLEXPORTED unsigned char* whitenImage(int *width, int *height, int *scanWidth, int *bpp);

		extern "C" DLLEXPORTED void whitenRelease();

		/*=================================================================================*/
		// special usage interface 
		/*=================================================================================*/
		extern "C" DLLEXPORTED void whitenFace(double strength, double rosyDegree, int range);

		extern "C" DLLEXPORTED void whitenAll(double strength, double rosyDegree);

		/*=================================================================================*/
		// common usage interface
		/*=================================================================================*/
		extern "C" DLLEXPORTED void whitenPoint(int x, int y, double strength, double rosyDegree, int range);
	}
}

#endif /* WHITEN_MANAGE_WHITEN */