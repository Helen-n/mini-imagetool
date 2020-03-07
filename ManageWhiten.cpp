#include "ManageWhiten.h"

namespace wrap
{
	namespace ImageWhiten
	{
		/*=================================================================================*/
		// common part interface
		/*=================================================================================*/
		/* whiten init */
		bool whitenInit(int width, int height, int scanWidth, int bpp, unsigned char* rawData,
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
			m_landmark = new Landmark();
			Array2Landmark(nCount, doubleArray, *m_landmark);
			m_whiten = new Whiten();
			imshow("【SRC_WHITEN】", *m_srcImage);
			if (m_srcImage && m_dstImage && m_landmark && m_whiten)
				return true;

			return false;
		}

		/* get whitened image */
		unsigned char* whitenImage(int *width, int *height, int *scanWidth, int *bpp)
		{
			if (width && height && scanWidth && m_whiten && m_dstImage)
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

		/* release the whiten */
		void whitenRelease()
		{
			if (m_whiten)
				delete m_whiten;

			if (m_srcImage)
				delete m_srcImage;

			if (m_dstImage)
				delete m_dstImage;

			if (m_landmark)
				delete m_landmark;
		}

		/*=================================================================================*/
		// special usage interface 
		/*=================================================================================*/
		/* whiten the face */
	    void whitenFace(double strength, double rosyDegree, int range)
		{
			*m_dstImage = m_srcImage->clone();

			// 在界面前定义处理的区域
			// 在界面后进行处理

			//// 第一部分，针对额头上部区域进行美白
			//Mat temp = (*m_dstImage)(Rect(x, y, w, h));
			//Mat mask;
			//skinMask(&temp, mask);
			//Mat temp2 = (*m_dstImage)(Rect(x, y, w, h));
			//for (int row = 0; row < mask.rows; row++)
			//{
			//	for (int col = 0; col < mask.cols; col++)
			//	{
			//		if (mask.at<unsigned char>(row, col) == 255)
			//			whiten->doWhiten(temp2.at<cv::Vec3b>(row, col), strength);
			//	}
			//}
			//Mat laplaian;
			//Mat elemnt2 = getStructuringElement(MORPH_RECT, Size(6, 6));
			//Laplacian(mask, laplaian, mask.depth());
			//Mat tmpMask(m_dstImage->size(), CV_8UC1, Scalar(0));
			//laplaian.copyTo(tmpMask(Rect(x, y, w, h)));
			//dilate(tmpMask, tmpMask, elemnt2);

			//InseSmooth(m_dstImage, tmpMask);

			//// 第二部分，根据传回来的点，进行美白
			//vector<vector<Point2i> >outCurve;
			//vector<Point2i> curve;
			//Mat maskFill(m_dstImage->size(), CV_8UC1, Scalar(0));
			//Mat maskLine(m_dstImage->size(), CV_8UC1, Scalar(0));
			//for (int i = 0; i < nCount; i += 2)
			//{
			//	Point2i point(intArray[i], intArray[i + 1]);
			//	curve.push_back(point);
			//}
			//outCurve.push_back(curve);

			//drawContours(maskFill, outCurve, -1, Scalar(255), CV_FILLED);
			//for (int row = 0; row < maskFill.rows; ++row)
			//{
			//	for (int col = 0; col < maskFill.cols; ++col)
			//	{
			//		if (maskFill.at<unsigned char>(row, col))
			//			whiten->doWhiten(m_dstImage->at<cv::Vec3b>(row, col), strength);
			//	}
			//}
			//Mat tmpDst = m_srcImage->clone();


			//drawContours(maskLine, outCurve, -1, Scalar(255));
			//
			//doLineSmooth(m_dstImage, maskLine, cv::Point(240, 370), 30, 30);
		}

		/* whiten the all image */
		void whitenAll(double strength, double rosyDegree)
		{
			*m_dstImage = m_srcImage->clone();
			m_whiten->doWhiten(*m_dstImage, *m_dstImage, strength);
			m_whiten->doRosy(*m_dstImage, *m_dstImage, rosyDegree);
			imshow("【DST_WHITEN_ALL】", *m_dstImage);
		}

		/*=================================================================================*/
		// common usage interface
		/*=================================================================================*/
		/* whiten by the point*/
		void whitenPoint(int x, int y, double strength, double rosyDegree, int range)
		{
			*m_dstImage = m_srcImage->clone();

			int nRows = m_dstImage->rows;
			int nCols = m_dstImage->cols;

			for (int row = max(x - range / 2, 0); row < min(x + range / 2, nRows); row++)
			{
				for (int col = max(y - range / 2, 0); col < min(y + range / 2, nCols); col++)
				{
					double distance = (row - x)*(row - x) + (col - y)*(col - y);
					double alpha = (1 - distance * 8 / (range * range)) * strength;
					if (distance * 8 < (range * range))
						m_whiten->doWahitenPoint( m_dstImage->at<Vec3b>(row, col), alpha);						
				}
			}
			m_whiten->doRosy(*m_dstImage, *m_dstImage, rosyDegree);
			imshow("【DST_WHITEN_POINT】", *m_srcImage);
		}
	}
}
