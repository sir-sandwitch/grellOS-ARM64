#include "uspi/include/uspios.h"
#include "fb.h"
#include "osfunc.h"
#include "kernel.h"
#include "irqlist.h"

void MemoryPoolInit(unsigned nBlocks, unsigned nBlockSize){
    // initialize a memory pool
    void *ptr;
    void *pFreeList[nBlocks];
    for (int i = 0; i < nBlocks; i++){
        pFreeList[i] = ptr;
    }
    pMemoryPool->nBlocks = nBlocks;
    pMemoryPool->nBlockSize = nBlockSize;
    pMemoryPool->nFreeBlocks = nBlocks;
    pMemoryPool->pFreeList = pFreeList;
    pMemoryPool->nAllocatedBlocks = 0;
    pMemoryPool->nFailedAllocations = 0;
    pMemoryPool->nFailedFrees = 0;
}

void *malloc(unsigned nSize){
    // allocate nSize bytes of memory (4 byte aligned)
    // return a pointer to the allocated memory

    if(nSize > pMemoryPool->nBlockSize){
        //alloc multiple blocks
        int nBlocks = nSize / pMemoryPool->nBlockSize;
        if(nSize % pMemoryPool->nBlockSize != 0){
            nBlocks++;
        }
        if(nBlocks > pMemoryPool->nFreeBlocks){
            pMemoryPool->nFailedAllocations++;
            return 0;
        }
        void *ptr;
        for(int i = 0; i < nBlocks; i++){
            pMemoryPool->pFreeList[i+pMemoryPool->nAllocatedBlocks] = ptr;
        }
        pMemoryPool->nAllocatedBlocks += nBlocks;
        pMemoryPool->nFreeBlocks -= nBlocks;
        return ptr;
    }
    else{
        //alloc single block
        if(pMemoryPool->nFreeBlocks == 0){
            pMemoryPool->nFailedAllocations++;
            return 0;
        }
        void *ptr = pMemoryPool->pFreeList[pMemoryPool->nAllocatedBlocks];
        pMemoryPool->nAllocatedBlocks++;
        pMemoryPool->nFreeBlocks--;
        return ptr;
    }
}

void free(void *pBlock){
    // free a block of memory allocated with malloc
    // pBlock must be a pointer returned by malloc
    // pBlock may be 0 (in which case free does nothing)
    if(pBlock == 0){
        return;
    }
    pMemoryPool->pFreeList[pMemoryPool->nAllocatedBlocks] = pBlock;
    pMemoryPool->nAllocatedBlocks--;
    pMemoryPool->nFreeBlocks++;
}

void MsDelay(unsigned nMilliSeconds){
    // delay for nMilliSeconds milliseconds

    //use timer 3
    //timer is set to 0.25MHz

    if(nMilliSeconds == 0){
        return;
    }
    unsigned int initVal = timer3_val;
    unsigned int delay = nMilliSeconds * 250;
    if(delay > ((1<<sizeof(int))-timer3_val)){
        //delay overruns timer, need to store remainder
        unsigned int remainder = delay - ((1<<sizeof(int))-timer3_val);
        while(timer3_val < (1<<sizeof(int)));
        while(timer3_val < remainder);
    }
    else{
        while(timer3_val - initVal < delay);
    }
}

void usDelay(unsigned nMicroSeconds){
    // delay for nMicroSeconds microseconds

    //use timer 1
    //timer is set to 1MHz

    if(nMicroSeconds == 0){
        return;
    }

    unsigned int initVal = timer1_val;
    unsigned int delay = nMicroSeconds;

    if(delay > ((1<<sizeof(int))-timer1_val)){
        //delay overruns timer, need to store remainder
        unsigned int remainder = delay - ((1<<sizeof(int))-timer1_val);
        while(timer1_val < (1<<sizeof(int)));
        while(timer1_val < remainder);
    }
    else{
        while(timer1_val - initVal < delay);
    }
}

unsigned StartKernelTimer(unsigned nHzDelay, TKernelTimerHandler *pHandler, void *pParam, void *pContext){
    // start a kernel timer
    // return the timer handle (hTimer)

}

void CancelKernelTimer(unsigned hTimer){
    // cancel a kernel timer
}

void ConnectInterrupt(unsigned nIRQ, TInterruptHandler *pHandler, void *pParam){
    // connect an interrupt handler to an IRQ
    // USPi uses IRQ 9 for USB
    funcPointer = pHandler;
    argPointer = pParam;
}

int SetPowerStateOn(unsigned nDeviceId){
    // set power state to on
    // wait until completed
    // return 0 on failure
}

int GetMACAddress(unsigned char Buffer[6]){
    // get board MAC address
    // return 0 on failure
}

void LogWrite(const char *pSource, unsigned int nSeverity, const char *pMessage, ...){
    // write a message to the log
}

// display "assertion failed" message and halt
void uspi_assertion_failed (const char *pExpr, const char *pFile, unsigned nLine){

}

// display hex dump (pSource can be 0)
void DebugHexdump (const void *pBuffer, unsigned nBufLen, const char *pSource /* = 0 */){

}
