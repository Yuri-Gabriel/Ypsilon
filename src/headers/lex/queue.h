#ifndef QUEUE_H
#define QUEUE_H

#include "token.h"

typedef struct Node {
    Token* value;
    struct Node* prev;
} Node;

typedef struct Queue {
    Node* first;
} Queue;

Node* create_node(Token* value);
Queue* create_queue();
void push(Queue* queue, Token* value);
Node* pop(Queue* queue);
int size(Queue* queue);
void forEach(Queue* queue, void (*func)(Node* node));
void printTokens(Node* node);

#endif