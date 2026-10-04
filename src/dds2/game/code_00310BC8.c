#include "common.h"

#include "fpu.h"

extern s8 D_0037F510[];

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void func_00313BA8(s32, s32);

extern s32 sdfReleaseResourceAllocation(u32);

extern void sdfReleaseChipBlock();

extern void sdfClearTaskList();

extern void sdfDestroyCallbackWork();

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern s32 kwlnTaskGetTaskByName(u32);


/* Task item descriptor (0x14): key plus optional handlers, defaults filled in by func_00312A48. */
typedef struct SdfTaskItemDesc {
    s32 key;                         /* 0x00 */
    s32 (*init)(void);               /* 0x04 */
    void (*destroy)(s32, s32);       /* 0x08 */
    s32 (*update)(s32, s32);         /* 0x0C */
    void (*callback)(s32, s32);      /* 0x10 */
} SdfTaskItemDesc;

extern void *func_00312A48(SdfTaskItemDesc *);



extern s32 kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern void sdfGridReleaseAllCells();

extern f32 func_003532B8(f32);

extern f32 sdfSinPoly(f32);

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern void sdfQuatMultiply(f32 *, f32 *, f32 *);

extern f32 fldNormalizedVectorDot(f32 *, f32 *);

extern void func_00313A58(u8 *);

typedef struct SdfListNode {
    u32 index;                 /* 0x00 */
    s32 key;                   /* 0x04 */
    struct SdfListNode *next;  /* 0x08 */
    struct SdfListNode *prev;  /* 0x0C */
    void *value;               /* 0x10 */
} SdfListNode;

typedef struct SdfList {
    u32 allocation;            /* 0x00 */
    u32 count;                 /* 0x04 */
    SdfListNode *head;         /* 0x08 */
    SdfListNode *tail;         /* 0x0C */
    u32 userData;              /* 0x10 */
    void (*onRemove)();        /* 0x14: ordinal/payload hook, installed through the word-address API */
    void (*onDestroy)(s32, s32); /* 0x18 */
} SdfList;                     /* 0x1C, allocated by sdfCreateTaskHeader */

typedef struct TaskWork {
    u32 allocation;
    char *primaryTaskName;
    char *secondaryTaskName;
    SdfList *list;
    SdfListNode *currentNode; /* Next node to visit; reset to the list head at pass end. */
} TaskWork;

/* Low flag bits: 0 update, 1 callback, 2 initialize once, 15 pending removal.
 * The high word selects active, suspended, or pending-activation dispatch modes. */
typedef struct SdfTaskEntry {
    u32 flags;                   /* 0x00 */
    s32 key;                     /* 0x04 */
    s32 (*init)(void);           /* 0x08 */
    void (*destroy)(s32, s32);   /* 0x0C */
    s32 (*update)(s32, s32);     /* 0x10 */
    void (*callback)(s32, s32);  /* 0x14 */
    s32 initResult;              /* 0x18 */
} SdfTaskEntry;


extern f32 sdfQuatDot(f32 *, f32 *);

extern f32 func_00353140(f32);

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
    void (*onDestroy)(s32, u32); /* 0x20 */
    u16 cellWidth;         /* 0x24 */
    u16 cellHeight;        /* 0x26 */
    u16 visibleColumns;    /* 0x28 */
    u16 visibleRows;       /* 0x2A */
    u16 columnMargin;      /* 0x2C */
    u16 rowMargin;         /* 0x2E */
    u32 userData;          /* 0x30 */
} SdfGrid;

extern void sdfConvertQuaternionRotationMatrix(f32 *, f32 *);
extern void sdfTransformDirectionByMatrix(f32 *, f32 *);
extern void func_0030F8D0(f32 *);
extern void fldNormalizedVectorCross(f32 *, f32 *, f32 *);
extern void sdfVec3ScaleInPlace(f32, f32 *);

extern void func_0019D550(u64, s32, s32);
extern void frFontSetChildColors(u64, u64);
extern u64 func_0019CE78(u64, u64, s32, u64, u64);
extern void frFontSetContextPair(u64, s32, s32);
extern void frFontStoreShiftedContextValue(u64, u64);
extern void frFontSetChainFlag(u64, u8);
extern void frFontSetFlagAndMeasureGlyphs(u64, s32);

extern u64 func_0019F798(s32, s32, u64, u64, u64, u64);
extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);
extern u64 itfDrawBankTextWithLayoutFlags(s32, s32, u64, u64, u64, u64);

