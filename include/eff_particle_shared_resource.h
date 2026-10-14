#ifndef EFF_PARTICLE_SHARED_RESOURCE_H
#define EFF_PARTICLE_SHARED_RESOURCE_H

#include "common.h"

struct BillObj;
struct EffExpandedList;

/* DDS1 0029C530 / DDS2 002DE248 allocate this 0xAC-byte shared resource.
 * The copied +0x0C payload remains opaque; the trailing pointers are owned
 * billboard/reference handles managed by the resource lifecycle. */
typedef struct EffParticleSharedResource {
    u32 color;
    u32 option;
    u32 opaque08;
    u8 opaquePayload[0x98];
    struct BillObj *billHandle;
    struct EffExpandedList *reference;
} EffParticleSharedResource;

typedef char EffParticleSharedResourceSizeCheck[
    (sizeof(EffParticleSharedResource) == 0xAC) ? 1 : -1];
typedef char EffParticleSharedResourcePayloadOffsetCheck[
    ((u32)&((EffParticleSharedResource *)0)->opaquePayload == 0x0C) ? 1 : -1];
typedef char EffParticleSharedResourceBillOffsetCheck[
    ((u32)&((EffParticleSharedResource *)0)->billHandle == 0xA4) ? 1 : -1];
typedef char EffParticleSharedResourceReferenceOffsetCheck[
    ((u32)&((EffParticleSharedResource *)0)->reference == 0xA8) ? 1 : -1];

#endif
