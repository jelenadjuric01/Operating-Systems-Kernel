# 1 "src/supervisorTrap.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/supervisorTrap.S"
.align 4
.global _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function
.extern _ZN5Riscv20handleSupervisorTrapEv
.extern _ZN3TCB7runningE
.extern _ZN3TCB6isMainEv
_ZN5Riscv14supervisorTrapEv:

    csrw sscratch,x31
    ld x31,_ZN3TCB7runningE
    addi x31,x31,8
    .irp index, 0,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
    sd x\index, \index * 8(x31)
    .endr
    call _ZN3TCB6isMainEv
    beqz x31,secall
    csrr x31,scause
    addi x31,x31,-9
    beqz x31,secall
     ld x31,_ZN3TCB7runningE

    addi x31,x31,8

    sd x2, 2 * 8(x31)
    mv s1,x31
    csrr x31,sscratch
    sd x31, 31 * 8(s1)
    addi s1,s1,-8
    ld sp,0 * 8(s1)
    j f
    secall:
    ld x31,_ZN3TCB7runningE
    addi x31,x31,8
    mv s1,x31
    csrr x31,sscratch
    sd x31, 31 * 8(s1)
    f:
    call _ZN5Riscv20handleSupervisorTrapEv


    ld x31,_ZN3TCB7runningE

    sd sp, 0 * 8(x31)
    addi x31,x31,8


    call _ZN3TCB6isMainEv
    bnez x31,pop

    main:
    ld x31,_ZN3TCB7runningE
    addi x31,x31,8
    .irp index, 0,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
    ld x\index, \index * 8(x31)
    .endr
    j end

    pop:
    ld x31,_ZN3TCB7runningE
    addi x31,x31,8
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
    ld x\index, \index * 8(x31)
    .endr
    end:
    csrw sscratch, s1
    mv s1,x31
    ld x31, 31 * 8(s1)
    csrr s1,sscratch

    sret
