#ifndef EFF_REF_OBJ_H
#define EFF_REF_OBJ_H

#include "common.h"

struct SdfMemBlock;
struct SdfTextureFileHeader;

struct RefObj *func_0029BD90(struct SdfTextureFileHeader *source);
struct RefObj *func_002DDAA8(struct SdfTextureFileHeader *source);

/* Reference-counted texture payload placed at the end of its allocation. */
typedef struct RefObj {
    u8 *base;                         /* 0x00 retained allocation base */
    u8 *pixels;                       /* 0x04 image bytes */
    u8 *palette;                      /* 0x08 palette bytes */
    s32 paletteWidth;                 /* 0x0C */
    s32 paletteHeight;                /* 0x10 */
    s32 refCount;                     /* 0x14 retained references to this texture */
    s32 index;                        /* 0x18 caller-selected texture index */
    struct SdfMemBlock *allocationHandle; /* 0x1C final-release descriptor */
} RefObj;

typedef char RefObjSizeCheck[sizeof(RefObj) == 0x20 ? 1 : -1];
typedef char RefObjAllocationOffsetCheck[
    ((u32)&((RefObj *)0)->allocationHandle == 0x1C) ? 1 : -1];

RefObj *effCloneSharedReferenceWithValue(struct SdfTextureFileHeader *source, u32 textureIndex);
void effReleaseSharedReference(RefObj *obj);

#endif
