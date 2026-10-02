#include "common.h"
#include "pcp_vu0.h"

typedef struct PcpScatterRes PcpScatterRes;

extern PcpScatterRes *effPcpScatterResCreate(u32);
extern PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *);
extern void effPcpScatterResRelease(PcpScatterRes *);

/* Drawable allocation is 0x80 bytes; its resource and geometry arrays are
 * independent of the effect instance that supplies the transform and scale. */
typedef struct PcpScatterDraw {
    f32 origin[4];
    f32 matrix[16];
    u32 unk50;
    u32 color;
    u32 particleCount;
    s32 stride;
    f32 scale;
    f32 *points;
    f32 *uv;
    u32 *vertexColors;
    u32 *colors;
    u32 asset;
    u32 allocation;
    PcpScatterRes *sharedResource;
} PcpScatterDraw;

typedef struct PcpScatterPlainParams {
    f32 vec[4];
    u32 unk10;
    u8 loop;
    u8 pad15[3];
    s32 duration;
    u32 particleCount;
    u32 unk20;
    u32 randomDelayRange;
    s32 fadeIn;
    s32 fadeRange;
    f32 angleStepBase;
    f32 angleStepJitter;
    f32 heightBase;
    f32 heightJitter;
    f32 angularSpeed;
    f32 angularDamping;
    f32 radiusBase;
    f32 radiusJitter;
    f32 radialSpeed;
    f32 radialDamping;
    s32 radialDecayStart;
    s32 colorParam;
    u32 vCount;
    u32 vTail;
    u8 pad68[0x80];
} PcpScatterPlainParams;

typedef struct PcpScatterPlainParticle {
    f32 rot[3];
    s32 age;
    f32 angle;
    f32 angularSpeed;
    f32 angleStep;
    f32 radius;
    f32 radialSpeed;
    f32 height;
} PcpScatterPlainParticle;

/* The four setter callbacks belong to the flat-ring effect's 0x13C-byte
 * instance, not to the drawable used by the resource helpers below. */
typedef struct PcpScatterPlainInstance {
    f32 matrix[16];
    PcpScatterPlainParams params;
    PcpScatterPlainParticle *particles;
    f32 scale;
    u32 color;
    u32 scatterObject;
    u32 ownedBuffer;
} PcpScatterPlainInstance;

/* Copy the flat effect's source vector; the renderer reads the embedded copy. */
void func_0017D758(PcpScatterPlainInstance *work, void *src) {
    PCP_COPY_VECTOR(work->params.vec, src);
}

/* Set the instance scale that its update forwards to the drawable. */
void func_0017D770(PcpScatterPlainInstance *work, f32 value) {
    work->scale = value;
}

/* Set the instance color used by the particle fade pass. */
void func_0017D778(PcpScatterPlainInstance *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix through vf28-vf31 */
void func_0017D780(PcpScatterPlainInstance *work, void *src) {
    VU0_COPY_MATRIX(work->matrix, src);
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D7A8);

/* Release the shared scatter resource before the object's private assets. */
void effReleaseScatterObject(PcpScatterDraw *object) {
    if (object->sharedResource != 0) {
        effPcpScatterResRelease(object->sharedResource);
    }
    sdfQueueAssetRelease(object->asset);
    func_003297C8(object->allocation);
    sdfReleaseChipBlock(object);
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DA28);

/* Give this object its own reference to a newly created scatter resource. */
void effCreateScatterResource(PcpScatterDraw *object, u32 resource) {
    PcpScatterRes *shared;

    shared = effPcpScatterResCreate(resource);
    object->sharedResource = shared;
}

/* Share another object's scatter resource while retaining a separate reference. */
void effShareScatterResource(PcpScatterDraw *object, PcpScatterDraw *source) {
    PcpScatterRes *shared;

    shared = effPcpScatterResAddRef(source->sharedResource);
    object->sharedResource = shared;
}

/* Compute the address of a 16-byte-wide block within the stride. */
s32 effGetScatterWideBlock(PcpScatterDraw *object, s32 index) {
    return (s32)object->points + index * object->stride * 0x10;
}

/* Compute the address of an 8-byte-wide block within the stride. */
s32 effGetScatterNarrowBlock(PcpScatterDraw *object, s32 index) {
    return (s32)object->uv + index * object->stride * 8;
}

u32 effGetScatterEntry(PcpScatterDraw *object, s32 index) {
    return object->colors[index];
}

/* vu0 routine: copy a 4x4 matrix into the destination's second slot */
void effScatterStoreSourceTransformMatrix(PcpScatterDraw *object, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(object->matrix);
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DD50);
