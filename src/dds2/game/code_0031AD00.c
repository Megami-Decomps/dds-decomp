#include "common.h"
#include "mnu_shooting.h"
struct EffWorldNode;
extern u32 dds3AdvanceWorldCounter(void);
extern struct EffWorldNode *dds3CreateConfiguredCameraObject(s32 arg0, void *arg1, void *arg2, void *arg3);
extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);
extern void dds3SetCameraVector(struct EffWorldNode *camera, void *vector);
extern void effObjSetInnerFloat(s32 arg0, f32 arg1);
extern s32 dds3GetWorldSecondaryObject(void);
extern struct EffWorldNode *dds3SetWorldCameraObject(struct EffWorldNode *world, struct EffWorldNode *camera);
extern void func_001063A8(f32 arg0);
extern u8 D_0040ABC0[];
extern u8 D_0040ABB0[];
extern u8 D_0040ABD0[];
extern u8 D_00438938[];
extern s32 D_00438944;
extern f32 D_00435A7C;


extern void mnuOverrideActiveNodeModelDepth(MnuNodeList *list, f32 depth);
extern void mnuRestoreActiveNodeModelDepth(MnuNodeList *list);

void itfDispatchObjectFadeSequenceMode(MnuShootingWork *object) {
    switch (object->phase) {
    case 0:
        itfSetFadeMode(&object->roundFade.fade, 1, 8);
        itfQueueFadeMode(&object->roundFade.fade, 0, 8, 0x78);
        func_0031EEE8(&object->roundFade, object->round + 1);
        return;
    case 1:
        mnuDrawFadeSequenceOffset(&object->roundFade);
        return;
    case 5:
        itfDrawFadeGlyphTriplet(&object->scoreFade);
        return;
    case 10:
    case 11:
    case 12:
        mnuDrawFadeSequenceTwo(&object->choiceFade);
        break;
    }
}

void func_0031ADD8(MnuShootingWork *object) {
    MnuNodeList *entries = object->modelWork->groups;
    s32 i = 0;

    if (object->modelWork->count > 0) {
        MnuNodeList *entry = entries;
        do {
            mnuOverrideActiveNodeModelDepth(entry, 0.0f);
            i++;
            entry++;
        } while (i < object->modelWork->count);
    }
}

void func_0031AE48(MnuShootingWork *object) {
    MnuNodeList *entries = object->modelWork->groups;
    s32 i = 0;

    if (object->modelWork->count > 0) {
        MnuNodeList *entry = entries;
        do {
            mnuRestoreActiveNodeModelDepth(entry);
            i++;
            entry++;
        } while (i < object->modelWork->count);
    }
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AEB8);


void func_0031AF58(MnuShootingWork *object) {
    object->choiceIndex = 0;
}

void func_0031AF60(void) {
}

typedef struct SoundSlot SoundSlot;
extern SoundSlot *dds3ClaimSoundSlot(u32 sequence, u32 frames);
extern s8 D_0037F510[];

/* Clamp cursor movement before confirm/cancel override the navigation sound. */
s32 func_0031AF68(MnuShootingWork *object) {
    s32 result = -1;
    s32 sound = 0;

    if (D_0037F510[0x26] < 0) {
        if (--object->choiceIndex < 0) {
            object->choiceIndex = 0;
        } else {
            sound = 1;
        }
    } else if (D_0037F510[0x27] < 0) {
        if (++object->choiceIndex < 3) {
            sound = 1;
        } else {
            object->choiceIndex = 2;
        }
    }
    if (D_0037F510[0x21] < 0) {
        result = object->choiceIndex;
        sound = 2;
    } else if (D_0037F510[0x23] < 0) {
        result = 0;
        sound = 3;
    }
    switch (sound) {
    case 1:
        dds3ClaimSoundSlot(0, 0);
        break;
    case 2:
        dds3ClaimSoundSlot(8, 0);
        break;
    case 3:
        dds3ClaimSoundSlot(10, 0);
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B080);

void func_0031B0F8(void) {
    s32 object;
    u8 *vector = D_0040ABD0;

    object = (s32)dds3CreateConfiguredCameraObject(dds3AdvanceWorldCounter(), D_0040ABC0, vector, D_0040ABB0);
    D_00438944 = object;
    dds3SetWorldNodeValue((struct EffWorldNode *)object, (u32)D_00438938);
    dds3SetCameraVector((struct EffWorldNode *)D_00438944, vector);
    effObjSetInnerFloat(D_00438944, 2.0f);
    object = dds3GetWorldSecondaryObject();
    dds3SetWorldCameraObject((struct EffWorldNode *)object, (struct EffWorldNode *)D_00438944);
    func_001063A8(0.6283184886f);
}
