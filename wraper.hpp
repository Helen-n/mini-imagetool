#ifndef WRAPER
#define WRAPER

#ifdef HAVE_WRAPER_BLUR
#include "ManageBlur.h"
#endif

#ifdef HAVE_WRAPER_INPAINT
#include "ManageInpaint.h"
#endif

#ifdef HAVE_WRAPER_WARPING
#include "ManageWarper.h"
#endif

#ifdef HAVE_WRAPER_WHITEN
#include "ManageWhiten.h"
#endif

#ifdef HAVE_WRAPER_TEST
#include "ts.hpp"
#endif

#endif /* WRAPER */
