#include <iostream>
#include <wiringPi.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
using namespace std;

//g++ `pkg-config opencv --cflags` trans.cpp -o trans `pkg-config opencv --libs` -l wiringPi
//sudo ./trans

const int bittime=1000; //Microsecond pulse times

int sliderPosition =2; // Initial slider position
int divisions = 1;     // Initial number of divisions

int blockXSize;         // Horizontal block size
int blockYSize;         // Vertical block size

int pixelCount;         // The number of pixels in a block (blockXSize multiplied by blockYSize)

int width;              // The width  of the input stream
int height;             // The height of the input stream

int arraX[8]={}; 	//sets digital out pin
int arraY[8]={}; 	//sets digital out pin
int arraB[8]={}; 	//sets digital out pin

const int laspin = 4; 	// Regular LED - Broadcom pin 24, P1 pin GPIO 12
const int inPin=24; 	//input Pin Broadcom pin 18, gpio pin 24

int transmit=0;
int syncd[2]={1,0};
int initd[8]={1,1,1,1,0,0,0,0};
const char*  windown="Pixelized Display";
//Converts decimal Integers into 8 bit unsigned binary
void binConv8bit(int blck,int arr[])
{
memset(arr, 0, 8);
//cout <<   "   Decimal = "<< blck<< endl;
int X =blck;
int D = 1;
 for (int i=0 ;i<8; i++  )
	{

	arr[i]=X/(128/D);

	if (arr[i] ==1){X=X-(128/D);}
	D= D*2;
	}

//cout <<    arr[0] << arr[1] <<arr[2] <<arr[3] <<arr[4] <<arr[5] <<arr[6] <<arr[7] <<endl;
}
void high(){
	digitalWrite(laspin,HIGH);
	delayMicroseconds(bittime);
}
void low(){
	digitalWrite(laspin,LOW);
	delayMicroseconds(bittime);
}
//convert decimal to binary
void dec2bin(unsigned int in, int count, int* out)
{
    unsigned int mask = 1U << (count-1);
    int i;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}

//output binary of from array
void outbyte(int* A){

	//every byte start with initialize sequence
	cout << "**********************************************" << endl;
	cout << "Initial Stream: ";
	for(int i=0; i<8; i++){

		if(initd[i]==1){
			high();
			cout << "1";
		}
		if(initd[i]==0){
			low();
			cout << "0";
		}
	}cout<< "" << endl;

	//sync sequence
	cout << "Start Stream: ";
	for(int n=0; n<2; n++){
		if(syncd[n]==1){
			high();
			cout << "1";
		}
		if(syncd[n]==0){
			low();
			cout << "0" << endl;
		}
	}cout<< "" << endl;

	//output bits
	cout << "Brightness Stream: ";
	for(int k=0; k<8; k++){
		if(A[k]==1){
			high();
			cout << "1";
			low();
			cout << "0";
		}
		if(A[k]==0){
			low();
			cout << "0";
			high();
			cout << "1";
		}
	}cout<< "" << endl;
	digitalWrite(laspin,LOW);   //done sending, then stay low, don't float
}

