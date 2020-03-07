#ifndef WARPER_WARPER_CANVAS
#define WARPER_WARPER_CANVAS

#include "core.hpp"

namespace wrap 
{
	namespace ImageWarper
	{
		class DLLEXPORTED WarperCanvas
		{
		public:
			int           m_GridStep;
			int           m_IncrementStep;
			int           m_MaxWidth;
			int           m_MaxHeight;

			Rect          m_Boundary;
			Mat2i        *m_GridPoints;
			Mat2f        *m_OffsetPoints;

			void Initalize(Rect &rcBoundary);
			void SeletctGridStep();
			void ResizeGrid(Point &ptEnd);
			void ResetGridPos();
			void CalcOffsets();

			virtual void DoForce(Point &ptStart, Point &ptEnd, Mat2i *gridPoint, Rect &boundary) = 0;
		public:
			int    GetGridStep();
			Rect*  GetBoundary();
			Mat2i* GetGridPoints();
			Mat2f* GetOffsetPoints();

			WarperCanvas(Rect &boundary, int iMaxWidth, int iMaxHeight);
			virtual ~WarperCanvas();

			// return TRUE if the Bounding box is update.
			bool Force(Point &ptStart, Point &ptEnd);
		};
	}
}

#endif /* WARPER_WARPER_CANVAS */