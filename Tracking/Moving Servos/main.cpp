/**
 * This is the main program interface where the identifying and the 
 * detection of the object happens. Then a decision on how to move the 
 * servos is made!
 * 
 * 
 * This program assumes that the coordinates of the center of the object
 * is known. As well as, the inital position of the servos. This is made
 * by desogn as it allows for greater flixability and a more accurate 
 * results.
 * 
 */




#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <vector>




#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"



#include "mySerial.h"
#include "updateServos.h"
#include "Tracker.h"


// Importing names spaces: Standard and computer Vision
using namespace std;
using namespace cv;




/** Global variables */
String eye_cascade_name = "/home/pi/Desktop/project_files/haarcascade_eye.xml";

CascadeClassifier eye_cascade; // used to detect the object

unsigned char dataArray[2]={};



/** Thresholds and limits for image processing */

int insigChange = 5;
int level1Change = 10;
int level2Change = 20;

int changeDir_Hor = 5;
int changeDir_Ver = 5;






//CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";


/* Initilize Coordinates and Servos
Upon starting the application, users have to define the inital value
the two servos and the inital location of the center of the object
 */

int default_servo_ver = 96;
int default_servo_hor = 99;
  
int default_center_x = 162;
int default_center_y = 147;


/** Function Headers */
vector<int> detectAndDisplay( Mat frame );

// moving the mirror function
void moveMirror (int,  int, mySerial);

/** 
 * updating horizental coordinate, takes in the currunt horezintal 
 * coordiante, then processes it and makes a decision on wether or not
 * servos should be moved.
 * Returns: new horizenrtal angle degree integrers   
*/

int updateHorizental(int);
int updateVertical(int);



int main()
{	

	// initializing serial channel
	// for sending data through USB port to the servos
	
	mySerial serial("/dev/ttyUSB0",9600);
	usleep(2000000);
	
	
	/** Initilizing system. Moving servos to default position 
	 * Defailt position: Which is position zero of the target.
	 * 
	 * */
	 
    // Load default coordinates
    dataArray[0]=default_servo_ver;
    dataArray[1]=default_servo_hor;
          
    // Move servos to default position, initilize    
	serial.Send(dataArray,sizeof(dataArray));
    
    // Notify user of Servos location
    cout << "Servos: " << default_servo_ver <<", "<< default_servo_hor << endl;
    
	
	// Camera instance and frame
	// A frame will be captured from the Video Stream and saved onto
	// frame
	 
	VideoCapture capture;
	Mat frame;
	
	// vector of size 2 that hold x coordinate of center in
	// coord[0] and y coordinate in coord[1]
	vector<int> coord;
	
	
	
    // Load the cascades
    if( !eye_cascade.load( eye_cascade_name ) ){ printf("--(!)Error loading eye cascade\n"); return -1; };

    // Read the video stream
    capture.open( -1 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }


	/** Set Resolution of frame\
	 * This resolution was chosen after several trial and error itterations
	 * a high resolution means more data to process which leads to more 
	 * latency but better detection. A low resolution leads to mucg faster
	 * processing however, less reliable information
    */
    
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    
    
    
    /** Detection and Tracking Loop */
        
        while (true)
        {
			
			/** Error detection statements and Error Checking*/
			
			// VideoCapture capture
			if ( capture.read(frame) != 1)
				{cout << "--(!) Frame was not initilized, Frame was not read!" << endl;}

			if( frame.empty() )
				{printf(" --(!) No captured frame -- Break!");}
			 
			 
			 /** Detection */
			// Obtain currunt coordinates of center
			coord = detectAndDisplay( frame );
			
			// Load new coordinates
			int currunt_hor_coord = coord[0] ;
			int currunt_ver_coord = coord[1] ;
			
			
			int servo_hor_angle = updateHorizental(currunt_hor_coord);
			int servo_ver_angle = updateVertical(currunt_ver_coord);
		   
			// notify user of new angles
			cout<< "Center in pixels: " << coord[0] <<", "<< coord[1]<< endl;
			
			
			if (currunt_hor_coord!= 0 && currunt_ver_coord != 0 )
			{
				// Load new coordinates
				dataArray[0]=servo_ver_angle;
				dataArray[1]=servo_hor_angle;
				  
				
				// send new angles to servos and wait for 0.5 seconds to 
				// allow time for the servos to move
				serial.Send(dataArray,sizeof(dataArray));
				//usleep(500000);
			}
			
			
			
			
			   }
				
		   
	// Close serial channel when done looping
	serial.Close();
    return 0;
}




