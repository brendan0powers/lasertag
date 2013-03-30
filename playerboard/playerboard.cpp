/*
 * Blink
 *
 * The basic Arduino example.  Turns on an LED on for one second,
 * then off for one second, and so on...  We use pin 13 because,
 * depending on your Arduino board, it has either a built-in LED
 * or a built-in resistor so that you need only an LED.
 *
 * http://www.arduino.cc/en/Tutorial/Blink
 */

#include "rfid.h"
#include <IRremote.h>
#include <Wire.h>

#define LED_PIN 13
#define SPEAKER_PIN 12
#define IR_PIN 3
#define WIRE_ADDRESS 0x20
#define WIRE_DEST 0x10

#define NOTE_a 4545 // 220 Hz
#define NOTE_as 4290 // 233 Hz
#define NOTE_b 4050 // 247 Hz
#define NOTE_c 3822 // 262 Hz
#define NOTE_cs 3608 // 277 Hz
#define NOTE_d 3405 // 294 Hz
#define NOTE_ds 3214 // 311 Hz
#define NOTE_e 3034 // 330 Hz
#define NOTE_f 2863 // 349 Hz
#define NOTE_fs 2703 // 370 Hz
#define NOTE_g 2551 // 392 Hz
#define NOTE_af 2408 // 415 Hz
#define NOTE_A 2273 // 440 Hz
#define NOTE_AS 2145 // 466 Hz
#define NOTE_B 2025 // 494 Hz
#define NOTE_C 1911 // 523 Hz
#define NOTE_CS 1804 // 554 Hz
#define NOTE_D 1703 // 587 Hz
#define NOTE_DS 1607 // 622 Hz
#define NOTE_E 1517 // 659 Hz
#define NOTE_F 1432 // 698 Hz
#define NOTE_FS 1351 // 740 Hz
#define NOTE_G 1276 // 784 Hz
#define NOTE_AF 1204 // 831 Hz
// Define a special note, 'R', to represent a rest
#define  NOTE_R     1
#define NOTE_END 2

IRrecv irrecv(IR_PIN);
decode_results results;
const char* decodeString(long int str);
void sendWireString(const char *data);
void receiveEvent(int howMany);

void playTone(int tone, long duration)
{
        long elapsed_time=0;
	duration = duration *1000;
        if(tone > 1)
        {
                while(elapsed_time < duration)
                {
                        digitalWrite(SPEAKER_PIN,HIGH);
                        delayMicroseconds(tone/2);
                        digitalWrite(SPEAKER_PIN,LOW);
                        delayMicroseconds(tone/2);

                        elapsed_time += tone;
                }
        }
        else
        {
                delay(duration/1000);
        }
}

void setup()                    // run once, when the sketch starts
{
	pinMode(LED_PIN, OUTPUT);
	pinMode(SPEAKER_PIN, OUTPUT);
// 	pinMode(IR_PIN, INPUT);
	
	Serial.begin(57600);
// 	rfidInit();
	digitalWrite(LED_PIN,LOW);
	irrecv.enableIRIn(); // Start the receiver
// 	Serial.print(sizeof(int));
	//Wire.begin(WIRE_ADDRESS);
	//Wire.onReceive(receiveEvent);
	Serial.println("Playing Node");
	playTone(NOTE_C, 500);
	Serial.println("Done");
}

void loop()                     // run over and over again
{
	/*
	if(rfidAvailable())
	{
		digitalWrite(LED_PIN,HIGH);
		delay(500);
		digitalWrite(LED_PIN,LOW);

		char id[11];
		if(readRFID(id))
		{
			Serial.println(id);
		}
		else
		{
			Serial.println("Failed to read the RFID.. id?...");
		}
	}
	*/
// 	if(digitalRead(IR_PIN))
// 		digitalWrite(LED_PIN,LOW);
// 	else
// 		digitalWrite(LED_PIN,HIGH);

	/*
	if (irrecv.decode(&results)) {
		(decodeString(results.value));
		irrecv.resume(); // Receive the next value
	}
	*/

	if (irrecv.decode(&results)) {
		digitalWrite(LED_PIN, HIGH);
		Serial.print("Received: ");
		Serial.print(results.value);
		Serial.println();

		switch(results.value)
		{
			case 600:
				playTone(NOTE_F, 250);
				break;
			case 500:
				playTone(NOTE_E, 250);
				break;
			case 400:
				playTone(NOTE_D, 250);
				break;
			case 300:
				playTone(NOTE_C, 250);
				break;
			case 200:
				playTone(NOTE_B, 250);
				break;
			case 100:
				playTone(NOTE_A, 250);
				break;
			default:
				playTone(NOTE_a, 250);
				break;
		}

		digitalWrite(LED_PIN, LOW);
		irrecv.resume(); // Receive the next value
	}
}


const char* decodeString(long int str)
{
	static char strBuff[4];
	for (int i = 0; i < 4; ++i)
	{
		char c = ((str & 0xff000000) >> 24);
		strBuff[i] = c;
		if (c != 0)
		{
			Serial.print(c);
			Wire.beginTransmission(WIRE_DEST);
			Wire.send(c);
			Wire.endTransmission();
		}
		str = str << 8;
	}
	/*
	for (char c = ((str & 0xff000000) >> 24); c != 0; c = ((str & 0xff000000) >> 24))
	{
		Serial.print(c);
		str = str << 8;
	}
	*/
	return strBuff;
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
/*
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
			outPut = 0;
		}
	}
	if (i != 1)
	{
		for (;i != 1; ++i)
		{
			outPut = outPut << 8;
		}
		ir.sendNEC(outPut, 32);
	}
}
*/
