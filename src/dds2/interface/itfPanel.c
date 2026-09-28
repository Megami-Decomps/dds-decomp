#include "common.h"

/* Primary work object (created by func_00192CC8): owned buffers plus the
 * channel-B cursor (count at +0x8, index at +0x20). Extends past 0x2C with
 * the rand/emit control words used by func_00192C00. */
typedef struct EffPrim {
    void *unk0;       /* 0x0: buffer freed by func_00192DE0 */
    void *unk4;       /* 0x4: buffer freed by func_00192DE0 */
    u32 unk8;         /* 0x8: count advanced by func_00192E30 */
    u16 unkC;         /* 0xC: flag set by func_00192CC8 */
    u8 unkE[2];       /* 0xE */
    s32 unk10;        /* 0x10 */
    void *unk14;      /* 0x14: buffer or NULL, tested by func_00192DE0 */
    void *unk18;      /* 0x18 */
    void *unk1C;      /* 0x1C */
    u32 unk20;        /* 0x20: channel-B index advanced by func_00192E30 */
    f32 unk24;        /* 0x24: channel-B position advanced by func_00192E30 */
    f32 unk28;        /* 0x28: channel-B step (0.05f) */
    u8 unk2C[0x18];   /* 0x2C */
    u32 unk44;        /* 0x44: rand count for func_00192C00 */
    u32 unk48;        /* 0x48: rand modulus for func_00192C00 */
    u8 unk4C[0x11C];  /* 0x4C */
    struct EffCntRec *unk168; /* 0x168: counter records */
    s32 *unk16C;      /* 0x16C: base for func_0018E200 */
} EffPrim;

/* 8-byte counter record at EffPrim.unk168. */
typedef struct EffCntRec {
    s32 unk0; /* 0x0: rand slot advanced by func_00192C00 */
    u32 unk4; /* 0x4: id released by func_00192638 */
} EffCntRec;

extern void func_003297C8(void *arg0);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A1930);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A1980);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A19C8);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A1A50);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A1A98);
