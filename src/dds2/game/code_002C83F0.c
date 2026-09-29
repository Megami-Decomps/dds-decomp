#include "common.h"

/* File request entries are 0x64 bytes each; fields mirror the DDS1 table. */
typedef struct FileReqEntry {
    u32 unk0;      /* 0x00 */
    u32 unk4;      /* 0x04 */
    u32 sizeKiB;   /* 0x08: fileReqGetSize converts this to bytes */
    u32 unkC;      /* 0x0C */
    u8 unk10;      /* 0x10 */
    u8 status;     /* 0x11 */
    u8 slotMetadataDirty; /* 0x12 */
    s8 selectedSlot; /* 0x13 */
    u32 slotFlags[20]; /* 0x14 */
} FileReqEntry;

extern FileReqEntry D_00457F68[];

extern s32 D_00439000;

extern s32 (*D_00438BC0)(void);

void fileReqInit(s32 request);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C83F0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C85B0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8638);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8878);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8900);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8AC0);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileManDispatchDone);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileManUpdate);

/* Task callback driving asynchronous file work. */
u32 fileMan(void) {
    fileManUpdate();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002C83F0", fileManInit);

INCLUDE_ASM(const s32, "game/code_002C83F0", fileReqInit);

/* Select and reinitialize one file request, then clear its +0x10 byte. */
void fileReqBegin(s32 request) {
    D_00439000 = request;
    fileReqInit(request);
    D_00457F68[request].unk10 = 0;
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

