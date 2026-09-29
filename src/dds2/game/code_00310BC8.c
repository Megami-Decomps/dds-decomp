#include "common.h"
#include "fpu.h"

extern s32 func_003292A8(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void func_00313BA8();

extern s32 func_003297C8(u32);

extern void func_00328E48();

extern void sdfClearTaskList();

extern s32 sdfDestroyCallbackWork();

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern s32 func_00101740(u32);

typedef struct ShortPair2C {
    u8 pad_0x00[0x2C]; // 0x00
    s16 h2C;           // 0x2C
    s16 h2E;           // 0x2E
} ShortPair2C; // 0x30

extern u32 func_00312A48(u32 *);

extern u32 func_00312188(u32, u32, u32);

extern s32 func_003123D0(void *, void *);

extern s32 kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern s32 func_003139D8();

extern f32 func_003532B8(f32);

extern f32 func_003406A0(f32);

extern f32 func_003407A0(f32);

extern void sdfQuatMultiply(f32 *, f32 *, f32 *);

extern f32 fldNormalizedVectorDot(f32 *, f32 *);

extern void func_00313A58(u8 *);

typedef struct SdfLink {
    u8 pad00[8];
    struct SdfLink *prev;
    struct SdfLink *next;
} SdfLink;

typedef struct SdfTaskOwner {
    u32 handle;        /* 0x00 */
    u8 pad04[0xC];
    u32 removeArg;     /* 0x10 */
    u8 pad14[4];
    void (*onRemove)(s32, u32); /* 0x18 */
} SdfTaskOwner;

typedef struct TaskListNode {
    u32 handle; /* 0x00 */
    s32 key;    /* 0x04 */
    struct TaskListNode *next; /* 0x08 */
    u8 pad0C[4];
    u32 value;  /* 0x10 */
} TaskListNode;

typedef struct {
    u32 pad00;
    u32 tail;  /* 0x04 */
    TaskListNode *head; /* 0x08 */
    u32 count; /* 0x0C */
    u32 pad10;
    void (*onRemove)(u32, u32); /* 0x14 */
} TaskList;

typedef struct TaskWork {
    u32 handle;
    char *primaryTaskName;
    char *secondaryTaskName;
    TaskList *list;
    u32 firstItemHandle;
} TaskWork;

typedef struct SdfGridOwner {
    u32 handle;        /* 0x00 */
    u8 pad04[0x1C];
    void (*onDestroy)(s32, u32); /* 0x20 */
    u8 pad24[0xC];
    u32 destroyArg;    /* 0x30 */
} SdfGridOwner;

float mdlQuaternionLengthSquared(float *quaternion) {
    return *quaternion * *quaternion + quaternion[1] * quaternion[1] +
                  quaternion[2] * quaternion[2] + quaternion[3] * quaternion[3];
}

float sdfQuaternionMagnitude(float *quaternion) {
    return fsqrtf(mdlQuaternionLengthSquared(quaternion));
}

void sdfQuaternionInverse(float *values) {
    float lengthSquared = mdlQuaternionLengthSquared(values);
    if (lengthSquared != 0.0f) {
        values[0] = -values[0] / lengthSquared;
        values[1] = -values[1] / lengthSquared;
        values[2] = -values[2] / lengthSquared;
        values[3] = values[3] / lengthSquared;
    }
}

void sdfQuaternionNormalize(float *values) {
    float length = sdfQuaternionMagnitude(values);
    values[0] /= length;
    values[1] /= length;
    values[2] /= length;
    values[3] /= length;
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00310D28);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00310DE8);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00310EB0);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00310FD0);

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfQuaternionBlendNormalize);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311178);

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfQuatBlendAngular);

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfQuatSquad);

void sdfQuatLog(f32 *out, f32 *in) {
    f32 angle = func_003532B8(in[3]);
    f32 sine = func_003406A0(angle);
    out[3] = 0.0f;
    if (out[3] < sine) {
        out[0] = angle * in[0] / sine;
        out[1] = angle * in[1] / sine;
        out[2] = angle * in[2] / sine;
    } else {
        out[0] = out[1] = out[2] = out[3];
    }
}

