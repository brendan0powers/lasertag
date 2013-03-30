#include "commlib.h"
#include "WProgram.h"

void comm_recv();

unsigned long int _lasttime = 0;
bool _lastval = false;
bool _msgstarted = false;
CommHandler _handler;
unsigned char _data = 0;;
unsigned char _pos = 0;

void comm_setup(CommHandler func)
{
	pinMode(COMM_PIN, INPUT);
	_handler = func;
	attachInterrupt(0, comm_recv, CHANGE);
}

void comm_sendChar(unsigned char data)
{
	unsigned char clock=0x00;
	
	for(int i=0; i < 8; i++)
	{
		clock = 0x01;
		digitalWrite(COMM_PIN, LOW);
		delayMicroseconds(COMM_DELAY);
		clock = 0x00;
		digitalWrite(COMM_PIN, ((data ^ clock) == 0x01) ? HIGH : LOW);
		delayMicroseconds(COMM_DELAY);
	}
	
	data = data >> 1;
}

void comm_send(char *data, int len)
{
	pinMode(COMM_PIN, OUTPUT);
	digitalWrite(COMM_PIN, LOW);
	delayMicroseconds(COMM_MSG_DELAY);
	for(int i = 0; i < len; i++)
	{
		comm_sendChar(data[i]);
	}
	
	digitalWrite(COMM_PIN, HIGH);
	pinMode(COMM_PIN, INPUT);
}

void comm_send(const char *str)
{
	comm_send((char *)str, strlen(str) + 1);
}

void comm_send(int data)
{
	comm_send((char *)&data, 2);
}

void comm_send(long int data)
{
	comm_send((char *)&data, 4);
}

void comm_recv()
{
	bool curval = digitalRead(COMM_PIN) == HIGH;
	unsigned long int interval = micros() - _lasttime;
	
	if(_lasttime == 0) //Start of a message, just record it and do nothing
	{
		_lastval = curval;
		_lasttime = micros();
		return;
	}
	else if(!_msgstarted)
	{
		if((interval >= 180) && (interval <= 220))
		{
			_msgstarted = true;
			_lastval = curval;
			_data = 0;
			_pos = 0;
		}
		return;
	}
	
	if(_lastval == curval) //This shouldn't happen, if it does, something broke
	{
		_lastval = false;
		_lasttime = 0;
		_msgstarted = false;
		return;
	}
	
	if(!_lastval && curval)
	{
		_data |= 0x01;
	}
	else
	{
		_data |= 0x00;
	}
	
	_data = _data << 1;
	_pos++;
	
	_lasttime = micros();
	
	if(_pos = 7)
	{
		_pos = 0;
		(*_handler)(_data);
	}
}

void comm_loop()
{
	unsigned long int interval = micros() - _lasttime;
	
	if(_msgstarted && (interval >= 500))
	{
		_lastval = false;
		_lasttime = 0;
		_msgstarted = false;
	}
}
