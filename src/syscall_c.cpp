//
// Created by os on 8/11/22.
//
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
/*
inline void takeArg(Codes c){
    __asm__ volatile("mv %0, %1" : "=r"(arr[0]): "r"(c));
    __asm__ volatile("mv %0, a0" : "=r"(arr[1]));
    __asm__ volatile("mv %0, a1" : "=r"(arr[2]));
    __asm__ volatile("mv %0, a2" : "=r"(arr[3]));
    __asm__ volatile("mv %0, a3" : "=r"(arr[4]));
    __asm__ volatile("ecall");
}
inline void takeArg(Codes c){
    __asm__ volatile("mv a4, a3");
    __asm__ volatile("mv a3, a2");
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(c));
    __asm__ volatile("ecall");
}
*/
void *mem_alloc(size_t size) {
    if(!size) return nullptr;
    size_t blocks;
    if((size+ sizeof(MemoryAllocator::MemDescr))% MEM_BLOCK_SIZE==0) blocks=(size+ sizeof(MemoryAllocator::MemDescr))/MEM_BLOCK_SIZE;
    else blocks=((size+ sizeof(MemoryAllocator::MemDescr))/MEM_BLOCK_SIZE)+1;
    __asm__ volatile("mv a0,%0" : : "r"(blocks));
    Riscv::takeArg(MEM_ALLOC);
    uint64 ret;
    __asm__ volatile("mv %0,a0" : "=r"(ret));
    return (void*)ret;
}

int mem_free(void * adr) {
    if(!adr) return -1;
    Riscv::takeArg(MEM_FREE);
    return Riscv::returnValue();
}
void userMod(){
    Riscv::takeArg(USER_MOD);
}
int thread_arg(Codes code,thread_t *handle, void (*routine)(void *), void *arg) {
    Riscv::takeArg(code);
    uint64 ret;
    __asm__ volatile("mv %0,a0" : "=r"(ret));
    return ret;
}

int thread_create(thread_t* handle, void (*routine)(void *), void *arg) {
    if(!handle) return -1;
   void* stack=nullptr;
   stack = mem_alloc(DEFAULT_STACK_SIZE* sizeof(uint64));
   if(!stack) return -2;
    return TCB::threadCreate(handle,routine,arg,stack);

}

int thread_exit() {
    Riscv::takeArg(THREAD_EXIT);
    return Riscv::returnValue();
}

void thread_dispatch() {
    Riscv::takeArg(THREAD_DISPATCH);
    printString("");
}
int sem_open(sem_t *handle, unsigned init) {
    if(!handle)
        return -1;

    Riscv::takeArg(SEM_OPEN);
    return Riscv::returnValue();
}

int sem_close(sem_t handle) {
    if(!handle)
        return -1;
    Riscv::takeArg(SEM_CLOSE);
    return Riscv::returnValue();
}
int sem_wait(sem_t id) {
    if(!id) return -1;
    Riscv::takeArg(SEM_WAIT);
    printString("");
    return Riscv::returnValue();
}

int sem_signal(sem_t id) {
    if(id==nullptr) return -1;
    Riscv::takeArg(SEM_SIGNAL);
    return Riscv::returnValue();
}
int thread_make(thread_t *handle, void (*routine)(void *), void *arg) {
    if(!handle) return -1;
    void* stack=nullptr;
    stack = mem_alloc(DEFAULT_STACK_SIZE* sizeof(uint64));
    if(!stack) return -2;
    return TCB::threadMake(handle,routine,arg,stack);
}

int time_sleep(time_t time) {
    Riscv::takeArg(TIME_SLEEP);
    printString("");
    return Riscv::returnValue();
}

char getc() {
    Riscv::takeArg(GETC);
    return (char)Riscv::returnValue();
}

void putc(char c) {
    Riscv::takeArg(PUTC);

}
int getThreadId(){
    Riscv::takeArg(GET_ID);
    //printString("");
    return Riscv::returnValue();
}
void blockInter(){
    Riscv::takeArg(BLOCK);
}
void unblockInter(){
    Riscv::takeArg(UNBLOCK);
}