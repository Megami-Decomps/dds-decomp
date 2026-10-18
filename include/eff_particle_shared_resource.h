#ifndef EFF_PARTICLE_SHARED_RESOURCE_H
#define EFF_PARTICLE_SHARED_RESOURCE_H

#include "common.h"
#include "eff_curve.h"
#include "sdf.h"

struct BillObj;
struct EffExpandedList;

/* The +0x0C bytes (colorTrack through y) are copied from the file. */
#define EFF_PARTICLE_SHARED_PAYLOAD_BYTES 0x98

/* DDS1 0029C530 / DDS2 002DE248 allocate this 0xAC-byte shared resource.
 * The trailing pointers are owned billboard/reference handles managed by the
 * resource lifecycle. */
typedef struct EffParticleSharedResource {
    u32 color;
    u32 option;
    s32 state;                      /* 0x08: frames drawn so far */
    SdfColorTrack colorTrack;       /* 0x0C */
    SdfAlphaTrack alphaTrack;       /* 0x30: surfaceIndex selects the blend (0..6) */
    EffScalarTrack scaleTrack;      /* 0x40 */
    EffScalarTrack rotationTrack;   /* 0x6C */
    s32 limit;                      /* 0x98: frame count, 0 = unlimited */
    s32 x;                          /* 0x9C */
    s32 y;                          /* 0xA0 */
    struct BillObj *billHandle;     /* 0xA4 */
    struct EffExpandedList *reference; /* 0xA8 */
} EffParticleSharedResource;

typedef char EffParticleSharedResourceSizeCheck[
    (sizeof(EffParticleSharedResource) == 0xAC) ? 1 : -1];
typedef char EffParticleSharedResourcePayloadOffsetCheck[
    ((u32)&((EffParticleSharedResource *)0)->colorTrack == 0x0C) ? 1 : -1];
typedef char EffParticleSharedResourceBillOffsetCheck[
    ((u32)&((EffParticleSharedResource *)0)->billHandle == 0xA4) ? 1 : -1];
typedef char EffParticleSharedResourceReferenceOffsetCheck[
    ((u32)&((EffParticleSharedResource *)0)->reference == 0xA8) ? 1 : -1];

#endif
