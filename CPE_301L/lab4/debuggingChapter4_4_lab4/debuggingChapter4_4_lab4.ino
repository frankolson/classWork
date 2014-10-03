/*
  Run-Time Debugging Practice 2
  Frank Olson
  V.1
  26 February 2014
  Arduino Mega 2560 R3
*/

void NewDelay(unsigned long mSecondsApx);

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

void NewDelay(unsigned long mSecondsApx){
  volatile unsigned char i;
  unsigned char j;
  unsigned long k;
  unsigned long endTime = 1000 * mSecondsApx;
  
  for (i=0; i<endTime; i++){
    j = 10;
    do {
      j = j-1;
      k = 1/j;
    } while (k>0);
    // Correction: Comment out code inside the for loop, it is
    //             pointless and the while statement will never
    //             be true 
  }
}
