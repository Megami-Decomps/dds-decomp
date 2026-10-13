#ifndef FLD_NPC_MOTION_H
#define FLD_NPC_MOTION_H

#include "common.h"

/* Rows loaded from FLDALL.TBL and queried by the actor-motion attribute helper. */
typedef struct FldNpcMotion {
    s32 defaultMotionId; /* 0x00 */
    u8 primaryName[0x10]; /* 0x04 */
    u8 secondaryName[0x10]; /* 0x14 */
    u8 unk24[0x10]; /* 0x24: opaque row bytes returned through attribute 3 */
    u8 unk34[0x10]; /* 0x34: opaque row bytes returned through attribute 4 */
    s32 unk44; /* 0x44: value returned through attribute 5 */
} FldNpcMotion;

enum { FLD_NPC_MOTION_ROW_COUNT = 0x40 };

typedef char FldNpcMotion_size[(sizeof(FldNpcMotion) == 0x48) ? 1 : -1];
typedef char FldNpcMotion_defaultMotionId_offset[((unsigned long)&((FldNpcMotion *)0)->defaultMotionId == 0x00) ? 1 : -1];
typedef char FldNpcMotion_primaryName_offset[((unsigned long)&((FldNpcMotion *)0)->primaryName == 0x04) ? 1 : -1];
typedef char FldNpcMotion_secondaryName_offset[((unsigned long)&((FldNpcMotion *)0)->secondaryName == 0x14) ? 1 : -1];
typedef char FldNpcMotion_unk24_offset[((unsigned long)&((FldNpcMotion *)0)->unk24 == 0x24) ? 1 : -1];
typedef char FldNpcMotion_unk34_offset[((unsigned long)&((FldNpcMotion *)0)->unk34 == 0x34) ? 1 : -1];
typedef char FldNpcMotion_unk44_offset[((unsigned long)&((FldNpcMotion *)0)->unk44 == 0x44) ? 1 : -1];
typedef char FldNpcMotion_table_extent[(FLD_NPC_MOTION_ROW_COUNT * sizeof(FldNpcMotion) == 0x1200) ? 1 : -1];

#endif
