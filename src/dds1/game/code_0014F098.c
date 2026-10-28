#include "common.h"
#include "scr_integer_parameter_api.h"
#include "evt_unit_object_key.h"
#include "fld_inf.h"
#include "scr.h"
#include "scr_string_parameter_api.h"

/* The setter returns the context it set the value in (scrTraceCode.c). */
extern scrProcessWork_s *scrSetIntReturnValue(s32 retVal);

struct evtUnitHandle_t;



extern const char *fldEveHitResName_Find(dds3ProcessID_t *task);

extern void fldDoor_ExitCamera(const char *entry);

extern s32 fldResolveEventSelectorTarget(s32 index);

extern s32 fldDispatchInteractionAction(s32 param0, s32 param1);

extern s32 fldGetActorSlotAttribute(s32 actorId, FldInfMessageAttribute attribute);

extern void fldInfToWAP(void);

extern s32 fldGetSelectedActorMotionId(void);

extern s32 fldQuerySelectedActorMotionState(s32 value);

extern void fldPauseBgm(void);

extern void fldBgmFadeOut(void);

extern void fldBgmStop(void);

extern void fldPlayFieldSeVolumePan(s32 soundId);

extern void fldPlayFieldSe(s32 soundId);

extern u8 fldPollArchiveLoad(s32 archiveId);

extern void fldSetArchiveSoundVolumePan(s32 archiveId, s32 soundId);

extern void fldPlayArchiveSound(s32 archiveId, s32 soundId);

extern void fldInitializeHuntSparkUnit(struct evtUnitHandle_t *unit);

extern s32 fldFindEffectByName(const char *name);

extern void fldStartTitle(s32 field, s32 mode, s32 option);

extern struct evtUnitHandle_t *evtGetUnitHandle(EvtUnitObjectKey worldObjectKey);


/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */

extern s32 D_0032E48C[];


extern u8 fldDoor_ChkWarp(void);

extern s32 fldCurrentBgmId[];

s32 fldCommand_FLD_WARP_GET_DOOR(void) {
    scrSetIntReturnValue(fldQuerySelectedActorMotionState(scrGetIntPara(0)));
    return 1;
}

extern s32 fldEveHitResID_Find(dds3ProcessID_t *task);
extern void fldApplyActorEntryTrigger(s32 checkTaskRecord);

u32 fldCommand_FLD_WARP_RUN_START(void) {
    s32 scene;
    if (fldDoor_ChkWarp()) {
        scene = 0;
    } else {
        scene = fldEveHitResID_Find(scrGetScriptHandle()->task);
    }
    fldApplyActorEntryTrigger(scene);
    return 1;
}

s32 fldCommand_FLD_INF_NEXT(void) {
    s32 param0 = scrGetIntPara(0);
    s32 param1 = scrGetIntPara(1);

    scrSetIntReturnValue(fldDispatchInteractionAction(param0, param1));
    return 1;
}

s32 fldCommand_FLD_INF_GET(void) {
    s32 actorId = scrGetIntPara(0);
    s32 attribute = scrGetIntPara(1);

    scrSetIntReturnValue(fldGetActorSlotAttribute(actorId, attribute));
    return 1;
}

s32 fldCommand_FLD_INF_GETPANEL(void) {
    return (s32)scrSetIntReturnValue(fldResolveEventSelectorTarget(1));
}

s32 fldCommand_FLD_INF_GOWAP(void) {
    fldInfToWAP();
    return 1;
}

extern void fldStartSceneBgm(void);

s32 fldCmdStartSceneBgm(void) {
    fldCurrentBgmId[0] = scrGetIntPara(0);
    fldStartSceneBgm();
    return 1;
}

extern void fldStartSceneBgmAlternate(void);

s32 fldCmdStartSceneBgmAlternate(void) {
    fldCurrentBgmId[0] = scrGetIntPara(0);
    fldStartSceneBgmAlternate();
    return 1;
}

s32 fldCmdStopCurrentBgm(void) {
    fldPauseBgm();
    return 1;
}

s32 fldCmdPlayCurrentBgmSound(void) {
    fldBgmStop();
    return 1;
}

s32 fldCmdReleaseCurrentBgm(void) {
    fldBgmFadeOut();
    return 1;
}

s32 fldCommandPlaySeVolumePan(void) {
    fldPlayFieldSeVolumePan(scrGetIntPara(0));
    return 1;
}

s32 fldCommandPlaySe(void) {
    fldPlayFieldSe(scrGetIntPara(0));
    return 1;
}

/* Unlike the other field commands, this reports the archive poll result. */
u8 fldCommandLoadArchive(void) {
    return fldPollArchiveLoad(scrGetIntPara(0)) != 0;
}

/* Script command: set volume and pan for a grouped sequence ID. */
s32 fldCommand_FLD_MSE_PLAY2(void) {
    s32 sequenceGroup = scrGetIntPara(0);
    s32 sequenceIndex = scrGetIntPara(1);

    fldSetArchiveSoundVolumePan(sequenceGroup, sequenceIndex);
    return 1;
}

/* Script command: pass the same grouped sequence ID to the other sound path. */
s32 fldCommand_FLD_MSE_STOP2(void) {
    s32 sequenceGroup = scrGetIntPara(0);
    s32 sequenceIndex = scrGetIntPara(1);

    fldPlayArchiveSound(sequenceGroup, sequenceIndex);
    return 1;
}

/* Script command: start a field title using its field ID and display argument. */
s32 fldCommandStartTitle(void) {
    s32 fieldId = scrGetIntPara(0);
    s32 titleArg = scrGetIntPara(1);

    fldStartTitle(fieldId, titleArg, 0x3c);
    return 1;
}

s32 fldCommand_FLD_SET_ENCOUNT(void) {
    s32 value;

    value = scrGetIntPara(0);
    D_0032E48C[0] = value;
    return 1;
}

/* Look up the command's task record before applying its associated entry. */
s32 fldCommand_FLD_EXIT_CAMERA(void) {
    scrProcessWork_s *command = scrGetScriptHandle();
    const char *record = fldEveHitResName_Find(command->task);

    if (record != NULL) {
        fldDoor_ExitCamera(record);
    }
    return 1;
}

s32 fldCommand_FLD_WARP_GET_WARPEFF(void) {
    scrSetIntReturnValue(fldGetSelectedActorMotionId());
    return 1;
}

s32 fldCmdInitializeHuntSparkUnit(void) {
    fldInitializeHuntSparkUnit(evtGetUnitHandle(scrGetIntPara(0)));
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 fldCommandFindEffectByName(void) {
    const char *effectName = scrGetStrPara(0);

    scrSetIntReturnValue(fldFindEffectByName(effectName));
    return 1;
}
