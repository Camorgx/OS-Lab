#include "kernel/scheduling/taskVector.h"
#include "kernel/mem/mem.h"
#include "string.h"

const unsigned vector_initial_size = 8;

void vInit(taskVector* vector) {
    vector->dat = (TCB*) kmalloc(vector_initial_size * sizeof(TCB));
    vector->actual_size = vector_initial_size;
    vector->array_size = 0;
}

unsigned vSize(taskVector* vector) {
    return vector->array_size;
}

unsigned vEmpty(taskVector* vector) {
    return (vector->array_size == 0);
}

TCB* vAt(taskVector* vector, unsigned id) {
    return vector->dat + id;
}

void vClear(taskVector* vector) {
    vector->array_size = 0;
}

void vPushBack(taskVector* vector, TCB x) {
    if (vector->array_size >= vector->actual_size) {
        TCB* tmp = (TCB*) kmalloc((vector->actual_size * 2) * sizeof(TCB));
        memcpy(tmp, vector->dat, vector->actual_size * sizeof(TCB));
        kfree((unsigned long)(vector->dat));
        vector->dat = tmp;
        vector->actual_size *= 2;
    }
    vector->dat[vector->array_size++] = x;
}

void vPopBack(taskVector* vector) {
    if (vector->array_size) --vector->array_size;
}