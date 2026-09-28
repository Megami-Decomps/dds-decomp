#include "common.h"
#include "fpu.h"

extern u32 func_00197760(s32, s32, s32, u32, u32, s32);
extern void func_00195880(u32, s32);
extern void func_00194920(u32);
extern s32 frFontMeasureGlyphChain(u32);
extern void func_001958A0(u32, s32, s32);
extern u32 func_001951C8(u32, u32, s32, u32, u32);
extern void func_001954C8(u32, u32);
extern void func_00195450(u32, s32, s32);
extern void func_00195460(u32, u32);
extern void func_00195470(u32, u8);
extern u32 func_00197C40(s32, s32, u32, u16, u32, u32);
extern f32 fldNormalizedVectorDot(f32 *, f32 *);
extern f32 func_002FA1C0(f32);
extern f32 func_002E77F8(f32);
extern f32 func_002E78F8(f32);
extern void sdfQuatMultiply(f32 *, f32 *, f32 *);
extern void func_002C94A8(f32 *, f32 *);
extern void func_002C8F40(f32 *, f32 *);
extern void func_002C84F0(f32 *);
extern void func_002CC5F0(u8 *);
extern f32 sdfQuatDot(f32 *, f32 *);
extern f32 func_002FA060(f32);

extern s32 kwlnTaskGetTaskByName(u32);

typedef struct ShortPair2C {
    u8 pad_0x00[0x2C]; // 0x00
    s16 h2C;           // 0x2C
    s16 h2E;           // 0x2E
} ShortPair2C; // 0x30

extern void func_002CAF78(void *, void *);
extern u32 func_002CB5F0(u32 *);
extern u32 func_002CAD30(u32, u32, u32);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

float sdfQuatLengthSquared(float *arg0) {
    return *arg0 * *arg0 + arg0[1] * arg0[1] + arg0[2] * arg0[2] +
                  arg0[3] * arg0[3];
}

float quaternionMagnitude(float *values) {
    return fsqrtf(sdfQuatLengthSquared(values));
}

void quaternionInverse(float *values) {
    float lengthSquared = sdfQuatLengthSquared(values);
    if (lengthSquared != 0.0f) {
        values[0] = -values[0] / lengthSquared;
        values[1] = -values[1] / lengthSquared;
        values[2] = -values[2] / lengthSquared;
        values[3] = values[3] / lengthSquared;
    }
}

void quaternionNormalize(float *values) {
    float length = quaternionMagnitude(values);
    values[0] /= length;
    values[1] /= length;
    values[2] /= length;
    values[3] /= length;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9948);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9A08);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9AD0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9BF0);

void quaternionBlendNormalize(float *out, float *from, float *to, float fraction) {
    float inv = 1.0f - fraction;
    out[0] = from[0] * inv + to[0] * fraction;
    out[1] = from[1] * inv + to[1] * fraction;
    out[2] = from[2] * inv + to[2] * fraction;
    out[3] = from[3] * inv + to[3] * fraction;
    quaternionNormalize(out);
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9D98);

void sdfQuatBlendAngular(f32 *out, f32 *from, f32 *to, f32 fraction) {
    f32 angle = sdfQuatDot(from, to);
    if (-0.95f < angle && angle < 0.95f) {
        f32 firstWeight = func_002FA060(angle * (1.0f - fraction));
        f32 secondWeight = func_002FA060(angle * fraction);
        f32 denominator = func_002FA060(angle);
        out[0] = (from[0] * firstWeight + to[0] * secondWeight) / denominator;
        out[1] = (from[1] * firstWeight + to[1] * secondWeight) / denominator;
        out[2] = (from[2] * firstWeight + to[2] * secondWeight) / denominator;
        out[3] = (from[3] * firstWeight + to[3] * secondWeight) / denominator;
    } else {
        quaternionBlendNormalize(out, from, to, fraction);
    }
}

void sdfQuatSquad(f32 *out, f32 *first, f32 *second, f32 *third, f32 *fourth, f32 fraction) {
    f32 firstBlend[4];
    f32 secondBlend[4];
    sdfQuatBlendAngular(firstBlend, first, fourth, fraction);
    sdfQuatBlendAngular(secondBlend, second, third, fraction);
    sdfQuatBlendAngular(out, firstBlend, secondBlend, (fraction + fraction) * (1.0f - fraction));
}

