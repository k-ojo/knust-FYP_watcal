#include <stdio.h>
#include <stdlib.h>
#include <mbus/mbus.h>

int main() {
    mbus_handle *handle;
    mbus_frame frame;
    mbus_frame reply;
    int address = 1;  // Assuming slave address is 1

    // Initialize M-Bus context for serial communication
    handle = mbus_context_serial("/tmp/virtualcom0");
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

    // Send a request frame to the slave
    if (mbus_send_request_frame(handle, address) == -1) {
        fprintf(stderr, "Failed to send request frame to the slave.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return -1;
    }

    printf("Request frame sent to slave.\n");

    // Wait for a response from the slave
    if (mbus_recv_frame(handle, &reply) == -1) {
        fprintf(stderr, "Failed to receive reply from the slave.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return -1;
    }

    printf("Received response from slave.\n");

    // Process the response here (e.g., print the frame content)
    mbus_frame_print(&reply);

    // Disconnect and free the M-Bus context
    mbus_disconnect(handle);
    mbus_context_free(handle);
    return 0;
}
