#include "common.h"
#include "scr_integer_return_value_api.h"
#include "pty_roster_api.h"

/* Persona 4 func_00298d70 @ 00298D70 (src/promoted/code1_0029.c), recompiled unchanged */
s32 ptyScriptAddUnitAndReturnResult(void) {
    scrSetIntegerReturnValue(ptyAddUnit(scrReadIntParameter(0)));
    return 1;
}
