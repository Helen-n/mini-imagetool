#ifndef WARPER_MANAGEWARPER
#define WARPER_MANAGEWARPER

#include "Warper.h"

namespace wrap 
{
	namespace ImageWarper
	{
		/*=================================================================================*/
		// common variable
		/*=================================================================================*/
		static int       m_channels;
		static Mat      *m_srcImage;
		static Mat      *m_dstImage;
		static Warper   *m_warper;
		static Landmark *m_landmark;

		/*=================================================================================*/
		// common part interface
		/*=================================================================================*/
		extern "C" DLLEXPORTED bool warperInit(int width, int height, int scanWidth, int bpp, unsigned char* rawData,
			int nCount, const double* doubleArray[]);

		extern "C" DLLEXPORTED unsigned char* warperImage(int *width, int *height, int *scanWidth, int *bpp);

		extern "C" DLLEXPORTED void warperRelease();

		/*=================================================================================*/
		// special usage interface 
		/*=================================================================================*/
		extern "C" DLLEXPORTED void warperFace(int brushSize, int strength, WarperFaceType faceType);

		extern "C" DLLEXPORTED void warperNose(int brushSize, int strength, WarperNoseType noseType);

		extern "C" DLLEXPORTED void warperFill(int brushSize, int strength, WarperFillType fillType);

		/*=================================================================================*/
		// common usage interface
		/*=================================================================================*/
		extern "C" DLLEXPORTED void warperPoint(int sx, int sy, int ex, int ey, int brushSize, WarperType warperType, DerectionType derection=DERECTION_ALL);
	}
}

#endif /* WARPER_MANAGEWARPER */







