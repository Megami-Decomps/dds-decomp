#include "common.h"

/* File request entry: D_003DC698 table, 0x64 bytes per entry. */
typedef struct FileReqEntry {
    u32 unk0;      /* 0x00 */
    u32 unk4;      /* 0x04 */
    u32 unk8;      /* 0x08 */
    u32 unkC;      /* 0x0C */
    u8 unk10;      /* 0x10 */
    u8 unk11;      /* 0x11 */
    u8 unk12;      /* 0x12 */
    s8 unk13;      /* 0x13 */
    u32 unk14[20]; /* 0x14 */
} FileReqEntry;

extern FileReqEntry D_00457F68[];

extern s32 D_00439000;

extern s32 (*D_00438BC0)(void);

void fileReqInit(s32 arg0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C83F0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C85B0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8638);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8878);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8900);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8AC0);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileManDispatchDone);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileManUpdate);

u32 fileMan(void) {
    fileManUpdate();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002C83F0", fileManInit);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqInit);

void fileReqBegin(s32 slot) {
    D_00439000 = slot;
    fileReqInit(slot);
    D_00457F68[slot].unk10 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqPoll);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqGetStatus);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqGetSize);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqIsSlotMetadataDirty);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqClearSlotMetadataDirty);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqMarkSlotMetadataDirty);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqClearSlotFlags);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqSetSlotFlags);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqGetSlotFlags);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqGetSelectedSlot);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqSetSelectedSlot);

void func_002C92D0(u32 arg0) {
    func_0034FCE0(arg0, 0);
}

INCLUDE_SDATA(const s32, "game/code_002C83F0", D_00437CC0);

INCLUDE_SDATA(const s32, "game/code_002C83F0", D_00437CC8);

