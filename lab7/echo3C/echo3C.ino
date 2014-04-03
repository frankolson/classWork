/*
Echo 3C
Frank Olson
V.1
02 April 2014
Arduino Mega 2560 R3
Last Update: 02 April 2014

This program is for lab 7. This is a simple erial echo program that takes a byte
from the computer input and spits it back out to the serial monitor in its hex
value equivalent.
*/

// Mask to get least significant bits
#define LEASTSIGBITS 0x0F

// RDA: Recieve Data Avalable
#define RDA 0x80 
// TBE: Transmiter Buffer Empty
#define TBE 0x20

void setup(){
  // Initialize the serial port on USART 0
  USART0init(9600);
  
}

void loop(){
  char tempByte;
  
  while(USART0kbhit()==0){}
  tempByte = USART0getchar();
  USART0putchar(tempByte);
  
}

// Functions to initialize the serial port
unsigned USART0init(int USART0baud){
  // defining the clock speed
  unsigned long FCPU = 16000000;
  // Initializing the BAUD rate variable
  unsigned int tBaud;
  // Setting the value of tBaud
  tBaud = (FCPU / 16 / USART0baud-1);

  // Set normal transmission speed, disabled multi-processor mode, turn on data
  // data register empty
  UCSR0A = 0x20;
  // Enabled the reciever and transmitter, turned off both RX and TX complete
  // interupt 
  UCSR0B = 0x18;
  // Set the character size to 8-bit, parity mode 0ff, 1 stop bit, set to 
  // asynchronous USART mode
  UCSR0C = 0x06;
  
  // Setting the BAUD rate
  UBRR0H = (tBaud>>8);
  UBRR0L = (tBaud & 0xFF);
}

// Function that will examine the RDA status bit and return a true
// (non-zero) if RDA is true otherwise it will return a false (0
// value in ACC
unsigned char USART0kbhit(){
  // And UCSR0A with RDA to check whether RXC0(bit7) is high
  if(UCSR0A & RDA){
    // Return TRUE value
    return 1;
  }
  // Return FALSE value
  return 0;
}

// Function that will read one character from the serial port and 
// return it
unsigned char USART0getchar(){
  // Read and clear value from UDR0 data register
  return UDR0;
}

// Function will wait until the serial port TBE status bit is high
// and then will take the character USART0pdata and send the 
// charatcer out to the serial port
void USART0putchar(unsigned char USART0pdata){
  // Initialize variable to temporarily store char value
  unsigned char tempChar;
  // Shift over 4 bits to take the most significant bits
  tempChar = USART0pdata >> 4;
  // take the least significant bits (actually most significant
  // bits, haha trickery!)
  UDR0 = (tempChar & LEASTSIGBITS) + '0';
  
  // Properly convert to HEX
  if((USART0pdata & LEASTSIGBITS) == 10){
    tempChar = 'A';
    UDR0 = tempChar;
  }else if((USART0pdata & LEASTSIGBITS) == 11){
    tempChar = 'B';
    UDR0 = tempChar;
  }else if((USART0pdata & LEASTSIGBITS) == 12){
    tempChar = 'C';
    UDR0 = tempChar;
  }else if((USART0pdata & LEASTSIGBITS) == 13){
    tempChar = 'D';
    UDR0 = tempChar;
  }else if((USART0pdata & LEASTSIGBITS) == 14){
    tempChar = 'E';
    UDR0 = tempChar;
  }else if((USART0pdata & LEASTSIGBITS) == 15){
    tempChar = 'F';
    UDR0 = tempChar;
  }else{
    tempChar = (USART0pdata & LEASTSIGBITS);
    UDR0 = tempChar + '0';
  }
  
  // Wait until TBE is ready
  while((UCSR0A & TBE) == 0){}
  
  // Add in space for asthetics
  UDR0 = ' ';
}