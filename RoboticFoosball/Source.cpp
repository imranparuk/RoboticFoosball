/*#include <opencv2/opencv.hpp>


using namespace cv;

int main(int, char**)
{
VideoCapture cap(0); // open the default camera
if (!cap.isOpened())  // check if we succeeded
return -1;

Mat edges;
namedWindow("edges", 1);
for (;;)
{
Mat frame;
cap >> frame; // get a new frame from camera
cvtColor(frame, edges, COLOR_BGR2GRAY);
GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
Canny(edges, edges, 0, 30, 3);
imshow("edges", frame);
if (waitKey(30) >= 0) break;
}
// the camera will be deinitialized automatically in VideoCapture destructor
return 0;
}*/


#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	VideoCapture cap(0); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	while (true)
	{
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat imgHSV;
		Mat imgThresholdedPlayerA;
		Mat imgThresholdedPlayerB;


		cvtColor(imgOriginal, imgHSV, CV_BGR2HSV); //Convert the captured frame from BGR to HSV
		//cv::inRange(imgHSV, cv::Scalar(0, 0, 0, 0), cv::Scalar(180, 255, 30, 0), imgThresholded);

		cv::Mat lower_red_hue_range;
		cv::Mat upper_red_hue_range;
		cv::inRange(imgHSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
		cv::inRange(imgHSV, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

		Mat yellow;
		
		inRange(imgHSV, Scalar(11, 0, 100), Scalar(40, 255, 255), imgThresholdedPlayerB);

		// Combine the above two images

		cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, imgThresholdedPlayerA);
		
		//addWeighted(yellow, 1.0, imgThresholded, 1.0, 0.0, imgThresholded);

		//morphological opening (remove small objects from the foreground)
		erode(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological opening (remove small objects from the foreground)
		erode(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		std::vector< std::vector<cv::Point> > contoursA;
		std::vector<cv::Point> pointsA;
		cv::findContours(imgThresholdedPlayerA, contoursA, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		std::vector< std::vector<cv::Point> > contoursB;
		std::vector<cv::Point> pointsB;
		cv::findContours(imgThresholdedPlayerB, contoursB, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		// And process the points or contours to pick up specified object.


		for (int i = 0; i < contoursA.size(); ++i)
		{

			//Rect rect;
			RotatedRect rotate_rect;
			Point2f points[4];

			//rect = boundingRect(contoursB[i]);
			rotate_rect = minAreaRect(contoursA[i]);

			rotate_rect.points(points);

			//draw them on the bounding image.
			//cv::rectangle(imgOriginal, rect, Scalar(0, 0, 0), 2);
			vector< vector< Point> > polylines;
			polylines.resize(1);
			for (int j = 0; j < 4; ++j)
				polylines[0].push_back(points[j]);

			//draw them on the bounding image.
			cv::polylines(imgOriginal, polylines, true, Scalar(255, 255, 255), 2);
			
			Moments m = moments(contoursA[i], true);
			Point center(m.m10 / m.m00, m.m01 / m.m00);
			circle(imgOriginal, center, 5.0, Scalar(0, 0, 0), 2, 8);
		}

		for (int i = 0; i < contoursB.size(); ++i)
		{

			//Rect rect;
			RotatedRect rotate_rect;
			Point2f points[4];

			//rect = boundingRect(contoursB[i]);
			rotate_rect = minAreaRect(contoursB[i]);

			rotate_rect.points(points);

			//draw them on the bounding image.
			//cv::rectangle(imgOriginal, rect, Scalar(0, 0, 0), 2);
			vector< vector< Point> > polylines;
			polylines.resize(1);
			for (int j = 0; j < 4; ++j)
				polylines[0].push_back(points[j]);

			//draw them on the bounding image.
			cv::polylines(imgOriginal, polylines, true, Scalar(0, 0, 0), 2);

			Moments m = moments(contoursB[i], true);
			Point center(m.m10 / m.m00, m.m01 / m.m00);
			circle(imgOriginal, center, 5.0, Scalar(70, 0, 255), 2, 8);

		}

		//imshow("Thresholded Image", imgThresholded); //show the thresholded image
		imshow("Original", imgOriginal); //show the original image

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;

}