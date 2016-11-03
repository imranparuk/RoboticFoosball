
# define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include "Player.h"
#include "PlayerLine.h"

using namespace cv;
using namespace std;

cv::Mat detectPlayerA(cv::Mat dst, Point *positions);
cv::Mat detectPlayerB(cv::Mat dst);
cv::Mat detectBall(cv::Mat dst);
cv::Mat warp(cv::Mat imgOriginal);

Point ballLoc;
Point prevBallLoc;

int main(int argc, char** argv)
{
	VideoCapture cap(0); //capture the video from web cam
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 360);

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	Serial* SP = new Serial("\\\\.\\COM6");    // adjust as needed
	if (SP->IsConnected())
		cout << ("We're connected") << endl;

	Player playa = Player(SP);

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
		cv::Mat dst;
		dst = warp(imgOriginal);
		imshow("orig", imgOriginal);
		Point latestPositions[11];
		Mat dA = detectPlayerA(dst, latestPositions);
		Mat dB = detectBall(dA);
		playa.moveToBall(ballLoc);
		cout << "Player " << "3" <<  ":" << latestPositions[3] << endl;
		imshow("ya", dB);
		
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;

}


cv::Mat detectPlayerA(cv::Mat dst, Point *positions)
{
	Mat imgHSV;
	Mat lower_red_hue_range;
	Mat upper_red_hue_range;
	Mat imgThresholdedPlayerA;
	Point men_left[4];
	Point men_right[7];

	cvtColor(dst, imgHSV, CV_BGR2HSV); //Convert the captured frame from BGR to HSV
	inRange(imgHSV, Scalar(0, 50, 100), Scalar(10, 255, 255), lower_red_hue_range);
	inRange(imgHSV, Scalar(160, 50, 100), Scalar(179, 255, 255), upper_red_hue_range);
	addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, imgThresholdedPlayerA);

	erode(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	dilate(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	std::vector< std::vector<cv::Point> > contoursA;
	std::vector<cv::Point> pointsA;
	cv::findContours(imgThresholdedPlayerA, contoursA, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	Point men[100];
	int count = 0;
	for (int i = 0; i < contoursA.size(); ++i)
	{
		if (contourArea(contoursA[i]) > 50 && contourArea(contoursA[i]) < 600)
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
			cv::polylines(dst, polylines, true, Scalar(255, 255, 255), 2);

			Moments m = moments(contoursA[i], true);
			Point center(m.m10 / m.m00, m.m01 / m.m00);
			men[count] = center;
			count++;
			//cout << "Player A: " << i << center << endl;
			circle(dst, center, 5.0, Scalar(0, 0, 0), 2, 8);
		}
	}

	if (count != 11)
	{
		return dst;
	}

	//for (int i = 0; i < 11;i++)
	//	cout << "Player " << i << ":" << men[i] << endl;
//--------------------------------correct-------------------------
	cout << "CHECK! " << endl;

	int c_left = 0;
	int c_right = 0;

		//  [593, 340] ya
		//	[59.6724, 319.741] ya
		//	[601.672, 49.7414] ya
		//	[70, 36] ya
		
	for (int i = 0; i < 11; i++)
	{
		if ((men[i].x < 150) && (c_left < 4))
		{
			men_left[c_left] = men[i];
			c_left++;
		}
		if ((men[i].x >= 150) && (c_right < 7))
		{
			men_right[c_right] = men[i];
			c_right++;
		}
	}
	if (c_left != 4 && c_right != 7) {
		cout << "error!" << endl;
		return dst;
	}
		

	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			if (men_left[j].x < men_left[i].x)
			{
				Point temp = men_left[i];
				men_left[i] = men_left[j];
				men_left[j] = temp;
			}
		}
	}
	for (int i = 1; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			if (men_left[j].y < men_left[i].y)
			{
				Point temp = men_left[i];
				men_left[i] = men_left[j];
				men_left[j] = temp;
			}
		}
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = i + 1; j < 7; j++)
		{
			if (men_right[j].x < men_right[i].x)
			{
				Point temp = men_right[i];
				men_right[i] = men_right[j];
				men_right[j] = temp;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			if (men_right[j].y < men_right[i].y)
			{
				Point temp = men_right[i];
				men_right[i] = men_right[j];
				men_right[j] = temp;
			}
		}
	}

	for (int i = 4; i < 7; i++)
	{
		for (int j = i + 1; j < 7; j++)
		{
			if (men_right[j].y < men_right[i].y)
			{
				Point temp = men_right[i];
				men_right[i] = men_right[j];
				men_right[j] = temp;
			}
		}
	}


	for (int i = 0; i < 4; i++)
	{
		men[i] = men_left[i];
	}
	for (int i = 0; i < 7; i++)
	{
		men[i + 4] = men_right[i];
	}
	//for (int i = 0; i < 11; i++)
	//	cout << "Player " << i << ":" << men[i] << endl;
	//cout << "Player " << "4" << ":" << men[3] << endl;
	//men is sorted in its correct positions
	for (int i = 0; i < 11; i++)
	{
		positions[i] = men[i];
	}
