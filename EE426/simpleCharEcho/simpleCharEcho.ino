/*
simpleCharEcho
Frank Olson
v.2
15 September 2014
Arduino Mega 2560 v3
Last Update: 17 September 2014

This Program is for Lab 3 of EE 426. A character will be inputted over the
serial monitor (or another program like putty) then the program will check
if it is a letter. If it is it will chnage cases and send out the char as 
a 10 bit signal.

Currently using Arduino Libraries, but I want to switch it to ANSI C.
*/


// Setup Function
void setup (){
  Serial.begin(9600);
  UCSR0C = UCSR0C | B00000110;
  UCSR0B = UCSR0B | B00000100;
 }

// Main Loop
void loop(){
  
  // read from port 0
  if (Serial.available()) {
    char inByte = Serial.read();
    
    if (inByte >= 65) {
      if (inByte <= 90) {
        inByte = inByte + 32;
      } else if (inByte >= 97) {
        if (inByte <= 122) {
          inByte = inByte - 32;
        }
      }
    }
    
    Serial.write(inByte);
  }
  
}
