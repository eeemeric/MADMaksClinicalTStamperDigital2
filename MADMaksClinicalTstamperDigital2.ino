//#include <IO_Port_8bit.h>

#include<IO_Port_8bit.h>
#include <SoftwareSerial.h>
// create 8 bit port object
IO_Port_8bit myport(6,7,8,9,10,11,12,13, 0);
// create software serial object
SoftwareSerial mySerial(2,3); // Rx and Tx for the bluetooth

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;
boolean bytesFlag = false;
boolean readFlg = false;
boolean evtIdReceived = false;
String inString = "";    // string to hold input


boolean pdFlg = false;
int oldPDstate;
int newPDstate;
int idx = 0;

const int A0_0  = 4;// analog output for pulses
const int A0_1  = 5;// analog output for pulses
//const int A0_2  = 14;// photodiode

const int pwdth = 2000; // pulse width in microseconds
const int ipi   = 2000; // inter pulse interval in microseconds
//const int BR    = 115200;
unsigned long strt_b;
unsigned long strt_r;
unsigned long strt_p;
String serialOutString;


String bin      = "";
String pBin     = "";
/*
// had to get rid of this bit because the Uno only has 14 digital channels
// need to use 0 and 1 for the serial usb, 2-9 for the 8 bit digital
// 10-11 for rx and tx of the bluetooth
// 12-13 for analog output to ephys 

const int blueLEDpin    = 7; // blue tooth connection intdicator
const int greenLEDpin   = 5; // transmit pulses
const int yellowLEDpin  = 6; // serial receive
const int redLEDpin     = 4; // serial transmit
*/
void setup() 
{
    // used to for output to physiol system
  pinMode(A0_0, OUTPUT); // sets the digital pin 0 as output
  pinMode(A0_1, OUTPUT); // sets the digital pin 1 as output
  // used to read the state of photodiode
  // pinMode(A0_2, INPUT); // sets the digital pin 1 as input
  
  // had to get rid of this bit because the Uno only has 14 digital channels
  /*
   pinMode(redLEDpin, OUTPUT);
  pinMode(yellowLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);
  pinMode(blueLEDpin, OUTPUT);
 
  // set LEDs to off
  digitalWrite(blueLEDpin, LOW);
  digitalWrite(greenLEDpin, LOW);
  digitalWrite(yellowLEDpin, LOW);
  digitalWrite(redLEDpin, LOW);
   */

  // set physiol channels to low
  digitalWrite(A0_0, LOW);
  digitalWrite(A0_1, LOW);

  // Establish serial connection
  mySerial.begin(115200);
  // wait for serial connection
  while (!mySerial){
  }
  //Clear serial buffer
  mySerial.flush();
}

void loop() 
{
  recvDateWithEndMarker();
  /*
  myport.send_8bit_data(85);
  delay(500);
  myport.send_8bit_data(170);
  delay(500);
  */
}