void sdfQuatLog(f32 *out, f32 *in) {
    f32 angle = func_002FA1C0(in[3]);
    f32 sine = func_002E77F8(angle);
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
    f32 sine = func_002E77F8(length);
    out[3] = func_002E78F8(length);
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

void sdfQuatForwardVector(f32 *rotation, f32 *out) {
    f32 matrix[16];
    f32 vector[4];
    memset(vector, 0, sizeof(vector));
    vector[2] = 1.0f;
    func_002C94A8(matrix, rotation);
    func_002C8F40(vector, matrix);
    func_002C84F0(vector);
    memcpy(out, vector, sizeof(vector));
}

f32 func_002CA4A8(f32 *direction, f32 *target) {
    f32 quaternion[4];
    memset(quaternion, 0, sizeof(quaternion));
    quaternion[2] = 1.0f;
    sdfQuatForwardVector(direction, quaternion);
    return fldNormalizedVectorDot(quaternion, target);
}

f32 func_002CA508(f32 *direction, f32 *target) {
    f32 quaternion[4];
    f32 projection;
    f32 component;
    memset(quaternion, 0, sizeof(quaternion));
    quaternion[1] = 1.0f;
    projection = func_002CA4A8(target, quaternion);
    component = direction[1];
    if (component < 0.0f) {
        return -component / projection;
    }
    return component / projection;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA598);

void func_002CA6B0(s32 x, s32 y, u32 first, u32 second) {
    u32 handle = func_00197760(x << 4, y << 3, 0, first, second, 0);
    func_00195880(handle, 1);
    func_00194920(handle);
}

s32 func_002CA708(s32 x, s32 y, u32 first, u32 second, u32 third, s32 option) {
    u32 handle = func_00197760(x << 4, y << 3, first, second, third, 0);
    s32 result = frFontMeasureGlyphChain(handle);
    func_001958A0(handle, 1, option);
    func_00194920(handle);
    return result;
}

s32 func_002CA778(s32 x, s32 y, u32 first, u32 second, s8 type, u32 name, s32 flag, s32 option) {
    u32 handle = func_001951C8(name, 0, type, 0, 0);
    s32 result = 0;
    func_001954C8(handle, second);
    func_00195450(handle, x << 4, y << 3);
    func_00195460(handle, first);
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, option);
    func_00194920(handle);
    return result;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA858);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA8F0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA988);

s32 func_002CAA20(s32 x, s32 y, u32 first, u32 second, u8 opacity, u16 width, u32 name, u32 extra, s32 flag, s32 option) {
    u32 handle = func_00197C40(x << 4, y << 3, first, width, name, extra);
    s32 result = 0;
    func_00195470(handle, opacity);
    func_001954C8(handle, second);
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, option);
    func_00194920(handle);
    return result;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAAC8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAC60);

void sdfDestroyTaskWork(u8 *work) {
    if (work != NULL) {
        sdfClearTaskList();
        (*(void (**)(s32, u32))(work + 0x18))(-1, *(u32 *)(work + 0x10));
        func_002D0918(*(u32 *)work);
    }
}

void func_002CAD20(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x18) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAD30);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CADD0);

void func_002CAEB8(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAEC8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAF78);

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

void sdfClearTaskList(TaskList *list) {
    TaskListNode *node;
    if (list != NULL) {
        node = list->head;
        if (node != NULL) {
            do {
                TaskListNode *current = node;
                node = node->next;
                list->onRemove(current->handle, current->value);
                func_002CFF98(current);
            } while (node != NULL);
        }
        list->count = 0;
        list->head = 0;
        list->tail = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB058);

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

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB120);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB1C8);

typedef struct TaskWork {
    u32 handle;
    char *primaryTaskName;
    char *secondaryTaskName;
    TaskList *list;
    u32 firstItemHandle;
} TaskWork;

void func_002CB278(TaskWork *work) {
    if (work != NULL) {
        kwlnTaskDestroyWithHierarchyByName(work->primaryTaskName, 1);
        kwlnTaskDestroyWithHierarchyByName(work->secondaryTaskName, 0);
    }
}

