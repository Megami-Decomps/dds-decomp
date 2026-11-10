#include "common.h"
#include "draw3d.h"
#include "ps2asm.h"
#include "sdfFrame.h"

/* DDS's names for the same objects. */
#define dds3EffectBase effDefaultRandomState
#define func_00168A50 polyBandLayoutRing
#define sdfMatSetTex sdfSetAssetPrimaryTextureAddress

typedef struct
{
    u128 *pVertex;
    u32 *pVC;
    s32 count;
    s32 rec;
    u32 color;
} dds3TrackPolyList_t;

typedef struct dds3TrackPoly_t
{
    u16 type;
    u16 mode;
    s32 number;
    s32 point;
    s32 rec;
    sdfMemHandle_t *hMem;
    dds3TrackPolyList_t *trackList;
    u128 *pVertex;
    u32 *pVC;
    sdfMat_t *pMat;
    struct dds3TrackPoly_t *next;
    s32 d_next;
} dds3TrackPoly_t;


/* Polygon owners follow the variable-sized attribute arrays in one allocation. */
typedef struct dds3Polygon_t
{
    s16 flags;
    u8 unk02[2];
    s32 capacity;
    s32 strip_count;
    s32 drawCount;
    sdfMemHandle_t *hMem;
    sdf3DPrimPolyParm_t parm;
    sdfMat_t *pMat;
    u128 *pVertex;
    u32 (*texcoords)[2];
    u128 *pNormal;
    u32 *pVertexColor;
    struct dds3Polygon_t *next;
    s32 d_next;
} dds3Polygon_t;

typedef struct
{
    s32 number;
    u32 abgr;
    u128 *pVertex;
    u32 *pVC;
    sdfMat_t *pMat;
    sdfMemHandle_t *hMem;
} dds3TrianglePoly_t;

enum trackTopology
{
    TRACK_TOPOLOGY_PAIR,
    TRACK_TOPOLOGY_TRIANGLE,
    TRACK_TOPOLOGY_SIX_VECTOR,
    TRACK_TOPOLOGY_FOUR_VECTOR,
    TRACK_TOPOLOGY_FIVE_VECTOR
};

/* Packed color words written in the vertex order used by each cell geometry. */
typedef struct trackTriangleColors_t {
    s32 edge0;  /* 0x00 */
    s32 middle; /* 0x04 */
    s32 edge1;  /* 0x08 */
} trackTriangleColors_t;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);
extern s32 sdfMemLock(sdfMemHandle_t *aHandle);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern sdfMat_t *sdfMatNew(void);
extern void sdfMatFreeDelayed(sdfMat_t *aMat);
extern void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);
extern void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);
extern s32 sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);
extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, s32 aAllocator);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, s32 aAllocator);
extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);
extern f32 dds3TrackPolyVCType1Param[4];
extern f32 dds3TrackPolyVCType2Param[4];
extern f32 dds3TrackPolyVCSetAcuteParam[4];
extern f32 dds3TrackPolyVCSetAcuteWParam[4];
extern f32 dds3TrackPolyVCAcuteParam[4];
extern void (*VCSetFunc[])(void *, void *, void *);
extern u32 dds3TrackPolyPairFaceIndex[];
extern u32 dds3TrackPolyTriangleFaceIndex[];
extern u32 dds3TrackPolyFiveVectorFaceIndex[];
extern u32 dds3TrackPolySixVectorFaceIndex[];
extern u32 dds3TrackPolyDefaultFaceIndex[];
extern sdfGrOt_t *dds3TrackPolyOtTable[];
extern sdfGrOt_t sdfGrOtTable[85];

void dds3TrackPolyListInit(dds3TrackPoly_t *hTrackPoly, s32 number);
extern dds3TrackPoly_t *dds3TrackPolyTag;

typedef struct dds3SpPoly_Basic_t {
    f32 origin[4];
    u32 number; /* 0x10: number of trailing per-entry records. */
    s32 frame;
    u32 templateSize; /* Byte length copied from the source template. */
    u8 pad1C[4];
    f32 matrix[16];
    u32 abgr;
    u8 loop;
    u8 pad65;
    u16 mode; /* 0x66: selected particle draw bucket. */
    u32 alpha_s;
    u32 alpha_e;
    f32 matrixCopy[16]; /* Initialized from matrix when the owner is created. */
    u16 type;
    u16 state; /* Cleared when every entry finishes. */
    u8 padB4[0xC];
} dds3SpPoly_Basic_t;


typedef struct dds3SpPoly_RingPhysics_t {
    s32 frame;
    f32 spread_s;
} dds3SpPoly_RingPhysics_t;

typedef struct dds3SpPoly_SphereRingPhysics_t {
    s32 frame;
    u32 radius; /* Native initializer converts a float to u32. */
} dds3SpPoly_SphereRingPhysics_t;

typedef struct dds3SpPoly_RollRingPhysics_t {
    s32 frame;
    f32 radius;
    f32 spread_s;
    f32 rot_x;
    f32 rot_y;
} dds3SpPoly_RollRingPhysics_t;


typedef struct dds3SpPoly_Ring_Old_t3 {
    f32 origin[4];
    u16 entryCount;
    u8 pad12[2];
    s32 duration;
    s32 headerSize;
    u8 pad1C[4];
    f32 matrix[16];
    u32 color;
    u8 templateLoop;
    u8 pad65;
    u16 drawBucket;
    u32 startColorRampFrames;
    u32 endColorRampFrames;
    f32 matrixCopy[16];
    u16 handler;
    u16 active;
    u8 padB4[0xC];
    u8 loop;
    u8 padC1[3];
    s32 spawnDelayStep;
    u16 segments;
    u8 padCA[2];
    f32 radius;
    f32 pairDisplacement;
    u8 padD4[8];
    dds3TrackPoly_t *hTrackPoly;
    s32 *ages;
} dds3SpPoly_Ring_Old_t3;


typedef struct dds3SpPoly_Ring_t3 {
    dds3SpPoly_Basic_t head;
    u32 spawnDelayStep;
    u32 vertex;
    f32 radialWidth;
    f32 initialRadius;
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    f32 liftStep;
    u8 padE0[4];
    void *dispatchArgB;             /* +0xE4 */
    u8 padE8[4];
    void *dispatchArgA;             /* +0xEC */
    struct dds3TrackPoly_t *hTrackPoly;         /* +0xF0 */
    dds3SpPoly_RingPhysics_t *pPhysics;         /* +0xF4 */
    struct sdfMemHandle_t *hMem;  /* +0xF8 */
    u8 padFC[4];
} dds3SpPoly_Ring_t3;


typedef struct dds3SpPolyOldCellParam_t {
    u8 pad00[8];
    u16 verticesPerCell; /* 0x08 */
    u8 pad0A[0xA];
    void *dispatchArgA;
    void *dispatchArgB;          /* 0x14: forwarded as two opaque callback arguments */
    struct dds3TrackPoly_t *cellSystem; /* 0x1C: allocated cell system */
    u32 *cellWords;       /* 0x20: inline per-cell word array */
    u8 pad24[0xC];
} dds3SpPolyOldCellParam_t;

/* Fixed 0xF0 descriptor prefix; variable per-cell words begin at +0xF0. */
typedef struct dds3SpPolyOldDesc_t {
    u8 pad00[0x10];
    s32 count;            /* 0x10 */
    u8 pad14[4];
    s32 headerSize;       /* 0x18: serialized-header byte count */
    u8 pad1C[0xA4];
    dds3SpPolyOldCellParam_t cells; /* 0xC0 */
} dds3SpPolyOldDesc_t;


enum {
    POLY_INACTIVE_ENTRY_AGE = -0xFFFFFF,
    POLY_RESET_ENTRY_AGE = 0xFFFFFF0,
    POLY_RESET_DURATION = 0xFFFFFFF
};

typedef struct {
    dds3SpPoly_Basic_t head;
    u32 spawnDelayStep;
    u32 vertex;
    f32 radialWidth;
    f32 radius;
    f32 radius_r;
    void *dispatchArgB;            /* 0xD4 */
    u8 padD8[4];
    void *dispatchArgA;            /* 0xDC */
    dds3TrackPoly_t *hTrackPoly;              /* 0xE0 */
    dds3SpPoly_SphereRingPhysics_t *pPhysics;        /* 0xE4 */
    struct sdfMemHandle_t *hMem; /* 0xE8 */
    u8 padEC[4];
} dds3SpPoly_SphereRing_t3; /* 0xF0, followed by eight-byte pPhysics */

typedef struct {
    dds3SpPoly_Basic_t head;
    u32 spawnDelayStep;
    u32 vertex;
    f32 width;
    f32 radius_s;
    f32 radius_e;
    f32 radius_s_r;
    f32 radius_e_r;
    u32 simu_num;
    f32 incline;
    f32 roll_s;
    void *color_edge_in;            /* 0xE8 */
    u8 color[4];
    void *color_edge_out;            /* 0xF0 */
    dds3TrackPoly_t *hTrackPoly;              /* 0xF4 */
    dds3SpPoly_RollRingPhysics_t *pPhysics; /* 0xF8 */
    struct sdfMemHandle_t *allocation; /* 0xFC */
} dds3SpPoly_RollRing_t3; /* 0x100, followed by 20-byte pPhysics */

/* The ring kinds share the template prefix and their leading parameter words;
 * the creation paths clamp the segment count through this shared view. */
typedef union {
    dds3SpPoly_Ring_t3 band;
    dds3SpPoly_SphereRing_t3 arc;
    dds3SpPoly_RollRing_t3 rotating;
} dds3SpPolyRingUnion_t;


typedef struct
{
    void *(*func)(void *params);
    void (*callback)(void *work);
    void (*destroy)(void *work);
} dds3SpPolyEntry_t;

typedef struct
{
    u8 unk00[0x64];
    u8 loop;
    u8 unk65[0x4B];
    u16 handler;
    u16 active;
    u8 unkB4[0xC];
    u8 oldLoop;
} dds3SpPolyLoopWork_t;

extern void *memcpy(void *a, const void *b, size_t c);
extern void *memset(void *a, s32 b, size_t c);
extern dds3SpPolyEntry_t dds3SpPolyTable[];
extern void (*dds3SpPolyResetTable[])(void *work);
extern void (*dds3SpPolySetScaleTable[])(void *work, f32 scale);
extern f32 dds3SpPolyColorTo[4];
extern f32 dds3SpPolyColorFrom[4];
extern f32 dds3PolygonTriangleParam[4];
extern u8 dds3EffectBase[];
extern f32 dds3GetRandFloatA(void *name);
extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern f32 sdfCos(f32 aRadius);
extern f32 sdfSin(f32 aRadius);
extern void SetRotateMatrixX(f32 aAngle);
extern void SetRotateMatrixY_B(f32 aAngle);
extern void MultMatrixRevNoArg(void);
extern void MultMatrixNoArg(void);
extern void dds3SpPoly_RingBuffInit(dds3SpPoly_Ring_t3 *pRing, s32 number);
/* Separate geometry builder reached by the initializer's native tail call. */
extern void func_00168A50(dds3SpPoly_Ring_t3 *ring, s32 number, f32 radius);

