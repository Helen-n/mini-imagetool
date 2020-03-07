#ifndef WRAPER_CORE_BASE_H
#define WRAPER_CORE_BASE_H

#include <opencv2/core/core.hpp>
#include "cstruct.hpp"

namespace wrap
{
	/* max of three int */
	static int Max3(int a, int b, int c)
	{
		return (a > b) ? max(a,c) : max(b,c);
	}

	/* min of three int */
	static int Min3(int a, int b, int c)
	{
		return (a < b) ? min(a, c) : min(b, c);
	}

	/* clip range*/
	static int ClipRange(int value, int min, int max)
	{
		return (value > max) ? (max) : (value < min ? min : value);
	}

	/* color range 0-255 */
	static int ColorRange(int value)
	{
		return ClipRange(value, 0, 255);
	}

	static float DistanceSqr(const Point &pt1, int x, int y)
	{
		float d = (float)(pt1.x - x) * (pt1.x - x);
		d += (pt1.y - y) * (pt1.y - y);
		return d;
	}

	/* calculate distance*/
	static float DistanceSqr(const Point &pt1, const Point &pt2)
	{
		return DistanceSqr(pt1, pt2.x, pt2.y);
	}

	static float Distance(const Point &pt1, int x, int y)
	{
		return (float)sqrt(DistanceSqr(pt1, x, y));
	}

	static float Distance(const Point &pt1, const Point &pt2)
	{
		return (float)sqrt(DistanceSqr(pt1, pt2.x, pt2.y));
	}



	/* two linear interpolation */
	static void InsertSmooth(const Mat *src, const Mat &mask, Mat &dst)
	{
		dst = src->clone();
		unsigned char px00, px01, px10, px11;
		float fPx0, fPx1;
		int nRows = src->rows;
		int nCols = src->cols;

		for (int col = 0; col < nCols - 1; ++col)
		{
			for (int row = 0; row < nRows - 1; ++row)
			{
				if (!mask.at<unsigned char>(row, col))
					continue;

				for (int k = 3 - 1; k >= 0; --k)
				{
					px00 = (int)(src->at<cv::Vec3b>(max(row - 1, 0), max(col - 1, 0))[k]);
					px01 = (int)(src->at<cv::Vec3b>(max(row - 1, 0), min(col + 1, nCols))[k]);
					px10 = (int)(src->at<cv::Vec3b>(min(row + 1, nRows), max(col - 1, 0))[k]);
					px11 = (int)(src->at<cv::Vec3b>(min(row + 1, nRows), min(col + 1, nCols))[k]);

					fPx0 = px01 * 0.5 + px00 * 0.5;
					fPx1 = px11 * 0.5 + px10 * 0.5;

					dst.at<cv::Vec3b>(row, col)[k] = (unsigned char)(fPx0 * (0.5) + fPx1 * (0.5));
				}
			}
		}
	}

