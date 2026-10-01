#include "common.h"
#include "dds3Admin.h"

extern AdminWork* dds3GetAdminTaskWork(void);
extern void func_001028E8(s32 a0, s32 a1, s32 a2, s32 a3);
extern void* kwlnTaskGetUserValue(void* task);
extern void kwlnTaskSetUserValue(void* task, u32 value);
extern void* func_002CFEB8(s32 a0);
extern void sdfReleaseChipBlock(void* ptr);
extern void* memcpy(void* dst, void* src, s32 n);
extern void* D_003297D0[];
extern void* D_003297D4[];
extern void* D_003297D8[];
extern u8 D_003BA848[];

/* Configure administrative state from three caller-supplied parameters. */
void dds3AdminSubmitMarkedRequest(s32 a0, s32 a1, s32 a2)
{
    AdminWork* work;

    func_001028E8(a0, a1, a2, 0);
    work = dds3GetAdminTaskWork();
    work->flags |= 8;
}

/* Mark the admin state with its second independent control flag. */
void dds3AdminSetControlFlag(void)
{
    AdminWork* work;

    work = dds3GetAdminTaskWork();
    work->flags |= 2;
}

s8 func_00102A40(void)
{
    return dds3GetAdminTaskWork()->unk08;
}

s8 func_00102A60(void)
{
    return dds3GetAdminTaskWork()->unk09;
}

/* Read the signed sample immediately before the ring buffer's write index. */
s8 dds3AdminReadPreviousSignedSample(void)
{
    AdminWork* work;

    work = dds3GetAdminTaskWork();
    return work->signedHistory[(work->historyIndex + 7) & 7];
}

/* Read the corresponding unsigned sample from the previous ring slot. */
u8 dds3AdminReadPreviousUnsignedSample(void)
{
    AdminWork* work;

    work = dds3GetAdminTaskWork();
    return work->unsignedHistory[(work->historyIndex + 7) & 7];
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102AE0);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102C58);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102CD8);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102E58);