dds3SpPolyOldDesc_t *dds3SpPoly_Ring_Old_Create(dds3SpPolyOldDesc_t *src);
void dds3SpPoly_Ring_Old_Delete(dds3SpPoly_Ring_Old_t3 *hData);
void dds3SpPoly_Ring_Old_BuffInit(dds3SpPoly_Ring_Old_t3 *hData);
void dds3SpPoly_Ring_Old_SetScale(dds3SpPoly_Ring_Old_t3 *hData, f32 scale);
void dds3SpPoly_Ring_Old_Calc(dds3SpPoly_Ring_Old_t3 *pRing);
void dds3SpPoly_Ring_Old_Draw(dds3SpPoly_Ring_Old_t3 *hData);
void dds3SpPoly_Ring_Old_New(dds3SpPoly_Ring_Old_t3 *pRing, s32 number);
void dds3SpPoly_Ring_Old_Move(dds3SpPoly_Ring_Old_t3 *pRing, s32 number);
u32 dds3SpPoly_Color(u32 frame, u32 frame_end, u32 abgr);
dds3SpPoly_Ring_t3 *dds3SpPoly_RingCreate(dds3SpPoly_Basic_t *templateHead);
void dds3SpPoly_RingDelete(dds3SpPoly_Ring_t3 *hData);
void dds3SpPoly_RingAllBuffInit(dds3SpPoly_Ring_t3 *band);
void dds3SpPoly_RingMove(dds3SpPoly_Ring_t3 *pRing, s32 number);
void dds3SpPoly_RingSetScale(dds3SpPoly_Ring_t3 *hData, f32 scale);
void dds3SpPoly_RingDraw(dds3SpPoly_Ring_t3 *hData);
dds3SpPoly_SphereRing_t3 *dds3SpPoly_SphereRingCreate(dds3SpPoly_SphereRing_t3 *source);
void dds3SpPoly_SphereRingDelete(dds3SpPoly_SphereRing_t3 *hData);
void dds3SpPoly_SphereRingAllBuffInit(dds3SpPoly_SphereRing_t3 *arc);
void dds3SpPoly_SphereRingBuffInit(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number);
void dds3SpPoly_SphereRingMove(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number);
void dds3SpPoly_SphereRingSetScale(dds3SpPoly_SphereRing_t3 *hData, f32 scale);
void dds3SpPoly_SphereRingDraw(dds3SpPoly_SphereRing_t3 *hData);
dds3SpPoly_RollRing_t3 *dds3SpPoly_RollRingCreate(dds3SpPoly_Basic_t *templateHead);
void dds3SpPoly_RollRingDelete(dds3SpPoly_RollRing_t3 *hData);
void dds3SpPoly_RollRingAllBuffInit(dds3SpPoly_RollRing_t3 *rotatingBand);
void dds3SpPoly_RollRingBuffInit(dds3SpPoly_RollRing_t3 *pRollRing, u32 number);
void dds3SpPoly_RollRingMove(dds3SpPoly_RollRing_t3 *pRollRing, s32 number);
void dds3SpPoly_RollRingSetScale(dds3SpPoly_RollRing_t3 *hData, f32 scale);
void dds3SpPoly_RollRingSetEndless(dds3SpPoly_RollRing_t3 *pRollRing);
void *dds3SpPolyCreate(s32 index, void *arg);
void *dds3SpPolyCopy(dds3SpPoly_Basic_t *effect);
void dds3SpPolyDelete(dds3SpPoly_Basic_t *hData);
void dds3SpPolyDraw(dds3SpPoly_Basic_t *hData);
void dds3SpPolyReset(dds3SpPoly_Basic_t *hData);
void dds3SpPolySetScale(dds3SpPoly_Basic_t *hData, f32 scale);
u16 dds3SpPolyGetState(dds3SpPoly_Basic_t *hData);
void dds3SpPolySetPosition(void *hData, void *pPos);
void dds3SpPolySetRotateMatrix(dds3SpPoly_Basic_t *hData, void *pMatrix);
void dds3SpPolySetRGBA(dds3SpPoly_Basic_t *hData, u32 abgr);
void dds3SpPolySetLoop(dds3SpPolyLoopWork_t *hData, u8 loop);
u8 dds3SpPolyGetLoop(dds3SpPolyLoopWork_t *hData);
void dds3PolygonSetColor(dds3Polygon_t *hPolygon, u32 color);
void dds3PolygonTriangleSet(dds3Polygon_t *hPolygon, const f32 *center);
void dds3TrackPolyColorSet(dds3TrackPoly_t *hTrackPoly, s32 number, u32 color);
extern dds3Polygon_t *hDrawListTop;
extern u8 dds3PolygonRandState[];
extern sdf3DPrimPolyParm_t dds3PolygonPrimParm;

extern void sdfRandomizeByTime(void *aCB);

void dds3TrackPolyInit(void);

void dds3PolygonManagerInit(void)
{
    hDrawListTop = 0;
    dds3TrackPolyInit();
    sdfRandomizeByTime(dds3PolygonRandState);
}

void dds3PolygonManagerExit(void) {
}

dds3Polygon_t *dds3PolygonCreate(s32 face, s16 type, s16 attr) {
    s32 vertexCount;
    s32 bytes;
    sdfMemHandle_t *allocation;
    u8 *buffer;
    dds3Polygon_t *record;
    sdfMat_t *asset;

    if (type & 8) vertexCount = face + 2;
    else vertexCount = face * 3;
    bytes = vertexCount * sizeof(u128);
    if (type & 1) bytes += vertexCount * sizeof(record->texcoords[0]);
    if (type & 2) bytes += vertexCount * sizeof(u128);
    if (type & 4) bytes += vertexCount * sizeof(u32);
    allocation = sdfMemAllocDirect(bytes + sizeof(*record));
    buffer = EE_ADDR_AS(u8 *, sdfMemLock(allocation));
    record = (dds3Polygon_t *)(buffer + bytes);
    memset(record, 0, sizeof(*record));
    record->pVertex = (u128 *)buffer;
    /* Allocation and layout reuse one byte extent; DDS1 cc1probe:
     * sandbox_ClusterX/ctor_reentry_F4_probe/decisions.log:16561-16660.
     * The saved span ties allocation at 1250, below vertex count at 3500;
     * a separate span instead has priority 3846 and takes the count's home. */
    bytes = vertexCount * sizeof(u128);
    buffer += bytes;
    if (type & 1) {
        record->texcoords = (u32 (*)[2])buffer;
        buffer += vertexCount * sizeof(record->texcoords[0]);
    }
    if (type & 2) record->pNormal = (u128 *)buffer;
    if (type & 4) record->pVertexColor = (u32 *)buffer;
    record->flags = type;
    record->capacity = vertexCount;
    record->hMem = allocation;
    record->drawCount = 0;
    memset(&record->parm, 0, sizeof(record->parm));
    record->parm.color = 0x80808080;
    if (!(attr & 1)) record->parm.attr = 0x4000;
    asset = sdfMatNew();
    record->pMat = asset;
    if (!(attr & 2)) sdfMatSetConstant(asset, 1.0f);
    return record;
}

void dds3PolygonDelete(dds3Polygon_t *hPolygon) {
    hPolygon->flags = 1;
    sdfMatFreeDelayed(hPolygon->pMat);
    sdfMemFreeDirect(hPolygon->hMem);
}

void dds3PolygonDraw(dds3Polygon_t *hPolygon) {
    hPolygon->next = hDrawListTop;
    hDrawListTop = hPolygon;
}

extern sdfGrOt_t sdfGrOtTable[85];
SYMBOL_AT(sdfGrOt_t, D_002F64B8, sdfGrOtTable, 0x700);

static inline void parAppendDrawSurfacePool(sdfGrOt_t *pool, sdfDrawTag_t *packet) {
    pool->regFunc(pool, packet);
}

void dds3PolygonLinkOt(void) {
    dds3Polygon_t *record = hDrawListTop;
    sdfDrawTag_t *packet;
    s32 remaining;
    s16 flags;
    sdfGrOt_t *priorityPool;

    if (record != NULL) {
        priorityPool = &sdfGrOtTable[51];
        do {
            if (record->drawCount != 0 && record->flags != 1) {
                packet = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
                sdfDrawTagInit(packet);
                sdf3DPrimLinkResetTm(packet, 0);
                sdf3DPrimLinkMat(packet, record->pMat, 0);
                flags = record->flags;
                remaining = record->drawCount;
                record->parm.vertex = (sdfVector4_t *)record->pVertex;
                record->parm.uv = (sdfVector2_t *)record->texcoords;
                record->parm.vertexColor = record->pVertexColor;
                record->parm.normal = (sdfVector4_t *)record->pNormal;
                record->parm.faceCount = 16;
                record->parm.pointCount = 48;
                while (remaining >= 48) {
                    sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(&record->parm)));
                    record->parm.vertex += 48;
                    if (flags & 1) record->parm.uv += 48;
                    if (flags & 2) record->parm.normal += 48;
                    remaining -= 48;
                    if (flags & 4) record->parm.vertexColor += 48;
                }
                if (remaining > 0) {
                    record->parm.faceCount = remaining / 3;
                    record->parm.pointCount = remaining;
                    sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(&record->parm)));
                }
                record->drawCount = 0;
                if (flags & 0x10) parAppendDrawSurfacePool(priorityPool, packet);
                else if (flags & 0x20) parAppendDrawSurfacePool(&sdfGrOtTable[55], packet);
                else parAppendDrawSurfacePool(&sdfGrOtTable[39], packet);
            }
            record = record->next;
        } while (record != NULL);
    }
    hDrawListTop = NULL;
}

void dds3PolygonSetTex(dds3Polygon_t *hPolygon, sdfTexHandle_t *hTex) {
    sdfMatSetTex(hPolygon->pMat, hTex);
}

void dds3TrackPolyInit(void)
{
    memset(&dds3PolygonPrimParm, 0, sizeof(dds3PolygonPrimParm));
    dds3PolygonPrimParm.attr = 0x4000;
}

dds3TrackPoly_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type) {
    s32 total;
    sdfMemHandle_t *allocation;
    s32 base;
    s32 cellsSize;
    dds3TrackPoly_t *system;
    s32 i;

    if (type == TRACK_TOPOLOGY_FIVE_VECTOR) {
        face = face * 5 + 5;
    } else if (type == TRACK_TOPOLOGY_FOUR_VECTOR) {
        face = face * 4 + 4;
    } else if (type == TRACK_TOPOLOGY_SIX_VECTOR) {
        face = face * 6 + 6;
    } else {
        face = face * 2;
        if (rec != 0) {
            if (face % rec != 0) {
                face = face / rec + 2;
            } else {
                face = face / rec;
            }
        }
        face &= ~1;
        face += 2;
        if (type == TRACK_TOPOLOGY_TRIANGLE) {
            face += face >> 1;
        }
    }
    total = number * face;
    cellsSize = (total + number) * 0x14;
    allocation = sdfMemAllocDirect(cellsSize + 0x2C);
    base = sdfMemLock(allocation);
    system = EE_ADDR_AS(dds3TrackPoly_t *, (base + cellsSize));
    memset(system, 0, 0x2C);
    system->pVertex = EE_ADDR_AS(u128 *, base);
    base += total * 0x10;
    system->pVC = EE_ADDR_AS(u32 *, base);
    base += total * 4;
    system->trackList = EE_ADDR_AS(dds3TrackPolyList_t *, base);
    for (i = 0; i < number; i++) {
        dds3TrackPolyList_t *cell = (dds3TrackPolyList_t *)(i * sizeof(dds3TrackPolyList_t) + EE_ADDR_S(system->trackList));
        cell->pVertex = (u128 *)((u8 *)system->pVertex + i * face * 0x10);
        cell->pVC = system->pVC + i * face;
        dds3TrackPolyListInit(system, i);
    }
    system->pMat = sdfMatNew();
    sdfMatSetConstant(system->pMat, 1.0f);
    system->type = type;
    system->number = number;
    system->mode = 2;
    system->point = face;
    system->rec = rec;
    system->hMem = allocation;
    system->next = 0;
    system->d_next = 0;
    return system;
}

void dds3TrackPolyDelete(dds3TrackPoly_t *hTrackPoly) {
    sdfMatFreeDelayed(hTrackPoly->pMat);
    sdfMemFreeDirect(hTrackPoly->hMem);
}

void dds3TrackPolyListInit(dds3TrackPoly_t *hTrackPoly, s32 number)
{
    dds3TrackPolyList_t *list = (dds3TrackPolyList_t *)(number * 20 + EE_ADDR(hTrackPoly->trackList));

    list->color = 0x80808080;
    list->rec = 0;
    list->count = 0;
}

void dds3TrackPolyDraw(dds3TrackPoly_t *node) {
    node->next = dds3TrackPolyTag;
    dds3TrackPolyTag = node;
}

void dds3TrackPolySet(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pV) {
    dds3TrackPolyList_t *cell = &hTrackPoly->trackList[number];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->rec == 0) {
        shiftCount = hTrackPoly->point - 2;
        vertex = cell->pVertex + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            SDF_VU0_COPYVECTOR(vertex + 2, vertex);
        }
        cell->rec = SDF_FRAMES(hTrackPoly->rec + 1) - 1;
        if (cell->count < shiftCount + 2) {
            cell->count += 2;
        }
    } else {
        cell->rec--;
        vertex = cell->pVertex;
    }
    SDF_VU0_COPYVECTOR(vertex, pV);
    SDF_VU0_COPYVECTOR(vertex + 1, pV + 1);
}

