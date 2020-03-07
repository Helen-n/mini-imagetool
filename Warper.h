#ifndef WARPER_WARPER
#define WARPER_WARPER

#include "core.hpp"
#include "warperCanvas.h"

namespace wrap
{
	namespace ImageWarper
	{
		class DLLEXPORTED Warper
		{
			Point           m_ptCenterPos;
			Mat             *m_imgOriginal;
			WarperCanvas   *m_canvas;
			WarperImage    *m_WarperImage;
			bool            m_bEdgeBlurring;
			 
			static bool OffsetFilter(Mat *bmpOriginal, Mat2f *offset, Mat *bmpNew);

			static bool FilterSmooth(Mat *bmpOriginal, Mat2f *offset, Mat *bmpNew);

			//static bool FilterSmooth2(Mat *bmpOriginal, Mat2i *offset, Mat *bmpNew);

			static void EdgeBlur(Mat *bmpOriginal, Rect *bounds, Mat *bmpNew);

		public:

			Warper(Mat &img);

			Warper(Mat *img);

			virtual ~Warper();

			void GetImage(Mat &mat);

			void UpdateWarp(Point &stPoint, Point edPoint, int brushSize, WarperType warperType, DerectionType derection=DERECTION_ALL);

		public:

			void doWarperFace(int curveStart, int curveEnd, int brushSize, double strength, LineParamer &paramer, vector<Point> &curve);

			void doWarperNoseShrink(int brushSize, double strength, Point centerPoint);

			void doWarperNoseWarper(int brushSize, double strength, vector<double> controlF, LineParamer &paramer, vector<Point> &curve);

			void doWarperFill(int brushSize, double strength, Point centerPoint, DerectionType derection = DERECTION_ALL);

		};
	}
}

#endif /* WARPER_WARPER */


