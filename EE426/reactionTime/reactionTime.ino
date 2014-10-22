 /*
 reactionTime
 Frank Olson
 v.1
 15 October 2014
 Arduino Mega 2560 v3
 Last Update: 15 October 2014 

 This Program is for Lab 5 of EE 426. This program will time a users
 reaction time to a light and speaker going off using a button, a switch,
 a speaker, an led, and seven segment displays.

 */

 // setup pins
 int button = 2;
 int led = 13;
 
 // initialize variable
 int buttonState = 0;
 int startTime;
 int duration;

 void setup(){
   // set pin modes
   pinMode(button, INPUT);
   pinMode(led, OUTPUT);

   // initialize serial;
   Serial.begin(9600);

 }

 void loop(){


   // intial delay between 1-7 seconds
   delay(random(1000,7000));
   
   // grab system time
   startTime = millis();

   // turn on led
   digitalWrite(led, HIGH);

   // wait for user reaction
   while(!buttonState){
     buttonState = digitalRead(button);
   }

   // after user input get response time
   duration = millis() - startTime;

   // print time
   Serial.println(duration);
   
   // give user time to see result, 10 seconds
   delay(10000);

   // turn led off and get ready for next round
   digitalWrite(led, LOW);

 }