void dds3TrackPolyMove(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pMove) {
    dds3TrackPolyList_t *cell = hTrackPoly->trackList + number;
    s32 count = hTrackPoly->point;
    u128 *vertex = cell->pVertex;
    s32 i;
    SDF_VU0_LOADVECTOR_B_BARRIER(pMove);
    if (count > 0) {
        i = count;
        do {
            SDF_VU0_LOADVECTOR(vertex);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex);
            i--;
            vertex++;
        } while (i != 0);
    }
}

void dds3TrackPolySet2(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pV) {
    dds3TrackPolyList_t *cell = &hTrackPoly->trackList[number];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->rec == 0) {
        shiftCount = hTrackPoly->point - 3;
        vertex = cell->pVertex + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            SDF_VU0_COPYVECTOR(vertex + 3, vertex);
        }
        cell->rec = SDF_FRAMES(hTrackPoly->rec + 1) - 1;
        if (cell->count < shiftCount + 3) {
            cell->count += 3;
        }
    } else {
        cell->rec--;
        vertex = cell->pVertex;
    }
    SDF_VU0_COPYVECTOR(vertex, pV);
    SDF_VU0_COPYVECTOR(vertex + 1, pV + 1);
    SDF_VU0_COPYVECTOR(vertex + 2, pV + 2);
}

void dds3TrackPolyMove2(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pMove) {
    dds3TrackPolyList_t *cell = hTrackPoly->trackList + number;
    s32 count = cell->count / 3;
    u128 *vertex = cell->pVertex;
    s32 i;
    SDF_VU0_LOADVECTOR_B(pMove);
    if (count > 0) {
        i = count;
        do {
            SDF_VU0_LOADVECTOR(vertex);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex);
            SDF_VU0_LOADVECTOR(vertex + 1);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex + 1);
            SDF_VU0_LOADVECTOR(vertex + 2);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex + 2);
            i--;
            vertex += 3;
        } while (i != 0);
    }
}

void dds3TrackPolyVCRefresh_ParType1(dds3TrackPoly_t *hTrackPoly, s32 index);

void dds3TrackPolyVCRefresh_ParType1(dds3TrackPoly_t *hTrackPoly, s32 index) {
    u32 startCarrier[4];
    u32 cellCarrier[4];
    u32 packed[4];
    dds3TrackPolyList_t *cell = (dds3TrackPolyList_t *)((u8 *)(index * sizeof(dds3TrackPolyList_t)) + EE_ADDR(hTrackPoly->trackList));
    u32 *vc = cell->pVC;
    u32 perCell = cell->count >> 1;
    u32 color = vc[0];
    u32 j;

    if (perCell < 2) {
        return;
    }
    startCarrier[0] = color & 0xFF000000;
    MMI_RGBA_UNPACK(startCarrier, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(dds3TrackPolyVCType1Param);
    VU0_LERP_VF10_VF11(1.0f / (f32)perCell);
    SDF_VU0_COPYVECTOR_A_TO_C();
    cellCarrier[0] = color;
    MMI_RGBA_UNPACK(cellCarrier, 1.0f / 128.0f);
    for (j = 0; j < perCell; j++) {
        u32 packedColor;

        SDF_VU0_COPYVECTOR_A_TO_B();
        MMI_RGBA_PACK_UNIT(packedColor, 0x43000000);
        packed[0] = packedColor;
        vc[0] = packedColor;
        vc[1] = packedColor;
        SDF_VU0_SUBVECTOR_B_C();
        SDF_VU0_COPYVECTOR_B_TO_A();
        vc += 2;
    }
}


void dds3TrackPolyVCRefresh_ParType2(dds3TrackPoly_t *hTrackPoly, s32 index);

void dds3TrackPolyVCRefresh_ParType2(dds3TrackPoly_t *hTrackPoly, s32 index) {
    dds3TrackPolyList_t *cell = (dds3TrackPolyList_t *)((u8 *)(index * sizeof(dds3TrackPolyList_t)) + EE_ADDR(hTrackPoly->trackList));
    u32 count = cell->count / 3;
    trackTriangleColors_t *vertex;
    u32 middleWord;
    u32 edgeWord;
    sdfVector4_t cur0;
    sdfVector4_t cur1;
    sdfVector4_t step0;
    sdfVector4_t step1;
    s32 alpha0[4];
    s32 alpha1[4];
    s32 col0[4];
    s32 col1[4];
    s32 out0[4];
    s32 out1[4];
    u32 j;
    u32 packedMiddle;
    u32 packedEdge;

    if (count >= 2) {
        vertex = (trackTriangleColors_t *)cell->pVC;
        middleWord = vertex->middle;
        edgeWord = vertex->edge0;
        alpha0[0] = middleWord & 0xFF000000;
        MMI_RGBA_UNPACK(alpha0, 1.0f / 128.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(dds3TrackPolyVCType2Param);
        SDF_VU0_INTERPOLATE(1.0f / count);
        SDF_VU0_STOREVECTOR(&step0);
        alpha1[0] = edgeWord & 0xFF000000;
        MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(dds3TrackPolyVCType2Param);
        SDF_VU0_INTERPOLATE(1.0f / count);
        SDF_VU0_STOREVECTOR_NOMEM(&step1);
        col0[0] = middleWord;
        MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
        SDF_VU0_STOREVECTOR_NOMEM(&cur0);
        col1[0] = edgeWord;
        MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
        SDF_VU0_STOREVECTOR_NOMEM(&cur1);
        for (j = 0; j < count; j++) {
            SDF_VU0_LOADVECTOR(&cur0);
            SDF_VU0_COPYVECTOR_A_TO_B();
            /* Read each packed output once, then reuse its color word for the vertex fields. */
            MMI_RGBA_PACK_UNIT(out0[0], 128.0f);
            packedMiddle = out0[0];
            vertex->middle = packedMiddle;
            SDF_VU0_LOADVECTOR_C(&step0);
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(&cur0);
            SDF_VU0_LOADVECTOR(&cur1);
            SDF_VU0_COPYVECTOR_A_TO_B();
            MMI_RGBA_PACK_UNIT(out1[0], 128.0f);
            packedEdge = out1[0];
            vertex->edge0 = packedEdge;
            vertex->edge1 = packedEdge;
            SDF_VU0_LOADVECTOR_C(&step1);
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(&cur1);
            vertex++;
        }
    }
}

void dds3TrackPolyVCSet_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out)
{
    s32 perCell = hTrackPoly->point >> 1;
    s32 count = hTrackPoly->number;
    s32 i;

    for (i = 0; i < count; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[0] = abgr_in;
            vc[1] = abgr_out;
            vc += 2;
        }
    }
}

void dds3TrackPolyVCSet_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    u32 startCarrier[4];
    u32 cellCarrier[4];
    u32 packed[4];
    u32 perCell = hTrackPoly->point >> 1;
    u32 cellCount = hTrackPoly->number;
    s32 i;

    startCarrier[0] = abgr_in & 0xFF000000;
    MMI_RGBA_UNPACK(startCarrier, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(dds3TrackPolyVCSetAcuteParam);
    VU0_LERP_VF10_VF11(1.0f / (f32)perCell);
    SDF_VU0_COPYVECTOR_A_TO_C();
    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        s32 j;

        cellCarrier[0] = abgr_in;
        MMI_RGBA_UNPACK_NOMEM(cellCarrier, 0x3C000000);
        for (j = 0; j < perCell; j++) {
            u32 color;

            SDF_VU0_COPYVECTOR_A_TO_B();
            MMI_RGBA_PACK_UNIT(color, 0x43000000);
            packed[0] = color;
            vc[0] = color;
            vc[1] = color;
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_COPYVECTOR_B_TO_A();
            vc += 2;
        }
    }
}


void dds3TrackPolyVCSet_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 color) {
    u32 words = hTrackPoly->point;
    u32 count = hTrackPoly->number;
    u32 quarter = words >> 2;
    u32 i;
    u32 j;
    u32 **cell;
    u32 *vertex;
    s32 alpha[4];
    s32 start[4];
    s32 rise[4];
    s32 fall[4];
    u32 base;
    u32 odd;
    u32 packed;

    alpha[0] = color & 0xFF000000;
    MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(dds3TrackPolyVCSetAcuteWParam);
    SDF_VU0_INTERPOLATE(1.0f / quarter);
    SDF_VU0_COPYVECTOR_A_TO_C();
    i = 0;
    if (count != 0) {
        base = color & 0x00FFFFFF;
        odd = (words >> 1) & 1;
        cell = (u32 **)&hTrackPoly->trackList[0].pVC;
        do {
            vertex = *cell;
            start[0] = base;
            MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            /* Keep each packed ramp value in its output lane before copying it to both vertex words. */
            for (j = 0; j < quarter; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                MMI_RGBA_PACK_UNIT(packed, 128.0f);
                rise[0] = packed;
                vertex[0] = rise[0];
                vertex[1] = vertex[0];
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            if (odd) {
                vertex[0] = color;
                vertex[1] = color;
                vertex += 2;
            }
            for (j = 0; j < quarter; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                MMI_RGBA_PACK_UNIT(packed, 128.0f);
                fall[0] = packed;
                vertex[0] = fall[0];
                vertex[1] = vertex[0];
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            i++;
            cell = (u32 **)((u8 *)cell + sizeof(dds3TrackPolyList_t));
        } while (i < count);
    }
}

void dds3TrackPolyVCSet2_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = hTrackPoly->point / 3;
    s32 i;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[1] = abgr_in;
            vc[2] = abgr_out;
            vc[0] = abgr_out;
            vc += 3;
        }
    }
}


void dds3TrackPolyVCSet2_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    sdfVector4_t cur0;
    sdfVector4_t cur1;
    sdfVector4_t step0;
    sdfVector4_t step1;
    s32 alpha0[4];
    s32 alpha1[4];
    s32 col0[4];
    s32 col1[4];
    s32 out0[4];
    s32 out1[4];
    s32 groups = 3;
    u32 perCell = hTrackPoly->point / groups;
    u32 count = hTrackPoly->number;
    u32 i;
    u32 j;
    u32 **cell;
    trackTriangleColors_t *vertex;
    u32 packedMiddle;
    u32 packedEdge;

    alpha0[0] = abgr_in & 0xFF000000;
    MMI_RGBA_UNPACK(alpha0, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(dds3TrackPolyVCAcuteParam);
    SDF_VU0_INTERPOLATE(1.0f / perCell);
    SDF_VU0_STOREVECTOR(&step0);
    alpha1[0] = abgr_out & 0xFF000000;
    MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(dds3TrackPolyVCAcuteParam);
    SDF_VU0_INTERPOLATE(1.0f / perCell);
    SDF_VU0_STOREVECTOR(&step1);
    i = 0;
    if (count != 0) {
        cell = &hTrackPoly->trackList[0].pVC;
        do {
            vertex = (trackTriangleColors_t *)*cell;
            col0[0] = abgr_in;
            MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
            SDF_VU0_STOREVECTOR(&cur0);
            col1[0] = abgr_out;
            MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
            SDF_VU0_STOREVECTOR(&cur1);
            for (j = 0; j < perCell; j++) {
                SDF_VU0_LOADVECTOR(&cur0);
                SDF_VU0_COPYVECTOR_A_TO_B();
                /* These packed words feed every color field for their vertex. */
                MMI_RGBA_PACK_UNIT(out0[0], 0x43000000);
                packedMiddle = out0[0];
                vertex->middle = packedMiddle;
                SDF_VU0_LOADVECTOR_C(&step0);
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(&cur0);
                SDF_VU0_LOADVECTOR(&cur1);
                SDF_VU0_COPYVECTOR_A_TO_B();
                MMI_RGBA_PACK_UNIT(out1[0], 0x43000000);
                packedEdge = out1[0];
                vertex->edge0 = packedEdge;
                vertex->edge1 = packedEdge;
                SDF_VU0_LOADVECTOR_C(&step1);
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(&cur1);
                vertex++;
            }
            i++;
            cell = (u32 **)((u8 *)cell + sizeof(dds3TrackPolyList_t));
        } while (i < count);
    }
}

