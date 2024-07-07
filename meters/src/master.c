#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mbus/mbus.h>

int communicate_with_slave(const char *port, int address) {
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
        return (-1);
    }

    while (1) {
        // Create and send a request frame to the slave
        memset(&frame, 0, sizeof(mbus_frame));
        frame.type = MBUS_FRAME_TYPE_SHORT;
        frame.start1 = MBUS_FRAME_SHORT_START;
        frame.control = MBUS_CONTROL_MASK_REQ_UD1;
        frame.address = address;
        frame.stop = MBUS_FRAME_STOP;
        frame.checksum = mbus_frame_calc_checksum(&frame);

        if (mbus_send_frame(handle, &frame) == -1) {
            fprintf(stderr, "Failed to send request frame.\n");
            mbus_disconnect(handle);
            mbus_context_free(handle);
            return (-1);
        }

        // Wait for the reply
        if (mbus_recv_frame(handle, &reply) == -1) {
            fprintf(stderr, "Failed to receive reply frame.\n");
            // Continue to the next iteration to try again
            continue;
        }

        printf("Received reply from slave:\n");
        mbus_frame_print(&reply);

        // Sleep for a short period before sending the next frame
        usleep(500000);  // 500 ms
    }

    mbus_disconnect(handle);
    mbus_context_free(handle);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <port> <address>\n", argv[0]);
        return 1;
    }

    const char *port = argv[1];
    int address = atoi(argv[2]);


    communicate_with_slave(port, address);

    return 0;
}
