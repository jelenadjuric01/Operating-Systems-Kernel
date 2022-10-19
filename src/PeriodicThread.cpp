//
// Created by os on 8/21/22.
//
#include "../h/syscall_cpp.hpp"
struct Arg{
    PeriodicThread* thread;
    time_t period;
};
PeriodicThread::PeriodicThread(time_t period):Thread(wrapper,(void*)(new (Arg){this,period})),work(true){
}

void PeriodicThread::wrapper(void *arg) {
    Arg* par=(Arg*)arg;
    while(par->thread->work){
        time_sleep(par->period);
        par->thread->periodicActivation();
    }
    delete par;
}

void PeriodicThread::stopThread() {
    work=false;
}
