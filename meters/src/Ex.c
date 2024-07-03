#include "../include/main.h"


/**
 * process_request - process incoming M-Bus frame
 * @handle: M-Bus handle
 * @frame: pointer to the M-Bus frame
 */
void process_request(mbus_handle *handle, mbus_frame *frame)
{
    // Process the incoming frame
    printf("Received frame from master\n");
    printf("Frame address: 0x%x\n", frame->address);

    // Create a response frame
    mbus_frame reply;
    memset(&reply, 0, sizeof(mbus_frame));
    
    reply.start1 = MBUS_FRAME_ACK_START;
    reply.control = MBUS_CONTROL_MASK_RSP_UD | MBUS_CONTROL_MASK_DIR_S2M;
    reply.address = frame->address;
    reply.control_information = MBUS_CONTROL_INFO_DATA_SEND;
    reply.data_size = 1;
    reply.data[0] = 0x06; // Sample response data
    reply.stop = MBUS_FRAME_STOP;

    // Send the response frame
    if (mbus_send_frame(handle, &reply) == -1)
    {
        fprintf(stderr, "Failed to send response frame: %s\n", mbus_error_str());
    }
    else
    {
        printf("Sent response frame to address 0x%x\n", frame->address);
    }
}

int main()
{
    // Open the slave serial port
    int s_fd = open(SLAVE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (s_fd == -1) {
        perror("Failed to open slave port");
        return -1;
    }

    // Configure the serial port
    configure_serial_port(s_fd);

    // Initialize M-Bus context
    mbus_handle *handle = mbus_context_serial(SLAVE);
    if (handle == NULL) {
        perror("Failed to initialize M-Bus context");
        close(s_fd);
        return -1;
    }

    // Main loop to receive and process frames
    while (1)
    {
        mbus_frame frame;
        if (mbus_recv_frame(handle, &frame) == -1)
        {
            fprintf(stderr, "Failed to receive frame: %s\n", mbus_error_str());
        }
        else
        {
            process_request(handle, &frame);
        }
    }

    // Clean up
    mbus_context_free(handle);
    close(s_fd);
    return 0;
}
