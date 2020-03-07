#ifndef WHITEN_WHITEN
#define WHITEN_WHITEN

#include "core.hpp"
#include "WhitenCanvas.h"
#include "SkinDetector.h"

namespace wrap
{
	namespace ImageWhiten
	{
		class DLLEXPORTED Whiten
		{
		private:
			WhitenCanvas *m_whiten;

		public:

			Whiten();

			virtual ~Whiten();

			

		public:

			void doWhiten(Mat &srcImage, Mat &dstImage, double strength);

			void doRosy(Mat &srcImage, Mat &dstImage, double rosyDegree);

			void doWahitenPoint(Vec3b &p, int strength);

		};
	}
}

#endif /* WHITEN_WHITEN */