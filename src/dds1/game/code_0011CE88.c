#include "common.h"



/* Add the party unit from script operand 0 and store its result in the VM. */
s32 func_0011CE88(void) {
    func_0010D5F0(ptyAddUnit(scrReadIntParameter(0)));
    return 1;
}
