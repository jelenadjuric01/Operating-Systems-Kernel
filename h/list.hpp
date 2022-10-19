//
// Created by os on 8/11/22.
//

#ifndef PROJECT_BASE_V1_1_LIST_HPP
#define PROJECT_BASE_V1_1_LIST_HPP
#include "MemoryAllocator.hpp"

template<typename T>
class List{
private:
    struct Elem{
        T* data;
        Elem* next;
        time_t time;
        Elem(T* data, Elem* next): data(data), next(next),time(0){}
    };
    Elem* head,*tail;
    int size;
public:

    int getSize() const {
        return size;
    }
    List():head(0),tail(0),size(0){}
    List(const List<T>&) = delete;
    List<T>& operator=(const List<T>&)=delete;
    void addFirst(T* data){
        //Elem* elem=new Elem(data,head);
        Elem* elem=(Elem*)MemoryAllocator::mem_alloc(sizeof(Elem)+sizeof(MemoryAllocator::MemDescr));
        elem->data=data;
        elem->next=head;
        head=elem;
        if(tail) tail=head;
        size++;
    }
    void addLast(T* data){
        //Elem* elem=new Elem(data,0);
        Elem* elem=(Elem*)MemoryAllocator::mem_alloc(sizeof(Elem)+sizeof(MemoryAllocator::MemDescr));
        elem->data=data;
        elem->next=nullptr;
        if(tail){
            tail->next=elem;
            tail=elem;
        }
        else{
            head=tail=elem;
        }
        size++;
    }
    T* removeFirst(){
        if(!head) return 0;
        Elem* elem=head;
        head=head->next;
        if(!head) tail=0;
        T* ret=elem->data;
        //delete elem;
        MemoryAllocator::mem_free(elem);
        size--;
        return ret;
    }
    T* peekFirst(){
        if(!head) return 0;
        return head->data;
    }
    T* removeLast(){
        if(!head) return 0;
        Elem* prev=0;
        for(Elem* cur=head;cur && cur!=tail; cur=cur->next){
            prev=cur;
        }
        Elem* elem=tail;
        if(prev) { prev->next=0;}
        else{ head=0;}
        tail=prev;
        T* ret = elem->data;
        //delete elem;
        MemoryAllocator::mem_free(elem);
        size--;
        return ret;
    }
    T* peekLast(){
        if(!tail) return 0;
        return tail->data;
    }


};
#endif //PROJECT_BASE_V1_1_LIST_HPP