u8 func_002CB2B8(TaskWork *work) {
    u8 exists;
    s64 task;

    exists = 0;
    if (work != NULL) {
        task = kwlnTaskGetTaskByName((u32)work->primaryTaskName);
        exists = task != 0;
    }
    return exists;
}

s32 kwlnTaskExists(u32 name) {
    return kwlnTaskGetTaskByName(name) != 0;
}

void sdfAttachTaskItem(TaskWork *work, u32 *item) {
    u32 result = func_002CAD30((u32)work->list, *item, func_002CB5F0(item));
    if (work->firstItemHandle == 0) {
        work->firstItemHandle = result;
    }
}

void sdfRemoveTaskItem(TaskWork *work, s32 key) {
    void *item = sdfFindTaskListNodeByKey(work->list, key);
    if (item != NULL) {
        func_002CAF78(work->list, item);
    }
}

s32 func_002CB390(TaskWork *work, s32 key) {
    TaskListNode *item;

    item = sdfFindTaskListNodeByKey(work->list, key);
    if (item != NULL) {
        return item->value;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB3B8);

INCLUDE_RODATA(const s32, "game/code_002C97E8", D_003B3EE0);

void func_002CB3F8(void *list, s32 key, u32 mode) {
    u32 *item = func_002CB390(list, key);
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

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB4B8);

void func_002CB5A8(u8 *work) {
    if (work != NULL) {
        sdfDestroyTaskWork(*(u8 **)(work + 0x0C));
        func_002CFF98(*(void **)(work + 4));
        func_002CFF98(*(void **)(work + 8));
        func_002D0918(*(u32 *)work);
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB5F0);

void func_002CB6B8(u8 *work) {
    if (work != NULL) {
        void (*destroy)(u32, u32) = *(void (**)(u32, u32))(work + 0x0C);
        destroy(*(u32 *)(work + 4), *(u32 *)(work + 0x18));
        func_002CFF98(work);
    }
}

void func_002CB6F8(u32 unused, u8 *work) {
    func_002CB6B8(work);
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB718);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB850);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB8E0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB938);

void func_002CB990(void) {
    func_002CB5A8(func_00101A70());
}

void func_002CB9B8(void) {
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB9C0);

void func_002CBAF0(ShortPair2C *p, s32 a, s32 b) {
    p->h2C = a;
    p->h2E = b;
}

void sdfDestroyGridWork(u8 *work) {
    if (work != NULL) {
        func_002CC570();
        (*(void (**)(s32, u32))(work + 0x20))(0, *(u32 *)(work + 0x30));
        func_002D0918(*(u32 *)work);
    }
}

void func_002CBB48(void) {
    func_002CC570();
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBB68);

void func_002CBBA0(void *p, u32 *mod, u32 *div) {
    u32 *t = *(u32 **)((s32)p + 8);
    *mod = *t % *(u32 *)((s32)p + 0x14);
    *div = *t / *(u32 *)((s32)p + 0x14);
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBBD8);

void func_002CBC10(s32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    u32 temp_v0;

    temp_v0 = arg2 * *(s32 *)(arg0 + 0x14) + arg1;
    if (temp_v0 < *(u32 *)(arg0 + 0x10)) {
        *(u32 *)(temp_v0 * 8 + *(s32 *)(arg0 + 4) + 4) = arg3;
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBC48);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBC98);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBCF0);

u8 *func_002CBD48(u8 *grid) {
    u8 *cell = *(u8 **)(grid + 8);
    u32 width = *(u32 *)(grid + 0x14);
    u32 index = *(u32 *)cell;
    cell += 8;
    if (index % width == width - 1) {
        return NULL;
    }
    *(u8 **)(grid + 8) = cell;
    func_002CC5F0(grid);
    return cell;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBDA8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBE18);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBF60);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC0D0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC238);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC3A8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC430);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC570);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC5F0);

float func_002CC728(float arg0, float arg1, float arg2) {
    return arg0 + arg1 * arg2;
}

u32 func_002CC738(void) {
    return 0;
}

void func_002CC740(void) {
}

u32 func_002CC748(void) {
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD288);

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD290);

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD298);

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD2A0);

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD2A8);

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD2B0);

