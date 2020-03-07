#include "WarperCanvas.h"

namespace wrap
{
	namespace ImageWarper
	{

		WarperCanvas::WarperCanvas(Rect &boundary, int iMaxWidth, int iMaxHeight)
			:m_GridPoints(0),
			m_OffsetPoints(0),
			m_IncrementStep(10),
			m_MaxWidth(iMaxWidth),
			m_MaxHeight(iMaxHeight)
		{
			Initalize(boundary);
			m_IncrementStep = boundary.width / 2;
		}

		WarperCanvas::~WarperCanvas()
		{
			if (m_GridPoints)
				delete m_GridPoints;
			if (m_OffsetPoints)
				delete m_OffsetPoints;
		}

		/*--------------------------------------------------------------------------*/
		// Properties
		/*--------------------------------------------------------------------------*/
		Mat2i* WarperCanvas::GetGridPoints()
		{
			return m_GridPoints;
		}

		Mat2f* WarperCanvas::GetOffsetPoints()
		{
			return m_OffsetPoints;
		}

		int WarperCanvas::GetGridStep()
		{
			return m_GridStep;
		}

		Rect* WarperCanvas::GetBoundary()
		{
			return &m_Boundary;
		}

		/*--------------------------------------------------------------------------*/
		// public interface
		/*--------------------------------------------------------------------------*/
		bool WarperCanvas::Force(Point &ptStart, Point &ptEnd)
		{
			bool bGridUpdate = false;

			// makde sure the start and end points are in the bondary area
			if (!m_Boundary.contains(ptEnd))
			{
				// resize calls resizeGridPos() automatically.
				// This will also update the offset array.
				ResizeGrid(ptEnd);
				bGridUpdate = true;
			}
			else
			{
				ResetGridPos();
			}

			// call the children class.
			DoForce(ptStart, ptEnd, m_GridPoints, m_Boundary);

			// update the offset array.
			CalcOffsets();

			return bGridUpdate;
		}

		/*--------------------------------------------------------------------------*/
		// private unilities
		/*--------------------------------------------------------------------------*/
		void WarperCanvas::Initalize(Rect &rcBoundary)
		{
			m_Boundary = rcBoundary;

			// automatically select the grid step.
			SeletctGridStep();

			// reset the grid to match new grid step.
			ResetGridPos();

			// reset the offset grid to match the new size.
			if (m_OffsetPoints)
				delete m_OffsetPoints;

			m_OffsetPoints = new Mat2f(m_Boundary.width, m_Boundary.height);
		}

		void WarperCanvas::SeletctGridStep()
		{
			int iMinDim = std::min(m_Boundary.width, m_Boundary.height);
			m_GridStep = GRIDSTEP_DEFAULT;

			if ((iMinDim / m_GridStep) < (iMinDim / GRIDSTEP_MAX))
				m_GridStep = GRIDSTEP_MAX;
			else if ((iMinDim / m_GridStep) > (iMinDim / GRIDSTEP_MIN))
				m_GridStep = GRIDSTEP_MIN;
		}

		void WarperCanvas::ResetGridPos()
		{
			Point2i ptTmp;
			int iGridPtCntX = (m_Boundary.width + m_GridStep - 1) / m_GridStep + 1;
			int iGridPtCntY = (m_Boundary.height + m_GridStep - 1) / m_GridStep + 1;

			if (m_GridPoints)
				delete m_GridPoints;
			m_GridPoints = new Mat2i(iGridPtCntX, iGridPtCntY);

			for (int i = 0; i < iGridPtCntX; ++i)
			{
				for (int j = 0; j < iGridPtCntY; ++j)
				{
					ptTmp.x = m_Boundary.x + i * m_GridStep;
					ptTmp.y = m_Boundary.y + j * m_GridStep;

					// last colum
					if (i == iGridPtCntX - 1)
						ptTmp.x = m_Boundary.br().x;

					// last row
					if (j == iGridPtCntY - 1)
						ptTmp.y = m_Boundary.br().y;

					m_GridPoints->at<Vec2i>(i, j) = ptTmp;
				}
			}
		}

