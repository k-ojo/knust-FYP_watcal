#include <stdio.h>
#include <stdlib.h>
#include <mbus/mbus.h>

int main() {
    mbus_handle *handle;
    mbus_frame frame;
    mbus_frame reply;
    mbus_frame_data frame_data;
    int address = 1;  // Slave address

    // Initialize M-Bus context for serial communication
    handle = mbus_context_serial("/tmp/virtualcom1");
    if (handle == NULL) {
        fprintf(stderr, "Failed to initialize M-Bus context.\n");
        return -1;
    }

    // Connect to the M-Bus context
    if (mbus_connect(handle) == -1) {
        fprintf(stderr, "Failed to connect to M-Bus context.\n");
        mbus_context_free(handle);
        return -1;
    }

    // Wait for a request frame from the master
    if (mbus_recv_frame(handle, &frame) == -1) {
        fprintf(stderr, "Failed to receive request frame from master.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return -1;
    }

    printf("Received request frame from master.\n");

    // Process the request and prepare a reply frame
    mbus_frame_data_parse(&frame, &frame_data);

    // Create a simple response frame
    mbus_frame_init(&reply);
    reply.start = MBUS_FRAME_LONG_START;
    reply.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_DIR;
    reply.address = address;
    reply.control_information = 0x08; // example control information
    reply.data = (unsigned char*) "example data";
    reply.data_size = strlen((char*)reply.data);
    mbus_frame_calc_checksum(&reply);

    // Send the reply frame to the master
    if (mbus_send_frame(handle, &reply) == -1) {
        fprintf(stderr, "Failed to send reply frame to master.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return -1;
    }

    printf("Sent response frame to master.\n");

    // Disconnect and free the M-Bus context
    mbus_disconnect(handle);
    mbus_context_free(handle);
    return 0;
}
