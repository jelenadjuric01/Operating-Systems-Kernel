//
// Created by os on 8/13/22.
//
#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.hpp"

void* operator new(uint64 n){
    return mem_alloc(n+ sizeof(MemoryAllocator::MemDescr));
}
void* operator new[](uint64 n){
    return mem_alloc(n+ sizeof(MemoryAllocator::MemDescr));
}
void operator delete(void* p) noexcept{
    mem_free(p);
}
void operator delete[](void* p) noexcept{
    mem_free(p);
}