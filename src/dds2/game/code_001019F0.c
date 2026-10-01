#include "common.h"
#include "dds3Admin.h"

extern u32 D_00435D8C;

extern AdminWork *dds3GetAdminTaskWork(void);
extern char D_00435C18[];
extern void *func_00101740(char *);
extern u32 kwlnTaskGetUserValue(void *);

INCLUDE_ASM(const s32, "game/code_001019F0", func_001019F0);

void dds3SetScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00100F48(1, object, mask, scope);
}

void dds3ClearScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00100F48(0, object, mask, scope);
}

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101AC0);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101C50);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101D30);

INCLUDE_ASM(const s32, "game/code_001019F0", func_001023C8);

u32 func_00102740(void) {
    func_0010FC28(D_00435D8C);
    return 0;
}

u32 func_00102768(void) {
    func_0010FC68(D_00435D8C);
    return 0;
}

AdminWork *dds3GetAdminTaskWork(void) {
    return (AdminWork *)kwlnTaskGetUserValue(func_00101740(D_00435C18));
}

u32 dds3GetAdminTaskValue(void) {
    AdminWork *work;

    work = (AdminWork *)dds3GetAdminTaskWork();
    return work->value;
}

extern void *func_00328D68(s32 size);

/* Replace the admin task's attached data block (copied, max 0x100 bytes) and set its mode byte and flags. */
void func_001027D8(s32 value, void *data, u32 size, s32 flag) {
    AdminWork *work;
    void *old;
    u32 flags;

    if (data == NULL || size <= 0x100) {
        work = dds3GetAdminTaskWork();
        old = work->unk1C;
        work->unk09 = value;
        flags = work->flags;
        flags |= 1;
        flags &= ~8;
        flags &= ~0x10000;
        work->flags = flags;
        work->unk21 = 2;
        if (old != NULL) {
            sdfReleaseChipBlock(old);
            work->unk1C = NULL;
            work->unk20 = 0;
        }
        if (data != NULL) {
            work->unk1C = func_00328D68(size);
            memcpy(work->unk1C, data, size);
            work->unk20 = size;
        } else {
            work->unk1C = NULL;
            work->unk20 = 0;
        }
        if (flag != 0) {
            work->flags |= 4;
        } else {
            work->flags &= ~4;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411198);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C0C);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C14);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C18);

