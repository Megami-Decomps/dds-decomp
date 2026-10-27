#ifndef DDS_SCR_INTEGER_PARAMETER_API_H
#define DDS_SCR_INTEGER_PARAMETER_API_H

#include "scr_command_parameter_index.h"
#include "scr_vm_integer.h"

/* Read a command parameter as a signed VM integer, converting floats and
 * dereferencing global integer or float references. */
ScrVmInteger scrGetIntPara(ScrCommandParameterIndex parameterIndex);

#endif /* DDS_SCR_INTEGER_PARAMETER_API_H */
