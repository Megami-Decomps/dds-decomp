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

/* Allocate a three-word task parameter block and format its "mse_..." name. */
void evtCreateMotionSeTask(s32 taskArg, s32 namePart1, s32 namePart2) {
    char taskName[0x20];
    s32 *params;

    func_0035C860(taskName, D_00424890, namePart1, namePart2);
    params = func_00328D68(0xC);
    memset(params, 0, 0xC);
    params[0] = taskArg;
    params[1] = namePart1;
    params[2] = namePart2;
    kwlnTaskCreate(taskName, 0x3EC, 0, 0, func_0025D4D0, evtFreeEventPackState, params);
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

/* Keep this task resource layout identical to DDS1's EvtPackResources. */
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

/* Release the owned effect channels and handles before freeing the task. */
void evtReleaseEventPackResources(void) {
    s32 obj = func_00101958();
    EvtPackResources *resources = (EvtPackResources *)obj;

    func_002C81E8();
    if (obj != 0) {
        if (resources->effect72 != 0) {
            effInitCh72Id();
            func_0032BBB0(resources->effect72);
        }
        if (resources->effect71 != 0) {
            effInitCh71Id();
            func_0032BBB0(resources->effect71);
        }
        if (resources->effect76 != 0) {
            effInitCh76Id();
            func_0032BBB0(resources->effect76);
        }
        if (resources->effect75 != 0) {
            effInitCh75Id();
            func_0032BBB0(resources->effect75);
        }
        if (resources->objectHandle != 0) {
            func_002C7D00(resources->objectHandle);
        }
        if (resources->resourceHandle != 0) {
            func_003298C0(resources->resourceHandle);
        }
        if (resources->sceneAllocation1 != 0) {
            func_003297C8(resources->sceneAllocation1);
        }
        if (resources->sceneAllocation2 != 0) {
            func_003297C8(resources->sceneAllocation2);
        }
    }
    func_00328E48(obj);
}

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377D8);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E8);