void dds3TrackPolyVCSet2_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = (hTrackPoly->point / 3) >> 1;
    u32 firstAlpha = abgr_in & 0xFF000000;
    u32 secondAlpha = abgr_out & 0xFF000000;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    s32 i;

    abgr_in &= 0x00FFFFFF;
    abgr_out &= 0x00FFFFFF;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 rampFirst = 0;
        u32 rampSecond = 0;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[1] = abgr_in | (rampFirst & 0xFF000000);
            vc[2] = abgr_out | (rampSecond & 0xFF000000);
            vc[0] = abgr_out | (rampSecond & 0xFF000000);
            rampFirst += firstStep;
            rampSecond += secondStep;
            vc += 3;
        }
        rampFirst = firstAlpha;
        rampSecond = secondAlpha;
        for (j = 0; j < perCell; j++) {
            rampFirst -= firstStep;
            rampSecond -= secondStep;
            vc[1] = abgr_in | (rampFirst & 0xFF000000);
            vc[2] = abgr_out | (rampSecond & 0xFF000000);
            vc[0] = abgr_out | (rampSecond & 0xFF000000);
            vc += 3;
        }
    }
}


void dds3TrackPolyVC3Set_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = hTrackPoly->point / 6;
    s32 i;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[0] = abgr_2;
            vc[1] = abgr_1;
            vc[2] = abgr_0;
            vc[3] = abgr_0;
            vc[4] = abgr_1;
            vc[5] = abgr_2;
            vc += 6;
        }
    }
}


void dds3TrackPolyVC3Set_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = hTrackPoly->point / 6;
    u32 firstAlpha = abgr_0 & 0xFF000000;
    u32 secondAlpha = abgr_1 & 0xFF000000;
    u32 thirdAlpha = abgr_2 & 0xFF000000;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    u32 thirdStep = thirdAlpha / perCell;
    s32 i;

    abgr_0 &= 0x00FFFFFF;
    abgr_1 &= 0x00FFFFFF;
    abgr_2 &= 0x00FFFFFF;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 rampFirst = firstAlpha;
        u32 rampSecond = secondAlpha;
        u32 rampThird = thirdAlpha;
        s32 j;

        for (j = 0; j < perCell; j++) {
            rampFirst -= firstStep;
            rampSecond -= secondStep;
            rampThird -= thirdStep;
            vc[0] = abgr_2 | (rampThird & 0xFF000000);
            vc[1] = abgr_1 | (rampSecond & 0xFF000000);
            vc[2] = abgr_0 | (rampFirst & 0xFF000000);
            vc[3] = abgr_0 | (rampFirst & 0xFF000000);
            vc[4] = abgr_1 | (rampSecond & 0xFF000000);
            vc[5] = abgr_2 | (rampThird & 0xFF000000);
            vc += 6;
        }
    }
}


void dds3TrackPolyVC3Set_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = (hTrackPoly->point / 6) >> 1;
    u32 firstAlpha = abgr_0 & 0xFF000000;
    u32 secondAlpha = abgr_1 & 0xFF000000;
    u32 thirdAlpha = abgr_2 & 0xFF000000;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    u32 thirdStep = thirdAlpha / perCell;
    s32 i;

    abgr_0 &= 0x00FFFFFF;
    abgr_1 &= 0x00FFFFFF;
    abgr_2 &= 0x00FFFFFF;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 rampFirst = 0;
        u32 rampSecond = 0;
        u32 rampThird = 0;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[0] = abgr_2 | (rampThird & 0xFF000000);
            vc[1] = abgr_1 | (rampSecond & 0xFF000000);
            vc[2] = abgr_0 | (rampFirst & 0xFF000000);
            vc[3] = abgr_0 | (rampFirst & 0xFF000000);
            vc[4] = abgr_1 | (rampSecond & 0xFF000000);
            vc[5] = abgr_2 | (rampThird & 0xFF000000);
            rampFirst += firstStep;
            rampSecond += secondStep;
            rampThird += thirdStep;
            vc += 6;
        }
        rampFirst = firstAlpha;
        rampSecond = secondAlpha;
        rampThird = thirdAlpha;
        for (j = 0; j < perCell; j++) {
            rampFirst -= firstStep;
            rampSecond -= secondStep;
            rampThird -= thirdStep;
            vc[0] = abgr_2 | (rampThird & 0xFF000000);
            vc[1] = abgr_1 | (rampSecond & 0xFF000000);
            vc[2] = abgr_0 | (rampFirst & 0xFF000000);
            vc[3] = abgr_0 | (rampFirst & 0xFF000000);
            vc[4] = abgr_1 | (rampSecond & 0xFF000000);
            vc[5] = abgr_2 | (rampThird & 0xFF000000);
            vc += 6;
        }
    }
}


void dds3TrackPolyVCSet4_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    s32 perCell = hTrackPoly->point >> 2;
    s32 cellCount = hTrackPoly->number;

    s32 cellIndex;

    for (cellIndex = 0; cellIndex < cellCount; cellIndex++) {
        u32 *vertices = hTrackPoly->trackList[cellIndex].pVC;
        s32 i;

        for (i = 0; i < perCell; i++) {
            vertices[2] = abgr_in;
            vertices[1] = abgr_in;
            vertices[3] = abgr_out;
            vertices[0] = abgr_out;
            vertices += 4;
        }
    }
}

void dds3TrackPolyVCSet4_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    s32 perCell = hTrackPoly->point >> 2;
    u32 firstAlpha = abgr_in & 0xFF000000;
    u32 secondAlpha = abgr_out & 0xFF000000;
    s32 cellCount = hTrackPoly->number;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    u32 firstRGB = abgr_in & 0x00FFFFFF;
    u32 secondRGB = abgr_out & 0x00FFFFFF;
    s32 i;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 alphaA = firstAlpha;
        u32 alphaB = secondAlpha;
        s32 j;

        for (j = 0; j < perCell; j++) {
            alphaA -= firstStep;
            alphaB -= secondStep;
            vc[2] = firstRGB | (alphaA & 0xFF000000);
            vc[1] = firstRGB | (alphaA & 0xFF000000);
            vc[3] = secondRGB | (alphaB & 0xFF000000);
            vc[0] = secondRGB | (alphaB & 0xFF000000);
            vc += 4;
        }
    }
}


void dds3TrackPolyVCSet4_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out) {
    s32 perCell = hTrackPoly->point >> 3;
    u32 firstAlpha = abgr_in & 0xFF000000;
    u32 secondAlpha = abgr_out & 0xFF000000;
    s32 cellCount = hTrackPoly->number;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    u32 firstRGB = abgr_in & 0x00FFFFFF;
    u32 secondRGB = abgr_out & 0x00FFFFFF;
    s32 i;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 alphaA = 0;
        u32 alphaB = 0;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[2] = firstRGB | (alphaA & 0xFF000000);
            vc[1] = firstRGB | (alphaA & 0xFF000000);
            vc[3] = secondRGB | (alphaB & 0xFF000000);
            vc[0] = secondRGB | (alphaB & 0xFF000000);
            alphaA += firstStep;
            alphaB += secondStep;
            vc += 4;
        }
        alphaA = firstAlpha;
        alphaB = secondAlpha;
        for (j = 0; j < perCell; j++) {
            alphaA -= firstStep;
            alphaB -= secondStep;
            vc[2] = firstRGB | (alphaA & 0xFF000000);
            vc[1] = firstRGB | (alphaA & 0xFF000000);
            vc[3] = secondRGB | (alphaB & 0xFF000000);
            vc[0] = secondRGB | (alphaB & 0xFF000000);
            vc += 4;
        }
    }
}


void dds3TrackPolyVC5Set_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = hTrackPoly->point / 5;
    s32 i;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[0] = abgr_2;
            vc[1] = abgr_1;
            vc[2] = abgr_0;
            vc[3] = abgr_1;
            vc[4] = abgr_2;
            vc += 5;
        }
    }
}


void dds3TrackPolyVC5Set_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = hTrackPoly->point / 5;
    u32 firstAlpha = abgr_0 & 0xFF000000;
    u32 secondAlpha = abgr_1 & 0xFF000000;
    u32 thirdAlpha = abgr_2 & 0xFF000000;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    u32 thirdStep = thirdAlpha / perCell;
    s32 i;

    abgr_0 &= 0x00FFFFFF;
    abgr_1 &= 0x00FFFFFF;
    abgr_2 &= 0x00FFFFFF;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 rampFirst = firstAlpha;
        u32 rampSecond = secondAlpha;
        u32 rampThird = thirdAlpha;
        s32 j;

        for (j = 0; j < perCell; j++) {
            rampFirst -= firstStep;
            rampSecond -= secondStep;
            rampThird -= thirdStep;
            vc[0] = abgr_2 | (rampThird & 0xFF000000);
            vc[1] = abgr_1 | (rampSecond & 0xFF000000);
            vc[2] = abgr_0 | (rampFirst & 0xFF000000);
            vc[3] = abgr_1 | (rampSecond & 0xFF000000);
            vc[4] = abgr_2 | (rampThird & 0xFF000000);
            vc += 5;
        }
    }
}


void dds3TrackPolyVC5Set_AcuteR(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = hTrackPoly->point / 5;
    u32 firstAlpha = abgr_0 & 0xFF000000;
    u32 secondAlpha = abgr_1 & 0xFF000000;
    u32 thirdAlpha = abgr_2 & 0xFF000000;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    u32 thirdStep = thirdAlpha / perCell;
    s32 i;

    abgr_0 &= 0x00FFFFFF;
    abgr_1 &= 0x00FFFFFF;
    abgr_2 &= 0x00FFFFFF;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 rampFirst = 0;
        u32 rampSecond = 0;
        u32 rampThird = 0;
        s32 j;

        for (j = 0; j < perCell; j++) {
            rampFirst += firstStep;
            rampSecond += secondStep;
            rampThird += thirdStep;
            vc[0] = abgr_2 | (rampThird & 0xFF000000);
            vc[1] = abgr_1 | (rampSecond & 0xFF000000);
            vc[2] = abgr_0 | (rampFirst & 0xFF000000);
            vc[3] = abgr_1 | (rampSecond & 0xFF000000);
            vc[4] = abgr_2 | (rampThird & 0xFF000000);
            vc += 5;
        }
    }
}


void dds3TrackPolyVC5Set_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2) {
    s32 cellCount = hTrackPoly->number;
    s32 perCell = (hTrackPoly->point / 5) >> 1;
    u32 firstAlpha = abgr_0 & 0xFF000000;
    u32 secondAlpha = abgr_1 & 0xFF000000;
    u32 thirdAlpha = abgr_2 & 0xFF000000;
    u32 firstStep = firstAlpha / perCell;
    u32 secondStep = secondAlpha / perCell;
    u32 thirdStep = thirdAlpha / perCell;
    s32 i;

    abgr_0 &= 0x00FFFFFF;
    abgr_1 &= 0x00FFFFFF;
    abgr_2 &= 0x00FFFFFF;

    for (i = 0; i < cellCount; i++) {
        u32 *vc = hTrackPoly->trackList[i].pVC;
        u32 rampFirst = 0;
        u32 rampSecond = 0;
        u32 rampThird = 0;
        s32 j;

        for (j = 0; j < perCell; j++) {
            vc[0] = abgr_2 | (rampThird & 0xFF000000);
            vc[1] = abgr_1 | (rampSecond & 0xFF000000);
            vc[2] = abgr_0 | (rampFirst & 0xFF000000);
            vc[3] = abgr_1 | (rampSecond & 0xFF000000);
            vc[4] = abgr_2 | (rampThird & 0xFF000000);
            rampFirst += firstStep;
            rampSecond += secondStep;
            rampThird += thirdStep;
            vc += 5;
        }
        rampFirst = firstAlpha;
        rampSecond = secondAlpha;
        rampThird = thirdAlpha;
        for (j = 0; j < perCell; j++) {
            rampFirst -= firstStep;
            rampSecond -= secondStep;
            rampThird -= thirdStep;
            vc[0] = abgr_2 | (rampThird & 0xFF000000);
            vc[1] = abgr_1 | (rampSecond & 0xFF000000);
            vc[2] = abgr_0 | (rampFirst & 0xFF000000);
            vc[3] = abgr_1 | (rampSecond & 0xFF000000);
            vc[4] = abgr_2 | (rampThird & 0xFF000000);
            vc += 5;
        }
    }
}


void dds3TrackPolySetMode(dds3TrackPoly_t *hTrackPoly, u16 mode) {
    hTrackPoly->mode = mode;
}

void dds3TrackPolyVCSet(dds3TrackPoly_t *hTrackPoly, s32 type, void *abgr_in, void *abgr_out) {
    u16 id = hTrackPoly->type;

    VCSetFunc[id * 3 + type](hTrackPoly, abgr_in, abgr_out);
}

