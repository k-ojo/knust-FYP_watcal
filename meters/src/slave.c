#include "../include/main.h"

int main() {
    mbus_handle *handle;

    handle = mbus_context_serial(SLAVE);
    if (handle == NULL)
    {
        perror("Oops!\n");
        return (1);
    } 
    return 0;
}
