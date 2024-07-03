#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <mbus/mbus.h>

#define DEVICE "/dev/pts/2" // Change to the master virtual port


/**
* configure_serial_port- configuration for mbus protocol
* @fd: input file decripter
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
    options.c_cflag &= ~CRTSCTS;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    tcsetattr(fd, TCSANOW, &options);
}

int main() {
    int fd = open(DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("Could not open");
        return 1;
    }

    configure_serial_port(fd);

    mbus_frame request;
    memset(&request, 0, sizeof(mbus_frame));
    request.start1 = 0x10;
    request.control = 0x5B; // Single character
    request.address = 0x78; // Example address
    request.checksum = 0x5B; // Example checksum
    request.stop = 0x16; // Stop character

    while (1) {
        write(fd, &request, sizeof(request));
        usleep(100000); // Sleep for 100ms

        char buffer[256];
        int n = read(fd, buffer, sizeof(buffer));
        if (n > 0) {
            buffer[n] = '\0';
            printf("Received data: %s\n", buffer);
        }
        sleep(1);
    }

    close(fd);
    return 0;
}