void sdfQuatExp(f32 *out, f32 *in) {
    f32 x = in[0], y = in[1], z = in[2];
    f32 length = fsqrtf(x * x + y * y + z * z);
    f32 sine = func_003406A0(length);
    out[3] = func_003407A0(length);
    if (length > 0.0f) {
        out[0] = sine * in[0] / length;
        out[1] = sine * in[1] / length;
        out[2] = sine * in[2] / length;
    } else {
        out[0] = out[1] = out[2] = 0.0f;
    }
}

void sdfQuatSquadControl(f32 *out, f32 *from, f32 *rotation, f32 *to) {
    f32 first[4], second[4], middle[4];
    out[0] = -rotation[0];
    out[1] = -rotation[1];
    out[2] = -rotation[2];
    out[3] = rotation[3];
    sdfQuatMultiply(first, out, from);
    sdfQuatLog(first, first);
    sdfQuatMultiply(second, out, to);
    sdfQuatLog(second, second);
    middle[0] = (first[0] + second[0]) * -0.25f;
    middle[1] = (first[1] + second[1]) * -0.25f;
    middle[2] = (first[2] + second[2]) * -0.25f;
    middle[3] = (first[3] + second[3]) * -0.25f;
    sdfQuatExp(middle, middle);
    sdfQuatMultiply(out, rotation, middle);
}

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfQuatForwardVector);

f32 sdfQuatForwardDot(f32 *direction, f32 *target) {
    f32 quaternion[4];
    memset(quaternion, 0, sizeof(quaternion));
    quaternion[2] = 1.0f;
    sdfQuatForwardVector(direction, quaternion);
    return fldNormalizedVectorDot(quaternion, target);
}

f32 func_003118E8(f32 *direction, f32 *target) {
    f32 quaternion[4];
    f32 projection;
    f32 component;
    memset(quaternion, 0, sizeof(quaternion));
    quaternion[1] = 1.0f;
    projection = sdfQuatForwardDot(target, quaternion);
    component = direction[1];
    if (component < 0.0f) {
        return -component / projection;
    }
    return component / projection;
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311978);

s64 sdfFontRegisterShort(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F460(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D530(temp_v0, 1);
    return func_0019C5B0(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311AE8);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311B58);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311C50);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311D00);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311DB0);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311E60);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00311F20);

void *sdfCreateCallbackNode(u32 arg0) {
    s32 allocation = func_003292A8(0x1C);
    u32 *obj = sdfMemoryGetBlockAddress(allocation);

    memset(obj, 0, 0x1C);
    obj[0] = allocation;
    obj[4] = arg0;
    obj[5] = (u32)func_00313BA8;
    obj[6] = (u32)func_00313BA8;
    return obj;
}

s64 sdfDestroyTaskWork(SdfTaskOwner *owner) {
    if (owner != NULL) {
        sdfClearTaskList(owner);
        owner->onRemove(-1, owner->removeArg);
        return func_003297C8(owner->handle);
    }
}

void sdfSetTaskDestroyCallback(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x18) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312188);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312228);

void sdfSetTaskSecondaryCallback(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312320);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_003123D0);

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfClearTaskList);

void func_003124B0(SdfLink *list, SdfLink *a, SdfLink *b) {
    SdfLink *tmpNext;
    SdfLink *tmpPrev;

    if (a != NULL && b != NULL) {
        if (a->prev != NULL) {
            a->prev->next = b;
        }
        if (a->next != NULL) {
            a->next->prev = b;
        }
        if (b->prev != NULL) {
            b->prev->next = a;
        }
        if (b->next != NULL) {
            b->next->prev = a;
        }
        tmpNext = a->next;
        a->next = b->next;
        tmpPrev = a->prev;
        a->prev = b->prev;
        b->next = tmpNext;
        b->prev = tmpPrev;
        if (a->next == NULL) {
            list->prev = a;
        }
        if (a->prev == NULL) {
            list->next = a;
        }
        if (b->next == NULL) {
            list->prev = b;
        }
        if (b->prev == NULL) {
            list->next = b;
        }
    }
}

void *sdfFindTaskListNodeByKey(TaskList *list, s32 key) {
    TaskListNode *node;

    node = list->head;
    while (node->key != key) {
        node = node->next;
        if (node == NULL) {
            break;
        }
    }
    return node;
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312578);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312620);

s64 sdfDestroyTaskPair(TaskWork *work) {
    if (work != NULL) {
        kwlnTaskDestroyWithHierarchyByName(work->primaryTaskName, 1);
        return kwlnTaskDestroyWithHierarchyByName(work->secondaryTaskName, 0);
    }
}

