// vim: ft=c_clang
#include <stdint.h>

#define GCIO_ACK __asm__("bit $4016")
namespace gcio {
    enum TASKS : uint8_t {
        LEGACY,
        REPORT,
        BEHAVE,
        INMASK,
        INVERT,
        RUMBLE,
        LSETUP,
        END
    }

    inline static void __SwitchTask(const TASKS task)
        __attribute__(
                (diagnose_if(!__builtin_constant_p(task), "task must be constant", "error"))
        )
        __attribute__(
                (diagnose_if(task >= END, "task must be less than END", "error"))
        )
    {
        *(volatile char*)0x4016 = 1;
        #pragma unroll
        for (uint8_t i = 0; i < task; i++){
            GCIO_ACK;
        }
        *(volatile char*)0x4016 = 0;
    }

    inline void PreparePolling() {
        __SwitchTask(REPORT)
    }

    void ChangeBehavior(const uint8_t behave){
        __SwitchTask(BEHAVE);

        for (uint8_t i = 0; i < 8; i++){
            *(volatile uint8_t*)0x4016 = behave >> i;
            GCIO_ACK;
        }
    }

    inline static void __WriteQWord(const uint8_t* qword){
        for (uint8_t i = 0; i < 64; i++){
           *(volatile uint8_t*)0x4016 = behave[i >> 3] >> [i & 3];
            GCIO_ACK; 
        }
    }

    void ChangeMask(const uint8_t* mask){
        __SwitchTask(INMASK);
        __WriteQWord(mask);
    }

    void ChangeInvert(const uint8_t* invert){
        __SwitchInvert(INVERT);
        __WriteQWord(mask);
    }
}
