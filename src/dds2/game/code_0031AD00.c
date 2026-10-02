#include "common.h"
extern s32 dds3AdvanceWorldCounter(void);
extern s32 dds3CreateConfiguredCameraObject(s32 arg0, void *arg1, void *arg2, void *arg3);
extern void dds3SetWorldEntryCallbackTarget(s32 arg0, void *arg1);
extern void dds3SetCameraVector(s32 arg0, void *arg1);
extern void effObjSetInnerFloat(s32 arg0, f32 arg1);
extern s32 dds3GetWorldSecondaryObject(void);
extern void dds3SetWorldCameraObject(s32 arg0, s32 arg1);
extern void func_001063A8(f32 arg0);
extern u8 D_0040ABC0[];
extern u8 D_0040ABB0[];
extern u8 D_0040ABD0[];
extern u8 D_00438938[];
extern s32 D_00438944;
extern f32 D_00435A7C;

extern void itfSetFadeMode(void *, s32, s32);
extern void itfQueueFadeMode(void *, u32, u32, u32);
extern void func_0031EEE8(void *, u32);
extern void mnuDrawFadeSequenceOffset(void *);
extern void itfDrawFadeGlyphTriplet(void *);
extern void mnuDrawFadeSequenceTwo(void *);

typedef struct MnuDepthNodeList {
    void *nodes;
    s32 count;
} MnuDepthNodeList;

typedef struct MnuDepthNodeEntry {
    MnuDepthNodeList list;
    u32 unk8;
    u32 unkC;
} MnuDepthNodeEntry;

typedef struct MnuDepthEntryArray {
    u32 unk0;
    s32 count;
    MnuDepthNodeEntry *entries;
} MnuDepthEntryArray;

typedef struct MnuDepthObject {
    u8 unk00[0x1C];
    MnuDepthEntryArray *depthEntries;
} MnuDepthObject;

extern void mnuOverrideActiveNodeModelDepth(MnuDepthNodeList *list, f32 depth);
extern void mnuRestoreActiveNodeModelDepth(MnuDepthNodeList *list);

void itfDispatchObjectFadeSequenceMode(u8 *object) {
    s16 mode = *(s16 *)(object + 0x98);

    switch (mode) {
    case 0:
        itfSetFadeMode(object + 0x168, 1, 8);
        itfQueueFadeMode(object + 0x168, 0, 8, 0x78);
        func_0031EEE8(object + 0x168,
                      *(s16 *)(object + 0x96) + 1);
        return;
    case 1:
        mnuDrawFadeSequenceOffset(object + 0x168);
        return;
    case 5:
        itfDrawFadeGlyphTriplet(object + 0x184);
        return;
    case 10:
    case 11:
    case 12:
        mnuDrawFadeSequenceTwo(object + 0x19C);
        break;
    }
}

void func_0031ADD8(MnuDepthObject *object) {
    MnuDepthNodeEntry *entries = object->depthEntries->entries;
    s32 i = 0;

    if (object->depthEntries->count > 0) {
        MnuDepthNodeEntry *entry = entries;
        do {
            mnuOverrideActiveNodeModelDepth(&entry->list, 0.0f);
            i++;
            entry++;
        } while (i < object->depthEntries->count);
    }
}

void func_0031AE48(MnuDepthObject *object) {
    MnuDepthNodeEntry *entries = object->depthEntries->entries;
    s32 i = 0;

    if (object->depthEntries->count > 0) {
        MnuDepthNodeEntry *entry = entries;
        do {
            mnuRestoreActiveNodeModelDepth(&entry->list);
            i++;
            entry++;
        } while (i < object->depthEntries->count);
    }
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AEB8);

void func_0031AF58(s32 object) {
    *(u32 *)(object + 0x1d4) = 0;
}

void func_0031AF60(void) {
}

typedef struct FadeSoundWork {
    u8 pad00[0x1D4];
    s32 cursor;
} FadeSoundWork;

typedef struct SoundSlot SoundSlot;

extern s8 D_0037F510[];
extern SoundSlot *sndClaimFreeSoundSlot(u32 sequence, u32 frames);

/* Step the bounded sound cursor from pad input and play the matching cue. */
s32 func_0031AF68(FadeSoundWork *work) {
    s32 result = -1;
    s32 action = 0;

    if (D_0037F510[0x26] < 0) {
        work->cursor--;
        if (work->cursor >= 0) {
            action = 1;
        } else {
            work->cursor = 0;
        }
    } else if (D_0037F510[0x27] < 0) {
        work->cursor++;
        if (work->cursor < 3) {
            action = 1;
        } else {
            work->cursor = 2;
        }
    }

    if (D_0037F510[0x21] < 0) {
        result = work->cursor;
        action = 2;
    } else if (D_0037F510[0x23] < 0) {
        result = 0;
        action = 3;
    }

    switch (action) {
    case 1:
        sndClaimFreeSoundSlot(0, 0);
        break;
    case 2:
        sndClaimFreeSoundSlot(8, 0);
        break;
    case 3:
        sndClaimFreeSoundSlot(10, 0);
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B080);

void func_0031B0F8(void) {
    s32 object;
    u8 *vector = D_0040ABD0;

    object = dds3CreateConfiguredCameraObject(dds3AdvanceWorldCounter(), D_0040ABC0, vector, D_0040ABB0);
    D_00438944 = object;
    dds3SetWorldEntryCallbackTarget(object, D_00438938);
    dds3SetCameraVector(D_00438944, vector);
    effObjSetInnerFloat(D_00438944, 2.0f);
    object = dds3GetWorldSecondaryObject();
    dds3SetWorldCameraObject(object, D_00438944);
    func_001063A8(0.6283184886f);
}
