#include "common.h"
#include "scr_command_parameter_index.h"
#include "scr_integer_return_value_api.h"
#include "pty_roster_api.h"



extern s32 scrGetIntPara(ScrCommandParameterIndex parameterIndex);

/* Add the party unit from script operand 0 and store its result in the VM. */
s32 ptyScriptAddUnitAndReturnResult(void) {
    scrSetIntReturnValue(ptyAddUnit(scrGetIntPara(0)));
    return 1;
}
