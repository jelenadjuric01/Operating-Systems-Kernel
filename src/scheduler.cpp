//
// Created by os on 8/12/22.
//

#include "../h/scheduler.hpp"


TCB *Scheduler::get() {
    if(!readyThreads.peekFirst()){
        //napraviti idle nit
        return nullptr;
    }
    else{
        return readyThreads.removeFirst();
    }
}

void Scheduler::put(TCB *tcb) {
    readyThreads.addLast(tcb);
}

Scheduler *Scheduler::getInstance() {
    static Scheduler instance;
    return &instance;
}
