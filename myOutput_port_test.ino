/*
 * myOutput_port__test.ino
 * Test for the 8 bit digital input to the natus system
 * the arduino will output the numbers 1-10 20 times with a 
 * 10 second delay inbetween each set of 1-10
 * the duration an interval will vary between 100 microseconds
 * and 2 seconds in 100 microsecond increments
 * erik.emeric@gmail.com
 */

#include<IO_Port_8bit.h>

IO_Port_8bit myport(2,3,4,5,6,7,8,9,0);
const int digPWDT = 100;
const int digIPI = 100;
void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);   
}

void loop() 
{
  for (int j = 1; j < 21;j++){; //multiplier for the duration of the pulse
    digitalWrite(LED_BUILTIN, HIGH);
    for (int i = 1; i < 11;i++){;
      myport.send_8bit_data(i);
      delayMicroseconds(digPWDT*j);
      myport.send_8bit_data(0);
      delayMicroseconds(digIPI*j);
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
  /*
 myport.send_8bit_data(0);
  delay(dly);
  myport.send_8bit_data(2);
  delay(dly);
  myport.send_8bit_data(0);
  delay(dly);
  myport.send_8bit_data(3);
  delay(dly);
  myport.send_8bit_data(0);
  delay(dly);
  myport.send_8bit_data(4);
  delay(dly);
  myport.send_8bit_data(0);
  delay(dly);
  myport.send_8bit_data(5);
  delay(dly);
  myport.send_8bit_data(0);
  delay(dly);
  myport.send_8bit_data(6);
  delay(dly);
  myport.send_8bit_data(0);
  delay(dly);
  myport.send_8bit_data(7);
  delay(dly);
  myport.send_8bit_data(0);
  delay(dly);
  myport.send_8bit_data(7);
  delay(dly);
  myport.send_8bit_data(8);
  delay(dly);
  */
}
