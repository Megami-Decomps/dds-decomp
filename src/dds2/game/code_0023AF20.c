#include "common.h"
#include "pcp_vu0.h"

extern void *dds3GetSlot(void *obj, s32 index);
extern void *dds3GetObjectResourceHandle(void *slot);
extern s32 func_001177D8(void *handle);
extern f32 evtGetValueScaleFactor(void *handle);

s32 func_0023AF20(void *obj) {
    f32 target = 1.0f;
    void *slot;
    void *handle;
    s32 kind;

    if (obj == NULL) {
        return -1;
    }
    slot = dds3GetSlot(obj, 1);
    if (slot == NULL) {
        return -1;
    }
    handle = dds3GetObjectResourceHandle(slot);
    if (handle == NULL) {
        return -1;
    }
    kind = func_001177D8(handle);
    if (kind != 0) {
        if (kind != 1) {
            return -1;
        }
        target = 0.0f;
    }
    if (evtGetValueScaleFactor(handle) == target) {
        return 0;
    }
    return 1;
}

extern void *dds3GetSlot1Data(void *obj);
extern void func_00117838(void *data);
extern void func_00117848(void *data);

void func_0023AFC8(void *obj, s32 flag) {
    void *data = dds3GetSlot1Data(obj);

    if (data == NULL) {
        return;
    }
    if (flag != 0) {
        func_00117838(data);
    } else {
        func_00117848(data);
    }
}

extern s32 dds3AdvanceWorldCounter();
extern s32 func_00112E30(s32 world, f32 *pos, f32 *rot);
extern void effObjSetInnerFloat(s32 obj, f32 value);

s32 func_0023B018(f32 *pos, f32 *rot) {
    s32 obj = func_00112E30(dds3AdvanceWorldCounter(), pos, rot);

    if (obj == 0) {
        return obj;
    }
    effObjSetInnerFloat(obj, 1.0f);
    return obj;
}

typedef struct EvtNodeInner {
    u8 pad00[0x08];
    s32 kind;           /* 0x08 */
    void *node;         /* 0x0C */
} EvtNodeInner;

typedef struct EvtNodeOwner {
    u8 pad00[0x18];
    EvtNodeInner *inner; /* 0x18 */
} EvtNodeOwner;

extern void func_00157838();

void func_0023B078(EvtNodeOwner *owner, s32 flag) {
    EvtNodeInner *inner = owner->inner;
    void *node;

    switch (inner->kind) {
    case 1:
    case 7:
    case 8:
        break;
    default:
        return;
    }
    node = inner->node;
    if (flag == 0) {
        func_00157838(node, flag);
    }
}

typedef struct EvtWorldNode {
    u8 pad00[0x20];
    struct EvtWorldNode *next; /* 0x20 */
} EvtWorldNode;

typedef struct EvtWorldList {
    u8 pad00[0x4C];
    EvtWorldNode *head; /* 0x4C */
} EvtWorldList;

typedef struct EvtWorldRoot {
    u8 pad00[0x08];
    EvtWorldList *list; /* 0x08 */
} EvtWorldRoot;

typedef struct EvtWorldObj {
    u8 pad00[0x18];
    EvtWorldRoot *root; /* 0x18 */
} EvtWorldObj;

extern EvtWorldObj *dds3GetWorldObject(void);
extern void func_00112230(void *node, s32 value);

s32 func_0023B0D0(s32 index, s32 base) {
    EvtWorldObj *world = dds3GetWorldObject();
    EvtWorldNode *node;
    s32 value;

    if (world == NULL) {
        return 0;
    }
    value = index * 3 + base;
    for (node = world->root->list->head; node != NULL; node = node->next) {
        func_00112230(node, value);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023AF20", func_0023B148);

INCLUDE_ASM(const s32, "game/code_0023AF20", func_0023B170);

INCLUDE_ASM(const s32, "game/code_0023AF20", func_0023B1E8);

typedef struct EvtMoveUnit {
    u8 pad00[0x70];
    s128 vector;        /* 0x70 */
    u8 pad80[0x20];     /* 0x80 */
    void *scaled;       /* 0xA0 */
    f32 delta;          /* 0xA4 */
    u32 flags;          /* 0xA8 */
    s16 mode;           /* 0xAC */
    s16 state;          /* 0xAE */
} EvtMoveUnit;

extern void evtScaleValueByMultiplier(void *value, f32 multiplier);
extern void func_001171A0(void *value);

s32 func_0023B2B8(EvtMoveUnit *unit) {
    f32 t;

    if (unit->mode != 1) {
        return 0;
    }
    switch (unit->state) {
    case 0:
    case 1:
        break;
    case 2:
        t = evtGetValueScaleFactor(unit->scaled);
        if (unit->flags & 4) {
            if (t == 0.0f) {
                return 0;
            }
        } else {
            if (t == 1.0f) {
                return 0;
            }
        }
        t += unit->delta;
        if (t < 0.0f) {
            t = 0.0f;
        }
        if (1.0f < t) {
            t = 1.0f;
        }
        evtScaleValueByMultiplier(unit->scaled, t);
        func_001171A0(unit->scaled);
        VU0_STORE_VF($vf10, &unit->vector);
        return 1;
    }
    return 0;
}
