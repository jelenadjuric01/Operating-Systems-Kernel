//
// Created by os on 8/11/22.
//

#ifndef PROJECT_BASE_V1_1_RISCV_HPP
#define PROJECT_BASE_V1_1_RISCV_HPP
#include "../lib/hw.h"
#include "../lib/console.h"
#include "codes.hpp"
#include "tcb.hpp"
#include "TimeSleep.hpp"
#include "_console.hpp"
class Riscv {
private:

    static void handleSupervisorTrap();

    static void unexpectedTrap();

    static void syscall(uint64 code,uint64 a1,uint64 a2,uint64 a3,uint64 a4);
    static void kernelPrint(char const *string){
        while (*string != '\0')
        {
            output->cons_put((*string));
            string++;
        }
    }
    static char digits[];
    static void kernelPrintInt(int xx, int base=10, int sgn=0){
        char buf[16];
        int i, neg;
        uint x;

        neg = 0;
        if(sgn && xx < 0){
            neg = 1;
            x = -xx;
        } else {
            x = xx;
        }

        i = 0;
        do{
            buf[i++] = digits[x % base];
        }while((x /= base) != 0);
        if(neg)
            buf[i++] = '-';

        while(--i >= 0)
            output->cons_put(buf[i]);

    }

public:
    static uint64 returnValue();
    static void determinePrivilege();
    static void loadParams(uint64 *);
    static void takeArg(Codes c){
        __asm__ volatile("mv a4, a3");
        __asm__ volatile("mv a3, a2");
        __asm__ volatile("mv a2, a1");
        __asm__ volatile("mv a1, a0");
        __asm__ volatile("mv a0, %0" : : "r"(c));
        __asm__ volatile("ecall");
    }
    static void retTosret();

    static uint64 r_scause();

    static void w_scause(uint64 scause);

    static uint64 r_sepc();

    static void w_sepc(uint64 sepc);

    static uint64 r_stvec();

    static void w_stvec(uint64 stvec);

    static uint64 r_stval();

    static void w_stval(uint64 stval);

    enum BitMaskSip : uint64 {
        SIP_SSIP = (1UL << 1),
        SIP_STIP = (1UL << 5),
        SIP_SEIP = (1UL << 9),
    };

    static void ms_sip(uint64 mask);

    static void mc_sip(uint64 mask);

    static uint64 r_sip();

    static void w_sip(uint64 sip);
    static void buffersReady();
    enum BitMaskSstatus : uint64
    {
        SSTATUS_SIE=(1UL<<1),
        SSTATUS_SPIE=(1UL<<5),
        SSTATUS_SPP=(1UL<<8),
    };
    static void ms_sstatus(uint64 mask);
    static void mc_sstatus(uint64 mask);
    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);
    static void supervisorTrap();
    static _console *input,*output;

};
inline uint64 Riscv::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}
inline void Riscv::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}
inline uint64 Riscv::r_sepc() {
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc],sepc":[sepc] "=r"(sepc));
    return sepc;
}
inline void Riscv::w_sepc(uint64 sepc) {
    __asm__ volatile("csrw sepc,%[sepc]": :[sepc] "r"(sepc));
}
inline uint64 Riscv::r_stvec() {
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec],stvec" : [stvec] "=r"(stvec));
    return stvec;
}
inline void Riscv::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec,%[stvec]" : : [stvec] "r"(stvec));
}
inline uint64 Riscv::r_stval() {
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}
inline void Riscv::w_stval(uint64 stval) {
    __asm__ volatile("csrw stval,%[stval]" : : [stval] "r"(stval));
}
inline void Riscv::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip,%[mask]" : : [mask] "r"(mask));
}
inline void Riscv::mc_sip(uint64 mask) {
    __asm__ volatile ("csrc sip,%[mask]" : : [mask] "r"(mask));
}
inline uint64 Riscv::r_sip() {
    uint64 volatile sip;
    __asm__ volatile("csrr %[sip],sip" : [sip] "=r"(sip));
    return sip;
}
inline void Riscv::w_sip(uint64 sip) {
    __asm__ volatile("csrw sip,%[sip]" : : [sip] "r"(sip));
}
inline void Riscv::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus,%[mask]" : : [mask] "r"(mask));
}
inline void Riscv::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus,%[mask]" : : [mask] "r"(mask));
}
inline uint64 Riscv::r_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile("csrr %[sstatus],sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}
inline void Riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile("csrw sstatus,%[sstatus]" : : [sstatus] "r"(sstatus));
}

inline void Riscv::loadParams(uint64 *arr) {
    __asm__ volatile("mv %0, a0" : "=r"(arr[0]));
    __asm__ volatile("mv %0, a1" : "=r"(arr[1]));
    __asm__ volatile("mv %0, a2" : "=r"(arr[2]));
    __asm__ volatile("mv %0, a3" : "=r"(arr[3]));
    __asm__ volatile("mv %0, a4" : "=r"(arr[4]));
}



#endif //PROJECT_BASE_V1_1_RISCV_HPP
