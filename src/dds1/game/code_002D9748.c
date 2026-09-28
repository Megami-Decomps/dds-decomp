#include "common.h"

typedef struct SdfSubParam {
    u64 unk0; /* 0x0 */
    u64 unk8; /* 0x8 */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
} SdfSubParam;

typedef struct SdfTextParam {
    u8 pad00[6]; /* 0x00 */
    u8 unk06; /* 0x06: dirty flags for the setters below */
    u8 pad07[9]; /* 0x07 */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
    u8 unk18; /* 0x18: func_002DA5B0 stores a u32 over 0x18-0x1B */
    u8 unk19; /* 0x19: bit 0x2 selects unk88/unk8C over defaults */
    u8 unk1A; /* 0x1A */
    u8 unk1B; /* 0x1B */
    f32 unk1C; /* 0x1C */
    u32 unk20; /* 0x20 */
    u8 pad24[4]; /* 0x24 */
    f32 unk28; /* 0x28 */
    f32 unk2C; /* 0x2C */
    u32 unk30; /* 0x30 */
    u32 unk34; /* 0x34 */
    SdfSubParam *unk38; /* 0x38 */
    SdfSubParam *unk3C; /* 0x3C */
    f32 unk40; /* 0x40 */
    f32 unk44; /* 0x44 */
    u8 pad48[0x40]; /* 0x48 */
    f32 unk88; /* 0x88 */
    f32 unk8C; /* 0x8C */
    void *unk90; /* 0x90: resource chunk searched by tag */
} SdfTextParam;

typedef struct SdfChunk {
    u32 unk0; /* 0x0: entry id, 0 terminates the list */
    u32 unk4; /* 0x4: byte offset to the next entry */
} SdfChunk;

typedef struct SdfNode {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 unk3; /* 0x3: type tag (0x20/0x30/0x50) */
    u32 unk4; /* 0x4 */
    u32 unk8; /* 0x8 */
    u32 unkC; /* 0xC */
} SdfNode;


extern SdfSubParam *sdfSubParamCreate(void);

extern u32 D_003BD34C;
extern f32 D_003BD358;
extern f32 D_003BD35C;
extern u8 D_003BDA10;
extern u8 D_003BDA18;

void *sdfChunkFindById(SdfChunk *chunk, s32 id);
void *func_002CFEB8(s32 size);
s32 func_002D9DD8(void);
void func_002D9D00(s32 arg0, s32 arg1);
void func_002D9D80(s32 arg0, s32 arg1);
void func_002D3BE0(void *arg0, void (*arg1)(void));
void func_002D3C30(void *arg0, s32 arg1);
void func_002DA290(void);
void func_002DAA00(void);
void func_002E7680(void);
INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9748);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D99B0);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9A70);

void *sdfChunkFindById(SdfChunk *chunk, s32 id) {
    u32 cur;

    if (chunk == NULL) {
        return NULL;
    }
    cur = chunk->unk0;
    while (cur != 0) {
        if (cur == id) {
            return (void *)chunk;
        }
        chunk = (SdfChunk *)((u8 *)chunk + chunk->unk4);
        cur = chunk->unk0;
    }
    return NULL;
}

void *sdfChunkFindByTag(SdfTextParam *arg0, s32 tag) {
    return sdfChunkFindById(arg0->unk90, tag);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9B68);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9C28);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9CC8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9D00);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9D80);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9DD8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9E58);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9E98);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9ED8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9F08);

void func_002D9F38(SdfTextParam *arg0, f32 fparg0, f32 fparg1) {
    arg0->unk88 = fparg0;
    arg0->unk8C = fparg1;
    arg0->unk19 = arg0->unk19 | 2;
}

void func_002D9F50(SdfTextParam *arg0) {
    arg0->unk19 = arg0->unk19 & ~2;
}

f32 func_002D9F60(SdfTextParam *arg0) {
    if ((arg0->unk19 & 2) != 0) {
        return arg0->unk88;
    }
    return D_003BD358;
}

f32 func_002D9F80(SdfTextParam *arg0) {
    if ((arg0->unk19 & 2) != 0) {
        return arg0->unk8C;
    }
    return D_003BD35C;
}

void func_002D9FA0(u32 arg0) {
    D_003BD34C = arg0;
}

void func_002D9FA8(u32 arg0) {
    devCreateRequest(arg0, 4, 4);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9FC8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA058);

