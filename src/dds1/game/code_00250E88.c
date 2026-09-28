#include "common.h"

extern void func_002512F0(s32, s32);

extern void func_0025DB80(s32);

extern void func_00250978(s32);

extern void func_00257E78(s32);

extern void destroyGridWork(s32);

extern void func_00256C28(s32);

extern void func_002D0918(s32);

extern void menuResetWorkFloats(void);

extern s32 func_002CB3B8(u32, u32);

extern u32 D_003BC4CC;

typedef struct {
    u32 unk0;
    u16 unk4;
    u16 unk6;
    u32 unk8;
} SceneEntry;

extern SceneEntry D_0036BE38[];

extern void func_002CFF98(void *);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250E88);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250F60);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251260);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002512F0);

void func_002515C8(s32 unused, void *data) {
    if (data != NULL) {
        func_002CFF98(data);
    }
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_002515F0);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002517C0);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251960);

void func_002519E8(s32 unused, s32 context) {
    func_002CB3B8(D_003BC4CC, -1);
    destroyGridWork(*(s32 *)(context + 0x484));
    func_00256C28(context + 0x584);
    func_002D0918(*(s32 *)context);
    menuResetWorkFloats();
}

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF850);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251A38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251E38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252CE8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252E38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252F88);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253018);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002530D8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253208);

void func_00253520(s32 context) {
    destroyGridWork(*(s32 *)(context + 0x484));
    func_00250978(context);
    func_00257E78(context);
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253558);

s32 func_00253608(void) {
    s32 temp_v0 = func_002CB3B8(D_003BC4CC, 1);

    if (temp_v0 == 0) {
        return 0;
    }
    return *(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0x484) + 8) + 4);
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253640);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253778);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253830);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253AD0);

typedef struct {
    u8 pad00[0x59C];
    u16 entryX; /* 0x59C */
    u16 entryY; /* 0x59E */
} SceneMetadataContext;

typedef struct {
    u8 pad00[0xC];
    u16 entryIndex; /* 0x0C */
} SceneMetadataNode;

void updateSceneEntryMetadata(s32 context) {
    SceneMetadataNode *node = (SceneMetadataNode *)*(s32 *)(*(s32 *)(*(s32 *)(context + 0x484) + 8) + 4);
    u16 index = node->entryIndex;
    ((SceneMetadataContext *)context)->entryX = D_0036BE38[index].unk4;
    index = node->entryIndex;
    ((SceneMetadataContext *)context)->entryY = D_0036BE38[index].unk6;
}

s32 resetSceneState(void) {
    updateSceneEntryMetadata(func_002CB3B8(D_003BC4CC, 1));
    return 0;
}

void func_00253CF8(void) {
    s32 context = func_002CB3B8(D_003BC4CC, 1);
    func_002512F0(context, 1);
    func_00257E78(context);
    func_0025DB80(context + 0x590);
}

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC420);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC424);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC428);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC430);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC438);

