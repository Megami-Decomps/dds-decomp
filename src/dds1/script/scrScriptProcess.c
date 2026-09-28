#include "common.h"

typedef struct { u8 pad0[0x40]; s32 unk40[256]; s32 unk440[256]; } ScrProcGlobals;
extern ScrProcGlobals *D_003BAA00;

/* Task object created by func_0010BA18/func_0010B7C0 and started below. */
typedef struct {
    u8   pad_0x00[0xB4]; /* 0x00 */
    s32  unkB4;          /* 0xB4 */
    u8   pad_0xB8[0x10]; /* 0xB8 */
    s32  unkC8;          /* 0xC8 */
    u8   pad_0xCC[0x0C]; /* 0xCC */
    void *unkD8;         /* 0xD8: back-pointer to the script handle */
    u8   pad_0xDC[0x08]; /* 0xDC */
    s32  unkE4;          /* 0xE4: task id from kwlnTaskCreate */
} ScrProcTask; /* 0xE8 */
void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 func_0010C0C8();
s32 func_0010C070();
s32 func_0010B7C0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
s32 func_0010BA18(s32 arg0, s32 arg1);
s32 func_0010BB28(s32 arg0, s32 arg1)
{
    u32 buf[4];
    void *handle;
    s32 id;
    ScrProcTask *task;
    handle = func_002EB028(arg0, buf, 0);
    id = buf[0];
    if (id == 0)
    {
        return 0;
    }
    task = (ScrProcTask *)func_0010BA18(id, arg1);
    if (task != NULL)
    {
        task->unkD8 = handle;
    }
    return (s32)task;
}

s32 scrProcCreateTask(s32 arg0, ScrProcTask *task)
{
    s32 id;
    id = kwlnTaskCreate(task->unkB4 + (task->unkC8 << 5), arg0, 1, 1, func_0010C0C8, func_0010C070, (s32)task);
    task->unkE4 = id;
    return id;
}

void func_0010BBE0(void)
{
    s32 i;
    /* Countdown with a forward index; gcc keeps a single pointer (see asm). */
    for (i = 255; i >= 0; i--)
    {
        D_003BAA00->unk40[255 - i] = 0;
        D_003BAA00->unk440[255 - i] = 0;
    }
}

void func_0010BC18(s32 arg0, s32 arg1, s32 arg2)
{
    scrProcCreateTask(arg0, func_0010BB28(arg1, arg2));
}

s32 func_0010BC50(s32 arg0)
{
    return func_0010BB28(arg0, 0);
}

void func_0010BC68(s32 arg0, s32 arg1, s32 arg2)
{
    scrProcCreateTask(arg0, func_0010BA18(arg1, arg2));
}

s32 func_0010BCA0(s32 arg0)
{
    return func_0010BA18(arg0, 0);
}

s32 func_0010BCB8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    return scrProcCreateTask(a0, func_0010B7C0(a1, a2, a3, a4, a5, a6, a7, a8));
}

s32 func_0010BD08(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
    return func_0010B7C0(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0);
}

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BD20);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BDB8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BE30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BED8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_0010BF30);





INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_0039E288);