void dds3TrackPolyLinkOt(void) {
    sdfDrawTag_t *lists[5];
    sdfDrawTag_t **slot;
    sdfDrawTag_t *list;
    sdfDrawTag_t *specialList;
    s32 count;
    s32 i;
    s32 remaining;
    dds3TrackPoly_t *system;
    dds3TrackPolyList_t *cell;
    u64 *packet;

    if (dds3TrackPolyTag == NULL) {
        return;
    }
    memset(lists, 0, sizeof(lists));
    for (system = dds3TrackPolyTag; system != NULL; system = system->next) {
        slot = &lists[system->mode];
        list = *slot;
        if (list == 0) {
            *slot = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
            sdfDrawTagInit(*slot);
            sdf3DPrimLinkResetTm(*slot, 0);
            list = *slot;
        }
        sdf3DPrimLinkMat(list, system->pMat, 0);
        count = system->number;
        if (system->type == TRACK_TOPOLOGY_PAIR) {
            dds3PolygonPrimParm.faceIndex = dds3TrackPolyPairFaceIndex;
            for (i = 0; i < count; i++) {
                cell = &system->trackList[i];
                dds3PolygonPrimParm.faceCount = 16;
                dds3PolygonPrimParm.pointCount = 18;
                remaining = cell->count;
                dds3PolygonPrimParm.vertex = (sdfVector4_t *)cell->pVertex;
                dds3PolygonPrimParm.vertexColor = cell->pVC;
                dds3PolygonPrimParm.color = cell->color;
                while (remaining >= 18) {
                    remaining -= 16;
                    sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&dds3PolygonPrimParm)));
                    dds3PolygonPrimParm.vertex += 16;
                    dds3PolygonPrimParm.vertexColor += 16;
                }
                if (remaining >= 4) {
                    dds3PolygonPrimParm.faceCount = remaining - 2;
                    dds3PolygonPrimParm.pointCount = remaining;
                    sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&dds3PolygonPrimParm)));
                }
            }
        } else if (system->type == TRACK_TOPOLOGY_TRIANGLE) {
            dds3PolygonPrimParm.faceIndex = dds3TrackPolyTriangleFaceIndex;
            for (i = 0; i < count; i++) {
                cell = &system->trackList[i];
                dds3PolygonPrimParm.faceCount = 16;
                dds3PolygonPrimParm.pointCount = 15;
                remaining = cell->count;
                dds3PolygonPrimParm.vertex = (sdfVector4_t *)cell->pVertex;
                dds3PolygonPrimParm.vertexColor = cell->pVC;
                dds3PolygonPrimParm.color = cell->color;
                while (remaining >= 15) {
                    remaining -= 12;
                    sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&dds3PolygonPrimParm)));
                    dds3PolygonPrimParm.vertex += 12;
                    dds3PolygonPrimParm.vertexColor += 12;
                }
                if (remaining >= 6) {
                    dds3PolygonPrimParm.faceCount = (remaining / 3) * 4 - 4;
                    dds3PolygonPrimParm.pointCount = remaining;
                    sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&dds3PolygonPrimParm)));
                }
            }
        } else if (system->type == TRACK_TOPOLOGY_FIVE_VECTOR) {
            dds3PolygonPrimParm.faceIndex = dds3TrackPolyFiveVectorFaceIndex;
            for (i = 0; i < count; i++) {
                cell = &system->trackList[i];
                dds3PolygonPrimParm.faceCount = 8;
                dds3PolygonPrimParm.pointCount = 10;
                remaining = cell->count;
                dds3PolygonPrimParm.vertex = (sdfVector4_t *)cell->pVertex;
                dds3PolygonPrimParm.vertexColor = cell->pVC;
                dds3PolygonPrimParm.color = cell->color;
                while (remaining >= 10) {
                    remaining -= 5;
                    sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&dds3PolygonPrimParm)));
                    dds3PolygonPrimParm.vertex += 5;
                    dds3PolygonPrimParm.vertexColor += 5;
                }
            }
        } else if (system->type == TRACK_TOPOLOGY_SIX_VECTOR) {
            dds3PolygonPrimParm.faceIndex = dds3TrackPolySixVectorFaceIndex;
            for (i = 0; i < count; i++) {
                cell = &system->trackList[i];
                dds3PolygonPrimParm.faceCount = 10;
                dds3PolygonPrimParm.pointCount = 12;
                remaining = cell->count;
                dds3PolygonPrimParm.vertex = (sdfVector4_t *)cell->pVertex;
                dds3PolygonPrimParm.vertexColor = cell->pVC;
                dds3PolygonPrimParm.color = cell->color;
                while (remaining >= 12) {
                    remaining -= 6;
                    sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&dds3PolygonPrimParm)));
                    dds3PolygonPrimParm.vertex += 6;
                    dds3PolygonPrimParm.vertexColor += 6;
                }
            }
        } else {
            dds3PolygonPrimParm.faceIndex = dds3TrackPolyDefaultFaceIndex;
            for (i = 0; i < count; i++) {
                cell = &system->trackList[i];
                dds3PolygonPrimParm.faceCount = 12;
                dds3PolygonPrimParm.pointCount = 16;
                remaining = cell->count;
                dds3PolygonPrimParm.vertex = (sdfVector4_t *)cell->pVertex;
                dds3PolygonPrimParm.vertexColor = cell->pVC;
                dds3PolygonPrimParm.color = cell->color;
                while (remaining >= 16) {
                    remaining -= 12;
                    sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&dds3PolygonPrimParm)));
                    dds3PolygonPrimParm.vertex += 12;
                    dds3PolygonPrimParm.vertexColor += 12;
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (lists[i] != 0) {
            ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))dds3TrackPolyOtTable[i]->regFunc)(dds3TrackPolyOtTable[i], lists[i]);
        }
    }
    if (lists[4] != 0) {
        specialList = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(specialList);
        packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x30));
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x8000000026ULL;
        packet[5] = 0x42;
        sdfDrawTagLink(specialList, EE_ADDR(packet));
        D_002F64B8.regFunc(&D_002F64B8, specialList);
        packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x30));
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(lists[4], EE_ADDR(packet));
        D_002F64B8.regFunc(&D_002F64B8, lists[4]);
    }
    dds3TrackPolyTag = NULL;
}

dds3TrianglePoly_t *dds3TrianglePolyCreate(s32 number) {
    s32 points = number * 3;
    s32 colorBytes = points * 4;
    sdfMemHandle_t *allocation = sdfMemAllocDirect((colorBytes + points) * 4 + 0x18);
    s32 base = sdfMemLock(allocation);
    u8 *vertices = EE_ADDR_AS(u8 *, base) + points * 16;
    dds3TrianglePoly_t *block = (dds3TrianglePoly_t *)(vertices + colorBytes);
    block->abgr = 0x80808080;
    block->number = number;
    block->pVC = (u32 *)vertices;
    block->hMem = allocation;
    block->pVertex = EE_ADDR_AS(u128 *, base);
    block->pMat = sdfMatNew();
    sdfMatSetConstant(block->pMat, 1.0f);
    return block;
}

void dds3TrianglePolyDelete(dds3TrianglePoly_t *hTriPoly) {
    sdfMatFreeDelayed(hTriPoly->pMat);
    sdfMemFreeDirect(hTriPoly->hMem);
}

void dds3TrianglePolyLinkOt(sdfGrOt_t *pOt, dds3TrianglePoly_t *hTriPoly) {
    sdfDrawTag_t *list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdf3DPrimPolyParm_t state;
    s32 remaining;
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    remaining = hTriPoly->number * 3;
    memset(&state, 0, sizeof(state));
    state.faceCount = 0x10;
    state.pointCount = 0x30;
    state.attr = 0x4000;
    state.vertex = (sdfVector4_t *)hTriPoly->pVertex;
    state.vertexColor = hTriPoly->pVC;
    state.color = hTriPoly->abgr;
    while (remaining >= 0x30) {
        remaining -= 0x30;
        sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&state)));
    }
    if (remaining > 0) {
        dds3PolygonPrimParm.faceCount = remaining / 3;
        dds3PolygonPrimParm.pointCount = remaining;
        sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&state)));
    }
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))pOt->regFunc)(pOt, list);
}

dds3SpPolyOldDesc_t *dds3SpPoly_Ring_Old_Create(dds3SpPolyOldDesc_t *src) {
    s32 tailSize = sizeof(dds3SpPolyOldCellParam_t);
    dds3SpPolyOldDesc_t *desc = sdfChipAllocDirect(src->count * 4 + 0xF0);

    memset(desc, 0, 0xF0);
    memcpy(desc, src, src->headerSize);
    memcpy(&desc->cells, (u8 *)src + src->headerSize, tailSize);
    desc->headerSize = 0xC0;
    desc->cells.cellWords = (u32 *)(desc + 1);
    if (desc->cells.verticesPerCell < 3) {
        desc->cells.verticesPerCell = 3;
    }
    desc->cells.cellSystem = dds3TrackPolyCreate(desc->count, desc->cells.verticesPerCell, 1, TRACK_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(desc->cells.cellSystem, 0,
                   desc->cells.dispatchArgA,
                   desc->cells.dispatchArgB);
    dds3SpPoly_Ring_Old_BuffInit((dds3SpPoly_Ring_Old_t3 *)desc);
    return desc;
}

void dds3SpPoly_Ring_Old_Delete(dds3SpPoly_Ring_Old_t3 *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    sdfChipFreeDirect(hData);
}

void dds3SpPoly_Ring_Old_BuffInit(dds3SpPoly_Ring_Old_t3 *hData) {
    u16 count;
    s32 age;
    s32 *ages;
    u16 i;
    dds3TrackPoly_t *system;
    dds3TrackPolyList_t *cells;

    count = hData->entryCount;
    age = POLY_INACTIVE_ENTRY_AGE;
    ages = hData->ages;
    system = hData->hTrackPoly;
    i = 0;
    if (hData->entryCount != 0) {
        cells = system->trackList;
        do {
            cells[i].rec = 0;
            cells[i].count = 0;
            cells[i].color = 0x00808080;
            *ages = age;
            ages++;
            age -= SDF_FRAMES(hData->spawnDelayStep);
            i++;
        } while (i < count);
    }
}

void dds3SpPoly_Ring_Old_SetScale(dds3SpPoly_Ring_Old_t3 *hData, f32 scale) {
    hData->radius = hData->radius * scale;
    hData->pairDisplacement = hData->pairDisplacement * scale;
}

void dds3SpPoly_Ring_Old_Calc(dds3SpPoly_Ring_Old_t3 *pRing) {
    u16 index;
    u16 completed;
    u16 count;
    u8 loop;
    s32 duration;
    f32 alphaStep;
    dds3TrackPolyList_t *entry;
    s32 *ages;

    if (pRing->active != 0) {
        duration = SDF_FRAMES(pRing->duration);
        completed = 0;
        count = pRing->entryCount;
        entry = pRing->hTrackPoly->trackList;
        alphaStep = 128.0f / (f32)duration;
        ages = pRing->ages;
        loop = pRing->loop;
        for (index = 0; index < count; index++) {
            s32 age = *ages;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                dds3SpPoly_Ring_Old_New(pRing, index);
                *ages = 0;
            } else if (age >= 0) {
                entry->color = ((u32)(alphaStep * (f32)(duration - age)) << 24) | 0x808080;
                dds3SpPoly_Ring_Old_Move(pRing, index);
            }
            if (age >= duration) {
                entry->color = 0x808080;
                if (loop != 0) {
                    *ages = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= count) {
                        entry->color = 0;
                        pRing->active = 0;
                    }
                }
            } else {
                (*ages)++;
            }
            ages++;
            entry++;
        }
    }
}

void dds3SpPoly_Ring_Old_Draw(dds3SpPoly_Ring_Old_t3 *hData) {
    dds3SpPoly_Ring_Old_Calc(hData);
    dds3TrackPolyDraw(hData->hTrackPoly);
}

