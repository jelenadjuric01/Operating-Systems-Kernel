//
// Created by os on 8/11/22.
//

#ifndef PROJECT_BASE_V1_1_CODES_HPP
#define PROJECT_BASE_V1_1_CODES_HPP
#include "../lib/hw.h"
enum Codes: uint64
{
    MEM_ALLOC=0x01,
    MEM_FREE=0x02,
    USER_MOD=0x03,
    GET_ID=0x04,
    BLOCK=0x05,
    UNBLOCK=0x06,
    THREAD_CREATE=0x11,
    THREAD_EXIT=0x12,
    THREAD_DISPATCH=0x13,
    THREAD_MAKE=0x14,
    SEM_OPEN=0x21,
    SEM_CLOSE=0x22,
    SEM_WAIT=0x23,
    SEM_SIGNAL=0x24,
    TIME_SLEEP=0x31,
    GETC=0x41,
    PUTC=0x42
};
#endif //PROJECT_BASE_V1_1_CODES_HPP
