#include "../include/main.h"

int main() {
    int fd = open(SLAVE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    configure_serial_port(fd);

    mbus_frame reply;
    mbus_frame reply_data;
    memset(&reply, 0, sizeof(mbus_frame));
    memset(&reply_data, 0, sizeof(mbus_frame));

    while (1) {
        char buffer[256];
        int n = read(fd, buffer, sizeof(buffer));
        if (n > 0) {
            // Process the received request and send a reply
            reply_data.start1 = 0x68;
            reply_data.length1 = 3;
            reply_data.length2 = 3;
            reply_data.start2 = 0x68;
            reply_data.control = 0x08; // Single character
            reply_data.address = 0x78; // Example address
            reply_data.control_information = 0x72; // Data send
            reply_data.data[0] = 0xFF;
            reply_data.data[1] = 0xFF;
            reply_data.data[2] = 0x16; // End character
            reply_data.checksum = 0x16; // Example checksum
            reply_data.stop = 0x16; // Stop character

            write(fd, &reply_data, sizeof(reply_data));
        }
        usleep(100000); // Sleep for 100ms
    }

    close(fd);
    return 0;
}
