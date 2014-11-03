 /*
 switchingPowerSupply
 Frank Olson
 v.1
 27 October 2014
 Arduino Mega 2560 v3
 Last Update: 27 October 2014 

 This Program is for Lab 9 of EE 426. 

 */
 
 
 // Initialize pins
 int powerPin = 13;
 int reset = 2;
  
 // other variables
 const float r1 = 1500;
 const float r2 = 1000;
 int duty = 0;
 float input = 0.0;
 int resetState = 0;
 const float resFactor = 1023.0/(r2/(r1+r2));
 
 void setup(){
   // set pinMOde
   pinMode(powerPin, OUTPUT);
   pinMode(reset, INPUT);
   
   // Setup Serial and ask for input
   Serial.begin(9600);
   Serial.print("Enter a voltage level from 0-9V (i.e: 000mV): ");
   input = userDigits();
   duty = map(input,0,899,0,255);
 }
 
 void loop(){
   // run power supply unless user wants to change the duty cycle
   analogWrite(powerPin, duty);
   
   // print voltage level... need to see if this will work
   float sensor = analogRead(A0);
   float voltage = (sensor/resFactor) * (50.0);
   Serial.println(voltage);
   
   // Correct
   if(voltage > (input/100.0)){
     duty = duty - 1;
   }else if(voltage < (input/100.0)){
     duty = duty + 1;
   }
   
   // check for reset button
   resetState = digitalRead(reset);
   if(resetState == HIGH){
     changeDuty();
   }
 }
 
 // function for changing the Duty Cycle
 void changeDuty(){
   Serial.println();
   Serial.print("Enter a voltage level from 0-9V (i.e: 0.00V): ");
   duty = map(userDigits(),0,900,0,255);
 }
 
 // function for gathering user input
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
