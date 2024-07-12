#include "../include/main.h"
int communicate_with_slave(const char *port, int address) {
    mbus_handle *handle;
    mbus_frame frame;
    mbus_frame reply;
    mbus_frame_data data;

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
        //send request frame
	mbus_send_request_frame(handle, address);

	// Wait for the reply
        if (mbus_recv_frame(handle, &reply) == -1) {
            fprintf(stderr, "Failed to receive reply frame.\n");
            // Continue to the next iteration to try again
            continue;
        }

        printf("Received reply from slave:\n");
        mbus_frame_print(&reply);
	if (mbus_frame_data_parse(&reply, &data))
	{
    	    fprintf(stderr, "Failed to parse MBus data: %s\n", mbus_error_str()); // Handle parsing error
	}
	mbus_frame_data_print(&data);


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

      // Example M-Bus frame data in JSON format
    const char *frame_json = "{\"frameType\":\"short\",\"address\":\"1234\",\"controlField\":\"01\",\"data\":\"ABCDEF\",\"timestamp\":\"2024-07-12T12:00:00Z\",\"checksum\":\"89\",\"length\":10}";


    const char *port = argv[1];
    int address = atoi(argv[2]);


    communicate_with_slave(port, address);

    return 0;
}
