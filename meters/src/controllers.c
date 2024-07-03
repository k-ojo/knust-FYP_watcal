#include "../include/main.h"

/**
* send_relay_command- sends command to specified slave to master
* @fd- slave device descriptor
* @address- address of slave device
* @command- turn off or turn on relay command
* Return- nothing
*/
int send_relay_command(int fd, uint8_t address, uint8_t command)
{
	mbus_handle *handle;
	mbus_frame request;

	memset(&request, 0, sizeof(mbus_frame));

	handle = mbus_context_serial(SLAVE);
	handle->fd = fd;
	request.start1 = MBUS_FRAME_ACK_START;
	request.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_DIR_M2S;
	request.address = address; //address of slave meter
	request.data_size = 1; //8 bit command
	request.data[0] = command;
	request.stop = MBUS_FRAME_STOP;

	//sending frame
	if (mbus_send_frame(handle, &request) == -1)
	{
		fprintf(stderr, "Failed to send frame\n");
		return (-1);
	}
	return (0);
}

/**
* configure_serial_port- configuration for mbus protocol
* @fd: input file descriptor
* Return: nothing
*/
void configure_serial_port(int fd) {
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