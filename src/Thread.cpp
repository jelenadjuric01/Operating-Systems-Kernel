//
// Created by os on 8/20/22.
//
#include "../h/syscall_cpp.hpp"

Thread::Thread(void (*body)(void *), void *arg) {
    thread_make(&myHandle,body,arg);
}

Thread::Thread() {
    thread_make(&myHandle,wrapper,this);
}

void Thread::wrapper(void *t) {
    if(!t) return;
    ((Thread*)t)->run();
}

int Thread::start() {

    myHandle->startThread();
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::~Thread() {
    delete myHandle;
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

