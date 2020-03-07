#ifndef WARPER_TRANSLATE_CANVAS
#define WARPER_TRANSLATE_CANVAS

#include "WarperCanvas.h"

namespace wrap
{
	namespace ImageWarper
	{
		class DLLEXPORTED TranslateCanvas : public WarperCanvas
		{
		public:
			TranslateCanvas(Rect boundary, int iMaxWidth, int iMaxHeight);
			virtual~TranslateCanvas();
			virtual void DoForce(Point &ptStart, Point &ptEnd, Mat2i* gridPoints, Rect &boundary);
		};
	}
}

#endif /* WARPER_TRANSLATE_CANVAS */
