#include "Warper.h"
#include "TranslateCanvas.h"
#include "ShrinkGrowCanvas.h"

namespace wrap
{
	namespace ImageWarper
	{
		/*-------------------------------------------------------------------------------*/
		// common utilities
		/*-------------------------------------------------------------------------------*/

		Warper::Warper(Mat &img)
			:m_canvas(0),
			m_bEdgeBlurring(false)
		{
			m_imgOriginal = new Mat(img);
			m_WarperImage = new WarperImage();
		}

		Warper::Warper(Mat *img)
			:m_canvas(0),
			m_bEdgeBlurring(false),
			m_imgOriginal(img)
		{
			m_WarperImage = new WarperImage();
		}

		Warper::~Warper()
		{
			if (m_imgOriginal)
				delete m_imgOriginal;
			if (m_WarperImage)
				delete m_WarperImage;
		}

		void Warper::UpdateWarp(Point &stPoint, Point edPoint, int brushSize, WarperType warperType, DerectionType derection)
		{
			Point ptTopLeft(max(0, stPoint.x - 2 * brushSize),max(0, stPoint.y - 2 * brushSize));
			Rect bound(ptTopLeft.x, ptTopLeft.y,
				min(m_imgOriginal->cols - ptTopLeft.x, 4 * brushSize),
				min(m_imgOriginal->rows - ptTopLeft.y, 4 * brushSize));
			m_ptCenterPos = stPoint;

			// create the canvas
			if (m_canvas)
				delete m_canvas;
			switch (warperType)
			{
			case WARPER_TRANSLATE:
				m_canvas = new TranslateCanvas(bound, m_imgOriginal->cols, m_imgOriginal->rows);
				break;
			case WARPER_GROW:
				m_canvas = new ShrinkGrowCanvas(bound, m_imgOriginal->cols, m_imgOriginal->rows, true, derection);
				break;
			case WARPER_SHRINK:
				m_canvas = new ShrinkGrowCanvas(bound, m_imgOriginal->cols, m_imgOriginal->rows, false, derection);
				break;
			default:
				assert(false);
			}
			m_WarperImage->position.x = bound.x;
			m_WarperImage->position.y = bound.y;
			Mat3b matTmp(Size(bound.width, bound.height));
			m_WarperImage->imagedata = matTmp.clone();
			if (!m_canvas)
				return;
			if (m_canvas->Force(m_ptCenterPos, edPoint))
			{
				Rect canvasBounds = *m_canvas->GetBoundary();
				m_WarperImage->position.x = canvasBounds.x;
				m_WarperImage->position.y = canvasBounds.y;

				Mat3b matTmp(Size(canvasBounds.width, canvasBounds.height));
				m_WarperImage->imagedata = matTmp.clone();
			}
			Warper::OffsetFilter(m_imgOriginal, m_canvas->GetOffsetPoints(), &m_WarperImage->imagedata);

			assert(m_WarperImage->imagedata.type() == m_imgOriginal->type());
			// change code here!
			Warper::FilterSmooth(m_imgOriginal, m_canvas->GetOffsetPoints(), &m_WarperImage->imagedata);

			if (m_bEdgeBlurring)
				EdgeBlur(m_imgOriginal, m_canvas->GetBoundary(), &m_WarperImage->imagedata);

			Mat &imgWarped = m_WarperImage->imagedata;
			int iLeftBound = m_WarperImage->position.x;
			int iTopBound = m_WarperImage->position.y;

			m_WarperImage->imagedata.copyTo((*m_imgOriginal)(Rect(iLeftBound, iTopBound, imgWarped.cols, imgWarped.rows)));
		}
		
		void Warper::GetImage(Mat &mat)
		{
			mat = m_imgOriginal->clone();
		}

		/*-------------------------------------------------------------------------------*/
		// private utilities
		/*-------------------------------------------------------------------------------*/

