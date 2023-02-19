#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {

	VideoCapture cap("Videos/Arriba2.avi");
	Mat frame;
	CascadeClassifier peopleCascade;
	peopleCascade.load("Videos/haarcascade_fullbody.xml");

	vector<Rect> people;
	
	
	

	// Loop through available frames
	while (cap.read(frame)) {
		resize(frame, frame, Size(), 0.3, 0.3);
		peopleCascade.detectMultiScale(frame, people);
		for (int i = 0; i < people.size(); i++) {
			
			rectangle(frame, people[i].tl(), people[i].br(), Scalar(255, 0, 255), 3);
		
		}
		imshow("frame", frame);
		waitKey(1);

	} 

	return 0;

}