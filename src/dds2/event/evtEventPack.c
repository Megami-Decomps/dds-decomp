#include "common.h"

extern s32 func_00101958(void);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D230);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D390);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D4D0);

void evtFreeEventPackState(void) {
    s32 state;

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

extern void func_002C81E8(void);
extern void effInitCh72Id(void);
extern void effInitCh71Id(void);
extern void effInitCh76Id(void);
extern void effInitCh75Id(void);
extern void func_0032BBB0(s32);
extern void func_002C7D00(s32);
extern void func_003298C0(s32);
extern void func_003297C8(s32);
extern void func_00328E48(s32);

/* DDS2 twin of DDS1 func_00242510: release every effect/resource the battle-
   event work holds, then free it. */
void func_0025D928(void) {
    s32 obj = func_00101958();

    func_002C81E8();
    if (obj != 0) {
        if (*(s32 *)(obj + 0x38) != 0) {
            effInitCh72Id();
            func_0032BBB0(*(s32 *)(obj + 0x38));
        }
        if (*(s32 *)(obj + 0x3C) != 0) {
            effInitCh71Id();
            func_0032BBB0(*(s32 *)(obj + 0x3C));
        }
        if (*(s32 *)(obj + 0x40) != 0) {
            effInitCh76Id();
            func_0032BBB0(*(s32 *)(obj + 0x40));
        }
        if (*(s32 *)(obj + 0x44) != 0) {
            effInitCh75Id();
            func_0032BBB0(*(s32 *)(obj + 0x44));
        }
        if (*(s32 *)(obj + 8) != 0) {
            func_002C7D00(*(s32 *)(obj + 8));
        }
        if (*(s32 *)(obj + 0xC) != 0) {
            func_003298C0(*(s32 *)(obj + 0xC));
        }
        if (*(s32 *)(obj + 0x24) != 0) {
            func_003297C8(*(s32 *)(obj + 0x24));
        }
        if (*(s32 *)(obj + 0x30) != 0) {
            func_003297C8(*(s32 *)(obj + 0x30));
        }
    }
    func_00328E48(obj);
}

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377D8);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E8);

