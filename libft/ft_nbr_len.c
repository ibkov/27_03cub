#include "libft.h"

int ft_nbr_len(int nbr, int base_len)
{
    int i;

    i = 1;
    while (nbr >= base_len)
    {
        nbr /= base_len;
        i++; 
    }
    return (i);
}