extern void *sdfAllocSizeClassBlock(s32);
extern void *memset(void *, s32, u32);

extern void func_00313BA8(s32, s32);

extern s32 func_00313BA0(void);
extern s32 func_00313BB0(s32, s32);

extern s32 sdfAllocGeneralBlock(s32);
extern u32 strlen(const char *);
extern s32 func_0035C860(char *buffer, const char *fmt, ...);
extern char D_004388D8[];
extern char D_004388E0[];
extern void sdfCallbackWorkOnRemove();

extern s32 sdfTaskWorkRunAllEntries(void);
extern s32 sdfTaskWorkRunAll(void);
extern void kwlnTaskCreate();
extern TaskWork *sdfCreateNamedTaskWork();

extern void sdfReleaseCurrentTaskOwnedResources(void);
extern s32 sdfTaskWorkRunAllEntries(void);
extern s32 sdfTaskWorkRunAll(void);
extern void sdfReleaseCurrentTaskOwnedResources(void);
extern void kwlnTaskCreate();
extern TaskWork *sdfCreateNamedTaskWork();

extern void func_00312E20(void);

float sdfQuatLengthSquared(float *quaternion) {
    return *quaternion * *quaternion + quaternion[1] * quaternion[1] +
                  quaternion[2] * quaternion[2] + quaternion[3] * quaternion[3];
}

float sdfQuaternionMagnitude(float *quaternion) {
    return fsqrtf(sdfQuatLengthSquared(quaternion));
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

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00310D28);

/* Quaternion rotating direction `from` onto `to`. */
void sdfQuatFromVectors(f32 *out, f32 *from, f32 *to) {
    f32 cross[4];
    f32 scale;

    func_0030F8D0(from);
    func_0030F8D0(to);
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

void func_00310FD0(f32 *axis, f32 *angle, f32 *quaternion) {
    f32 w = quaternion[3];
    f32 twiceAngle = 2.0f * func_003532B8(w);
    f32 scale = fsqrtf(1.0f - w * w);
    double magnitude = scale;

    if (magnitude < 0.0) {
        magnitude = -magnitude;
    }
    if (magnitude < 0.0005f) {
        scale = 1.0f;
    }
    axis[0] = quaternion[0] / scale;
    axis[1] = quaternion[1] / scale;
    axis[2] = quaternion[2] / scale;
    *angle = twiceAngle;
}

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
        angle = func_003532B8(angle);
        firstWeight = func_00353140(angle * (1.0f - fraction));
        secondWeight = func_00353140(angle * fraction);
        denominator = func_00353140(angle);
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
        f32 firstWeight = func_00353140(angle * (1.0f - fraction));
        f32 secondWeight = func_00353140(angle * fraction);
        f32 denominator = func_00353140(angle);
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
    f32 angle = func_003532B8(in[3]);
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
    func_0030F8D0(vector);
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
    func_0030F8D0(forward);
    dist = plane[1];
    if (dist < 0.0f) {
        dist = -dist / dot;
    } else {
        dist = dist / dot;
    }
    sdfVec3ScaleInPlace(dist, forward);
    memcpy(out, forward, 16);
}