		void WarperCanvas::CalcOffsets()
		{
			float xfrac, yfrac;
			float s, t, u, v;

			Point ptTL, ptTR, ptBL, ptBR;

			int iPtXLimit, iPtYLimit;
			int iColumnX, iPxCountX;
			int iRowY, iPxCountY;
			int i, j, m, n;
			Point2f ptTmp;
			Point   ptTmpX;

			iPtXLimit = m_GridPoints->cols - 1;
			iPtYLimit = m_GridPoints->rows - 1;

			for (i = 0; i < iPtXLimit; i++)
			{
				ptTmpX = m_GridPoints->at<Vec2i>(i, 0);
				iColumnX = ptTmpX.x;
				ptTmpX = m_GridPoints->at<Vec2i>(i + 1, 0);
				iPxCountX = ptTmpX.x - iColumnX;

				for (j = 0; j < iPtYLimit; j++)
				{
					ptTmpX = m_GridPoints->at<Vec2i>(0, j);
					iRowY = ptTmpX.y;

					ptTmpX = m_GridPoints->at<Vec2i>(0, j + 1);
					iPxCountY = ptTmpX.y - iRowY;

					xfrac = 1.0f / iPxCountX;
					yfrac = 1.0f / iPxCountY;

					ptTL = m_GridPoints->at<Vec2i>(i, j);
					ptTR = m_GridPoints->at<Vec2i>(i + 1, j);
					ptBL = m_GridPoints->at<Vec2i>(i, j + 1);
					ptBR = m_GridPoints->at<Vec2i>(i + 1, j + 1);

					for (m = 0; m < iPxCountX; m++)
					{
						for (n = 0; n < iPxCountY; n++)
						{
							s = (ptTR.x - ptTL.x) * m * xfrac + ptTL.x;
							t = (ptTR.y - ptTL.y) * m * xfrac + ptTL.y;
							u = (ptBR.x - ptBL.x) * m * xfrac + ptBL.x;
							v = (ptBR.y - ptBL.y) * m * xfrac + ptBL.y;

							ptTmp.x = (s + (u - s) * n * yfrac);
							ptTmp.y = (t + (v - t) * n * yfrac);

							m_OffsetPoints->at<Vec2f>(iColumnX + m - m_Boundary.x,
								iRowY + n - m_Boundary.y) = ptTmp;
						}
					}
				}
			}

		}

		void WarperCanvas::ResizeGrid(Point &ptEnd)
		{
			Rect rcNewBounds(m_Boundary);
			bool bChanged = false;
			int iChangeStep;

			// left
			while (ptEnd.x < rcNewBounds.x && rcNewBounds.x >= m_IncrementStep)
			{
				iChangeStep = std::min(rcNewBounds.x, m_IncrementStep);
				rcNewBounds.x -= iChangeStep;
				rcNewBounds.width += iChangeStep;
				bChanged = true;
			}
			// right
			while (ptEnd.x > rcNewBounds.br().x && rcNewBounds.br().x < m_MaxWidth - m_IncrementStep)
			{
				rcNewBounds.width = std::min(m_MaxWidth, rcNewBounds.width + m_IncrementStep);
				bChanged = true;
			}
			// top
			while (ptEnd.y < rcNewBounds.y && rcNewBounds.y >= m_IncrementStep)
			{
				iChangeStep = std::min(rcNewBounds.y, m_IncrementStep);
				rcNewBounds.y -= iChangeStep;
				rcNewBounds.height += iChangeStep;
				bChanged = true;
			}
			// bottom
			while (ptEnd.y > rcNewBounds.br().y && rcNewBounds.br().y < (m_MaxHeight - m_IncrementStep))
			{
				rcNewBounds.height = std::min(m_MaxHeight, rcNewBounds.height + m_IncrementStep);
				bChanged = true;
			}

			if (bChanged)
			{
				Initalize(rcNewBounds);
			}
		}
	}
}



