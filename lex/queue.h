#pragma once

#include <stdlib.h>

#include "token/token.h"

typedef struct Node {
    Token* value;
    struct Node* prev;
} Node;

typedef struct {
    Node* first;
} Queue;

Node* create_node(Token* value) {
    Node* node = (Node*) malloc(sizeof(Node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->prev = NULL;

    return node;
}

Queue* create_queue() {
    Queue* queue = (Queue*) malloc(sizeof(Queue));

    if (queue == NULL) {
        return NULL;
    }

    queue->first = NULL;

    return queue;
}

void push(Queue* queue, Token* value) {
    if (queue->first == NULL) {
        queue->first = create_node(value);
        return;
    }

    Node* current_node = queue->first;

    while (current_node->prev != NULL) {
        current_node = current_node->prev;
    }

    current_node->prev = create_node(value);
}

Node* pop(Queue* queue) {
    if(queue->first == NULL) return NULL;

    Node* node = queue->first;
    queue->first = queue->first->prev;
    return node;
}

int size(Queue* queue) {
    int cont = 0;

    Node* current = queue->first;

    while(current != NULL) {
        cont++;
        current = current->prev;
    }

    return cont;
}

void forEach(Queue* queue, void (*func)(Node* node)) {
    Node* current = queue->first;

    while(current != NULL) {
        func(current);
        current = current->prev;
    }
}

void printTokens(Node* node) {
    printf("\n{ value: '%s' | type: %s }\n", node->value->value, getTypeName(node->value->type));
}