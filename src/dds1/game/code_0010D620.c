#include "common.h"
#include "scr_integer_return_value_api.h"
#include "scr_command_timer_api.h"
#include "scr.h"
#include "mdl_flag_api.h"


extern s32 scrGetIntPara(s32 idx);


extern s8 D_00398629[];

u32 scrGetProcedureCodeAddress(s32 procedureIndex) {
    return nowpwork->procedures[procedureIndex].addr;
}

u32 scrGetLabelCodeAddress(s32 labelIndex) {
    return nowpwork->labels[labelIndex].addr;
}

u32 scrGetCodePointer(void) {
    return nowpwork->pc;
}

void scrSetCodePointer(u32 address) {
    nowpwork->pc = address;
}

u32 scrGetScriptTimer(void) {
    return nowpwork->timer;
}

u32 scrGetCommandTimer(void) {
    return nowpwork->cmdTimer;
}

u32 scrGetMessageHandle(void) {
    return nowpwork->resourceIndex;
}

scrProcessWork_s *scrGetScriptHandle(void) {
    return nowpwork;
}

void *scrGetData(void) {
    return nowpwork->actor;
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
