#ifndef DDS3_ADMIN_CONTROL_API_H
#define DDS3_ADMIN_CONTROL_API_H

#include "common.h"

/* Set bit 1 in the active administration task's flags; that task must exist. */
void dds3AdminSetControlFlag(void);

#endif /* DDS3_ADMIN_CONTROL_API_H */
