#include "WhitenCanvas.h"

namespace wrap{

	namespace ImageWhiten {

		WhitenCanvas::WhitenCanvas()
		{
			/* init the array*/
			for (int i = 0; i < 256; i++)
			{
				highlights_add[i] = (1.075 - 1 / ((double)i / 16.0 + 1));
				midtones_add[i] = 0.667 * (1 - (((double)i - 127.0) / 127.0) * (((double)i - 127.0) / 127.0));
				shadows_add[i] = 0.667 * (1 - (((double)i - 127.0) / 127.0) * (((double)i - 127.0) / 127.0));
			}
		}

		WhitenCanvas::~WhitenCanvas()
		{}

		bool WhitenCanvas::doWhiten(Vec3b &p, int strength)
		{
			int red = p[0];
			int green = p[1];
			int blue = p[2];

			red += (int)(strength * midtones_add[red]);
			red = max(0, min(0xFF, red));
			p[0] = (unsigned char)red;

			green += (int)(strength * midtones_add[green]);
			green = max(0, min(0xFF, green));
			p[1] = (unsigned char)green;

			blue += (int)(strength * midtones_add[blue]);
			blue = max(0, min(0xFF, blue));
			p[2] = (unsigned char)blue;

			return true;
		}
	}
}
