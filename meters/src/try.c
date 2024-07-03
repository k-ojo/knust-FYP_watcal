#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <mbus/mbus.h>

#define MASTER "/dev/pts/9" // Adjust this as necessary

typedef enum {
    RELAY_COMMAND,
    DATA_REQUEST
} FrameType;

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

/**
 * send_request - send M-Bus request frame
 * @handle: M-Bus handle
 * @address: address of the slave
 */
void send_request(mbus_handle *handle, uint8_t address)
{
    mbus_frame request;
    memset(&request, 0, sizeof(mbus_frame));
    
    request.start1 = MBUS_FRAME_ACK_START;
    request.control = MBUS_CONTROL_MASK_REQ_UD2;
    request.address = address;
    request.control_information = MBUS_CONTROL_INFO_DATA_SEND;
    request.data_size = 1;
    request.data[0] = 0x16; // Sample request data
    request.stop = MBUS_FRAME_STOP;

    if (mbus_send_frame(handle, &request) == -1)
    {
        fprintf(stderr, "Failed to send request frame: %s\n", mbus_error_str());
    }
    else
    {
        printf("Sent request frame to address 0x%x\n", address);
    }
}

/**
 * process_response - process incoming M-Bus frame
 * @handle: M-Bus handle
 * @frame: pointer to the M-Bus frame
 */
void process_response(mbus_handle *handle, mbus_frame *frame)
{
    // Process the incoming frame
    printf("Received frame from slave\n");
    printf("Frame address: 0x%x\n", frame->address);
    printf("Frame data: 0x%x\n", frame->data[0]);
}

int main()
{
    // Open the master serial port
    int m_fd = open(MASTER, O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_fd == -1) {
        perror("Failed to open master port");
        return -1;
    }

    // Configure the serial port
    configure_serial_port(m_fd);

    // Initialize M-Bus context
    mbus_handle *handle = mbus_context_serial(MASTER);
    if (handle == NULL) {
        perror("Failed to initialize M-Bus context");
        close(m_fd);
        return -1;
    }

    // Send request to the slave
    send_request(handle, 0x01); // Adjust the slave address as necessary

    // Receive and process response
    mbus_frame frame;
    if (mbus_recv_frame(handle, &frame) == -1)
    {
        fprintf(stderr, "Failed to receive frame: %s\n", mbus_error_str());
    }
    else
    {
        process_response(handle, &frame);
    }

    // Clean up
    mbus_context_free(handle);
    close(m_fd);
    return 0;
}
