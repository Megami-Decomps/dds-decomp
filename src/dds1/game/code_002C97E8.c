#include "common.h"

#include "fpu.h"

extern s8 D_00324510[];

extern u32 func_00197760(s32, s32, s32, u32, u32, s32);

extern void frFontDrawGlyphWithSharedFlags(u32, s32);

extern void frFontQueueGlyphInSelectedSlot(u32);

extern s32 frFontMeasureGlyphChain(u32);

extern void func_001958A0(u32, s32, s32);

extern u32 func_001951C8(u32, u32, s32, u32, u32);

extern void frFontSetChildColors(u32, u32);

extern void frFontSetContextPair(u32, s32, s32);

extern void frFontStoreShiftedContextValue(u32, u32);

extern void frFontSetChainFlag(u32, u8);

extern u32 func_00197C40(s32, s32, u32, u16, u32, u32);

extern f32 fldNormalizedVectorDot(f32 *, f32 *);

extern f32 func_002FA1C0(f32);

extern f32 sdfSinPoly(f32);

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern void sdfQuatMultiply(f32 *, f32 *, f32 *);

extern void sdfConvertQuaternionRotationMatrix(f32 *, f32 *);

extern void sdfTransformDirectionByMatrix(f32 *, f32 *);

extern void func_002C84F0(f32 *);

extern void fldNormalizedVectorCross(f32 *, f32 *, f32 *);

extern void sdfVec3ScaleInPlace(f32, f32 *);

extern void func_002CC5F0(u8 *);

extern f32 sdfQuatDot(f32 *, f32 *);

extern f32 func_002FA060(f32);

extern s32 kwlnTaskGetTaskByName(u32);

typedef struct ShortPair2C {
    u8 pad_0x00[0x2C]; // 0x00
    s16 h2C;           // 0x2C
    s16 h2E;           // 0x2E
} ShortPair2C; // 0x30

extern u32 func_002CB5F0(u32 *);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void func_002CC740();

typedef struct SdfListNode {
    u32 index;                  /* 0x00 */
    s32 key;                    /* 0x04 */
    struct SdfListNode *next;   /* 0x08 */
    struct SdfListNode *prev;   /* 0x0C */
    void *value;                /* 0x10 */
} SdfListNode;

typedef struct SdfList {
    u32 pad00;
    u32 count;                  /* 0x04 */
    SdfListNode *head;          /* 0x08 */
    SdfListNode *tail;          /* 0x0C */
    u32 pad10;
    void (*onRemove)(u32, void *); /* 0x14 */
} SdfList;

extern void *sdfAllocSizeClassBlock(s32);

float sdfQuatLengthSquared(float *quaternion) {
    return *quaternion * *quaternion + quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2] +
                  quaternion[3] * quaternion[3];
}

float sdfQuaternionMagnitude(float *values) {
    return fsqrtf(sdfQuatLengthSquared(values));
}

void sdfQuaternionInverse(float *values) {
    float lengthSquared = sdfQuatLengthSquared(values);
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

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9948);

/* Quaternion rotating direction `from` onto `to`. */
void sdfQuatFromVectors(f32 *out, f32 *from, f32 *to) {
    f32 cross[4];
    f32 scale;

    func_002C84F0(from);
    func_002C84F0(to);
    fldNormalizedVectorCross(cross, from, to);
    scale = fsqrtf(2.0f * (fldNormalizedVectorDot(from, to) + 1.0f));
    out[0] = cross[0] / scale;
    out[1] = cross[1] / scale;
    out[2] = cross[2] / scale;
    out[3] = scale * 0.5f;
}

/* Quaternion from Euler angles: half angles are negated. */
void sdfQuatFromEuler(f32 *out, f32 x, f32 y, f32 z) {
    f32 half;
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;

    half = -x * 0.5f;
    cx = sdfEvaluateCosineViaSinePhaseShift(half);
    sx = sdfSinPoly(half);
    half = -y * 0.5f;
    cy = sdfEvaluateCosineViaSinePhaseShift(half);
    sy = sdfSinPoly(half);
    half = -z * 0.5f;
    cz = sdfEvaluateCosineViaSinePhaseShift(half);
    sz = sdfSinPoly(half);
    out[0] = sz * sy * cx + cz * cy * sx;
    out[1] = cz * sy * cx - sz * cy * sx;
    out[2] = sz * cy * cx + cz * sy * sx;
    out[3] = cz * cy * cx - sz * sy * sx;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9BF0);

