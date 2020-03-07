#include "ManageBlur.h"

namespace wrap
{
	namespace ImageBlur
	{
			/*=================================================================================*/
			// common part interface
			/*=================================================================================*/
			bool blurInit(int width, int height, int scanWidth, int bpp, unsigned char* rawData,
				int nCount, const double* doubleArray[])
			{
				m_channels = bpp;
				if (m_channels == 4)
				{
					m_srcImage = new Mat(height, width, CV_8UC4, rawData, scanWidth);
					cvtColor(*m_srcImage, *m_srcImage, CV_RGBA2RGB);
					m_dstImage = new Mat(height, width, CV_8UC4, rawData, scanWidth);
					cvtColor(*m_dstImage, *m_dstImage, CV_RGBA2RGB);
					m_landmark = new Landmark();
				}
				else
				{				
					m_srcImage = new Mat(height, width, CV_8UC3, rawData, scanWidth);
					imshow("¡¾SRC_BLUR¡¿", *m_srcImage);
					m_dstImage = new Mat(height, width, CV_8UC3, rawData, scanWidth);
					m_landmark = new Landmark();
				}

				Array2Landmark(nCount, doubleArray, *m_landmark);
				m_blur = new Blur();
				if (m_srcImage && m_dstImage && m_landmark&& m_blur)
					return true;

				return false;
			}

			unsigned char* blurImage(int *width, int *height, int *scanWidth, int *bpp)
			{
				if (width && height && scanWidth && m_blur && m_dstImage)
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

			void blurRelease()
			{
				if (m_blur)
					delete m_blur;

				if (m_srcImage)
					delete m_srcImage;

				if (m_dstImage)
					delete m_dstImage;
			}

			/*=================================================================================*/
			// special usage interface 
			/*=================================================================================*/
			void blurFlw(int brushSize, int strength)
			{
				double bottom_x = (m_landmark->mouth_left_corner.x + m_landmark->mouth_right_corner.x) / 2;
				double bottom_y = (m_landmark->mouth_left_corner.y + m_landmark->mouth_right_corner.y) / 2;

				double top_x = (m_landmark->nose_contour_left2.x + m_landmark->nose_contour_right2.x) / 2;
				double top_y = (m_landmark->nose_contour_left2.y + m_landmark->nose_contour_right2.y) / 2;

				double w = (m_landmark->mouth_right_corner.x - m_landmark->mouth_left_corner.x) * 1.4;
				double h = bottom_y - top_y;

				double x = top_x - w / 2;
				double y = top_y;

				m_blur->doFlw(*m_srcImage, *m_dstImage, Rect(x, y, w, h));

			}

			/*=================================================================================*/
			// common usage interface
			/*=================================================================================*/
			void blurCommon(int x, int y, int w, int h, int r, BlurType blurType)
			{
				m_blur->doBlur(*m_srcImage, *m_dstImage, Rect(x, y, w, h), r, blurType);
				imshow("¡¾DST_BLUR¡¿", *m_dstImage);
			}
	   }
}




