#include <Servo.h>
#include <math.h>
Servo A;
Servo B;

int Apos = 0; 
int Apos2 = 95;
int Bpos = 0;
int Bpos2 = 90;
float da = 0;
float db =0;
float dt = 0;
byte number = 0;
unsigned char angbytes[2];

int pos = 0;

int binary2decimal(byte b){   //converts byte into decimal
 int dec = 0;
 int power = 1;
 byte mask;
 int weight;
 
 for (mask = 0x01; mask; mask <<= 1){
   if (b & mask){
     weight = 1;    
   }
   else{
     weight = 0;    
   }
   dec = dec + (power * weight);  
   power = power * 2;
}
 return dec;
}
void setup()
{
  A.attach(4);  //Motor A at pin 4  
  B.attach(9);  //MOtor B at pin 9
  Serial.begin(9600); //start serial com
  pinMode(2,OUTPUT);
  A.write(Apos2); //set motor initial position
  B.write(Bpos2); 
}

void loop()
{
  while(Serial.available()<2){}  //wait for 2 bytes
  for(int k=0;k<2;k++){
    angbytes[k]=Serial.read();  //get 2 bytes
  }
  byte Anum=angbytes[0];
  byte Bnum=angbytes[1];
  int Apos2=binary2decimal(Anum); //convert byte into decimal angle
  int Bpos2=binary2decimal(Bnum);

  if (Apos2==0 && Bpos2==0){    //incase serial com fail
    Apos2=Apos;                 //stay at last position
    Bpos2=Bpos;
  }
  A.write(Apos2); //rotate motor
  B.write(Bpos2);

  Apos=Apos2;
  Bpos=Bpos2;
  Serial.flush();        //clear serial buffer
}
