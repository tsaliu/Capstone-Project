/***


#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;





class Tracker
{
	public:
		static int* Track ( );
	protected:
	private:
};




// Function Headers 

//void detectAndDisplay( Mat frame, VideoCapture capture );
void detectAndDisplay( Mat frame );
// Global variables 
String eye_cascade_name = "/home/pi/Desktop/pupil_detection/haarcascade_eye.xml";
CascadeClassifier eye_cascade;
//CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";

// @function Tracker 

int* Tracker::Track( void )
{
    VideoCapture capture;
    Mat frame;

    //-- 1. Load the cascades
    if( !eye_cascade.load( eye_cascade_name ) ){ printf("--(!)Error loading eye cascade\n"); return -1; };

    //-- 2. Read the video stream
    capture.open( -1 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }

	// Set Resolution of frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    
    
    // Error checking    
		if ( capture.read(frame) != 1)
		{
			cout << "--(!) Frame was not initilized, Frame was not read!" << endl;
		}

        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }


        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame);
        
      	//detectAndDisplay( frame, capture );
        char c = (char)waitKey(10);
        if( c == 27 ) { break; } // escape


    return 0;
}



// @function detectAndDisplay
//void detectAndDisplay( Mat frame, VideoCapture capture  )
void detectAndDisplay( Mat frame)
{	
    std::vector<Rect> eyes;
    Mat frame_gray;
    //double frameRate = capture.get(CAP_PROP_FRAME_COUNT );

/home/pi/Desktop/project_files/Tracker.h:33:24: error: variable or field ‘detectAndDisplay’ declared void

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect eyes
    eye_cascade.detectMultiScale( frame_gray, eyes, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
		
    for ( size_t i = 0; i < eyes.size(); i++ )
    {
        Point center( eyes[i].x + eyes[i].width/2, eyes[i].y + eyes[i].height/2 );
        ellipse( frame, center, Size( eyes[i].width/2, eyes[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        
        cout << eyes[i].x + eyes[i].width/2 <<", "<< eyes[i].y + eyes[i].height/2<< endl;
        

	
    }
    //-- Show what you got
    imshow( window_name, frame );
    
    
    // int fillarr(int* arr)
    // int foo [5] = { 16, 2, 77, 40, 12071 };
    
    
    int coordinates [2] = ; 
    
    
    
}

***/
