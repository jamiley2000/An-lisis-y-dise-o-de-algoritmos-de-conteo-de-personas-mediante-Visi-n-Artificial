#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main() {

	VideoCapture Cap("Videos/sample-video.avi");
	Mat Frame1, Frame2, Frame3, Frame4;
	Mat grayImage1, grayImage2;
	Mat differenceImage, mask;
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

	int blur = 21;
	int canny_low = 15;
	int canny_high = 150;
	int min_area = 0.0005;
	int max_area = 0.95;
	int dilate_iter = 10;
	int erode_iter = 10;
	Scalar mask_color = (0.0, 0.0, 0.0);
	Ptr<BackgroundSubtractorKNN> subtractor = createBackgroundSubtractorKNN();

	while (1) {

		//-----------------------------------------------

		Cap.read(Frame1);
		cvtColor(Frame1, grayImage1, COLOR_BGR2GRAY);
		//GaussianBlur(Frame1, Frame1, Size(3, 3), 0);
		Canny(grayImage1, cannyImage, canny_low, canny_high);

		//subtractor->apply(Frame1, Frame2);
		
		/*dilate(cannyImage, cannyImage, kernel);
		erode(cannyImage, cannyImage, kernel);*/

		findContours(cannyImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		Mat mask = Mat::zeros(Frame1.rows, Frame1.cols, CV_8U);

		for (int i = 0; i < contours.size(); i++) {

			int area = contourArea(contours[i]);
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
				cout << Frame2.size().width << " " << Frame2.size().height << endl;

			}


		}

		imshow("Frame1", Frame1);
	


		

		waitKey(1);
	}
	return 0;

}