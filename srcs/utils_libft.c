#include "ping_functions.h"

void print_as_hexa(void *data, int size)
{
    const unsigned char *ptr = (const unsigned char *)data;

    for (int i = 0; i < size; i++)
        printf("%02x ", ptr[i]);
    printf("\n");
}

int ft_isstrfloat(const char *s)
{
    int i           = 0;
    int nb_dot      = 0;
    int nb_digit    = 0;

    if (s && s[i] && (s[i] == '-' || s[i] == '+') && s[i + 1])
        i++;
    while (s && s[i])
    {
        if (s[i] == '.')
        {
            nb_dot++;
            if (nb_dot > 1)
                return 0;
        }
        else if (!ft_isdigit(s[i]))
            return 0;
        nb_digit++;
        i++;
    }
    return (nb_digit > 0 && nb_dot <= 1);
}

float ft_atof(const char *s)
{
    int i;
    float res;
    float fraction;
    int n;

    i = 0;
    res = 0.0f;
    fraction = 0.0f;
    n = 1;

    while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
        i++;

    if (s[i] == '+' || s[i] == '-') {
        if (s[i] == '-')
            n = -1;
        i++;
    }

    while (ft_isdigit(s[i])) {
        res = res * 10.0f + (s[i] - '0');
        i++;
    }

    if (s[i] == '.') {
        i++;

        float divisor = 10.0f;
        while (ft_isdigit(s[i])) {
            fraction += (s[i] - '0') / divisor;
            divisor *= 10.0;
            i++;
        }
    }
    return n * (res + fraction);
}

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

unsigned int ft_sqrt(unsigned int x)
{
    unsigned int r = x;
    while (r * r > x) {
        r = (r + x / r) / 2;
    }
    return r;
}
