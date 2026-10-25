#ifndef DDS_SCR_STRING_PARAMETER_API_H
#define DDS_SCR_STRING_PARAMETER_API_H

#include "common.h"

/* Returns a borrowed read-only string for a STRING-tagged parameter, or NULL. */
const char *scrGetStrPara(s32 idx);

#endif /* DDS_SCR_STRING_PARAMETER_API_H */
