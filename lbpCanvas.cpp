#include "lbpCanvas.h"

namespace wrap
{
	namespace ImageInpaint
	{
		lbpCanvas::lbpCanvas()
		{}

		lbpCanvas::~lbpCanvas()
		{}

		void lbpCanvas::doInpaint(Mat &srcImage, Mat &dstImage, vector<Point> &curve, int r, LbpType lbpType, int getMask)
		{
			dstImage = srcImage.clone();
			int nRows = srcImage.rows;
			int nCols = srcImage.cols;

			/* get mask by curve point */
			Mat mask;
			PointArray2Mask(curve, mask, dstImage.size());
			Mat gray, lbp;
			cvtColor(srcImage, gray, CV_RGB2GRAY);

			/* select lbp inpaint type */
			if (lbpType == LBP_GRAY)
				computeLBPImage_Gray(gray, lbp);
			else
				computeLBPImage_Uniform(gray, lbp);
			for (int row = 0; row < nRows; ++row)
			{
				for (int col = 0; col < nCols; ++col)
				{
					if (!mask.at<unsigned char>(row, col))
						continue;
					Point near(0, 0);
					float distance = 99999;
					vector<Point>  Nbrs;
					for (int m = max(0, row - r); m < min(nRows, row + r); ++m)
					{
						int n = col;
						{
							if (mask.at<unsigned char>(m, n))
								continue;
							if (abs(lbp.at<unsigned char>(row, col) - lbp.at<unsigned char>(m, n)) > distance)
								continue;
							distance = abs(lbp.at<unsigned char>(row, col) - lbp.at<unsigned char>(m, n));
							near = Point(m, n);
						}
					}
					dstImage.at<Vec3b>(row, col) = dstImage.at<Vec3b>(near.x, near.y);
					mask.at<unsigned char>(row, col) = 0;
				}
			}

		    /* inpaint the brow around area */
			cv::Mat rgb, dst;
			Mat maskLine;
			PointArray2MaskLine(curve, maskLine, dstImage.size(),4);
			cv::inpaint(dstImage, maskLine, dstImage, 3, INPAINT_NS);
		}

		void lbpCanvas::computeLBPImage_Gray(const Mat &srcImage, Mat &lbpImage)
		{
			// 参数检查，内存分配
			CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);
			lbpImage.create(srcImage.size(), srcImage.type());

			// 扩充原图像边界，便于边界处理
			cv::Mat extendedImage;
			copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, cv::BORDER_DEFAULT);

