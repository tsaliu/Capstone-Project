#include "updateServos.h"

using namespace std;


void sendToServos (unsigned char motor1, unsigned char motor2, mySerial serial )
{	
	unsigned char dataArray[2]={};
	dataArray[0] = motor1;
	dataArray[1] = motor2;
	
	printf ("%d", dataArray[0]);
	printf (", ");
	printf ("%d", dataArray[1]);
	printf ("\n");
		
	serial.Send(dataArray,sizeof(dataArray));
	
}

 
	