int main()
{
    wiringPiSetupGpio();
    pinMode(laspin, OUTPUT);
    // lock pin is input
    pinMode(inPin,INPUT);

    // Create two windows
    //cvNamedWindow("WebCam", CV_WINDOW_AUTOSIZE);
    cvNamedWindow(windown, CV_WINDOW_AUTOSIZE);

    //Full Screen setting
    //cvSetWindowProperty("Low Rez Stream",CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    //int maxSliderValue = 11;

    // Create the divisions slider lider
    //cvCreateTrackbar("Divisions", windown, &sliderPosition, maxSliderValue, onDivisionSlide);

    // Start capturing data from the web cam
    CvCapture* pCapture = cvCaptureFromCAM(-1);

    // Reduce framerate from default
    //cvSetCaptureProperty( pCapture, CV_CAP_PROP_FPS , 5 );

    //Set stream resolution
    //cvSetCaptureProperty( pCapture, CV_CAP_PROP_FRAME_WIDTH , 320);
    //cvSetCaptureProperty( pCapture, CV_CAP_PROP_FRAME_HEIGHT , 240 );

    // Get an initial frame so we know the size of things (cvQueryFrame is a combination of cvGrabFrame and cvRetrieveFrame)
    IplImage* pFrame = NULL;
    pFrame = cvQueryFrame(pCapture);

    // Create an image the same size and colour-depth as our input stream
    IplImage* pLowRezFrame = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);

    uchar *ptr; // Pointer to our pixel

    int red, green, blue; // Integers to hold our pixel values

    // Get the width and height of our webcam input stream
    int width  = pFrame->width;
    int height = pFrame->height;

    // Integers to hold our total colour values (used to find the average)
    int redSum     = 0;
    int greenSum   = 0;
    int blueSum    = 0;

    // Loop controling vars
    char keypress;
    bool quit = false;
	//Loop Output Coordinates
	int XX=0;
	int YY=0;
    while (quit == false)
    {

	//reads pixels if transmit pin is high
	//Jumper from 3.3V to start
	if (digitalRead(inPin)==1)
{
        // Grab a frame from the webcam
        pFrame = cvQueryFrame(pCapture);

        // Draw the original frame and low resolution version
		//cvShowImage("WebCam", pFrame);
        cvShowImage(windown, pLowRezFrame);

        // Calculate our blocksize per frame to cater for slider
        blockXSize = width  / divisions;
        blockYSize = height / divisions;

        pixelCount = blockXSize * blockYSize; // How many pixels we'll read per block - used to find the average colour

        // Draw a rectangle of Black

			cvRectangle(
                    pLowRezFrame,
                    cvPoint(0,0),
                    cvPoint(640,480),
                    CV_RGB(0, 0, 0),
                    CV_FILLED,
                    8,
                    0
                );

        // Loop through each block horizontally
        for (int xLoop = 0; xLoop < width; xLoop += blockXSize)
        {
            // Loop through each block vertically
            for (int yLoop = 0; yLoop < height; yLoop += blockYSize)
            {
                // Reset our colour counters for each block
                redSum   =greenSum  = blueSum = 0;
                // Read every pixel in the block and calculate the average colour
                for (int pixXLoop = 0; pixXLoop < blockXSize; pixXLoop++)
                {
                    for (int pixYLoop = 0; pixYLoop < blockYSize; pixYLoop++)
                    {
                        // Get the pixel colour from the webcam stream
                        ptr = cvPtr2D(pFrame, yLoop + pixYLoop, xLoop + pixXLoop, NULL);
                        // Add each component to its sum
                        redSum   += ((ptr[2]+ptr[1]+ptr[0])/3);

                    } // End of inner y pixel counting loop
                } // End of outer x pixel counter loop
                // Calculate the average brightness of the block
                red =green=blue =redSum   / pixelCount;
                //cout << redSum << endl;
                //cout << pixelCount << endl;
                //cout << red << endl;

        //convert pixelblocks into binary array
		binConv8bit(xLoop/blockXSize,arraX);
		binConv8bit(yLoop/blockYSize,arraY);
		binConv8bit(red,arraB);

		cout << "Brightness: " << red << endl;
		cout << "**********************************************" << endl;
        //output binary array
		outbyte(arraB);
		//draw circle
        cvCircle(
                pLowRezFrame,
		cvPoint(
				xLoop + ((blockXSize)/2), yLoop + ((blockYSize)/2)
			    ),
            //Radius
			((blockYSize)/2),
            //Colour
		    CV_RGB(red, green, blue),
                    CV_FILLED,
                    8,    0
                );
            } // End of inner y loop
        } // End of outer x loop
} // end of transmit
        // Wait 1 millisecond
        keypress = cvWaitKey(1);
        // Set the flag to quit if the key pressed was escape
        if (keypress == 27)
        {
           quit = true;
        }
    } // End of while loop
    // Release our stream capture object
    cvReleaseCapture(&pCapture);
    // Release our images & destroy all windows
    cvReleaseImage(&pFrame);
    cvReleaseImage(&pLowRezFrame);
    cvDestroyAllWindows();
}