	/* connect the landmark with UI */
	/* good way to solve this problem is try to sent a struct from UI to DLL*/
	static void Array2Landmark(int nCount,  const double *doubleArray[], Landmark &landmark)
	{
		landmark.contour_chin = Point2d(*doubleArray[0], *doubleArray[1]);
		landmark.contour_left1 = Point2d(*doubleArray[2], *doubleArray[3]);
		landmark.contour_left2 = Point2d(*doubleArray[4], *doubleArray[5]);
		landmark.contour_left3 = Point2d(*doubleArray[6], *doubleArray[7]);
		landmark.contour_left4 = Point2d(*doubleArray[8], *doubleArray[9]);
		landmark.contour_left5 = Point2d(*doubleArray[10], *doubleArray[11]);
		landmark.contour_left6 = Point2d(*doubleArray[12], *doubleArray[13]);
		landmark.contour_left7 = Point2d(*doubleArray[14], *doubleArray[15]);
		landmark.contour_left8 = Point2d(*doubleArray[16], *doubleArray[17]);
		landmark.contour_left9 = Point2d(*doubleArray[18], *doubleArray[19]);
		landmark.contour_right1 = Point2d(*doubleArray[20], *doubleArray[21]);
		landmark.contour_right2 = Point2d(*doubleArray[22], *doubleArray[23]);
		landmark.contour_right3 = Point2d(*doubleArray[24], *doubleArray[25]);
		landmark.contour_right4 = Point2d(*doubleArray[26], *doubleArray[27]);
		landmark.contour_right5 = Point2d(*doubleArray[28], *doubleArray[29]);
		landmark.contour_right6 = Point2d(*doubleArray[30], *doubleArray[31]);
		landmark.contour_right7 = Point2d(*doubleArray[32], *doubleArray[33]);
		landmark.contour_right8 = Point2d(*doubleArray[34], *doubleArray[35]);
		landmark.contour_right9 = Point2d(*doubleArray[36], *doubleArray[37]);
		landmark.left_eye_bottom = Point2d(*doubleArray[38], *doubleArray[39]);
		landmark.left_eye_center = Point2d(*doubleArray[40], *doubleArray[41]);
		landmark.left_eye_left_corner = Point2d(*doubleArray[42], *doubleArray[43]);
		landmark.left_eye_lower_left_quarter = Point2d(*doubleArray[44], *doubleArray[45]);
		landmark.left_eye_lower_right_quarter = Point2d(*doubleArray[46], *doubleArray[47]);
		landmark.left_eye_pupil = Point2d(*doubleArray[48], *doubleArray[49]);
		landmark.left_eye_right_corner = Point2d(*doubleArray[50], *doubleArray[51]);
		landmark.left_eye_top = Point2d(*doubleArray[52], *doubleArray[53]);
		landmark.left_eye_upper_left_quarter = Point2d(*doubleArray[54], *doubleArray[55]);
		landmark.left_eye_upper_right_quarter = Point2d(*doubleArray[56], *doubleArray[57]);
		landmark.left_eyebrow_left_corner = Point2d(*doubleArray[58], *doubleArray[59]);
		landmark.left_eyebrow_lower_left_quarter = Point2d(*doubleArray[60], *doubleArray[61]);
		landmark.left_eyebrow_lower_middle = Point2d(*doubleArray[62], *doubleArray[63]);
		landmark.left_eyebrow_lower_right_quarter = Point2d(*doubleArray[64], *doubleArray[65]);
		landmark.left_eyebrow_right_corner = Point2d(*doubleArray[66], *doubleArray[67]);
		landmark.left_eyebrow_upper_left_quarter = Point2d(*doubleArray[68], *doubleArray[69]);
		landmark.left_eyebrow_upper_middle = Point2d(*doubleArray[70], *doubleArray[71]);
		landmark.left_eyebrow_upper_right_quarter = Point2d(*doubleArray[72], *doubleArray[73]);
		landmark.mouth_left_corner = Point2d(*doubleArray[74], *doubleArray[75]);
		landmark.mouth_lower_lip_bottom = Point2d(*doubleArray[76], *doubleArray[77]);
		landmark.mouth_lower_lip_left_contour1 = Point2d(*doubleArray[78], *doubleArray[79]);
		landmark.mouth_lower_lip_left_contour2 = Point2d(*doubleArray[80], *doubleArray[81]);
		landmark.mouth_lower_lip_left_contour3 = Point2d(*doubleArray[82], *doubleArray[83]);
		landmark.mouth_lower_lip_right_contour1 = Point2d(*doubleArray[84], *doubleArray[85]);
		landmark.mouth_lower_lip_right_contour2 = Point2d(*doubleArray[86], *doubleArray[87]);
		landmark.mouth_lower_lip_right_contour3 = Point2d(*doubleArray[88], *doubleArray[89]);
		landmark.mouth_lower_lip_top = Point2d(*doubleArray[90], *doubleArray[91]);
		landmark.mouth_right_corner = Point2d(*doubleArray[92], *doubleArray[93]);
		landmark.mouth_upper_lip_bottom = Point2d(*doubleArray[94], *doubleArray[95]);
		landmark.mouth_upper_lip_left_contour1 = Point2d(*doubleArray[96], *doubleArray[97]);
		landmark.mouth_upper_lip_left_contour2 = Point2d(*doubleArray[98], *doubleArray[99]);
		landmark.mouth_upper_lip_left_contour3 = Point2d(*doubleArray[100], *doubleArray[101]);
		landmark.mouth_upper_lip_right_contour1 = Point2d(*doubleArray[102], *doubleArray[103]);
		landmark.mouth_upper_lip_right_contour2 = Point2d(*doubleArray[104], *doubleArray[105]);
		landmark.mouth_upper_lip_right_contour3 = Point2d(*doubleArray[106], *doubleArray[107]);
		landmark.mouth_upper_lip_top = Point2d(*doubleArray[108], *doubleArray[109]);
		landmark.nose_contour_left1 = Point2d(*doubleArray[110], *doubleArray[111]);
		landmark.nose_contour_left2 = Point2d(*doubleArray[112], *doubleArray[113]);
		landmark.nose_contour_left3 = Point2d(*doubleArray[114], *doubleArray[115]);
		landmark.nose_contour_lower_middle = Point2d(*doubleArray[116], *doubleArray[117]);
		landmark.nose_contour_right1 = Point2d(*doubleArray[118], *doubleArray[119]);
		landmark.nose_contour_right2 = Point2d(*doubleArray[120], *doubleArray[121]);
		landmark.nose_contour_right3 = Point2d(*doubleArray[122], *doubleArray[123]);
		landmark.nose_left = Point2d(*doubleArray[124], *doubleArray[125]);
		landmark.nose_right = Point2d(*doubleArray[126], *doubleArray[127]);
		landmark.nose_tip = Point2d(*doubleArray[128], *doubleArray[129]);
		landmark.right_eye_bottom = Point2d(*doubleArray[130], *doubleArray[131]);
		landmark.right_eye_center = Point2d(*doubleArray[132], *doubleArray[133]);
		landmark.right_eye_left_corner = Point2d(*doubleArray[134], *doubleArray[135]);
		landmark.right_eye_lower_left_quarter = Point2d(*doubleArray[136], *doubleArray[137]);
		landmark.right_eye_lower_right_quarter = Point2d(*doubleArray[138], *doubleArray[139]);
		landmark.right_eye_pupil = Point2d(*doubleArray[140], *doubleArray[141]);
		landmark.right_eye_right_corner = Point2d(*doubleArray[142], *doubleArray[143]);
		landmark.right_eye_top = Point2d(*doubleArray[144], *doubleArray[145]);
		landmark.right_eye_upper_left_quarter = Point2d(*doubleArray[146], *doubleArray[147]);
		landmark.right_eye_upper_right_quarter = Point2d(*doubleArray[148], *doubleArray[149]);
		landmark.right_eyebrow_left_corner = Point2d(*doubleArray[150], *doubleArray[151]);
		landmark.right_eyebrow_lower_left_quarter = Point2d(*doubleArray[152], *doubleArray[153]);
		landmark.right_eyebrow_lower_middle = Point2d(*doubleArray[154], *doubleArray[155]);
		landmark.right_eyebrow_lower_right_quarter = Point2d(*doubleArray[156], *doubleArray[157]);
		landmark.right_eyebrow_right_corner = Point2d(*doubleArray[158], *doubleArray[159]);
		landmark.right_eyebrow_upper_left_quarter = Point2d(*doubleArray[160], *doubleArray[161]);
		landmark.right_eyebrow_upper_middle = Point2d(*doubleArray[162], *doubleArray[163]);
		landmark.right_eyebrow_upper_right_quarter = Point2d(*doubleArray[164], *doubleArray[165]);
	}

