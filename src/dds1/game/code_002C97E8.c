#include "common.h"

#include "fpu.h"

extern u32 func_00197760(s32, s32, s32, u32, u32, s32);

extern void func_00195880(u32, s32);

extern void func_00194920(u32);

extern s32 frFontMeasureGlyphChain(u32);

extern void func_001958A0(u32, s32, s32);

extern u32 func_001951C8(u32, u32, s32, u32, u32);

extern void frFontSetChildColors(u32, u32);

extern void func_00195450(u32, s32, s32);

extern void func_00195460(u32, u32);

extern void frFontSetChainFlag(u32, u8);

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

extern s32 func_002D03F8(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void func_002CC740();

float sdfQuatLengthSquared(float *arg0) {
    return *arg0 * *arg0 + arg0[1] * arg0[1] + arg0[2] * arg0[2] +
                  arg0[3] * arg0[3];
}

float sdfQuatMagnitude(float *values) {
    return fsqrtf(sdfQuatLengthSquared(values));
}

void sdfQuatInverse(float *values) {
    float lengthSquared = sdfQuatLengthSquared(values);
    if (lengthSquared != 0.0f) {
        values[0] = -values[0] / lengthSquared;
        values[1] = -values[1] / lengthSquared;
        values[2] = -values[2] / lengthSquared;
        values[3] = values[3] / lengthSquared;
    }
}

void sdfQuatNormalize(float *values) {
    float length = sdfQuatMagnitude(values);
    values[0] /= length;
    values[1] /= length;
    values[2] /= length;
    values[3] /= length;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9948);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9A08);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9AD0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9BF0);

void sdfQuatBlendNormalize(float *out, float *from, float *to, float fraction) {
    float inv = 1.0f - fraction;
    out[0] = from[0] * inv + to[0] * fraction;
    out[1] = from[1] * inv + to[1] * fraction;
    out[2] = from[2] * inv + to[2] * fraction;
    out[3] = from[3] * inv + to[3] * fraction;
    sdfQuatNormalize(out);
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
        sdfQuatBlendNormalize(out, from, to, fraction);
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

f32 sdfQuatForwardDot(f32 *direction, f32 *target) {
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
    projection = sdfQuatForwardDot(target, quaternion);
    component = direction[1];
    if (component < 0.0f) {
        return -component / projection;
    }
    return component / projection;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA598);

void sdfFontRegisterShort(s32 x, s32 y, u32 first, u32 second) {
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
    frFontSetChildColors(handle, second);
    func_00195450(handle, x << 4, y << 3);
    func_00195460(handle, first);
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, option);
    func_00194920(handle);
    return result;
}

s32 func_002CA858(s32 x, s32 y, s32 z, u32 w, u8 flags, u32 style, s32 width, u32 color) {
    u32 handle = func_00197760(x << 4, y << 3, z, w, style, 0);
    s32 result;

    frFontSetChainFlag(handle, flags);
    if (width >= 0) {
        result = 0;
    } else {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, color);
    func_00194920(handle);
    return result;
}

extern u32 func_00197A98(s32, s32, s32, u32, u32, s32);

s32 func_002CA8F0(s32 x, s32 y, s32 z, u32 w, u8 flags, u32 style, s32 width, u32 color) {
    u32 handle = func_00197A98(x << 4, y << 3, z, w, style, 0);
    s32 result;

    frFontSetChainFlag(handle, flags);
    if (width >= 0) {
        result = 0;
    } else {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, color);
    func_00194920(handle);
    return result;
}

extern u32 func_001978E8(s32, s32, s32, u32, u32, s32);

s32 func_002CA988(s32 x, s32 y, s32 z, u32 w, u8 flags, u32 style, s32 width, u32 color) {
    u32 handle = func_001978E8(x << 4, y << 3, z, w, style, 0);
    s32 result;

    frFontSetChainFlag(handle, flags);
    if (width >= 0) {
        result = 0;
    } else {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, color);
    func_00194920(handle);
    return result;
}

