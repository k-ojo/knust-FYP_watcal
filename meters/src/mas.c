#include "../include/main.h"

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

    // Set slave address
    int slave_addr = 0x51;

    while (1) {
        // Send request to slave
        if