		bool Warper::OffsetFilter(Mat *bmpOriginal, Mat2f *offset, Mat *bmpNew)
		{
			int nwidth = bmpNew->cols;
			int nheight = bmpNew->rows;

			int xOffset, yOffset;

			for (int x = 0; x < nwidth; ++x)
			{
				for (int y = 0; y < nheight; ++y)
				{
					Point2f ptTmp = offset->at<Vec2f>(x, y);
					xOffset = std::min(std::max(0, (int)ptTmp.x), bmpOriginal->rows - 1);
					yOffset = std::min(std::max(0, (int)ptTmp.y), bmpOriginal->cols - 1);
					bmpNew->at<Vec3b>(x, y) = bmpOriginal->at<Vec3b>(xOffset, yOffset);
				}
			}
			return true;
		}
		
		bool Warper::FilterSmooth(Mat *bmpOriginal, Mat2f *offset, Mat *bmpNew)
		{
			int nwidth = bmpNew->cols;
			int nheight = bmpNew->rows;
			float fXOffset, fYOffset, dXLeft, dXRight, dYTop, dYBottom;
			int xOffsetLeft, xOffsetRight, yOffsetTop, yOffsetBottom;

			unsigned char px00, px01, px10, px11;
			float fPx0, fPx1;

			for (int y = 0; y < nheight; ++y)
			{
				for (int x = 0; x < nwidth; ++x)
				{
					Point2f ptTmp = offset->at<Vec2f>(x, y);
					fXOffset = ptTmp.x;
					fYOffset = ptTmp.y;

					xOffsetLeft = min(max((int)floor(fXOffset), 0), bmpOriginal->cols - 1);
					xOffsetRight = min(max(xOffsetLeft + 1, 0), bmpOriginal->cols - 1);
					yOffsetTop = min(max((int)floor(fYOffset), 0), bmpOriginal->rows - 1);
					yOffsetBottom = min(std::max(yOffsetTop + 1, 0), bmpOriginal->rows - 1);

					if (fXOffset < xOffsetLeft || fXOffset > xOffsetRight)
					{
						fXOffset = (xOffsetLeft + xOffsetRight) / 2.0f;
					}
					if (fYOffset < yOffsetTop || fYOffset > yOffsetBottom)
					{
						fYOffset = (yOffsetTop + yOffsetBottom) / 2.0f;
					}

					dXLeft = fXOffset - xOffsetLeft;
					dXRight = xOffsetRight - fXOffset;
					if (dXLeft == 0 && dXRight == 0)
						dXLeft = 1;

					dYTop = fYOffset - yOffsetTop;
					dYBottom = yOffsetBottom - fYOffset;
					if (dYTop == 0 && dYBottom == 0)
						dYTop = 1;

					for (int k = 3 - 1; k >= 0; --k)
					{
						px00 = (int)(bmpOriginal->at<Vec3b>(yOffsetTop, xOffsetLeft)[k]);
						px01 = (int)(bmpOriginal->at<Vec3b>(yOffsetTop, xOffsetRight)[k]);
						px10 = (int)(bmpOriginal->at<Vec3b>(yOffsetBottom, xOffsetLeft)[k]);
						px11 = (int)(bmpOriginal->at<Vec3b>(yOffsetBottom, xOffsetRight)[k]);

						fPx0 = px01 * dXLeft + px00 * dXRight;
						fPx1 = px11 * dXLeft + px10 * dXRight;

						bmpNew->at<Vec3b>(y, x)[k] = (unsigned char)(fPx0 * (dYBottom)+fPx1 * (dYTop));
					}
				}
			}
			return true;
		}

