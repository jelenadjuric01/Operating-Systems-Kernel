//
// Created by os on 8/19/22.
//

#ifndef PROJECT_BASE_V1_1__SEM_HPP
#define PROJECT_BASE_V1_1__SEM_HPP
#include "list.hpp"
#include "syscall_c.h"
#include "tcb.hpp"
#include "scheduler.hpp"


class Sem {
public:
    friend class Riscv;
    Sem(unsigned initval=1):val(initval),closed(false){}
    //static Sem* createSem(unsigned initval=1);
    ~Sem();
    int wait();
    int signal();
    int value() const{
        return val;
    }
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

    int close();
protected:
    void block();
    void deblock();
    int val;
private:
    List<TCB> blocked;
    bool closed;
};



#endif //PROJECT_BASE_V1_1__SEM_HPP
