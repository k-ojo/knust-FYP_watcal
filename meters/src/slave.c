#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mbus/mbus.h>

void simulate_slave(const char *port, int address) {
    mbus_handle *handle;
    mbus_frame frame;
    mbus_frame reply;

    handle = mbus_context_serial(port);
    if (handle == NULL) {
        fprintf(stderr, "Failed to initialize M-Bus context.\n");
        return;
    }

    if (mbus_connect(handle) == -1) {
        fprintf(stderr, "Failed to connect to M-Bus device.\n");
        mbus_context_free(handle);
        return;
    }

    while (1) {
        memset(&frame, 0, sizeof(mbus_frame));
        if (mbus_recv_frame(handle, &frame) == -1) {
            fprintf(stderr, "Failed to receive M-Bus frame.\n");
            continue;
        }

        // Print the received frame
        mbus_frame_print(&frame);

        // Check if the frame is a ping frame or a request frame to the specified address
        if (frame.address == address && (frame.control == MBUS_CONTROL_MASK_SND_NKE || frame.control == MBUS_CONTROL_MASK_REQ_UD1 || frame.control == MBUS_CONTROL_MASK_REQ_UD2)) {
            printf("Request received from master\n");

            // Respond with a data frame
            memset(&reply, 0, sizeof(mbus_frame));
            reply.start = MBUS_FRAME_LONG_START;
            reply.control = MBUS_CONTROL_MASK_RSP_UD;
            reply.address = address;
            reply.control_information = 0x72;  // Data send (SND_UD)

            // Create example data to send back
            reply.data_size = 5;
            reply.data[0] = 0x01;  // Example data: DIF
            reply.data[1] = rand() % 256;  // Example data: random value
            reply.data[2] = rand() % 256;  // Example data: random value
            reply.data[3] = rand() % 256;  // Example data: random value
            reply.data[4] = rand() % 256;  // Example data: random value

            reply.checksum = mbus_calc_checksum(&reply);
            reply.stop = MBUS_FRAME_STOP;

            if (mbus_send_frame(handle, &reply) == -1) {
                fprintf(stderr, "Failed to send data frame.\n");
            }
        }
        usleep(100000);
    }

    mbus_disconnect(handle);
    mbus_context_free(handle);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <port> <address>\n", argv[0]);
        return 1;
    }

    const char *port = argv[1];
    int address = atoi(argv[2]);

    simulate_slave(port, address);

    return 0;
}
