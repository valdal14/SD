#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define QUEUE_SIZE 5
#define DEF_TRANS_MASK 0x8
#define INV_TRANS_MASK 0x0
#define BIT(x) (1U << (x))
#define PRO_BIT 0
#define DEL_BIT 1
#define VAL_BIT 3
#define STO_BIT 7

typedef struct Transaction
{
    uint64_t amount; // 8 bytes
    uint32_t id; // 4 byte
    /**
     * bit 0 = processed
     * bit 2 = delivered
     * bit 4 = valid
     * bit 8 = stored
     */
    uint8_t flag; // 1 byte
    uint8_t _paddings[3]; // 3 bytes paddings
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
 * @param void(*proc)(Queue *queue, uint8_t index) callback 
 * to process and deliver the transaction
 * @return void
 */
void process_transaction(Queue *queue, Transaction *transaction, void(*proc)(Queue *queue, uint8_t index));

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
 * @param void(*proc)(Queue *queue, uint8_t index) callback 
 * to process and deliver the transaction
 * @return void
 */
void run(Queue *queue, Transaction *transactions, size_t size, void(*proc)(Queue *queue, uint8_t index));

/**
 * @brief Processes and Delivers the Transaction
 * @param Queue queue pointer 
 * @param uint8_t index 
 * @return void
 */
void deliver(Queue *queue, uint8_t index);

/**
 * @brief Clean ups all allocated memory
 * @param Queue queue pointer
 * @param Transaction transactions pointer
 * @return void
 */
void clean(Queue *queue, Transaction *transactions);

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
    run(queue, transactions, trans_amt, deliver);
    clean(queue, transactions);
    
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
        Transaction t1 = {0};
        t1.id = (i + 1);
        t1.amount = 14 + i;

        if(i == 3) 
            t1.flag = INV_TRANS_MASK;
        else 
            t1.flag = DEF_TRANS_MASK;

        trans[i] = t1;
    }

    *transactions = trans;
}

void process_transaction(Queue *queue, Transaction *transaction, void(*proc)(Queue *queue, uint8_t index))
{
    if((transaction->flag & BIT(VAL_BIT)) == 0)
    {
        printf("----------------------------------\n");
        printf("Invalid Transaction: \n");
        printf("STATUS = %d\n", transaction->flag); 
        printf("AMOUNT = %llu\n", transaction->amount); 
        printf("This Transaction will be discarded\n");
        printf("----------------------------------\n");
    }
    else
    {
        // set the transaction flags
        transaction->flag |= BIT(0);

        // Store the transaction at the current safe index
        queue->transactions[queue->index] = *transaction;

        // Process and deliver using the current safe index
        proc(queue, queue->index);

        // Increment the index 
        queue->index = (queue->index + 1) % queue->capacity;;
    }
}


void run(Queue *queue, Transaction *transactions, size_t size, void(*proc)(Queue *queue, uint8_t index))
{
    for(uint8_t i = 0; i < size; i++)
        process_transaction(queue, &transactions[i], proc);
}


void deliver(Queue *queue, uint8_t index)
{
    printf("--------------------------------------\n");
    printf("Delivering Transaction ID  : %d\n", queue->transactions[index].id);
    printf("Transaction Stored at index: %d\n", index);
    printf("Transaction Amount         : %llu\n", queue->transactions[index].amount);
    printf("--------------------------------------\n");
    // set the flag to deliver 
    queue->transactions[index].flag |= BIT(DEL_BIT);
    queue->transactions[index].flag |= BIT(STO_BIT);
}

void clean(Queue *queue, Transaction *transactions)
{
    if(transactions != NULL) free(transactions);
    if(queue->transactions != NULL) free(queue->transactions);
    if(queue != NULL) free(queue);
}
