#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define QUEUE_SIZE 5
#define DEF_TRANS_MASK 0x0
#define BIT(x) (1 << (x))
#define PRO_BIT 0
#define DEL_BIT 1
#define VAL_BIT 3
#define STO_BIT 7

typedef struct Transaction
{
    uint64_t amount; // 8 bytes
    /**
     * bit 0 = processed
     * bit 2 = delivered
     * bit 4 = valid
     * bit 8 = stored
     */
    uint8_t flag; // 1 byte
    uint8_t paddings[7]; // 7 bytes paddings
    // Total 16 bytes 
} Transaction;

typedef struct 
{
    struct Transaction *transactions; // 16 bytes 
    uint8_t size; // 1 byte 
    uint8_t capacity; // 1 byte 
    uint8_t paddings[6]; // 6 bytes paddings  
    // Total 24 bytes 
} Queue;

/**
 * @brief Inits the Queue
 * @param Queue queue double pointer
 * @return void
 */
void init_queue(Queue **queue);

int main(void)
{
    Queue *queue = NULL;
    init_queue(&queue);
    printf("Queue up and running at address: %p\n", queue);
    return 0;
}

// Implementation ---------------------------------------------

void init_queue(Queue **queue)
{
    Queue *new_queue = (Queue *)calloc(1, sizeof(Queue));

    if(new_queue == NULL)
    {
        fprintf(stderr, "Could not allocate space for the Queue\n");
        exit(EXIT_FAILURE);
    }

    new_queue->transactions = (Transaction *)calloc(QUEUE_SIZE, sizeof(Transaction));

    if(new_queue->transactions == NULL)
    {
        fprintf(stderr, "Could not allocate space for holding the transactions\n");
        exit(EXIT_FAILURE);
    }

    new_queue->capacity = QUEUE_SIZE;
    new_queue->size = 0;

    *queue = new_queue;
}
