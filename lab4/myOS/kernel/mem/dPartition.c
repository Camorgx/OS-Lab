#include "libio.h"

//dPartition 是整个动态分区内存的数据结构
typedef struct dPartition {
	unsigned long size;
	unsigned long firstFreeStart; 
} dPartition;	//共占8个字节

void show_dPartition(struct dPartition *dp) {
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}

// EMB每一个block的数据结构，userdata可以暂时不用管。
typedef struct EMB {
	unsigned long size;
    unsigned long free;
    struct EMB* prev; // 表示上一个EMB的起始地址，不一定是空闲的，为了便于释放内存
    struct EMB* nextStart;
    struct EMB* prevStart; // 链表中上一个空闲块的地址
} EMB;	// 共占20个字节

void showEMB(struct EMB * emb) {
	myPrintk(0x3,"EMB(start=%p, free=%d, size=0x%x, nextStart=%p, prevStart=%p)\n",
             emb, emb->free, emb->size, emb->nextStart, emb->prevStart);
}

unsigned long dPartitionInit(unsigned long start, unsigned long totalSize) {
	// 本函数需要实现！！！
	/*  功能：初始化内存。
        1.在地址start处，首先是要有dPartition结构体表示整个数据结构(也即句柄)。
        2.然后，一整块的EMB被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB。
        3.返回start首地址(也即句柄)。
        注意有两个地方的大小问题：
            第一个是由于内存肯定要有一个EMB和一个dPartition，totalSize肯定要比这两个加起来大。
            第二个注意EMB的size属性不是totalSize，因为dPartition和EMB自身都需要要占空间。
	*/
    if (totalSize <= sizeof(EMB) + sizeof(dPartition)) return 0;
    dPartition* handle = (dPartition*)start;
    EMB* emb = (EMB*)(start + sizeof(dPartition));
    *handle = (dPartition){.size = totalSize - sizeof(dPartition), .firstFreeStart = (unsigned long)emb};
    *emb = (EMB){.size = handle->size - sizeof(EMB), .free = 1, .prev = 0, .nextStart = 0};
    return start;
}

void dPartitionWalkByAddr(unsigned long dp) {
	// 本函数需要实现！！！
	/*  功能：本函数遍历输出EMB 方便调试
        1.先打印dP的信息，可调用上面的show_dPartition。
        2.然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息
	*/
    dPartition* d_par =(dPartition*)dp;
    show_dPartition(d_par);
    for (unsigned long i = dp + sizeof(dPartition); i - dp - sizeof(dPartition) <= d_par->size;
        i = ((EMB*)i)->size + i + sizeof(EMB))
        showEMB((EMB*)i);
}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/

unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	// 本函数需要实现！！！
	/*  功能：分配一个空间
        1.使用firstfit的算法分配空间，当然也可以使用其他fit，不限制。
        2.成功分配返回首地址，不成功返回0
        3.从空闲内存块组成的链表中拿出一块供我们来分配空间(如果提供给分配空间的内存块空间大于size，我们还将把剩余部分放回链表中)，
            并维护相应的空闲链表以及句柄
        注意的地方：
            1.EMB类型的数据的存在本身就占用了一定的空间。
	*/
    dPartition* d_par = (dPartition*)dp;
    EMB* emb = (EMB*)(d_par->firstFreeStart);
    int flag = 0;
    for (; emb; emb = emb->nextStart) {
        if (emb->size >= size + sizeof(EMB)
            | emb->size == size) { flag = 1; break; }
    }
    if (!flag) return 0;
    if (emb->size == size) {
        if (emb->prevStart == 0) // Head of the list.
            d_par->firstFreeStart = (unsigned long)emb->nextStart;
        else emb->prevStart->nextStart = emb->nextStart;
        emb->free = 0;
    }
    else {
        emb->size = size; emb->free = 0;
        unsigned long new_emb_pos = (unsigned long)(emb + 1) + size;
        *(EMB*)(new_emb_pos) = (EMB){.size = emb->size - size - sizeof(EMB), .free = 1,
                .nextStart = emb->nextStart, .prev = emb, .prevStart = emb->prevStart};
        if (emb->prevStart == 0) d_par->firstFreeStart = new_emb_pos;
        else emb->prevStart->nextStart = (EMB*)new_emb_pos;
    }
    return (unsigned long)(emb + 1);
}

unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	//本函数需要实现！！！
	/*功能：释放一个空间
        1.按照对应的fit的算法释放空间
        2.注意检查要释放的start~end这个范围是否在dp有效分配范围内
            返回0 没问题
            返回1 error
        3.需要考虑两个空闲且相邻的内存块的合并
	*/
    unsigned long max_len = dp + ((dPartition*)dp)->size + sizeof(dPartition);
	if (start >= max_len) return 1;
    EMB* emb = (EMB*)start - 1;
    if (emb->free) return 1;
    EMB* next = (EMB*)(start + emb->size);
    if (start + emb->size < max_len && next->free) {
        // First remove the next block.
        next->prevStart->nextStart = next->nextStart;
        next->nextStart->prevStart = next->prevStart;
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
        emb->nextStart = (EMB*)(((dPartition*)dp)->firstFreeStart);
        emb->prevStart = 0;
        emb->prev = 0;
        emb->free = 1;
        ((dPartition*)dp)->firstFreeStart = (unsigned long)emb;
    }
    return 0;
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
