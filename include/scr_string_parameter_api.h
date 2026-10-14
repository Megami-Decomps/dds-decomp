#ifndef DDS_SCR_STRING_PARAMETER_API_H
#define DDS_SCR_STRING_PARAMETER_API_H

#include "common.h"

/* Returns the string for a STRING-tagged current script parameter, or NULL. */
char *scrReadStringParameter(s32 idx);

#endif /* DDS_SCR_STRING_PARAMETER_API_H */
