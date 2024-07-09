#include "../include/main.h"

int watCal_set_frame(mbus_frame *reply, int address)
{
            memset(reply, 0, sizeof(mbus_frame));
            reply->type = MBUS_FRAME_TYPE_LONG;
            reply->start1 = MBUS_FRAME_LONG_START;
            reply->control = MBUS_CONTROL_MASK_RSP_UD;
            reply->address = address;
            reply->control_information = 0x72;  // Data send (SND_UD)

            // Create example data to send back
            reply->data_size = 5;
            reply->data[0] = 0x01;  // Example data: DIF
            reply->data[1] = rand() % 256;  // Example data: random value
            reply->data[2] = rand() % 256;  // Example data: random value
            reply->data[3] = rand() % 256;  // Example data: random value
            reply->data[4] = rand() % 256;  // Example data: random value

            reply->checksum = mbus_frame_calc_checksum(reply);
            reply->stop = MBUS_FRAME_STOP;
            return (1);

}