	/* insert line by step */
	static void InsertLine(int insertSize, const vector<Point> &curve, vector<Point> &outCurve)
	{
		for (int m = 0; m < curve.size() - 1; ++m)
		{
			double dx = curve[m + 1].x - curve[m].x;
			double dy = curve[m + 1].y - curve[m].y;
			for (int step = 0; step < insertSize; ++step)
			{
				Point tmpPoint;
				tmpPoint.x = (int)(curve[m].x + step * dx / insertSize);
				tmpPoint.y = (int)(curve[m].y + step * dy / insertSize);
				outCurve.push_back(tmpPoint);
			}
		}
	}

	/* y = ax + b, calculate the line of a and b */
	static void  CalLineParam(const Point &stPoint, const Point &edPoint, LineParamer &paramer)
	{
		if ((edPoint.x - stPoint.x) != 0)
		{
			paramer.a = (double)(edPoint.x - stPoint.x) / (edPoint.y - stPoint.y);
			paramer.b = (stPoint.y * edPoint.x - edPoint.y * stPoint.x) / (edPoint.y - stPoint.y);
		}
		else
		{
			paramer.a = 0;
			paramer.b = -edPoint.x;
		}
	}

	/* calculate the offset of point, param about linear paramer alpha, beta, sigma */
	static void CalOffset(const LineParamer &param, const Point &srcPoint, Point &dstPoint, double &alpha, double &beta, double sigma)
	{
		double dection = (sqrt(param.a * param.a + 1));
		double distance = (double)((param.a * srcPoint.y - srcPoint.x - param.b) / dection) * alpha * beta * sigma;
		// calculate the next point.
		dstPoint.x = (int)(srcPoint.x + (1 / dection) * distance);
		dstPoint.y = (int)(srcPoint.y + (param.a / dection) * distance);
	}

