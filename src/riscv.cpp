//
// Created by os on 8/11/22.
//

#include "../h/riscv.hpp"


_console *Riscv::input =nullptr;
_console *Riscv::output =nullptr;
char Riscv::digits[]="0123456789ABCDEF";
enum InterCodes: uint64
{
    SOFT= 0x8000000000000001UL,
    ECALL_S=0x0000000000000009UL,
    ECALL_U=0x0000000000000008UL,
    HARD= 0x8000000000000009UL
};

void Riscv::syscall(uint64 code,uint64 a1,uint64 a2,uint64 a3,uint64 a4) {

    //__asm__ volatile("mv %0,a0" : "=r"(code));
    switch(code){
        case MEM_ALLOC:{
            size_t size=a1*MEM_BLOCK_SIZE;
            TCB::running->context.a0=(uint64)MemoryAllocator::mem_alloc(size);
        }
        break;
        case MEM_FREE:{
            TCB::running->context.a0=MemoryAllocator::mem_free((void*)a1);
        }
        break;
        case USER_MOD:{
            mc_sstatus(SSTATUS_SPP);
        }
        break;
        case THREAD_CREATE:
        {
            thread_t* handle=(thread_t*)a1;
            TCB::Body body=(TCB::Body)a2;
            void* arg=(void*)a3;
            void* stack=(void*)a4;
            *handle = TCB::createConst(body,arg,stack);
            (*handle)->startThread();
            TCB::running->context.a0=(*handle==0?-1:0);

        }
        break;
        case THREAD_MAKE:{
            thread_t* handle=(thread_t*)a1;
            TCB::Body body=(TCB::Body)a2;
            void* arg=(void*)a3;
            void* stack=(void*)a4;
            *handle = TCB::createConst(body,arg,stack);
            TCB::running->context.a0=(*handle==0?-1:0);
        }
        break;
        case THREAD_EXIT:
        {
            TCB::running->context.a0=TCB::thread_exit();
        }
        break;
        case THREAD_DISPATCH:{
            uint64 sepc=r_sepc();
            TCB::running->context.sepc=sepc;
            uint64 sstatus=r_sstatus();
            TCB::running->context.sstatus=sstatus;
            //TCB::timeSliceCounter=0;
            TCB::dispatch();
            w_sepc(TCB::running->context.sepc);
            w_sstatus(TCB::running->context.sstatus);

        }
        break;
        case SEM_OPEN:
        {
            sem_t* handle=(sem_t*)a1;
            unsigned init=(unsigned)a2;
            *handle=new Sem(init);
            TCB::running->context.a0=0;

        } break;
        case SEM_CLOSE:
        {
            sem_t handle=(sem_t)a1;
            TCB::running->context.a0=handle->close();
        } break;
        case SEM_SIGNAL:
        {
            sem_t id=(sem_t)a1;
            TCB::running->context.a0=id->signal();
        }
        break;
        case SEM_WAIT:
        {
            sem_t id=(sem_t)a1;
            uint64 sepc=r_sepc();
            TCB::running->context.sepc=sepc;
            uint64 sstatus=r_sstatus();
            TCB::running->context.sstatus=sstatus;
            //TCB::timeSliceCounter=0;
            TCB::running->context.a0=id->wait();
            w_sepc(TCB::running->context.sepc);
            w_sstatus(TCB::running->context.sstatus);
        }
        break;
        case TIME_SLEEP:
        {
            uint64 sepc=r_sepc();
            TCB::running->context.sepc=sepc;
            uint64 sstatus=r_sstatus();
            TCB::running->context.sstatus=sstatus;
            //TCB::timeSliceCounter=0;
            TCB::blockThread((time_t)a1);
            w_sepc(TCB::running->context.sepc);
            w_sstatus(TCB::running->context.sstatus);
            TCB::running->context.a0=0;
        }
        break;
        case GETC:
        {
            uint64 sepc=r_sepc();
            TCB::running->context.sepc=sepc;
            uint64 sstatus=r_sstatus();
            TCB::running->context.sstatus=sstatus;
            char c=input->cons_get();
            TCB::running->context.a0=(uint64)c;
            w_sepc(TCB::running->context.sepc);
            w_sstatus(TCB::running->context.sstatus);
            __asm__ volatile("mv a0,%0": : "r"((uint64)c));
        }
        break;
        case PUTC:
        {
            uint64 sepc=r_sepc();
            TCB::running->context.sepc=sepc;
            uint64 sstatus=r_sstatus();
            TCB::running->context.sstatus=sstatus;
            output->cons_put((char)a1);
            w_sepc(TCB::running->context.sepc);
            w_sstatus(TCB::running->context.sstatus);

        } break;
        case GET_ID:
        {
            uint64 sepc=r_sepc();
            TCB::running->context.sepc=sepc;
            uint64 sstatus=r_sstatus();
            TCB::running->context.sstatus=sstatus;
            //TCB::timeSliceCounter=0;
            TCB::dispatch();
            w_sepc(TCB::running->context.sepc);
            w_sstatus(TCB::running->context.sstatus);
            TCB::running->context.a0=TCB::running->id;
        } break;
        case BLOCK:
        {
            mc_sstatus(SSTATUS_SPIE);
        }
        break;
        case UNBLOCK:
        {
            ms_sstatus(SSTATUS_SPIE);
        }break;
        default:{
            printString("Invalid code\n");
        }
        break;
    }
}


