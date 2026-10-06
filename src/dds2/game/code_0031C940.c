#include "common.h"


extern f32 func_00353228(f32);
extern f32 D_0037F5EC[];
extern f32 sdfViewEyeVector[4];
extern f32 sdfViewTargetVector[4];

/* Records are 0x34 bytes; bit 0 of flags marks a claimed slot. */
typedef struct ModelInstance {
    f32 position[3];
    u32 valueC;
    f32 positionStep[3];
    u8 pad1C[4];
    u32 flags;
    u16 remainingLifetime; /* decremented by each unpaused timed update */
    u16 initialLifetime; /* retained as the draw ratio denominator; zero disables timed motion */
    s16 animationFrame;
    s16 animationLength;
    u8 pad2C[4];
    f32 scale;
} ModelInstance;

void itfDeactivateModelInstance(ModelInstance *item);
void itfDrawModelInstanceImage(ModelInstance *item);
extern s32 itfDrawUniformlyScaledIndexedImage(s32, s32, s32, s32, s32, s32, s32, f32);

typedef struct ModelInstanceList {
    ModelInstance *items;
    s32 count;
} ModelInstanceList;

typedef struct ModelInstanceWork {
    u32 handle;
    s32 count;
    ModelInstanceList *lists;
    u32 unkC;
} ModelInstanceWork;

extern u32 sdfAllocGeneralBlock(s32);
extern void *sdfMemoryGetBlockAddress(u32);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031C940);

/* Map a screen point onto the view plane using the eye/target Z separation. */
void itfProjectScreenPointToViewPlane(f32 *position, s32 screenX, s32 screenY) {
    f32 halfViewAngle;
    f32 eyeTargetDepth;

    halfViewAngle = D_0037F5EC[0] * 0.5f;
    eyeTargetDepth = sdfViewEyeVector[2] - sdfViewTargetVector[2];
    position[0] = eyeTargetDepth * func_00353228(halfViewAngle * 1.3f) *
        ((f32)(screenX - 256) * 0.00390625f);
    position[1] = eyeTargetDepth * func_00353228(halfViewAngle) *
        ((f32)(screenY - 224) / 224.0f);
    position[2] = 0.0f;
}

/* Variant of the screen-point projection with a fixed -300 eye depth and Z. */
void func_0031CAE8(f32 *position, s32 screenX, s32 screenY) {
    f32 halfViewAngle;
    f32 depth;

    halfViewAngle = D_0037F5EC[0] * 0.5f;
    depth = -300.0f - sdfViewTargetVector[2];
    position[0] = depth * func_00353228(halfViewAngle * 1.3f) *
        ((f32)(screenX - 256) * 0.00390625f);
    position[1] = depth * func_00353228(halfViewAngle) *
        ((f32)(screenY - 224) / 224.0f);
    position[2] = -300.0f;
}

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CBC8);

extern u32 D_0045C840[];
void func_0031CDE8(u32 *source, s32 count) {
    s32 i;

    memset(D_0045C840, 0, 0x1C);
    for (i = 0; i < count; i++) {
        D_0045C840[i] = source[i];
    }
}

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CE60);

INCLUDE_ASM(const s32, "game/code_0031C940", itfDrawUniformlyScaledIndexedImage);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CF88);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D120);

/* Allocate list headers and all instance records in one contiguous work block. */
ModelInstanceWork *itfCreateModelInstanceWork(s32 listCount, s32 *instanceCounts) {
    s32 listBytes = listCount * 8;
    s32 allocationSize = listBytes + 16;
    s32 i;
    u32 handle;
    ModelInstanceWork *work;
    ModelInstanceList *list;
    u8 *records;

    for (i = 0; i < listCount; i++) allocationSize += instanceCounts[i] * sizeof(ModelInstance);
    evtPrintDeveloperConsoleMessage("SpriteWork Object Size %d\n", allocationSize);
    handle = sdfAllocGeneralBlock(allocationSize);
    work = sdfMemoryGetBlockAddress(handle);
    memset(work, 0, allocationSize);
    work->handle = handle;
    work->count = listCount;
    work->lists = (ModelInstanceList *)(work + 1);
    list = work->lists;
    records = (u8 *)list + listBytes;
    for (i = 0; i < listCount; i++, list++) {
        list->items = (ModelInstance *)records;
        list->count = instanceCounts[i];
        records += instanceCounts[i] * sizeof(ModelInstance);
    }
    return work;
}

/* Clear all slots; the loop index intentionally wraps to 16 bits. */
void itfClearModelInstances(ModelInstanceList *list) {
    ModelInstance *item;
    u32 index;

    index = 0;
    item = list->items;
    if (0 < list->count) {
        do {
            memset(item, 0, 0x34);
            index = (index + 1) & 0xffff;
            item = item + 1;
        } while ((s32)index < list->count);
    }
}

