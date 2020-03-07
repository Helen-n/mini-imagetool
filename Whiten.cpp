#include "Whiten.h"

namespace wrap
{
	namespace ImageWhiten
	{
		/*-------------------------------------------------------------------------------*/
		// common utilities
		/*-------------------------------------------------------------------------------*/
		Whiten::Whiten()
		{
			m_whiten = new WhitenCanvas();
		}

		Whiten:: ~Whiten()
		{
			if (m_whiten)
				delete m_whiten;
		}

		void Whiten::doWahitenPoint(Vec3b &p, int strength)
		{
			m_whiten->doWhiten(p,  strength);
		}

		/*--------------------------------------------------------------------------*/
		// public interface
		/*--------------------------------------------------------------------------*/

		void Whiten::doWhiten(Mat &srcImage, Mat &dstImage, double strength)
		{
			dstImage = srcImage.clone();
			int nRows = srcImage.rows;
			int nCols = srcImage.cols;
			for (int row = 0; row < nRows; ++row)
			{
				for (int col = 0; col < nCols; ++col)
					m_whiten->doWhiten(dstImage.at<cv::Vec3b>(row,col), strength);
			}

		}

		void Whiten::doRosy(Mat &srcImage, Mat &dstImage, double rosyDegree)
		{
			Mat HSV;
			int nRows = srcImage.rows;
			int nCols = srcImage.cols;
			cvtColor(srcImage, HSV, CV_RGB2HSV);

			for (int row = 0; row < nRows; ++row)
			{
				for (int col = 0; col < nCols; ++col)
					HSV.at<cv::Vec3b>(row, col)[1] += rosyDegree;
			}
			cvtColor(HSV, dstImage, CV_HSV2RGB);			
		}
	}
}