void dds3SpPoly_Ring_Old_New(dds3SpPoly_Ring_Old_t3 *pRing, s32 number) {
    dds3TrackPoly_t *strip = pRing->hTrackPoly;
    dds3TrackPolyList_t *cell = &strip->trackList[number];
    f32 radius = pRing->radius;
    sdfVector4_t origin;
    sdfVector4_t offset;
    f32 angle;
    f32 step;
    f32 cosine;
    f32 sine;
    u128 *p;
    u128 *hist;
    s32 half;
    s32 pairs;
    s32 i;

    step = 6.2831853f / (f32)pRing->segments;
    cell->count = strip->point;
    p = cell->pVertex;
    half = strip->point / 2;
    SDF_VU0_LOADMATRIX(pRing->matrix);
    angle = 0.0f;
    SDF_VU0_COPYVECTOR_F32(&origin, pRing->origin);
    pairs = half - 1;
    for (i = 0; i < pairs; i++) {
        cosine = sdfCos(angle);
        sine = sdfSin(angle);
        SDF_VU0_LOADVECTOR_B(&origin);
        SDF_VU0_STOREVECTOR_B(p);
        offset.x = cosine * radius;
        offset.y = 0.0f;
        offset.z = sine * radius;
        SDF_VU0_LOADVECTOR(&offset);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(p + 1);
        p += 2;
        angle += step;
    }
    hist = cell->pVertex;
    SDF_VU0_COPYVECTOR(p, hist);
    SDF_VU0_COPYVECTOR(p + 1, hist + 1);
}

void dds3SpPoly_Ring_Old_Move(dds3SpPoly_Ring_Old_t3 *pRing, s32 number) {
    dds3TrackPoly_t *strip = pRing->hTrackPoly;
    dds3TrackPolyList_t *entry = &strip->trackList[number];
    sdfVector4_t scale;
    f32 *p;
    s32 pairs;
    s32 i;

    p = (f32 *)entry->pVertex;
    pairs = strip->point / 2;
    scale.x = scale.y = scale.z = SDF_FRAME_STEP(pRing->pairDisplacement);
    SDF_VU0_LOADVECTOR_C(&scale);
    for (i = 0; i < pairs; i++) {
        SDF_VU0_LOADVECTOR(p + 4);
        SDF_VU0_LOADVECTOR_B(p);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(p);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(p);
        SDF_VU0_LOADVECTOR(p + 4);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(p + 4);
        p += 8;
    }
}

u32 dds3SpPoly_Color(u32 frame, u32 frame_end, u32 abgr) {
    f32 ratio = 1.0f;
    s32 tint[4];
    u8 unused[16]; /* SMT3 and DDS1 twin both reserve 0x20 bytes,
                      * but neither touches the spare 0x10-byte local. */
    u32 packed;

    if (frame < frame_end) {
        ratio = (f32)frame / (f32)frame_end;
    }
    SDF_VU0_LOADVECTOR(dds3SpPolyColorFrom);
    SDF_VU0_LOADVECTOR_B(dds3SpPolyColorTo);
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_SCALEVECTOR_B_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    tint[0] = abgr;
    MMI_RGBA_UNPACK(tint, 1.0f / 128.0f);
    SDF_VU0_MULVECTOR();
    MMI_RGBA_PACK_UNIT(packed, 128.0f);
    return packed;
}

dds3SpPoly_Ring_t3 *dds3SpPoly_RingCreate(dds3SpPoly_Basic_t *templateHead) {
    s32 tailSize = sizeof(dds3SpPoly_Ring_t3) - sizeof(dds3SpPoly_Basic_t);
    u32 size = templateHead->number * sizeof(dds3SpPoly_RingPhysics_t) + sizeof(dds3SpPoly_Ring_t3);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    dds3SpPoly_Ring_t3 *band = EE_ADDR_AS(dds3SpPoly_Ring_t3 *, sdfMemLock(allocation));

    memset(band, 0, size);
    memcpy(band, templateHead, templateHead->templateSize);
    memcpy(&band->spawnDelayStep, (u8 *)templateHead + templateHead->templateSize, tailSize);
    band->head.templateSize = sizeof(dds3SpPoly_Basic_t);
    band->pPhysics = (dds3SpPoly_RingPhysics_t *)(band + 1);
    band->hMem = allocation;
    if (((dds3SpPolyRingUnion_t *)band)->band.vertex < 3) {
        ((dds3SpPolyRingUnion_t *)band)->band.vertex = 3;
    }
    band->hTrackPoly = dds3TrackPolyCreate(band->head.number, band->vertex, 1, 0);
    dds3TrackPolyVCSet(band->hTrackPoly, 0, band->dispatchArgA, band->dispatchArgB);
    dds3SpPoly_RingAllBuffInit(band);
    SDF_VU0_LOADMATRIX(band->head.matrix);
    SDF_VU0_STOREMATRIX(band->head.matrixCopy);
    dds3TrackPolySetMode(band->hTrackPoly, band->head.mode);
    return band;
}

void dds3SpPoly_RingDelete(dds3SpPoly_Ring_t3 *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    sdfMemFreeDirect(hData->hMem);
}

void dds3SpPoly_RingAllBuffInit(dds3SpPoly_Ring_t3 *band) {
    u32 count = band->head.number;
    dds3SpPoly_RingPhysics_t *record = band->pPhysics;
    u32 delayStep = SDF_FRAMES(band->spawnDelayStep);
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            index++;
            record->frame = age;
            age -= delayStep;
            record++;
        } while (index < count);
    }
}

void dds3SpPoly_RingBuffInit(dds3SpPoly_Ring_t3 *ring, s32 number)
{
    dds3SpPoly_RingPhysics_t *record;
    f32 spread;
    f32 radius;
    f32 endRadius;
    s32 frames;

    record = ring->pPhysics;
    record += number;
    spread = ring->initialRadiusJitter;
    radius = ring->initialRadius * (dds3GetRandFloatA(dds3EffectBase) * spread + (1.0f - spread));
    spread = ring->targetRadiusJitter;
    endRadius = ring->targetRadius * (dds3GetRandFloatA(dds3EffectBase) * spread + (1.0f - spread));
    record->frame = 0;
    frames = ring->head.frame;
    if (frames > 0) {
        record->spread_s = (endRadius - radius) / (f32)frames;
    } else {
        record->spread_s = endRadius - radius;
    }
    func_00168A50(ring, number, radius);
}

void func_00168A50(dds3SpPoly_Ring_t3 *ring, s32 number, f32 radius)
{
    dds3TrackPoly_t *strip = ring->hTrackPoly;
    dds3TrackPolyList_t *entry = &strip->trackList[number];
    sdfVector4_t radial;
    sdfVector4_t inner;
    sdfVector4_t outer;
    f32 angleStep;
    f32 angle;
    u128 *pointCursor;
    u128 *firstPointPair;
    s32 pairs;
    s32 i;

    entry->count = strip->point;
    pointCursor = entry->pVertex;
    pairs = strip->point / 2;
    inner.x = radius;
    inner.y = radius;
    inner.z = radius;
    outer.x = radius + ring->radialWidth;
    outer.y = radius + ring->radialWidth;
    outer.z = radius + ring->radialWidth;
    angleStep = 6.2831853f / (f32)ring->vertex;
    SDF_VU0_LOADMATRIX(ring->head.matrix);
    angle = 0.0f;
    SDF_VU0_LOADVECTOR_C(ring->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        radial.x = sdfCos(angle);
        radial.y = 0.0f;
        radial.z = sdfSin(angle);
        SDF_VU0_LOADVECTOR(&radial);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&radial);
        SDF_VU0_LOADVECTOR_B(&inner);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pointCursor + 1);
        SDF_VU0_LOADVECTOR(&radial);
        SDF_VU0_LOADVECTOR_B(&outer);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pointCursor);
        pointCursor += 2;
        angle += angleStep;
    }
    firstPointPair = entry->pVertex;
    SDF_VU0_COPYVECTOR(pointCursor, firstPointPair);
    SDF_VU0_COPYVECTOR(pointCursor + 1, firstPointPair + 1);
}

void dds3SpPoly_RingMove(dds3SpPoly_Ring_t3 *pRing, s32 number) {
    dds3TrackPoly_t *strip = pRing->hTrackPoly;
    dds3SpPoly_RingPhysics_t *rec = &pRing->pPhysics[number];
    dds3TrackPolyList_t *entry = &strip->trackList[number];
    sdfVector4_t radialDirection;
    sdfVector4_t radiusStepVector;
    sdfVector4_t liftVector;
    f32 angleStep;
    f32 angle;
    f32 *pointCursor;
    f32 *firstPointPair;
    s32 pairs;
    s32 i;

    entry->count = strip->point;
    pointCursor = (f32 *)entry->pVertex;
    pairs = strip->point >> 1;
    radiusStepVector.z = radiusStepVector.y = radiusStepVector.x = SDF_FRAME_STEP(rec->spread_s);
    liftVector.y = SDF_FRAME_STEP(pRing->liftStep);
    liftVector.z = liftVector.x = 0;
    angleStep = 3.14159265f * 2.0f / (f32)pRing->vertex;
    SDF_VU0_LOADMATRIX(pRing->head.matrix);
    angle = 0.0f;
    for (i = 0; i < pairs - 1; i++) {
        radialDirection.x = sdfCos(angle);
        radialDirection.y = 0;
        radialDirection.z = sdfSin(angle);
        SDF_VU0_LOADVECTOR(&radialDirection);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&radiusStepVector);
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(&liftVector);
        SDF_VU0_APPLYMATRIX_B();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(pointCursor + 4);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(pointCursor + 4);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_LOADVECTOR_B(pointCursor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(pointCursor);
        pointCursor += 8;
        angle += angleStep;
    }
    firstPointPair = (f32 *)entry->pVertex;
    SDF_VU0_COPYVECTOR(pointCursor, firstPointPair);
    SDF_VU0_COPYVECTOR(pointCursor + 4, firstPointPair + 4);
}

void dds3SpPoly_RingSetScale(dds3SpPoly_Ring_t3 *hData, f32 scale) {
    hData->radialWidth = hData->radialWidth * scale;
    hData->liftStep = hData->liftStep * scale;
    hData->initialRadius = hData->initialRadius * scale;
    hData->targetRadius = hData->targetRadius * scale;
}

void dds3SpPoly_RingDraw(dds3SpPoly_Ring_t3 *hData) {
    s32 recordIndex = 0;
    u32 completed = 0;
    u8 loop;
    dds3TrackPoly_t *strip;
    s32 count;
    dds3SpPoly_RingPhysics_t *record;
    s32 duration;
    u32 color;
    dds3TrackPolyList_t *entry;

    strip = hData->hTrackPoly;
    count = hData->head.number;
    record = hData->pPhysics;
    duration = SDF_FRAMES(hData->head.frame);
    loop = hData->head.loop;
    color = hData->head.abgr;
    entry = strip->trackList;

    if (count > 0) {
        do {
            s32 age = record->frame;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                dds3SpPoly_RingBuffInit(hData, recordIndex);
                age = record->frame;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < SDF_FRAMES(hData->head.alpha_s)) {
                    entry->color = dds3SpPoly_Color(age, SDF_FRAMES(hData->head.alpha_s), color);
                } else if (age <= duration &&
                           (u32)age >= duration - SDF_FRAMES(hData->head.alpha_e)) {
                    entry->color = dds3SpPoly_Color(duration - age, SDF_FRAMES(hData->head.alpha_e), color);
                }

                if (entry->color & 0xFF000000) {
                    dds3SpPoly_RingMove(hData, recordIndex);
                    age++;
                } else {
                    entry->count = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        hData->head.state = 0;
                    }
                }
                entry->count = 0;
            }

            record->frame = age;
            recordIndex++;
            entry++;
            record++;
        } while (recordIndex < count);
    }
    dds3TrackPolyDraw(hData->hTrackPoly);
}

dds3SpPoly_SphereRing_t3 *dds3SpPoly_SphereRingCreate(dds3SpPoly_SphereRing_t3 *source) {
    s32 tailBytes = sizeof(dds3SpPoly_SphereRing_t3) - sizeof(dds3SpPoly_Basic_t);
    s32 size = source->head.number * sizeof(dds3SpPoly_SphereRingPhysics_t) + sizeof(dds3SpPoly_Basic_t) + tailBytes;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    dds3SpPoly_SphereRing_t3 *clone = EE_ADDR_AS(dds3SpPoly_SphereRing_t3 *, sdfMemLock(allocation));

    memset(clone, 0, size);
    memcpy(clone, source, source->head.templateSize);
    memcpy(&clone->spawnDelayStep, (u8 *)source + source->head.templateSize, tailBytes);
    clone->head.templateSize = 0xC0;
    clone->pPhysics = (dds3SpPoly_SphereRingPhysics_t *)(clone + 1);
    clone->hMem = allocation;
    if (((dds3SpPolyRingUnion_t *)clone)->arc.vertex < 3) {
        ((dds3SpPolyRingUnion_t *)clone)->arc.vertex = 3;
    }
    clone->hTrackPoly = dds3TrackPolyCreate(clone->head.number, clone->vertex, 1, 0);
    dds3TrackPolyVCSet(clone->hTrackPoly, 0, clone->dispatchArgA, clone->dispatchArgB);
    dds3SpPoly_SphereRingAllBuffInit(clone);
    SDF_VU0_LOADMATRIX(clone->head.matrix);
    SDF_VU0_STOREMATRIX(clone->head.matrixCopy);
    dds3TrackPolySetMode(clone->hTrackPoly, clone->head.mode);
    return clone;
}