/* Store an eight-bit parameter in flags bits 3..10, preserving other flags. */
void itfSetModelInstanceParameterBits(ModelInstanceList *list, u32 parameter) {
    ModelInstance *item;
    s32 index;

    index = 0;
    item = list->items;
    if (0 < list->count) {
        do {
            index = index + 1;
            item->flags = (item->flags & 0xfffff807) | ((parameter & 0xff) << 3);
            item = item + 1;
        } while (index < list->count);
    }
}

void itfDeactivateModelInstances(ModelInstanceList *list) {
    ModelInstance *item = list->items;
    s32 index = 0;
    if (list->count > 0) {
        do {
            itfDeactivateModelInstance(item);
            item += 1;
            index++;
        } while (index < list->count);
    }
}

/* Claim a slot with a fresh animation cycle and no timed fade/motion yet. */
s32 itfClaimFreeModelInstance(ModelInstanceList *list) {
    ModelInstance *item;
    s32 index;

    index = 0;
    item = list->items;
    if (0 < list->count) {
        do {
            if ((item->flags & 1) == 0) {
                item->animationLength = 10;
                item->flags = item->flags | 1;
                item->animationFrame = 0;
                item->remainingLifetime = 0;
                item->initialLifetime = 0;
                return (s32)item;
            }
            index = index + 1;
            item = item + 1;
        } while (index < list->count);
    }
    return 0;
}

void itfDeactivateModelInstance(ModelInstance *item) {
    item->flags = item->flags & 0xfffffffe;
}

/* Store both the live countdown and its initial value for timed draw/motion updates. */
void itfSetModelInstanceValuePair(ModelInstance *model, s32 lifetime) {
    lifetime &= 0xFFFF;
    model->remainingLifetime = lifetime;
    model->initialLifetime = lifetime;
}

/* Set the per-update position increment without restarting the timed motion. */
void itfSetModelInstanceSecondaryVector(ModelInstance *model, f32 x, f32 y, f32 z) {
    model->positionStep[0] = x;
    model->positionStep[1] = y;
    model->positionStep[2] = z;
}

/* Set the draw position and reset its associated, otherwise unknown word. */
void itfSetModelInstancePrimaryVector(ModelInstance *model, f32 x, f32 y, f32 z) {
    model->position[0] = x;
    model->position[1] = y;
    model->position[2] = z;
    model->valueC = 0;
}

/* Draw each active instance first; bit 0 skips its subsequent aging and movement.
 * The final position increment still occurs after its countdown deactivates it. */
void func_0031D558(ModelInstanceWork *work, u32 flags) {
    s32 listIndex = 0;
    s32 instanceIndex;
    ModelInstanceList *list = work->lists;
    ModelInstance *item;

    for (; listIndex < work->count; listIndex++, list++) {
        item = list->items;
        for (instanceIndex = 0; instanceIndex < list->count; instanceIndex++, item++) {
            if (item->flags & 1) {
                itfDrawModelInstanceImage(item);
                if ((flags & 1) == 0) {
                    item->animationFrame++;
                }
                if (item->initialLifetime != 0 && (flags & 1) == 0) {
                    if (--item->remainingLifetime == 0) {
                        itfDeactivateModelInstance(item);
                    }
                    item->position[0] += item->positionStep[0];
                    item->position[1] += item->positionStep[1];
                    item->position[2] += item->positionStep[2];
                }
            }
        }
    }
}

void itfDrawModelInstanceImage(ModelInstance *item) {
    f32 alpha = 1.0f;
    s32 kind;

    if (item->initialLifetime != 0) {
        alpha = (f32)item->remainingLifetime / (f32)item->initialLifetime;
    }
    kind = (item->flags >> 3) & 0xFF;
    switch (kind) {
    case 17:
        itfDrawUniformlyScaledIndexedImage((s32)item->position[0], (s32)item->position[1],
            0, (s32)(alpha * 128.0f), 0, 0, 0x53, item->scale);
        return;
    case 18:
        if (item->animationLength <= item->animationFrame) {
            item->animationFrame = 0;
        }
        itfDrawUniformlyScaledIndexedImage((s32)item->position[0], (s32)item->position[1],
            0, (s32)(alpha * 128.0f), 0, item->animationFrame + 31, 0x53, item->scale);
        return;
    case 19:
        item->animationFrame = 0;
        itfDrawUniformlyScaledIndexedImage((s32)item->position[0], (s32)item->position[1],
            0, (s32)(alpha * 128.0f), 0, 29, 0x53, item->scale);
        break;
    case 20:
        if (item->animationLength <= item->animationFrame) {
            item->animationFrame = 0;
        }
        itfDrawUniformlyScaledIndexedImage((s32)item->position[0], (s32)item->position[1],
            0, (s32)(alpha * 128.0f), 0, item->animationFrame + 9, 0x53, item->scale);
        break;
    }
}