	 /* calclate tow line cross point by four point */
	 static void CalCrosPoint(const Point &line1Pt1, const Point &line1Pt2, const Point &line2Pt1, const Point &line2Pt2, Point &crosPoint)
	 {
		 double xl0, xl1, xl2, xl3;
		 double yl0, yl1, yl2, yl3;
		 xl0 = line1Pt1.x;
		 yl0 = line1Pt1.y;

		 xl1 = line1Pt2.x;
		 yl1 = line1Pt2.y;

		 xl2 = line2Pt1.x;
		 yl2 = line2Pt1.y;

		 xl3 = line2Pt2.x;
		 yl3 = line2Pt2.y;

		 crosPoint.y = ((yl0 - yl1) * (yl3 - yl2) * xl0 + (yl3 - yl2) * (xl1 - xl0) * yl0
			 + (yl1 - yl0) * (yl3 - yl2) * xl2 + (xl2 - xl3) * (yl1 - yl0) * yl2) /
			 ((xl1 - xl0) * (yl3 - yl2) + (yl0 - yl1) * (xl3 - xl2));

		 crosPoint.x = xl2 + (xl3 - xl2) * (crosPoint.y - yl2) / (yl3 - yl2);
	 }

	 /* find the max distance of points*/
	 static void FindMaxDist(const Point &centerPt, const vector<Point> &points, double &maxDist)
	 {
		 double tmpCircle = 0, maxCircle = 0;
		 for (int i = 0; i < points.size(); ++i)
		 {
			 tmpCircle = Distance(centerPt, points[i]);
			 if (tmpCircle > maxCircle)
				 maxCircle = tmpCircle;
		 }
		 maxDist = maxCircle;
	 }

	 /* make point array curve to full mask */
	 static void PointArray2Mask(const vector<Point> &curve, Mat &mask, const Size &size)
	 {
		 vector<vector<Point> > outCurve;
		 outCurve.push_back(curve);
		 mask = Mat(size, CV_8UC1, Scalar(0));
		 drawContours(mask, outCurve, -1, Scalar(255), CV_FILLED);
	 }

	 /* make point array curve to line mask */
	 static void PointArray2MaskLine(const vector<Point> &curve, Mat &mask, const Size &size, int r)
	 {
		 vector<vector<Point> > outCurve;
		 outCurve.push_back(curve);
		 mask = Mat(size, CV_8UC1, Scalar(0));
		 drawContours(mask, outCurve, -1, Scalar(255), r);
	 }
}

#endif /* WRAPER_CORE_BASE_H */


