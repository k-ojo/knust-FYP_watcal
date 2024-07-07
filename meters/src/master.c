#include "../include/main.h"


void read_data(mbus_handle *handle) {
    mbus_frame reply;
    mbus_frame_data reply_data;

    memset(&reply, 0, sizeof(mbus_frame));
    memset(&reply_data, 0, sizeof(mbus_frame_data));

    if (mbus_send_ping_frame(handle, 9, 1) == -1) {
        fprintf(stderr, "Failed to send M-Bus ping frame.\n");
        return;
    }

    if (mbus_recv_frame(handle, &reply) == -1) {
        fprintf(stderr, "Failed to receive M-Bus reply frame.\n");
        return;
    }

    // Print the received M-Bus frame
    mbus_frame_print(&reply);

    if (mbus_frame_data_parse(&reply, &reply_data) == -1) {
        fprintf(stderr, "Failed to parse M-Bus reply frame.\n");
        return;
    }

    // Ensure the data record is valid and has at least one record
    if (reply_data.data_var.record[0].data_len > 0) {
        int value = reply_data.data_var.record[0].data[0];
        char postdata[100];
        snprintf(postdata, sizeof(postdata), "value=%d", value);

        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/api/mbus/data");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
            res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
        }
    } else {
        fprintf(stderr, "No data records found.\n");
    }
}

int main(int argc, char *argv[]) {
    char *device = "/dev/pts/5";
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
        read_data(handle);
        sleep(5);
    }

    mbus_disconnect(handle);
    mbus_context_free(handle);

    return 0;
}
