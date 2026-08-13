#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define QUEUE_SIZE 5
#define DEF_TRANS_MASK 0x8
#define INV_TRANS_MASK 0x0
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
    uint8_t _paddings[7]; // 7 bytes paddings
    // Total 16 bytes 
} Transaction;

typedef struct 
{
    struct Transaction *transactions; // 16 bytes 
    uint8_t capacity; // 1 byte 
    uint8_t index; // 1 byte 
    uint8_t _paddings[6]; // 6 bytes paddings  
    // Total 24 bytes 
} Queue;

/**
 * @brief Inits the Queue
 * @param Queue queue double pointer
 * @return void
 */
void init_queue(Queue **queue);

/**
 * @brief Processes a transaction 
 * @param Queue queue pointer
 * @param Transaction transaction pointer
 * @return void
 */
void process_transaction(Queue *queue, Transaction *transaction);

/**
 * @brief Helper method to setup transaction examples
 * @param Transactions transactions double pointer
 * @param size_t trans_amt
 * @return void
 */
void setup_transactions(Transaction **transactions, size_t trans_amt);

/**
 * @brief Runs a test of the current Implementation
 * @param Queue queue pointer
 * @param Transaction transactions pointer
 * @param size_t size 
 * @return void
 */
void run(Queue *queue, Transaction *transactions, size_t size);

int main(void)
{
    size_t trans_amt = 8;
    Queue *queue = NULL;
    Transaction *transactions = NULL;
    init_queue(&queue);
    setup_transactions(&transactions, trans_amt);
    printf("Queue up and running at address: %p\n", queue);
    printf("Sample Transactions created at address: %p\n", transactions);
    // Processing the Transactions
    run(queue, transactions, trans_amt);
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
    new_queue->index = 0;
    *queue = new_queue;
}

void setup_transactions(Transaction **transactions, size_t trans_amt)
{
    Transaction *trans = (Transaction *)calloc(trans_amt, sizeof(Transaction)); 
    
    if(trans == NULL)
    {
        fprintf(stderr, "Could not allocate space for the transaction examples\n");
        exit(EXIT_FAILURE);
    }

    for(uint8_t i = 0; i < trans_amt; i++)
    {
        Transaction *t1 = (Transaction *)calloc(1, sizeof(Transaction));
        if(t1 == NULL) exit(EXIT_FAILURE);
        t1->amount = 14 + i;

        if(i == 3) 
            t1->flag = INV_TRANS_MASK;
        else 
            t1->flag = DEF_TRANS_MASK;

        trans[i] = *t1;
    }

    *transactions = trans;
}

void process_transaction(Queue *queue, Transaction *transaction)
{
    if(queue->index == queue->capacity) queue->index = 0;

    queue->index = queue->index % queue->capacity;
    printf("idx = %d\n", queue->index);
    queue->transactions[queue->index] = *transaction;
    printf("AMT = %llu\n", queue->transactions[queue->index].amount); 
    printf("TRA = %d\n", queue->transactions[queue->index].flag); 
    queue->index += 1;
    printf("--------------------\n");
}


void run(Queue *queue, Transaction *transactions, size_t size)
{
    for(uint8_t i = 0; i < size; i++)
        process_transaction(queue, &transactions[i]);
}
