Platform: Raspberry Pi

Contributor: Anas Zurkiyeh

There are various method on tracking the target. We are using HSV tracking and Haar Cascade.

The coordinates obtained from the tracking is converted into degrees and send to serial port to control the reflective mirror. The initial calibration of the reflective mirror motors are done with "Moving Servos". Then the "Tracking" is performed.

The tracking code is based on example provided on the OpenCV webiste:

http://docs.opencv.org/2.4/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html

HSV tracking code isa modified version of:

https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp

