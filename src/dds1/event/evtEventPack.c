#include "common.h"

s32 func_00101A70(void);
void func_002CFF98(s32 arg0);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00241E18);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00241F78);

INCLUDE_ASM(const s32, "event/evtEventPack", func_002420B8);

void evtFreeEventPackState(void)
{
    func_002CFF98(func_00101A70());
}

void func_002420B8(void);
extern void func_003014F0(char *, char *, ...);
extern void *func_002CFEB8(s32 size);
extern void kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);
extern char D_003AF260[];

/* Build the "mse_<a1>_<a2>" task name, allocate a 3-word param block, and
   spawn the motion-SE task (update func_002420B8, free evtFreeEventPackState). */
void func_00242298(s32 a0, s32 a1, s32 a2) {
    char buf[0x20];
    s32 *obj;

    func_003014F0(buf, D_003AF260, a1, a2);
    obj = func_002CFEB8(0xC);
    memset(obj, 0, 0xC);
    obj[0] = a0;
    obj[1] = a1;
    obj[2] = a2;
    kwlnTaskCreate(buf, 0x3EC, 0, 0, func_002420B8, evtFreeEventPackState, obj);
}

INCLUDE_RODATA(const s32, "event/evtEventPack", D_003AF260);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00242340);

INCLUDE_ASM(const s32, "event/evtEventPack", func_002423C8);

INCLUDE_ASM(const s32, "event/evtEventPack", func_002424B0);

extern void func_00288C68(void);
extern void effInitCh72Id(void);
extern void effInitCh71Id(void);
extern void effInitCh76Id(void);
extern void effInitCh75Id(void);
extern void func_002D2D00(s32);
extern void func_002887A0(s32);
extern void func_002D0A10(s32);
extern void func_002D0918(s32);

/* Release every effect/resource the battle-event work holds, then free it. */
void func_00242510(void) {
    s32 obj = func_00101A70();

    func_00288C68();
    if (obj != 0) {
        if (*(s32 *)(obj + 0x38) != 0) {
            effInitCh72Id();
            func_002D2D00(*(s32 *)(obj + 0x38));
        }
        if (*(s32 *)(obj + 0x3C) != 0) {
            effInitCh71Id();
            func_002D2D00(*(s32 *)(obj + 0x3C));
        }
        if (*(s32 *)(obj + 0x40) != 0) {
            effInitCh76Id();
            func_002D2D00(*(s32 *)(obj + 0x40));
        }
        if (*(s32 *)(obj + 0x44) != 0) {
            effInitCh75Id();
            func_002D2D00(*(s32 *)(obj + 0x44));
        }
        if (*(s32 *)(obj + 8) != 0) {
            func_002887A0(*(s32 *)(obj + 8));
        }
        if (*(s32 *)(obj + 0xC) != 0) {
            func_002D0A10(*(s32 *)(obj + 0xC));
        }
        if (*(s32 *)(obj + 0x24) != 0) {
            func_002D0918(*(s32 *)(obj + 0x24));
        }
        if (*(s32 *)(obj + 0x30) != 0) {
            func_002D0918(*(s32 *)(obj + 0x30));
        }
    }
    func_002CFF98(obj);
}

INCLUDE_SDATA(const s32, "event/evtEventPack", D_003BC368);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_003BC370);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_003BC378);