s32 func_002CAA20(s32 x, s32 y, u32 first, u32 second, u8 opacity, u16 width, u32 name, u32 extra, s32 flag, s32 option) {
    u32 handle = func_00197C40(x << 4, y << 3, first, width, name, extra);
    s32 result = 0;
    frFontSetChainFlag(handle, opacity);
    frFontSetChildColors(handle, second);
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, option);
    func_00194920(handle);
    return result;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAAC8);

typedef struct SdfTaskHeader {
    u32 allocation;                     /* 0x00 */
    u8 pad04[0x0C];
    u32 userData;                       /* 0x10 */
    u32 callback14;                     /* 0x14: installed separately */
    void (*onDestroy)(s32, u32);       /* 0x18 */
} SdfTaskHeader;

void *sdfCreateTaskHeader(u32 userData) {
    s32 allocation = func_002D03F8(0x1C);
    SdfTaskHeader *obj = sdfMemoryGetBlockAddress(allocation);

    memset(obj, 0, 0x1C);
    obj->allocation = allocation;
    obj->userData = userData;
    obj->callback14 = (u32)func_002CC740;
    obj->onDestroy = (void (*)(s32, u32))func_002CC740;
    return obj;
}

void sdfDestroyTaskWork(SdfTaskHeader *work) {
    if (work != NULL) {
        sdfClearTaskList();
        work->onDestroy(-1, work->userData);
        func_002D0918(work->allocation);
    }
}

void sdfSetTaskDestroyCallback(SdfTaskHeader *work, s32 callback) {
    if (callback != 0) {
        work->onDestroy = (void (*)(s32, u32))callback;
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAD30);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CADD0);