void func_002DA0C0(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    if ((arg1 < *(s16 *)(arg0 + 4)) && (arg2 != 0)) {
        temp_v0 = (s32)arg1;
        do {
            temp_v0 = temp_v0 + 1;
        } while ((s64)temp_v0 != (s64)*(s16 *)(arg0 + 4));
        *(s16 *)(arg0 + 4) = (s16)arg1;
    }
    func_002E7730();
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA118);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA1B0);

void func_002DA240(void) {
    func_002D3BE0(&D_003BDA10, func_002DA290);
    func_002D3BE0(&D_003BDA18, func_002DAA00);
}

void func_002DA270(u32 arg0) {
    devCreateRequest(arg0, 4, 8);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA290);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA2F8);

void func_002DA340(void) {
    func_002E76B0();
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA358);

void func_002DA3C0(SdfTextParam *arg0, u32 arg1) {
    arg0->unk10 = arg1;
    arg0->unk06 |= 3;
}

void func_002DA3D8(SdfTextParam *arg0, u32 arg1) {
    arg0->unk14 = arg1;
    arg0->unk06 |= 3;
}

void func_002DA3F0(SdfTextParam *arg0, u32 arg1) {
    arg0->unk20 = arg1;
    arg0->unk06 |= 3;
}

void func_002DA408(SdfTextParam *arg0, u32 arg1) {
    *(u32 *)&arg0->unk28 = arg1;
    arg0->unk06 |= 3;
}

void func_002DA420(SdfTextParam *arg0, f32 fparg0) {
    arg0->unk1C = fparg0;
    arg0->unk06 = arg0->unk06 | 3;
}

void func_002DA438(SdfTextParam *arg0, u32 arg1) {
    *(u32 *)&arg0->unk2C = arg1;
    arg0->unk06 |= 3;
}

SdfSubParam *sdfSubParamCreate(void) {
    SdfSubParam *temp;

    temp = func_002CFEB8(0x18);
    temp->unk8 = (((u64)0x3F800000 << 16 | 0x3F80) << 16);
    temp->unk0 = 0;
    temp->unk10 = 0;
    return temp;
}

void func_002DA490(SdfTextParam *arg0) {
    if (arg0->unk38 == 0) {
        arg0->unk38 = sdfSubParamCreate();
    }
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA4C8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA548);

void func_002DA5B0(SdfTextParam *arg0, u32 arg1) {
    *(u32 *)((u8 *)arg0 + 0x18) = arg1;
    arg0->unk06 |= 0x30;
}

void func_002DA5C8(SdfTextParam *arg0, u32 arg1) {
    arg0->unk34 = arg1;
    arg0->unk06 |= 0x30;
}

void func_002DA5E0(SdfTextParam *arg0, u32 arg1) {
    arg0->unk30 = arg1;
    arg0->unk06 |= 0x30;
}

void func_002DA5F8(SdfTextParam *arg0) {
    if (arg0->unk3C == 0) {
        arg0->unk3C = sdfSubParamCreate();
    }
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA630);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA6B0);

void func_002DA718(SdfTextParam *arg0, f32 fparg0, f32 fparg1) {
    arg0->unk40 = fparg0;
    arg0->unk44 = fparg1;
    arg0->unk06 = arg0->unk06 | 0xC0;
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA730);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA830);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAA00);

void func_002DAA68(s32 arg0) {
    s32 id = arg0;

    if (id != 0) {
        func_002D3C30(&D_003BDA18, id);
    }
}

void *func_002DAAA0(u32 *arg0, SdfNode *arg1, s32 arg2) {
    u32 *entry = arg0 + arg2;

    arg1->unk3 = 0x30;
    arg1->unk4 = entry[2] & 0x0FFFFFFF;
    arg1->unk0 = 0xA;
    arg1->unk8 = 0;
    arg1->unkC = 0;
    return (void *)((u8 *)arg1 + 0x10);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAAE0);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAB80);

void func_002DAC68(s32 arg0, s32 arg1) {
    func_002DAB80(arg1 + 0x68, *(u32 *)(arg0 + 0x38));
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAC88);

void func_002DAD18(SdfTextParam *arg0, SdfTextParam *arg1) {
    arg1->unk28 = arg0->unk40;
    arg1->unk2C = arg0->unk44;
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAD30);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAE00);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAEE8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAF88);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAFE8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB048);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB158);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB1C8);

INCLUDE_SDATA(const s32, "game/code_002D9748", D_003BD348);

INCLUDE_SDATA(const s32, "game/code_002D9748", D_003BD34C);

