#ifndef KWLN_DRAW_LIGHT_BINDING_H
#define KWLN_DRAW_LIGHT_BINDING_H

#include "common.h"

/* Install caller-owned mutable light data in a draw slot. The draw table
 * retains this address without copying or freeing it, and light-update code
 * may write through it. The source must remain valid while it is installed. */
void dds3SetLight(f32 (*lightSource)[4], s32 slot);

#endif /* KWLN_DRAW_LIGHT_BINDING_H */
