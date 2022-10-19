.global _ZN3TCB13contextSwitchEPNS_7ContextES1_
.type _ZN3TCB13contextSwitchEPNS_7ContextES1_,@function
.extern _ZN3TCB9whatStackEv
_ZN3TCB13contextSwitchEPNS_7ContextES1_:

    #a0=&old
    #a1=&new
    //sd ra,1 * 8(a0)


    sd ra,1 * 8(a0)
    sd s2, 34*8(a0)
    addi a0,a0,-8
    sd sp,0 * 8(a0)
    addi a0,a0,8

    csrw sscratch, x31
    call _ZN3TCB9whatStackEv

    ld ra, 1* 8(a1)
    ld s2,34*8(a1)
    beqz x31, user
    addi a1,a1,-8
    ld sp, 0* 8(a1)
    addi a1,a1,8
    j end
    user:
    ld sp, 2* 8(a1)
    end:
    csrr x31,sscratch
    ret