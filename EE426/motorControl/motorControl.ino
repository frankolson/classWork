 /*
 motorControl
 Frank Olson
 v.1
 03 November 2014
 Arduino Mega 2560 v3
 Last Update: 03 November 2014 

 This Program is for Lab 10 of EE 426. 
 This also utilizes a SevSeg Display instead of the serial monitor.

 */
 
 
 ////////////// Variables for 4digit SevSeg Display //////////////
 const int A = 22;
 const int B = 23;
 const int C = 24;
 const int D = 25;
 const int E = 26;
 const int F = 27;
 const int G = 28;
 const int GND1 = 30;
 const int GND2 = 31;
 const int GND3 = 32;
 const int GND4 = 33;
 int time = 0;
 /////////////////////////////////////////////////////////////////
 
 ///////////////// Variables for motor adjustment// //////////////
 const float motorRes = 8.6;
 float testVoltage;
 int sensorTestVoltage;
 /////////////////////////////////////////////////////////////////
 
 
 /////////////// Variables for Switch Power Supply ///////////////
 const int powerPin = 13;
 int duty = 0;
 float input = 0.0;
 /////////////////////////////////////////////////////////////////
 
 
 ////////////////// Constants for ADC Volt Meter /////////////////
 const float r1 = 1000;
 const float r2 = 1000;
 const float resFactor = 1023.0*(r2/(r1+r2));
 /////////////////////////////////////////////////////////////////
 
 
 void setup(){
   // Setup of SevSeg Display pins
   pinMode(A, OUTPUT);
   pinMode(B, OUTPUT);
   pinMode(C, OUTPUT);
   pinMode(D, OUTPUT);
   pinMode(E, OUTPUT);
   pinMode(F, OUTPUT);
   pinMode(G, OUTPUT);
   pinMode(GND1, OUTPUT);
   pinMode(GND2, OUTPUT);
   pinMode(GND3, OUTPUT);
   pinMode(GND4, OUTPUT);
   
   // Setup of switching power supply pin
   pinMode(powerPin, OUTPUT);
   
   // Ask for initial user input
   Serial.begin(9600);
   Serial.print("Enter a voltage level from 0-6V (i.e: 000mV): ");
   input = userDigits();
   duty = map(input,0,600,0,255);
   Serial.println(duty);
   delay(2000);
 }
 
 void loop(){
   // Run power supply unless user wants to change the duty cycle
   analogWrite(powerPin, duty);
   
   // Print voltage level... need to see if this will work
   float sensor = analogRead(A0);
   float voltage = (sensor/resFactor) * (5);
   displayDigs(voltage * 100);
   
   
   /*
   if(time==500){
     Serial.println(voltage);
     time=0;
   }else{
     time++;
   }
   */
   
   // Get back emf
   sensorTestVoltage = analogRead(A1);
   testVoltage = sensorTestVoltage * (5.0/1023);
   
   if(time==500){
     Serial.println(testVoltage);
     time=0;
   }else{
     time++;
   }
   
   // Correct speed
   if(testVoltage > 0.01){
     duty = duty + 1;
   }
 }
 
 //////////////////// Functions for each digit ///////////////////
 void digit0() {
   digitalWrite(A, LOW);
   digitalWrite(B, LOW);
   digitalWrite(C, LOW);
   digitalWrite(D, LOW);
   digitalWrite(E, LOW);
   digitalWrite(F, LOW);
   digitalWrite(G, HIGH);
 }
 
 void digit1() {
   digitalWrite(A,HIGH);
   digitalWrite(B, LOW);
   digitalWrite(C, LOW);
   digitalWrite(D, HIGH);
   digitalWrite(E, HIGH);
   digitalWrite(F, HIGH);
   digitalWrite(G, HIGH);
 }
 
 void digit2() {
   digitalWrite(A,LOW);
   digitalWrite(B, LOW);
   digitalWrite(C, HIGH);
   digitalWrite(D, LOW);
   digitalWrite(E, LOW);
   digitalWrite(F, HIGH);
   digitalWrite(G, LOW);
 }
 
 void digit3() {
   digitalWrite(A,LOW);
   digitalWrite(B, LOW);
   digitalWrite(C, LOW);
   digitalWrite(D, LOW);
   digitalWrite(E, HIGH);
   digitalWrite(F, HIGH);
   digitalWrite(G, LOW);
 }
 
 void digit4() {
   digitalWrite(A,HIGH);
   digitalWrite(B, LOW);
   digitalWrite(C, LOW);
   digitalWrite(D, HIGH);
   digitalWrite(E, HIGH);
   digitalWrite(F, LOW);
   digitalWrite(G, LOW);
 }
 
 void digit5() {
   digitalWrite(A,LOW);
   digitalWrite(B, HIGH);
   digitalWrite(C, LOW);
   digitalWrite(D, LOW);
   digitalWrite(E, HIGH);
   digitalWrite(F, LOW);
   digitalWrite(G, LOW);
 }
 
 void digit6() {
   digitalWrite(A,LOW);
   digitalWrite(B, HIGH);
   digitalWrite(C, LOW);
   digitalWrite(D, LOW);
   digitalWrite(E, LOW);
   digitalWrite(F, LOW);
   digitalWrite(G, LOW);
 }
 
 void digit7() {
   digitalWrite(A,LOW);
   digitalWrite(B, LOW);
   digitalWrite(C, LOW);
   digitalWrite(D, HIGH);
   digitalWrite(E, HIGH);
   digitalWrite(F, HIGH);
   digitalWrite(G, HIGH);
 }
 
 void digit8() {
   digitalWrite(A, LOW);
   digitalWrite(B, LOW);
   digitalWrite(C, LOW);
   digitalWrite(D, LOW);
   digitalWrite(E, LOW);
   digitalWrite(F, LOW);
   digitalWrite(G, LOW);
 }
 
 void digit9() {
   digitalWrite(A, LOW);
   digitalWrite(B, LOW);
   digitalWrite(C, LOW);
   digitalWrite(D, LOW);
   digitalWrite(E, HIGH);
   digitalWrite(F, LOW);
   digitalWrite(G, LOW);
 }
 /////////////////////////////////////////////////////////////////
 
 
 /////////////// Function to display a single digit //////////////
 void displayDig(int digit){
   switch(digit){
     case 0:
     digit0();
     break;
     
     case 1:
     digit1();
     break;
     
     case 2:
     digit2();
     break;
     
     case 3:
     digit3();
     break;
     
     case 4:
     digit4();
     break;
     
     case 5:
     digit5();
     break;
     
     case 6:
     digit6();
     break;
     
     case 7:
     digit7();
     break;
     
     case 8:
     digit8();
     break;
     
     case 9:
     digit9();
     break;
   }
 }
 /////////////////////////////////////////////////////////////////
 
 
 //////////// Function to display a four digit number ////////////
 void displayDigs(int number){
   // define temp number
   int temp;
   
   // First Digit
   temp = number/1000;
   displayDig(temp);
   digitalWrite(GND1, HIGH);
   digitalWrite(GND2, LOW);
   digitalWrite(GND3, LOW);
   digitalWrite(GND4, LOW);
   delay(1);
   
   // Second Digit
   temp = number%1000;
   digitalWrite(GND1, LOW);
   displayDig(temp/100);
   digitalWrite(GND2, HIGH);
   delay(1);
    
   // Third Digit
   temp = number%100;
   digitalWrite(GND2, LOW);
   displayDig(temp/10);
   digitalWrite(GND3, HIGH);
   delay(1);
   
   // Fourth Digit
   temp = number%10;
   digitalWrite(GND3, LOW);
   displayDig(temp);
   digitalWrite(GND4, HIGH);
   delay(1);
 }
 /////////////////////////////////////////////////////////////////
 
 
 /////////////// Function for gathering user input ///////////////
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
 /////////////////////////////////////////////////////////////////