void sdfFontRegisterShort(s32 x, s32 y, u64 first, u64 second) {
    u64 handle;

    handle = func_0019F460(x << 4, y << 3, 0, first, second, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
}

s32 frFontMeasureAndQueueGlyph(s32 x, s32 y, u64 first, u64 second, u64 third, s32 option) {
    u64 handle = func_0019F460(x << 4, y << 3, first, second, third, 0);
    s32 result = frFontMeasureGlyphChain(handle);
    func_0019D550(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

s32 frFontQueueTextAndOptionallyMeasure(s32 x, s32 y, u64 first, u64 second, s8 type, u64 name, s32 flag, s32 option) {
    u64 handle = func_0019CE78(name, 0, type, 0, 0);
    s32 result = 0;
    frFontSetChildColors(handle, second);
    frFontSetContextPair(handle, x << 4, y << 3);
    frFontStoreShiftedContextValue(handle, first);
    if (flag & 0x100) {
        frFontSetFlagAndMeasureGlyphs(handle, flag & 0xFF);
    }
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_0019D550(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

s32 frFontDrawStyledGlyphChainAndMeasure(s32 x, s32 y, u64 z, u64 w, u8 flags, u64 style, s32 flag, s32 option) {
    u64 handle = func_0019F460(x << 4, y << 3, z, w, style, 0);
    s32 result = 0;
    frFontSetChainFlag(handle, flags);
    if (flag & 0x100) {
        frFontSetFlagAndMeasureGlyphs(handle, flag & 0xFF);
    }
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_0019D550(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

s32 func_00311D00(s32 x, s32 y, u64 z, u64 w, u8 flags, u64 style, s32 flag, s32 option) {
    u64 handle = func_0019F798(x << 4, y << 3, z, w, style, 0);
    s32 result = 0;
    frFontSetChainFlag(handle, flags);
    if (flag & 0x100) {
        frFontSetFlagAndMeasureGlyphs(handle, flag & 0xFF);
    }
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_0019D550(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

s32 func_00311DB0(s32 x, s32 y, u64 z, u64 w, u8 flags, u64 style, s32 flag, s32 option) {
    u64 handle = func_0019F5E8(x << 4, y << 3, z, w, style, 0);
    s32 result = 0;
    frFontSetChainFlag(handle, flags);
    if (flag & 0x100) {
        frFontSetFlagAndMeasureGlyphs(handle, flag & 0xFF);
    }
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_0019D550(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

s32 frFontQueueTintedGlyphChainAndMeasure(s32 x, s32 y, u64 first, u64 second, u8 opacity, u16 width, u64 name, u64 extra, s32 flag, s32 option) {
    u64 handle = itfDrawBankTextWithLayoutFlags(x << 4, y << 3, first, width, name, extra);
    s32 result = 0;
    frFontSetChainFlag(handle, opacity);
    frFontSetChildColors(handle, second);
    if (flag & 0x100) {
        frFontSetFlagAndMeasureGlyphs(handle, flag & 0xFF);
    }
    if (flag < 0) {
        result = frFontMeasureGlyphChain(handle);
    }
    func_0019D550(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
    return result;
}

typedef struct SdfOverlaySurface {
    u8 pad00[0x10];
    void (*submit)(struct SdfOverlaySurface *, s32);
    u8 pad14[0xC];
} SdfOverlaySurface;

extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern s32 sdfConsInitPacketHeader(s32, s32, s32, s64, s32);
extern s32 sdfConsMeasurePacketWithHeader(s32);
extern void sdfAppendPacket(s32, s32);
extern SdfOverlaySurface kwlnDrawSurfaces[];

/* Build an indexed RGBA/XYZ2 packet and submit it through the selected surface. */
void func_00311F20(s32 *points, u32 tail, u32 *colors, s32 count,
                   s32 useFirstColor, s32 surfaceIndex) {
    s32 list = sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *vertex;
    SdfOverlaySurface *surface;
    u32 *selectedColors;
    s32 i;
    s32 x;
    s32 y;
    u32 color;

    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, count));
    sdfConsInitPacketHeader(packet, 0x14D, 2, 0x41, count);
    vertex = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    for (i = 0; i < count; i++) {
        if (useFirstColor & 1) {
            selectedColors = colors;
        } else {
            selectedColors = &colors[i];
        }
        color = *selectedColors;
        x = points[i * 2];
        y = points[i * 2 + 1];

        vertex[0] = (color & 0xFF) | ((u64)((color & 0xFF00) >> 8) << 32);
        vertex[1] = ((color & 0xFF0000) >> 16) |
                    ((u64)((color & 0xFF000000) >> 24) << 32);
        vertex += 2;
        vertex[0] = (u32)((x << 4) + 0x7000) |
                    ((u64)((y << 3) + 0x7900) << 32);
        vertex[1] = tail;
        vertex += 2;
    }
    sdfAppendPacket(list, packet);
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->submit(surface, list);
}

/* Return a callback-list header address, retaining its allocation handle and teardown userData. */
void *sdfCreateTaskHeader(u32 userData) {
    s32 allocation = sdfAllocGeneralBlock(0x1C);
    SdfList *obj = sdfMemoryGetBlockAddress(allocation);

    memset(obj, 0, 0x1C);
    obj->allocation = allocation;
    obj->userData = userData;
    obj->onRemove = func_00313BA8;
    obj->onDestroy = func_00313BA8;
    return obj;
}

s64 sdfDestroyTaskWork(SdfList *owner) {
    if (owner != NULL) {
        sdfClearTaskList(owner);
        owner->onDestroy(-1, owner->userData);
        return sdfReleaseResourceAllocation(owner->allocation);
    }
}

void sdfSetTaskDestroyCallback(s32 work, s32 callback) {
    if (callback != 0) {
        ((SdfList *)work)->onDestroy = (void (*)(s32, s32))callback;
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

void sdfSetTaskSecondaryCallback(s32 work, s32 callback) {
    if (callback != 0) {
        ((SdfList *)work)->onRemove = (void (*)())callback;
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

/* Release each row through its ordinal/payload hook, then clear tail, head and count in that order. */
void sdfClearTaskList(SdfList *list) {
    SdfListNode *node;
    if (list != NULL) {
        node = list->head;
        if (node != NULL) {
            do {
                SdfListNode *current = node;
                node = node->next;
                list->onRemove(current->index, current->value);
                sdfReleaseChipBlock(current);
            } while (node != NULL);
        }
        list->tail = 0;
        list->head = 0;
        list->count = 0;
    }
}

/* Exchange native next/prev links and list endpoints without changing row ordinals. */
void sdfSwapLinkedListNodes(SdfList *list, SdfListNode *a, SdfListNode *b) {
    SdfListNode *previous;
    SdfListNode *next;

    if (a != NULL && b != NULL) {
        if (a->next != NULL) {
            a->next->prev = b;
        }
        if (a->prev != NULL) {
            a->prev->next = b;
        }
        if (b->next != NULL) {
            b->next->prev = a;
        }
        if (b->prev != NULL) {
            b->prev->next = a;
        }
        previous = a->prev;
        a->prev = b->prev;
        next = a->next;
        a->next = b->next;
        b->prev = previous;
        b->next = next;
        if (a->prev == NULL) {
            list->head = a;
        }
        if (a->next == NULL) {
            list->tail = a;
        }
        if (b->prev == NULL) {
            list->head = b;
        }
        if (b->next == NULL) {
            list->tail = b;
        }
    }
}

/* Find a key in a nonempty list; the first node is read before traversal exhaustion is checked. */
void *sdfFindTaskListNodeByKey(SdfList *list, s32 key) {
    SdfListNode *node;

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

    if (D_0037F510[0x26] < 0) {
        flags |= 1;
    }
    if (D_0037F510[0x26] & 2) {
        flags |= 1;
    }
    if (D_0037F510[0x26]) {
        flags |= 2;
    }
    if (D_0037F510[0x27] < 0) {
        flags |= 4;
    }
    if (D_0037F510[0x27] & 2) {
        flags |= 4;
    }
    if (D_0037F510[0x27]) {
        flags |= 8;
    }
    if (D_0037F510[0x24] < 0) {
        flags |= 0x10;
    }
    if (D_0037F510[0x24] & 2) {
        flags |= 0x10;
    }
    if (D_0037F510[0x24]) {
        flags |= 0x20;
    }
    if (D_0037F510[0x25] < 0) {
        flags |= 0x40;
    }
    if (D_0037F510[0x25] & 2) {
        flags |= 0x40;
    }
    if (D_0037F510[0x25]) {
        flags |= 0x80;
    }
    return flags;
}

TaskWork *sdfCreateTaskWorker(char *name, s32 first, s32 second, u32 item, s32 destroyCallback, u32 userData) {
    TaskWork *work;

    work = sdfCreateNamedTaskWork(name, destroyCallback, userData);
    sdfAttachTaskItem(work, item);
    kwlnTaskCreate(work->primaryTaskName, first, 1, 1, sdfTaskWorkRunAllEntries, sdfReleaseCurrentTaskOwnedResources, work);
    kwlnTaskCreate(work->secondaryTaskName, second, 1, 0, sdfTaskWorkRunAll, NULL, work);
    return work;
}

s64 sdfDestroyTaskWorkerTasks(TaskWork *work) {
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
        task = kwlnTaskGetTaskByName((u32)work->primaryTaskName);
        exists = task != 0;
    }
    return exists;
}

s32 kwlnTaskExists(u32 name) {
    return kwlnTaskGetTaskByName(name) != 0;
}

void sdfAttachTaskItem(TaskWork *work, SdfTaskItemDesc *item) {
    SdfListNode *node = sdfListAppend(work->list, item->key, func_00312A48(item));
    if (work->currentNode == NULL) {
        work->currentNode = node;
    }
}

s64 sdfRemoveTaskItem(TaskWork *work, s32 key) {
    void *node = sdfFindTaskListNodeByKey(work->list, key);
    if (node != NULL) {
        return sdfListRemoveNode(work->list, node);
    }
}

s32 sdfFindTaskItemValueByKey(TaskWork *work, s32 key) {
    SdfListNode *item;

    item = sdfFindTaskListNodeByKey(work->list, key);
    if (item != NULL) {
        return (s32)item->value;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00310BC8", func_00312810);
