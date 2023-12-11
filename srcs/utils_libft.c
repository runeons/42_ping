#include "ping_functions.h"

char    *int_to_bin(int n, int len)
{
    char    *bin;
    int     k = 0;

    bin = (char*)mmalloc(sizeof(char) * len);
    if (bin == NULL)
        exit_error("ping: malloc failure.");
    for (unsigned i = (1 << (len - 1)); i > 0; i = i / 2)
        bin[k++] = (n & i) ? '1' : '0';
    bin[k] = '\0';
    return bin;
}

void print_as_hexa(void *data, int size)
{
    const unsigned char *ptr = (const unsigned char *)data;

    for (int i = 0; i < size; i++)
        printf("%02x ", ptr[i]);
    printf("\n");
}
