#include "common.h"
#include "scr_command_parameter_index.h"
#include "scr_integer_return_value_api.h"
#include "pty_roster_api.h"

extern s32 scrGetIntPara(ScrCommandParameterIndex parameterIndex);

/* Persona 4 func_00298d70 @ 00298D70 (src/promoted/code1_0029.c), recompiled unchanged */
s32 ptyScriptAddUnitAndReturnResult(void) {
    DatUnitId unitId = scrGetIntPara(0);

    scrSetIntReturnValue(ptyAddUnit(unitId));
    return 1;
}
