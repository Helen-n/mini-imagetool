#include "TranslateCanvas.h"

namespace wrap
{
	namespace ImageWarper
	{
		TranslateCanvas::TranslateCanvas(Rect boundary, int iMaxWidth, int iMaxHeight)
			: WarperCanvas(boundary, iMaxWidth, iMaxHeight)
		{}

		TranslateCanvas::~TranslateCanvas()
		{}

		void TranslateCanvas::DoForce(Point &ptStart, Point &ptEnd, Mat2i *gridPoints, Rect &boundary)
		{
			Point2i ptDelta(ptEnd.x - ptStart.x, ptEnd.y - ptStart.y);
			float fCrossDistance = (float)min(
				max(ptStart.x - boundary.x, boundary.br().x - ptStart.x),
				max(ptStart.y - boundary.y, boundary.br().y - ptStart.y));

			float fDist;
			Point2i ptTmp;

			for (int i = gridPoints->cols - 1; i >= 0; --i)
			{
				for (int j = gridPoints->rows - 1; j >= 0; --j)
				{
					ptTmp = gridPoints->at<Vec2i>(i, j);
					fDist = (Distance(ptTmp, ptStart) / fCrossDistance) - 1;
					;
					if (fDist < 0)
					{
						ptTmp.x += (int)(ptDelta.x * fDist);
						ptTmp.y += (int)(ptDelta.y * fDist);
						gridPoints->at<Vec2i>(i, j) = ptTmp;
					}
				}
			}
		}
	}
}