/** Move servos to new specified location */
void moveMirror (int motor_1,  int motor_2, mySerial serial)
{
	
	unsigned char dataArray[2]={};

	dataArray[0] = motor_1;
	dataArray[1] = motor_2 ;
		
	serial.Send(dataArray,sizeof(dataArray));
	usleep(1000000);
	
	
}



vector<int> detectAndDisplay( Mat frame  )
{		
		
		std::vector<Rect> eyes;
		Mat frame_gray;
		vector<int> coord (2);
	
		cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
		equalizeHist( frame_gray, frame_gray );

		//-- Detect eyes
		eye_cascade.detectMultiScale( frame_gray, eyes, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
		
			
		for ( size_t i = 0; i < eyes.size(); i++ )
		{
			Point center( eyes[i].x + eyes[i].width/2, eyes[i].y + eyes[i].height/2 );
			ellipse( frame, center, Size( eyes[i].width/4, eyes[i].height/4 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
			
			//cout << eyes[i].x + eyes[i].width/2 <<", "<< eyes[i].y + eyes[i].height/2<< endl;
			
		coord [0] = eyes[i].x + eyes[i].width/2;
		coord [1] = eyes[i].y + eyes[i].height/2;
		}
		
		
		
		
		
		//-- Show what you got
		imshow( window_name, frame );
		
		
		char c = (char)waitKey(10);
		//if( c == 27 ) { break; } // escape
		
		
		return coord;

}



 int updateHorizental(int current_x_coor)
 {
 	int updatedCoord;
 	
 	// direction = 1 means move to the left. Direction = -1 means move to the right
 	int direction = 1;
 	int key = default_center_x - current_x_coor ;
 	
 	
 	// Following statement determines if motors move to the left or right
 	// based on the value direction
 	if (key < -1*changeDir_Hor) 
 	{direction = -1;
 	}
 	
 	
 	// Due to the inaccurecy of the servos. A linear Mapping had to be 
 	// used instead of the original approached we planned. 
 	
 	
 	// Linear mapping of certain regions of the camera plane to certain 
 	// angles of the servos. 
 	
 	// When using MEMs, this mapping is to be replaced by pinhole Camera
 	// Model. Angles are then calculated using equations obtained 
 	// in last term's report. 
 	
 	// Mapping is done using if statements but can easily be changed
 	// to switch case
 	 
 	if (key > -insigChange && key < insigChange)
 		updatedCoord = default_servo_hor ;
 	else if (key < level1Change || key > -1*level1Change)
 		
 		{	
			// In this case servo undershoots to the right side so 
			// the factor had to be doubled
			if (direction == -1)
			{
				updatedCoord = default_servo_hor +  2*direction;			
		}
		else {updatedCoord = default_servo_hor +  1*direction;}
		}
 	else if (key < level2Change || key > -1*level2Change)
 		updatedCoord = default_servo_hor +  2*direction;
 	else 
 		updatedCoord = default_servo_hor +  3*direction;	
 		
 	cout << "New Horizontal Angle: " << 	updatedCoord << endl;
 	return updatedCoord;
 		
 }


	
//	This method is identical to the previous method. However its used \
// 	to update the vertical axis
	
 int updateVertical(int current_y_coor)
 {
 	int updatedCoord;
 	
 	// direction = 1 means move to the Down. Direction = -1 means move to the Up
 	int direction = -1;
 	int key = default_center_y - current_y_coor ;
 	
 	
 	 	
 	// Following statement determines if motors move to the Up or Down
 	// based on the value direction
 	if (key < -1*changeDir_Ver) 
 	{direction = 1;
 	}
 	
 	
 	if (key > -insigChange && key < insigChange)
 		updatedCoord = default_servo_ver ;
 	else if (key < level1Change || key > -1*level1Change)
			
 		{
			if (direction == -1)
			{
				updatedCoord = default_servo_ver +  3*direction;			
		}
		else {updatedCoord = default_servo_ver +  2*direction;}
		}
 	
 	
 		
 	else if (key < level2Change || key > -1*level2Change)
 		updatedCoord = default_servo_ver +  2*direction;	
 	else 
 		updatedCoord = default_servo_ver +  3*direction;	
 		
 	
 	cout << "New Vertical Angle: " << 	updatedCoord<< endl;
 	cout << "******************************************"<<   "\n" <<endl;
			
 	return updatedCoord;
 		
 }





