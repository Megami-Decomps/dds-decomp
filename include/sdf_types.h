#ifndef SDF_TYPES_H
#define SDF_TYPES_H

/* Native math and DMA types shared by renderer packet headers. Keeping these
 * independent of the engine owners lets packet constructors use the canonical
 * layouts without importing unrelated material, texture and queue definitions. */
#include "common.h"

/* The VU0 paths load and store these with lq/sq, so every instance the engine
 * touches that way sits on a 16-byte boundary by virtue of its offset. ee-gcc
 * aligns them to 4, which is what reproduces the verified layouts. */
typedef struct
{
    f32 x;
    f32 y;
} sdfVector2_t;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
} sdfVector3_t;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} sdfVector4_t;

typedef struct
{
    f32 m[16];
} sdfMatrix4_t;

/* DMAC source-chain tag. id 0x20 is `ref`, written by sdfDrawTagLink. */
typedef struct QWORD_ALIGNED
{
    u16 qwc;
    u8 pad;
    u8 id;
    void *addr;
    u32 dat0;
    u32 dat1;
} sdfDmaTag_t;

typedef struct QWORD_ALIGNED
{
    u64 parm;
    u64 regs;
} sdfGifTag_t;

/* One GS register write in a GIFtag A+D packet. */
typedef struct
{
    u64 data;
    u64 reg;
} sdfGrTagStdReg_t;

/* A GIF packet opens with a DMA tag and a GIF tag, followed by its payload. */
typedef struct sdfGrPacketHeader_t
{
    union
    {
        sdfDmaTag_t dmatag;
        /* Whole hardware words used by the GS packet constructors. */
        struct
        {
            u64 tag;
            u64 vif;
        } dmaWords;
    };
    sdfGifTag_t giftag;
} sdfGrPacketHeader_t;

#endif /* SDF_TYPES_H */
