#ifndef FLD_INF_H
#define FLD_INF_H

#include "common.h"

/* Fixed little-endian INF serialization, documented in docs/inf.md and
 * tools/inf.py. Native loaders and static buffers use exactly 0x3B80 bytes.
 * Unsigned selector fields preserve their serialized bytes. Graph readers
 * sign-extend eventHit, branch targets and extra-action selectors; view stays
 * unsigned. Extra-action dispatch consumes the low signed halfword of the
 * encoded 32-bit action. These interpretations do not alter the wire layout. */
typedef struct FldInfHit {
    s8 area; /* The event-name resolver performs a signed byte comparison. */
    char eventName[15];
} FldInfHit;

typedef struct FldInfPack {
    s32 setIndex;
    FldInfHit hits[5];
} FldInfPack;

typedef struct FldInfView {
    s32 player;
    s32 motion;
    char positionName[12];
    char cameraName[12];
} FldInfView;

typedef struct FldInfExtraAction {
    s32 action;
    s32 parameters[4];
} FldInfExtraAction;

typedef struct FldInfFlagSelector {
    s16 flag;
    u8 offTarget;
    u8 onTarget;
} FldInfFlagSelector;

typedef struct FldInfMessageRow {
    s16 kind;
    s16 message;
    u8 targets[4];
    s16 flagOff;
    s16 flagOn;
    u8 view;
    u8 extraAction;
} FldInfMessageRow;

typedef struct FldInfSet {
    union {
        s16 packed; /* Native resolver compares both bytes as one signed halfword. */
        struct {
            u8 kind;
            u8 area;
        } bytes;
    } kindArea;
    s8 action;
    u8 eventHit;
    char eventName[12];
    FldInfFlagSelector flags[4];
    FldInfMessageRow messages[20];
} FldInfSet;

typedef struct FldInfTable {
    FldInfPack packs[8];
    FldInfView views[40];
    FldInfExtraAction extraActions[40];
    FldInfSet sets[40];
} FldInfTable;

typedef char FldInfHit_size[(sizeof(FldInfHit) == 0x10) ? 1 : -1];
typedef char FldInfPack_size[(sizeof(FldInfPack) == 0x54) ? 1 : -1];
typedef char FldInfView_size[(sizeof(FldInfView) == 0x20) ? 1 : -1];
typedef char FldInfExtraAction_size[(sizeof(FldInfExtraAction) == 0x14) ? 1 : -1];
typedef char FldInfMessageRow_size[(sizeof(FldInfMessageRow) == 0xE) ? 1 : -1];
typedef char FldInfSet_size[(sizeof(FldInfSet) == 0x138) ? 1 : -1];
typedef char FldInfTable_size[(sizeof(FldInfTable) == 0x3B80) ? 1 : -1];
typedef char FldInfTable_views_offset[((unsigned long)&((FldInfTable *)0)->views == 0x2A0) ? 1 : -1];
typedef char FldInfTable_extraActions_offset[((unsigned long)&((FldInfTable *)0)->extraActions == 0x7A0) ? 1 : -1];
typedef char FldInfTable_sets_offset[((unsigned long)&((FldInfTable *)0)->sets == 0xAC0) ? 1 : -1];

#endif
