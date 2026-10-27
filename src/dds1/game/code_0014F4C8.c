#include "fld_area_work.h"
#include "common.h"
#include "scr_integer_parameter_api.h"
#include "scr_integer_return_value_api.h"
#include "scr_string_parameter_api.h"
#include "evt_solar.h"
#include "eff_random_mod.h"
#include "kwln_task_create.h"
#include "eff_manager_task.h"
#include "eff_random.h"
#include "mnu_title_sound.h"

extern s32 fldGetCurrentSceneSelectionResource(void);

extern s32 D_0032E4E4[];

u32 fldCmdGetCurrentSceneSelectionResource(void) {
    s32 selectionResource;

    selectionResource = fldGetCurrentSceneSelectionResource();
    scrSetIntReturnValue(selectionResource);
    return 1;
}

/* Clear the selected flag only if it was set, and report whether it changed. */
s32 fldCommand_FLD_GET_KIRETA(void) {
    s32 changed = 0;
    switch (scrGetIntPara(0)) {
    case 0:
        if (fldGlobalBase.consumedFlags & 1) {
            fldGlobalBase.consumedFlags &= ~1;
            changed = 1;
        }
        break;
    case 1:
        if (fldGlobalBase.consumedFlags & 2) {
            fldGlobalBase.consumedFlags &= ~2;
            changed = 1;
        }
        break;
    case 2:
        if (fldGlobalBase.consumedFlags & 4) {
            fldGlobalBase.consumedFlags &= ~4;
            changed = 1;
        }
        break;
    case 3:
        if (fldGlobalBase.consumedFlags & 8) {
            fldGlobalBase.consumedFlags &= ~8;
            changed = 1;
        }
        break;
    }
    scrSetIntReturnValue(changed);
    return 1;
}



extern s32 fldMirroredSolarThresholds[];

/* Roll against the threshold associated with the mirrored solar phase. */
s32 fldCmdRollMirroredSolarThreshold(void) {
    s32 solarPhaseThreshold = fldMirroredSolarThresholds[evtGetMirroredSolarPhase()];
    if (solarPhaseThreshold >= (s32)dds3GetRandIntA(0, 100)) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

extern s32 p2sdLoadPcmFile(const char *name, u8 channel);
extern void p2sdSyncSound(void);

/* Handle an occupied sound-command channel before dispatching a named sound. */
s32 fldCommandSendNamedSound(void) {
    const char *commandName;
    commandName = scrGetStrPara(0);
    if (p2sdGetPcmState() != 0) {
        p2sdSyncSound();
    }
    p2sdLoadPcmFile(commandName, 0x7f);
    return 1;
}

u32 fldCommandSendSoundControl(void) {
    p2sdSyncSound();
    return 1;
}

/* Return sound-command busy state to the field script interpreter. */
u32 fldCommandIsSoundBusy(void) {
    u32 busy;

    busy = p2sdGetPcmState();
    scrSetIntReturnValue(busy);
    return 1;
}

extern s32 D_0032E3C0[];

extern void fldMarkStageExplorationBitmapRegion(s32, s32, s32, s32, s32, s32);

/* Pass five field-script arguments to the underlying handler. */
s32 fldCmdMarkBitmapRegion(void) {
    s32 first = scrGetIntPara(0);
    s32 second = scrGetIntPara(1);
    s32 third = scrGetIntPara(2);
    s32 fourth = scrGetIntPara(3);
    fldMarkStageExplorationBitmapRegion(D_0032E3C0[0], first, second, third, fourth, scrGetIntPara(4));
    return 1;
}

s32 fldCmdSetStateFlag(void) {
    D_0032E4E4[0] = 1;
    return 1;
}

extern EffRandState effDefaultRandomState[];

extern void fileManagerResetSubsystems(void);

extern void effInitializeBillResourceOwners(void);

extern void dds3PolygonManagerInit(void);

extern void dds3ParticleManagerInit(void);

extern void effFieldTaskStartupHook(void);

extern void Eve_EffectInit(void);

extern void effBTLFieldColorResetFlags(void);

extern EffRandState effDefaultRandomState[];

extern char D_003BB008[];

void fldCreateFieldEffectTask(void) {
    sdfRandomizeByTime((EffRandState *)effDefaultRandomState);
    fileManagerResetSubsystems();
    effInitializeBillResourceOwners();
    dds3PolygonManagerInit();
    dds3ParticleManagerInit();
    dds3StartProcess("effect_f", 0x2B04, 0, 0, effSubmitFieldBlendStateTask, NULL, 0);
    dds3StartProcess(D_003BB008, 0x2B18, 0, 0, dds3EffectManagerRun, effManagerInitializeSubsystems, 0);
    effFieldTaskStartupHook();
    Eve_EffectInit();
    effBTLFieldColorResetFlags();
}

INCLUDE_SDATA(const s32, "game/code_0014F4C8", D_003BB008);
