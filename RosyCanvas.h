#ifndef WHITEN_ROSY
#define WHITEN_ROSY

#include "core.hpp"

namespace wrap
{
	namespace ImageWhiten
	{
		class DLLEXPORTED HSLChannel {
		public:
			int hue;          //色度调整值，     数据范围:  [-180, 180]
			int saturation;   //饱和度调整值，数据范围:  [-100, 100]
			int brightness;   //明度调整值，    数据范围:  [-100, 100]

			int   colorIndex;  //color index: 0 = RED, 1 = YELLOW, 2 = GREEN
			float left_left;  //hue range left-left
			float left;       //hue range left
			float right;	  //hue range right
			float right_right;//hue range right-right
			bool defined;     //

			HSLChannel();
			virtual ~HSLChannel();

			void calcDefined();
			void setColorIndex(int index);
			bool match(float hue);
			void adjust(int h, float *delta_hsb);
		};

		class DLLEXPORTED RosyCanvas {
		public:
			RosyCanvas();

			virtual ~RosyCanvas();

			HSLChannel channels[7];

			int adjust(InputArray src, OutputArray dst);

			void RGB2HSB(uchar R, uchar G, uchar B, float *h, float *s, float *b);

			void HSB2RGB(float h, float s, float b, uchar *R, uchar *G, uchar *B);

			void BGR2HSB(const uchar *bgr, float *hsb);

			void HSB2BGR(const float *hsb, uchar *bgr);
		};
	}
	
} 

#endif /* WHITEN_ROSY */

