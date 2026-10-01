#include "common.h"

extern u64 sdfSoundIsCommandBusy(void);

extern u64 fldGetCurrentSceneSelectionResource(void);

extern s32 D_0032E4E4[];

u32 fldCmdGetCurrentSceneSelectionResource(void) {
    u64 value;

    value = fldGetCurrentSceneSelectionResource();
    scrSetIntegerReturnValue(value);
    return 1;
}

extern s32 fldAreaState[];

extern s32 scrReadIntParameter(s32);

/* Clear the selected flag only if it was set, and report whether it changed. */
s32 fldCommandClearSelectedFlag(void) {
    s32 changed = 0;
    switch (scrReadIntParameter(0)) {
    case 0:
        if (fldAreaState[3] & 1) {
            fldAreaState[3] &= ~1;
            changed = 1;
        }
        break;
    case 1:
        if (fldAreaState[3] & 2) {
            fldAreaState[3] &= ~2;
            changed = 1;
        }
        break;
    case 2:
        if (fldAreaState[3] & 4) {
            fldAreaState[3] &= ~4;
            changed = 1;
        }
        break;
    case 3:
        if (fldAreaState[3] & 8) {
            fldAreaState[3] &= ~8;
            changed = 1;
        }
        break;
    }
    scrSetIntegerReturnValue(changed);
    return 1;
}

extern s32 effMiscRandMod(s32, s32);

extern s32 evtGetMirroredSolarPhase(void);

extern s32 fldMirroredSolarThresholds[];

/* Roll against the threshold associated with the mirrored solar phase. */
s32 fldCmdRollMirroredSolarThreshold(void) {
    s32 solarPhaseThreshold = fldMirroredSolarThresholds[evtGetMirroredSolarPhase()];
    if (solarPhaseThreshold >= effMiscRandMod(0, 100)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

/* Handle an occupied sound-command channel before dispatching a named sound. */
s32 fldCommandSendNamedSound(void) {
    s32 commandName;
    commandName = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        func_002E97E8();
    }
    sdfSoundSendNamedCommand(commandName, 0x7f);
    return 1;
}

u32 fldCommandSendSoundControl(void) {
    func_002E97E8();
    return 1;
}

/* Return sound-command busy state to the field script interpreter. */
u32 fldCommandIsSoundBusy(void) {
    u64 busy;

    busy = sdfSoundIsCommandBusy();
    scrSetIntegerReturnValue(busy);
    return 1;
}

extern s32 D_0032E3C0[];

extern s32 scrReadIntParameter(s32);

extern void func_00121DE0(s32, s32, s32, s32, s32, s32);

/* Pass five field-script arguments to the underlying handler. */
s32 fldCmdMarkBitmapRegion(void) {
    s32 first = scrReadIntParameter(0);
    s32 second = scrReadIntParameter(1);
    s32 third = scrReadIntParameter(2);
    s32 fourth = scrReadIntParameter(3);
    func_00121DE0(D_0032E3C0[0], first, second, third, fourth, scrReadIntParameter(4));
    return 1;
}

s32 func_0014F780(void) {
    D_0032E4E4[0] = 1;
    return 1;
}

extern void func_002E84A0(u8 *);

extern void fileManagerResetSubsystems(void);

extern void func_00150040(void);

extern void parSysReset(void);

extern void func_00153680(void);

extern void kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern void func_0014F860(void);

extern void effManagerUpdateAndDispatch(void);

extern void effManagerInitializeSubsystems(void);

extern void func_0018CE38(void);

extern void effInitWorks(void);

extern void effBTLFieldColorResetFlags(void);

extern u8 D_0034DF38[];

extern char D_003BB008[];

void fldCreateFieldEffectTask(void) {
    func_002E84A0(D_0034DF38);
    fileManagerResetSubsystems();
    func_00150040();
    parSysReset();
    func_00153680();
    kwlnTaskCreate("effect_f", 0x2B04, 0, 0, func_0014F860, NULL, 0);
    kwlnTaskCreate(D_003BB008, 0x2B18, 0, 0, effManagerUpdateAndDispatch, effManagerInitializeSubsystems, 0);
    func_0018CE38();
    effInitWorks();
    effBTLFieldColorResetFlags();
}

INCLUDE_SDATA(const s32, "game/code_0014F4C8", D_003BB008);

