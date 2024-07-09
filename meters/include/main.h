#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <mbus/mbus.h>

#define SLAVE "/dev/pts/8" // Change to the slave virtual port
#define MASTER "/dev/pts/2" // Change to master port

//#define R_COMMAND 0x53;
//#define D_REQUEST 0x52;

typedef enum
{	RELAY_COMMAND,
	DATA_REQUEST
}FrameType;

/**
* watcal_respond - slave action
* @type- type, whether control or not
*/
typedef struct _watCal_respond
{
	int type;
}watCal_respond;

int watCal_set_frame(mbus_frame *, int);
void watCal_proccess_frame();
#endif
