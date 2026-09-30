#include "common.h"
#include "dds3Admin.h"

extern AdminWork* func_00102790(void);

extern void func_001027D8(s32 a0, s32 a1, s32 a2, s32 a3);

/* Configure administrative state from three caller-supplied parameters. */
void dds3AdminSubmitMarkedRequest(s32 a0, s32 a1, s32 a2)
{
    AdminWork* work;

    func_001027D8(a0, a1, a2, 0);
    work = func_00102790();
    work->flags |= 8;
}

/* Mark the admin state with its second independent control flag. */
void dds3AdminSetControlFlag(void)
{
    AdminWork* work;

    work = func_00102790();
    work->flags |= 2;
}

s8 func_00102930(void)
{
    return func_00102790()->unk08;
}

s8 func_00102950(void)
{
    return func_00102790()->unk09;
}

/* Read the signed sample immediately before the ring buffer's write index. */
s8 dds3AdminReadPreviousSignedSample(void)
{
    AdminWork* work;

    work = func_00102790();
    return work->signedHistory[(work->historyIndex + 7) & 7];
}

/* Read the corresponding unsigned sample from the previous ring slot. */
u8 dds3AdminReadPreviousUnsignedSample(void)
{
    AdminWork* work;

    work = func_00102790();
    return work->unsignedHistory[(work->historyIndex + 7) & 7];
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_001029D0);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102B48);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102BC8);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102D48);