			// 计算LBP特征图
			int heightOfExtendedImage = extendedImage.rows;
			int widthOfExtendedImage = extendedImage.cols;
			int widthOfLBP = lbpImage.cols;
			uchar *rowOfExtendedImage = extendedImage.data + widthOfExtendedImage + 1;
			uchar *rowOfLBPImage = lbpImage.data;
			for (int y = 1; y <= heightOfExtendedImage - 2; ++y, rowOfExtendedImage += widthOfExtendedImage, rowOfLBPImage += widthOfLBP)
			{
				// 列
				uchar *colOfExtendedImage = rowOfExtendedImage;
				uchar *colOfLBPImage = rowOfLBPImage;
				for (int x = 1; x <= widthOfExtendedImage - 2; ++x, ++colOfExtendedImage, ++colOfLBPImage)
				{
					// 计算LBP值
					int LBPValue = 0;
					if (colOfExtendedImage[0 - widthOfExtendedImage - 1] >= colOfExtendedImage[0])
						LBPValue += 128;
					if (colOfExtendedImage[0 - widthOfExtendedImage] >= colOfExtendedImage[0])
						LBPValue += 64;
					if (colOfExtendedImage[0 - widthOfExtendedImage + 1] >= colOfExtendedImage[0])
						LBPValue += 32;
					if (colOfExtendedImage[0 + 1] >= colOfExtendedImage[0])
						LBPValue += 16;
					if (colOfExtendedImage[0 + widthOfExtendedImage + 1] >= colOfExtendedImage[0])
						LBPValue += 8;
					if (colOfExtendedImage[0 + widthOfExtendedImage] >= colOfExtendedImage[0])
						LBPValue += 4;
					if (colOfExtendedImage[0 + widthOfExtendedImage - 1] >= colOfExtendedImage[0])
						LBPValue += 2;
					if (colOfExtendedImage[0 - 1] >= colOfExtendedImage[0])
						LBPValue += 1;

					colOfLBPImage[0] = LBPValue;

				}  // x

			}// y
		}

		void lbpCanvas::computeLBPImage_Uniform(const Mat &srcImage, Mat &lbpImage)
		{
			// 参数检查，内存分配
			CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);
			lbpImage.create(srcImage.size(), srcImage.type());

			// 计算LBP图
			// 扩充原图像边界，便于边界处理
			Mat extendedImage;
			copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_DEFAULT);

			// 构建LBP 等价模式查找表
			//int table[256];
			//BuildUniformPatternTable(table);

			// LUT(256种每一种模式对应的等价模式)
			static const int table[256] = { 1, 2, 3, 4, 5, 0, 6, 7, 8, 0, 0, 0, 9, 0, 10, 11, 12, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 14, 0, 15, 16, 17, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 20, 0, 21, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25,
				0, 0, 0, 0, 0, 0, 0, 26, 0, 0, 0, 27, 0, 28, 29, 30, 31, 0, 32, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0
				, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 36, 37, 38, 0, 39, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42
				, 43, 44, 0, 45, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 47, 48, 49, 0, 50, 0, 0, 0, 51, 52, 53, 0, 54, 55, 56, 57, 58 };

			// 计算LBP
			int heightOfExtendedImage = extendedImage.rows;
			int widthOfExtendedImage = extendedImage.cols;
			int widthOfLBP = lbpImage.cols;
			uchar *rowOfExtendedImage = extendedImage.data + widthOfExtendedImage + 1;
			uchar *rowOfLBPImage = lbpImage.data;
			for (int y = 1; y <= heightOfExtendedImage - 2; ++y, rowOfExtendedImage += widthOfExtendedImage, rowOfLBPImage += widthOfLBP)
			{
				// 列
				uchar *colOfExtendedImage = rowOfExtendedImage;
				uchar *colOfLBPImage = rowOfLBPImage;
				for (int x = 1; x <= widthOfExtendedImage - 2; ++x, ++colOfExtendedImage, ++colOfLBPImage)
				{
					// 计算LBP值
					int LBPValue = 0;
					if (colOfExtendedImage[0 - widthOfExtendedImage - 1] >= colOfExtendedImage[0])
						LBPValue += 128;
					if (colOfExtendedImage[0 - widthOfExtendedImage] >= colOfExtendedImage[0])
						LBPValue += 64;
					if (colOfExtendedImage[0 - widthOfExtendedImage + 1] >= colOfExtendedImage[0])
						LBPValue += 32;
					if (colOfExtendedImage[0 + 1] >= colOfExtendedImage[0])
						LBPValue += 16;
					if (colOfExtendedImage[0 + widthOfExtendedImage + 1] >= colOfExtendedImage[0])
						LBPValue += 8;
					if (colOfExtendedImage[0 + widthOfExtendedImage] >= colOfExtendedImage[0])
						LBPValue += 4;
					if (colOfExtendedImage[0 + widthOfExtendedImage - 1] >= colOfExtendedImage[0])
						LBPValue += 2;
					if (colOfExtendedImage[0 - 1] >= colOfExtendedImage[0])
						LBPValue += 1;

					colOfLBPImage[0] = table[LBPValue];

				} // x

			}// y
		}

		void lbpCanvas::getMask(Mat &srcMat, Mat &mask)
		{
			mask.create(srcMat.size(), CV_8UC1);
			for (int row = 0; row < srcMat.rows; ++row)
			{
				for (int col = 0; col < srcMat.cols; ++col)
				{
					if (srcMat.at<cv::Vec3b>(row, col)[2]>220 && srcMat.at<cv::Vec3b>(row, col)[1]<40 && srcMat.at<cv::Vec3b>(row, col)[0] < 80)
						mask.at<unsigned char>(row, col) = 250;
					else
						mask.at<unsigned char>(row, col) = 0;
				}
			}
		}
	}
}