		void Warper::EdgeBlur(Mat *bmpOriginal, Rect *bounds, Mat *bmpNew)
		{
			assert(bmpOriginal->type() == bmpNew->type());
			int iBbp = 3;// bmpOriginal->type();
			int iSrcwidth = bmpOriginal->cols - 1;
			int iSrcheight = bmpOriginal->rows - 1;
			float fSum;

#define FILTER_SUM(y, x, k)	(bmpOriginal->at<Vec3b>(std::max(0, y - 1),std::max(0, x - 1))[k] + \
								bmpOriginal->at<Vec3b>(std::max(0, y - 1), std::min(iSrcwidth, x + 1))[k] + \
								bmpOriginal->at<Vec3b>(std::min(iSrcheight, y + 1), std::max(0, x - 1))[k] + \
								bmpOriginal->at<Vec3b>(std::min(iSrcheight, y + 1) , std::min(iSrcwidth, x + 1))[k] + \
							 2*(bmpOriginal->at<Vec3b>(std::max(0, y - 1), x)[k] + \
								bmpOriginal->at<Vec3b>(std::min(iSrcheight, y + 1), x)[k] + \
								bmpOriginal->at<Vec3b>(y, std::max(0, x - 1))[k] + \
								bmpOriginal->at<Vec3b>(y, std::min(iSrcwidth, x + 1))[k]) + \
							  4*bmpOriginal->at<Vec3b>(y, x)[k])

			for (int j = 1; j >= 0; --j)
			{
				int iTopY = bounds->y + j;
				int iBottomY = bounds->br().y - j;

				// 2 top and bottom rows 
				for (int i = bounds->br().x - 1; i >= bounds->x; --i)
				{
					for (int k = 0; k < iBbp; ++k)
					{
						fSum = (float)FILTER_SUM(iTopY, i, k);

						bmpNew->at<Vec3b>(j, (i - bounds->x))[k] = (unsigned char)(fSum / 16.0f);

						fSum = (float)FILTER_SUM(iBottomY, i, k);
						bmpNew->at<Vec3b>((bounds->height - j), (i - bounds->x))[k] = (unsigned char)(fSum / 16.0f);
					}
				}

				int iLeftX = bounds->x + j;
				int iRightX = bounds->br().x - j;

				// 2 left and right columns
				for (int i = bounds->br().y - 1; i >= bounds->y; --i)
				{
					for (int k = 0; k < iBbp; ++k)
					{
						fSum = (float)FILTER_SUM(i, iLeftX, k);

						bmpNew->at<Vec3b>((i - bounds->y), j)[k] = (unsigned char)(fSum / 16.0f);

						fSum = (float)FILTER_SUM(i, iRightX, k);

						bmpNew->at<Vec3b>((i - bounds->y), (bounds->width - j))[k] = (unsigned char)(fSum / 16.0f);
					}
				}
			}
		}


		/*--------------------------------------------------------------------------*/
		// public interface
		/*--------------------------------------------------------------------------*/

		/* interface of warper face */
		void Warper::doWarperFace(int curveStart, int curveEnd,int brushSize, double strength, LineParamer &paramer, vector<Point> &curve)
		{
			int maxLength = curve.size()-1;
			for (int k = curveStart; k < curveEnd; ++k)
			{
				double alpha = sin(CV_PI * ((double)k / maxLength));
				Point startPoint = curve[k];
				Point offsetPoint;
				CalOffset(paramer, startPoint, offsetPoint, alpha, strength, 1.2);
				UpdateWarp(startPoint, offsetPoint, brushSize, WARPER_TRANSLATE);
			}
		}
		
		/* interface of warperfill */
		void Warper::doWarperFill(int brushSize, double strength, Point centerPoint, DerectionType derection)
		{
			UpdateWarp(centerPoint, centerPoint, brushSize*strength, WARPER_GROW, derection);
		}

		void Warper::doWarperNoseWarper( int brushSize, double strength, vector<double> controlF, LineParamer &paramer, vector<Point> &curve)
		{	
			int maxLength = curve.size() - 1;
			for (int k = 0; k < curve.size(); ++k)
			{
				Point startPoint = curve[k];
				Point offsetPoint;
				CalOffset(paramer, startPoint, offsetPoint, controlF[k], strength, 1.2);
				UpdateWarp(startPoint, offsetPoint, brushSize, WARPER_TRANSLATE);
			}
		}

		void Warper::doWarperNoseShrink(int brushSize, double strength, Point centerPoint)
		{
			UpdateWarp(centerPoint, centerPoint, brushSize*strength, WARPER_SHRINK);
		}

