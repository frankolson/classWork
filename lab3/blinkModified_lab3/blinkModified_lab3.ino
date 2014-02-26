/*
  Modified Blink Program
  Frank Olson
  V.1
  20 February 2014
  Arduino Mega 2560 R3
*/

// Assign Led to Pin 13
int led = 13;

// Setup Function
void setup() {
  // initialize the digital pin 13 as output.
  pinMode(led, OUTPUT);
}

// Loop Function
void loop() {
  // SOS signal
  signalFast( 3 );
  signalSlow( 3 );
  signalFast( 3 );
  delay(2000);
}

// Fast Blinking
void signalFast( int reps ) {
  // Using the for loop to blink 'reps' amount of times
  for( int i=0; i<reps; i++ ) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500); 
    digitalWrite(led, LOW);   // turn the LED off (LOW is the voltage level)
    delay(500);
  }
}

// Slow Blinking
void signalSlow( int reps ) {
  // Using the for loop to blink 'reps' amount of times
  for( int i=0; i<reps; i++ ) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000); 
    digitalWrite(led, LOW);   // turn the LED off (LOW is the voltage level)
    delay(1000);
  }
}
