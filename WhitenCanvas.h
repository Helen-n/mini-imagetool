#ifndef WHITEN_CANVAS
#define WHITEN_CANVAS

#include "core.hpp"
namespace wrap
{
	namespace ImageWhiten 
	{
		class DLLEXPORTED WhitenCanvas
		{
			double highlights_add[256], midtones_add[256], shadows_add[256];

		public:
			WhitenCanvas();
			~WhitenCanvas();
			bool doWhiten(cv::Vec3b &p, int strength);
		};
	}
}
#endif /* WHITEN_CANVAS */