		//bool Warper::FilterSmooth2(Mat *bmpOriginal, Mat2f *offset, Mat *bmpNew)
		//{
		//// stride
		//int scanline = bmpOriginal->Scanwidth;
		//unsigned char* p = (unsigned char*)bmpNew->data;
		//unsigned char* pSrc = (unsigned char*)bmpOriginal->data;
		//int nOffset = bmpNew->Scanwidth - bmpNew->width * bmpNew.type();
		//int nwidth = bmpNew->width;
		//int nheight = bmpNew->height;
		//float fXOffset, fYOffset, dXLeft, dXRight, dYTop, dYBottom;
		//int xOffsetLeft, xOffsetRight, yOffsetTop, yOffsetBottom;
		//int srcOffset00, srcOffset01, srcOffset02, srcOffset03;
		//int srcOffset10, srcOffset11, srcOffset12, srcOffset13;
		//int srcOffset20, srcOffset21, srcOffset22, srcOffset23;
		//int srcOffset30, srcOffset31, srcOffset32, srcOffset33;
		//unsigned char px00, px01, px02, px03;
		//unsigned char px10, px11, px12, px13;
		//unsigned char px20, px21, px22, px23;
		//unsigned char px30, px31, px32, px33;
		//float fPx0, fPx1;
		//for (int y = 0; y < nheight; ++y)
		//{
		//for (int x = 0; x < nwidth; ++x)
		//{
		//fXOffset = offset->GetItem(x, y)->X;
		//fYOffset = offset->GetItem(x, y)->Y;
		//xOffsetLeft = std::min(std::max((int)floor(fXOffset - 1), 0), bmpOriginal->width - 1);
		//xOffsetRight = std::min(std::max(xOffsetLeft + 2, 0), bmpOriginal->width - 1);
		//yOffsetTop = std::min(std::max((int)floor(fYOffset - 1), 0), bmpOriginal->height - 1);
		//yOffsetBottom = std::min(std::max(yOffsetTop + 2, 0), bmpOriginal->height - 1);
		//srcOffset00 = (yOffsetTop * scanline) + (xOffsetLeft * bmpOriginal.type());
		//srcOffset01 = (yOffsetTop * scanline) + ((xOffsetLeft + 1) * bmpOriginal.type());
		//srcOffset02 = (yOffsetTop * scanline) + ((xOffsetLeft + 2) * bmpOriginal.type());
		//srcOffset03 = (yOffsetTop * scanline) + (xOffsetRight * bmpOriginal.type());
		//srcOffset10 = ((yOffsetTop + 1) * scanline) + (xOffsetLeft * bmpOriginal.type());
		//srcOffset11 = ((yOffsetTop + 1) * scanline) + ((xOffsetLeft + 1)  * bmpOriginal.type());
		//srcOffset12 = ((yOffsetTop + 1) * scanline) + ((xOffsetLeft + 2) * bmpOriginal.type());
		//srcOffset13 = ((yOffsetTop + 1) * scanline) + (xOffsetRight * bmpOriginal.type());
		//srcOffset20 = ((yOffsetTop + 2) * scanline) + (xOffsetLeft * bmpOriginal.type());
		//srcOffset21 = ((yOffsetTop + 2) * scanline) + ((xOffsetLeft + 1)  * bmpOriginal.type());
		//srcOffset22 = ((yOffsetTop + 2) * scanline) + ((xOffsetLeft + 2) * bmpOriginal.type());
		//srcOffset23 = ((yOffsetTop + 2) * scanline) + (xOffsetRight * bmpOriginal.type());
		//srcOffset30 = (yOffsetBottom * scanline) + (xOffsetLeft * bmpOriginal.type());
		//srcOffset31 = (yOffsetBottom * scanline) + ((xOffsetLeft + 1)  * bmpOriginal.type());
		//srcOffset32 = (yOffsetBottom * scanline) + ((xOffsetLeft + 2) * bmpOriginal.type());
		//srcOffset33 = (yOffsetBottom * scanline) + (xOffsetRight * bmpOriginal.type());
		//if (fXOffset < xOffsetLeft || fXOffset > xOffsetRight)
		//{
		//fXOffset = (xOffsetLeft + xOffsetRight) / 2.0f;
		//}
		//if (fYOffset < yOffsetTop || fYOffset > yOffsetBottom)
		//{
		//fYOffset = (yOffsetTop + yOffsetBottom) / 2.0f;
		//}
		//dXLeft = fXOffset - xOffsetLeft;
		//dXRight = xOffsetRight - fXOffset;
		//if (dXLeft == 0 && dXRight == 0)
		//dXLeft = 1;
		//dYTop = fYOffset - yOffsetTop;
		//dYBottom = yOffsetBottom - fYOffset;
		//if (dYTop == 0 && dYBottom == 0)
		//dYTop = 1;
		//int intXX, intYY;
		//intXX = floor(fXOffset - 1);
		//intYY = floor(fYOffset - 1);
		//float u, v;
		//u = fXOffset - 1 - intXX;
		//v = fYOffset - 1 - intYY;
		//float u0, u1, u2, u3;
		//float v0, v1, v2, v3;
		//u0 = 1 + u;
		//u1 = u;
		//u2 = 1 - u;
		//u3 = 2 - u;
		//v0 = 1 + v;
		//v1 = v;
		//v2 = 1 - v;
		//v3 = 2 - v;
		//u0 = 4 - 8 * u0 + 5 * u0*u0 - u0*u0*u0;
		//u1 = 1 - 2 * u1*u1 + u1*u1*u1;
		//u2 = 1 - 2 * u2*u2 + u2*u2*u2;
		//u3 = 4 - 8 * u3 + 5 * u3*u3 - u3*u3*u3;
		//v0 = 4 - 8 * v0 + 5 * v0*v0 - v0*v0*v0;
		//v1 = 1 - 2 * v1*v1 + v1*v1*v1;
		//v2 = 1 - 2 * v2*v2 + v2*v2*v2;
		//v3 = 4 - 8 * v3 + 5 * v3*v3 - v3*v3*v3;
		//for (int k = bmpOriginal->type() - 1; k >= 0; --k)
		//{
		//px00 = pSrc[srcOffset00 + k];
		//px01 = pSrc[srcOffset01 + k];
		//px02 = pSrc[srcOffset02 + k];
		//px03 = pSrc[srcOffset03 + k];
		//px10 = pSrc[srcOffset10 + k];
		//px11 = pSrc[srcOffset11 + k];
		//px12 = pSrc[srcOffset12 + k];
		//px13 = pSrc[srcOffset13 + k];
		//px20 = pSrc[srcOffset20 + k];
		//px21 = pSrc[srcOffset21 + k];
		//px22 = pSrc[srcOffset22 + k];
		//px23 = pSrc[srcOffset23 + k];
		//px30 = pSrc[srcOffset30 + k];
		//px31 = pSrc[srcOffset31 + k];
		//px32 = pSrc[srcOffset32 + k];
		//px33 = pSrc[srcOffset33 + k];
		////fPx0 = px01 * dXLeft + px00 * dXRight;
		////fPx1 = px11 * dXLeft + px10 * dXRight;
		////p[k] = (unsigned char)(fPx0 * (dYBottom)+fPx1 * (dYTop));
		//float g0, g1, g2, g3;
		//g0 = v0*px00 + v1*px10 + v2*px20 + v3*px30;
		//g1 = v0*px01 + v1*px11 + v2*px21 + v3*px31;
		//g2 = v0*px02 + v1*px12 + v2*px22 + v3*px32;
		//g3 = v0*px03 + v1*px13 + v2*px23 + v3*px33;
		//float s;
		//s = g0*u0 + g1*u1 + g2*u2 + g3*u3;
		//if (s <= 0) s = 0;
		//if (s >= 255) s = 255;
		//p[k] = (unsigned char)s;
		//}
		//p += bmpNew.type();
		//}
		//p += nOffset;
		//}
		//return true;
		//}
	}
}
