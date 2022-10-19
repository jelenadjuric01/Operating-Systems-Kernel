//
// Created by os on 8/21/22.
//
#include "../h/syscall_cpp.hpp"

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}
