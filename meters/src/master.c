#include "../include/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <mbus/mbus.h>

#define MASTER "/dev/pts/3" // Adjust this as necessary

typedef enum {
    RELAY_COMMAND,
    DATA_REQUEST
} FrameType;

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
 * send_request - sends command to specified slave
 * @handle: M-Bus handle
 * @address: address of the slave device
 * @type: type of the frame
 * Return: 0 on success, -1 on failure
 */
int send_request(mbus_handle *handle, uint8_t address, FrameType type)
{
    mbus_frame frame;
    initialize_frame(&frame, type, address);

    // sending frame
    if (mbus_send_frame(handle, &frame) == -1)
    {
        fprintf(stderr, "Failed to send frame: %s\n", mbus_error_str());
        return -1;
    }

    printf("Sent frame to address 0x%x\n", address);
    return 0;
}

/**
 * configure_serial_port - configuration for mbus protocol
 * @fd: input file descriptor
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

int main()
{
    int m_fd = open(MASTER, O_RDWR | O_NOCTTY | O_NDELAY);
    mbus_handle *handle;

    handle = mbus_context_serial(MASTER);
    if (handle == NULL)
    {
        perror("failed to initialize");
        return (-1);
    };  //initializes right parameters for mbus protocol

    int slave_addr = 0x51;

    while (1) {
        send_request(handle, slave_addr, RELAY_COMMAND);
    }

    close(m_fd);
    return 0;
}
