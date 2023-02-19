#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main() {

	VideoCapture Cap;
	Mat Frame1, Frame2, Prueba;
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



	vector<Point2f> features_prev, features_next;
	vector<uchar> status;
	vector<float> err;


	Cap.open("Videos/Arriba_E.avi");

	while (1) {

		Cap.read(Frame1);
		Cap.read(Frame2);
		resize(Frame1, Frame1, Size(), 0.3, 0.3, INTER_AREA);
		resize(Frame2, Frame2, Size(), 0.3, 0.3, INTER_AREA);
		cvtColor(Frame1, grayImage1, COLOR_BGR2GRAY);
		cvtColor(Frame2, grayImage2, COLOR_BGR2GRAY);
		absdiff(grayImage1, grayImage2, differenceImage);
		threshold(differenceImage, thresholdImage, 20, 255, THRESH_BINARY);
		GaussianBlur(thresholdImage, thresholdImage, Size(5, 5), 0);
		threshold(thresholdImage, thresholdImage, 20, 255, THRESH_BINARY);
		Canny(thresholdImage, cannyImage, 20, 75);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
		dilate(cannyImage, cannyDilate, kernel);


		findContours(cannyDilate, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);



		// for every frame



		contornos.clear();

		for (int i = 0; i < contours.size(); i++) {

			cout << contourArea(contours[i]) << endl;
			area = contourArea(contours[i]);
			if (area > 5000)
				contornos.push_back(contours[i]);

		}

		//cout << contours.size() << " : " << contornos.size() << endl;

		for (int i = 0; i < contornos.size(); i++) {

			objectBoundingRectangle = boundingRect(contornos[i]);
			int x = objectBoundingRectangle.x;
			int y = objectBoundingRectangle.y;
			int w = objectBoundingRectangle.width;
			int h = objectBoundingRectangle.height;
			int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
			int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
			/*cout << "xpos: " << xpos << " " << "ypos: " << ypos << " "
				<< "width: " << objectBoundingRectangle.width << " " << "height: " << objectBoundingRectangle.height << endl;*/
			rectangle(Frame2, Point(x, y), Point(w + x, h + y), Scalar(0, 255, 0), 2);
			circle(Frame2, Point(xpos, ypos), 20, Scalar(0, 0, 255), 2);
			//cout << Frame2.size().width << " " << Frame2.size().height << endl;


		}





		line(Frame2, Point(0, 288), Point(324, 288), Scalar(255, 0, 0), 3);

		imshow("Contornos", Frame2);
		imshow("Contornos 2", cannyDilate);

		waitKey(1);
	}
	return 0;

}