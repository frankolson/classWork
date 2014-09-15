/*
simpleCharEcho
Frank Olson
v.1
15 September 2014
Arduino Mega 2560 v3
Last Update: 15 September 2014

This Program is for Lab 3 of EE 426. A character will be inputted over the
serial monitor (or another program like putty) and will be transferred over
a TX port to another RX port. The serial monitor will be reading from that
RX port and will display the echoed text on the screen.

Based heavily on my Echo 3C program that can be found here:
https://github.com/frankolson/CPE_301L/blob/master/lab7/echo3C/echo3C.ino
*/

// Define initial parameters
int bRate = 9600;            //Baud Rate
#define LEASTSIGBITS 0x0F    // Mask to get least significant bits
#define RDA 0x80             // RDA: Recieve Data Avalable
#define TBE 0x20             // TBE: Transmiter Buffer Empty


// Setup Function
void setup (){
    // Initialize the serial port on USART 0
    USART3init(bRate);
}

// Main Loop
void loop(){
    char tempByte;
  
    // Wait for character to be entered
    while(USART3kbhit()==0){}

    // Print out entered char on monitor
    tempByte = USART3getchar();
    USART3putchar(tempByte);
}

// USART initializing function
void USART3init(int USART3baud){
    // defining the clock speed
    unsigned long FCPU = 16000000;
    // Initializing the BAUD rate variable
    unsigned int tBaud;
    // Setting the value of tBaud
    tBaud = (FCPU / 16 / USART3baud-1);

    // Set normal transmission speed, disabled multi-processor mode, turn on data
    // data register empty
    UCSR3A = 0x20;
    // Enabled the reciever and transmitter, turned off both RX and TX complete
    // interupt 
    UCSR3B = 0x18;
    // Set the character size to 8-bit, parity mode 0ff, 1 stop bit, set to 
    // asynchronous USART mode
    
    UCSR3C = 0x06;
  
  
    // Setting the BAUD rate
    UBRR3H = (tBaud>>8);
    UBRR3L = (tBaud & 0xFF);
}

// Function that will examine the RDA status bit and return a true
// (non-zero) if RDA is true otherwise it will return a false (0
// value in ACC
unsigned char USART3kbhit(){
    // And UCSR3A with RDA to check whether RXC3(bit7) is high
    if(UCSR3A & RDA){
      // Return TRUE value
      return 1;
    }

    // Return FALSE value
    return 0;
}

// Function that will read one character from the serial port and return it
unsigned char USART3getchar(){
    // Read and clear value from UDR0 data register
    return UDR3;
}

// Function will wait until the serial port TBE status bit is high
// and then will take the character USART0pdata and send the 
// charatcer out to the serial port
void USART3putchar(unsigned char USART3pdata){
    // Initialize variable to temporarily store char value
    unsigned char tempChar;
    // Shift over 4 bits to take the most significant bits
    tempChar = USART3pdata >> 4;
    // take the least significant bits (actually most significant
    // bits, haha trickery!)
    UDR3 = (tempChar & LEASTSIGBITS) + '0';
  
    // Wait until TBE is ready
    while((UCSR3A & TBE) == 0){}
  
    // Add in space for asthetics
    UDR3 = ' ';
}
