#include "common.h"
#include "dds3Admin.h"

extern AdminWork *dds3GetAdminTaskWork(void);

extern u32 D_003BA9BC;

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101B08);

void dds3SetScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00101060(1, object, mask, scope);
}

void dds3ClearScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00101060(0, object, mask, scope);
}

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101BD8);

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101D60);

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101E40);

INCLUDE_ASM(const s32, "game/code_00101B08", func_001024D8);

u32 func_00102850(void) {
    func_0010FA00(D_003BA9BC);
    return 0;
}

u32 func_00102878(void) {
    func_0010FA40(D_003BA9BC);
    return 0;
}

extern char D_003BA848[];
extern void *kwlnTaskGetTaskByName(char *);
extern AdminWork *kwlnTaskGetUserValue(void *);

AdminWork *dds3GetAdminTaskWork(void) {
    return kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_003BA848));
}

u32 dds3GetAdminTaskValue(void) {
    AdminWork *context;

    context = dds3GetAdminTaskWork();
    return context->value;
}

extern void *func_002CFEB8(s32 size);

/* Replace the admin task's attached data block (copied, max 0x100 bytes) and set its mode byte and flags. */
void func_001028E8(s32 value, void *data, u32 size, s32 flag) {
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
            work->unk1C = func_002CFEB8(size);
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

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039E018);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA83C);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA844);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA848);

