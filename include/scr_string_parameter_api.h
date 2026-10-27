#ifndef DDS_SCR_STRING_PARAMETER_API_H
#define DDS_SCR_STRING_PARAMETER_API_H

#include "scr_command_parameter_index.h"

/* Returns a borrowed read-only string for a STRING-tagged parameter, or NULL. */
const char *scrGetStrPara(ScrCommandParameterIndex parameterIndex);

#endif /* DDS_SCR_STRING_PARAMETER_API_H */
