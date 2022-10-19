//
// Created by os on 8/9/22.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#include "../lib/hw.h"

class MemoryAllocator {

private:
    enum Status{FREE,TAKEN};
public:
    typedef struct MemDescr{
        char* base_addr;
        MemDescr* prev,*next;
        Status stat;
        size_t size;
    } MemDescr;
private:
    static void insert(MemDescr** head, MemDescr* node);
    static void remove(MemDescr** head,MemDescr* node);
    static void tryToJoin(MemDescr* node);
public:
    static int mem_free(void* node);
    static void* mem_alloc(size_t sz);
    static void init();
    static MemDescr* free,*taken;

    MemoryAllocator()=delete;
    MemoryAllocator(const MemoryAllocator&)=delete;
    MemoryAllocator(MemoryAllocator&&)=delete;
};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
