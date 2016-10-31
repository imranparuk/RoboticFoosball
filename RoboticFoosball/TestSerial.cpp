//#include <stdio.h>
//#include <tchar.h>
//#include "Serial.h"	// Library described above
//#include <string>
//#include <iostream>
//using namespace std;
//// application reads from the specified serial port and reports the collected data
//int main(int argc, _TCHAR* argv[])
//{
//	printf("Welcome to the serial test app!\n\n");
//
//	Serial* SP = new Serial("\\\\.\\COM6");    // adjust as needed
//
//	if (SP->IsConnected())
//		printf("We're connected");
//
//	//char incomingData[256] = "";			// don't forget to pre-allocate memory
//	////printf("%s\n",incomingData);
//	//int dataLength = 255;
//	//int readResult = 0;
//
//	int i = 0;
//	int j = 0;
//	int k = 0;
//	int l = 0;
//	string str = "<90#100#90#100#90#100#90#100>";
//	string str2 = "<0#0#0#0#0#0#0#0>";
//	char *buff = (char*)str.c_str();
//	char *buff2 = (char*)str2.c_str();
//	
//	int toggle = 0;
//
//	while (SP->IsConnected())
//	{
//		//readResult = SP->ReadData(incomingData, dataLength);
//		//// printf("Bytes read: (0 means no data available) %i\n",readResult);
//		//incomingData[readResult] = 0;
//
//		//printf("%s", incomingData);
//		
//		SP->WriteData(buff, 30);
//		Sleep(500);
//		SP->WriteData(buff2, 30);
//		Sleep(500);
//
//	}
//	return 0;
//}