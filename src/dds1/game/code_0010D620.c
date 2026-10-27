#include "common.h"
#include "scr_integer_parameter_api.h"
#include "scr_integer_return_value_api.h"
#include "scr_command_timer_api.h"
#include "scr.h"
#include "scr_program_counter_api.h"
#include "scr_current_process_user_data_api.h"
#include "mdl_flag_api.h"

extern s8 D_00398629[];

ScrProgramCounter scrGetProcedureCodeAddress(ScrProcedureIndex procedureIndex) {
    return nowpwork->procedures[procedureIndex].addr;
}

ScrProgramCounter scrGetLabelCodeAddress(ScrLabelIndex labelIndex) {
    return nowpwork->labels[labelIndex].addr;
}

ScrProgramCounter scrGetCodePointer(void) {
    return nowpwork->pc;
}

void scrSetCodePointer(ScrProgramCounter programCounter) {
    nowpwork->pc = programCounter;
}

u32 scrGetScriptTimer(void) {
    return nowpwork->timer;
}

ScrCommandYieldCount scrGetCommandTimer(void) {
    return nowpwork->cmdTimer;
}

ItfMesWindowIndex scrGetMessageHandle(void) {
    return nowpwork->messageWindowIndex;
}

scrProcessWork_s *scrGetScriptHandle(void) {
    return nowpwork;
}

void *scrGetCurrentProcessUserData(void) {
    return nowpwork->userData;
}

s32 scrCommand_KEY(void) {
    if (scrGetCommandTimer() == 0) {
        return 0;
    }
    if (D_00398629[0] < 0) {
        return 1;
    }
    return 0;
}

/* Evaluate a script-supplied model flag and push the test result. */
u32 scrCommand_BIT_CHK(void) {
    s32 value;

    value = scrGetIntPara(0);
    value = evtBitCheck(value);
    scrSetIntReturnValue(value);
    return 1;
}

u32 scrCommand_BIT_ON(void) {
    s32 flagId;

    flagId = scrGetIntPara(0);
    evtBitOn(flagId);
    return 1;
}

u32 scrCommand_BIT_OFF(void) {
    s32 flagId;

    flagId = scrGetIntPara(0);
    evtBitOff(flagId);
    return 1;
}
