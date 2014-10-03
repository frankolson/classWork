/*
  Modified Blink Program in ANSI C
  Frank Olson
  V.1
  20 February 2014
  Arduino Mega 2560 R3
*/

// Define functions
void MyDelay(unsigned long mSecondsApx);
void signalFast( unsigned long reps );
void signalSlow( unsigned long reps );

// Setup Function
void setup() {
  unsigned char *portDDRB;
  
  portDDRB = (unsigned char *) 0x24;
  
  *portDDRB |= 0x80;
}

// Loop Function
void loop() {
  // SOS signal
  signalFast( 3 );
  signalSlow( 3 );
  signalFast( 3 );
  MyDelay(2000);
}

// Delay Function
void MyDelay(unsigned long mSecondsApx) {
  volatile unsigned long i;
  unsigned long endTime = 1000 *mSecondsApx;
  
  for( i=0; i<endTime; i++);
}

// Fast Blinking
void signalFast( unsigned long reps ) {
  volatile unsigned long i;
  unsigned char *portB;
  
  // Using the for loop to blink 'reps' amount of times
  for( i=0; i<reps; i++ ) {
    portB = (unsigned char *) 0x25; 
    
    // Turn on portB then wait half a second
    *portB |= 0x80;
    MyDelay(500);
    // Turn off portB then wait half a second 
    *portB &= 0x7F;
    MyDelay(500);
  }
}

// Slow Blinking
void signalSlow( unsigned long reps ) {
  volatile unsigned long i;
  unsigned char *portB;
  
  // Using the for loop to blink 'reps' amount of times
  for( i=0; i<reps; i++ ) {
    portB = (unsigned char *) 0x25; 
    
    // Turn on portB then wait a second
    *portB |= 0x80;
    MyDelay(1000);
    // Turn off portB then wait a second
    *portB &= 0x7F;
    MyDelay(1000);
  }
}
