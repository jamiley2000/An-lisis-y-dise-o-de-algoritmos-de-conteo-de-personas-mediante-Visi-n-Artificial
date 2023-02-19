#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {

	bool pause = false;
	bool debugMode = false;
	bool dMode = false;
	VideoCapture Cap(0);
	Mat Frame1, Frame2;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage;
	
	
	Cap.read(Frame1);
	imshow("Frame", Frame1);
	cvtColor(Frame1, grayImage1, COLOR_BGR2GRAY);
	
	while (1) {

		Cap.read(Frame2);
		imshow("Frames restantes", Frame2);
		cvtColor(Frame2, grayImage2, COLOR_BGR2GRAY);
		absdiff(grayImage1, grayImage2, differenceImage);
		threshold(differenceImage, thresholdImage, 40, 255, THRESH_BINARY);
		blur(thresholdImage, thresholdImage, Size(15, 15));
		threshold(thresholdImage, thresholdImage, 40, 255, THRESH_BINARY);
		if (debugMode == true) {
			imshow("gray1", grayImage1);
			imshow("gray2", grayImage2);
			imshow("differenceImage", differenceImage);
			imshow("Threshold", thresholdImage);
			dMode = true;
		}
		else {
			//if not in debug mode, destroy the windows so we don't see them anymore
			if (dMode) {
				destroyWindow("gray1");
				destroyWindow("gray2");
				destroyWindow("differenceImage");
				destroyWindow("Threshold");
				dMode = false;
			}
		}
		switch (waitKey(1)) {

		case 27: //'esc' key has been pressed, exit program.
			return 0;
		case 100: //'d' has been pressed. this will debug mode
			debugMode = !debugMode;
			if (debugMode == false) cout << "Debug mode disabled." << endl;
			else cout << "Debug mode enabled." << endl;
			break;
		case 112: //'p' has been pressed. this will pause/resume the code.
			pause = !pause;
			if (pause == true) {
				cout << "Code paused, press 'p' again to resume" << endl;
				while (pause == true) {
					//stay in this loop until 
					switch (waitKey()) {
						//a switch statement inside a switch statement? Mind blown.
					case 112:
						//change pause back to false
						pause = false;
						cout << "Code Resumed" << endl;
						break;
					}
				}
			}



		}
	}
	return 0;

}