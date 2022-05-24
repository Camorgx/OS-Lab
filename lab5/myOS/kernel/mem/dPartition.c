#include "lib/libio.h"

// dPartition 是整个动态分区内存的数据结构
typedef struct dPartition {
	unsigned long size;
	unsigned long firstFreeStart; 
} dPartition;	// 共占 8 个字节

void show_dPartition(struct dPartition *dp) {
	printk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}

// EMB 是每一个block的数据结构
typedef struct EMB {
	unsigned long size;
    unsigned long free;
    struct EMB* prev; // 表示上一个 EMB 的起始地址，不一定是空闲的，为了便于释放内存
    struct EMB* nextStart;
    struct EMB* prevStart; // 链表中上一个空闲块的地址
} EMB;	// 共占20个字节

void showEMB(struct EMB * emb) {
	printk(0x3,"EMB(start=%p, free=%d, size=0x%x, nextStart=%p, prevStart=%p)\n",
             emb, emb->free, emb->size, emb->nextStart, emb->prevStart);
}

/*
    功能：初始化内存。
        1. 在地址start处，首先是要有 dPartition 结构体表示整个数据结构(也即句柄)。
        2. 然后，一整块的 EMB 被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在 dP 后面，然后 dP 的 firstFreeStart 指向 EMB。
        3. 返回 start 首地址(也即句柄)。
    注意有两个地方的大小问题：
        第一个是由于内存肯定要有一个 EMB 和 一个dPartition，totalSize 肯定要比这两个加起来大。
        第二个注意 EMB 的 size 属性不是 totalSize，因为 dPartition 和 EMB 自身都需要要占空间。
*/
unsigned long dPartitionInit(unsigned long start, unsigned long totalSize) {
    if (totalSize <= sizeof(EMB) + sizeof(dPartition)) return 0;
    dPartition* handle = (dPartition*)start;
    EMB* emb = (EMB*)(start + sizeof(dPartition));
    *handle = (dPartition){.size = totalSize - sizeof(dPartition), .firstFreeStart = (unsigned long)emb};
    *emb = (EMB){.size = handle->size - sizeof(EMB), .free = 1, .prev = 0, .nextStart = 0};
    return start;
}

/*
    功能：本函数遍历输出EMB 方便调试
        1. 先打印 dP 的信息，可调用上面的 show_dPartition。
        2. 然后按地址的大小遍历 EMB ，对于每一个 EMB，可以调用上面的 showEMB 输出其信息
*/
void dPartitionWalkByAddr(unsigned long dp) {
    dPartition* d_par =(dPartition*)dp;
    show_dPartition(d_par);
    for (unsigned long i = dp + sizeof(dPartition); i - dp - sizeof(dPartition) < d_par->size;
        i = ((EMB*)i)->size + i + sizeof(EMB))
        showEMB((EMB*)i);
}

/*
    功能：分配一个空间
        1. 使用 first fit 的算法分配空间，当然也可以使用其他 fit，不限制。
        2. 成功分配返回首地址，不成功返回 0
        3. 从空闲内存块组成的链表中拿出一块供我们来分配空间(如果提供给分配空间的内存块空间大于 size，我们还将把剩余部分放回链表中)，
            并维护相应的空闲链表以及句柄
    注意的地方：
        1.EMB类型的数据的存在本身就占用了一定的空间。
*/
extern unsigned long align;
unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size) {
    if (size == 0) return 0;
    dPartition* d_par = (dPartition*)dp;
    EMB* emb = (EMB*)(d_par->firstFreeStart);
    if (size % align) size = (size / align + 1) * align;
    int flag = 0;
    for (; emb; emb = emb->nextStart) {
        if (emb->size >= size + sizeof(EMB)
            || emb->size == size) { flag = 1; break; }
    }
    if (!flag) return 0;
    if (emb->size == size) {
        if (emb->prevStart == 0) // Head of the list.
            d_par->firstFreeStart = (unsigned long)emb->nextStart;
        else emb->prevStart->nextStart = emb->nextStart;
        emb->free = 0;
    }
    else {
        unsigned long new_emb_pos = (unsigned long)(emb + 1) + size;
        *(EMB*)(new_emb_pos) = (EMB){.size = emb->size - size - sizeof(EMB), .free = 1,
                .nextStart = emb->nextStart, .prev = emb, .prevStart = emb->prevStart};
        emb->size = size; emb->free = 0;
        if (emb->prevStart == 0) d_par->firstFreeStart = new_emb_pos;
        else emb->prevStart->nextStart = (EMB*)new_emb_pos;
    }
    return (unsigned long)(emb + 1);
}

/*
    功能：释放一个空间
        1. 按照对应的 fit 的算法释放空间
        2. 注意检查要释放的 start~end 这个范围是否在dp有效分配范围内
            返回 0 没问题
            返回 1 error
        3. 需要考虑两个空闲且相邻的内存块的合并
*/
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start) {
    unsigned long max_len = dp + ((dPartition*)dp)->size + sizeof(dPartition);
	if (start >= max_len) return 1;
    dPartition* dP = (dPartition*)dp;
    EMB* emb = (EMB*)start - 1;
    if (emb->free) return 1;
    EMB* next = (EMB*)(start + emb->size);
    if (start + emb->size < max_len && next->free) {
        // First remove the next block.
        if (next->prevStart) next->prevStart->nextStart = next->nextStart;
        else dP->firstFreeStart = 0;
        if (next->nextStart) next->nextStart->prevStart = next->prevStart;
        // Merge the next block into the current one.
        emb->size += sizeof(EMB) + next->size;
        if ((unsigned long)next + next->size + sizeof(EMB) < max_len)
            ((EMB*)((unsigned long)next + next->size + sizeof(EMB)))->prev = emb;
    }
    EMB* prev = emb->prev;
    if (prev && prev->free) {
        prev->size += sizeof(EMB) + emb->size;
        if (start + emb->size < max_len)
            ((EMB*)(start + emb->size))->prev = prev;
    }
    else {
        // Insert emb to the head of the list.
        emb->nextStart = (EMB*)(dP->firstFreeStart);
        emb->prevStart = 0;
        emb->prev = 0;
        emb->free = 1;
        if (dP->firstFreeStart)
            ((EMB*)(dP->firstFreeStart))->prevStart = emb;
        dP->firstFreeStart = (unsigned long)emb;
    }
    return 0;
}

// wrap: we select firstFit, you can select another one
// user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
