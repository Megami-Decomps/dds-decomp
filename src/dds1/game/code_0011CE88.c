#include "common.h"
#include "scr_integer_return_value_api.h"
#include "pty_roster_api.h"



extern s32 scrReadIntParameter(s32 parameterIndex);

/* Add the party unit from script operand 0 and store its result in the VM. */
s32 ptyScriptAddUnitAndReturnResult(void) {
    scrSetIntegerReturnValue(ptyAddUnit(scrReadIntParameter(0)));
    return 1;
}
