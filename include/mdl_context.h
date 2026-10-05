#ifndef MDL_CONTEXT_H
#define MDL_CONTEXT_H

#include "mdl.h"

/* Model context and motion-node primaries owned by the model helpers in
 * DDS2 game/code_00231A80. Field activation reads first->unk30 from the
 * same objects; keep its representation shared with that owner. */

/* Entry searched by func_00217E10/func_00216BB0 on its s16 id at +0x28.
 * Only the fields read by the matched helpers below are known. */
typedef struct MdlNode {
    struct MdlNode *next; /* 0x0 */
    u8 pad4[4];           /* 0x4 */
    void *unk8;           /* 0x8: dereferenced by func_00218410 */
    u8 padC[0x10];        /* 0xC */
    f32 unk1C;            /* 0x1C: numerically converted to s32 by mdlGetNodeInt1C */
    f32 floatValue;       /* 0x20: accessed as a float by mdlGet/SetNodeFloat20 */
    u8 pad24[4];          /* 0x24 */
    s16 searchId;          /* 0x28: identifies a node in list lookups */
    s16 slotIndex;         /* 0x2A: slot index used by func_00216B78 */
    u16 unk2C;            /* 0x2C */
    u16 unk2E;            /* 0x2E */
    u8 unk30;             /* 0x30: compared against 5 */
    u8 pad31[7];          /* 0x31 */
} MdlNode;

/* Context shared by the matched mdlManager helpers. */
struct MdlCtx {
    u32 flags;         /* 0x0: 1 = skip update, 2 = skip anchors, 4 = needs inner flag 0x10 */
    u8 unk4[8];        /* 0x4 */
    struct MdlSub *sub;       /* 0xC */
    union {
        u32 word;      /* 0x10: low byte read by mdlIsInnerSentinel */
        struct {
            s16 id;    /* 0x10 */
            s16 arg;   /* 0x12 */
        } h;
    } current;
    u32 *list14;       /* 0x14: intrusive list walked by mdlSetAllResourceFrames */
    struct MdlInner *inner;   /* 0x18 */
    struct MdlNode *first;    /* 0x1C */
    struct MdlNode *slots[4]; /* 0x20 */
    struct MdlDevList *devList; /* 0x30: device slots released with the model */
};

typedef char MdlCtx_size_must_be_0x34[(sizeof(MdlCtx) == 0x34) ? 1 : -1];
typedef char MdlNode_size_must_be_0x38[(sizeof(MdlNode) == 0x38) ? 1 : -1];

#endif /* MDL_CONTEXT_H */
