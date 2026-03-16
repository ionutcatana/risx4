#ifndef SYNC_MESSAGE_H
#define SYNC_MESSAGE_H 1

#include <stdint.h>

struct message {
    uint32_t sender;
    uint32_t type;
    union {
        uint64_t part[3];   // work in progress
    } payload;
} __attribute__((packed));

int send(int dest, struct message* msg);
int receive(int src, struct message* msg);

#endif