void Riscv::handleSupervisorTrap() {
    uint64  code,a1,a2,a3,a4;
    __asm__ volatile("mv %0, a0" : "=r"(code));
    __asm__ volatile("mv %0, a1" : "=r"(a1));
    __asm__ volatile("mv %0, a2" : "=r"(a2));
    __asm__ volatile("mv %0, a3" : "=r"(a3));
    __asm__ volatile("mv %0, a4" : "=r"(a4));
    uint64 scause=r_scause();
    if(scause==ECALL_S || scause==ECALL_U){
        //ecall iz supervisor rezima (ecall u sepc stavlja tu adresu, u sepc treba
        //dodati 4)
        uint64 sepc=r_sepc();
        sepc+=4;
        w_sepc(sepc);
        syscall(code,a1,a2,a3,a4);
        //printString("Prekid iz super rezima\n");
    }
    else if(scause==SOFT){
        //timer

        TCB::timeSliceCounter++;
        TimeSleep::getInstance()->released();
        if(TCB::timeSliceCounter>=TCB::running->timeSlice)
        {
            uint64 sepc=r_sepc();
            TCB::running->context.sepc=sepc;
            uint64 sstatus=r_sstatus();
            TCB::running->context.sstatus=sstatus;
            //TCB::timeSliceCounter=0;
            TCB::dispatch();
            w_sepc(TCB::running->context.sepc);
            w_sstatus(TCB::running->context.sstatus);
        }

        mc_sip(SIP_SSIP);
    }
    else if(scause==HARD){
        //prekid konzole
        int id=plic_claim();
        if(id==CONSOLE_IRQ){
            volatile char* stat=(char*)CONSOLE_STATUS;
            while((*stat&CONSOLE_RX_STATUS_BIT) && input->getSize()<256){
                char c=(char)(*((char*)CONSOLE_RX_DATA));
                input->cons_put(c);
                stat=(char*)CONSOLE_STATUS;
            }

        }
        plic_complete(CONSOLE_IRQ);
    }
    else{
        unexpectedTrap();
    }

}

void Riscv::retTosret() {

    __asm__ volatile ("csrw sepc,ra");
    __asm__ volatile ("sret");
}
void Riscv::determinePrivilege() {
    if(TCB::running->sys){
        ms_sstatus(SSTATUS_SPP);
    }
    else{
        mc_sstatus(SSTATUS_SPP);
    }
}

inline void Riscv::unexpectedTrap() {
    uint64 scause= r_scause();
    uint64 sepc= r_sepc();
    uint64  stval=r_stval();
    uint64 stvec= r_stvec();
    kernelPrint("scause: ");
    kernelPrintInt(scause,16);
    kernelPrint("\n");
    kernelPrint("stval: ");
    kernelPrintInt(stval,16);
    kernelPrint("\n");
    kernelPrint("stvec: ");
    kernelPrintInt(stvec,16);
    kernelPrint("\n");
    kernelPrint("sepc: ");
    kernelPrintInt(sepc,16);
    kernelPrint("\n");
}
void Riscv::buffersReady() {
    input=new _console();
    output=new _console();
}

uint64 Riscv::returnValue() {
    uint64 volatile ret;
    __asm__ volatile("mv %0,a0" : "=r"(ret));
    return ret;
}