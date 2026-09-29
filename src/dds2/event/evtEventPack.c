#include "common.h"

extern u64 func_00101958(void);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D230);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D390);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D4D0);

void evtFreeEventPackState(void) {
    u64 state;

    state = func_00101958();
    func_00328E48(state);
}

void func_0025D4D0(void);
extern void func_0035C860(char *, char *, ...);
extern void *func_00328D68(s32 size);
extern void kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);
extern char D_00424890[];

/* DDS2 twin of DDS1 func_00242298: sprintf the motion-SE task name, alloc a
   3-word param block, and spawn the task (update func_0025D4D0). */
void func_0025D6B0(s32 a0, s32 a1, s32 a2) {
    char buf[0x20];
    s32 *obj;

    func_0035C860(buf, D_00424890, a1, a2);
    obj = func_00328D68(0xC);
    memset(obj, 0, 0xC);
    obj[0] = a0;
    obj[1] = a1;
    obj[2] = a2;
    kwlnTaskCreate(buf, 0x3EC, 0, 0, func_0025D4D0, evtFreeEventPackState, obj);
}

INCLUDE_RODATA(const s32, "event/evtEventPack", D_00424890);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D758);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D7E0);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D8C8);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D928);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377D8);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E8);

