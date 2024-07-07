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
