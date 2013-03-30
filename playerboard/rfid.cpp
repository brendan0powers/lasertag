#include "rfid.h"
#include "WProgram.h"
#include <NewSoftSerial.h>

NewSoftSerial Serial1(RFID_RX,RFID_TX);

char asciiToHex(char value)
{
	if (value > 57)
		return (value - 55);
	else
		return (value ^ 48);
}

bool readRFID(char id[11])
{
	id[10] = 0;

	byte begin = 0;
	//byte id[10];
	byte parity[2];
	byte end = 0;

	begin = Serial1.read();
	for(int i = 0; i < 10; i++)
	{
		id[i] = Serial1.read();
	}
	parity[0] = Serial1.read();
	parity[1] = Serial1.read();
	end = Serial1.read();

	/*
	Serial.print(begin,HEX);
	Serial.print(' ',BYTE);
	for(int i = 0; i < 10; i++)
		Serial.print(id[i],BYTE);
	Serial.print(' ',BYTE);
	Serial.print(parity[0],BYTE);
	Serial.print(parity[1],BYTE);
	Serial.print(' ',BYTE);
	Serial.print(end,HEX);
	Serial.print(' ',BYTE);
	*/	

	byte data[5];
	byte checkdata = 0;

	for(int i=0; i < 5; i++)
	{
		data[i] = (asciiToHex(id[i*2]) << 4) | (asciiToHex(id[(i*2) + 1]));
		//Serial.print(data[i],HEX);	
		checkdata ^= data[i];
	}

	//Serial.print(' ',BYTE);

	if (checkdata == ((asciiToHex(parity[0]) << 4) | (asciiToHex(parity[1]))))
	{
		//Serial.println("VALID");
		return(true);
	}
	else
	{
		//Serial.println("NOT VALID");
		return(false);
	}
}

void rfidInit()
{
	Serial1.begin(9600);
}

bool rfidAvailable()
{
	return(Serial1.available());
}