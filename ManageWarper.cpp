#include "ManageWarper.h"

namespace wrap
{
	namespace ImageWarper
	{
		/*=================================================================================*/
		// common part interface
		/*=================================================================================*/
		/* warper init */
		bool warperInit(int width, int height, int scanWidth, int bpp, unsigned char* rawData,
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
			m_warper = new Warper(m_srcImage);
			imshow("¡¾SRC_WARPER¡¿",*m_srcImage);
			if (m_srcImage && m_dstImage && m_landmark)
				return true;

			return false;
		}

		/* get warpered image */
		unsigned char* warperImage(int *width, int *height, int *scanWidth, int *bpp)
		{
			if (width && height && scanWidth && m_warper && m_dstImage)
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

		/* release the warper */
		void warperRelease()
		{
			if (m_warper)
				delete m_warper;

			//if (m_srcImage)
				//delete m_srcImage;

			if (m_dstImage)
				delete m_dstImage;
			
			if (m_landmark)
				delete m_landmark;
		}

		/*=================================================================================*/
		// special usage interface 
		/*=================================================================================*/
		/* warper by face shape */
		void warperFace(int brushSize, int strength, WarperFaceType faceType)
		{
			vector<Point> curve;
			vector<Point> outCurve;
			curve.push_back(m_landmark->contour_left2);
			curve.push_back(m_landmark->contour_left3);
			curve.push_back(m_landmark->contour_left4);
			curve.push_back(m_landmark->contour_left5);
			curve.push_back(m_landmark->contour_left6);
			curve.push_back(m_landmark->contour_left7);
			curve.push_back(m_landmark->contour_left8);
			curve.push_back(m_landmark->contour_left9);
			curve.push_back(m_landmark->contour_chin);
			curve.push_back(m_landmark->contour_right9);
			curve.push_back(m_landmark->contour_right8);
			curve.push_back(m_landmark->contour_right7);
			curve.push_back(m_landmark->contour_right6);
			curve.push_back(m_landmark->contour_right5);
			curve.push_back(m_landmark->contour_right4);
			curve.push_back(m_landmark->contour_right3);
			curve.push_back(m_landmark->contour_right2);

			Point lineStart, lineEnd;
			lineStart.x = m_landmark->contour_chin.x;
			lineStart.y = m_landmark->contour_chin.y;
			double sumX = m_landmark->nose_contour_lower_middle.x + m_landmark->nose_tip.x;
			double sumY = m_landmark->nose_contour_lower_middle.y + m_landmark->nose_tip.y;
			lineEnd.x = (int)((sumX) / 2);
			lineEnd.y = (int)((sumY) / 2);

			double dist = Distance((curve[0]), (curve[16]));

			int insertSize = 6;
			for (int m = 0; m < curve.size() - 1; ++m)
			{
				double dx = curve[m + 1].x - curve[m].x;
				double dy = curve[m + 1].y - curve[m].y;
				for (int step = 0; step < insertSize; ++step)
				{
					Point tmpPoint;
					tmpPoint.x = curve[m].x + step * dx / insertSize;
					tmpPoint.y = curve[m].y + step * dy / insertSize;
					outCurve.push_back(tmpPoint);
				}
			}

			int curveStart = 0;
			int curveEnd = outCurve.size();
			LineParamer paramer;
			CalLineParam(lineEnd, lineEnd, paramer);

			switch (faceType)
			{
			case wrap::FACE_LEFT:
				curveEnd /= 2;
				break;
			case wrap::FACE_RIGHT:
				curveStart = curveEnd / 2;
				break;
			case wrap::FACE_ALL:
				break;
			default:
				break;
			}
			m_warper->doWarperFace(curveStart, curveEnd, brushSize, strength, paramer, outCurve);
			m_warper->GetImage(*m_dstImage);
		}

		/* warper by nose shape*/
		void warperNose(int brushSize, int strength, WarperNoseType noseType)
		{
			/* part of one : calculate the control function of warper */
			vector<Point> curve;
			vector<Point> outCurve;
			curve.push_back(m_landmark->nose_contour_left1);
			curve.push_back(m_landmark->nose_contour_left2);
			curve.push_back(m_landmark->nose_left);
			curve.push_back(m_landmark->nose_contour_left3);
			curve.push_back(m_landmark->nose_contour_lower_middle);
			curve.push_back(m_landmark->nose_contour_right3);
			curve.push_back(m_landmark->nose_right);
			curve.push_back(m_landmark->nose_contour_right2);
			curve.push_back(m_landmark->nose_contour_right1);
			Point lineStart, lineEnd;
			lineStart = m_landmark->nose_tip;
			lineEnd = m_landmark->nose_contour_lower_middle;

			LineParamer paramer;
			CalLineParam(lineEnd, lineEnd, paramer);

			double controlParam[4];
			double controlParamSum = 0;
			for (int i = 0; i < 4; ++i)
			{
				controlParam[i] = Distance(curve[i], curve[8 - i]);
				controlParamSum += controlParam[i];
			}
			double dist = Distance(curve[2], curve[6]);
			int insertSize = 8;
			InsertLine(insertSize, curve, outCurve);

			double maxLength = outCurve.size();
			vector<double> controlF;
			/* ------------------------------------------- */

			/* part of two : calculate the range of shrink */
			vector<Point> rangeCurve;
			rangeCurve.push_back(m_landmark->nose_contour_left2);
			rangeCurve.push_back(m_landmark->nose_left);
			rangeCurve.push_back(m_landmark->nose_contour_left3);
			rangeCurve.push_back(m_landmark->nose_contour_lower_middle);
			rangeCurve.push_back(m_landmark->nose_contour_right3);
			rangeCurve.push_back(m_landmark->nose_right);
			rangeCurve.push_back(m_landmark->nose_contour_right2);
			Point midTip = m_landmark->nose_contour_lower_middle;
			double maxCircle = 0;
			FindMaxDist(midTip, rangeCurve, maxCircle);
			/* --------------------------------------------------- */
			int j;
			switch (noseType)
			{
			case wrap::NOSE_REDUCE_MIDDLE:
				for (j = 0; j < maxLength; j++)
				{
					if (j < 8 || j > 55)
						controlF[j] = controlParam[2] / controlParamSum * 2;
					else if ((7 < j && j < 16) || (47 < j && j < 56))
						controlF[j] = controlParam[1] / controlParamSum;
					else if ((15 < j && j < 24) || (39 < j && j < 48))
						controlF[j] = controlParam[1] / controlParamSum;
					else
						controlF[j] = 0;
				}

				m_warper->doWarperNoseWarper(brushSize, strength, controlF, paramer, outCurve);
				m_warper->doWarperNoseShrink(maxCircle, strength, midTip);
				break;

			case wrap::NOSE_ADD_HEIGHT:
				for (j = 0; j < maxLength; j++)
				{
					if (j < 8 || j > 55)
						controlF[j] = controlParam[2] / controlParamSum * 4;
					else
						controlF[j] = 0;
				}

				m_warper->doWarperNoseWarper(brushSize, strength, controlF, paramer, outCurve);
				m_warper->doWarperNoseShrink(maxCircle, strength, midTip);
				break;

			default:
				for (j = 0; j < maxLength; j++)
					controlF[j] = 1;
				m_warper->doWarperNoseShrink(maxCircle, strength, midTip);
				break;
			}
			m_warper->GetImage(*m_dstImage);
		}

		/* warper fill by grow */
		void warperFill(int brushSize, int strength, WarperFillType fillType)
		{
			switch (fillType)
			{

			case wrap::FILL_APPLE:
			{
				Point leftPt(0, 0);
				CalCrosPoint(m_landmark->left_eye_bottom, m_landmark->contour_left6, m_landmark->nose_left, m_landmark->contour_left3, leftPt);
				m_warper->doWarperFill(brushSize, strength, leftPt, DERECTION_ALL);
				Point rightPt(0, 0);
				CalCrosPoint(m_landmark->right_eye_bottom, m_landmark->contour_right6, m_landmark->nose_right, m_landmark->contour_right3, rightPt);
				m_warper->doWarperFill(brushSize, strength, rightPt, DERECTION_ALL);
				break;
			}

			case wrap::FILL_LIP:
			{
				Point mouth_Low_Lip_Top = m_landmark->mouth_lower_lip_top;
				m_warper->doWarperFill(brushSize, strength, mouth_Low_Lip_Top, DERECTION_DOWN);
				Point mouth_upper_lip_bottom = m_landmark->mouth_upper_lip_bottom;
				m_warper->doWarperFill(brushSize, strength, mouth_upper_lip_bottom, DERECTION_TOP);
				break;
			}

			case wrap::FILL_EYE:
			{
				Point eyeLeft = m_landmark->left_eye_center;
				Point eyeRight = m_landmark->right_eye_center;
				m_warper->doWarperFill(brushSize, strength, eyeLeft, DERECTION_TOP);
				m_warper->doWarperFill(brushSize, strength, eyeRight, DERECTION_TOP);
				break;
			}
			/*
			case wrap::FILL_BROW:
			break;

			case wrap::FILL_TEMPLE:
			break;

			case wrap::FILL_WOCAN:
			break;

			case wrap::FILL_CHEEK:
			break;

		case wrap::FILL_CHIN:
			break;

		case wrap::FILL_DUDU:
			break;
			*/
			default:
				break;
			}
			m_warper->GetImage(*m_dstImage);
		}

		/*=================================================================================*/
		// common usage interface
		/*=================================================================================*/
		/* warper by point , start point (x,y) and end point (x,y)*/
		void warperPoint(int sx, int sy, int ex, int ey, int brushSize, WarperType warperType, DerectionType derection)
		{
			m_warper->UpdateWarp(Point(sx, sy), Point(ex, ey-2), brushSize, warperType, derection);
			m_warper->GetImage(*m_dstImage);
			imshow("¡¾DST_WARPER_POINT¡¿",*m_dstImage);
		}
	}
}