#include "common.h"



/* Add the party unit from script operand 0 and store its result in the VM. */
s32 ptyScriptAddUnitAndReturnResult(void) {
    scrSetIntegerReturnValue(ptyAddUnit(scrReadIntParameter(0)));
    return 1;
}
