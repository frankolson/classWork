 /*
 eepromWriteRead
 Frank Olson
 v.1
 24 September 2014
 Arduino Mega 2560 v3
 Last Update: 24 September 2014 

 This program is for Lab 4 of EE 426. This program will allow users to 
 read and write to eeprom.
 */

 // Add libraries
 #include <EEPROM.h>
 
 // Initialize variables
 int address = 0;
 byte value;
 int choice = 0;


 void setup(){
        // Initialize serial connection and prompt user
 	Serial.begin(9600);
 	Serial.print("Choose option. 0:Read all; 1: Write to address...: ");
 }

 void loop(){
        // setup buffer for multiple digits in user response
 	char buffer[] = {' ',' ',' ',' ',' ',' ',' '}; // Receive up to 7
 
        // wait for user response
 	while(!Serial.available());
 	if (Serial.available()){
 		// read incoming
 		choice = Serial.read() - '0';
 		Serial.println(choice);
 
 		if(choice == 0){
 			while(address < 512){
 				// read a byte from the current address of the EEPROM
 				value = EEPROM.read(address);
		                
                                // print address and value
 				Serial.print(address);
 				Serial.print("\t");
				Serial.print(value, DEC);
 				Serial.println();
 		
  				// advance to the next address of the EEPROM
 				address = address + 1;
 				
 				delay(500);
 			}
 		}else if(choice == 1){
 			Serial.print("Choose an address (int): ");
 			while(!Serial.available()); 
                        // read all of the users response
 			Serial.readBytesUntil('\n', buffer, 7);
 			address = int(atoi(buffer));
 			Serial.println(address);
 
 			Serial.print("Choose a data value (char): ");
 			while(!Serial.available());
                        // read all of the users response
 			Serial.readBytesUntil('\n', buffer, 7);
 			value = byte(atoi(buffer));
 			Serial.println(value);
 
 			EEPROM.write(address, value);
 
                        // print changes made
 			Serial.println("Change made: ");
 			Serial.print(address);
 			Serial.print("\t");
 			Serial.print(EEPROM.read(address));
 			Serial.println();
 			
 		}
 	}
 	
 	address = 0;
 	Serial.print("Choose option. 0:Read all; 1: Write to address...: ");
 
 }
