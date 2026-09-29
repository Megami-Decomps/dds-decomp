#include "common.h"

extern char *func_0010D5A8(s32 idx);

extern void func_0010D5F0(s32 value);

extern s32 fldGetTaskRecordValue(u32 key);

extern void func_0013DDF0(void *entry);

extern s32 func_0013CBA8(s32 index);

extern s32 func_0013CEB0(s32 param0, s32 param1);

extern s32 func_0013D410(s32 param0, s32 param1);

extern void func_0013D650(void);

extern s32 func_0013DB28(void);

extern s32 func_0013DB58(s32 value);

extern void func_00141D18(void);

extern void func_00141D40(void);

extern void func_00141D98(void);

extern void fldPlayFieldSeVolumePan(s32 param);

extern void fldPlayFieldSe(s32 param);

extern s32 fldPollArchiveLoad(s32 param);

extern void func_001421D0(s32 param0, s32 param1);

extern void func_00142200(s32 param0, s32 param1);

extern void func_00147DB0(s32 handle);

extern s32 fldFindEffectByName(char *str);

extern void fldStartTitle(s32 param0, s32 param1, s32 param2);

extern s32 func_00222090(s32 param);

/* Script command context: +0xE4 is the task-record lookup key. */
typedef struct {
    u8 pad00[0xE4];
    u32 key;
} FldCommandWork;

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
extern s32 func_0010D428(s32);

extern s32 D_0032E48C[];

extern s32 func_0010D6A0(void);

extern s32 func_0013DF18(void);

extern s32 D_0032E3D8[];

s32 func_0014F098(void) {
    func_0010D5F0(func_0013DB58(func_0010D428(0)));
    return 1;
}

u32 func_0014F0C8(void) {
    s32 scene;
    if (func_0013DF18()) {
        scene = 0;
    } else {
        scene = fldFindTaskRecordId(((FldCommandWork *)func_0010D6A0())->key);
    }
    func_0013DC08(scene);
    return 1;
}

s32 func_0014F110(void) {
    s32 param0 = func_0010D428(0);
    s32 param1 = func_0010D428(1);

    func_0010D5F0(func_0013CEB0(param0, param1));
    return 1;
}

s32 func_0014F158(void) {
    s32 param0 = func_0010D428(0);
    s32 param1 = func_0010D428(1);

    func_0010D5F0(func_0013D410(param0, param1));
    return 1;
}

void func_0014F1A0(void) {
    func_0010D5F0(func_0013CBA8(1));
}

s32 func_0014F1C0(void) {
    func_0013D650();
    return 1;
}

s32 func_0014F1E0(void) {
    D_0032E3D8[0] = func_0010D428(0);
    func_00141B10();
    return 1;
}

s32 func_0014F210(void) {
    D_0032E3D8[0] = func_0010D428(0);
    func_00141C40();
    return 1;
}

s32 func_0014F240(void) {
    func_00141D18();
    return 1;
}

s32 func_0014F260(void) {
    func_00141D98();
    return 1;
}

s32 func_0014F280(void) {
    func_00141D40();
    return 1;
}

s32 fldCommandPlaySeVolumePan(void) {
    fldPlayFieldSeVolumePan(func_0010D428(0));
    return 1;
}

s32 fldCommandPlaySe(void) {
    fldPlayFieldSe(func_0010D428(0));
    return 1;
}

/* Unlike the other field commands, this reports the archive poll result. */
u8 fldCommandLoadArchive(void) {
    return fldPollArchiveLoad(func_0010D428(0)) != 0;
}

/* Script command: set volume and pan for a grouped sequence ID. */
s32 func_0014F318(void) {
    s32 sequenceGroup = func_0010D428(0);
    s32 sequenceIndex = func_0010D428(1);

    func_001421D0(sequenceGroup, sequenceIndex);
    return 1;
}

/* Script command: pass the same grouped sequence ID to the other sound path. */
s32 func_0014F358(void) {
    s32 sequenceGroup = func_0010D428(0);
    s32 sequenceIndex = func_0010D428(1);

    func_00142200(sequenceGroup, sequenceIndex);
    return 1;
}

/* Script command: start a field title using its field ID and display argument. */
s32 func_0014F398(void) {
    s32 fieldId = func_0010D428(0);
    s32 titleArg = func_0010D428(1);

    fldStartTitle(fieldId, titleArg, 0x3c);
    return 1;
}

s32 func_0014F3E0(void) {
    s32 value;

    value = func_0010D428(0);
    D_0032E48C[0] = value;
    return 1;
}

/* Look up the command's task record before applying its associated entry. */
s32 func_0014F408(void) {
    FldCommandWork *command = func_0010D6A0();
    void *record = fldGetTaskRecordValue(command->key);

    if (record != NULL) {
        func_0013DDF0(record);
    }
    return 1;
}

s32 func_0014F440(void) {
    func_0010D5F0(func_0013DB28());
    return 1;
}

s32 func_0014F468(void) {
    func_00147DB0(func_00222090(func_0010D428(0)));
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 fldCommandFindEffectByName(void) {
    char *effectName = func_0010D5A8(0);

    func_0010D5F0(fldFindEffectByName(effectName));
    return 1;
}
