#include "mySerial.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>

using namespace std;


class updateServos
{
	public:
		static void sendToServos (unsigned char motor1, unsigned char motor2, mySerial serial );
	protected:
	private:
};


void updateServos::sendToServos (unsigned char motor1, unsigned char motor2, mySerial serial )
{	
	unsigned char dataArray[2]={};

	dataArray[0] = motor1;
	dataArray[1] = motor2;
		
	serial.Send(dataArray,sizeof(dataArray));
	
}