u8 sdfIsPrimaryTaskRegistered(TaskWork *work) {
    u8 exists;
    s64 task;

    exists = 0;
    if (work != NULL) {
        task = func_00101740((u32)work->primaryTaskName);
        exists = task != 0;
    }
    return exists;
}

s32 kwlnTaskExists(u32 name) {
    return func_00101740(name) != 0;
}

void sdfAttachTaskItem(TaskWork *work, u32 *item) {
    u32 result = func_00312188((u32)work->list, *item, func_00312A48(item));
    if (work->firstItemHandle == 0) {
        work->firstItemHandle = result;
    }
}

s64 sdfRemoveTaskItem(TaskWork *work, s32 key) {
    void *node = sdfFindTaskListNodeByKey(work->list, key);
    if (node != NULL) {
        return func_003123D0(work->list, node);
    }
}

s32 sdfFindTaskItemValueByKey(TaskWork *work, s32 key) {
    TaskListNode *item;

    item = sdfFindTaskListNodeByKey(work->list, key);
    if (item != NULL) {
        return item->value;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312810);

void sdfSetTaskItemMode(void *list, s32 key, u32 mode) {
    u32 *item = sdfFindTaskItemValueByKey(list, key);
    if (item == NULL) {
        return;
    }
    switch (mode) {
    case 3:
        *item = (*(u16 *)item & ~1) | 0x10002;
        break;
    case 4:
        *item = (*(u16 *)item & ~2) | 0x10001;
        break;
    case 2:
        *item = *(u16 *)item | 0x20000;
        break;
    case 1:
        *item = *(u16 *)item | 0x100000;
        break;
    case 0:
        *item = *(u16 *)item | 0x10003;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312910);

s64 sdfFreeTaskWork(TaskWork *work) {
    if (work != NULL) {
        sdfDestroyTaskWork(work->list);
        func_00328E48(work->primaryTaskName);
        func_00328E48(work->secondaryTaskName);
        return func_003297C8(work->handle);
    }
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312A48);

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfDestroyCallbackWork);

s64 func_00312B50(a, b)
s32 a;
s32 b;
{
    return sdfDestroyCallbackWork(b);
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312B70);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312C78);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312D48);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312DA0);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312DF8);

void func_00312E20(void) {
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312E28);

void func_00312F58(ShortPair2C *p, s32 a, s32 b) {
    p->h2C = a;
    p->h2E = b;
}

s64 sdfDestroyGridWork(SdfGridOwner *owner) {
    if (owner != NULL) {
        func_003139D8();
        owner->onDestroy(0, owner->destroyArg);
        return func_003297C8(owner->handle);
    }
}

s64 func_00312FB0(void) {
    return func_003139D8();
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312FD0);

void sdfGridGetCursorCoordinates(void *p, u32 *mod, u32 *div) {
    u32 *t = *(u32 **)((s32)p + 8);
    *mod = *t % *(u32 *)((s32)p + 0x14);
    *div = *t / *(u32 *)((s32)p + 0x14);
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00313040);

void sdfGridSetCellValue(s32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    u32 temp_v0;

    temp_v0 = arg2 * *(s32 *)(arg0 + 0x14) + arg1;
    if (temp_v0 < *(u32 *)(arg0 + 0x10)) {
        *(u32 *)(temp_v0 * 8 + *(s32 *)(arg0 + 4) + 4) = arg3;
    }
}

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfGridCursorUp);

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfGridCursorDown);

INCLUDE_ASM(const s32, "game/code_00310BC8", sdfGridCursorLeft);

u8 *sdfGridCursorRight(u8 *grid) {
    u8 *cell = *(u8 **)(grid + 8);
    u32 width = *(u32 *)(grid + 0x14);
    u32 index = *(u32 *)cell;
    cell += 8;
    if (index % width == width - 1) {
        return NULL;
    }
    *(u8 **)(grid + 8) = cell;
    func_00313A58(grid);
    return cell;
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00313210);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00313280);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_003133C8);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00313538);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_003136A0);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00313810);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00313898);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_003139D8);

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00313A58);

float func_00313B90(float arg0, float arg1, float arg2) {
    return arg0 + arg1 * arg2;
}

u32 func_00313BA0(void) {
    return 0;
}

void func_00313BA8(void) {
}

u32 func_00313BB0(void) {
    return 0;
}
