#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <mbus/mbus.h>
#include <mosquitto.h>

#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC "meters/data"

//#define R_COMMAND 0x53;
//#define D_REQUEST 0x52;

typedef enum
{	RELAY_COMMAND,
	DATA_REQUEST
}FrameType;

int watCal_set_frame(mbus_frame *, int);
void watCal_proccess_frame();
#endif
