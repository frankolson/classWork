/*
Echo 2C
Frank Olson
V.1
31 March 2014
Arduino Mega 2560 R3


*/

// RDA: Recieve Data Avalable
#define RDA 0x80 
// TBE: Transmiter Buffer Empty
#define TBE 0x20

void setup(){
  
}

void loop(){
  
}

// Functions to initialize the serial port
unsigned USART0baud(int USART0baud){
  
}

// Function that will examine the RDA status bit and return a true
// (non-zero) if RDA is true otherwise it will return a false (0
// value in ACC
unsigned char USART0kbhit(){
  
}

// Function that will read one character from the serial port and 
// return it
unsigned char USART0getchar(){
  
}

// Function will wait until the serial port TBE status bit is high
// and then will take the character USART0pdata and send the 
// charatcer out to the serial port
void USART0putchar(unsigned char USART0pdata){
  
}