void dds3SpPoly_SphereRingDelete(dds3SpPoly_SphereRing_t3 *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    sdfMemFreeDirect(hData->hMem);
}

void dds3SpPoly_SphereRingAllBuffInit(dds3SpPoly_SphereRing_t3 *arc) {
    u32 count = arc->head.number;
    dds3SpPoly_SphereRingPhysics_t *record = arc->pPhysics;
    u32 delayStep = SDF_FRAMES(arc->spawnDelayStep);
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            index++;
            record->frame = age;
            age -= delayStep;
            record++;
        } while (index < count);
    }
}

void dds3SpPoly_SphereRingBuffInit(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number) {
    dds3SpPoly_SphereRingPhysics_t *record = pSphereRing->pPhysics;
    f32 spread = pSphereRing->radius_r;

    record += number;
    record->frame = 0;
    record->radius = pSphereRing->radius * (dds3GetRandFloatA(dds3EffectBase) * spread + (1.0f - spread));
}

void dds3SpPoly_SphereRingMove(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number) {
    dds3TrackPoly_t *strip = pSphereRing->hTrackPoly;
    dds3SpPoly_SphereRingPhysics_t *record = &pSphereRing->pPhysics[number];
    dds3TrackPolyList_t *stripEntry = &strip->trackList[number];
    sdfVector4_t innerPointVector;
    sdfVector4_t outerPointVector;
    f32 recordRadius;
    f32 projectedInnerRadius;
    f32 verticalOffset;
    f32 radialWidth;
    f32 angleStep;
    f32 angle;
    f32 *pointCursor;
    f32 *firstPointPair;
    s32 pairs;
    s32 i;

    stripEntry->count = strip->point;
    pointCursor = (f32 *)stripEntry->pVertex;
    pairs = strip->point >> 1;
    recordRadius = record->radius;
    angle = (f32)record->frame / (f32)SDF_FRAMES(pSphereRing->head.frame) * 3.14159265f;
    projectedInnerRadius = recordRadius * sdfSin(angle);
    verticalOffset = recordRadius * sdfCos(angle) - recordRadius;
    angleStep = 3.14159265f * 2.0f / (f32)pSphereRing->vertex;
    SDF_VU0_LOADMATRIX(pSphereRing->head.matrix);
    radialWidth = pSphereRing->radialWidth;
    angle = 0.0f;
    SDF_VU0_LOADVECTOR_C(pSphereRing->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        innerPointVector.x = outerPointVector.x = sdfCos(angle);
        innerPointVector.z = sdfSin(angle);
        outerPointVector.y = verticalOffset;
        outerPointVector.z = innerPointVector.z * (projectedInnerRadius + radialWidth);
        outerPointVector.x *= projectedInnerRadius + radialWidth;
        SDF_VU0_LOADVECTOR(&outerPointVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&outerPointVector);
        innerPointVector.y = verticalOffset;
        innerPointVector.x *= projectedInnerRadius;
        innerPointVector.z *= projectedInnerRadius;
        SDF_VU0_LOADVECTOR(&innerPointVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pointCursor + 4);
        SDF_VU0_LOADVECTOR(&outerPointVector);
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pointCursor);
        pointCursor += 8;
        angle += angleStep;
    }
    firstPointPair = (f32 *)stripEntry->pVertex;
    SDF_VU0_COPYVECTOR(pointCursor, firstPointPair);
    SDF_VU0_COPYVECTOR(pointCursor + 4, firstPointPair + 4);
}

void dds3SpPoly_SphereRingSetScale(dds3SpPoly_SphereRing_t3 *hData, f32 scale) {
    hData->radius = hData->radius * scale;
}

void dds3SpPoly_SphereRingDraw(dds3SpPoly_SphereRing_t3 *hData) {
    s32 index = 0;
    u32 completed = 0;
    u8 loop;
    dds3TrackPoly_t *strip;
    s32 count;
    dds3SpPoly_SphereRingPhysics_t *record;
    s32 duration;
    u32 color;
    dds3TrackPolyList_t *entry;

    strip = hData->hTrackPoly;
    count = hData->head.number;
    record = hData->pPhysics;
    duration = SDF_FRAMES(hData->head.frame);
    loop = hData->head.loop;
    color = hData->head.abgr;
    entry = strip->trackList;

    if (count > 0) {
        do {
            s32 age = record->frame;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                dds3SpPoly_SphereRingBuffInit(hData, index);
                age = record->frame;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < SDF_FRAMES(hData->head.alpha_s)) {
                    entry->color = dds3SpPoly_Color(age, SDF_FRAMES(hData->head.alpha_s), color);
                } else if (age <= duration &&
                           (u32)age >= duration - SDF_FRAMES(hData->head.alpha_e)) {
                    entry->color = dds3SpPoly_Color(duration - age, SDF_FRAMES(hData->head.alpha_e), color);
                }

                if (entry->color & 0xFF000000) {
                    dds3SpPoly_SphereRingMove(hData, index);
                    age++;
                } else {
                    entry->count = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        hData->head.state = 0;
                    }
                }
                entry->count = 0;
            }

            record->frame = age;
            index++;
            entry++;
            record++;
        } while (index < count);
    }
    dds3TrackPolyDraw(hData->hTrackPoly);
}

dds3SpPoly_RollRing_t3 *dds3SpPoly_RollRingCreate(dds3SpPoly_Basic_t *templateHead) {
    s32 tailSize = sizeof(dds3SpPoly_RollRing_t3) - sizeof(dds3SpPoly_Basic_t);
    u32 size = templateHead->number * sizeof(dds3SpPoly_RollRingPhysics_t) + sizeof(dds3SpPoly_RollRing_t3);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    dds3SpPoly_RollRing_t3 *band = EE_ADDR_AS(dds3SpPoly_RollRing_t3 *, sdfMemLock(allocation));

    memset(band, 0, size);
    memcpy(band, templateHead, templateHead->templateSize);
    memcpy(&band->spawnDelayStep, (u8 *)templateHead + templateHead->templateSize, tailSize);
    band->head.templateSize = sizeof(dds3SpPoly_Basic_t);
    band->pPhysics = (dds3SpPoly_RollRingPhysics_t *)(band + 1);
    band->allocation = allocation;
    if (((dds3SpPolyRingUnion_t *)band)->rotating.vertex < 3) {
        ((dds3SpPolyRingUnion_t *)band)->rotating.vertex = 3;
    }
    band->hTrackPoly = dds3TrackPolyCreate(band->head.number, band->vertex, 1, 0);
    dds3TrackPolyVCSet(band->hTrackPoly, 0, band->color_edge_out, band->color_edge_in);
    dds3SpPoly_RollRingAllBuffInit(band);
    SDF_VU0_LOADMATRIX(band->head.matrix);
    SDF_VU0_STOREMATRIX(band->head.matrixCopy);
    dds3TrackPolySetMode(band->hTrackPoly, band->head.mode);
    return band;
}

void dds3SpPoly_RollRingDelete(dds3SpPoly_RollRing_t3 *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    sdfMemFreeDirect(hData->allocation);
}

void dds3SpPoly_RollRingAllBuffInit(dds3SpPoly_RollRing_t3 *rotatingBand) {
    u32 count = rotatingBand->head.number;
    dds3SpPoly_RollRingPhysics_t *record = rotatingBand->pPhysics;
    u32 delayStep = SDF_FRAMES(rotatingBand->spawnDelayStep);
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            record->frame = age;
            record++;
            if ((index + 1) % rotatingBand->simu_num == 0) {
                age -= delayStep;
            }
            index++;
        } while (index < count);
    }
}

void dds3SpPoly_RollRingBuffInit(dds3SpPoly_RollRing_t3 *pRollRing, u32 number) {
    dds3SpPoly_RollRingPhysics_t *record = pRollRing->pPhysics;
    f32 spread;

    record += number;
    spread = pRollRing->radius_s_r;
    record->frame = 0;
    record->radius = pRollRing->radius_s * (dds3GetRandFloatA(dds3EffectBase) * spread + (1.0f - spread));
    spread = pRollRing->radius_e_r;
    record->spread_s = (pRollRing->radius_e * (dds3GetRandFloatA(dds3EffectBase) * spread + (1.0f - spread)) - record->radius) / (f32)pRollRing->head.frame;
    record->rot_x = pRollRing->incline * 0.017453292f;
    record->rot_y = 6.2831852f / (f32)pRollRing->simu_num * (f32)(number % pRollRing->simu_num);
}

void dds3SpPoly_RollRingMove(dds3SpPoly_RollRing_t3 *pRollRing, s32 number)
{
    dds3TrackPoly_t *strip = pRollRing->hTrackPoly;
    dds3SpPoly_RollRingPhysics_t *rec = &pRollRing->pPhysics[number];
    dds3TrackPolyList_t *entry = &strip->trackList[number];
    sdfVector4_t dir;
    sdfVector4_t baseRadiusVector;
    sdfVector4_t offsetRadiusVector;
    f32 step;
    f32 radius;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->point;
    out = (f32 *)entry->pVertex;
    pairs = strip->point >> 1;
    SetRotateMatrixX(rec->rot_x);
    SetRotateMatrixY_B(rec->rot_y);
    MultMatrixRevNoArg();
    rec->rot_y += SDF_FRAME_STEP(pRollRing->roll_s * (3.14159265f / 180.0f));
    radius = rec->radius;
    rec->radius = radius + SDF_FRAME_STEP(rec->spread_s);
    step = 3.14159265f * 2.0f / (f32)pRollRing->vertex;
    SDF_VU0_LOADMATRIX_B(pRollRing->head.matrix);
    MultMatrixRevNoArg();
    baseRadiusVector.x = radius;
    baseRadiusVector.y = radius;
    baseRadiusVector.z = radius;
    offsetRadiusVector.x = radius + pRollRing->width;
    offsetRadiusVector.y = radius + pRollRing->width;
    offsetRadiusVector.z = radius + pRollRing->width;
    angle = 0.0f;
    SDF_VU0_LOADVECTOR_C(pRollRing->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        dir.x = sdfCos(angle);
        dir.y = 0;
        dir.z = sdfSin(angle);
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&dir);
        SDF_VU0_LOADVECTOR_B(&baseRadiusVector);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(out + 4);
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_LOADVECTOR_B(&offsetRadiusVector);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(out);
        out += 8;
        angle += step;
    }
    first = (f32 *)entry->pVertex;
    SDF_VU0_COPYVECTOR(out, first);
    SDF_VU0_COPYVECTOR(out + 4, first + 4);
}

void dds3SpPoly_RollRingSetScale(dds3SpPoly_RollRing_t3 *hData, f32 scale) {
    hData->radius_s = hData->radius_s * scale;
    hData->radius_e = hData->radius_e * scale;
}

/* DDS leaves a record of a one-frame effect running. */
void dds3SpPoly_RollRingDraw(dds3SpPoly_RollRing_t3 *hData) {
    s32 recordIndex = 0;
    u32 completed = 0;
    u8 loop;
    dds3TrackPoly_t *strip;
    s32 count;
    dds3SpPoly_RollRingPhysics_t *record;
    s32 duration;
    u32 color;
    dds3TrackPolyList_t *entry;

    strip = hData->hTrackPoly;
    count = hData->head.number;
    record = hData->pPhysics;
    duration = SDF_FRAMES(hData->head.frame);
    loop = hData->head.loop;
    color = hData->head.abgr;
    entry = strip->trackList;

    if (count > 0) {
        s32 inactiveAge = POLY_INACTIVE_ENTRY_AGE;
        s32 shortDuration = duration < 2;

        do {
            s32 age = record->frame;

            if (age == inactiveAge) {
                dds3SpPoly_RollRingBuffInit(hData, recordIndex);
                age = record->frame;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < SDF_FRAMES(hData->head.alpha_s)) {
                    entry->color = dds3SpPoly_Color(age, SDF_FRAMES(hData->head.alpha_s), color);
                } else if (age <= duration &&
                           (u32)age >= duration - SDF_FRAMES(hData->head.alpha_e)) {
                    entry->color = dds3SpPoly_Color(duration - age, SDF_FRAMES(hData->head.alpha_e), color);
                } else {
                    entry->color = color;
                }

                if (entry->color & 0xFF000000) {
                    dds3SpPoly_RollRingMove(hData, recordIndex);
                } else {
                    entry->count = 0;
                }
                age++;
            }

            if (age >= duration && !shortDuration) {
                if (loop != 0) {
                    age = inactiveAge;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        hData->head.state = 0;
                    }
                }
                entry->count = 0;
            }

            record->frame = age;
            recordIndex++;
            entry++;
            record++;
        } while (recordIndex < count);
    }
    dds3TrackPolyDraw(hData->hTrackPoly);
}


