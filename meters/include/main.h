#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <mbus/mbus.h>
#include <curl/curl.h>

#define SLAVE "/dev/pts/8" // Change to the slave virtual port
#define MASTER "/dev/pts/2" // Change to master port

//#define R_COMMAND 0x53;
//#define D_REQUEST 0x52;

typedef enum
{	RELAY_COMMAND,
	DATA_REQUEST
}FrameType;

void initialize_frame(mbus_frame *frame, FrameType type, uint8_t address);
void configure_serial_port(int fd);
void send_request(mbus_handle *handle, uint8_t address);
void initialize_frame(mbus_frame *frame, FrameType type, uint8_t address);
void initialize_slave(mbus_handle *handle, uint8_t);
#endif
