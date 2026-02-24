#ifndef SYNC_MESSAGE_H
#define SYNC_MESSAGE_H 1

#include <stdint.h>

typedef uint64_t message_t;

int send(int dest, message_t msg);
int receive(int src, message_t* msg);

#endif
