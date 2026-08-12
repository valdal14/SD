#include <stdlib.h>
#include "bitset.h"

int main(void)
{
    bitset_t set = bitset_create(254);
    printf("Sets allocated at address %p\n", set.array);
    return 0;
}

/**
 * @brief Creates an instance of the bitset_t struct
 * @param size_t num_bits
 * @return bitset_t
 */
bitset_t bitset_create(size_t num_bits)
{
    bitset_t set;
    set.capacity = num_bits;
    set.array = (uint64_t *)calloc(num_bits, sizeof(uint64_t));
   
    if(set.array == NULL) 
    {
        ERRN0;
        exit(EXIT_FAILURE);
    }

    return set;
}
