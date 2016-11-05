#pragma once

#ifndef PLAYERMAN_H
#define PLAYERMAN_H

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

class PlayerMan
{
private:
	Point position;
	int ymin;
	int ymax;

public:
	//Player(Serial*, playerEntity*);
	PlayerMan()
	{}
	~PlayerMan()
	{}

	void setRange(int yMin,int yMax)
	{
		ymin = yMin;
		ymax = yMax;
	}
	void setPostion(Point pos)
	{
		position = pos;
	}

	Point getPostion()
	{
		return position;
	}

	int getYMin()
	{
		return ymin;
	}
	int getYMax()
	{
		return ymax;
	}
	
};

#endif // AGENT_H