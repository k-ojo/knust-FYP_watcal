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
#include <curl/curl.h>
#include <cjson/cJSON.h>
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC "meters/data"

//#define R_COMMAND 0x53;
//#define D_REQUEST 0x52;

typedef enum
{	RELAY_COMMAND,
	DATA_REQUEST
}FrameType;

typedef struct _mbusReading
{
	int room_id;
	double r_val;
	char meter_address[16];
	char serial_number[16];
	time_t timestamp;
}_mbusReading;


int watCal_set_frame(mbus_frame *, int);
void watCal_proccess_frame(mbus_frame *frame);
void send_mbus_frame(const char* frame_json);
#endif
