#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <vector>
#include <math.h>
#include "mcp3008Spi.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

//sudo g++ -Wall -o rec2 `pkg-config opencv --cflags` mcp3008Spi.cpp `pkg-config opencv --libs` rec2.cpp -lwiringPi
//Need to enable SPI
//Vdd -- 5V
//Vref -- 5V
//AGND -- GND
//CLK -- SCLK
//Dout -- MISO
//Din -- MISI
//CS/SHDN -- CSO (GPIO8)
//DGND -- GND
//Currently using CH0, change with data[1]

#define	BASE 100
#define	SPI_CHAN 0

using namespace std;
using namespace cv;

int val;
int recbit;
int tempbit;
int syncd[2]={1,0};
int initd[8]={1,1,1,1,0,0,0,0};
int bittime=1000;
int adcmint=50;
int vthtime=100000;
float vth;
int discount=0;
const int length=3;
unsigned char frame[length];

//adc min sample period 50us per bit
//read adc value, return logic high low for threshold vth
int adcread(float vth){
	int a2dVal = 0;
    int a2dChannel = 0;
    unsigned char data[3];
    mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
	data[0] = 1;  //  first byte transmitted (start bit)
    data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
    data[2] = 0; // third byte transmitted(don't care)
    a2d.spiWriteRead(data, sizeof(data) );

    a2dVal = 0;
    a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
    a2dVal |=  (data[2] & 0xff);

    if (a2dVal <= vth){
	tempbit=0;
	}
	else{
	tempbit=1;
	}
    return tempbit;
}
//read adc return adc value 0 to 1023
int adcreadth(void){
	int a2dVal = 0;
    int a2dChannel = 0;
    unsigned char data[3];
    mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
	data[0] = 1;  //  first byte transmitted (start bit)
    data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
    data[2] = 0; // third byte transmitted(don't care)
    a2d.spiWriteRead(data, sizeof(data) );

    a2dVal = 0;
    a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
    a2dVal |=  (data[2] & 0xff);

    return a2dVal;
}
//calculate threshold voltage
float vthcalc(void){
	cout.setf(ios::showpoint);
	//cout << "Calculating Threshold Voltage" << endl;
	float vth=0;
	int vtotal=0;
	int counter=0;
	float tvth=clock();
	float tempvtht;
	while(tempvtht<vthtime){

		vtotal=vtotal+adcreadth();
		counter=counter+1;
		tempvtht=clock()-tvth;
		tempvtht=tempvtht/CLOCKS_PER_SEC;
		tempvtht=tempvtht/(1e-6);
		//cout << tempvtht << endl;
		//cout << counter << endl;
	}
	vth=vtotal/counter;
	float threshold=(vth*5)/1024;
	cout << "Current Threshold: " << threshold << " V" << endl;
	//cout << vth << endl;
	return vth;
}


int main(void){
	//cout.setf(ios::showpoint);	//display float in cout
	wiringPiSetupGpio(); 		//GPIO setup
	//create window to show image
	cvNamedWindow("Display", CV_WINDOW_AUTOSIZE);
	//create empty image of 640x480
	IplImage* image = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
	char keypress;
	bool quit = false;
	while(!quit){
	while(1){
        //display current frame
		cvShowImage("Display", image);
        //wait to finish drawing
		char c = (char)cvWaitKey(10);
        if( c == 27 ) { break; } // escape
		vth=vthcalc();				//Calculate threshold voltage
		bool start=false;
		//loop until initialize sequence found
		while(!start){
			if(adcread(vth)==1){
					float fetchinit=clock();
					while(adcread(vth)==1){
					}
					float fetchtime=clock()-fetchinit;
					fetchtime=fetchtime/CLOCKS_PER_SEC;
					fetchtime=fetchtime/(1e-6);
					int bitcounthigh=round(fetchtime/bittime);
					//cout << fetch8time << endl;
					//cout << "high bit number:" << bitcounthigh<<endl;
					if(bitcounthigh>=4){
						float fetchinit=clock();
						while(adcread(vth)==0){
						}
						float fetchtime=clock()-fetchinit;
						fetchtime=fetchtime/CLOCKS_PER_SEC;
						fetchtime=fetchtime/(1e-6);
						int bitcountlow=round(fetchtime/bittime);
						//cout << fetch8time << endl;
						//cout << "low bit number:" << bitcountlow<<endl;
						if(bitcountlow==4){
						start=true; //found initialize sequence
						}
					}
				}
		}
		//cout << "done" << endl;
		unsigned char rec;
		int nowbit=1;
		int noclock=0;
		//define how many bytes the data has
		//int framecount=0;
		//do{
			do{
				float transtime=clock();
				if(adcread(vth)==1){
					while(adcread(vth)==1){}
					//cout << "high" << endl;
				}
				else{
					while(adcread(vth)==0){}
					//cout << "low" << endl;
				}
				transtime=((clock()-transtime)/CLOCKS_PER_SEC)/(1e-6);
				//cout << transtime << endl;
				int bitcount=round(transtime/(bittime));
				//cout << "bitcount" << bitcount << endl;
				if(!(bitcount%2)){	//transition happen
					nowbit=!nowbit;
					noclock=noclock+bitcount;
					//cout << nowbit << endl;
				}
				else{
					noclock=noclock+bitcount;
					//cout << nowbit << endl;
				}
				//cout << "noclock;" <<noclock << endl;
				if(!(noclock%2)){
					rec <<= 1;
					rec += nowbit;
					//cout << nowbit << endl;
				}
				else{
					if(bitcount==2){
						rec <<= 1;
						rec += nowbit;
						//cout << nowbit << endl;
					}
				}

			}while(noclock<18);
			rec = rec & 0x0ff;
			std::cout << "Decoded Data: " << (int)rec <<std::endl;
			//frame[framecount]=rec;    //save current frame data
			//framecount=framecount+1;  //increment frame count
		//}while(framecount < length);  //loop until all bytes fetched
		//std::cout << ""<<std::endl;
		int width=640;
		int height=480;
		//draw a black rectangle for background
		cvRectangle(
                    image,
                    cvPoint(0,0),
                    cvPoint(640,480),
                    CV_RGB(0, 0, 0),
                    CV_FILLED,
                    8,
                    0
                );
        int red, green, blue;
        int xLoop=0;
        int yLoop=0;
        int blockXSize=width;
        int blockYSize=height;
        int color;
        //turn char into int
        color=reinterpret_cast<unsigned char&>(rec);
        //grayscale red=blue=green
        red=color;
        green=color;
        blue=color;
        cout << "Brightness: " << color << endl;
        //draw circle with red=blue=green=brightness
        cvCircle(
                image,
                //Centre
				cvPoint(xLoop + ((blockXSize)/2), yLoop + ((blockYSize)/2)),
                //Radius
				((blockYSize)/2),
				//Colour
				CV_RGB(red, green, blue),
                CV_FILLED,
                8,0);
	}
	//wait to finish drawing
	keypress = cvWaitKey(0);
	if (keypress == 27)
        {
           quit = true;
        }
	}
	//release the image array
    cvReleaseImage(&image);
    //close all windows
    cvDestroyAllWindows();
}


