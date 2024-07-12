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


/**
* mbus_frame_to_json- changes mbus frame to json
* @frame: frame to be converted
* Return: json string
*/
// Function to generate JSON representation of M-Bus variable data
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//------------------------------------------------------------------------------
//
// JSON RELATED FUNCTIONS
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
///
/// Encode string to JSON
///
//------------------------------------------------------------------------------
int
mbus_str_json_encode(char *dst, const unsigned char *src, size_t max_len)
{
    size_t i, len;

    i = 0;
    len = 0;

    if (dst == NULL)
    {
        return -1;
    }

    if (src == NULL)
    {
        dst[len] = '\0';
        return -2;
    }

    while((len+6) < max_len)
    {
        if (src[i] == '\0')
        {
            break;
        }

        if (iscntrl(src[i]))
        {
            // convert all control chars into spaces
            dst[len++] = ' ';
        }
        else
        {
            switch (src[i])
            {
                case '\\':
                    len += snprintf(&dst[len], max_len - len, "\\\\");
                    break;
                case '\"':
                    len += snprintf(&dst[len], max_len - len, "\\\"");
                    break;
                case '\b':
                    len += snprintf(&dst[len], max_len - len, "\\b");
                    break;
                case '\f':
                    len += snprintf(&dst[len], max_len - len, "\\f");
                    break;
                case '\n':
                    len += snprintf(&dst[len], max_len - len, "\\n");
                    break;
                case '\r':
                    len += snprintf(&dst[len], max_len - len, "\\r");
                    break;
                case '\t':
                    len += snprintf(&dst[len], max_len - len, "\\t");
                    break;
                default:
                    dst[len++] = src[i];
                    break;
            }
        }

        i++;
    }

    dst[len] = '\0';
    return 0;
}

char *
mbus_data_variable_header_json(mbus_data_variable_header *header)
{
    static char buff[8192];
    char str_encoded[768];
    size_t len = 0;

    if (header)
    {
        len += snprintf(&buff[len], sizeof(buff) - len, "    {\n");

        len += snprintf(&buff[len], sizeof(buff) - len, "        \"Id\": \"%llX\",\n", mbus_data_bcd_decode_hex(header->id_bcd, 4));
        len += snprintf(&buff[len], sizeof(buff) - len, "        \"Manufacturer\": \"%s\",\n",
                mbus_decode_manufacturer(header->manufacturer[0], header->manufacturer[1]));
        len += snprintf(&buff[len], sizeof(buff) - len, "        \"Version\": %d,\n", header->version);

        mbus_str_json_encode(str_encoded, mbus_data_product_name(header), sizeof(str_encoded));
        len += snprintf(&buff[len], sizeof(buff) - len, "        \"ProductName\": \"%s\",\n", str_encoded);

        mbus_str_json_encode(str_encoded, mbus_data_variable_medium_lookup(header->medium), sizeof(str_encoded));
        len += snprintf(&buff[len], sizeof(buff) - len, "        \"Medium\": \"%s\",\n", str_encoded);

        len += snprintf(&buff[len], sizeof(buff) - len, "        \"AccessNumber\": %d,\n", header->access_no);
        len += snprintf(&buff[len], sizeof(buff) - len, "        \"Status\": \"%.2X\",\n", header->status);
        len += snprintf(&buff[len], sizeof(buff) - len, "        \"Signature\": \"%.2X%.2X\"\n", header->signature[1], header->signature[0]);

        len += snprintf(&buff[len], sizeof(buff) - len, "    }\n");

        return buff;
    }

    return "{}";
}

char *
mbus_data_variable_json_normalized(mbus_data_variable *data)
{
    mbus_data_record *record;
    mbus_record *norm_record;
    char *buff = NULL, *new_buff = NULL;
    char str_encoded[768] = "";
    size_t len = 0, buff_size = 8192;
    size_t i;

    if (data)
    {
        buff = (char*) malloc(buff_size);

        if (buff == NULL)
            return NULL;

        len += snprintf(&buff[len], buff_size - len, "{\n");

        len += snprintf(&buff[len], buff_size - len, "\"MBusData\": {\n");

        len += snprintf(&buff[len], buff_size - len, "\"SlaveInformation\": %s,\n", mbus_data_variable_header_json(&(data->header)));

        len += snprintf(&buff[len], buff_size - len, "\"DataRecords\": [\n");

        for (record = data->record, i = 0; record; record = record->next, i++)
        {
            norm_record = mbus_parse_variable_record(record);

            if ((buff_size - len) < 1024)
            {
                buff_size *= 2;
                new_buff = (char*) realloc(buff,buff_size);

                if (new_buff == NULL)
                {
                    mbus_record_free(norm_record);
                    free(buff);
                    return NULL;
                }

                buff = new_buff;
            }

            len += snprintf(&buff[len], buff_size - len, "    {\n");

            if (norm_record != NULL)
            {
                mbus_str_json_encode(str_encoded, norm_record->function_medium, sizeof(str_encoded));
                len += snprintf(&buff[len], buff_size - len, "        \"Function\": \"%s\",\n", str_encoded);

                len += snprintf(&buff[len], buff_size - len, "        \"StorageNumber\": %ld,\n", norm_record->storage_number);

                if (norm_record->tariff >= 0)
                {
                    len += snprintf(&buff[len], buff_size - len, "        \"Tariff\": %ld,\n", norm_record->tariff);
                    len += snprintf(&buff[len], buff_size - len, "        \"Device\": %d,\n", norm_record->device);
                }

                mbus_str_json_encode(str_encoded, norm_record->unit, sizeof(str_encoded));
                len += snprintf(&buff[len], buff_size - len, "        \"Unit\": \"%s\",\n", str_encoded);

                mbus_str_json_encode(str_encoded, norm_record->quantity, sizeof(str_encoded));
                len += snprintf(&buff[len], buff_size - len, "        \"Quantity\": \"%s\",\n", str_encoded);

                if (norm_record->is_numeric)
                {
                    len += snprintf(&buff[len], buff_size - len, "        \"Value\": %f\n", norm_record->value.real_val);
                }
                else
                {
                    mbus_str_json_encode(str_encoded, norm_record->value.str_val.value, sizeof(str_encoded));
                    len += snprintf(&buff[len], buff_size - len, "        \"Value\": \"%s\"\n", str_encoded);
                }

                mbus_record_free(norm_record);
            }
            else
            {
            }

            len += snprintf(&buff[len], buff_size - len, "    }");

            if (record->next)
            {
                len += snprintf(&buff[len], buff_size - len, ",\n");
            }
            else
            {
                len += snprintf(&buff[len], buff_size - len, "\n");
            }
        }

        len += snprintf(&buff[len], buff_size - len, "]\n");

        len += snprintf(&buff[len], buff_size - len, "}\n");

        len += snprintf(&buff[len], buff_size - len, "}\n");

        return buff;
    }

    return NULL;
}
