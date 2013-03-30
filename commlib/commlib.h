#ifndef __COMLIB_H
#define __COMLIB_H

#define COMM_DELAY 208
#define COMM_MSG_DELAY 2000
#define COMM_PIN 4

typedef void(*CommHandler)(unsigned char);

void comm_setup(CommHandler func);
void comm_loop();
void comm_send(char *data, int len);
void comm_send(const char *str);
void comm_send(int data);
void comm_send(long int data);

#endif