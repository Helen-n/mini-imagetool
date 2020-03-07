#ifndef WRAPER_CORE_CSTRUCT_H
#define WRAPER_CORE_CSTRUCT_H

#include "cdef.hpp"

namespace wrap
{
	/* calculate control derection */
	struct ControlDection
	{
		int sAngle;  // start angle
		int eAngle;  // end   angle
		ControlDection(int aAngle_, int eAngle_)
		{
			sAngle = aAngle_;
			eAngle = eAngle_;
		}

		ControlDection(DerectionType dection)
		{
			switch (dection)
			{
			case DERECTION_ALL:
				sAngle = 0;
				eAngle = 360;
				break;
			case DERECTION_TOP:
				sAngle = 0;
				eAngle = 180;
				break;
			case DERECTION_DOWN:
				sAngle = 180;
				eAngle = 360;
				break;
			case DERECTION_LEFT:
				sAngle = 90;
				eAngle = 270;
				break;
			case DERECTION_RIGHT:
				sAngle = 270;
				eAngle = 420;
				break;
			default:
				sAngle = 0;
				eAngle = 360;
				break;
			}
		}

	};

	/* y = ax + b, paramer of a and b */
	struct  LineParamer
	{
		double a;
		double b;
		LineParamer() {}
		LineParamer(double a_, double b_)
		{
			a = a_;
			b = b_;
		}
	};

	struct WarperImage
	{
		Mat     imagedata;
		Point   position;
	};

	struct Landmark
	{
		Point2d contour_chin;
		Point2d	contour_left1;
		Point2d	contour_left2;
		Point2d	contour_left3;
		Point2d	contour_left4;
		Point2d	contour_left5;
		Point2d contour_left6;
		Point2d contour_left7;
		Point2d contour_left8;
		Point2d contour_left9;
		Point2d contour_right1;
		Point2d contour_right2;
		Point2d contour_right3;
		Point2d contour_right4;
		Point2d contour_right5;
		Point2d contour_right6;
		Point2d contour_right7;
		Point2d contour_right8;
		Point2d contour_right9;
		Point2d left_eye_bottom;
		Point2d left_eye_center;
		Point2d left_eye_left_corner;
		Point2d left_eye_lower_left_quarter;
		Point2d left_eye_lower_right_quarter;
		Point2d left_eye_pupil;
		Point2d	left_eye_right_corner;
		Point2d	left_eye_top;
		Point2d	left_eye_upper_left_quarter;
		Point2d left_eye_upper_right_quarter;
		Point2d left_eyebrow_left_corner;
		Point2d left_eyebrow_lower_left_quarter;
		Point2d left_eyebrow_lower_middle;
		Point2d left_eyebrow_lower_right_quarter;
		Point2d left_eyebrow_right_corner;
		Point2d left_eyebrow_upper_left_quarter;
		Point2d left_eyebrow_upper_middle;
		Point2d left_eyebrow_upper_right_quarter;
		Point2d	mouth_left_corner;
		Point2d	mouth_lower_lip_bottom;
		Point2d	mouth_lower_lip_left_contour1;
		Point2d	mouth_lower_lip_left_contour2;
		Point2d	mouth_lower_lip_left_contour3;
		Point2d	mouth_lower_lip_right_contour1;
		Point2d	mouth_lower_lip_right_contour2;
		Point2d	mouth_lower_lip_right_contour3;
		Point2d	mouth_lower_lip_top;
		Point2d	mouth_right_corner;
		Point2d	mouth_upper_lip_bottom;
		Point2d	mouth_upper_lip_left_contour1;
		Point2d	mouth_upper_lip_left_contour2;
		Point2d	mouth_upper_lip_left_contour3;
		Point2d	mouth_upper_lip_right_contour1;
		Point2d	mouth_upper_lip_right_contour2;
		Point2d	mouth_upper_lip_right_contour3;
		Point2d	mouth_upper_lip_top;
		Point2d	nose_contour_left1;
		Point2d	nose_contour_left2;
		Point2d	nose_contour_left3;
		Point2d	nose_contour_lower_middle;
		Point2d	nose_contour_right1;
		Point2d	nose_contour_right2;
		Point2d	nose_contour_right3;
		Point2d	nose_left;
		Point2d	nose_right;
		Point2d	nose_tip;
		Point2d	right_eye_bottom;
		Point2d	right_eye_center;
		Point2d	right_eye_left_corner;
		Point2d	right_eye_lower_left_quarter;
		Point2d	right_eye_lower_right_quarter;
		Point2d	right_eye_pupil;
		Point2d	right_eye_right_corner;
		Point2d	right_eye_top;
		Point2d	right_eye_upper_left_quarter;
		Point2d	right_eye_upper_right_quarter;
		Point2d	right_eyebrow_left_corner;
		Point2d	right_eyebrow_lower_left_quarter;
		Point2d	right_eyebrow_lower_middle;
		Point2d	right_eyebrow_lower_right_quarter;
		Point2d	right_eyebrow_right_corner;
		Point2d	right_eyebrow_upper_left_quarter;
		Point2d	right_eyebrow_upper_middle;
		Point2d	right_eyebrow_upper_right_quarter;
	};
}

#endif /* WRAPER_CORE_CSTRUCT_H */
