
//# define _CRT_SECURE_NO_WARNINGS
//
//
//#include <iostream>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//using namespace cv;
//using namespace std;
//
//int main(int argc, char** argv)
//{
//	VideoCapture cap(0); //capture the video from web cam
//	cap.set(CV_CAP_PROP_FRAME_WIDTH, 600);
//	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 100);
//
//	if (!cap.isOpened())  // if not success, exit program
//	{
//		cout << "Cannot open the web cam" << endl;
//		return -1;
//	}
//
//	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
//
//	int iLowH = 0;
//	int iHighH = 179;
//
//	int iLowS = 0;
//	int iHighS = 255;
//
//	int iLowV = 0;
//	int iHighV = 255;
//
//	//Create trackbars in "Control" window
//	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
//	cvCreateTrackbar("HighH", "Control", &iHighH, 179);
//
//	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
//	cvCreateTrackbar("HighS", "Control", &iHighS, 255);
//
//	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
//	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
//
//
//	while (true)
//	{
//
//		Mat imgOriginal;
//
//		bool bSuccess = cap.read(imgOriginal); // read a new frame from video
//
//		if (!bSuccess) //if not success, break loop
//		{
//			cout << "Cannot read a frame from video stream" << endl;
//			break;
//		}
//		
//
//		Mat imgHSV;
//		Mat imgThresholdedPlayerA;
//		Mat imgThresholdedPlayerB;
//		Mat table;
//		Mat imgThresholdedBall;
//		Mat imgThresholdCorners;
//		cvtColor(imgOriginal, imgHSV, CV_BGR2HSV); //Convert the captured frame from BGR to HSV
//		//morphological opening (remove small objects from the foreground)
//		        
//		Mat lower_red_hue_range;
//		Mat upper_red_hue_range;
//
//		inRange(imgHSV, Scalar(0  , 50, 100), Scalar(10 , 255, 255), lower_red_hue_range);
//		inRange(imgHSV, Scalar(160, 50, 100), Scalar(179, 255, 255), upper_red_hue_range);
//		
//		inRange(imgHSV, Scalar(11 , 10, 100), Scalar(40 , 255, 255), imgThresholdedPlayerB);
//		inRange(imgHSV, Scalar(130, 0 , 100), Scalar(150, 255, 255), imgThresholdedBall);
//		inRange(imgHSV, Scalar(40 , 0 , 255), Scalar(100, 100, 255), imgThresholdCorners);
//
//		addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, imgThresholdedPlayerA);
//		
//		//addWeighted(yellow, 1.0, imgThresholded, 1.0, 0.0, imgThresholded);
//
//		erode(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		dilate(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		erode(imgThresholdedPlayerA, imgThresholdedPlayerA, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		erode(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		dilate(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		erode(imgThresholdedPlayerB, imgThresholdedPlayerB, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		erode(imgThresholdedBall, imgThresholdedBall, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(imgThresholdedBall, imgThresholdedBall, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		dilate(imgThresholdedBall, imgThresholdedBall, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		erode(imgThresholdedBall, imgThresholdedBall, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		erode(imgThresholdCorners, imgThresholdCorners, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(imgThresholdCorners, imgThresholdCorners, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//	
//		//imshow("Thresholded Image B", imgThresholdedPlayerA); //show the thresholded image
//
//
//		std::vector< std::vector<cv::Point> > contoursA;
//		std::vector<cv::Point> pointsA;
//		cv::findContours(imgThresholdedPlayerA, contoursA, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
//
//		std::vector< std::vector<cv::Point> > contoursB;
//		std::vector<cv::Point> pointsB;
//		cv::findContours(imgThresholdedPlayerB, contoursB, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
//
//		std::vector< std::vector<cv::Point> > contoursBall;
//		std::vector<cv::Point> pointsBall;
//		cv::findContours(imgThresholdedBall, contoursBall, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
//
//		std::vector< std::vector<cv::Point> > contoursCorners;
//		std::vector<cv::Point> pointsCorners;
//		cv::findContours(imgThresholdCorners, contoursCorners, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
//		//3.3 find bounding of each contour, and draw it on the source image.
//
//		
//		Point2f myMat[4] = { 0 };
//		int counter = 0;
//
//		for (int i = 0; i < contoursCorners.size(); ++i)
//		{
//
//			Point2f center;
//			float radius;
//
//			minEnclosingCircle(contoursCorners[i], center, radius);
//
//			cv::circle(imgOriginal, center, radius, Scalar(90, 30, 44), 2);
//
//			//myMat[counter] = center;
//			//counter++;
//			if (i == 3) cout << center << endl;
//			
//
//		}
//		
//
//		Point2f my1(67.5, 314);
//		Point2f my2(597.013, 305.851);
//		Point2f my3(595.083, 13.7222);
//		Point2f my4(67.5, 13);    
//        //Point2f myMat[4] = { my1, my2, my3, my4 };
//		
//		//[67.5, 314] : [597.5, 305.643] : [57.5, 28] : [595.143, 14]
//		//[67.5, 314] : [597.013, 305.851] : [57.5, 28] : [595.083, 13.7222]
//		//	pt4					pt3			pt1				pt2
//		Point2f newmyMat[4] = { 0 };
//		int lowestIndex = -1;
//
//		Point2f lowestP = 0;
//		Point2f HighestP = 0;
//		for (int k = 0; k < 4; k++)
//		{
//			
//
//		}
//		double resW2 = cv::norm(myMat[4] - myMat[3]);
//		double resW1 = cv::norm(myMat[1] - myMat[2]);
//
//		double resH2 = cv::norm(myMat[4] - myMat[2]);
//		double resH1 = cv::norm(myMat[3] - myMat[1]);
//
//		cout << myMat[0] << " : " << myMat[1] << " : " << myMat[2] << " : " << myMat[3] << endl;
//
//		double maxW = 0;
//		((resW1 >= resW2) ? (maxW = resW1) : (maxW = resW2));
//		
//		double maxH = 0;
//		((resH1 >= resH2) ? (maxH = resH1) : (maxH = resH2));
//
//		Point2f pt1(0, 0);
//		Point2f pt2(maxW-1, 0);
//		Point2f pt3(maxW-1, maxH-1);
//		Point2f pt4(0, maxH-1);
//
//		//Point2f dest_points[4] = { pt1, pt2, pt3, pt4 };
//		Point2f dest_points[4] = { pt4, pt3, pt1, pt2 };
//
//		//4312
//	
//
//
//		for (int i = 0; i < contoursBall.size(); ++i)
//		{
//			if (contourArea(contoursBall[i]) < 50 || contourArea(contoursBall[i]) > 100) continue;
//
//			Point2f center;
//			float radius;
//	
//			minEnclosingCircle(contoursBall[i], center, radius);
//
//			cv::circle(imgOriginal, center, radius, Scalar(0, 0, 0), 2);
//
//		}
//
//		for (int i = 0; i < contoursA.size(); ++i)
//		{
//			if (contourArea(contoursA[i]) < 50 || contourArea(contoursA[i]) > 600) continue;
//			
//			//Rect rect;
//			RotatedRect rotate_rect;
//			Point2f points[4];
//
//			//rect = boundingRect(contoursB[i]);
//			rotate_rect = minAreaRect(contoursA[i]);
//
//			rotate_rect.points(points);
//
//			//draw them on the bounding image.
//			//cv::rectangle(imgOriginal, rect, Scalar(0, 0, 0), 2);
//			vector< vector< Point> > polylines;
//			polylines.resize(1);
//			for (int j = 0; j < 4; ++j)
//				polylines[0].push_back(points[j]);
//
//			//draw them on the bounding image.
//			cv::polylines(imgOriginal, polylines, true, Scalar(255, 255, 255), 2);
//			
//			Moments m = moments(contoursA[i], true);
//			Point center(m.m10 / m.m00, m.m01 / m.m00);
//			circle(imgOriginal, center, 5.0, Scalar(0, 0, 0), 2, 8);
//		}
//
//		for (int i = 0; i < contoursB.size(); ++i)
//		{
//			if (contourArea(contoursB[i]) < 50 || contourArea(contoursB[i]) > 600) continue;
//			//Rect rect;
//			RotatedRect rotate_rect;
//			Point2f points[4];
//
//			//rect = boundingRect(contoursB[i]);
//			rotate_rect = minAreaRect(contoursB[i]);
//
//			rotate_rect.points(points);
//
//			//draw them on the bounding image.
//			//cv::rectangle(imgOriginal, rect, Scalar(0, 0, 0), 2);
//			vector< vector< Point> > polylines;
//			polylines.resize(1);
//			for (int j = 0; j < 4; ++j)
//				polylines[0].push_back(points[j]);
//
//			//draw them on the bounding image.
//			cv::polylines(imgOriginal, polylines, true, Scalar(0, 0, 0), 2);
//
//			Moments m = moments(contoursB[i], true);
//			Point center(m.m10 / m.m00, m.m01 / m.m00);
//			circle(imgOriginal, center, 5.0, Scalar(70, 0, 255), 2, 8);
//			
//		}
//
//		cv::Mat dst;
//		Mat transform_matrix = cv::getPerspectiveTransform(myMat, dest_points);
//		//warpPerspective(imgOriginal, imgOriginal, transform_matrix, cv::Size(maxW, maxH));
//
//
////		imshow("Thresholded Image A", table); //show the thresholded image
//		
//		imshow("orig", imgOriginal);
//	//	imshow("Overlay", dst); //show the original image
//		
//
//
//		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//		{
//			cout << "esc key is pressed by user" << endl;
//			break;
//		}
//	}
//
//	return 0;
//
//}