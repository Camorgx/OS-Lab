#include "mem.h"

unsigned long kmalloc(unsigned long size) {
    return dPartitionAlloc(kernelMemHandler, size);
}

unsigned long kfree(unsigned long start) {
    return dPartitionFree(kernelMemHandler, start);
}

unsigned long malloc(unsigned long size) {
    return dPartitionAlloc(userMemHandler, size);
}

unsigned long free(unsigned long start) {
    return dPartitionFree(userMemHandler, start);
}