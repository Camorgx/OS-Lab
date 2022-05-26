#include "kernel/scheduling/taskQueueFIFO.h"
#include "kernel/mem/mem.h"
#include "string.h"

taskQueueFIFO taskQueue;

const unsigned initial_size = 8;

#define QUEUE_TYPE taskQueueFIFO
#define ITEM_TYPE TCB
#define NULL_ITEM NULL_TCB

// 以 head + 1 表示队列的第一个元素，tail - 1 表示队列的最后一个元素。
unsigned qpush(QUEUE_TYPE* queue, ITEM_TYPE tcb) {
    if (queue->tail == queue->size - 1) {
        if (queue->head == 0) {
            ITEM_TYPE* tmp = (ITEM_TYPE*) kmalloc(2 * queue->size);
            if (!tmp) return 1;
            memcpy(tmp, queue->data, queue->size * sizeof(ITEM_TYPE));
            kfree((unsigned long)(queue->data));
            queue->data = tmp;
            queue->data[queue->tail++] = tcb;
            queue->size *= 2;
        }
        else {
            queue->data[queue->tail] = tcb;
            queue->tail = 0;
        }
    }
    else if (queue->tail == queue->head) {
        ITEM_TYPE* tmp = (ITEM_TYPE*) kmalloc(2 * queue->size);
        if (!tmp) return 1;
        memcpy(tmp, queue->data + queue->head, (queue->size - queue->head) * sizeof(ITEM_TYPE));
        memcpy(tmp + queue->size - queue->head, queue->data, queue->tail * sizeof(ITEM_TYPE));
        kfree((unsigned long)(queue->data));
        queue->tail = queue->size; queue->head = 0;
        queue->size *= 2; queue->data = tmp;
        queue->data[queue->tail++] = tcb;
    }
    else queue->data[queue->tail++] = tcb;
    return 0;
}

unsigned qpop(QUEUE_TYPE* queue) {
    if (qempty(queue)) return 1;
    else if (queue->head == queue->size - 1) queue->head = 0;
    else ++queue->head;
}

ITEM_TYPE qfront(const QUEUE_TYPE* queue) {
    if (qempty(queue)) return NULL_ITEM;
    else if (queue->head == queue->size - 1) return queue->data[0];
    else return queue->data[queue->head + 1];
}

ITEM_TYPE qback(const QUEUE_TYPE* queue) {
    if (qempty(queue)) return NULL_ITEM;
    else if (queue->tail == 0) return queue->data[queue->size - 1];
    else return queue->data[queue->tail - 1];
}

unsigned qlength(const QUEUE_TYPE* queue) {
    if (queue->tail > queue->head) return queue->tail - queue->head - 1;
    else return queue->tail + queue->size - queue->head - 1;
}

unsigned qfull(const QUEUE_TYPE* queue) {
    return (queue->head == queue->tail) 
        || (queue->head == 0 && queue->tail == queue->size - 1);
}

unsigned qempty(const QUEUE_TYPE* queue) {
    return (queue->tail == queue->head + 1 )
        || (queue->tail == 0 && queue->head == queue->size - 1);
}
