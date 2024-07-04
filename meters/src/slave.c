#include <stdio.h>
#include <mbus/mbus.h>

int main() {
    mbus_handle *handle;

    handle = mbus_context_serial("/tmp/virtualcom1");
    if (handle == NULL) {
        fprintf(stderr, "Failed to initialize M-Bus context.\n");
        return -1;
    }

    if (mbus_connect(handle) == -1) {
        fprintf(stderr, "Failed to connect to M-Bus context.\n");
        mbus_context_free(handle);
        return -1;
    }

    // Implement logic to receive and process frames from the master

    mbus_disconnect(handle);
    mbus_context_free(handle);
    return 0;
}
