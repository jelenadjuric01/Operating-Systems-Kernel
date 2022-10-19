//
// Created by os on 8/21/22.
//

#include "../h/TimeSleep.hpp"

void TimeSleep::released() {
    TCB* thread;
    sub();
    while(true) {
        thread=release();
        if(thread==nullptr) break;
        Scheduler::getInstance()->put(thread);
    }

}

void TimeSleep::put(TCB *tcb,time_t t) {
    addTime(tcb,t);
}

TimeSleep *TimeSleep::getInstance() {
    static TimeSleep instance;
    return &instance;
}
