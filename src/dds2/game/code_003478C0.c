#include "common.h"

typedef struct SdfRequest {
    u8 active;
    u8 pad01[3];
    u32 value;
} SdfRequest;

extern void sdfReleasePoolNode();

extern void func_00348B78();

INCLUDE_ASM(const s32, "game/code_003478C0", func_003478C0);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00347948);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00347988);

void sdfActivateRequest(SdfRequest *request, u32 value) {
    request->value = value;
    request->active = 1;
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00347D50);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348158);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348188);

INCLUDE_ASM(const s32, "game/code_003478C0", func_003481E8);

INCLUDE_ASM(const s32, "game/code_003478C0", sdfStreamSendChunk);

INCLUDE_ASM(const s32, "game/code_003478C0", func_003482B0);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348408);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348540);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348630);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348670);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348700);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348780);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348800);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348900);

INCLUDE_ASM(const s32, "game/code_003478C0", sdfReleasePoolNode);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348A30);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348AA0);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348B10);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348B78);

void func_00348BD8(u32 *work) {
    work[4] = (u32)sdfReleasePoolNode;
    work[5] = (u32)func_00348B78;
}

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDD0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDE0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDF0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EE00);

