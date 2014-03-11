/*
Arduino Piano
Frank Olson
V.1
10 March 2014
Arduino Mega 2560 R3
Last Update: 11 March 2014

Simple Serial ECHO script : Written by ScottC 03/07/2012
*/

// Use a variable called byteRead to temporarily store
// the data coming from the computer
byte byteRead;

void setup() {                
  // Turn the Serial Protocol ON
  Serial.begin(9600);
}

void loop() {
  // Check if data has been sent from the computer:
  if (Serial.available()) {
    // Read the most recent byte
    byteRead = Serial.read();
    // ECHO the value that was read, back to the serial port
    Serial.write(byteRead);
  }
}
