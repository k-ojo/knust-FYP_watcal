#include "../include/main.h"

int main() {
    int fd = open(MASTER, O_RDWR | O_NOCTTY | O_NDELAY);
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
