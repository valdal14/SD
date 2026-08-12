#include <stdlib.h>
#include "bitset.h"

int main(void)
{
    bitset_t set = bitset_create(254);
    printf("Sets allocated at address %p\n", set.array);
    
    bitset_set(&set, 0);
    bool test = bitset_test(&set, 0);
    show_test_result(test, 0);
    bitset_set(&set, 0);
    
    bitset_clear(&set, 0);
    bitset_clear(&set, 12);
    test = bitset_test(&set, 0);
    show_test_result(test, 0);
    
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
    if(num_bits > MAX_BIT_SIZE) 
    {
        ERRN1;
        exit(EXIT_FAILURE);
    }

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
    {
        set->array[index] |= BIT(index);
        set->capacity += 1;
    }
    else
        WARN0(index);
}

/**
 * @brief Sets the bit at index 0
 * @param bitset_t set pointer
 * @param size_t index
 * @return void
 */
void bitset_clear(bitset_t *set, size_t index)
{
    if((set->array[index] & BIT(index)) == 1)
    {
        set->array[index] &= ~BIT(index);
        PTRSET(index, set->array[index]);
    }
    else
        WARN1(index);
}

/**
 * @brief Returns true (1) if set, otherwise false (0)
 * @param bitset_t set pointer
 * @param size_t index
 * @return void
 */
bool bitset_test(bitset_t *set, size_t index)
{
    if((set->array[index] & BIT(index)) == 1)
        return true;
    else
        return false;
}

/**
 * @brief Prints out the result of the bitset_set
 * return value
 * @param bool val
 * @param size_t index
 * @return void
 */
void show_test_result(bool val, size_t index)
{
    val == 1 ? 
    printf("Bit at index %lu is TRUE\n", index) : 
    printf("Bit at index %lu is FALSE\n", index);
}
