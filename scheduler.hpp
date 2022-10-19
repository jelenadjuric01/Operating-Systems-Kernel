//
// Created by os on 8/12/22.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP
#include "list.hpp"
#include "MemoryAllocator.hpp"
class TCB;
class Scheduler {
private:
    List<TCB> readyThreads;
    Scheduler(){}
public:
    void* operator new(uint64 n){
        return MemoryAllocator::mem_alloc(n+sizeof(MemoryAllocator::MemDescr));
    }
    void* operator new[](uint64 n){
        return MemoryAllocator::mem_alloc(n+sizeof(MemoryAllocator::MemDescr));
    }
    void operator delete(void* p) noexcept{
        MemoryAllocator::mem_free(p);
    }
    void operator delete[](void* p) noexcept{
        MemoryAllocator::mem_free(p);
    }
     TCB* get();
     void put(TCB* tcb);
    static Scheduler* getInstance();
    Scheduler(const Scheduler&)=delete;
    Scheduler& operator=(const Scheduler&)=delete;
};


#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
