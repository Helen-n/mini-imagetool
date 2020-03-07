#ifndef WHITEN_COLORSPACE
#define WHITEN_COLORSPACE

#include "core.hpp"

namespace wrap {

	namespace ImageWhiten {

		class DLLEXPORTED ColorSpace
		{

			static float HUE2RGB(float p, float q, float t);

		public:
			void BGR2Lab(const uchar *bgr, float *lab);

			void Lab2BGR(const  float *lab, uchar *bgr);

			void BGR2HSL(const uchar *bgr, float *hsl);

			void HSL2BGR(const float *hsl, uchar *bgr);

			void BGR2HSB(const uchar *bgr, float *hsb);

			void HSB2BGR(const float *hsb, uchar *bgr);

			void BGR2CMYK(const uchar *bgr, float *cmyk);

			void CMYK2BGR(const float *cmyk, uchar *bgr);

			void RGB2HSL(uchar R, uchar G, uchar B, float *H, float *S, float *L);

			void HSL2RGB(float H, float S, float L, uchar *R, uchar *G, uchar *B);

			void RGB2HSB(uchar R, uchar G, uchar B, float *h, float *s, float *b);

			void HSB2RGB(float h, float s, float b, uchar *R, uchar *G, uchar *B);

			void RGB2CMYK(uchar R, uchar G, uchar B, float *C, float *M, float *Y, float *K);

			void CMYK2RGB(float C, float M, float Y, float K, uchar *R, uchar *G, uchar *B);

			void RGB2Lab(uchar R, uchar G, uchar B, float *L, float *a, float *b);

			void Lab2RGB(float L, float a, float b, uchar *R, uchar *G, uchar *B);
		};
	} 
}

#endif /* WHITEN_COLORSPACE */
