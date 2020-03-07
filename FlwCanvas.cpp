#include "FlwCanvas.h"

namespace wrap
{
	namespace ImageBlur
	{
		FlwCanvas::FlwCanvas()
		{}

		FlwCanvas::~FlwCanvas()
		{}

		void FlwCanvas::doFlwBlur(const Mat &srcImage, Mat &dstImage, Rect &roi)
		{
			dstImage = srcImage.clone();

			Mat temp = dstImage;
			Mat flw = temp(roi);
			Mat gray, binary;
			cvtColor(flw, gray, COLOR_BGR2GRAY);
			threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

			int dx = flw.cols / 4;
			for (int row = 0; row < binary.rows; row++) {
				for (int col = 0; col < dx; col++) {
					int p = binary.at<uchar>(row, col);
					if (p == 0) {
						binary.at<uchar>(row, col) = 255;
					}
				}
				for (int col = (binary.cols - dx); col < binary.cols; col++) {
					int p = binary.at<uchar>(row, col);
					if (p == 0) {
						binary.at<uchar>(row, col) = 255;
					}
				}
			}

			Mat mask1;
			bitwise_not(binary, mask1);

			// 实现高斯双边模糊
			Mat dst;
			bilateralFilter(flw, dst, 15, 150, 15);

			// 反向叠加处理， 根据mask
			Mat result1 = Mat::zeros(flw.size(), flw.type());
			add(flw, result1, result1, mask1);

			Mat result2 = Mat::zeros(flw.size(), flw.type());
			add(dst, result1, result2, binary);

			// 输出最终效果图
			Mat result;
			add(result1, result2, result);
			Mat copy = dstImage.clone();

			result.copyTo(flw);

			// 全局高斯mask
			Mat mask = Mat::zeros(dstImage.size(), CV_8UC1);
			Rect mroi = Rect();
			mroi.x = roi.x + 3;
			mroi.y = roi.y + 3;
			mroi.width = roi.width - 6;
			mroi.height = roi.height - 6;

			// 生成模板系数
			rectangle(mask, mroi, Scalar(255), -1, 8, 0);
			GaussianBlur(mask, mask, Size(5, 5), 0, 0);

			double w = 0;
			int r1 = 0, g1 = 0, b1 = 0;
			int r2 = 0, g2 = 0, b2 = 0;
			int r = 0, g = 0, b = 0;

			// 羽化混合重建图像
			for (int row = 0; row < dstImage.rows; row++) {
				for (int col = 0; col < dstImage.cols; col++) {
					int p = mask.at<uchar>(row, col);
					w = p / 255.0;
					b1 = copy.at<Vec3b>(row, col)[0];
					g1 = copy.at<Vec3b>(row, col)[1];
					r1 = copy.at<Vec3b>(row, col)[2];

					b2 = dstImage.at<Vec3b>(row, col)[0];
					g2 = dstImage.at<Vec3b>(row, col)[1];
					r2 = dstImage.at<Vec3b>(row, col)[2];

					r = static_cast<int>(r1*(1.0 - w) + r2*w);
					g = static_cast<int>(g1*(1.0 - w) + g2*w);
					b = static_cast<int>(b1*(1.0 - w) + b2*w);

					dstImage.at<Vec3b>(row, col)[0] = b;
					dstImage.at<Vec3b>(row, col)[1] = g;
					dstImage.at<Vec3b>(row, col)[2] = r;
				}
			}

			Mat shape = (Mat_<float>(3, 3) << 1, -1.5, 1, -1.5, 3, -1.5, 1, -1.5, 1);
			filter2D(dstImage(roi), dstImage(roi), dstImage.depth(), shape);
		}
	}
}

