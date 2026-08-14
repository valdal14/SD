#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Concurrency variables
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;

#define MAX_DEL_PKT 16 

typedef struct Packet
{
    double stock_val; // 8 bytes 
    uint32_t id; // 4 bytes
    uint8_t _paddings[4]; // 4 bytes
    // --- 16 bytes Packet ---
} Packet;

typedef struct Deliverable
{
    struct Packet *packets; // 16 bytes
    void *(* deliver)(void *arg); // 8 bytes 
    uint8_t size; // 1 byte  
    uint8_t index; // 1 byte 
    uint8_t tail; // 1 byte 
    uint8_t capacity; // 1 byte 
    uint8_t _paddings[4]; // 4 bytes 
    // --- 32 bytes Deliverable ---
} Deliverable;

typedef struct
{
    struct Deliverable *deliverable;
    uint32_t id;
    double val;
} UDP;

/**
 * @brief Allocates the space and init the Deliverable interface
 * @param Deliverable deliverable double pointer
 * @return void
 */
void init_interface(Deliverable **deliverable);

/**
 * @brief Helper function used to prepare the packets 
 * that needs to be delivered.
 * @param Deliverable double pointer
 * @param uint32_t id
 * @param double val
 * @return void
 */
void prepare_packet(Deliverable **deliverable, uint32_t id, double val);

/**
 * @brief Callback used to deliver packets 
 * @param void arg pointer
 * @return void pointer
 */
void *deliver(void *arg);

/**
 * @brief Helper function used to prepare the packets that needs
 * to be delivered to test this API.
 * @param UDP packet pointer 
 * @param void*(*deliver)(void *arg) Callback
 * @return void
 */
void process_packet(UDP *packet, void*(*deliver)(void *arg));

/// Main Method -----------------------------------------------------------

int main(void)
{
    Deliverable *deliverable = NULL;
    init_interface(&deliverable);
    printf("Deliverable Interface up and running at address: %p\n", deliverable);
    
    UDP udp_packet;
    udp_packet.deliverable = deliverable;
    udp_packet.id = 0;
    udp_packet.val = 14.23;

    process_packet(&udp_packet, deliver);
    return 0;
}

/// Implementation --------------------------------------------------------

void *deliver(void *arg)
{
    UDP *packet = (UDP *)arg;

    printf("Deliving Packet with ID: %d\n", packet->id);
    sleep(1); 
    printf("Packet successufully delivered\n");
    
    return (void *)packet;
}

void init_interface(Deliverable **deliverable)
{
    Deliverable *new = (Deliverable *)calloc(1, sizeof(Deliverable));

    if(new == NULL)
    {
        fprintf(stderr, "Could not allocate space for the delivery interface\n");
        exit(EXIT_FAILURE);
    }

    new->packets = (Packet *)calloc(MAX_DEL_PKT, sizeof(Packet));

    if(new->packets == NULL)
    {
        fprintf(stderr, "Could not allocate space for the packets buffer\n");
        exit(EXIT_FAILURE);
    }
    
    new->deliver = deliver;
    new->capacity = MAX_DEL_PKT;
    new->index = 0;
    new->tail = 0;
    new->size = 0;
    *deliverable = new;
}

void process_packet(UDP *packet, void*(*deliver)(void *arg))
{
    pthread_t worker;
    pthread_create(&worker, NULL, deliver, (void *)packet);
    pthread_join(worker, NULL);
}
