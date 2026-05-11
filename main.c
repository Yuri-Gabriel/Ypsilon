#include "data_struct/queue.h"

int main() {
    struct Queue* queue = create_queue();
    push(queue, "1");
    push(queue, "2");
    push(queue, "3");
    push(queue, "4");
    push(queue, "5");

    struct Node* first = pop(queue);
    return 0;
}