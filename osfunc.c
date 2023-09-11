#ifndef uspios_h
#include "uspi/include/uspios.h"
#define uspios_h
#endif
#ifndef fb_h
#include "fb.h"
#define fb_h
#endif
#ifndef osfunc_h
#include "osfunc.h"
#define osfunc_h
#endif
#ifndef kernel_h
#include "kernel.h"
#define kernel_h
#endif
// #ifndef irqlist_h
// #include "irqlist.h"
// #define irqlist_h
// #endif
#ifndef mb_h
#include "mb.h"
#define mb_h
#endif
#ifndef io_h
#include "io.h"
#define io_h
#endif

void (*funcPointer)(void*);
void (*argPointer);
MemoryPool_t *pMemoryPool; //only defined here, sorry :( call extern in other files i guess

void MemoryPoolInit(unsigned nBlocks, unsigned nBlockSize){
    // initialize a memory pool
    void *pFreeList[nBlocks];
    for (int i = 0; i < nBlocks; i++){
        pFreeList[i] = 0;
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
            //check if there are any freed blocks
            for(int i = 0; i < pMemoryPool->nBlocks; i++){
                if(pMemoryPool->pFreeList[i] == 0){
                    void *ptr;
                    pMemoryPool->pFreeList[i] = ptr;
                    pMemoryPool->nAllocatedBlocks++;
                    pMemoryPool->nFreeBlocks--;
                    return ptr;
                }
            }
            //no free blocks, return 0
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
    // sets the memory to 0

    //check if pBlock is a valid pointer
    int valid = 0;
    int validAddr = 0;
    for(int i = 0; i < pMemoryPool->nBlocks; i++){
        if(pMemoryPool->pFreeList[i] == pBlock){
            valid = 1;
            validAddr = i;
            break;
        }
    }
    if(valid == 0){
        pMemoryPool->nFailedFrees++;
        return;
    }
    //free block
    pMemoryPool->pFreeList[validAddr] = 0;
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
    // set power state of device to on
    // wait until completed
    // return 0 on failure

    //prepare mailbox message
    mbox[0] = 8 * 4; //length of message
    mbox[1] = MBOX_REQUEST; //message type
    mbox[2] = MBOX_TAG_SETPOWER; //tag
    mbox[3] = 8; //tag data length
    mbox[4] = 0; //tag data size
    mbox[5] = nDeviceId; //device id
    mbox[6] = 1; //device state
    mbox[7] = 0; //end tag

    for(int i = 8; i < 36; i++){
        mbox[i] = 0;
    }

    //send message
    mbox_call(MBOX_CH_PROP);


}

//implement this
int GetMACAddress(unsigned char Buffer[6]){
    // get board MAC address
    // return 0 on failure

}

void LogWrite(const char *pSource, unsigned int nSeverity, const char *pMessage, ...){
    // write a message to the log
    uart_writeText(pSource);
    uart_writeText(pMessage);
}

// display "assertion failed" message and halt
void uspi_assertion_failed (const char *pExpr, const char *pFile, unsigned nLine){
    //display assertion failed message
    uart_writeText("Assertion failed: ");
    uart_writeText(pExpr);
    uart_writeText(" in ");
    uart_writeText(pFile);
    uart_writeText(" at line ");
    uart_writeText(numToString(&nLine, 10, 0));
    uart_writeText("\n");
    //halt
    while(1);
}

char* numToString(void *num, int base, char *buf){

    char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i = 0;
    int j = 0;

    int number = (int*)num;

    if(number == 0){
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }
    else{
        while(number != 0){
            buf[i] = digits[number % base];
            number /= base;
            i++;
        }
        buf[i] = '\0';
        i--;
        while(j < i){
            char temp = buf[j];
            buf[j] = buf[i];
            buf[i] = temp;
            j++;
            i--;
        }
        return buf;
    }
}


// display hex dump (pSource can be 0)
void DebugHexdump (const void *pBuffer, unsigned nBufLen, const char *pSource /* = 0 */){
    //display hex dump
    uart_writeText(pSource);
    uart_writeText("\n");
    for(int i = 0; i < nBufLen; i++){
        //convert to string in format hex 00        
        char *buf;
        numToString(&pBuffer[i], 16, buf);
        uart_writeText(buf);
        uart_writeText(" ");
    }
    uart_writeText("\n");
}