void dds3SpPoly_RollRingSetEndless(dds3SpPoly_RollRing_t3 *pRollRing) {
    u32 count;
    dds3SpPoly_RollRingPhysics_t *entry;
    u32 i;

    count = pRollRing->head.number;
    i = 0;
    pRollRing->head.frame = POLY_RESET_DURATION;
    pRollRing->head.alpha_e = 0;
    pRollRing->head.alpha_s = 0;
    entry = pRollRing->pPhysics;
    if (count != 0) {
        do {
            if (entry->frame != POLY_INACTIVE_ENTRY_AGE) {
                entry->frame = SDF_FRAMES(POLY_RESET_DURATION) - SDF_FRAMES(POLY_RESET_DURATION - POLY_RESET_ENTRY_AGE);
            }
            i++;
            entry++;
        } while (i < count);
    }
}

void *dds3SpPolyCreate(s32 index, void *arg) {
    dds3SpPoly_Basic_t *effect = dds3SpPolyTable[index].func(arg);

    effect->type = index;
    effect->state = 1;
    return effect;
}

void *dds3SpPolyCopy(dds3SpPoly_Basic_t *effect) {
    dds3SpPoly_Basic_t *result = dds3SpPolyTable[effect->type].func(effect);

    result->type = effect->type;
    result->state = 1;
    return result;
}

void dds3SpPolyDelete(dds3SpPoly_Basic_t *hData) {
    dds3SpPolyTable[hData->type].destroy(hData);
}

void dds3SpPolyDraw(dds3SpPoly_Basic_t *hData) {
    dds3SpPolyTable[hData->type].callback(hData);
}

void dds3SpPolyReset(dds3SpPoly_Basic_t *hData) {
    dds3SpPolyResetTable[hData->type](hData);
    hData->state = 1;
}

void dds3SpPolySetScale(dds3SpPoly_Basic_t *hData, f32 scale) {
    dds3SpPolySetScaleTable[hData->type](hData, scale);
    dds3SpPolyReset(hData);
}

u16 dds3SpPolyGetState(dds3SpPoly_Basic_t *hData) {
    return hData->state;
}

void dds3SpPolySetPosition(void *hData, void *pPos) {
    SDF_VU0_COPYVECTOR(hData, pPos);
}

/* vu0 routine: compose the new rotation with the retained source matrix. */
void dds3SpPolySetRotateMatrix(dds3SpPoly_Basic_t *hData, void *pMatrix) {
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_LOADMATRIX_B(hData->matrixCopy);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX(hData->matrix);
}

void dds3SpPolySetRGBA(dds3SpPoly_Basic_t *hData, u32 abgr) {
    hData->abgr = abgr;
}

void dds3SpPolySetLoop(dds3SpPolyLoopWork_t *hData, u8 loop) {
    if (hData->handler == 0) {
        hData->oldLoop = loop;
    }
    hData->loop = loop;
}

u8 dds3SpPolyGetLoop(dds3SpPolyLoopWork_t *hData) {
    return hData->loop;
}

void dds3PolygonSetColor(dds3Polygon_t *hPolygon, u32 color) {
    hPolygon->parm.color = color;
}

void dds3PolygonTriangleSet(dds3Polygon_t *hPolygon, const f32 *center) {
    s32 count = hPolygon->drawCount;
    f32 point[4] __attribute__((aligned(16)));
    f32 (*positions)[4];
    u16 flags;

    SDF_VU0_COPYVECTOR(point, center);
    positions = (f32 (*)[4])(((u32)count << 4) + EE_ADDR(hPolygon->pVertex));
    SDF_VU0_COPYVECTOR(positions[0], point);
    point[1] += 5.0f;
    SDF_VU0_COPYVECTOR(positions[1], point);
    point[2] += 5.0f;
    SDF_VU0_COPYVECTOR(positions[2], point);
    flags = hPolygon->flags;
    if (flags & 2) {
        f32 (*normals)[4] = (f32 (*)[4])(((u32)count << 4) + EE_ADDR(hPolygon->pNormal));
        SDF_VU0_COPYVECTOR(normals[0], dds3PolygonTriangleParam);
        SDF_VU0_COPYVECTOR(normals[1], dds3PolygonTriangleParam);
        SDF_VU0_COPYVECTOR(normals[2], dds3PolygonTriangleParam);
    }
    if (flags & 4) {
        u32 *colors = (u32 *)(((u32)count << 2) + EE_ADDR(hPolygon->pVertexColor));
        colors[0] = 0x80000080;
        colors[1] = 0x80000080;
        colors[2] = 0x80000080;
        count = hPolygon->drawCount;
    }
    hPolygon->drawCount = count + 3;
}

void dds3TrackPolyColorSet(dds3TrackPoly_t *hTrackPoly, s32 number, u32 color) {
    hTrackPoly->trackList[number].color = color;
}

typedef void (*dds3TrackPolyVCSetFunc_t)(void *, void *, void *);
typedef void *(*dds3SpPolyCreateFunc_t)(void *);
typedef void (*dds3SpPolyWorkFunc_t)(void *);
typedef void (*dds3SpPolyScaleFunc_t)(void *, f32);

u32 dds3TrackPolyPairFaceIndex[32] = {
    0x00030100, 0x00020300, 0x00050302, 0x00040502,
    0x00070504, 0x00060704, 0x00090706, 0x00080906,
    0x000B0908, 0x000A0B08, 0x000D0B0A, 0x000C0D0A,
    0x000F0D0C, 0x000E0F0C, 0x00110F0E, 0x0010110E,
    0x00131110, 0x00121310, 0x00151312, 0x00141512,
    0x00171514, 0x00161714, 0x00191716, 0x00181916,
    0x001B1918, 0x001A1B18, 0x001D1B1A, 0x001C1D1A,
    0x001F1D1C, 0x001E1F1C, 0x00000000, 0x00000000,
};
u32 dds3TrackPolyTriangleFaceIndex[28] = {
    0x00040100, 0x00030400, 0x00050201, 0x00040501,
    0x00070403, 0x00060703, 0x00080504, 0x00070804,
    0x000A0706, 0x00090A06, 0x000B0807, 0x000A0B07,
    0x000D0A09, 0x000C0D09, 0x000E0B0A, 0x000D0E0A,
    0x00100D0C, 0x000F100C, 0x00110E0D, 0x0010110D,
    0x0013100F, 0x0012130F, 0x00141110, 0x00131410,
    0x00161312, 0x00151612, 0x00171413, 0x00161713,
};
u32 dds3TrackPolySixVectorFaceIndex[32] = {
    0x00070100, 0x00060700, 0x00080201, 0x00070801,
    0x00090302, 0x00080902, 0x000A0403, 0x00090A03,
    0x000B0504, 0x000A0B04, 0x000D0706, 0x000C0D06,
    0x000E0807, 0x000D0E07, 0x000F0908, 0x000E0F08,
    0x00100A09, 0x000F1009, 0x00110B0A, 0x0010110A,
    0x00130D0C, 0x0012130C, 0x00140E0D, 0x0013140D,
    0x00150F0E, 0x0014150E, 0x0016100F, 0x0015160F,
    0x00171110, 0x00161710, 0x00000000, 0x00000000,
};
u32 dds3TrackPolyDefaultFaceIndex[20] = {
    0x00050100, 0x00040500, 0x00060201, 0x00050601,
    0x00070302, 0x00060702, 0x000D0908, 0x000C0D08,
    0x000E0A09, 0x000D0E09, 0x000F0B0A, 0x000E0F0A,
    0x00151110, 0x00141510, 0x00161211, 0x00151611,
    0x00171312, 0x00161712, 0x00000000, 0x00000000,
};
u32 dds3TrackPolyFiveVectorFaceIndex[24] = {
    0x00060100, 0x00050600, 0x00070201, 0x00060701,
    0x00080302, 0x00070802, 0x00090403, 0x00080903,
    0x00100B0A, 0x000F100A, 0x00110C0B, 0x0010110B,
    0x00120D0C, 0x0011120C, 0x00130E0D, 0x0012130D,
    0x001A1514, 0x00191A14, 0x001B1615, 0x001A1B15,
    0x001C1716, 0x001B1C16, 0x001D1817, 0x001C1D17,
};
f32 dds3PolygonTriangleParam[4] = { 0.0f, 0.0f, -1.0f, 0.0f };
f32 dds3TrackPolyVCType1Param[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 dds3TrackPolyVCType2Param[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 dds3TrackPolyVCSetAcuteParam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 dds3TrackPolyVCSetAcuteWParam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 dds3TrackPolyVCAcuteParam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
/* Indexed by track type * 3 + colour mode. Types 0, 1 and 3 are populated;
 * the IDB's dds3TrackPolyVCSet4Table is entry 9. */
void (*VCSetFunc[16])(void *, void *, void *) = {
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet_Normal,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet_Acute,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet_AcuteW,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet2_Normal,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet2_Acute,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet2_AcuteW,
    NULL,
    NULL,
    NULL,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet4_Normal,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet4_Acute,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet4_AcuteW,
};
sdfGrOt_t *dds3TrackPolyOtTable[4] = {
    &sdfGrOtTable[51],
    &sdfGrOtTable[51],
    &sdfGrOtTable[55],
    &sdfGrOtTable[56],
};
f32 dds3SpPolyColorTo[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
f32 dds3SpPolyColorFrom[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
dds3SpPolyEntry_t dds3SpPolyTable[4] = {
    { (dds3SpPolyCreateFunc_t)dds3SpPoly_Ring_Old_Create, (dds3SpPolyWorkFunc_t)dds3SpPoly_Ring_Old_Draw,
      (dds3SpPolyWorkFunc_t)dds3SpPoly_Ring_Old_Delete },
    { (dds3SpPolyCreateFunc_t)dds3SpPoly_RingCreate, (dds3SpPolyWorkFunc_t)dds3SpPoly_RingDraw,
      (dds3SpPolyWorkFunc_t)dds3SpPoly_RingDelete },
    { (dds3SpPolyCreateFunc_t)dds3SpPoly_SphereRingCreate, (dds3SpPolyWorkFunc_t)dds3SpPoly_SphereRingDraw,
      (dds3SpPolyWorkFunc_t)dds3SpPoly_SphereRingDelete },
    { (dds3SpPolyCreateFunc_t)dds3SpPoly_RollRingCreate, (dds3SpPolyWorkFunc_t)dds3SpPoly_RollRingDraw,
      (dds3SpPolyWorkFunc_t)dds3SpPoly_RollRingDelete },
};
void (*dds3SpPolyResetTable[4])(void *work) = {
    (dds3SpPolyWorkFunc_t)dds3SpPoly_Ring_Old_BuffInit,
    (dds3SpPolyWorkFunc_t)dds3SpPoly_RingAllBuffInit,
    (dds3SpPolyWorkFunc_t)dds3SpPoly_SphereRingAllBuffInit,
    (dds3SpPolyWorkFunc_t)dds3SpPoly_RollRingAllBuffInit,
};
void (*dds3SpPolySetScaleTable[4])(void *work, f32 scale) = {
    (dds3SpPolyScaleFunc_t)dds3SpPoly_Ring_Old_SetScale,
    (dds3SpPolyScaleFunc_t)dds3SpPoly_RingSetScale,
    (dds3SpPolyScaleFunc_t)dds3SpPoly_SphereRingSetScale,
    (dds3SpPolyScaleFunc_t)dds3SpPoly_RollRingSetScale,
};
INCLUDE_SDATA(const s32, "effect/polyManager", hDrawListTop);

INCLUDE_SDATA(const s32, "effect/polyManager", dds3TrackPolyTag);

