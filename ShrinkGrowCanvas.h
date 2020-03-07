#ifndef WARPER_SHRINKGROW_CANVAS
#define WARPER_SHRINKGROW_CANVAS

#include "WarperCanvas.h"

namespace wrap
{
	namespace ImageWarper
	{
		class DLLEXPORTED ShrinkGrowCanvas : public WarperCanvas
		{
			float m_GrowFactor;
			bool  m_bGrow;
			int   m_derectionType;

		public:
			ShrinkGrowCanvas(Rect &boundary, int iMaxWidth, int iMaxHeight, bool bGrow = true, int derecType = SHRINK_ALL);
			virtual ~ShrinkGrowCanvas();
			virtual void DoForce(Point &ptStart, Point &ptEnd,  Mat2i *gridPoints, Rect &boundary);
		};
	}
}

#endif /* WARPER_SHRINKGROW_CANVAS */