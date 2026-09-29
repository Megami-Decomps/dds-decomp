#include "common.h"

extern char *func_0010D5A8(s32 idx);

extern void func_0010D5F0(s32 value);

extern void *func_0010D6A0(void);

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

extern s32 fldLoadArchive(s32 param);

extern void func_001421D0(s32 param0, s32 param1);

extern void func_00142200(s32 param0, s32 param1);

extern void func_00147DB0(s32 handle);

extern s32 fldFindEffectByName(char *str);

extern void fldStartTitle(s32 param0, s32 param1, s32 param2);

extern s32 func_00222090(s32 param);

/* Work object queried by func_0014F408; +0xE4 holds the key for fldGetTaskRecordValue. */
typedef struct {
    u8 unk00[0xE4]; /* 0x00 */
    u32 key;        /* 0xE4 */
} EffCmdWork;

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
extern s32 func_0010D428(s32);

extern s32 D_0032E48C[];

s32 func_0014F098(void) {
    func_0010D5F0(func_0013DB58(func_0010D428(0)));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0014F098", func_0014F0C8);

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

INCLUDE_ASM(const s32, "game/code_0014F098", func_0014F1E0);

INCLUDE_ASM(const s32, "game/code_0014F098", func_0014F210);

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

s32 func_0014F2A0(void) {
    fldPlayFieldSeVolumePan(func_0010D428(0));
    return 1;
}

s32 func_0014F2C8(void) {
    fldPlayFieldSe(func_0010D428(0));
    return 1;
}

u8 func_0014F2F0(void) {
    return fldLoadArchive(func_0010D428(0)) != 0;
}

s32 func_0014F318(void) {
    s32 param0 = func_0010D428(0);
    s32 param1 = func_0010D428(1);

    func_001421D0(param0, param1);
    return 1;
}

s32 func_0014F358(void) {
    s32 param0 = func_0010D428(0);
    s32 param1 = func_0010D428(1);

    func_00142200(param0, param1);
    return 1;
}

s32 func_0014F398(void) {
    s32 param0 = func_0010D428(0);
    s32 param1 = func_0010D428(1);

    fldStartTitle(param0, param1, 0x3c);
    return 1;
}

s32 func_0014F3E0(void) {
    s32 value;

    value = func_0010D428(0);
    D_0032E48C[0] = value;
    return 1;
}

s32 func_0014F408(void) {
    EffCmdWork *work = func_0010D6A0();
    void *entry = fldGetTaskRecordValue(work->key);

    if (entry != NULL) {
        func_0013DDF0(entry);
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
s32 func_0014F498(void) {
    char *param = func_0010D5A8(0);

    func_0010D5F0(fldFindEffectByName(param));
    return 1;
}
