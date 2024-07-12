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

void watCal_proccess_frame(mbus_frame *frame)
{
	mbus_frame_data frame_data;
}


void send_mbus_frame(const char *frame_json) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/mbus-frame");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, frame_json);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
