#include <opencv2/opencv.hpp>
#include <random>


using namespace cv;
using namespace std;

int computeMedian(vector<int> elements)
{
	nth_element(elements.begin(), elements.begin() + elements.size() / 2, elements.end());

	//sort(elements.begin(),elements.end());
	return elements[elements.size() / 2];
}

cv::Mat compute_median(std::vector<cv::Mat> vec)
{
	// Note: Expects the image to be CV_8UC3
	cv::Mat medianImg(vec[0].rows, vec[0].cols, CV_8UC3, cv::Scalar(0, 0, 0));

	for (int row = 0; row < vec[0].rows; row++)
	{
		for (int col = 0; col < vec[0].cols; col++)
		{
			std::vector<int> elements_B;
			std::vector<int> elements_G;
			std::vector<int> elements_R;

			for (int imgNumber = 0; imgNumber < vec.size(); imgNumber++)
			{
				int B = vec[imgNumber].at<cv::Vec3b>(row, col)[0];
				int G = vec[imgNumber].at<cv::Vec3b>(row, col)[1];
				int R = vec[imgNumber].at<cv::Vec3b>(row, col)[2];

				elements_B.push_back(B);
				elements_G.push_back(G);
				elements_R.push_back(R);
			}

			medianImg.at<cv::Vec3b>(row, col)[0] = computeMedian(elements_B);
			medianImg.at<cv::Vec3b>(row, col)[1] = computeMedian(elements_G);
			medianImg.at<cv::Vec3b>(row, col)[2] = computeMedian(elements_R);
		}
	}
	return medianImg;
}

int main() {

	VideoCapture Cap;
	Mat Frame1, Frame2, Frame3;
	Mat grayImage1, grayImage2;
	Mat hist;
	Mat differenceImage;
	Mat thresholdImage;
	Mat cannyImage, cannyDilate, cannyImage2, cannyDilate2;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
	int ypos_AS = 0, ypos_AE = 0;
	int frames = 0;
	int contador = 0;
	bool flagE = false, flagS = false;
	int morph_size = 5;
	// Create structuring element
	Mat element = getStructuringElement(
		MORPH_RECT,
		Size(2 * morph_size + 1,
			2 * morph_size + 1),
		Point(morph_size, morph_size));


	Cap.open("Videos/TestVideo/TestVideo_15fps.avi");
	Ptr<BackgroundSubtractorKNN> subtractor = createBackgroundSubtractorKNN();
	//Ptr<BackgroundSubtractorMOG2> subtractor = createBackgroundSubtractorMOG2();

	
	default_random_engine generator;
	uniform_int_distribution<int>distribution(0, Cap.get(CAP_PROP_FRAME_COUNT));

	vector<Mat> Frames;
	Mat frame;

	for (int i = 0; i < 100; i++)
	{
		int fid = distribution(generator);
		Cap.set(CAP_PROP_POS_FRAMES, fid);
		Mat frame;
		Cap >> frame;
		if (frame.empty())
			continue;
		//resize(frame, frame, Size(), 0.3, 0.3);
		Frames.push_back(frame);
	}
	// Calculate the median along the time axis
	Mat medianFrame = compute_median(Frames);

	//while (1) {

	//	Cap.read(Frame1);
	//	cvtColor(Frame1, grayImage1, COLOR_BGR2GRAY);
	//	cvtColor(medianFrame, grayImage2, COLOR_BGR2GRAY);
	//	//equalizeHist(grayImage1, hist);
	//	//GaussianBlur(hist, hist, Size(7, 7), 0);
	//	
	//	absdiff(grayImage1, grayImage2, differenceImage);
	//	//subtractor->apply(differenceImage, thresholdImage);
	//	//threshold(differenceImage, thresholdImage, 10, 255, THRESH_BINARY);
	//	/*erode(thresholdImage, thresholdImage, hist, Point(-1, -1), 2);
	//	dilate(thresholdImage, thresholdImage, hist, Point(-1, -1), 2);*/

	//	/*morphologyEx(thresholdImage, Frame2, MORPH_CLOSE, element, Point(-1, -1), 2);
	//	morphologyEx(Frame2, Frame2, MORPH_OPEN, element, Point(-1, -1), 2);

	//	Canny(Frame2, cannyImage, 100, 200);
	//	dilate(cannyImage, cannyDilate, hist);


	//	findContours(cannyImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


	//	for (int i = 0; i < contours.size(); i++) {

	//		int area = contourArea(contours[i]);
	//		if (area > 20) {

	//			objectBoundingRectangle = boundingRect(contours[i]);
	//			int x = objectBoundingRectangle.x;
	//			int y = objectBoundingRectangle.y;
	//			int w = objectBoundingRectangle.width;
	//			int h = objectBoundingRectangle.height;
	//			int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
	//			int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
	//			cout << "xpos: " << xpos << " " << "ypos: " << ypos << " "
	//				<< "width: " << objectBoundingRectangle.width << " " << "height: " << objectBoundingRectangle.height << endl;
	//			rectangle(Frame1, Point(x, y), Point(w + x, h + y), Scalar(0, 255, 0), 2);
	//			circle(Frame1, Point(xpos, ypos), 5, Scalar(0, 0, 255), 2);
	//			cout << Frame1.size().width << " " << Frame1.size().height << endl;

	//		}


	//	}

	//	line(Frame1, Point(0, (Frame1.size().height / 2)), Point((Frame1.size().width/2), (Frame1.size().height / 2)), Scalar(255, 0, 0), 2);

	//	imshow("original", Frame1);
	//	imshow("sustractor", Frame2);
	//	/*imshow("gray", grayImage1);
	//	imshow("hist", hist);
	//	imshow("KNN", Frame2);*/
	//	imshow("frame", medianFrame);
	//	imshow("diff", differenceImage);
	//	/*imshow("threshold", thresholdImage);
	//	imshow("canny", cannyDilate);*/

	//	waitKey(1);
	//}
	imwrite("Videos/TestVideo_100frames.jpg", medianFrame);
	return 0;

}