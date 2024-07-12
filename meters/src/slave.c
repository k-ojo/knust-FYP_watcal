#include "../include/main.h"

/**
* simulate_slave- handles the slave meter on mbus protocol
*
* @port: the device
* @address: the address of slave
* Return: 0 on success, -1 on fail
*/
int simulate_slave(const char *port, int address) {
    mbus_handle *handle;
    mbus_frame frame;
    mbus_frame reply;
    mbus_frame_data *frame_data;

    unsigned char raw_buff[] = {
        0x68, 0x92, 0x92, 0x68, 0x08, 0x01, 0x72, 0x3E, 0x02, 0x00, 0x05, 0x43, 0x4C, 0x12, 0x02, 0x13,
        0x00, 0x00, 0x00, 0x8C, 0x10, 0x04, 0x52, 0x12, 0x00, 0x00, 0x8C, 0x11, 0x04, 0x52, 0x12, 0x00,
        0x00, 0x8C, 0x20, 0x04, 0x33, 0x44, 0x77, 0x01, 0x8C, 0x21, 0x04, 0x33, 0x44, 0x77, 0x01, 0x02,
        0xFD, 0xC9, 0xFF, 0x01, 0xED, 0x00, 0x02, 0xFD, 0xDB, 0xFF, 0x01, 0x20, 0x00, 0x02, 0xAC, 0xFF,
        0x01, 0x4F, 0x00, 0x82, 0x40, 0xAC, 0xFF, 0x01, 0xEE, 0xFF, 0x02, 0xFD, 0xC9, 0xFF, 0x02, 0xE7,
        0x00, 0x02, 0xFD, 0xDB, 0xFF, 0x02, 0x23, 0x00, 0x02, 0xAC, 0xFF, 0x02, 0x51, 0x00, 0x82, 0x40,
        0xAC, 0xFF, 0x02, 0xF1, 0xFF, 0x02, 0xFD, 0xC9, 0xFF, 0x03, 0xE4, 0x00, 0x02, 0xFD, 0xDB, 0xFF,
        0x03, 0x45, 0x00, 0x02, 0xAC, 0xFF, 0x03, 0xA0, 0x00, 0x82, 0x40, 0xAC, 0xFF, 0x03, 0xE0, 0xFF,
        0x02, 0xFF, 0x68, 0x00, 0x00, 0x02, 0xAC, 0xFF, 0x00, 0x40, 0x01, 0x82, 0x40, 0xAC, 0xFF, 0x00,
        0xBF, 0xFF, 0x01, 0xFF, 0x13, 0x04, 0xD9, 0x16
    };

   //generate random data
   //generate_random_hex_data(raw_buff, sizeof(raw_buff));


    handle = mbus_context_serial(port);
    if (handle == NULL) {
        fprintf(stderr, "Failed to initialize M-Bus context.\n");
        return (-1);
    }

    if (mbus_connect(handle) == -1) {
        fprintf(stderr, "Failed to connect to M-Bus device.\n");
        mbus_context_free(handle);
        return(-1);
    }

    while (1) {
        memset(&frame, 0, sizeof(mbus_frame));
        if (mbus_recv_frame(handle, &frame) == -1) {
            fprintf(stderr, "Failed to receive M-Bus frame.\n");
            continue;
        }

        // Check if the frame is a ping frame or a request frame to the specified address
        if (frame.address == address && (frame.control == MBUS_CONTROL_MASK_SND_NKE ||
		frame.control == MBUS_CONTROL_MASK_REQ_UD1 ||
		frame.control == MBUS_CONTROL_MASK_REQ_UD2)) {
            printf("Request received from master\n");
	    frame_data = _parse_mbus_frame(&reply, raw_buff, sizeof(raw_buff));
            watCal_set_frame(&reply, address);
            mbus_frame_print(&reply);

            if (mbus_send_frame(handle, &reply) == -1) {
                fprintf(stderr, "Failed to send data frame.\n");
            }
        }


        usleep(100000);
    }

    mbus_disconnect(handle);
    mbus_context_free(handle);
}


/**
* main- main program
* @ac- arguments count
* @av- argument vector
*/
int main(int ac, char *av[]) {
    if (ac < 3) {
        fprintf(stderr, "Usage: %s <port> <address>\n", av[0]);
        return 1;
    }

    const char *port = av[1];
    int address = atoi(av[2]);

    simulate_slave(port, address);

    return 0;
}

