// functions for osfunc.c not included in uspios.h
// INCLUDE USPIOS.H AS WELL

typedef unsigned MemoryBlock;

typedef struct MemoryPool{
    MemoryBlock *pFreeList;
    unsigned nBlocks;
    unsigned nBlockSize;
    unsigned nFreeBlocks;
    unsigned nAllocatedBlocks;
    unsigned nFailedAllocations;
    unsigned nFailedFrees;
} MemoryPool_t;

void MemoryPoolInit(unsigned nBlocks, unsigned nBlockSize);

MemoryPool_t *pMemoryPool;

char* numToString(void *num, int base, char *buf);