#ifndef BITSET_H
#define BITSET_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BIT(x) (1 << (x))

typedef struct
{
    uint64_t *array;
    size_t capacity;
} bitset_t;

/**
 * @brief Creates an instance of the bitset_t struct
 * @param size_t num_bits
 * @return bitset_t
 */
bitset_t bitset_create(size_t num_bits);

/**
 * @brief Destroyes the bitset_t instance
 * @param bitset_t set pointer
 * @return void 
 */
void bitset_destroy(bitset_t *set);


/**
 * @brief Sets the bit at given index 
 * @param bitset_t set pointer
 * @param size_t index
 * @return void
 */
void bitset_set(bitset_t *set, size_t index);

/**
 * @brief Sets the bit at index 0
 * @param bitset_t set pointer
 * @param size_t index
 * @return void
 */
void bitset_clear(bitset_t *set, size_t index);

/**
 * @brief Returns true (1) if set, otherwise false (0)
 * @param bitset_t set pointer
 * @param size_t index
 * @return void
 */
bool bitset_test(bitset_t *set, size_t index);

#define ERRN0 printf("Could not allocate space for the set");
#define WARN0(x) printf("Bit at index %lu is already set to 1\n", x);

#endif
