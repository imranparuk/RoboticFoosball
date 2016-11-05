#pragma once

#ifndef BALL_H
#define BALL_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;

class Ball
{
private:

	Point lastPosition;
	Point currentPosition;
	float gradient;
	float yIntercept;


public:
	Ball() //Ahmad
	{
		lastPosition = Point(1,1);
		currentPosition = Point(1, 1);
	}

	~Ball(){};

	int expectedY(int expectedX)
	{
		return (gradient*expectedX + yIntercept);
	}

	int getXdirection()
	{
		return (currentPosition.x - lastPosition.x) < 0 ? -1 : 1;
	}

	int getYdirection()
	{
		return (currentPosition.y - lastPosition.y) < 0 ? -1 : 1;
	}
	int ballStationery()
	{
		return (currentPosition == lastPosition) ? 1 : -1;
	}

	void setPosition(Point latest)
	{
		lastPosition = currentPosition;
		currentPosition = latest; 
		cout << "yo: " << latest << endl;
		if (lastPosition.x != currentPosition.x)
		{
			gradient = (currentPosition.y - lastPosition.y) / (currentPosition.x - lastPosition.x);
			yIntercept = -gradient*currentPosition.x + currentPosition.y;
		}

	}

	Point getCurrentPosition()
	{
		return currentPosition;
	}

	Point getLastPosition()
	{
		return lastPosition;
	}

};

#endif 
