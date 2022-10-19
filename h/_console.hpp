//
// Created by os on 8/22/22.
//

#ifndef PROJECT_BASE_V1_1__CONSOLE_HPP
#define PROJECT_BASE_V1_1__CONSOLE_HPP
#include "_sem.hpp"
#include "MemoryAllocator.hpp"

class _console {
private:
    static constexpr int SIZE=256;
    Sem* spaceAval,*itemAval;
    char buffer[SIZE];
    int head,tail,size;
public:
    int getSize() const;

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
    _console();
    ~_console();
    char cons_get();
    void cons_put(char c);
    char sys_get();
};


#endif //PROJECT_BASE_V1_1__CONSOLE_HPP
