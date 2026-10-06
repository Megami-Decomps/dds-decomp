#ifndef MDL_CONTEXT_H
#define MDL_CONTEXT_H

#include "mdl.h"
#include "sdf_draw.h"

/* Model context shared by the DDS2 model-manager helpers. The motion slots
 * point to the canonical Motion owner, including its state byte at +0x30. */

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
    Motion *first;             /* 0x1C */
    Motion *slots[4];          /* 0x20 */
    struct MdlDevList *devList; /* 0x30: device slots released with the model */
};

typedef char MdlCtx_size_must_be_0x34[(sizeof(MdlCtx) == 0x34) ? 1 : -1];

#endif /* MDL_CONTEXT_H */
