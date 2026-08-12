#include <stdlib.h>
#include "bitset.h"

int main(void)
{
    bitset_t set = bitset_create(254);
    printf("Sets allocated at address %p\n", set.array);
    
    bitset_set(&set, 0);
    bitset_set(&set, 0);
    bitset_destroy(&set);
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

/**
 * @brief Destroyes the bitset_t instance
 * @param bitset_t set pointer
 * @return void
 */
void bitset_destroy(bitset_t *set)
{
    free(set->array);
}

/**
 * @brief Sets the bit at given index
 * @param bitset_t set pointer
 * @param size_t index
 * @return void
 */
void bitset_set(bitset_t *set, size_t index)
{
    if((set->array[index] & BIT(index)) == 0)
        set->array[index] |= BIT(index);
    else
        WARN0(index);
}
