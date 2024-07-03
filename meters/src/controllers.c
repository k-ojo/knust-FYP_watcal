#include "../include/main.h"


/**
 * initialize_frame - creates frame
 * @frame: pointer to the M-Bus frame
 * @type: type of the frame
 * @address: address of the slave
 */
void initialize_frame(mbus_frame *frame, FrameType type, uint8_t address)
{
    memset(frame, 0, sizeof(mbus_frame));

    if (type == RELAY_COMMAND)
    {
        frame->start1 = MBUS_FRAME_ACK_START;
        frame->control = MBUS_CONTROL_MASK_SND_UD;
        frame->address = address;
        frame->control_information = MBUS_CONTROL_INFO_DATA_SEND | MBUS_CONTROL_MASK_DIR_M2S;
        frame->data_size = 1;
        frame->data[0] = 0x05;
        frame->stop = MBUS_FRAME_STOP;
        printf("Initialized frame for RELAY_COMMAND\n");
    }
    else if (type == DATA_REQUEST)
    {
        frame->start1 = MBUS_FRAME_ACK_START;
        frame->control = MBUS_CONTROL_MASK_REQ_UD2 | MBUS_CONTROL_MASK_DIR_M2S;
        frame->address = address;
        frame->control_information = MBUS_CONTROL_INFO_DATA_SEND;
        frame->data_size = 1;
        frame->data[0] = 0x05;
        frame->stop = MBUS_FRAME_STOP;
        printf("Initialized frame for DATA_REQUEST\n");
    }
}

/**
* initialize_frame- creates frame
*
* Return- nothing
*/
void initialize_frame(mbus_frame *frame, FrameType type, uint8_t address)
{
	memset(frame, 0, sizeof(mbus_frame));

	if (type == RELAY_COMMAND)
	{
		frame->start1 = MBUS_FRAME_ACK_START;
		frame->control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_DIR_M2S;
		frame->address = address;
		frame->control_information = MBUS_CONTROL_INFO_DATA_SEND;
		frame->data_size = 1;
		frame->data[0] = 0x05;
		frame->stop = MBUS_FRAME_STOP;
	}
	else if (type == DATA_REQUEST)
    {
        frame->start1 = MBUS_FRAME_ACK_START;
        frame->control = MBUS_CONTROL_MASK_REQ_UD2 | MBUS_CONTROL_MASK_DIR_M2S;
        frame->address = address;
        frame->control_information = MBUS_CONTROL_INFO_DATA_SEND ;
        frame->data_size = 1;
        frame->data[0] = 0x05;
		frame->stop = MBUS_FRAME_STOP;
    }

}
/**
* send_relay_command- sends command to specified slave to master
* @fd- slave device descriptor
* @address- address of slave device
* @command- turn off or turn on relay command
* Return- nothing
*/
int send_request(mbus_handle *handle, uint8_t address, FrameType type)
{
	mbus_frame frame;
	initialize_frame(&frame, type, address);

	//sending frame
	if (mbus_send_frame(handle, &frame) == -1)
	{
		fprintf(stderr, "Failed to send frame\n");
		return (-1);
	}
	mbus_context_free(handle);
	return (0);
}

/**
* configure_serial_port- configuration for mbus protocol
* @fd: input file descriptor
* Return: nothing
*/
void configure_serial_port(int fd)
{
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B2400);
    cfsetospeed(&options, B2400);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;

	#ifdef CRTSCTS
    	options.c_cflag &= ~CRTSCTS;
	#endif
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    tcsetattr(fd, TCSANOW, &options);

}

void initialize_slave(mbus_handle *handle, uint8_t)
{
}