void recvDateWithEndMarker() {
  String tempNum = "0";
  // Read serial input:
  //checkPDstate();
  while (mySerial.available() > 0) {
    int inChar = mySerial.read();
    String pStr = "";
    //send single pulse when the first byte arrives
    if (idx==0){
      strt_b = millis();
      // digitalWrite(yellowLEDpin, HIGH);
      sendSinglePulse();//1 pulse
      
      idx++;
    }
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
      idx++;
      //checkPDstate();
    }
    // if you get a newline,
    if (inChar == '\n') {
      // digitalWrite(yellowLEDpin,LOW);
      //checkPDstate();
      if (inString.length()==11){        
        parseDateStr();
      }
      else{
        divide(inString.toInt());
        bin = pBin;
      }

      if (inString.length()<3){
        // send 8 bit data
        myport.send_8bit_data(inString.toInt());
        delayMicroseconds(pwdth);
        myport.send_8bit_data(0);
        delayMicroseconds(pwdth);
        
        // add flag to wait till timestamp is received
        evtIdReceived = true;
        switch (inString.toInt()) {
          case 0:
            serialOutString = "00 Trial parameters received, " + String(strt_b);
            
            break;
          case 1:    // 1, trial start
            serialOutString = "01 Trialstart, " + String(strt_b);
            
            break;
          case 2:    // 2, Tap O1A1/A+1
            serialOutString = "02 Tap A+1, " + String(strt_b);
            
            break;
          case 3:    // 3, Tap O1P1/P+1
            serialOutString = "03 Tap P+1, " + String(strt_b);
            
            break;
          case 4:    // 4, Tap O1A2/A+2
            serialOutString = "04 Tap A+2, " + String(strt_b);
            
            break;
          case 5:    // 5, Tap O1P2/P+2
            serialOutString = "05 Tap P+2, " + String(strt_b);
            
            break;
          case 6:    // 6, Tap O2A1/A-1
            serialOutString = "06 Tap A-1, " + String(strt_b);
            
            break;
          case 7:    // 7, Tap O2P1/P-1
            serialOutString = "07 Tap P-1, " + String(strt_b);
            
            break;
          case 8:    // 8, Tap O2A2/A-2
            serialOutString = "08 Tap A-2, " + String(strt_b);
            
            break;
          case 9:    // 9, Tap O2P2/P-2
            serialOutString = "09 Tap P-2, " + String(strt_b);
            
            break;
          case 10:    // 10, Tap O3A1/A+3
            serialOutString = "10 Tap A+3, " + String(strt_b);
            
            break;
          case 11:    // 11, Tap O3P1/P+3
            serialOutString = "11 Tap P+3, " + String(strt_b);
            
            break;
          case 12:    // 12, Tap O3A2/A-3
            serialOutString = "12 Tap A-3, " + String(strt_b);
            
            break;
          case 13:    // 13, Tap O3P2/P-3
            serialOutString = "13 Tap P-3, " + String(strt_b);
            
            break;
          case 14:    // 14, Tap O4A1/A+4
            serialOutString = "14 Tap A+4, " + String(strt_b);
            
            break;
          case 15:    // 15, Tap O4P1/P+4
            serialOutString = "15 Tap P+4, " + String(strt_b);
            
            break;
          case 16:    // 16, Tap O4A2/A-4
            serialOutString = "16 Tap A-4, " + String(strt_b);
            
            break;
          case 17:    // 17, Tap O4P2/P-4
            serialOutString = "17 Tap P-4, " + String(strt_b);
            
            break;
          case 18:    // 18, Show O1A1/A+1
            serialOutString = "18 Show A+1, " + String(strt_b);
            
            break;
          case 19:    // 19, Show O1P1/P+1
            serialOutString = "19 Show P+1, " + String(strt_b);
            
            break;
          case 20:    // 20, Show O1A2/A+2
            serialOutString = "20 Show A+2, " + String(strt_b);
            
            break;
          case 21:    // 21, Show O1P2/P+2
            serialOutString = "21 Show P+2, " + String(strt_b);
            
            break;
          case 22:    // 22, Show O2A1/A-1
            serialOutString = "22 Show A-1, " + String(strt_b);
            
            break;
          case 23:    // 23, Show O2P1/P-1
            serialOutString = "23 Show P-1, " + String(strt_b);
            
            break;            
          case 24:    // 24, Show O2A2/A-2
            serialOutString = "24 Show A-2, " + String(strt_b);
            
            break;
          case 25:    // 25, Show O2P2/P-2
            serialOutString = "25 Show P-2, " + String(strt_b);
            
            break;
          case 26:    // 26, Show O3A1/A+3
            serialOutString = "26 Show A+3, " + String(strt_b);
            
            break;
          case 27:    // 27, Show O3P1/P+3
            serialOutString = "27 Show P+3, " + String(strt_b);
            
            break;
          case 28:    // 28, Show O3A2/A-3
            serialOutString = "28 Show A-3, " + String(strt_b);
            
            break;
          case 29:    // 29, Show O3P2/P-3
            serialOutString = "29 Show P-3, " + String(strt_b);
            
            break;
          case 30:    // 30, Show O4A1/A+4
            serialOutString = "30 Show A+4, " + String(strt_b);
            
            break;
          case 31:    // 31, Show O4P1/P+4
            serialOutString = "31 Show P+4, " + String(strt_b);
            
            break;
          case 32:    // 32, Show O4A2/A-4
            serialOutString = "32 Show A-4, " + String(strt_b);
            
            break;
          case 33:    // 33, Show O4P2/P-4
            serialOutString = "33 Show P-4, " + String(strt_b);
            
            break;
          case 34:    // 34, Mask On
            serialOutString = "34 Mask On, " + String(strt_b);
            
            break;
          case 35:    // 35, EarlyMaskOn
            serialOutString = "35 Early Mask On, " + String(strt_b);
            
            break;
          case 36:    // 36, Choice
            serialOutString = "36 Choice, " + String(strt_b);
            
            break;
          case 37:    // 37, Result
            serialOutString = "37 Result, " + String(strt_b);
            
            break;
          case 38:    // 38, NextTrialAvailable
            serialOutString = "38 NextTrialAvailable, " + String(strt_b);
            
            break;
          case 39:    // 39, NextTrialAvailable
            serialOutString = "39 NextButtonTapped, " + String(strt_b);
            
            break;
          case 40:    // 40, HorizontalDisplay
            serialOutString = "40 HorizontalDisplay, " + String(strt_b);
            
            break;
          case 41:    // 41, VerticalDisplay
            serialOutString = "41 VerticalDisplay, " + String(strt_b);
            
            break;
          case 42:    // 42, BlueScreenOn
            serialOutString = "42 BlueScreenOn, " + String(strt_b);
            
            break;
          case 43:    // 43, login time
            serialOutString = "43 LoginTime, " + String(strt_b);
            break;
        }
      }
      // timestamp from tablet is 11 characters long (mmddhhssmmm)
      // and is always preceeded by a 2 digit identifier
      //Serial1.println("received");
      if (evtIdReceived == true && inString.length()==11){
        // send digital start timestamp, 44
        myport.send_8bit_data(44);
        delayMicroseconds(pwdth);
        myport.send_8bit_data(0);
        delayMicroseconds(pwdth);

        // send timestamp values
        for (int i = 0; i < 11;i++){
          tempNum = inString[i];
          // had to add 1 to the timestamp so we can represent 0s in the date
          myport.send_8bit_data(tempNum.toInt()+1); 
          delayMicroseconds(pwdth);
          myport.send_8bit_data(0);
          delayMicroseconds(pwdth);
        }
        // send digital end timestamp, 45
        myport.send_8bit_data(44);
        delayMicroseconds(pwdth);
        myport.send_8bit_data(0);
        delayMicroseconds(pwdth);
        
        //send arduino timestamp back to tablet  
        mySerial.println(inString + " " + serialOutString);
        // Serial1.println(inString + " " + serialOutString);
        //Serial1.println(inString + " " + serialOutString);
        evtIdReceived = false;
        // start timestamp, 44
        // end timestamp, 45
        
      }
      // login date is yyyy:mm:dd:hh:mm:ss:mmm and 17 characters long
      /*
      if (evtIdReceived == true && inString.length()==17){
        //send arduino timestamp back to tablet  
        mySerial.println(inString + " " + serialOutString);
        //Serial1.println(inString + " " + serialOutString);
        evtIdReceived = false;
      }
      */
     //mySerial.println(inString);
      
      sendPulses();
      sendSinglePulse();
      delayMicroseconds(ipi);
      sendSinglePulse();
      delayMicroseconds(ipi);

      // digitalWrite(redLEDpin, HIGH);      
      //mySerial.println(strt_b);
      //digitalWrite(redLEDpin, LOW);
      //mySerial.println(inString);
      // clear the strings for new input:
      inString  = "";
      bin       = "";
      pBin      = "";
      newData   = true;
      idx = 0;
    }
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

void sendPulses(){  
  int  n = bin.length();  
  // digitalWrite(greenLEDpin, HIGH);
  sendSinglePulse();
  delayMicroseconds(ipi);
  for (int i = 0; i < n;i++){
    if (bin[i]=='0'){
      digitalWrite(A0_0, HIGH); // sets the digital pin 0 to HIGH
      delayMicroseconds(pwdth);
      digitalWrite(A0_0, LOW); // sets the digital pin 0 to LOW
    }
    else{
      digitalWrite(A0_1, HIGH); // sets the digital pin 0 to HIGH
      delayMicroseconds(pwdth);
      digitalWrite(A0_1, LOW); // sets the digital pin 0 to LOW
    }
    delayMicroseconds(ipi);      
  }
  sendSinglePulse();
  delayMicroseconds(ipi);
  //digitalWrite(greenLEDpin, LOW);
}

/*
void checkPDstate() {    
    newPDstate = digitalRead(A0_2);
    if (newPDstate != oldPDstate){
      //strt_p = millis();
      //digitalWrite(A0_0, HIGH); // sets the digital pin 0 to HIGH              
      //delayMicroseconds(pwdth);            // waits for a pwdth ms
      //digitalWrite(A0_0, LOW); // sets the digital pin 0 to LOW
      pdFlg = true;
      oldPDstate = newPDstate;            
    }  
}
*/

void divide(int num){
  if (num>1){
    divide(num/2);
  }
  //bin += num%2;
  pBin += num%2;
}

void parseDateStr(){
      /*
      parse the timestamp str, receivedChars
      5 bits for day
      5 bits for hour (24 hr clock)
      6 bits for minute
      6 bits for second
      10 bits for milisecond
      */
     
      // 5 bits for day
      String pStr = inString.substring(0,2);
      divide(pStr.toInt()); // convert to binary
      padStr(5); // pad string with zeros so the desired number of bits equals nBits
      bin += pBin; // concatenate strings
      pBin = "";
           
      // 5 bits for hour (24 hr clock)
      pStr = inString.substring(2,4);
      divide(pStr.toInt()); // convert to binary      
      padStr(5); // pad string with zeros
      bin += pBin; // concatenate strings
      pBin = "";
     
      // 6 bits for minute
      pStr = inString.substring(4,6);      
      divide(pStr.toInt()); // convert to binary      
      padStr(6); // pad string with zeros
      bin += pBin; // concatenate strings
      pBin = "";

      // 6 bits for second
      pStr = inString.substring(6,8);
      divide(pStr.toInt()); // convert to binary      
      padStr(6); // pad string with zeros
      bin += pBin; // concatenate strings
      pBin = "";

      // 10 bits for milisecond
      pStr = inString.substring(8,11);
      divide(pStr.toInt()); // convert to binary      
      padStr(10); // pad string with zeros
      bin += pBin; // concatenate strings
      pBin = "";
}

void padStr(int nBits){
  int  n = pBin.length();
  if (n<nBits){
    for (int i = 0; i< nBits - n;i++){
      pBin = "0" + pBin;
    }
  }  
}
