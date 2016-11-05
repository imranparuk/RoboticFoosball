#include "Player.h"

char startByte = '<';
char stopByte = '>';
char splitByte = '#';
char endLine = '\n';


//Player::Player(Serial *SP, Point *totalMen, int *yMin, int *yMax)
//{
//	serialPort = SP;
//	//playerLine::playerLine(int numP, Point *menInLine, int *minY, int *maxY)//Ahmad
//	pline[0] = playerLine(1, &totalMen[0], &yMin[0], &yMax[0]);
//
//	Point arrTemp1[3] = { totalMen[1], totalMen[2], totalMen[3] };
//	int yMinTemp1[3] = { yMin[1], yMin[2], yMin[3] };
//	int yMaxTemp1[3] = { yMax[1], yMax[2], yMax[3] };
//	pline[1] = playerLine(3, arrTemp1, yMinTemp1, yMaxTemp1);
//
//	Point arrTemp2[4] = { totalMen[4], totalMen[5], totalMen[6], totalMen[7] };
//	int yMinTemp2[4] = { yMin[4], yMin[5], yMin[6], yMin[7] };
//	int yMaxTemp2[4] = { yMax[4], yMax[5], yMax[6], yMin[7] };
//	pline[2] = playerLine(4, arrTemp2, yMinTemp2, yMaxTemp2);
//
//	Point arrTemp3[3] = { totalMen[8], totalMen[9], totalMen[10] };
//	int yMinTemp3[3] = { yMin[8], yMin[9], yMin[10] };
//	int yMaxTemp3[3] = { yMax[8], yMax[9], yMax[10] };
//	pline[3] = playerLine(3, arrTemp3, yMinTemp3, yMaxTemp3);
//
//
//}

Player::Player(Serial* SP)
{
	serialPort = SP;
	pline[0] = playerLine(180, 110, 120, 1);
	pline[1] = playerLine(273, 17, 110, 3);
	pline[2] = playerLine(273, 17, 90, 4);
	pline[3] = playerLine(273, 17, 110, 3);
}

Player::~Player(){
}


void Player::moveToBall(Point ballPosition)
{
	int i = 0;
	Point testD[4] = {};
	for (playerLine & tmp : pline) {
		testD[i].x = tmp.getBestPlayerForPosition(ballPosition);
		testD[i].y = 90;
		i++;
	}
	setMoveData(testD, 4);
	sendMoveData();
}


void Player::setMoveData(Point *data, int size)
{
	
	//while (data != nullptr) {
	////for (int i = 0; i < size; ++i) {
	////	pline[i].setPosition(data[i].x);
	////	pline[i].setRotation(data[i].y);
	//	pline[i].setPosition(data[i].x);
	//	pline[i].setRotation(data[i].y);
	//	i++;
	//	data++;
	//	cout << "ya " << (char)pline[i].getPosition() << " " << (char)pline[i].getRotation() << endl;
	//}
	int i = 0;
	for (playerLine & tmp : pline)
	{

		tmp.setPosition(data[i].x);
		tmp.setRotation(data[i].y);

		i++;
		//cout << "ya " << i << " " <<  (char)tmp.getPosition() << " " << (char)tmp.getRotation() << endl;
	}
}

void Player::sendMoveData()
{
	if (serialPort->IsConnected()) {
		comunicationStr += startByte;
		
		char tempA[5];
		char *strA = NULL;
		char tempB[5];
		char *strB = NULL;
		int sendPos = 0;
		int sendRot = 0;

		int i = 0;
		for (playerLine & tmp : pline){
			//cout << "Here: " << tmp.getRotation() << " " << tmp.getPosition() << endl;
			if (tmp.getRotation() > -9999 && tmp.getRotation() < 9999 && tmp.getPosition() > -9999 && tmp.getPosition() < 9999) {
				sendRot = tmp.getRotation();
				sendPos = tmp.getPosition();
			}
			else {
				sendRot = 0;
				sendPos = 0;
			}
			
			if (_itoa_s(sendRot, tempA, 5, 10) == 0)
				strA = tempA;
			if (_itoa_s(sendPos, tempB, 5, 10) == 0)
				strB = tempB;

			if (i != 0) comunicationStr += splitByte;
			else i++;

			comunicationStr += strA;
			comunicationStr += splitByte;
			comunicationStr += strB;
		}

		comunicationStr += stopByte;
		comunicationStr += endLine;
		serialPort->WriteDataThread(comunicationStr);
		//char* comunicationBuff = (char*)comunicationStr.c_str();
		//cout << comunicationBuff << endl;
		////serialPort->WriteData(comunicationBuff, comunicationStr.length());
		comunicationStr = "";
	}



}

void Player::lineToBeActivated(Ball b)
{
	if (b.ballStationery() != 1)
	{
		if (b.getXdirection() == -1)
		{

			for (int j = 0; j < 4; j++)
			{
				if (pline[j].getPosition() < b.getCurrentPosition().x)
				{
					int expectedY = b.expectedY(pline[j].getPosition());
					moveToBall(Point(0, expectedY));
				}

			}
		}
		else
		{
			for (int j = 0; j < 4; j++)
			{
				if (pline[j].getPosition() > b.getCurrentPosition().x)
				{
					int expectedY = b.expectedY(pline[j].getPosition());
					moveToBall(Point(0, expectedY));
				}

			}
		}
	}
	else
	{
		for (int j = 0; j < 4; j++)
		{
			if (pline[j].getPosition() < b.getCurrentPosition().x)
			{
				int expectedY = b.getCurrentPosition().y;
				moveToBall(Point(0, expectedY));
			}

		}
	}


}