//----------------------------------------------------------------------------------------

	return dst;
}

cv::Mat detectPlayerB(cv::Mat dst)
{
	cv::Mat imgHSV;

	Mat imgThresholdedPlayerB;
	cvtColor(dst, imgHSV, CV_BGR2HSV); //Convert the captured frame from BGR to HSV
	inRange(imgHSV, Scalar(11, 10, 100), Scalar(40, 255, 255), imgThresholdedPlayerB);

	erode(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	dilate(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	std::vector< std::vector<cv::Point> > contoursB;
	std::vector<cv::Point> pointsB;
	cv::findContours(imgThresholdedPlayerB, contoursB, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contoursB.size(); ++i)
	{
		if (contourArea(contoursB[i]) > 50 && contourArea(contoursB[i]) < 600)
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
			cv::polylines(dst, polylines, true, Scalar(0, 0, 0), 2);

			Moments m = moments(contoursB[i], true);
			Point center(m.m10 / m.m00, m.m01 / m.m00);
			circle(dst, center, 5.0, Scalar(70, 0, 255), 2, 8);
		}

	}
	return dst;
}

cv::Mat detectBall(cv::Mat dst)
{
	cv::Mat imgHSV;

	Mat imgThresholdedBall;
	cvtColor(dst, imgHSV, CV_BGR2HSV); //Convert the captured frame from BGR to HSV
	inRange(imgHSV, Scalar(100, 0, 100), Scalar(170, 255, 255), imgThresholdedBall);

	erode(imgThresholdedBall, imgThresholdedBall, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholdedBall, imgThresholdedBall, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	dilate(imgThresholdedBall, imgThresholdedBall, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholdedBall, imgThresholdedBall, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	std::vector< std::vector<cv::Point> > contoursBall;
	std::vector<cv::Point> pointsBall;
	cv::findContours(imgThresholdedBall, contoursBall, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contoursBall.size(); ++i)
	{
		if (contourArea(contoursBall[i]) > 100 && contourArea(contoursBall[i]) < 190) {

			Point2f center;
			float radius;

			minEnclosingCircle(contoursBall[i], center, radius);
			if (radius > 6 && radius < 10) {
				cv::circle(dst, center, radius, Scalar(255, 0, 0), 2);
				prevBallLoc = ballLoc;
				ballLoc = (Point)center;
			}
		}

	}
	return dst;
}

cv::Mat warp(cv::Mat imgOriginal)
{
	cv::Mat imgHSV;

	Mat imgThresholdCorners;
	cvtColor(imgOriginal, imgHSV, CV_BGR2HSV); //Convert the captured frame from BGR to HSV
	inRange(imgHSV, Scalar(35, 0, 255), Scalar(110, 100, 255), imgThresholdCorners);

	erode(imgThresholdCorners, imgThresholdCorners, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholdCorners, imgThresholdCorners, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	dilate(imgThresholdCorners, imgThresholdCorners, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholdCorners, imgThresholdCorners, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	Mat dst;
	std::vector< std::vector<cv::Point> > contoursCorners;
	std::vector<cv::Point> pointsCorners;
	cv::findContours(imgThresholdCorners, contoursCorners, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	Point2f myMat[4] = { 0 };
	int counter = 0;
	int hasEnough = 0;
	int pos1 = -1;
	int pos2 = -1;
	int pos3 = -1;
	int pos4 = -1;

	for (int i = 0; i < contoursCorners.size(); ++i)
	{
		if (i > 3) return imgOriginal;
		Point2f center;
		float radius;

		minEnclosingCircle(contoursCorners[i], center, radius);

		cv::circle(imgOriginal, center, radius, Scalar(90, 30, 44), 2);

		myMat[counter] = center;
		counter++;
		if (i == 3) {
			hasEnough = 1;
		}
		else continue;
	}

	if (hasEnough)
	{

		int x1_left = -1;
		int x2_left = -1;
		int x1_right = -1;
		int x2_right = -1;
		for (int i = 0; i < 4; i++)
		{
			if (myMat[i].x < 320)
			{
				if (x1_left == -1)
					x1_left = i;
				else
					x2_left = i;
			}
			else
			{
				if (x1_right == -1)
					x1_right = i;
				else
					x2_right = i;
			}
		}


		if (myMat[x1_left].y < myMat[x2_left].y)
		{
			pos1 = x1_left;
			pos2 = x2_left;
		}
		else
		{
			pos1 = x2_left;
			pos2 = x1_left;
		}
		if (myMat[x1_right].y < myMat[x2_right].y)
		{
			pos4 = x1_right;
			pos3 = x2_right;
		}
		else
		{
			pos4 = x2_right;
			pos3 = x1_right;
		}

		//cout << "SPostion 1: " << myMat[pos1] << " ";
		//cout << "Postion 2: " << myMat[pos2] << " ";
		//cout << "Postion 3: " << myMat[pos3] << " ";
		//cout << "Postion 4: " << myMat[pos4] << endl;
	}
	else {
		pos1 = 0;
		pos2 = 1;
		pos3 = 2;
		pos4 = 3;
	}

	double resW2 = cv::norm(myMat[pos1] - myMat[pos4]);
	double resW1 = cv::norm(myMat[pos2] - myMat[pos3]);

	double resH2 = cv::norm(myMat[pos1] - myMat[pos2]);
	double resH1 = cv::norm(myMat[pos4] - myMat[pos3]);


	double maxW = 0;
	((resW1 >= resW2) ? (maxW = resW1) : (maxW = resW2));

	double maxH = 0;
	((resH1 >= resH2) ? (maxH = resH1) : (maxH = resH2));

	if (maxH > 400)
		return imgOriginal;

	Point2f pt1(0, 0); //position 1
	Point2f pt2(0, maxH - 1);//position 2
	Point2f pt3(maxW - 1, maxH - 1);//position 3
	Point2f pt4(maxW - 1, 0); //postion 4
	//

	//cout << "Point 1: " << pt1 << " ";
	//cout << "Point 2: " << pt4 << " ";
	//cout << "Point 3: " << pt3 << " ";
	//cout << "Point 4: " << pt2 << endl << endl;
	//1st-------2nd
	//|			|
	//|			|
	//|			|
	//3rd------- 4th

	//Point2f dest_points[4] = { pt2, pt1, pt3, pt4 };
	//Point2f dest_points[4] = { pt4, pt3, pt1, pt2 };
	//Point2f dest_points[4] = { pt3, pt2, pt4, pt1}; //perfecto
	//Point2f dest_points[4] = { pt3, pt2, pt1, pt4 };
	//Point2f dest_points[4] =  { pt1, pt3, pt2, pt4 }; //perfecto
	//Point2f dest_points[4] = { pt4, pt3, pt2, pt1 };

	vector<Point2f> corners;
	corners.push_back(myMat[0]);
	corners.push_back(myMat[1]);
	corners.push_back(myMat[2]);
	corners.push_back(myMat[3]);


	// Corners of the destination image
	// output is the output image, should be defined before this operation
	vector<cv::Point2f> output_corner;
	//output_corner.push_back(pt2);
	//output_corner.push_back(pt3);
	//output_corner.push_back(pt4);
	//output_corner.push_back(pt1);
	output_corner.push_back(pt3);
	output_corner.push_back(pt2);
	output_corner.push_back(pt4);
	output_corner.push_back(pt1);

	// Get transformation matrix
	Mat H = getPerspectiveTransform(corners, output_corner);
	//cv::Mat dst;
	//Mat transform_matrix = cv::getPerspectiveTransform(nMat, dest_points);
	warpPerspective(imgOriginal, dst, H, cv::Size(maxW, maxH));

	//cv::Point2f src_vertices[3];
	//src_vertices[0] = myMat[0];
	//src_vertices[1] = myMat[2];
	//src_vertices[2] = myMat[3];
	////src_vertices[3] = not_a_rect_shape[3];

	//Point2f dst_vertices[3];
	//dst_vertices[0] = pt1;
	//dst_vertices[1] = pt3;
	//dst_vertices[2] = pt4;


	//Mat warpAffineMatrix = getAffineTransform(src_vertices, dst_vertices);

	//warpAffine(imgOriginal, dst, warpAffineMatrix, Size(maxW, maxH), INTER_LINEAR, BORDER_CONSTANT);

	return dst;
}
