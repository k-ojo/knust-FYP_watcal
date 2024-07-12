#include "../include/main.h"


/**
* watCal_set_frame: sets random data for the frame
* @reply: mbus frame
* @address: slave address
* Return: 1 on success
*/
int watCal_set_frame(mbus_frame* reply, int address)
{
    reply->address=address;
}

void watCal_proccess_frame(mbus_frame *frame)
{
	mbus_frame_data frame_data;
}

#include <stdio.h>
#include <curl/curl.h>

void send_mbus_frame(const char *xml_data) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/mbus-frame");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xml_data);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/xml");  // Set Content-Type as application/xml
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
* gen_xml- generates xml representation of mbus frame
* @frame_data- frame data
* @normalized: normalized form or not
* Return: xml string
*/
char *gen_xml(mbus_frame_data frame_data, int normalized)
{
	char *xml_result;

	xml_result = normalized ? mbus_frame_data_xml_normalized(&frame_data)
			: mbus_frame_data_xml(&frame_data);

	if (!xml_result)
	{
		fprintf(stderr, "Failed to generate XML representation of MBUS frame: %s\n", mbus_error_str());
		return (NULL);
	}
	return (xml_result);
}
void generate_random_hex_data(unsigned char *buffer, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        buffer[i] = (unsigned char)(rand() % 256);
    }
}
mbus_frame_data* _parse_mbus_frame(mbus_frame* reply, const unsigned char *raw_buff, size_t raw_buff_length)
{
    size_t buff_len;
    int result;
    unsigned char buff[4096];
    mbus_frame_data *frame_data = malloc(sizeof(mbus_frame_data));

    if (!frame_data)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    buff_len = mbus_hex2bin(buff, sizeof(buff), raw_buff, raw_buff_length);

    memset(frame_data, 0, sizeof(*frame_data));

    result = mbus_parse(reply, buff, buff_len);
    result = mbus_frame_data_parse(reply, frame_data);

    if (result < 0)
    {
        fprintf(stderr, "mbus_parse: %s\n", mbus_error_str());
        free(frame_data);
        return NULL;
    }
    else if (result > 0)
    {
        fprintf(stderr, "mbus_parse: need %d more bytes\n", result);
        free(frame_data);
        return NULL;
    }

    return frame_data;
}
