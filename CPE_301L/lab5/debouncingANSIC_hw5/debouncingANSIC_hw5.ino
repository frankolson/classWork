/*
Monitoring and Debouncing in ANSI C
Frank Olson
V.1
06 March 2014
Arduino Mega 2560 R3

This program was created to get used to polling and it should be
noted that a significant amount of this code was pulled from the
text however I tried my best to use my own comments.
*/

// Define functions
void MyDelay(unsigned long mSecondsApx);

// Setup Function
void setup() {
  // Declaring pointer variables
  unsigned char *portDDRB;
	unsigned char *portDataB;
	unsigned char *portPinB;
	
	// Loading up pointer variables
	portDDRB = (unsigned char *) 0x24;
	portDataB = (unsigned char *) 0x25;
  	portPinB = (unsigned char *) 0x23;

	// Making bit 0 of portB an input
	#define BITO_MASK 0x01 // 0000 0001
	*portDDRB = (*portDDRB) & (~BITO_MASK);	// Bit 0 as input
	*portDataB = (*portDataB) | BITO_MASK;	// Turn on the bit 0
																					// pull-up resistor

	// Turning of LED on bit 7 of port B
	*portDDRB |= 0x7F;

}

// Loop Function
void loop() {
	static unsigned volatile char previousSample = 0x00;
	static unsigned volatile char currentSample;

	currentSample = (*portPinB) & BITO_MASK; // Only looking at
																					 // bit 0
	if (currentSample != previousSample){
		// Wait temporarily to make sure this is not signal
		// bouncing from the button
		Mydelay(20);

   		// Reading value again
		currentSample = (*portPinB) & BITO_MASK; // Only looking
																					   // at bit 0
		// Run the if statement again after the check
		if (currentSample != previousSample){
			// Swap LED state
			*portDDRB ^= 0x80;

			// Store current sample in previous sample
			previousSample = currentSample;
		}
	}
}

// Delay Function
void MyDelay(unsigned long mSecondsApx) {
  volatile unsigned long i;
  unsigned long endTime = 1000 *mSecondsApx;
  
  for( i=0; i<endTime; i++);
}
