/*
 * myOutput_port__test.ino
 * Test for the 8 bit digital input to the natus system
 * the arduino will output the numbers 1-10 20 times with a 
 * 10 second delay inbetween each set of 1-10
 * the duration an interval will vary between 100 microseconds
 * and 2 milliseconds in 100 microsecond increments
 * erik.emeric@gmail.com
 */

#include<IO_Port_8bit.h>

const int A0_0  = 4;// analog output pin for pulses representing 0s
const int A0_1  = 5;// analog output pin for pulses representing 1s

IO_Port_8bit myport(6,7,8,9,10,11,12,13, 0);

// digital pulse parameters
const int digPWDT = 1000;// pulse width
const int digIPI = 1000; // interpulse interval
const int pwdth = 2000; // pulse width in microseconds
const int ipi   = 2000; // inter pulse interval in microseconds

void setup() 
{
 // used to for output to physiol system
  pinMode(A0_0, OUTPUT); // sets the digital pin 0 as output
  pinMode(A0_1, OUTPUT); // sets the digital pin 1 as output
  Serial.begin(9600); 
  while(!Serial){ 
  }
}

void loop() 
{
  for (int j = 1; j < 21;j++){; //multiplier for the duration of the pulse
    digitalWrite(LED_BUILTIN, HIGH);
    for (int i = 1; i < 255;i++){;
      sendSinglePulse();
      myport.send_8bit_data(i);
      delayMicroseconds(digPWDT*j);
      myport.send_8bit_data(0);
      delayMicroseconds(digIPI*j);
    }
    //Serial.println(i);
    //Serial.println(digPWDT*j);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
}

void sendSinglePulse(){
  // send single pulse to ephys system
  digitalWrite(A0_0, HIGH);// sets the digital pin 0 to HIGH  
  digitalWrite(A0_1, HIGH);// sets the digital pin 1 to HIGH              
  delayMicroseconds(pwdth);// waits for a pwdth ms
  digitalWrite(A0_0, LOW); // sets the digital pin 0 to LOW  
  digitalWrite(A0_1, LOW); // sets the digital pin 1 to LOW
}
