/*
Seven Segment Display with Potentiometer 
Frank Olson
V.1
09 March 2014
Arduino Mega 2560 R3
Last Update: 09 April 2014

This program uses ANSI C to acces the ports on the Arduino Mega
controling a seven segment display. The program loops continously
through all of the hexadecimal charactersand, the speed of which
is controled by a potentiometer. I used ideas and code from both 
my seven segment displa program and the AnalogInOut arduino
program included in the IDE download. This is practice for port 
selection using ANSI C compatible with most micro controllersand 
the answer to question three of homework 8.
*/

// Define hardware pointers
volatile unsigned char *myADCSRA = (unsigned char *) 0x7A;
volatile unsigned char *myADCSRB = (unsigned char *) 0x7B;
volatile unsigned char *myADMUX  = (unsigned char *) 0x7C;
volatile unsigned char *myDIDR0  = (unsigned char *) 0x7E;

// Variables to output converted analog data
volatile unsigned long outputValue = 0;
volatile unsigned long num = 900;
volatile unsigned long denom = 1023;


// Define functions
void MyDelay(unsigned long mSecondsApx);

// Setup Function
void setup(){
  // Initialize ports for ADC
  ADCinit();
  
  // Next few steps set up port addresses
  unsigned char *portDDRK;
  portDDRK = (unsigned char *) 0x107;
  *portDDRK = 0xFF;
}

// Loop Function
void loop(){
  sevenSegmentLoop();
}

void ADCinit(){ 
  // ADC enable, started, auto trigger enabled, prescaler of 16 selected
  ADCSRA = 0xE4;
  // Set to free running
  ADCSRB = 0;
  // Settings to use Vcc, right adjust, and channel select ADC5
  ADMUX = 0x45;
  // Input pin associated with ADC channel disabled
  DIDR0 = 0x20;
}

// Reads analog value from pinA5
unsigned int analogExtract(){
  // Get value from ADC data register and convert to int
  int value = (int)(ADC);
  
  // return the analog reading
  return value;
}

// Remaps range
unsigned long resistanceToTime(unsigned int inputValue){
  
  // remap the integer range from 0-1023 to 100-1000
  outputValue = ((num * inputValue)/denom) + 100;
  
  return outputValue;
}

// Delay Function
void MyDelay(unsigned long mSecondsApx){
  volatile unsigned long i;
  unsigned long endTime = 1000 *mSecondsApx;
  
  for( i=0; i<endTime; i++);
}

// Runs through hex digits on seven segment display
void sevenSegmentLoop(){
  unsigned char *portK;

  portK = (unsigned char *) 0x108;
  
  // Run through all the hexadicamal characters on the seven 
  // segment display
  
  // LED segment 0
  *portK = 0b01111110;
  MyDelay(resistanceToTime(analogExtract()));
  
  // LED segment 1
  *portK = 0b00001100;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment 2
  *portK = 0b10110110;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment 3
  *portK = 0b10011110;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment 4
  *portK = 0b11001100;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment 5
  *portK = 0b11011010;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment 6
  *portK = 0b11111010;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment 7
  *portK = 0b01001110;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment 8
  *portK = 0b11111110;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment 9
  *portK = 0b11001110;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment A
  *portK = 0b11101110;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment b
  *portK = 0b11111000;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment C
  *portK = 0b01110010;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment d
  *portK = 0b10111100;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment E
  *portK = 0b11110010;
  MyDelay(resistanceToTime(analogExtract()));
 
  // LED segment F
  *portK = 0b11100010;
  MyDelay(resistanceToTime(analogExtract()));
}
