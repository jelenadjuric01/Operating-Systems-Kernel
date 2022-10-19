//
// Created by os on 8/11/22.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP
#include "../lib/hw.h"
#include "../h/codes.hpp"
//#include "riscv.hpp"

class TCB;
typedef TCB* thread_t;
class Sem;
typedef Sem* sem_t;
extern void* mem_alloc(size_t size);

extern int mem_free(void*);
extern void userMod();
extern int thread_create(thread_t* handle,void (*routine)(void*),void* arg);
extern int thread_make(thread_t* handle,void (*routine)(void*),void* arg);
extern int thread_arg(thread_t* handle,void (*routine)(void*),void* arg,Codes code);
extern int thread_exit();
extern void thread_dispatch();
extern int sem_open(sem_t* handle,unsigned init);
extern int sem_close(sem_t handle);
extern int sem_wait(sem_t id);
extern int sem_signal(sem_t id);
extern int time_sleep(time_t);
extern char getc();
extern void putc(char);
extern int getThreadId();
extern void blockInter();
extern void unblockInter();
#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
