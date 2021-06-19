# MADMaksClinicalTStamperDigital2
Arduino code for controlling the bluetooth module used in the clinical MultiAttribute Decision Making (MADMaks) experiment
The Bluetooth module connects to an android tablet running the Experiment App, receives event identifiers and timestamps from the app, and provides 2 digital channels of output via BNC connectors to a Natus electrophysiology system. It also provides an 8 bit digital output to the same Natus electrophysiology system. 

**There are 2 sketches in this repository**
* *MADMaksClinicalTStamperDigital2.ino*
* *myOutput_port__test.ino*
  * Tests the 8 bit digital input to the Natus system for optimal and reliable input parameters
  * The Arduino will output the numbers 1-10 20 times with a 10 second delay inbetween each set of 1-10
  * The duration and interval will vary between 100 microseconds and 2 milliseconds in 100 microsecond increments. 
  * No Bluetooth connection is required for this sketch. Just plug in the VGA cable to the Natus system.   

**Dependencies**

*MADMaksClinicalTStamperDigital2* requires 2 libraries to function 
* IO_Port_8bit
  * This library creates an 8-bit IO port for Arduino by combining any 8 Arduino pins. With this library, one can send/get direct 8-bit data.
  * https://create.arduino.cc/projecthub/ambhatt/8-bit-io-port-library-for-arduino-5c11a6  
* SoftwareSerial
  * The SoftwareSerial library allows serial communication on digital pins of the Arduino other than the dedicated serial communication pins (digital pins 0 and 1), using software to replicate the functionality.

To install a new library into your Arduino IDE you can use the Library Manager (see the url below for a tutorial.
* SoftwareSerial is included with the Arduino IDE so it's simply a matter of selecting the library from the list of available libraries
* IO_Port_8bit is not included with the Arduino IDE but it can be downloaded from the URL below and has to be manually installed using the Arduino IDE
  * https://create.arduino.cc/projecthub/ambhatt/8-bit-io-port-library-for-arduino-5c11a6  

**Installing Additional Arduino Libraries**
* https://www.arduino.cc/en/guide/libraries
