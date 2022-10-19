//
// Created by os on 8/19/22.
//

#include "../h/_sem.hpp"

void Sem::block() {
    TCB* old=TCB::running;
    blocked.addLast(old);
    TCB* newT=Scheduler::getInstance()->get();
    TCB::changeRunning(&old,&newT);
}

void Sem::deblock() {
    TCB* thread=blocked.removeFirst();
    Scheduler::getInstance()->put(thread);
}

Sem::~Sem() {
    close();
}

int Sem::wait() {
    if(closed)
        return -1;
    if(--val<0)
        block();
    return (closed?-1:0);
}

int Sem::signal() {
    if(closed) return -1;
    if(val++<0)
        deblock();
    return 0;
}

int Sem::close() {
    if(closed) return -1;
    val=0;
    closed=true;
    int size=blocked.getSize();
    for(int i=0;i<size;i++){
        Scheduler::getInstance()->put(blocked.removeFirst());
    }
    return 0;
}
/*
Sem *Sem::createSem(unsigned int initval) {
    Sem* sem=(Sem*)MemoryAllocator::mem_alloc(sizeof(Sem)+ sizeof(MemoryAllocator::MemDescr));
    if(!sem) return nullptr;
    sem->val=initval;
    sem->closed=false;
    //sem->blocked=(List<TCB>)MemoryAllocator::mem_alloc(sizeof(List<TCB>)+ sizeof(MemoryAllocator::MemDescr));
    sem->blocked.head=sem->blocked.tail=0;
    sem->blocked.size=0;
    return sem;
}*/
