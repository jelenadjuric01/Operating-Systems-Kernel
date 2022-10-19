//
// Created by os on 8/22/22.
//

#include "../h/_console.hpp"

_console::_console() {
    head=tail=size=0;
    spaceAval=new Sem(SIZE);
    itemAval=new Sem(0);
}

_console::~_console() {

    delete spaceAval;
    delete itemAval;
    head=tail=size=0;
}

char _console::cons_get() {
    itemAval->wait();

    char c=buffer[head++];
    head%=SIZE;
    size--;

    spaceAval->signal();
    return c;
}

void _console::cons_put(char c) {
    spaceAval->wait();

    buffer[tail++]=c;
    tail%=SIZE;
    size++;

    itemAval->signal();
}

int _console::getSize() const {
    return size;
}

char _console::sys_get() {
    sem_wait(itemAval);

    char c=buffer[head++];
    head%=SIZE;
    size--;

    sem_signal(spaceAval);
    return c;
}
