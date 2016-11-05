#include "PlayerLine.h"

//playerEntity players[11];

playerLine::playerLine(int maxP, int minP, int maxS, int numPl)
{
	maxPixels = maxP;
	minPixels = minP;
	maxSteps = maxS;
	numPlayers = numPl;
}

//playerLine::playerLine(int numP, Point *menInLine, int *minY, int *maxY)//Ahmad
//{
//	numPlayers = numP;
//	int xTotal = 0;
//	for (int i = 0; i < numP; i++)
//	{
//		arrPlayers[i].setPostion(menInLine[i]);
//		arrPlayers[i].setRange(minY[i], maxY[i]);
//		xTotal += menInLine[i].x;
//	}
//	xPos = xTotal / numP;
//}

playerLine::playerLine()
{

}
//playerLine::playerLine(playerEntity *pl, int numPl)
//{
//	for (int i = 0 ; i < numPl; i++){
//		players[i] = *pl;
//		pl++;
//	}
//	
//	numPlayers = numPl;
//}


playerLine::~playerLine()
{
}

int playerLine::getPosition()
{
	return xPos;
}

int playerLine::getRotation()
{
	return yRot;
}

void playerLine::setPosition(int pos)
{
	xPos = pos;
}

void playerLine::setRotation(int rot)
{
	yRot = rot;
}

int playerLine::getBestPlayerForPosition(Point position, int playerNum)
{
	int offset = 10;
	

	if (position.y < maxPixels && position.y > minPixels){

		int absPixelRange = maxPixels - minPixels;
		int pixelsRangePerPlayer = absPixelRange / numPlayers;
		int offsetPixRangePerPlayer = pixelsRangePerPlayer - offset;
		float pixelsToSteps = (float)maxSteps / (float)offsetPixRangePerPlayer;

		float reqPosPix = (((position.y - minPixels)) % pixelsRangePerPlayer);

		int reqPos = (int)(pixelsToSteps*reqPosPix);
		//cout << "PixRange: " << absPixelRange << "PixPerPlay: " << pixelsRangePerPlayer << "PixToSteps: " << pixelsToSteps << "reqPix: " << reqPosPix << "reqSteps: " << reqPos << endl;
		return reqPos;
	}
	else if (position.y > maxPixels) return maxSteps;
	else if (position.y < minPixels) return 0;
}

int playerLine::getBestRotation(Point position)
{
	if (position.y < maxPixels && position.y > minPixels){
		int absPixelRange = maxPixels - minPixels;
		


		
	}
	else if (position.y > maxPixels) return 90;
	else if (position.y < minPixels) return 90;



}