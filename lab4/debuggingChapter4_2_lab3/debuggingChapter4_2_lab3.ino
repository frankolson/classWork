/*
  Run-Time Debugging Practice
  Frank Olson
  V.1
  26 February 2014
  Arduino Mega 2560 R3
*/

void NewDelay(unsigned char mSecondsApx);
// Correction: void NewDelay(unsigned long mSecondsApx);

void setup(){
  unsigned char *portDDRB;
  portDDRB = (unsigned char *) 0x24;
  *portDDRB |= 0x80;
}

void loop (){
  unsigned char *portB;
  portB = (unsigned char *) 0x25;
  
  *portB |= 0x80;
  NewDelay(100);
  *portB &= 0x7F;
  NewDelay(100);

}

void NewDelay(unsigned char mSecondsApx){
// Correction: void NewDelay(unsigned long mSecondsApx){
  volatile unsigned char i;
  // Correction: volatile unsigned long i;
  unsigned long endTime = 1000 * mSecondsApx;
  
  for (i=0; i<endTime; i++);
}

