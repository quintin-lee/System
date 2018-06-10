#include "../include/queue.h"
#include <stdio.h>

#define autofree(typename) smart_##typename typename

int main(void)
{
    //smart_queue Queue q = queue_create();
    autofree(Queue) q = queue_create();
    unsigned int len = 0;
    int i = 0;
    for (i = 0; i < 11; i++) {
        queue_enqueue(q, &i, sizeof(i));
    }
    //queue_make_empty(q);
    while (!queue_is_empty(q)) {
        int d = 0;
        queue_dequeue(q, &d, sizeof(d));
        queue_length(q, &len);
        printf("queue:%d  %d\n", d, len);
    }
    //queue_destroy(q);
    return 0;
}
