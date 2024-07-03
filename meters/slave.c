#include <stdio.h>
#include <string.h>
#include <mbus/mbus.h>

void handle_request(mbus_frame *frame) {
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
        mbus_send_frame(MBUS_HANDLE, &reply);
    } else if (frame->control == MBUS_CONTROL_MASK_REQ_UD1) {
        // Example response for relay control
        printf("Received relay control command\n");
        // Here you would handle the relay control
    }
}

int main() {
    // Initialize M-Bus slave
    mbus_frame frame;

    // socat setup to simulate serial port
    // socat -d -d PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1

    // Initialize slave on /tmp/virtualcom1
    mbus_init_slave("/tmp/virtualcom1");

    while (1) {
        // Wait for a frame from the master
        mbus_recv_frame(MBUS_HANDLE, &frame);
        // Handle the received frame
        handle_request(&frame);
    }

    // Cleanup
    mbus_cleanup();
    return 0;
}
