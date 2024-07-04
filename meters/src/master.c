#include <stdio.h>
#include <string.h>
#include <mbus/mbus.h>

void send_data_request(mbus_handle *handle, int address) {
    mbus_frame frame;
    memset(&frame, 0, sizeof(mbus_frame));
    frame.start1 = MBUS_FRAME_LONG_START;
    frame.control = MBUS_CONTROL_MASK_REQ_UD2;
    frame.address = address;
    frame.checksum = 0x55; // Example checksum
    frame.stop = MBUS_FRAME_STOP;
    mbus_send_frame(handle, &frame);
}

void send_relay_control(mbus_handle *handle, int address) {
    mbus_frame frame;
    memset(&frame, 0, sizeof(mbus_frame));
    frame.start1 = MBUS_FRAME_LONG_START;
    frame.control = MBUS_CONTROL_MASK_REQ_UD1;
    frame.address = address;
    frame.checksum = 0x55; // Example checksum
    frame.stop = MBUS_FRAME_STOP;
    mbus_send_frame(handle, &frame);
}

int main() {
    // Initialize M-Bus master
    mbus_handle *handle;

    // socat setup to simulate serial port
    // socat -d -d PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1

    // Initialize master on /tmp/virtualcom0
    handle = mbus_context_serial("/tmp/virtualcom0");

    if (handle == NULL) {
        fprintf(stderr, "Failed to initialize M-Bus master\n");
        return 1;
    }
    mbus_connect(handle);

    // Send a data request to the slave
    printf("break\n");
    send_data_request(handle, 1);
    printf("break\n");
    // Send a relay control command to the slave
    send_relay_control(handle, 1);

    // Cleanup
    mbus_disconnect(handle);
    return 0;
}
