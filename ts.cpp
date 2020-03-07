#include "ManageBlur.h"
#include "ManageInpaint.h"
#include "ManageWarper.h"
#include "ManageWhiten.h"

using namespace wrap;

/* this file is for test */
int main()
{
	double conmon_doubleArray[166] = { 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,
		1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,
		1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,
		1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,
		1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,
		1,2,3,4,5,6 };

	/*========================================================================================*/
	// test for blur
	/*========================================================================================*/
	string blur_path = "TestImage/TsForBlur.png";
	Mat blur_Image = imread(blur_path);
	ImageBlur::blurInit(blur_Image.cols, blur_Image.rows, blur_Image.step, 3, blur_Image.data, 166, conmon_doubleArray);

	// common usage interface
	ImageBlur::blurCommon(0, 0, blur_Image.cols, blur_Image.rows, 3, BLUR_GUASS);

	// special usage interface 
	//ImageBlur::blurFlw( brushSize,  strength);

	ImageBlur::blurRelease();
	waitKey(0);

	destroyWindow("¡¾SRC_BLUR¡¿");
	destroyWindow("¡¾DST_BLUR¡¿");

	/*========================================================================================*/
	// test for inpaint
	/*========================================================================================*/
	string inpaint_path = "TestImage/TsForInpaint.jpg";
	Mat inpaint_Image = imread(inpaint_path);
	ImageInpaint::inpaintInit(inpaint_Image.cols, inpaint_Image.rows, inpaint_Image.step, 3, inpaint_Image.data, 166, conmon_doubleArray);

	// common usage interface
	ImageInpaint::inpaintPoint( 422, 360, 8);
	ImageInpaint::inpaintRect( 370, 230, 100, 35,  12, LBP_GRAY);

	// special usage interface 
	//inpaintBrow(int r, LbpType lbpType);
	ImageInpaint::inpaintRelease();
	waitKey(0);

	destroyWindow("¡¾SRC_INPAINT¡¿");
	destroyWindow("¡¾DST_INPAINT_RECT¡¿");
	destroyWindow("¡¾DST_INPAINT_POINT¡¿");

	/*========================================================================================*/
	// test for warping
	/*========================================================================================*/
	string warper_path = "TestImage/TsForWarper.jpg";
	Mat warper_Image = imread(warper_path);
	ImageWarper::warperInit(warper_Image.cols, warper_Image.rows, warper_Image.step, 3, warper_Image.data, 166, conmon_doubleArray);

	// special usage interface 
	//ImageWarper::warperFace( brushSize,  strength,  faceType);
	//ImageWarper::warperNose( brushSize,  strength,  noseType);
	//ImageWarper::warperFill( brushSize,  strength,  fillType);

	// common usage interface
	ImageWarper::warperPoint( 400,290,400,290,50, WARPER_GROW, DERECTION_TOP);
	
	ImageWarper::warperRelease();
	waitKey(0);
	destroyWindow("¡¾SRC_WARPER¡¿");
	destroyWindow("¡¾DST_WARPER_POINT¡¿");

	/*========================================================================================*/
	// test for whiten
	/*========================================================================================*/
	string whiten_path = "TestImage/TsForWhiten.jpg";
	Mat whiten_Image = imread(whiten_path);
    ImageWhiten::whitenInit(whiten_Image.cols, whiten_Image.rows, whiten_Image.step, 3, whiten_Image.data, 166, conmon_doubleArray);
	
	// special usage interface 
	//ImageWhiten::whitenFace( brushSize,  strength,  faceType);
	ImageWhiten::whitenAll(30, 20);

	// common usage interface
	ImageWhiten::whitenPoint(whiten_Image.rows/2, whiten_Image.cols/2,  20,  20,  50);

	ImageWhiten::whitenRelease();
	waitKey(0);
	destroyWindow("¡¾SRC_WHITEN¡¿");
	destroyWindow("¡¾DST_WHITEN_ALL¡¿");
	destroyWindow("¡¾DST_WHITEN_POINT¡¿");

	system("pause");
	return 0;
}




