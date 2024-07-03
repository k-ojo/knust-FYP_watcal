#include <stdio.h>
#include <string.h>
#include <mbus/mbus.h>

void handle_request(mbus_handle *handle, mbus_frame *frame) {
    // Check the command and respond appropriately
    if (frame->control == MBUS_CONTROL_MASK_REQ_UD2) {
        // Example response for data request
        printf("Received data request\n");
        // Send back a fixed response
        mbus_frame reply;
        memset(&reply, 0, sizeof(mbus_frame));
        reply.start = MBUS_FRAME_LONG_START;
        reply.control = MBUS_CONTROL_MASK_SND_UD;
        reply.address = frame->address;
        reply.checksum = 0x55; // Example checksum
        reply.stop = MBUS_FRAME_STOP;
        // Send the response
        mbus_send_frame(handle, &reply);
    } else if (frame->control == MBUS_CONTROL_MASK_REQ_UD1) {
        // Example response for relay control
        printf("Received relay control command\n");
        // Here you would handle the relay control
    }
}

int main() {
    // Initialize M-Bus slave
    mbus_handle *handle;
    mbus_frame frame;

    // socat setup to simulate serial port
    // socat -d -d PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1

    // Initialize slave on /tmp/virtualcom1
    handle = mbus_connect_serial("/tmp/virtualcom1");
    if (handle == NULL) {
        fprintf(stderr, "Failed to initialize M-Bus slave\n");
        return 1;
    }

    while (1) {
        // Wait for a frame from the master
        if (mbus_recv_frame(handle, &frame) == -1) {
            fprintf(stderr, "Failed to receive frame\n");
            continue;
        }
        // Handle the received frame
        handle_request(handle, &frame);
    }

    // Cleanup
    mbus_disconnect(handle);
    return 0;
}
