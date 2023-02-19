#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main() {

	VideoCapture Cap("Videos/sample-video.avi");
	Mat Frame1, Frame2, Frame3, Frame4;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage, kernel;
	Mat cannyImage, cannyDilate;
	vector<vector<Point>> contours, contornos, prueba;
	vector<Vec4i> hierarchy;
	Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
	int ypos_AS = 0, ypos_AE = 0;
	int frames = 0;
	int contador = 0;
	int area;
	bool flagE = false, flagS = false;
	RNG rng(12345);


	//Ptr<BackgroundSubtractorKNN> subtractor = createBackgroundSubtractorKNN();
	Ptr<BackgroundSubtractorMOG2> subtractor = createBackgroundSubtractorMOG2();
	/*Cap.read(Frame1);
	resize(Frame1, Frame1, Size(), 0.5, 0.5);
	cvtColor(Frame1, grayImage1, COLOR_BGR2GRAY);
	GaussianBlur(grayImage1, grayImage1, Size(5, 5), 0);*/
	while (1) {

		//-----------------------------------------------
		
		Cap.read(Frame1);
		//resize(Frame1, Frame1, Size(), 0.5, 0.5);
		GaussianBlur(Frame1, Frame1, Size(3, 3), 0);
		subtractor->apply(Frame1, Frame2);

		/*erode(Frame2, Frame2, kernel, Point(-1, -1), 1);
		dilate(Frame2, Frame2, kernel, Point(-1, -1), 1);*/
		
		findContours(Frame2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


		for (int i = 0; i < contours.size(); i++) {

			int area = contourArea(contours[i]);
			Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
			/*drawContours(Frame1, contours, (int)i, color, 2, LINE_8, hierarchy, 0);*/
			if (area > 20) {

				objectBoundingRectangle = boundingRect(contours[i]);
				int x = objectBoundingRectangle.x;
				int y = objectBoundingRectangle.y;
				int w = objectBoundingRectangle.width;
				int h = objectBoundingRectangle.height;
				int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
				int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
				cout << "xpos: " << xpos << " " << "ypos: " << ypos << " "
					<< "width: " << objectBoundingRectangle.width << " " << "height: " << objectBoundingRectangle.height << endl;
				rectangle(Frame1, Point(x, y), Point(w + x, h + y), Scalar(0, 255, 0), 2);
				circle(Frame1, Point(xpos, ypos), 5, Scalar(0, 0, 255), 2);
				//cout << Frame2.size().width << " " << Frame2.size().height << endl;

			}


		}
		
		
		imshow("Frame1", Frame1);
		imshow("Frame2", Frame2);

		
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