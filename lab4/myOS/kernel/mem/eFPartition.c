#include "libio.h"

// 一个 EEB 表示一个空闲可用的 Block
typedef struct EEB {
	unsigned long next_start;
    unsigned long free;
} EEB;	// 占 4 个字节

void showEEB(struct EEB *eeb) {
	printk(0x7,"EEB(start=0x%x, free=%d, next=0x%x)\n", eeb, eeb->free, eeb->next_start);
}

// eFPartition 是表示整个内存的数据结构
typedef struct eFPartition{
	unsigned long totalN;
    unsigned long freeN;
	unsigned long perSize;  // unit: byte
	unsigned long firstFree;
} eFPartition;	// 占 12 个字节

void show_eFPartition(struct eFPartition *efp){
	printk(0x5,"eFPartition(start=0x%x, totalN=0x%x, freeN=0x%x, perSize=0x%x, firstFree=0x%x)\n",
             efp, efp->totalN, efp->freeN, efp->perSize, efp->firstFree);
}

/*
    功能：本函数是为了方便查看和调试的。
        1、打印 eFPartition 结构体的信息，可以调用上面的 show_eFPartition 函数。
        2、遍历每一个 EEB，打印出他们的地址以及下一个空闲的 EEB 的地址（可以调用上面的函数 showEEB）
*/
void eFPartitionWalkByAddr(unsigned long efpHandler) {
    eFPartition * efp = (eFPartition*)efpHandler;
    show_eFPartition(efp);
    unsigned long eeb = efpHandler + sizeof(eFPartition);
    for (unsigned long i = 0; i < efp->totalN; ++i) {
        showEEB((EEB*)eeb);
        eeb += efp->perSize + sizeof(EEB);
    }
}

/*
    功能：计算占用空间的实际大小，并将这个结果返回
        1. 根据参数 perSize（每个大小）和 n 个数计算总大小，注意 perSize 的对齐。
            例如 perSize 是 31 字节，你想 8 字节对齐，那么计算大小实际代入的一个块的大小就是 32 字节。
        2. 同时还需要注意“隔离带” EEB 的存在也会占用 4 字节的空间。
        3. 最后别忘记加上 eFPartition 这个数据结构的大小，因为它也占一定的空间。
*/
const unsigned long align = 4;
unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n) {
	unsigned long size = (perSize / align + 1) * align + sizeof(EEB);
    return size * n + sizeof(eFPartition);
}

/*
    功能：初始化内存
        1. 需要创建一个 eFPartition 结构体，需要注意的是结构体的 perSize 不是直接传入的参数 perSize，需要对齐。
            结构体的 next_start 也需要考虑一下其本身的大小。
        2. 就是先把首地址 start 开始的一部分空间作为存储 eFPartition 类型的空间
        3. 然后再对除去 eFPartition 存储空间后的剩余空间开辟若干连续的空闲内存块，将他们连起来构成一个链。
            注意最后一块的 EEB 的 next_start 应该是 0
        4. 需要返回一个句柄，也即返回 eFPartition * 类型的数据
        注意的地方：
            1. EEB类型的数据的存在本身就占用了一定的空间。
*/
unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n) {
    unsigned long pos = start + sizeof(eFPartition);
    unsigned long size = (perSize / 4 + 1) * 4;
    *((eFPartition*)start) = (eFPartition){.totalN = n, .freeN = n, .perSize = size, .firstFree = pos};
    for (int i = 0; i < n; ++i) {
        *((EEB*)pos) = (EEB){.free = 1, .next_start = (pos + size + sizeof(EEB))};
        pos += size + sizeof(EEB);
    }
    ((EEB*)(pos - size))->next_start = 0;
    return start;
}

/*
    功能：分配一个空间
        1. 本函数分配一个空闲块的内存并返回相应的地址，EFPHandler 表示整个内存的首地址
        2. 事实上 EFPHandler 就是我们的句柄，EFPHandler 作为 eFPartition * 类型的数据，其存放了我们需要的 firstFree 数据信息
        3. 从空闲内存块组成的链表中拿出一块供我们来分配空间，并维护相应的空闲链表以及句柄
    注意的地方：
        1. EEB 类型的数据的存在本身就占用了一定的空间。
*/
unsigned long eFPartitionAlloc(unsigned long EFPHandler) {
	eFPartition* efp = (eFPartition*)EFPHandler;
    if (efp->freeN == 0) return 0; // No memory block available.
    unsigned long ans = efp->firstFree + sizeof(EEB);
    EEB* first = (EEB*)(efp->firstFree);
    first->free = 0;
    efp->firstFree = first->next_start;
    --efp->freeN;
    if (efp->freeN == 0) efp->firstFree = 0;
    return ans;
}

/*
    功能：释放一个空间
        1. mbStart 将成为第一个空闲块，EFPHandler 的 firstFree 属性也需要相应大的更新。
        2. 同时我们也需要更新维护空闲内存块组成的链表。
*/
unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart) {
	eFPartition* efp = (eFPartition*)EFPHandler;
    EEB* eeb = (EEB*)(mbStart) - 1;
    if (eeb->free) return 1; // The block is already free.
    eeb->next_start = efp->firstFree;
    eeb->free = 1;
    efp->firstFree = (unsigned long)eeb;
    ++efp->freeN;
    return 0;
}
