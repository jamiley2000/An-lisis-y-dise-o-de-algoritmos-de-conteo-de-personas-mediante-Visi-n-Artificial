#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main() {

	VideoCapture Cap("Videos/Video_R.mp4");
	Mat Frame1, Frame2, Frame3;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage;
	Mat cannyImage, cannyDilate;
	vector<vector<Point>> contours, contornos, prueba;
	vector<Vec4i> hierarchy;
	Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
	int ypos_AS = 0, ypos_AE = 0;
	int frames = 0;
	int contador = 0;
	int area;
	bool flagE = false, flagS = false;

	Ptr<BackgroundSubtractorMOG2> subtractor = createBackgroundSubtractorMOG2();
	/*Cap.read(Frame1);
	resize(Frame1, Frame1, Size(), 0.5, 0.5);
	cvtColor(Frame1, grayImage1, COLOR_BGR2GRAY);
	GaussianBlur(grayImage1, grayImage1, Size(5, 5), 0);*/
	while (1) {

		//-----------------------------------------------

		Cap.read(Frame1);
		resize(Frame1, Frame1, Size(), 0.5, 0.5);
		/*GaussianBlur(Frame1, Frame1, Size(5, 5), 0);*/
		subtractor->apply(Frame1, Frame2);
		/*subtractor->apply(Frame1, Frame3);*/
		/*threshold(Frame2, grayImage1, 10, 255, THRESH_BINARY);*/
		/*threshold(Frame3, grayImage2, 10, 255, THRESH_BINARY);*/
		imshow("Frame1", Frame1);
		imshow("Frame2", Frame2);
		/*imshow("Frame3", Frame3);*/
		/*imshow("Grayimage1", grayImage1);
		imshow("Grayimage2", grayImage2);*/

		//------------------------------------------------


		//Cap.read(Frame2);
		//Cap.read(Frame1);
		//
		//
		//
		//
		//resize(Frame2, Frame2, Size(), 0.5, 0.5);
		//resize(Frame1, Frame1, Size(), 0.5, 0.5);
		//cvtColor(Frame1, grayImage1, COLOR_BGR2GRAY);
		//cvtColor(Frame2, grayImage2, COLOR_BGR2GRAY);
		//GaussianBlur(grayImage2, grayImage2, Size(5, 5), 0);
		//GaussianBlur(grayImage1, grayImage1, Size(5, 5), 0);
		//absdiff(grayImage1, grayImage2, differenceImage);
		//imshow("diff", differenceImage);
		//threshold(differenceImage, thresholdImage, 20, 255, THRESH_BINARY);
		////imshow("threshhold1", thresholdImage);
		//GaussianBlur(thresholdImage, thresholdImage, Size(5, 5), 0);
		////imshow("gausean", thresholdImage);
		//threshold(thresholdImage, thresholdImage, 20, 255, THRESH_BINARY);
		////imshow("threshhold2", thresholdImage);


		//--------------------------------------------------

		waitKey(1);
	}
	return 0;

}