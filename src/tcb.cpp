//
// Created by os on 8/12/22.
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

extern "C" void saveRegisters(TCB::Context* context);
extern "C" void returnRegisters(TCB::Context* context);

TCB* TCB::running=nullptr;
TCB* TCB::idle=nullptr;
TCB* TCB::consumer=nullptr;
uint64 TCB::timeSliceCounter=0;
int TCB::ID=0;
void TCB::yield() {

    thread_dispatch();
    //printString("");
}

void TCB::dispatch() {
    TCB* old=running;
    TCB* newT=Scheduler::getInstance()->get();
    if(!old->isFinished()){
        Scheduler::getInstance()->put(old);
    }
    changeRunning(&old,&newT);
}

TCB* TCB::createThread(TCB::Body body,void* par,void* stack,uint64 timeSlice) {
//    return new TCB(body,par,(uint64*)stack,timeSlice);

    TCB* thread=(TCB*)MemoryAllocator::mem_alloc(sizeof(TCB)+ sizeof(MemoryAllocator::MemDescr));
    thread->body=body;
    thread->par=par;
    thread->stack=(uint64*)stack;
    thread->sys=(body==nullptr || stack==nullptr);
    //thread->sstack=(uint64*)MemoryAllocator::mem_alloc(sizeof(uint64)*SSTACK_SIZE+ sizeof(MemoryAllocator::MemDescr));
    thread->sstack=(body!=nullptr?(uint64*)MemoryAllocator::mem_alloc(sizeof(uint64)*SSTACK_SIZE+ sizeof(MemoryAllocator::MemDescr)): nullptr);
    thread->finished=(body==idleBody);
    thread->timeSlice=timeSlice;
    thread->ssp=(thread->sstack!=nullptr?(uint64) &thread->sstack[SSTACK_SIZE]:0);
    thread->context.ra=((uint64) &threadWrapper);
    thread->context.sp=(thread->stack != nullptr ? (uint64) &thread->stack[DEFAULT_STACK_SIZE] : 0);
    //if(body!=nullptr && body!=idleBody) Scheduler::getInstance()->put(thread);
    return thread;
//    return new TCB(body);

}

void TCB::threadWrapper() {
    Riscv::determinePrivilege();
    Riscv::retTosret();
    running->body(running->par);
    thread_exit();
}


int TCB::thread_exit() {
    if(!running) return -1;
    running->setFinished(true);
    TCB::yield();
    return 0;
}

void TCB::isMain() {
    __asm__ volatile("mv t6,%0" : : "r"((running->sys?0:1)));
}

void TCB::idleBody(void *) {
    while(true){
        thread_dispatch();
    }

}

void TCB::whatStack() {
    __asm__ volatile("mv t6,%0" : : "r"(running->sys==false && running->context.ra==(uint64)&threadWrapper?0:1));
}

void TCB::changeRunning(TCB ** old, TCB** newT) {
    if(!(*newT)){
        running=idle;
    }
    else{
        running=*newT;
    }
    Riscv::determinePrivilege();
    TCB::timeSliceCounter=0;
    TCB::contextSwitch(&(*old)->context,&running->context);

}

void TCB::startThread() {
    if(body!=nullptr && body!=idleBody) Scheduler::getInstance()->put(this);
}

int TCB::threadCreate(thread_t *handle, void (*routine)(void *), void *arg, void *stack) {
    Riscv::takeArg(THREAD_CREATE);
    uint64 ret;
    __asm__ volatile("mv %0,a0" : "=r"(ret));
    return ret;
}

int TCB::threadMake(thread_t *handle, void (*routine)(void *), void *arg, void *stack) {
    Riscv::takeArg(THREAD_MAKE);
    uint64 ret;
    __asm__ volatile("mv %0,a0" : "=r"(ret));
    return ret;
}

TCB* TCB::createConst(TCB::Body body, void *par, void *stack, uint64 timeSlice) {
    return new TCB(body,par,(uint64*)stack,timeSlice);
}

void TCB::blockThread(time_t t) {
    TCB* old=TCB::running;
    TimeSleep::getInstance()->put(old,t);
    TCB* newT=Scheduler::getInstance()->get();
    TCB::changeRunning(&old,&newT);
}
void TCB::consumerBody(void *) {

        while(true) {
            volatile char* stat = (char*) CONSOLE_STATUS;
            while (*stat & CONSOLE_TX_STATUS_BIT){
                *((char*)CONSOLE_TX_DATA)=Riscv::output->sys_get();
                stat = (char*) CONSOLE_STATUS;
            }
        }
    }