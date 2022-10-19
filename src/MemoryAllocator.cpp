//
// Created by os on 8/9/22.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::MemDescr* MemoryAllocator::free=nullptr;
MemoryAllocator::MemDescr* MemoryAllocator::taken=nullptr;

int MemoryAllocator::mem_free(void* n) {
    if(!n) return -1;
    MemDescr* node=(MemDescr*)((char*)n- sizeof(MemDescr));
    if(node->stat!=TAKEN) return -2;
    if(node->base_addr>HEAP_END_ADDR )
        return -3;

     if(node->base_addr<HEAP_START_ADDR) return -4;

     if( node->base_addr+node->size>=HEAP_END_ADDR) return -5;
    remove(&taken,node);
    node->stat=FREE;
    insert(&free,node);
    tryToJoin(node);
    tryToJoin(node->prev);
    //node->next=node->prev=nullptr;
    return 0;
}

void *MemoryAllocator::mem_alloc(size_t sz) {
    /*
   if((sz+ sizeof(MemDescr))%MEM_BLOCK_SIZE==0){
        sz+= sizeof(MemDescr);
    }
    else{
        sz=((sz+ sizeof(MemDescr))/MEM_BLOCK_SIZE+1)*MEM_BLOCK_SIZE;
    }*/
    if(sz%MEM_BLOCK_SIZE!=0) sz=(sz/MEM_BLOCK_SIZE+1)*MEM_BLOCK_SIZE;
    for(MemDescr* cur=free;cur;cur=cur->next){
        if(cur->size==sz){
            remove(&free,cur);
            cur->stat=TAKEN;
            insert(&taken,cur);
            return (void*)(cur->base_addr+ sizeof(MemDescr));
        }
        else if(cur->size>sz){
            remove(&free,cur);
            MemDescr* n=(MemDescr*)((char*)cur->base_addr+sz);
            n->base_addr=cur->base_addr+sz;
            n->size=cur->size-sz;
            n->stat=FREE;
            n->next=n->prev=nullptr;
            insert(&free,n);
            cur->size=sz;
            cur->stat=TAKEN;
            insert(&taken,cur);
            void* p=(void*)(cur->base_addr+sizeof(MemDescr));
            return p;
        }
    }
    return nullptr;
}

void MemoryAllocator::init() {
    free=(MemDescr*)HEAP_START_ADDR;
    free->base_addr=(char*)HEAP_START_ADDR;
    free->stat=FREE;
    free->next=free->prev=nullptr;
    free->size=(size_t)((((char*)HEAP_END_ADDR-(char*)HEAP_START_ADDR)/MEM_BLOCK_SIZE)*MEM_BLOCK_SIZE);
}

void MemoryAllocator::insert(MemoryAllocator::MemDescr **head, MemoryAllocator::MemDescr *node) {
    if(!node) return;
    if(!(*head)){
        *head=node;
        (*head)->next=(*head)->prev=nullptr;
        return;
    }
    MemDescr* cur=*head;
    if((*head)->base_addr>node->base_addr){
        node->next=*head;
        node->prev=nullptr;
        (*head)->prev=node;
        *head=node;
        return;
    }
    else {
        for (; cur->next; cur = cur->next) {
            if (cur->base_addr <= node->base_addr && cur->next->base_addr >= node->base_addr) {
                break;
            }
        }
        if(cur){
            if(cur->next){
                cur->next->prev=node;
                node->next=cur->next;
            }
            else{
                node->next=nullptr;
            }
            cur->next=node;
            node->prev=cur;
        }
    }
}

void MemoryAllocator::remove(MemoryAllocator::MemDescr **head, MemoryAllocator::MemDescr *node) {
    if(!(*head) || !node) return;
    if(*head==node){
        //if((*head)->next) (*head)->next->prev=nullptr;
        *head=(*head)->next;
        //(*head)->next=nullptr;
        if(*head) (*head)->prev=nullptr;
        node->next=node->prev=nullptr;
        return;
    }
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev =node->prev;
    }
    node->next=node->prev=nullptr;
}

void MemoryAllocator::tryToJoin(MemoryAllocator::MemDescr *node) {
    if(!node) return;
    if(node->next && (char*)node+node->size == node->next->base_addr){
        node->size+=node->next->size;
        node->next=node->next->next;
        if(node->next) node->next->prev=node;
    }
}
