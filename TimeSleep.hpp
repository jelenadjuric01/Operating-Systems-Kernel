//
// Created by os on 8/21/22.
//

#ifndef PROJECT_BASE_V1_1_TIMESLEEP_HPP
#define PROJECT_BASE_V1_1_TIMESLEEP_HPP
#include "list.hpp"
#include "scheduler.hpp"
class TCB;
class TimeSleep {
private:
    TimeSleep(){}
    struct Elem{
        TCB* data;
        Elem* next;
        time_t time;
        Elem(TCB* data, Elem* next): data(data), next(next),time(0){}
    };
    Elem* head,*tail;
    int size;
    void addTime(TCB* data,time_t t) {
        Elem *elem = (Elem *) MemoryAllocator::mem_alloc(sizeof(Elem) + sizeof(MemoryAllocator::MemDescr));
        elem->data = data;
        elem->time = t;
        elem->next = nullptr;
        size++;
        if (tail) {
            tail->next = elem;
            tail = elem;
        } else {
            head = tail = elem;
        }
    }
    void sub(){
        for(Elem* cur=head;cur!=nullptr;cur=cur->next) if(cur->time>0) cur->time=cur->time-1;
    }
    TCB* remove(Elem* prev,Elem* cur){
        size--;
        if(cur==head){

            head=head->next;
            if(!head) tail=nullptr;
        }
        else if(cur==tail){
            prev->next=nullptr;
            tail=prev;
        }
        else{
            prev->next=cur->next;
        }
        TCB* ret=cur->data;
        MemoryAllocator::mem_free(cur);
        return ret;
    }
    TCB* release(){
        for(Elem* cur=head,*prev=0;cur!=nullptr;prev=cur,cur=cur->next){
            if(cur->time<=0)
                return remove(prev,cur);

        }
        return nullptr;
    }
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
    void released();
    void put(TCB* tcb,time_t);
    static TimeSleep* getInstance();
    TimeSleep(const TimeSleep&)=delete;
    TimeSleep& operator=(const TimeSleep&)=delete;

};


#endif //PROJECT_BASE_V1_1_TIMESLEEP_HPP