void sdfQuaternionBlendNormalize(float *out, float *from, float *to, float fraction) {
    float inv = 1.0f - fraction;
    out[0] = from[0] * inv + to[0] * fraction;
    out[1] = from[1] * inv + to[1] * fraction;
    out[2] = from[2] * inv + to[2] * fraction;
    out[3] = from[3] * inv + to[3] * fraction;
    sdfQuaternionNormalize(out);
}

/* Spherical interpolation along the shorter arc; falls back to a normalized blend for near-parallel inputs. */
void sdfQuatSlerp(f32 *out, f32 *from, f32 *to, f32 fraction) {
    f32 target[4];
    f32 angle = sdfQuatDot(from, to);
    f32 firstWeight;
    f32 secondWeight;
    f32 denominator;

    if (angle < 0.0f) {
        angle = -angle;
        target[0] = -to[0];
        target[1] = -to[1];
        target[2] = -to[2];
        target[3] = -to[3];
    } else {
        memcpy(target, to, 16);
    }
    if (angle < 0.95f) {
        angle = func_002FA1C0(angle);
        firstWeight = func_002FA060(angle * (1.0f - fraction));
        secondWeight = func_002FA060(angle * fraction);
        denominator = func_002FA060(angle);
        out[0] = (from[0] * firstWeight + target[0] * secondWeight) / denominator;
        out[1] = (from[1] * firstWeight + target[1] * secondWeight) / denominator;
        out[2] = (from[2] * firstWeight + target[2] * secondWeight) / denominator;
        out[3] = (from[3] * firstWeight + target[3] * secondWeight) / denominator;
    } else {
        sdfQuaternionBlendNormalize(out, from, target, fraction);
    }
}

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
        sdfQuaternionBlendNormalize(out, from, to, fraction);
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
    f32 sine = sdfSinPoly(angle);
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
    f32 sine = sdfSinPoly(length);
    out[3] = sdfEvaluateCosineViaSinePhaseShift(length);
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
    sdfConvertQuaternionRotationMatrix(matrix, rotation);
    sdfTransformDirectionByMatrix(vector, matrix);
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

