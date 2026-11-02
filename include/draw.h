#ifndef DRAW_H
#define DRAW_H

/* The GS side of the engine: the frame buffers, the three-slot draw queue, the
 * draw environment, and the A+D register packets the sdfPrim and sdf2D
 * builders file into an ordering table.
 *
 * sdfGrPacketHeader_t (libsdf.h) is every packet's first 32 bytes:
 * sdfGrSetRegsPacketHeader writes the DMA tag's qwc and VIF DIRECT code and
 * then the GIF tag, with NLOOP as the register count and REGS as A+D.
 * See include/README.md for the evidence per struct. */

#include "common.h"
#include "libsdf.h"

typedef struct sdfGrFrameSet_t sdfGrFrameSet_t;

/* One entry of sdfGrFrameModes, the screen geometry table sdfGrSetFrameSetMode
 * copies into a frame set. Its 12-byte stride is that function's index scale. */
typedef struct
{
    s16 w;
    s16 h;
    s16 buffH;
    u16 pixelFormat;
    u16 zbuffFormat;
    u16 fieldFrameMode; /* unconfirmed; nothing in sdfGraph.c reads it */
} sdfGrScreenDef_t;

/* The two frame buffers and the depth buffer they share. sdfGrMakeFrameSet
 * allocates all three at w by buffH, taking the frames' format from
 * pixelFormat and the depth buffer's from zbuffFormat. */
struct sdfGrFrameSet_t
{
    s16 w;
    s16 h;
    s16 buffH;
    u8 pixelFormat;
    u8 zbuffFormat;
    sdfGsMemSlice_t *frameHandle[2];
    sdfGsMemSlice_t *zbuffHandle;
};

/* One queued frame. sdfGrDrawState is three of these; sdfGrReqDraw fills the
 * next slot and sdfGrDrawFrame sends tag->head down channel 1. */
typedef struct
{
    sdfDrawTag_t *tag;
    sdfPreDrawHook_t *preDrawHead;
    u8 mode;
    u8 buffId;
    u8 frameId;
    u8 halfOffset;
    s32 pad;
} sdfGrDrawState_t;

/* PRMODECONT, COLCLAMP, DTHE and TEXA, in the order
 * sdfGrSetDrawDefParmPacket writes them. */
typedef struct QWORD_ALIGNED
{
    sdfGrTagStdReg_t prmodecont;
    sdfGrTagStdReg_t colclamp;
    sdfGrTagStdReg_t dthe;
    sdfGrTagStdReg_t texa;
} sdfGrDrawDefParmPacket_t;

/* Where a draw lands: sdfGrSetDrawFramePacket builds all four from the frame
 * set's geometry, naming the context-2 registers instead when its aContext
 * argument is set. */
typedef struct QWORD_ALIGNED
{
    sdfGrTagStdReg_t frame;
    sdfGrTagStdReg_t zbuf;
    sdfGrTagStdReg_t xyoffset;
    sdfGrTagStdReg_t scissor;
} sdfGrDrawFramePacket_t;

/* sdfGrSetClearPacket: a sprite over the whole buffer under a TEST that passes
 * everything, the two corners placed around the GS 4096 coordinate bias. */
typedef struct QWORD_ALIGNED
{
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t prim;
    union
    {
        sdfGrTagStdReg_t rgbaq;
        /* The clear-color setter replaces RGBA only, retaining Q and A+D. */
        u32 clearColor;
    };
    sdfGrTagStdReg_t xyzA;
    sdfGrTagStdReg_t xyzB;
} sdfGrClearPacket_t;

/* sdfGrSetDrawEnv: 21 A+D writes in one packet, the frame and test state for
 * both GS contexts. */
typedef struct QWORD_ALIGNED
{
    sdfGrPacketHeader_t header;
    sdfGrDrawDefParmPacket_t defParm;
    sdfGrDrawFramePacket_t frame1;
    sdfGrDrawFramePacket_t frame2;
    sdfGrClearPacket_t clear;
    sdfGrTagStdReg_t test1;
    sdfGrTagStdReg_t alpha1;
    sdfGrTagStdReg_t test2;
    sdfGrTagStdReg_t alpha2;
} sdfGrDrawEnv_t;

/* sdf2DMakeTexPacket: one texture handle's three registers, read back out of
 * the handle by sdfTexGetTex1Reg, sdfTexGetTex0Reg and sdfTexGetClampReg. The
 * register addresses are taken from the caller's base, so the same packet
 * serves either GS context. */
typedef struct QWORD_ALIGNED
{
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t tex1;
    sdfGrTagStdReg_t tex0;
    sdfGrTagStdReg_t clamp;
} sdf2DTexPacket_t;

/* The TEST and ALPHA pair that opens a transparency layer. One setter per
 * layer and per context writes it; see sdf_gs_alpha and sdf_gs_test for the
 * four combinations. */
typedef struct QWORD_ALIGNED
{
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t alpha;
} sdfPrimTransElem_t;

typedef struct QWORD_ALIGNED
{
    sdfGrPacketHeader_t header;
    sdfPrimTransElem_t elem;
} sdfPrimTransPacket_t;

