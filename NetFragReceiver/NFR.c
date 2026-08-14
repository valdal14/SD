#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
    void(* deliver)(struct Deliverable *deliverable); // 8 bytes 
    // --- 24 bytes Deliverable ---
} Deliverable;


/**
 * @brief Callback used to deliver packets 
 * @param Deliverable deliverable pointer
 * @return void
 */
void deliver(struct Deliverable *deliverable);

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

int main(void)
{
    Deliverable *deliverable = NULL;
    init_interface(&deliverable);
    printf("Deliverable Interface up and running at address: %p\n", deliverable);
    return 0;
}

/// Implementation --------------------------------------------------------

void deliver(Deliverable *deliverable)
{
    Packet *packets = deliverable->packets;

    for(uint8_t i = 0; i < MAX_DEL_PKT; i++)
        printf("Packet ID: %d\n", (packets + i)->id);
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
    *deliverable = new;
}


