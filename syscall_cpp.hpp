//
// Created by os on 8/19/22.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP

#include "syscall_c.h"
#include "tcb.hpp"

class Thread{
public:
    Thread(void (*body)(void*),void* arg);
    virtual ~Thread();
    int start();
    static void dispatch();
    static int sleep(time_t);
    static void wrapper(void* t);
protected:
    Thread();
    virtual void run() {}

private:
    thread_t myHandle;
};


class Semaphore{
public:
    Semaphore(unsigned init=1);
    virtual ~Semaphore();

    int wait();
    int signal();

private:
    sem_t myHandle;
};
class PeriodicThread: public Thread{
protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation(){}
public:
    void stopThread();
private:

    bool work;
    static void wrapper(void* arg);

};

class Console{
public:
    static char getc();
    static void putc(char);
};

#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
