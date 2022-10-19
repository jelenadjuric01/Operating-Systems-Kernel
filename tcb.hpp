//
// Created by os on 8/12/22.
//

#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP
#include "../lib/hw.h"
#include "scheduler.hpp"
#include "MemoryAllocator.hpp"
#include "../test/printing.hpp"
#include "syscall_c.h"
#include "../lib/mem.h"

class TCB {
public:
    ~TCB(){
        if(stack) {
            delete[] stack;
        }
        if(sstack) {
            delete[] sstack;
        }
        body=nullptr;
        sstack=stack=nullptr;
        par=nullptr;
        ssp=0;
        finished=true;
        timeSlice=0;
        context.ra=context.sp=0;
    }
    static void blockThread(time_t t);
    using Body = void (*)(void*);
    static TCB* createThread(Body body,void* par,void* stack,uint64 timeSlice=DEFAULT_TIME_SLICE);

    bool isFinished() const{
        return finished;
    }
    static void changeRunning(TCB** old,TCB** newT);
    void setFinished(bool finished){
        TCB::finished = finished;
    }
    static void yield();

    uint64 getTimeSlice() const{
        return timeSlice;
    }
    static int thread_exit();
    void setTimeSlice(uint64 timeSlice){
        TCB::timeSlice = timeSlice;
    }

    struct Context{
        uint64 x0,ra,sp,gp,tp,x5,x6,x7,s0,s1,a0,a1,a2,a3,a4,a5,a6,a7,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,x28,x29,x30,x31,sepc,sstatus,poms2;
        Context(uint64 ra,uint64 sp){
            this->ra=ra;
            this->sp=sp;
            x0=gp=tp=x5=x6=x7=s0=s1=a0=a1=a2=a3=a4=a5=a6=a7=s2=s3=s4=s5=s6=s7=s8=s9=s10=s11=x28=x29=x30=x31=sepc=sstatus=poms2=0;
        }
    };
    static TCB* running;
    void* operator new(uint64 n){

        return MemoryAllocator::mem_alloc(n+ sizeof(MemoryAllocator::MemDescr));
    }
    void* operator new[](uint64 n){
        return MemoryAllocator::mem_alloc(n+ sizeof(MemoryAllocator::MemDescr));
    }
    void operator delete(void* p) noexcept{
        MemoryAllocator::mem_free(p);
    }
    void operator delete[](void* p) noexcept{
        MemoryAllocator::mem_free(p);
    }

    static void isMain();
    static void whatStack();
    static TCB* idle;
    static void idleBody(void*);
    static int threadCreate(thread_t* handle, void (*routine)(void *), void *arg,void* stack);
    static int threadMake(thread_t* handle, void (*routine)(void *), void *arg,void* stack);
    static TCB* createConst(Body body,void* par,void* stack,uint64 timeSlice=DEFAULT_TIME_SLICE);
    static TCB* consumer;
    static void consumerBody(void*);
    void startThread();
private:
    uint64 ssp;
    Context context;
    friend class Riscv;
    friend class Thread;
    Body body;
    void* par;
    uint64* stack,*sstack;
    static int ID;
    int id=++ID;
    uint64  timeSlice;
    bool finished;
    bool sys;
    static uint64 timeSliceCounter;
    static void contextSwitch(Context *oldC, Context *newC);
    static void dispatch();
    static uint64 constexpr SSTACK_SIZE=1024;

    TCB(Body body,void* arg,uint64 *stack,uint64 timeSlice=DEFAULT_TIME_SLICE):ssp(0),context(0,0),body(body),
        stack(body !=nullptr? stack: nullptr),
        sstack(body!=nullptr?(uint64*)MemoryAllocator::mem_alloc(sizeof(uint64)*SSTACK_SIZE+ sizeof(MemoryAllocator::MemDescr)): nullptr),
        timeSlice(timeSlice),
                finished(body==idleBody)
    {
        par=arg;
        ssp=(sstack!=0?(uint64)&sstack[SSTACK_SIZE]:0);
        context.ra=((uint64)&threadWrapper);
        context.sp=(stack!= nullptr?(uint64)&stack[DEFAULT_STACK_SIZE]:0);
        sys=(body==nullptr || stack==nullptr);
    }


    static void threadWrapper();
};


#endif //PROJECT_BASE_V1_1_TCB_HPP
