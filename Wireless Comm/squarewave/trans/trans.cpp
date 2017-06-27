#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>

//sudo g++ -o trans trans.cpp -lwiringPi
//sudo ./trans

using namespace std;

int tmpbin[8];	//store binary bits
int laspin=4;	//laser output pin
int flagpin=5;	//flag pin

//Min Period 7.5e-5 sec 75usec
//1 bit 37.5usec 26.67kbps
float bittime=1000-75;		//1msec
int syncd[8]={1,0};
int initd[8]={1,1,1,1,0,0,0,0};
//costum byte for testing
int test[8]={1,1,1,1,1,1,1,1};

//Convert decimal to binary	(input decimal, number of bits, name of int output array)
void dec2bin(unsigned int in, int count, int* out)
{
    unsigned int mask = 1U << (count-1);
    int i;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}
//output data with delay included
void high(){
	digitalWrite(laspin,HIGH);
	//cout << "1" << endl;
	delayMicroseconds(bittime);
}
void low(){
	digitalWrite(laspin,LOW);
	//cout << "0" << endl;
	delayMicroseconds(bittime);
}
//output a whole size 8 array
void outbyte(int A){
	//every byte start with initialize sequence
	for(int i=0; i<8; i++){
		if(initd[i]==1){
			high();
		}
		if(initd[i]==0){
			low();
		}
	}
	//sync sequence
	for(int n=0; n<2; n++){
		if(syncd[n]==1){
			high();
		}
		if(syncd[n]==0){
			low();
		}
	}

	//output bits
		if(A==1){
			high();
			low();
		}
		if(A==0){
			low();
			high();
		}
}

int main(void){
	wiringPiSetupGpio();
	pinMode(laspin,OUTPUT);
	int testbit=1;

while(1){
    //send out 1010101010101....
	outbyte(testbit);
	testbit=!testbit;
}
}


