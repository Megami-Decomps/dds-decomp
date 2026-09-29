#include "common.h"

s32 func_00101A70(void);
void func_002CFF98(s32 arg0);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00241E18);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00241F78);

INCLUDE_ASM(const s32, "event/evtEventPack", func_002420B8);

/* Free the event state shared with the motion-sound task. */
void evtFreeEventPackState(void)
{
    func_002CFF98(func_00101A70());
}

void func_002420B8(void);
extern void func_003014F0(char *, char *, ...);
extern void *func_002CFEB8(s32 size);
extern void kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);
extern char D_003AF260[];

/* Allocate a three-word task parameter block and format its "mse_..." name. */
void evtCreateMotionSeTask(s32 taskArg, s32 namePart1, s32 namePart2) {
    char taskName[0x20];
    s32 *params;

    func_003014F0(taskName, D_003AF260, namePart1, namePart2);
    params = func_002CFEB8(0xC);
    memset(params, 0, 0xC);
    params[0] = taskArg;
    params[1] = namePart1;
    params[2] = namePart2;
    kwlnTaskCreate(taskName, 0x3EC, 0, 0, func_002420B8, evtFreeEventPackState, params);
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

/* Handles owned by the event task; +0x38..+0x44 are effect channels,
 * +0x24/+0x30 are scene allocations. */
typedef struct EvtPackResources {
    u8 pad00[8];
    s32 objectHandle;        /* 0x08 */
    s32 resourceHandle;      /* 0x0C */
    u8 pad10[0x14];
    s32 sceneAllocation1;    /* 0x24 */
    u8 pad28[8];
    s32 sceneAllocation2;    /* 0x30 */
    u8 pad34[4];
    s32 effect72;            /* 0x38 */
    s32 effect71;            /* 0x3C */
    s32 effect76;            /* 0x40 */
    s32 effect75;            /* 0x44 */
} EvtPackResources;

/* Release the event task's owned handles, then free its state. */
void evtReleaseEventPackResources(void) {
    s32 state = func_00101A70();
    EvtPackResources *resources = (EvtPackResources *)state;

    func_00288C68();
    if (state != 0) {
        if (resources->effect72 != 0) {
            effInitCh72Id();
            func_002D2D00(resources->effect72);
        }
        if (resources->effect71 != 0) {
            effInitCh71Id();
            func_002D2D00(resources->effect71);
        }
        if (resources->effect76 != 0) {
            effInitCh76Id();
            func_002D2D00(resources->effect76);
        }
        if (resources->effect75 != 0) {
            effInitCh75Id();
            func_002D2D00(resources->effect75);
        }
        if (resources->objectHandle != 0) {
            func_002887A0(resources->objectHandle);
        }
        if (resources->resourceHandle != 0) {
            func_002D0A10(resources->resourceHandle);
        }
        if (resources->sceneAllocation1 != 0) {
            func_002D0918(resources->sceneAllocation1);
        }
        if (resources->sceneAllocation2 != 0) {
            func_002D0918(resources->sceneAllocation2);
        }
    }
    func_002CFF98(state);
}

INCLUDE_SDATA(const s32, "event/evtEventPack", D_003BC368);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_003BC370);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_003BC378);

