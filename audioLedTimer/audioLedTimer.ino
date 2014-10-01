 /*
 audioLedTimer
 Frank Olson
 v.1
 30 September 2014
 Arduino Mega 2560 v3
 Last Update: 30 September 2014 

 This Program is for Lab 5 of EE 426. This project will utilize serial
 connection, an LED, and a button to create a timer. The program will
 prompt for a time interval and then wait for the user to push the button.
 once the user has pressed the button the LED will turn on and the 
 speaker will beep once. After the user's selected time interval is up,
 the LED will turn of and the speaker will beep again.

 */
 
 // Set pin numbers
 const int button1 = 2; // timer button
 const int button2 = 3; // reset button
 const int speaker = 4;
 const int led = 13;
 const int toneFreq = 1000;
 
 // Initialize variables
 int buttonState1 = 0;
 int buttonState2 = 0;
 int choice;
 int delayTime;
 
 void setup(){
   // Initialize pin states
   pinMode(led,OUTPUT);
   pinMode(button1,INPUT);
   pinMode(button2,INPUT);
   pinMode(speaker,OUTPUT);
   
   // Initialize serial communication
   Serial.begin(9600);
   
   // Give initial user prompt and get response
   Serial.print("What is the wanted time interval: ");
   delayTime = userDigits();
 }
 
 void loop(){
   // check buttonStates
   buttonState1 = digitalRead(button1);
   buttonState2 = digitalRead(button2);
   
   // action when buttons are pressed
   if(buttonState1==HIGH){
     // play sound
     tone(speaker, toneFreq, 500);
     
     // turn on LED and start timer
     digitalWrite(led, HIGH);
     delay(delayTime);
     
     // turn of LED, and play ending sound
     digitalWrite(led, LOW); 
     tone(speaker, toneFreq, 500);
   }
   
   // else if statement to allow users to change the time during the
   // program if the wish
   else if(buttonState2==HIGH){
     Serial.print("What is the wanted time interval: ");
     
     // get user input
     delayTime = userDigits();
   }
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
   
   // return converted input
   return value; 
 }
