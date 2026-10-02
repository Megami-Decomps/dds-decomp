#include "common.h"

extern char *scrReadStringParameter(s32 idx);

extern void scrSetIntegerReturnValue(s32 value);

extern s32 fldGetTaskRecordValue(u32 key);

extern void func_0013DDF0(void *entry);

extern s32 func_0013CBA8(s32 index);

extern s32 func_0013CEB0(s32 param0, s32 param1);

extern s32 fldGetActorSlotAttribute(s32 param0, s32 param1);

extern void func_0013D650(void);

extern s32 fldGetSelectedActorMotionId(void);

extern s32 fldQuerySelectedActorMotionState(s32 value);

extern void fldStopCurrentBgm(void);

extern void fldReleaseCurrentBgm(void);

extern void fldPlayCurrentBgmSound(void);

extern void fldPlayFieldSeVolumePan(s32 param);

extern void fldPlayFieldSe(s32 param);

extern s32 fldPollArchiveLoad(s32 param);

extern void fldSetArchiveSoundVolumePan(s32 param0, s32 param1);

extern void fldPlayArchiveSound(s32 param0, s32 param1);

extern void func_00147DB0(s32 handle);

extern s32 fldFindEffectByName(char *str);

extern void fldStartTitle(s32 param0, s32 param1, s32 param2);

extern s32 evtGetWorldUnitNestedValue(s32 param);

/* Script command context: +0xE4 is the task-record lookup key. */
typedef struct {
    u8 pad00[0xE4];
    u32 key;
} FldCommandWork;

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
extern s32 scrReadIntParameter(s32);

extern s32 D_0032E48C[];

extern s32 scrGetCurrentContext(void);

extern s32 fldIsSceneStateEight(void);

extern s32 fldCurrentBgmId[];

s32 fldCmdQuerySceneValue(void) {
    scrSetIntegerReturnValue(fldQuerySelectedActorMotionState(scrReadIntParameter(0)));
    return 1;
}

u32 fldCmdUpdateTaskRecordScene(void) {
    s32 scene;
    if (fldIsSceneStateEight()) {
        scene = 0;
    } else {
        scene = fldFindTaskRecordId(((FldCommandWork *)scrGetCurrentContext())->key);
    }
    fldApplyActorEntryTrigger(scene);
    return 1;
}

s32 func_0014F110(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    scrSetIntegerReturnValue(func_0013CEB0(param0, param1));
    return 1;
}

s32 fldCmdGetActorSlotAttribute(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    scrSetIntegerReturnValue(fldGetActorSlotAttribute(param0, param1));
    return 1;
}

void func_0014F1A0(void) {
    scrSetIntegerReturnValue(func_0013CBA8(1));
}

s32 func_0014F1C0(void) {
    func_0013D650();
    return 1;
}

s32 fldCmdStartSceneBgm(void) {
    fldCurrentBgmId[0] = scrReadIntParameter(0);
    fldStartSceneBgm();
    return 1;
}

s32 fldCmdStartSceneBgmAlternate(void) {
    fldCurrentBgmId[0] = scrReadIntParameter(0);
    fldStartSceneBgmAlternate();
    return 1;
}

s32 fldCmdStopCurrentBgm(void) {
    fldStopCurrentBgm();
    return 1;
}

s32 fldCmdPlayCurrentBgmSound(void) {
    fldPlayCurrentBgmSound();
    return 1;
}

s32 fldCmdReleaseCurrentBgm(void) {
    fldReleaseCurrentBgm();
    return 1;
}

s32 fldCommandPlaySeVolumePan(void) {
    fldPlayFieldSeVolumePan(scrReadIntParameter(0));
    return 1;
}

s32 fldCommandPlaySe(void) {
    fldPlayFieldSe(scrReadIntParameter(0));
    return 1;
}

/* Unlike the other field commands, this reports the archive poll result. */
u8 fldCommandLoadArchive(void) {
    return fldPollArchiveLoad(scrReadIntParameter(0)) != 0;
}

/* Script command: set volume and pan for a grouped sequence ID. */
s32 fldCmdSetArchiveSoundVolumePan(void) {
    s32 sequenceGroup = scrReadIntParameter(0);
    s32 sequenceIndex = scrReadIntParameter(1);

    fldSetArchiveSoundVolumePan(sequenceGroup, sequenceIndex);
    return 1;
}

/* Script command: pass the same grouped sequence ID to the other sound path. */
s32 fldCmdPlayArchiveSound(void) {
    s32 sequenceGroup = scrReadIntParameter(0);
    s32 sequenceIndex = scrReadIntParameter(1);

    fldPlayArchiveSound(sequenceGroup, sequenceIndex);
    return 1;
}

/* Script command: start a field title using its field ID and display argument. */
s32 fldCommandStartTitle(void) {
    s32 fieldId = scrReadIntParameter(0);
    s32 titleArg = scrReadIntParameter(1);

    fldStartTitle(fieldId, titleArg, 0x3c);
    return 1;
}

s32 func_0014F3E0(void) {
    s32 value;

    value = scrReadIntParameter(0);
    D_0032E48C[0] = value;
    return 1;
}

/* Look up the command's task record before applying its associated entry. */
s32 fldCmdApplyTaskRecordEntry(void) {
    FldCommandWork *command = scrGetCurrentContext();
    void *record = fldGetTaskRecordValue(command->key);

    if (record != NULL) {
        func_0013DDF0(record);
    }
    return 1;
}

s32 fldScriptReturnSelectedActorMotionId(void) {
    scrSetIntegerReturnValue(fldGetSelectedActorMotionId());
    return 1;
}

s32 func_0014F468(void) {
    func_00147DB0(evtGetWorldUnitNestedValue(scrReadIntParameter(0)));
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 fldCommandFindEffectByName(void) {
    char *effectName = scrReadStringParameter(0);

    scrSetIntegerReturnValue(fldFindEffectByName(effectName));
    return 1;
}
