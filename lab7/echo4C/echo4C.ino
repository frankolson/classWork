/*
Echo 4C
Frank Olson
V.2
31 March 2014
Arduino Mega 2560 R3
Last Update: 03 April 2014

This program is practice for the CPE301 midterm. This is a simple erial echo 
program that takes in multiple bytes from the computer input and spits back out 
the string to the serial monitor.

In this verison I tried to use pointers more, the other version was bugging me 
so I just restarted
*/

// RDA: Recieve Data Avalable
#define RDA 0x80 
// TBE: Transmiter Buffer Empty
#define TBE 0x20

// Initialize global variable pointers
unsigned char stringData[140];
volatile unsigned int i = 0;

void setup(){
  // Initialize the serial port on USART 0
  USART0init(9600);
}

void loop(){
  // Setting up array pointer
  volatile unsigned char *array;
  array = stringData;
  
  // setting up variables
  volatile unsigned char tempChar;
  volatile unsigned int intTest;
  
  // Wait for input then add to array
  while(USART0kbhit()==0){}
  tempChar = USART0getchar();
  array = USART0getstring(tempChar);
  intTest = (int)(tempChar);
  
  // Check for cairrage return, if so return the array and reset iterator
  if(intTest == 0xD){
    USART0putstring(array);
    i = 0;
  }
}

// Functions to initialize the serial port
unsigned USART0init(volatile unsigned int USART0baud){
  // defining the clock speed
  volatile unsigned long FCPU = 16000000;
  // Initializing the BAUD rate variable
  volatile unsigned int tBaud;
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
// (non-zero) if RDA is true otherwise it will return a false (0)
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
void USART0putchar(volatile unsigned char USART0pdata){
  // Write value to UDR0 Data register
  UDR0 = USART0pdata;
}

void USART0putstring(volatile unsigned char *string){
  // Write all of the array to UDRO
  for(unsigned int j = 0; j < i; j++){
    while((UCSR0A & TBE)==0){}
    UDR0 = *(string + j);
  }
}

unsigned char *USART0getstring(volatile unsigned char value){
  // Setting up array pointer
  unsigned char *p;
  p = stringData;
  
  // Store new value
  stringData[i] = value;
  
  // Increase incrementer
  i++;
  
  return p;
}
