#ifndef SYNC_MESSAGE_H
#define SYNC_MESSAGE_H 1

#include <stdint.h>

typedef struct message_t {
    uint32_t sender;
    uint32_t type;
    union {
        uint64_t part[3];   // work in progress
    } payload;
} __attribute__((packed)) message_t;

int send(int dest, message_t* msg);
int receive(int src, message_t* msg);

#endif
