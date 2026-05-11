#include <stdlib.h>

struct Node {
    char* value;
    struct Node* prev;
};

struct Queue {
    struct Node* first;
};

struct Node* create_node(char* value) {
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

void push(struct Queue* queue, char* value) {
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