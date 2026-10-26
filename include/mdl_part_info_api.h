#ifndef MDL_PART_INFO_API_H
#define MDL_PART_INFO_API_H

#include "mdl.h"

/* Read the number of attached resource parts from a borrowed PIB record. */
u16 mdlPibGetPartCount(const MdlRecord *record);

#endif /* MDL_PART_INFO_API_H */
