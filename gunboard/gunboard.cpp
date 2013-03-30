#include "IRremote.h"
#include <Wire.h>
#include "commlib.h"

#define LED_PIN 4
#define TRIGGER 13
#define RELOAD 2
#define WIRE_ADDRESS 0x10
#define WIRE_DEST 0x20

IRsend ir;
int shots = 5;

void sendString(const char* str)
{
	long int outPut = 0;
	const char* buff = str;
	int i = 1;
	for (; *buff; ++i)
	{
		outPut = outPut << 8;
		outPut |= *buff;
		buff++;
		if (i == 4)
		{
			i = 0;
			ir.sendNEC(outPut, 32);
			delay(100);
			outPut = 0;
		}
	}
	if (i != 1)
	{
		for (;i != 5; ++i)
		{
			outPut = outPut << 8;
		}
		ir.sendNEC(outPut, 32);
		delay(100);
	}
}

void sendWireString(const char *data)
{
	Wire.beginTransmission(WIRE_DEST);
	Wire.send((char *)data);
	Wire.endTransmission();
}

void receiveEvent(int howMany) 
{
	while(Wire.available()) {
		byte b = Wire.receive(); //receive the byte
		Serial.print(b);
	}
}

void setup()                    // run once, when the sketch starts
{
	pinMode(LED_PIN, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(TRIGGER, INPUT);           // set pin to input
	digitalWrite(TRIGGER, HIGH);       // turn on pullup resistors
	pinMode(RELOAD, INPUT);           // set pin to input
	digitalWrite(RELOAD, HIGH);       // turn on pullup resistors

	Serial.begin(57600);
	ir.enableIROut(38);
	//Wire.begin(WIRE_ADDRESS);
	//Wire.onReceive(receiveEvent);

	digitalWrite(LED_PIN, HIGH);
	delay(250);
	digitalWrite(LED_PIN, LOW);
	delay(250);
	digitalWrite(LED_PIN, HIGH);
	delay(250);
	digitalWrite(LED_PIN, LOW);
	delay(250);
	digitalWrite(LED_PIN, HIGH);
	delay(250);
	digitalWrite(LED_PIN, LOW);
	delay(250);
}

void loop()                     // run over and over again
{	
	if(!digitalRead(TRIGGER))
	{
		digitalWrite(LED_PIN,HIGH);

		Serial.print("Firing: ");
		Serial.print(shots);
		Serial.println();
		
		ir.sendNEC((shots + 1) * 100, 32);

		digitalWrite(LED_PIN,LOW);

		comm_send((shots + 1) * 100);
		
		if(shots > 0) shots--;
	}
	if(!digitalRead(RELOAD))
	{
		shots = 5;
	}
}