/* sdfPrimSetTexflushElem: one write of 0 to TEXFLUSH, which invalidates the
 * GS texture cache. */
typedef struct QWORD_ALIGNED
{
    sdfGrTagStdReg_t texflush;
} sdfPrimTexflushElem_t;

typedef struct QWORD_ALIGNED
{
    sdfGrPacketHeader_t header;
    sdfPrimTexflushElem_t texflush;
} sdfPrimTexflushPacket_t;

/* One GS local-memory to local-memory copy, as sdfPrimSetBitBltElem packs it:
 * source and destination buffer, the two corners, the size, and the direction. */
typedef struct QWORD_ALIGNED
{
    sdfGrTagStdReg_t bitbltbuf;
    sdfGrTagStdReg_t trxpos;
    sdfGrTagStdReg_t trxreg;
    sdfGrTagStdReg_t trxdir;
} sdfPrimBitBltElem_t;

typedef struct QWORD_ALIGNED
{
    sdfGrPacketHeader_t header;
    sdfPrimBitBltElem_t bitblt;
} sdfPrimBitBltPacket_t;

/* One ordering table's own draw tag plus the transparency state its draws run
 * under. sdfModelSetDrawEnv builds four, one per table, and
 * sdfModelDrawEnvLinkOt files each through the table's regFunc. */
typedef struct QWORD_ALIGNED
{
    sdfDrawTag_t drawTag;
    sdfPrimTransPacket_t transPacket;
} sdfModelDrawEnvSet_t;

/* The tail, filed into the last table: a draw tag whose only packet is a DMA
 * tag that ends the chain. */
typedef struct QWORD_ALIGNED
{
    sdfDrawTag_t drawTag;
    sdfDmaTag_t footer;
} sdfModelDrawEnvTerm_t;

typedef struct QWORD_ALIGNED
{
    sdfModelDrawEnvSet_t set[4];
    sdfModelDrawEnvTerm_t term;
} sdfModelDrawEnv_t;

/* The GS register an A+D write names in the low byte of sdfGrTagStdReg_t.reg.
 * Only the ones this engine writes are listed; a context-2 register sits one
 * address above its context-1 twin. */
enum sdf_gs_reg
{
    SDF_GS_REG_PRIM = 0x00,
    SDF_GS_REG_RGBAQ = 0x01,
    SDF_GS_REG_XYZ2 = 0x05,
    SDF_GS_REG_TEX0_1 = 0x06,
    SDF_GS_REG_CLAMP_1 = 0x08,
    SDF_GS_REG_TEX1_1 = 0x14,
    SDF_GS_REG_XYOFFSET_1 = 0x18,
    SDF_GS_REG_XYOFFSET_2 = 0x19,
    SDF_GS_REG_PRMODECONT = 0x1A,
    SDF_GS_REG_TEXA = 0x3B,
    SDF_GS_REG_FOGCOL = 0x3D,
    SDF_GS_REG_TEXFLUSH = 0x3F,
    SDF_GS_REG_SCISSOR_1 = 0x40,
    SDF_GS_REG_SCISSOR_2 = 0x41,
    SDF_GS_REG_ALPHA_1 = 0x42,
    SDF_GS_REG_ALPHA_2 = 0x43,
    SDF_GS_REG_DTHE = 0x45,
    SDF_GS_REG_COLCLAMP = 0x46,
    SDF_GS_REG_TEST_1 = 0x47,
    SDF_GS_REG_TEST_2 = 0x48,
    SDF_GS_REG_FRAME_1 = 0x4C,
    SDF_GS_REG_FRAME_2 = 0x4D,
    SDF_GS_REG_ZBUF_1 = 0x4E,
    SDF_GS_REG_ZBUF_2 = 0x4F,
    SDF_GS_REG_BITBLTBUF = 0x50,
    SDF_GS_REG_TRXPOS = 0x51,
    SDF_GS_REG_TRXREG = 0x52,
    SDF_GS_REG_TRXDIR = 0x53
};

/* The ALPHA value each transparency layer runs under, as the four
 * sdfPrimSet*TransElem functions write it. ALPHA is
 * Cv = (A - B) * C + D over the source, the destination and zero. */
enum sdf_gs_alpha
{
    SDF_GS_ALPHA_BLEND = 0x44,    /* Cs*As + Cd*(1-As): opaque and semi-transparent */
    SDF_GS_ALPHA_ADD = 0x48,      /* Cs*As + Cd */
    SDF_GS_ALPHA_SUBTRACT = 0x42  /* Cd - Cs*As */
};

/* The TEST value that goes with each. Every one of the three fails to the
 * frame buffer only, so a failing fragment keeps its colour write and loses
 * its depth write. */
enum sdf_gs_test
{
    /* ATST GEQUAL 127, ZTST GEQUAL: the opaque and semi-transparent layers. */
    SDF_GS_TEST_ALPHAGEQ = 0x717FB,
    /* ATST NEVER, so the additive and subtractive layers never write depth. */
    SDF_GS_TEST_NOZWRITE = 0x71801,
    /* As ALPHAGEQ with ZTST ALWAYS; sdfGrSetDrawEnv's own pair. */
    SDF_GS_TEST_DRAWENV = 0x517FB
};

#endif /* DRAW_H */
