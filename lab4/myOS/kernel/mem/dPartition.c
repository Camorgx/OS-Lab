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
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, belongs to user
	};	                           
} EMB;	// 共占8个字节

void showEMB(struct EMB * emb) {
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
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
    *emb = (EMB){.size = handle->size - sizeof(EMB), .nextStart = 0};
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
    for (EMB* emb = (EMB*)(d_par->firstFreeStart); emb; emb = (EMB*)emb->nextStart)
        showEMB(emb);
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
    EMB* emb = (EMB*)(d_par->firstFreeStart), * emb_pre;
    int flag = 0;
    for (; emb; emb = (EMB*)(emb->nextStart)) {
        if (emb->size >= size) { flag = 1; break; }
        emb_pre = emb;
    }
    if (!flag) return 0;
    if (emb->size == size) {
        if (emb == (EMB*)(d_par->firstFreeStart))
            d_par->firstFreeStart = emb->nextStart;
        else emb_pre->nextStart = emb->nextStart;
    }
    else {
        unsigned long new_emp_pos = (unsigned long)emb + size;
        EMB tmp = (EMB){.size = emb->size - size, .nextStart = emb->nextStart};
        *(EMB*)(new_emp_pos) = tmp;
        if (emb != (EMB*)(d_par->firstFreeStart)) emb_pre->nextStart = new_emp_pos;
        else d_par->firstFreeStart = new_emp_pos;
    }
    return (unsigned long)emb;
}

/*
 *r
 */
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	//本函数需要实现！！！
	/*功能：释放一个空间
	1.按照对应的fit的算法释放空间
	2.注意检查要释放的start~end这个范围是否在dp有效分配范围内
		返回0 没问题
		返回1 error
	3.需要考虑两个空闲且相邻的内存块的合并
	*/
	
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}