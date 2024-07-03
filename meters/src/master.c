#include "../include/main.h"

int main()
{
    int m_fd = open(MASTER, O_RDWR | O_NOCTTY | O_NDELAY);
    mbus_handle *handle;

    handle = mbus_context_serial(MASTER);
    if (handle == NULL)
    {
        perror("failed to initialize");
        return (-1);
    };  //initializes right parameters for mbus protocol

    int slave_addr = 0x51;

    

    while (1) {
        send_request(handle, slave_addr, RELAY_COMMAND);
    }

    close(m_fd);
    return 0;
}