f32 sdfDivideVerticalMagnitudeByForwardDot(f32 *direction, f32 *target) {
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

/* Intersect the rotated Z axis with the plane at height plane[1]; write the hit vector to out. */
void sdfRayPlaneHit(f32 *plane, f32 *rotation, f32 *out) {
    f32 matrix[16];
    f32 forward[4] = {0, 0, 1.0f, 0};
    f32 up[4] = {0, 1.0f, 0, 0};
    f32 dot;
    f32 dist;

    sdfConvertQuaternionRotationMatrix(matrix, rotation);
    sdfTransformDirectionByMatrix(forward, matrix);
    dot = fldNormalizedVectorDot(up, forward);
    func_002C84F0(forward);
    dist = plane[1];
    if (dist < 0.0f) {
        dist = -dist / dot;
    } else {
        dist = dist / dot;
    }
    sdfVec3ScaleInPlace(dist, forward);
    memcpy(out, forward, 16);
}

void sdfFontRegisterShort(s32 x, s32 y, u32 first, u32 second) {
    u32 handle = func_00197760(x << 4, y << 3, 0, first, second, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
}

s32 frFontMeasureAndQueueGlyph(s32 x, s32 y, u32 first, u32 second, u32 third, s32 option) {
    u32 handle = func_00197760(x << 4, y << 3, first, second, third, 0);
    s32 result = frFontMeasureGlyphChain(handle);
    func_001958A0(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

s32 frFontQueueTextAndOptionallyMeasure(s32 x, s32 y, u32 first, u32 second, s8 type, u32 name, s32 flag, s32 option) {
    u32 handle = func_001951C8(name, 0, type, 0, 0);
    s32 result = 0;
    frFontSetChildColors(handle, second);
    frFontSetContextPair(handle, x << 4, y << 3);
    frFontStoreShiftedContextValue(handle, first);
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

s32 frFontDrawStyledGlyphChainAndMeasure(s32 x, s32 y, s32 z, u32 w, u8 flags, u32 style, s32 width, u32 color) {
    u32 handle = func_00197760(x << 4, y << 3, z, w, style, 0);
    s32 result;

    frFontSetChainFlag(handle, flags);
    if (width >= 0) {
        result = 0;
    } else {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, color);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

extern u32 func_00197A98(s32, s32, s32, u32, u32, s32);

s32 itfDrawGlyphChainWithWidthQuery(s32 x, s32 y, s32 z, u32 w, u8 flags, u32 style, s32 width, u32 color) {
    u32 handle = func_00197A98(x << 4, y << 3, z, w, style, 0);
    s32 result;

    frFontSetChainFlag(handle, flags);
    if (width >= 0) {
        result = 0;
    } else {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, color);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

extern u32 func_001978E8(s32, s32, s32, u32, u32, s32);

s32 frFontQueueFlaggedGlyphAndMeasure(s32 x, s32 y, s32 z, u32 w, u8 flags, u32 style, s32 width, u32 color) {
    u32 handle = func_001978E8(x << 4, y << 3, z, w, style, 0);
    s32 result;

    frFontSetChainFlag(handle, flags);
    if (width >= 0) {
        result = 0;
    } else {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, color);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

s32 frFontDrawColoredGlyphChainAndMeasure(s32 x, s32 y, u32 first, u32 second, u8 opacity, u16 width, u32 name, u32 extra, s32 flag, s32 option) {
    u32 handle = func_00197C40(x << 4, y << 3, first, width, name, extra);
    s32 result = 0;
    frFontSetChainFlag(handle, opacity);
    frFontSetChildColors(handle, second);
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_001958A0(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
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
    s32 allocation = sdfAllocGeneralBlock(0x1C);
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
        sdfReleaseResourceAllocation(work->allocation);
    }
}

void sdfSetTaskDestroyCallback(SdfTaskHeader *work, s32 callback) {
    if (callback != 0) {
        work->onDestroy = (void (*)(s32, u32))callback;
    }
}

SdfListNode *sdfListAppend(SdfList *list, s32 key, void *value) {
    SdfListNode *node = sdfAllocSizeClassBlock(0x14);

    memset(node, 0, 0x14);
    node->value = value;
    node->index = list->count++;
    node->key = key;
    if (list->head == NULL) {
        list->head = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
    }
    list->tail = node;
    return node;
}

/* Insert a new node after `after`, bumping the index of every later node. */
SdfListNode *sdfListInsertAfter(SdfList *list, SdfListNode *after, s32 key, void *value) {
    SdfListNode *node = sdfAllocSizeClassBlock(0x14);
    SdfListNode *it;

    memset(node, 0, 0x14);
    node->index = after->index + 1;
    node->key = key;
    node->value = value;
    list->count++;
    for (it = after->next; it != NULL; it = it->next) {
        if (it->index != 0) {
            it->index++;
        }
    }
    node->prev = after;
    if (after->next != NULL) {
        node->next = after->next;
        after->next->prev = node;
        after->next = node;
    } else {
        after->next = node;
        list->tail = node;
    }
    return node;
}

void sdfSetTaskSecondaryCallback(SdfTaskHeader *work, s32 callback) {
    if (callback != 0) {
        work->callback14 = callback;
    }
}

SdfListNode *sdfRemoveAndReindexListNode(SdfList *list, SdfListNode *node) {
    SdfListNode *it;

    if (node == NULL) {
        return NULL;
    }
    for (it = list->head; it != NULL; it = it->next) {
        if (node->index < it->index) {
            it->index = it->index - 1;
        }
    }
    list->count = list->count - 1;
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node == list->head) {
        list->head = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }
    if (node->next != NULL) {
        return node->next;
    }
    return node->prev;
}

/* Unlink a node, hand it to the list's removal callback, free it, return its neighbour. */
SdfListNode *sdfListRemoveNode(SdfList *list, SdfListNode *node) {
    SdfListNode *neighbour;

    if (node == NULL) {
        return NULL;
    }
    neighbour = sdfRemoveAndReindexListNode(list, node);
    list->onRemove(node->index, node->value);
    sdfReleaseChipBlock(node);
    return neighbour;
}

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
    u32 current; /* 0x10 */
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
                sdfReleaseChipBlock(current);
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

void sdfSwapLinkedListNodes(SdfLink *list, SdfLink *a, SdfLink *b) {
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

u32 sdfReadPadDirectionMask(void) {
    u32 flags = 0;

    if (D_00324510[0x26] < 0) {
        flags |= 1;
    }
    if (D_00324510[0x26] & 2) {
        flags |= 1;
    }
    if (D_00324510[0x26]) {
        flags |= 2;
    }
    if (D_00324510[0x27] < 0) {
        flags |= 4;
    }
    if (D_00324510[0x27] & 2) {
        flags |= 4;
    }
    if (D_00324510[0x27]) {
        flags |= 8;
    }
    if (D_00324510[0x24] < 0) {
        flags |= 0x10;
    }
    if (D_00324510[0x24] & 2) {
        flags |= 0x10;
    }
    if (D_00324510[0x24]) {
        flags |= 0x20;
    }
    if (D_00324510[0x25] < 0) {
        flags |= 0x40;
    }
    if (D_00324510[0x25] & 2) {
        flags |= 0x40;
    }
    if (D_00324510[0x25]) {
        flags |= 0x80;
    }
    return flags;
}

typedef struct TaskWork {
    u32 handle;
    char *primaryTaskName;
    char *secondaryTaskName;
    TaskList *list;
    u32 firstItemHandle;
} TaskWork;

extern s32 sdfTaskWorkRunAllEntries(void);
extern s32 sdfTaskWorkRunAll(void);
extern void sdfReleaseCurrentTaskOwnedResources(void);
extern void kwlnTaskCreate();
extern TaskWork *sdfCreateNamedTaskWork();

TaskWork *sdfCreateTaskWorker(char *name, s32 first, s32 second, u32 item, s32 destroyCallback, u32 userData) {
    TaskWork *work;

    work = sdfCreateNamedTaskWork(name, destroyCallback, userData);
    sdfAttachTaskItem(work, item);
    kwlnTaskCreate(work->primaryTaskName, first, 1, 1, sdfTaskWorkRunAllEntries, sdfReleaseCurrentTaskOwnedResources, work);
    kwlnTaskCreate(work->secondaryTaskName, second, 1, 0, sdfTaskWorkRunAll, NULL, work);
    return work;
}

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
    u32 result = (u32)sdfListAppend((SdfList *)work->list, *item, (void *)func_002CB5F0(item));
    if (work->firstItemHandle == 0) {
        work->firstItemHandle = result;
    }
}

void sdfRemoveTaskItem(TaskWork *work, s32 key) {
    void *item = sdfFindTaskListNodeByKey(work->list, key);
    if (item != NULL) {
        sdfListRemoveNode(work->list, item);
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

extern s32 func_003014F0(char *buffer, const char *fmt, ...);
extern void sdfCallbackWorkOnRemove();

/* Create a task resource work block with the name copied to two formatted buffers. */
TaskWork *sdfCreateNamedTaskWork(char *name, s32 destroyCallback, u32 userData) {
    s32 allocation = sdfAllocGeneralBlock(0x14);
    TaskWork *work = sdfMemoryGetBlockAddress(allocation);

    memset(work, 0, 0x14);
    work->handle = allocation;
    work->list = sdfCreateTaskHeader(userData);
    sdfSetTaskDestroyCallback(work->list, destroyCallback);
    sdfSetTaskSecondaryCallback(work->list, (s32)sdfCallbackWorkOnRemove);
    work->primaryTaskName = sdfAllocSizeClassBlock(strlen(name));
    work->secondaryTaskName = sdfAllocSizeClassBlock(strlen(name) + 5);
    func_003014F0(work->primaryTaskName, "%s", name);
    func_003014F0(work->secondaryTaskName, "draw_%s", name);
    return work;
}

typedef struct SdfTaskResourceWork {
    u32 allocation;            /* 0x00 */
    void *firstBuffer;          /* 0x04 */
    void *secondBuffer;         /* 0x08 */
    SdfTaskHeader *taskWork;    /* 0x0C */
} SdfTaskResourceWork;

void sdfDestroyTaskResourceWork(SdfTaskResourceWork *work) {
    if (work != NULL) {
        sdfDestroyTaskWork(work->taskWork);
        sdfReleaseChipBlock(work->firstBuffer);
        sdfReleaseChipBlock(work->secondBuffer);
        sdfReleaseResourceAllocation(work->allocation);
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
        sdfReleaseChipBlock(work);
    }
}

void sdfCallbackWorkOnRemove(u32 unused, SdfCallbackWork *work) {
    sdfDestroyCallbackWork(work);
}

typedef struct SdfTaskEntry {
    u32 flags;                   /* 0x00 */
    s32 arg0;                    /* 0x04 */
    s32 (*init)(void);           /* 0x08 */
    u8 pad0C[4];
    s32 (*update)(s32, s32);     /* 0x10 */
    void (*callback)(s32, s32);  /* 0x14 */
    s32 arg1;                    /* 0x18 */
} SdfTaskEntry;

extern void *kwlnTaskGetUserValue(void);

s32 sdfTaskWorkStepEntry(TaskWork *work) {
    TaskListNode *node = (TaskListNode *)work->firstItemHandle;
    SdfTaskEntry *entry;
    u32 flags;

    if (node == NULL) {
        work->firstItemHandle = (u32)work->list->head;
        return 0;
    }
    entry = (SdfTaskEntry *)node->value;
    flags = entry->flags;
    work->firstItemHandle = (u32)node->next;
    switch (flags & 0xFFFF0000) {
    case 0x100000:
        flags = entry->flags = (flags & 0xFFEFFFFF) | 0x10000;
    case 0x10000:
        if (flags & 4) {
            entry->arg1 = entry->init();
            flags = entry->flags &= ~4;
        }
        if (flags & 0x8000) {
            sdfRemoveTaskItem(work, entry->arg0);
            return 1;
        }
        if (flags & 1) {
            if (entry->update(entry->arg0, entry->arg1) == -1) {
                entry->flags |= 0x8000;
            }
        }
        break;
    case 0x20000:
        break;
    }
    return 1;
}

/* Advance the work's cursor one node and run the node's callback when flagged. */
s32 sdfTaskWorkStep(TaskWork *work) {
    TaskListNode *node = (TaskListNode *)work->firstItemHandle;
    SdfTaskEntry *entry;
    u32 flags;

    if (node == NULL) {
        work->firstItemHandle = (u32)work->list->head;
        return 0;
    }
    entry = (SdfTaskEntry *)node->value;
    flags = entry->flags;
    work->firstItemHandle = (u32)node->next;
    switch (flags & 0xFFFF0000) {
    case 0x10000:
        if (flags & 2) {
            entry->callback(entry->arg0, entry->arg1);
        }
        break;
    case 0x20000:
        break;
    case 0x100000:
        break;
    }
    return 1;
}

s32 sdfTaskWorkRunAllEntries(void) {
    TaskWork *work = kwlnTaskGetUserValue();

    if (work->firstItemHandle == 0) {
        return -1;
    }
    while (sdfTaskWorkStepEntry(work) == 1) {
    }
    return 0;
}

s32 sdfTaskWorkRunAll(void) {
    TaskWork *work = kwlnTaskGetUserValue();

    if (work->firstItemHandle == 0) {
        return -1;
    }
    while (sdfTaskWorkStep(work) == 1) {
    }
    return 0;
}

void sdfReleaseCurrentTaskOwnedResources(void) {
    sdfDestroyTaskResourceWork(kwlnTaskGetUserValue());
}

void func_002CB9B8(void) {
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB9C0);

void sdfSetShortPairValues(ShortPair2C *p, s32 a, s32 b) {
    p->h2C = a;
    p->h2E = b;
}

void sdfDestroyGridWork(u8 *work) {
    if (work != NULL) {
        sdfGridReleaseAllCells();
        (*(void (**)(s32, u32))(work + 0x20))(0, *(u32 *)(work + 0x30));
        sdfReleaseResourceAllocation(*(u32 *)work);
    }
}

void func_002CBB48(void) {
    sdfGridReleaseAllCells();
}

typedef struct SdfGridCell {
    u32 index;
    u32 value;
} SdfGridCell;

typedef struct SdfGrid {
    u32 allocation;        /* 0x00 */
    SdfGridCell *cells;    /* 0x04 */
    SdfGridCell *cursor;   /* 0x08 */
    SdfGridCell *viewportOrigin; /* 0x0C */
    u32 cellCount;         /* 0x10 */
    u32 width;             /* 0x14 */
    void (*drawCell)(s32, s32, s32, struct SdfGrid *, SdfGridCell *, s32); /* 0x18 */
    void (*releaseCell)(u32, u32); /* 0x1C */
    void (*onSelect)();    /* 0x20 */
    u16 cellWidth;         /* 0x24 */
    u16 cellHeight;        /* 0x26 */
    u16 visibleColumns;    /* 0x28 */
    u16 visibleRows;       /* 0x2A */
    u16 columnMargin;      /* 0x2C */
    u16 rowMargin;         /* 0x2E */
    u32 userData;          /* 0x30 */
} SdfGrid;

SdfGridCell *sdfGridGetCell(SdfGrid *grid, s32 column, s32 row) {
    u32 cellIndex;

    cellIndex = row * grid->width + column;
    if (cellIndex >= grid->cellCount) {
        return NULL;
    }
    return &grid->cells[cellIndex];
}

void sdfGridGetCursorCoordinates(SdfGrid *grid, u32 *mod, u32 *div) {
    SdfGridCell *cell = grid->cursor;
    *mod = cell->index % grid->width;
    *div = cell->index / grid->width;
}

u32 sdfGridGetCellValue(SdfGrid *grid, s32 column, s32 row) {
    u32 cellIndex;

    cellIndex = row * grid->width + column;
    if (cellIndex >= grid->cellCount) {
        return 0;
    }
    return grid->cells[cellIndex].value;
}

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

SdfGridCell *sdfGridSetCursorCell(SdfGrid *grid, u32 column, u32 row) {
    SdfGridCell *result = NULL;

    if (column >= grid->width) {
        return result;
    }
    if (row >= grid->cellCount / grid->width) {
        return result;
    }
    grid->cursor = sdfGridGetCell(grid, column, row);
    func_002CC5F0((u8 *)grid);
    return grid->cursor;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBE18);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBF60);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC0D0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC238);

SdfGridCell *sdfGridSelectFilledCell(SdfGrid *grid, u32 column, u32 row) {
    SdfGridCell *result = NULL;
    SdfGridCell *cell;

    if (column >= grid->width) {
        return result;
    }
    if (row >= grid->cellCount / grid->width) {
        return result;
    }
    cell = sdfGridGetCell(grid, column, row);
    if (cell->value == 0) {
        return NULL;
    }
    grid->cursor = cell;
    func_002CC5F0((u8 *)grid);
    return cell;
}

void sdfGridDrawVisibleCells(s32 x, s32 y, s32 layer, SdfGrid *grid, s32 context) {
    SdfGridCell *cell = grid->viewportOrigin;
    s32 firstRow = cell->index / grid->width;
    s32 firstColumn = cell->index % grid->width;
    s32 endRow = firstRow + grid->visibleRows;
    s32 row;
    s32 column;

    for (row = firstRow; row < endRow; row++) {
        cell = grid->cells + row * grid->width + firstColumn;
        for (column = firstColumn; column < firstColumn + grid->visibleColumns; column++) {
            grid->drawCell(x + (column - firstColumn) * grid->cellWidth,
                           y + (row - firstRow) * grid->cellHeight,
                           layer, grid, cell, context);
            cell++;
        }
    }
}

void sdfGridReleaseAllCells(SdfGrid *grid) {
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

float sdfMultiplyAddFloat(float addend, float multiplicand, float multiplier) {
    return addend + multiplicand * multiplier;
}

u32 func_002CC738(void) {
    return 0;
}

void func_002CC740(void) {
}

u32 func_002CC748(void) {
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD298);

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD2A0);

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD2A8);

INCLUDE_SDATA(const s32, "game/code_002C97E8", D_003BD2B0);

