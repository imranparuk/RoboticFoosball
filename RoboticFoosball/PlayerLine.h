#pragma once

#ifndef PLINE_H
#define PLINE_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "PlayerMan.h"
#include "Ball.h"

using namespace cv;
using namespace std;

class playerLine
{
private:
	
	Point *playerPositions;

	int numPlayers;

	int maxSteps;
	int maxPixels;
	int minPixels;

	int xPos;
	int yRot;
	




public:
	//PlayerMan *arrPlayers;

	//playerLine(int , Point *,int *,int *); //Ahmad
	playerLine(int, int, int , int);
	playerLine();

	~playerLine();

	int getPosition();
	int getRotation();


	void setPosition(int);
	void setRotation(int);

	void setLinePostion();

	int getBestPlayerForPosition(Point,int);
	int getBestRotation(Point);




};

#endif 
