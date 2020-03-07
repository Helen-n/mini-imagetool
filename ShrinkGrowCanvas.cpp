#include "ShrinkGrowCanvas.h"

namespace wrap
{
    namespace ImageWarper
	{
		ShrinkGrowCanvas::ShrinkGrowCanvas(Rect &boundary, int iMaxWidth, int iMaxHeight, bool bGrow, int dectType)
			: WarperCanvas(boundary, iMaxWidth, iMaxHeight),
			m_GrowFactor(0.0f),
			m_bGrow(bGrow),
			m_derectionType(dectType)
		{}

		ShrinkGrowCanvas::~ShrinkGrowCanvas()
		{}

		void ShrinkGrowCanvas::DoForce(Point &ptStart, Point &ptEnd, Mat2i *gridPoints, Rect &boundary)
		{
			int left = gridPoints->cols - 1;
			int right = 0;
			int top = gridPoints->rows - 1;
			int down = 0;
			
			switch (m_derectionType)
			{
			case SHRINK_ALL:
				break;
			case SHRINK_TOP:
				top /= 2;
				break;
			case SHRINK_DOWN:
				down = top / 2;
				break;
			case SHRINK_LEFT:
				left /= 2;
				break;
			case SHRINK_RIGHT:
				right = left / 2;
				break;
			default:
				break;
			}
			float fCrossDistance = (float)min(
				min(ptEnd.x - boundary.x, boundary.br().x - ptEnd.x),
				min(ptEnd.y - boundary.y, boundary.br().y - ptEnd.y));
			Point2i ptTmp;
			float r;

			if (m_GrowFactor < 1)
				m_GrowFactor += GROWSTEP;

			if (m_bGrow)
			{
				// Calculating the new coordinate of the grid points.
				for (int i = left; i >= right; --i)
				{
					for (int j = top; j >= down; --j)
					{
						ptTmp = gridPoints->at<Vec2i>(i, j);
						r = Distance(ptTmp, ptEnd.x, ptEnd.y) / fCrossDistance;

						if (r < 1)
						{
							r = pow(r, m_GrowFactor);
							ptTmp.x = ptEnd.x + (int)((ptTmp.x - ptEnd.x)*r);
							ptTmp.y = ptEnd.y + (int)((ptTmp.y - ptEnd.y)*r);
							gridPoints->at<Vec2i>(i, j) = ptTmp;
						}
					}
				}
			}
			else
			{
				// Calculating the new coordinate of the grid points.
				for (int i = left; i >= right; --i)
				{
					for (int j = top; j >= down; --j)
					{
						ptTmp = gridPoints->at<Vec2i>(i, j);
						r = Distance(ptTmp, ptEnd.x, ptEnd.y) / fCrossDistance;

						if (r < 1 && r > 0.01)
						{
							r = pow(r, m_GrowFactor);
							ptTmp.x = ptEnd.x + (int)((ptTmp.x - ptEnd.x) / r);
							ptTmp.y = ptEnd.y + (int)((ptTmp.y - ptEnd.y) / r);
							gridPoints->at<Vec2i>(i, j) = ptTmp;
						}
					}
				}
			}
		}
	}
}
