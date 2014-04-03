/*
Echo 4C
Frank Olson
V.1
31 March 2014
Arduino Mega 2560 R3
Last Update: 02 April 2014

This program is practice for the CPE301 midterm. This is a simple erial echo 
program that takes in multiple bytes from the computer input and spits back out 
the string to the serial monitor.

///#### SCRATCH THIS, MOVED ONTO NEXT VERSION ####///


*/

// RDA: Recieve Data Avalable
#define RDA 0x80 
// TBE: Transmiter Buffer Empty
#define TBE 0x20

void setup(){
  // Initialize the serial port on USART 0
  USART0init(9600);
  
}

void loop(){
  //volatile unsigned char tempByte;
  unsigned char test[] ={'1','2','3','4','5','a','b','c','d','e'};
  
  //volatile unsigned int length = sizeof(test);
  //Serial.println(length);
  USART0putstring(test);
  
  // Insert a new line
  USART0putchar(0b00001010);
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
  // Write value to UDR0 Data register
  UDR0 = USART0pdata;
}

void USART0putstring(unsigned char USART0pdata[]){
  // Intitialize size variable and loop variable
  volatile unsigned int length = sizeof(USART0pdata);
  volatile unsigned int i;
  
  // Loop through array
  for(i = 0; i < length; i++){
    // Write value to UDR0 Data register
    UDR0 = USART0pdata[i];
    // Wait until TBE is ready
    while((UCSR0A & TBE) == 1){}
  }

}

void USART0getstring(){
  volatile unsigned char stringArray[140];
  volatile unsigned int length = 140;

  // Loop through adding to array
  for(i = 0; i < length; i++){
    stringArray[i] = USART0getchar();
  }
}
