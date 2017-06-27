#include <stdio.h>
#include <unistd.h>		//Used for UART
#include <fcntl.h>		//Used for UART
#include <termios.h>		//Used for UART
#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>

//sudo g++ -o trans trans.cpp -lwiringPi
//sudo ./trans

using namespace std;

int tmpbin[8];	//store binary bits
int laspin=4;	//laser output pin

//Min Period 7.5e-5 sec 75usec
//1 bit 37.5usec 26.67kbps
float bittime=1000-75;		//1msec
int syncd[8]={1,0};
int initd[8]={1,1,1,1,0,0,0,0};
//costum byte for testing
int test[8]={1,0,1,0,1,0,1,0};

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
void high(){
	digitalWrite(laspin,HIGH);
	delayMicroseconds(bittime);
}
void low(){
	digitalWrite(laspin,LOW);
	delayMicroseconds(bittime);
}
//encode and output size 8 array with initialize and start bits
void outbyte(int* A){

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
	for(int k=0; k<8; k++){
		if(A[k]==1){
			high();
			low();
		}
		if(A[k]==0){
			low();
			high();
		}
	}
}

int main(void){
	wiringPiSetupGpio();
	pinMode(laspin,OUTPUT);

while(1){
	//loop count from 0 to 255
	for(int count=0;count<256;count++){
		dec2bin(count,8,tmpbin);
		outbyte(tmpbin);
		cout << count << endl;
	}
	//outbyte(test);
}
}


