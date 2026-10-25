#ifndef DDS3_ADMIN_CONTROL_API_H
#define DDS3_ADMIN_CONTROL_API_H

#include "common.h"

/* Read the active mode; -1 means no administration mode is active. */
s8 dds3AdminiGetActiveSequence(void);

/* Read the signed mode stored in the previous history slot. */
s8 dds3AdminiGetPrevSequence(void);

/* Set bit 1 in the active administration task's flags; that task must exist. */
void dds3AdminiExitSequence(void);

#endif /* DDS3_ADMIN_CONTROL_API_H */
