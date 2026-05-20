#pragma once

#include <stdlib.h>

#include "token/token.h"

struct Node {
    struct Token* value;
    struct Node* prev;
};

struct Queue {
    struct Node* first;
};

struct Node* create_node(struct Token* value) {
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->prev = NULL;

    return node;
}

struct Queue* create_queue() {
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));

    if (queue == NULL) {
        return NULL;
    }

    queue->first = NULL;

    return queue;
}

void push(struct Queue* queue, struct Token* value) {
    if (queue->first == NULL) {
        queue->first = create_node(value);
        return;
    }

    struct Node* current_node = queue->first;

    while (current_node->prev != NULL) {
        current_node = current_node->prev;
    }

    current_node->prev = create_node(value);
}

struct Node* pop(struct Queue* queue) {
    if(queue->first == NULL) return NULL;

    struct Node* node = queue->first;
    queue->first = queue->first->prev;
    return node;
}

int size(struct Queue* queue) {
    int cont = 0;

    struct Node* current = queue->first;

    while(current != NULL) {
        cont++;
        current = current->prev;
    }

    return cont;
}

void forEach(struct Queue* queue, void (*func)(struct Node* node)) {
    struct Node* current = queue->first;

    while(current != NULL) {
        func(current);
        current = current->prev;
    }
}

void printTokens(struct Node* node) {
    printf("\n{ value: '%s' | type: %s }\n", node->value->value, getTypeName(node->value->type));
}