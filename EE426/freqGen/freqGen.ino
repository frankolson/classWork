/*
freqGen
Frank Olson
v.1
08 October 2014
Arduino Mega 2560 v3
Last Update: 08 October 2014

This Program is for Lab 6 of EE 426. 

*/

 // Set pin numbers
 const int button1 = 2; // speaker toggle
 const int button2 = 3; // reset button
 const int speaker = 4;
 const int toneFreq = 1000;
 
 // Initialize variables
 int buttonState1 = 0;
 int buttonState2 = 0;
 int state = LOW;
 int choice;
 int freq;
 
 void setup(){
   // Initialize pin states
   pinMode(button1,INPUT);
   pinMode(button2,INPUT);
   pinMode(speaker,OUTPUT);
   
   // Initialize serial communication
   Serial.begin(9600);
   
   // Give initial user prompt and get response
   Serial.print("What is the wanted speaker frequency (Hz): ");
   freq = userDigits();
 }
 
 void loop(){
   // check buttonStates
   buttonState1 = digitalRead(button1);
   buttonState2 = digitalRead(button2);

   // check to see if the button was previous pressed
   if (buttonState1 == HIGH && state == LOW ){
     Serial.print("Pressed");
     state = HIGH;
     delay(200);
   }else if(buttonState1 == HIGH && state == HIGH ){
     state = LOW;
     Serial.print("Pressed");
     delay(200);
   }
   
   // if conditions from previous if/else statement check out,
   // play tone on the speaker 
   if(state == HIGH){
     tone(speaker, freq, 10);
   }
   
   // check for reset button
   if(buttonState2 == HIGH){
     changeFreq();
     state = LOW;
   }
 }
 
 void changeFreq(){
   Serial.println();
   Serial.print("What is the wanted speaker frequency (Hz): ");
   freq = userDigits();
 }
 
 unsigned int userDigits(){
   // initialize variable
   int value;
   
   // setup buffer for multiple digits in user response
   char buffer[] = {' ',' ',' ',' ',' ',' ',' '}; // Receive up to 7
   
   // wait for input
   while(!Serial.available());
   
   // get user input and convert to int
   Serial.readBytesUntil('\n', buffer, 7);
   value = int(atoi(buffer));
   Serial.print(value);
   Serial.println();
   
   // return converted input
   return value; 
 }
