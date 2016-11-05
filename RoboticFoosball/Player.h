#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include "PlayerLine.h"
#include "Serial.h"
#include "Ball.h"

using namespace cv;
using namespace std;

class Player
{
private:
	Serial* serialPort;    
	String comunicationStr = "";
	int numberOfLines = 4;
	playerLine pline[4] = {};


public:
	//Player(Serial*, playerEntity*);
	Player(Serial*);
	//Player(Serial *, Point *totalMen, int *yMin, int *yMax);

	~Player();

	void sendMoveData();
	void setMoveData(Point[],int);
	void moveToBall(Point);

	void lineToBeActivated(Ball );


};

#endif // AGENT_H