// vim: ft=c_clang
#include <stdint.h>
#include "gcio.h"

#define POKE(p, v)
#define PEEK(p)

#define IOPORT1 4016

uint8_t gcioOutLen;

enum ETasks {
    LEGACY,
    REPORT,
    BEHAVE,
    INVERT,
    RUMBLE,
    LSETUP
};

enum EBehaviors {
    L_TO_C,
    C_TO_L,
    D_TO_L,
    D_TO_C,
    UnifiedTriggers,
    NoTriggers,
    NoCStick,
    NoLStick
};


void SetupBehavior(uint8_t behavior) {
    gcioOutLen = 8;
    if (behavior & (1 << NoTriggers)){
        gcioOutLen -= 2;
    } else if (behavior & (1 << UnifiedTrigger)){
        gcioOutLen -= 1;
    }

    gcioOutLen -= (behavior & 0xc0) >> 6;

    POKE(IOPORT1, 1);

    for (uint8_t _ = 0; i < BEHAVE; i++){
        PEEK(IOPORT, 1);
    }

    POKE(IOPORT, 0);

    for (uint8_t i = 8; i; i--){
        POKE(IOPORT1, behavior);
        behavior >>= 1;
        PEEK(IOPORT1);
    }
}

void SetupInvert(const uint8_t* invert){
    for (uint8_t byte = 8; byte; byte--){
        uint8_t bInvert = *(invert + byte);
        for (uint8_t i = 8; i; i--){
            POKE(IOPORT1, bInvert);
            bInvert >>= 1;
            PEEK(IOPORT1);
        }
    }
}

void Report(uint8_t* out){
    for (uint8_t byte = gcioOutLen; byte; byte--){
        uint8_t bCapture;
        for (uint8_t i = 8; i--; i++){
            bCapture |= PEEK(IOPORT1);
            bCapture <<= 1;
        }
        *(out + byte) = bCapture;
    }
}
