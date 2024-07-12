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

#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC "meters/data"

//#define R_COMMAND 0x53;
//#define D_REQUEST 0x52;

typedef enum
{	RELAY_COMMAND,
	DATA_REQUEST
}FrameType;

mbus_frame_data* _parse_mbus_frame(mbus_frame* reply, const unsigned char *raw_buff, size_t raw_buff_length);
int watCal_set_frame(mbus_frame *, int);
void watCal_proccess_frame(mbus_frame *frame);
void send_mbus_frame(const char* frame_json);
char *gen_xml(mbus_frame_data, int normalized);
void generate_random_hex_data(unsigned char *buffer, size_t length);
#endif
