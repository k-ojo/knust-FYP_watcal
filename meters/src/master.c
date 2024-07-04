#include <stdio.h>
#include <mbus/mbus.h>
int main() {
    mbus_handle *handle;

    handle = mbus_context_serial("/tmp/virtualcom0");
    if (handle == NULL) {
        fprintf(stderr, "Failed to initialize M-Bus context.\n");
        return -1;
    }
    mbus_connect(handle);

    if (mbus_connect(handle) == -1) {
        fprintf(stderr, "Failed to connect to M-Bus context.\n");
        mbus_context_free(handle);
        return -1;
    }

    // Send a frame to the slave
    mbus_frame frame;
    mbus_frame_init(&frame);
    // Populate frame with appropriate data

    if (mbus_send_frame(handle, &frame) == -1) {
        perror("mbus_send_frame");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return -1;
    }

    mbus_disconnect(handle);
    mbus_context_free(handle);
    return 0;
}
