#ifndef WRAPER_CORE_CDEF_H
#define WRAPER_CORE_CDEF_H

namespace wrap
{

#ifdef _WIN32
#define DLLEXPORTED __declspec( dllexport )
#else
#define DLLEXPORTED
#endif

#define GROWSTEP 0.05f

#define SWAP(a, b, t)  do { t = a; a = b; b = t; } while(0)

	/* warper gridstep type */
	enum  GridstepType
	{
		GRIDSTEP_MIN = 5,
		GRIDSTEP_MAX = 20,
		GRIDSTEP_DEFAULT = 10
	};

	/* image blur type */
	enum  BlurType
	{
		BLUR_GUASS  = 0,
		BLUR_BOX    = 1,
		BLUR_MIDDLE = 2,
		BLUR_DOUBLE = 3
	};

	/* warper shrink type */
	enum ShrinkType
	{
		SHRINK_ALL  = -1,
		SHRINK_TOP  =  0,
		SHRINK_DOWN =  1,
		SHRINK_LEFT =  2,
		SHRINK_RIGHT=  3
	};

	/* image warper type */
	enum WarperType
	{
		WARPER_TRANSLATE = 0,
		WARPER_GROW = 1,
		WARPER_SHRINK = 2
	};

	/* warper face reduce type */
	enum WarperFaceType
	{
		FACE_LEFT = 0,
		FACE_RIGHT = 1,
		FACE_ALL = 2
	};

	/* warper nose reduce type */
	enum WarperNoseType
	{
		NOSE_REDUCE_MIDDLE = 0,
		NOSE_REDUCE_BOTTOM = 1,
		NOSE_ADD_HEIGHT = 2
	};

	/* warper fill type */
	enum WarperFillType
	{
		FILL_BROW = 0,     // ·á¶îÍ·
		FILL_TEMPLE = 1,   // ·áÌ«ÑôÑ¨
		FILL_WOCAN = 2,    // ·áÎÔ²Ï
		FILL_CHEEK = 3,    // ·áÁ³¼Õ
		FILL_APPLE = 4,    // ·áÆ»¹û¼¡
		FILL_LIP = 5,      // ·á×ì´½
		FILL_CHIN = 6,     // µæÏÂ°Í
		FILL_DUDU = 7,     // à½à½´½
		FILL_EYE = 8       // ¿ªÑÛ½Ç
	};

	/* warper grow shrink derection type */
	enum DerectionType
	{
		DERECTION_ALL = 0,
		DERECTION_TOP = 1,
		DERECTION_DOWN = 2,
		DERECTION_LEFT = 3,
		DERECTION_RIGHT = 4
	};

	enum HslColor
	{
		HSL_ALL = 0,
		HSL_RED = 1,
		HSL_YELLOW = 2,
		HSL_GREEN = 3,
		HSL_CYAN = 4,
		HSL_BLUE = 5,
		HSL_MAGENTA = 6,
	};

	enum ComparisonMethod
	{
		 NORM_L1 = 1,
		 NORM_L2 = 6,
		 INTERSECTION = 2,
		 CORRELATION = 3,
		 CHI_SQUARED = 4,
		 BHATTACHRYA = 5,
		 BHATTACHRYA1 = 7,
	};

	enum LbpType
	{
		LBP_GRAY = 0,
	    LBP_UNIFORM =1
	};
}

#endif /* WRAPER_CORE_CDEF_H */


