#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mbus/mbus.h>

void send_data(mbus_handle *handle) {
    mbus_frame reply;
    mbus_frame_data reply_data;

    memset(&reply, 0, sizeof(mbus_frame));
    memset(&reply_data, 0, sizeof(mbus_frame_data));

    // Initialize the M-Bus frame data
    reply_data.type = MBUS_DATA_TYPE_VARIABLE;

    // Ensure that we have space for at least one record
    reply_data.data_var.record = calloc(1, sizeof(mbus_record));
    if (reply_data.data_var.record == NULL) {
        fprintf(stderr, "Failed to allocate memory for data record.\n");
        return;
    }

    reply_data.data_var.nrecords = 1;  // Number of records
    reply_data.data_var.record[0].drh.dib.dif = 0x04;  // DIF: 4-byte integer
    reply_data.data_var.record[0].data_len = 4;  // Data length
    reply_data.data_var.record[0].data[0] = rand() % 256;
    reply_data.data_var.record[0].data[1] = rand() % 256;
    reply_data.data_var.record[0].data[2] = rand() % 256;
    reply_data.data_var.record[0].data[3] = rand() % 256;

    // Set up the M-Bus frame
    reply.data = reply_data;

    // Send the M-Bus frame
    if (mbus_send_frame(handle, &reply) == -1) {
        fprintf(stderr, "Failed to send frame.\n");
    }

    free(reply_data.data_var.record);  // Free the allocated memory
}

int main(int argc, char *argv[]) {
    char *device = "/dev/pts/4";
    mbus_handle *handle = mbus_context_serial(device);

    if (handle == NULL) {
        fprintf(stderr, "Failed to initialize M-Bus context.\n");
        return 1;
    }

    if (mbus_connect(handle) == -1) {
        fprintf(stderr, "Failed to connect to M-Bus device.\n");
        mbus_context_free(handle);
        return 1;
    }

    while (1) {
        mbus_frame frame;
        memset(&frame, 0, sizeof(mbus_frame));

        if (mbus_recv_frame(handle, &frame) != -1) {
            send_data(handle);
        }
        usleep(100000);
    }

    mbus_disconnect(handle);
    mbus_context_free(handle);
    return 0;
}
