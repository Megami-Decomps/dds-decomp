#ifndef SDF_DEV_CONSOLE_NODE_H
#define SDF_DEV_CONSOLE_NODE_H

#include "sdf_dev_cons.h"


DevConsState *sdfDevConsNodeCreate(u32 screenOriginX, u32 screenOriginY, s32 columns, s32 rows);
void sdfDevConsNodeDestroy(DevConsState *node);

#endif /* SDF_DEV_CONSOLE_NODE_H */