void sdfSetTaskSecondaryCallback(SdfTaskHeader *work, s32 callback) {
    if (callback != 0) {
        work->callback14 = callback;
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

typedef struct SdfLink {
    u8 pad00[8];
    struct SdfLink *prev;
    struct SdfLink *next;
} SdfLink;
void func_002CB058(SdfLink *list, SdfLink *a, SdfLink *b) {
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

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB120);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB1C8);

typedef struct TaskWork {
    u32 handle;
    char *primaryTaskName;
    char *secondaryTaskName;
    TaskList *list;
    u32 firstItemHandle;
} TaskWork;

void sdfDestroyTaskWorkerTasks(TaskWork *work) {
    if (work != NULL) {
        kwlnTaskDestroyWithHierarchyByName(work->primaryTaskName, 1);
        kwlnTaskDestroyWithHierarchyByName(work->secondaryTaskName, 0);
    }
}

u8 sdfIsPrimaryTaskRegistered(TaskWork *work) {
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

u32 *sdfFindTaskItemValueByKey(TaskWork *work, s32 key) {
    TaskListNode *item;

    item = sdfFindTaskListNodeByKey(work->list, key);
    if (item != NULL) {
        return (u32 *)item->value;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB3B8);

INCLUDE_RODATA(const s32, "game/code_002C97E8", D_003B3EE0);

void sdfSetTaskItemMode(TaskWork *work, s32 key, u32 mode) {
    u32 *item = sdfFindTaskItemValueByKey(work, key);
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

typedef struct SdfTaskResourceWork {
    u32 allocation;            /* 0x00 */
    void *firstBuffer;          /* 0x04 */
    void *secondBuffer;         /* 0x08 */
    SdfTaskHeader *taskWork;    /* 0x0C */
} SdfTaskResourceWork;

void sdfDestroyTaskResourceWork(SdfTaskResourceWork *work) {
    if (work != NULL) {
        sdfDestroyTaskWork(work->taskWork);
        func_002CFF98(work->firstBuffer);
        func_002CFF98(work->secondBuffer);
        func_002D0918(work->allocation);
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB5F0);

typedef struct SdfCallbackWork {
    u8 pad00[4];
    u32 firstArg;                  /* 0x04 */
    u8 pad08[4];
    void (*destroy)(u32, u32);   /* 0x0C */
    u8 pad10[8];
    u32 secondArg;                 /* 0x18 */
} SdfCallbackWork;

void sdfDestroyCallbackWork(SdfCallbackWork *work) {
    if (work != NULL) {
        void (*destroy)(u32, u32) = work->destroy;
        destroy(work->firstArg, work->secondArg);
        func_002CFF98(work);
    }
}

void func_002CB6F8(u32 unused, SdfCallbackWork *work) {
    sdfDestroyCallbackWork(work);
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB718);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB850);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB8E0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB938);

void func_002CB990(void) {
    sdfDestroyTaskResourceWork(func_00101A70());
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

typedef struct SdfGridCell {
    u32 index;
    u32 value;
} SdfGridCell;

typedef struct SdfGrid {
    u32 allocation;        /* 0x00 */
    SdfGridCell *cells;    /* 0x04 */
    SdfGridCell *cursor;   /* 0x08 */
    u32 pad0C;
    u32 cellCount;         /* 0x10 */
    u32 width;             /* 0x14 */
    u32 pad18;
    void (*releaseCell)(u32, u32); /* 0x1C */
} SdfGrid;

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBB68);


void sdfGridGetCursorCoordinates(SdfGrid *grid, u32 *mod, u32 *div) {
    SdfGridCell *cell = grid->cursor;
    *mod = cell->index % grid->width;
    *div = cell->index / grid->width;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBBD8);

void sdfGridSetCellValue(SdfGrid *grid, s32 column, s32 row, u32 value) {
    u32 cellIndex;

    cellIndex = row * grid->width + column;
    if (cellIndex < grid->cellCount) {
        grid->cells[cellIndex].value = value;
    }
}

SdfGridCell *sdfGridCursorUp(SdfGrid *grid) {
    SdfGridCell *cell = grid->cursor;
    u32 width = grid->width;
    u32 index = cell->index;

    cell -= width;
    if (index < width) {
        return NULL;
    }
    grid->cursor = cell;
    func_002CC5F0((u8 *)grid);
    return cell;
}

SdfGridCell *sdfGridCursorDown(SdfGrid *grid) {
    SdfGridCell *cell = grid->cursor;
    u32 width = grid->width;

    if (cell->index >= grid->cellCount - width) {
        return NULL;
    }
    cell += width;
    grid->cursor = cell;
    func_002CC5F0((u8 *)grid);
    return cell;
}

SdfGridCell *sdfGridCursorLeft(SdfGrid *grid) {
    SdfGridCell *cell = grid->cursor;
    u32 width = grid->width;
    u32 index = cell->index;

    cell -= 1;
    if (index % width == 0) {
        return NULL;
    }
    grid->cursor = cell;
    func_002CC5F0((u8 *)grid);
    return cell;
}

SdfGridCell *sdfGridCursorRight(SdfGrid *grid) {
    SdfGridCell *cell = grid->cursor;
    u32 width = grid->width;
    u32 index = cell->index;
    cell += 1;
    if (index % width == width - 1) {
        return NULL;
    }
    grid->cursor = cell;
    func_002CC5F0((u8 *)grid);
    return cell;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBDA8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBE18);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBF60);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC0D0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC238);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC3A8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC430);

void func_002CC570(SdfGrid *grid) {
    u32 i = 0;
    SdfGridCell *cells = grid->cells;
    SdfGridCell *cell;

    if (grid->cellCount != 0) {
        cell = cells;
        do {
            u32 value = cell->value;
            cell->index = i;
            if (value != 0) {
                grid->releaseCell(i, value);
                cell->value = 0;
            }
            i++;
            cell++;
        } while (i < grid->cellCount);
    }
}

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

