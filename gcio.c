// vim: ft=c_clang
#include <stdint.h>

#define GCIO_ACK __asm__("bit $4016")
namespace gcio {
    enum TASKS : uint8_t {
        REPORT,
        BEHAVE,
        INMASK,
        INVERT,
        RUMBLE,
        END
    }

    inline static void __Restore(const TASKS task) 
        __attribute__(
                (diagnose_if(!__builtin_constant_p(task), "task must be constant", "error"))
        )
        __attribute__(
                (diagnose_if(task >= END, "task must be less than END", "error"))
        )
        {
        #pragma unroll(END - task + 1)
        for (uint8_t i = 0; i < END - task + 1; i++){
            GCIO_ACK;
        }
    }

    inline static void __SwitchTask(const TASKS task)
        __attribute__(
                (diagnose_if(!__builtin_constant_p(task), "task must be constant", "error"))
        )
        __attribute__(
                (diagnose_if(task >= END, "task must be less than END", "error"))
        )
    {
        #pragma unroll
        for (uint8_t i = 0; i < task; i++){
            GCIO_ACK;
        }
    }

    void ChangeBehavior(const uint8_t behave){
        __SwitchTask(BEHAVE);

        for (uint8_t i = 0; i < 8; i++){
            *(volatile uint8_t*)0x4016 = behave >> i;
            GCIO_ACK;
        }

        __RestoreTask(BEHAVE);
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
        __Restore(INMASK);
    }

    void ChangeInvert(const uint8_t* invert){
        __SwitchInvert(INVERT);
        __WriteQWord(mask);
        __Restore(INVERT)
    }
}
