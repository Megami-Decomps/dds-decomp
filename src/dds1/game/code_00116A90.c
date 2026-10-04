#include "common.h"
#include "eff.h"

typedef struct {
    u8 pad0[0x10];
    u32 value;
} PathState;

typedef struct {
    u8 pad0[0x18];
    PathState *state;
} PathObject;

typedef struct Dds3PathKeyframes {
    u32 count;
    f32 *data;
    u32 *frames;
} Dds3PathKeyframes;

extern u64 dds3GetWorldSecondaryObject(void);
extern s32 dds3FindWorldObjectNodeByKey(u64, u64, u64);

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    s32 unk8;     /* 0x8 */
    u8 unkC[0xC]; /* 0xC */
    void *unk18;  /* 0x18 */
} ActionObj;

typedef struct ActionSub {
    u8 unk0[0x10];
    s32 unk10;
} ActionSub;

extern ActionObj *dds3AppendWorldObjectNode();

ActionObj *evtSpawnActionObjB(s32 a, s32 b, s32 c, s32 d) {
    ActionObj *obj = dds3AppendWorldObjectNode(0xB);

    obj->unk8 = d;
    ((ActionSub *)obj->unk18)->unk10 = 0;
    obj->unk4 = a;
    return obj;
}

void dds3SetPathStateValue(PathObject *path, u32 value) {
    path->state->value = value;
}

u32 dds3GetPathStateValueById(u64 id) {
    PathObject *path;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    path = (PathObject *)dds3FindWorldObjectNodeByKey(world, id, 6);
    return path->state->value;
}

ActionObj *evtSpawnActionObjD(s32 a, void *work, s32 c) {
    ActionObj *obj = dds3AppendWorldObjectNode(0xD);

    obj->unk18 = work;
    obj->unk4 = a;
    obj->unk8 = c;
    return obj;
}

u32 dds3GetPathState(PathObject *path) {
    return (u32)path->state;
}

void func_00116B80(u32 *segment, f32 *weight, Dds3PathKeyframes *keys, f32 frame) {
    u32 count = keys->count;
    u32 i;
    u32 *frames = keys->frames;

    for (i = 0; i < count - 1; i++) {
        if ((f32)frames[i] <= frame && frame < (f32)frames[i + 1]) {
            *weight = 1.0f / (f32)(frames[i + 1] - frames[i]);
            *segment = i;
            *weight *= frame - (f32)frames[i];
            return;
        }
    }
    *segment = count - 2;
    *weight = 1.0f;
}

ActionObj *evtSpawnActionObj10(s32 a, void *work, s32 c) {
    ActionObj *obj = dds3AppendWorldObjectNode(0x10);

    obj->unk4 = a;
    obj->unk8 = c;
    obj->unk18 = work;
    return obj;
}

typedef struct Dds3PathCurveEntry {
    u32 kind;
    Dds3PathKeyframes *keys;
} Dds3PathCurveEntry;

typedef struct Dds3PathCurveTable {
    u32 count;
    Dds3PathCurveEntry entries[1];
} Dds3PathCurveTable;

typedef struct Dds3PathCurveWork {
    s32 state;
    u32 flags;
    f32 duration;
    f32 time;
    EffPrim *unk10;
    Dds3PathKeyframes *unk14;
    Dds3PathKeyframes *unk18;
    Dds3PathKeyframes *unk1C;
    Dds3PathKeyframes *unk20;
} Dds3PathCurveWork;

extern void *sdfAllocSizeClassBlock(s32 bytes);
extern void *memset(void *destination, s32 value, u32 bytes);
extern EffPrim *effCreatePrimitiveCurve(f32 *data, u32 count, s32 mode);

Dds3PathCurveWork *dds3CreatePathCurveWork(ActionObj *object) {
    Dds3PathCurveTable *table = object->unk18;
    Dds3PathCurveEntry *entry;
    Dds3PathKeyframes *keys;
    Dds3PathCurveWork *work;
    u32 i;

    if (table->count == 0) {
        return NULL;
    }
    entry = table->entries;
    work = sdfAllocSizeClassBlock(sizeof(*work));
    i = 0;
    memset(work, 0, sizeof(*work));
    work->state = 0;
    work->time = 0.0f;
    work->duration = 0.0f;
    for (; i < table->count; i++, entry++) {
        u32 lastFrame;

        keys = entry->keys;
        switch (entry->kind) {
        case 4:
            work->unk1C = keys;
            work->flags |= 4;
            break;
        case 0:
            work->flags |= 1;
            work->unk14 = keys;
            work->unk10 = effCreatePrimitiveCurve(keys->data, keys->count, 1);
            break;
        case 2:
            work->unk18 = keys;
            work->flags |= 2;
            break;
        case 5:
            work->unk20 = keys;
            work->flags |= 0x10;
            break;
        }
        lastFrame = keys->frames[keys->count - 1];
        if (work->duration < (f32)lastFrame) {
            work->duration = (f32)lastFrame;
        }
    }
    return work;
}
