#ifndef __RDIF_H_
#define __RFID_H_

#define RFID_RX 5
#define RFID_TX 6

char asciiToHex(char value);
bool readRFID(char id[11]);
void rfidInit();
bool rfidAvailable();

#endif