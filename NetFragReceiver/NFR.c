#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    double stock_val; // 8 bytes 
    uint32_t id; // 4 bytes
    uint8_t _paddings[4]; // 4 bytes
    // --- 16 bytes Packet ---
} Packet;

int main(void)
{
    return 0;
}
