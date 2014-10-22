 /*
 bigBenChimes
 Frank Olson
 v.1
 22 October 2014
 Arduino Mega 2560 v3
 Last Update: 22 October 2014 

 This Program is for Lab 8 of EE 426. 

 */
 
 // Pin constants
 int speaker = 6;
 
 // Assign note values in Hz for melody
 int six = 262;     // note C4
 int eight = 349;   // note F4
 int nine = 392;    // note G4
 int ten = 440;     // note A4
 int p = 0;         // pause
 
 // Initialize melody and constant duration
 int melody[] = {ten,eight,nine,six,p,six,nine,ten,eight,p,
                 ten,nine,eight,six,p,six,nine,ten,eight};     
 int duration = 250;
 
 void setup(){
   Serial.begin(9600);
   
   // setup speaker
   pinMode(speaker, OUTPUT);
   
   for(int i = 0; i<19; i++){
     // play and display note
     Serial.print(melody[i]);
     Serial.print(", ");
     Serial.println(duration);
     
     tone(speaker, melody[i], duration);
     delay(duration);
   }
   noTone(speaker);
 }
 
 void loop(){
   // no loop needed as the melody will only play once
 }
 
 
