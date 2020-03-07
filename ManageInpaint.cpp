#include "ManageInpaint.h"

namespace wrap
{
	namespace ImageInpaint
	{
		/*=================================================================================*/
		// common part interface
		/*=================================================================================*/
		bool inpaintInit(int width, int height, int scanWidth, int bpp, unsigned char* rawData,
			int nCount, const double* doubleArray[])
		{
			m_channels = bpp;
			if (m_channels == 4)
			{
				m_srcImage = new Mat(height, width, CV_8UC4, rawData, scanWidth);
				cvtColor(*m_srcImage, *m_srcImage, CV_RGBA2RGB);
				m_dstImage = new Mat(height, width, CV_8UC4, rawData, scanWidth);
				cvtColor(*m_dstImage, *m_dstImage, CV_RGBA2RGB);
			}
			else
			{
				m_srcImage = new Mat(height, width, CV_8UC3, rawData, scanWidth);
				m_dstImage = new Mat(height, width, CV_8UC3, rawData, scanWidth);
			}
			imshow("¡¾SRC_INPAINT¡¿",*m_srcImage);
			m_landmark = new Landmark();
			Array2Landmark(nCount, doubleArray, *m_landmark);

			m_inpaint = new Inpaint();

			if (m_srcImage && m_dstImage && m_landmark)
				return true;

			return false;
		}

		unsigned char* inpaintImage(int *width, int *height, int *scanWidth, int *bpp)
		{
			if (width && height && scanWidth && m_inpaint && m_dstImage)
			{
				if (m_channels == 4)
					cvtColor(*m_dstImage, *m_dstImage, CV_RGB2RGBA);

				*bpp = m_channels;
				*width = m_dstImage->cols;
				*height = m_dstImage->rows;
				*scanWidth = m_dstImage->step;
				return m_dstImage->data;
			}
			return NULL;
		}

		void inpaintRelease()
		{
			if (m_inpaint)
				delete m_inpaint;

			if (m_srcImage)
				delete m_srcImage;

			if (m_dstImage)
				delete m_dstImage;
		}

		/*=================================================================================*/
		// special usage interface 
		/*=================================================================================*/
		void inpaintBrow(int r, LbpType lbpType)
		{
			if (m_inpaint && m_srcImage)
			{
				*m_dstImage = m_srcImage->clone();

				vector<Point> curveLeft;
				curveLeft.push_back(m_landmark->left_eyebrow_left_corner);
				curveLeft.push_back(m_landmark->left_eyebrow_lower_left_quarter);
				curveLeft.push_back(m_landmark->left_eyebrow_lower_middle);
				curveLeft.push_back(m_landmark->left_eyebrow_lower_right_quarter);
				curveLeft.push_back(m_landmark->left_eyebrow_right_corner);
				curveLeft.push_back(m_landmark->left_eyebrow_upper_right_quarter);
				curveLeft.push_back(m_landmark->left_eyebrow_upper_middle);
				curveLeft.push_back(m_landmark->left_eyebrow_upper_left_quarter);
				m_inpaint->doInapintLbp(*m_dstImage, *m_dstImage, curveLeft, r, lbpType, -1);

				vector<Point> curveRight;
				curveRight.push_back(m_landmark->right_eyebrow_left_corner);
				curveRight.push_back(m_landmark->right_eyebrow_lower_left_quarter);
				curveRight.push_back(m_landmark->right_eyebrow_lower_middle);
				curveRight.push_back(m_landmark->right_eyebrow_lower_right_quarter);
				curveRight.push_back(m_landmark->right_eyebrow_right_corner);
				curveRight.push_back(m_landmark->right_eyebrow_upper_right_quarter);
				curveRight.push_back(m_landmark->right_eyebrow_upper_middle);
				curveRight.push_back(m_landmark->right_eyebrow_upper_left_quarter);
				m_inpaint->doInapintLbp(*m_dstImage, *m_dstImage, curveRight, r, lbpType, -1);

			}
		}

		/*=================================================================================*/
		// common usage interface
		/*=================================================================================*/
		void inpaintRect(int x, int y, int w, int h, int r, LbpType lbpType)
		{
			if (m_inpaint && m_srcImage)
			{
				*m_dstImage = m_srcImage->clone();

				vector<Point> curve;
				curve.push_back(Point(x, y));
				curve.push_back(Point(x, y + h));
				curve.push_back(Point(x + w, y + h));
				curve.push_back(Point(x + w, y));

				m_inpaint->doInapintLbp(*m_dstImage, *m_dstImage, curve, r, lbpType, -1);
			}
			imshow("¡¾DST_INPAINT_RECT¡¿", *m_dstImage);
		}

		void inpaintPoint(int x, int y, int r)
		{
			if (m_inpaint && m_srcImage)
			{
			   *m_dstImage = m_srcImage->clone();
				m_inpaint->doInpaint(*m_dstImage, *m_dstImage, Point(x, y), r);
			}
			imshow("¡¾DST_INPAINT_POINT¡¿", *m_dstImage);
		}

	}
}







