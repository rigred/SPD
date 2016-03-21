/*
 * Project: Modifying RAM SPD data
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2016 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/modifying-ram-spd-data/
 */

#include <Wire.h>
#include <util/crc16.h>

#define DRAM_MAX_CLK_800	0x14
#define DRAM_MAX_CLK_1066	0x0F
#define DRAM_MAX_CLK_1333	0x0C
#define DRAM_MAX_CLK_1600	0x0A

#define SPDSIZE 256
#define BUFFSIZE 16 // Must be a multiple of 2, upto 256
#define EEPADDR 0x50

static byte spdData[SPDSIZE];

void setup()
{
	Serial.begin(115200);

	byte initOK = eepInit();
	if(initOK != 0)
	{
		Serial.print(F("EEPROM Init failed ("));
		Serial.print(initOK);
		Serial.println(F(")"));
		while(1);
	}
	
//	Serial.println("Init OK");

	delay(2000);

	loadSPD();
	if(!checkCRC())
	{
		Serial.println(F("CRC failed!"));
		while(1);
	}

	setMaxClockSpeed(DRAM_MAX_CLK_1600);

	setCRC(calcCRC());
	//saveSPD(); // WARNING uncomment this line at your own risk!

	dump(false);
}

void loop()
{
}

static void loadSPD()
{
	for(byte i=0;i<SPDSIZE/BUFFSIZE;i++)
	{
		if(!eepRead(i * BUFFSIZE, spdData + (i * BUFFSIZE), BUFFSIZE))
		{
			Serial.print("Read failed at ");
			Serial.println(i * BUFFSIZE);
		}
	}
}

static void saveSPD()
{
	for(uint16_t i=0;i<SPDSIZE;i++)
	{
		if(!eepWrite(i, spdData[i]))
		{
			Serial.print("Write failed at ");
			Serial.println(i);
		}
	}
}

static bool checkCRC()
{
	if(calcCRC() == *((uint16_t*)&spdData[126]))
		return true;
	return false;
}

static uint16_t calcCRC()
{
	// Check the "Exclude serial from CRC" bit
	byte len = (spdData[0] & 0x80) ? 117 : 126;

	uint16_t crc = 0;
	for(byte i=0;i<len;i++)
		crc = _crc_xmodem_update(crc, spdData[i]);

	return crc;
}

static void setMaxClockSpeed(byte maxClk)
{
	spdData[12] = maxClk;
}

static void setCRC(uint16_t crc)
{
	*((uint16_t*)&spdData[126]) = crc;
}

static void dump(bool raw)
{
	if(raw) // Print raw bytes
	{
		for(byte i=0;i<SPDSIZE/BUFFSIZE;i++)
			Serial.write(spdData + (i * BUFFSIZE), BUFFSIZE);
	}
	else // Print bytes as hex values
	{
		for(uint16_t i=0;i<SPDSIZE;i++)
		{
			if((i % 16) == 0) // New line every 16 values
				Serial.println();
			printHex(spdData[i]);
			Serial.print(" ");
		}
	}
}

static void printHex(byte num)
{
	char hex[3];
	sprintf(hex, "%02X", num);
	Serial.print(hex);
}

static byte eepInit()
{
	Wire.begin();
	Wire.setClock(100000L); // We're not using any external pullups so use a slow clock
	Wire.beginTransmission(EEPADDR);
	Wire.write(0);
	byte status = Wire.endTransmission();
	return status;
}

static bool eepRead(byte addr, byte* buffer, byte count)
{
	Wire.beginTransmission(EEPADDR);
	Wire.write(addr);
	if(Wire.endTransmission() != 0)
		return false;

	Wire.requestFrom(EEPADDR, (int)count);
	for(uint16_t i=0;i<count;i++)
		buffer[i] = Wire.read();

	return true;
}

// Write 1 byte at a time, otherwise we have to deal with paging stuff
static bool eepWrite(byte addr, byte data)
{
	Wire.beginTransmission(EEPADDR);
	Wire.write(addr);
	Wire.write(data);
	if (Wire.endTransmission() != 0)
		return false;

	// Wait for write to complete
	for(byte i=200;i--;)
	{
		delayMicroseconds(100);
		Wire.beginTransmission(EEPADDR);
		Wire.write(0);
		if(Wire.endTransmission() == 0)
			return true;
	}

	return false;
}
