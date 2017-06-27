//Written By: Noor Allami
//Last Updated: April 1, 2017

#include <Servo.h> //servo library allows arduino board to control servo motors

//defining Variables
  int X; //horizontal angle
  int Y; //vertical angle

//defining Objects
  Servo vertical; //creating object to control vertical motion servo 
  Servo horizontal; // creating object to control horizontal motion servo


 int A_horizontal[]= {50, 60, 70, 80, 90, 100, 110};//creating global variable, array storing all the positions the horizontal servo can take +/- 30 degrees from centre position
 int B_vertical[]= {60, 70, 80, 90, 100, 110, 120};//creating global variable, array storing all the positions the vertical servo can take +/- 30 degrees from centre position


void setup() 
{
  
// connecting Servos to output pins
  vertical.attach(13); //attaching vertical servo to pin 13
  horizontal.attach(12); //attaching vertical servo to pin 12

 
  X= 80;//setting horizontal servo to centre position (0 coordinate)
  X=map(X, 50 ,110, 50, 110);//mapping the value obtained to ensure the values are in the range between 50 and 110
  Y= 90;//setting vertical servo to centre position (0 coordinate)
  Y=map(Y, 60, 120, 60, 120);  //mapping the value obtained to ensure the values are in the range between 60 and 120
   horizontal.write(X);//moving the horizontal servo to centre position
   vertical.write(Y);// moving the vertical servo to centre position

   delay (3000);//a delay of 3 seconds, before entering into the random loop

// initialize Serial
Serial.begin(9600); //sets the data rate in bits per second (baud) for serial data transmission
}

void loop() 
{
// Moving target and waiting
  X= A_horizontal[random_variable(0,6)]; //assigning position to horizontal servo, a value from the horizontal position array of random index
  X=map(X, 50 ,110, 50, 110);//mapping the obtained position to values ranging between 50 and 110 degrees
  Y= B_vertical[random_variable (0,6)];//assigning position to vertical servo, a value from the vertical position array of random index
  Y=map(Y, 60, 120, 60, 120); //mapping the obtained position to values ranging between 60 and 120 degrees
horizontal.write(X);//moving the horizontal servo to assigned random position from the horizontal array 
vertical.write(Y);//moving the vertical servo to assigned random position from the vertical array
   delay (3000);//a delay of 3 seconds, waiting in position for 3 seconds before next poition
}
//The below function obtains a minimum and a maximum value passed on by the calling function
//it returns a random number between the minimum and maximum numbers passed, saving it as an integer type
  int random_variable (int min, int max)//classifiying function type and defining the values it receives
  {int index;//defining variable index
  index= (int) random(min,max);//assigning the variable index an integer random value between the minimum and maximum numbers passed
  return index;}//returning the variable index (random integer value between passed min and max numbers)

