//
// Created by os on 9/2/22.
//

#ifndef PROJECT_BASE_V1_1_MOD_HPP
#define PROJECT_BASE_V1_1_MOD_HPP

#include "../h/syscall_cpp.hpp"

#include "printing.hpp"
bool finishedA = false;
bool finishedB = false;
bool finishedC = false;


class WorkerA: public PeriodicThread {

public:
    WorkerA():PeriodicThread(5) {}

    void periodicActivation() override {
        int id=getThreadId();
        printString("A: i=");
        printInt(id);
        printString("\n");
    }
};

class WorkerB: public PeriodicThread {

public:
    WorkerB():PeriodicThread(5) {}

    void periodicActivation() override {
        int id=getThreadId();
        printString("B: i=");
        printInt(id);
        printString("\n");
    }
};

class WorkerC: public PeriodicThread {

public:
    WorkerC():PeriodicThread(5) {}

    void periodicActivation() override {
        int id=getThreadId();
        printString("C: i=");
        printInt(id);
        printString("\n");
    }
};



void Threads() {
    PeriodicThread* threads[3];
    threads[0] = new WorkerB();
    printString("ThreadB created\n");

    threads[2] = new WorkerA();
    printString("ThreadA created\n");

    threads[1] = new WorkerC();
    printString("ThreadC created\n");





    for(int i=0; i<3; i++) {
        threads[i]->start();
    }
    char k;
    for(int i=0;i<3;i++) {
        while ((k = getc()) != 'k') {
            Thread::dispatch();
        }
        threads[i]->stopThread();
    }
    for (auto thread: threads) { delete thread; }
}


#endif //PROJECT_BASE_V1_1_MOD_HPP
