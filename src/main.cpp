//
// Created by os on 8/10/22.
//
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/syscall_cpp.hpp"

extern void userMain();
void userWrapper(void* sem){
    userMain();
    ((Semaphore*)sem)->signal();
}


int main(){
    MemoryAllocator::init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB::running=TCB::createConst(nullptr,nullptr,nullptr);
    Riscv::buffersReady();
    TCB::idle=TCB::createConst(TCB::idleBody,nullptr,nullptr);
    TCB::consumer=TCB::createConst(TCB::consumerBody,nullptr,nullptr);
    TCB::consumer->startThread();
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_t user;
    Semaphore* sem=new Semaphore(0);

    thread_create(&user,userWrapper,sem);

    sem->wait();

    while(Riscv::output->getSize()>0) thread_dispatch();
    delete sem;
    delete user;

    delete TCB::consumer;
    delete TCB::idle;

    delete TCB::running;



    Riscv::mc_sstatus(Riscv::SSTATUS_SPP);


    return 0;
}