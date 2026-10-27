#ifndef DDS_SCR_ARG_READ_H
#define DDS_SCR_ARG_READ_H

#include "scr_command_parameter_index.h"

/* Read the current script argument as a float, converting supported numeric tags. */
f32 scrGetFloatPara(ScrCommandParameterIndex parameterIndex);

#endif
