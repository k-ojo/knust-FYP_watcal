#include <stdio.h>
#include <string.h>
#include <mbus/mbus.h>

void send_data_request(int address) {
    mbus_frame frame;
    memset(&frame, 0, sizeof(mbus_frame));
    frame.start = MBUS_FRAME_LONG_START;
    frame.control = MBUS_CONTROL_MASK_REQ_UD2;
    frame.address = address;
    frame.checksum = 0x55; // Example checksum
    frame.stop = MBUS_FRAME_STOP;
    mbus_send_frame(MBUS_HANDLE, &frame);
}

void send_relay_control(int address) {
    mbus_frame frame;
    memset(&frame, 0, sizeof(mbus_frame));
    frame.start = MBUS_FRAME_LONG_START;
    frame.control = MBUS_CONTROL_MASK_REQ_UD1;
    frame.address = address;
    frame.checksum = 0x55; // Example checksum
    frame.stop = MBUS_FRAME_STOP;
    mbus_send_frame(MBUS_HANDLE, &frame);
}

int main() {
    // Initialize M-Bus master

    // socat setup to simulate serial port
    // socat -d -d PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1

    // Initialize master on /tmp/virtualcom0
    mbus_init_master("/tmp/virtualcom0");

    // Send a data request to the slave
    send_data_request(1);

    // Send a relay control command to the slave
    send_relay_control(1);

    // Cleanup
    mbus_cleanup();
    return 0;
}
