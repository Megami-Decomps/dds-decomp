#ifndef EFF_UNIT_OBJECT_H
#define EFF_UNIT_OBJECT_H

#include "common.h"
#include "eff.h"

#define EFF_INSTANCE_BYTES 0x88
#define EFF_MATRIX_BYTES 0x40

/* An effect unit instance (EFF_INSTANCE_BYTES): each effect scene object owns
 * its matrices, a billboard and an asset reference. */
typedef struct EffUnitObject {
    f32 localMatrix[4][4]; /* 0x00: VU0 matrix initialized on clone. */
    f32 transform[4][4]; /* 0x40: VU0 matrix copied by effCopyMatrixToNext. */
    BillObj *billboard; /* 0x80: scene-owned billboard */
    void *resource; /* 0x84 */
} EffUnitObject;
typedef char EffUnitObject_size_check[(sizeof(EffUnitObject) == EFF_INSTANCE_BYTES) ? 1 : -1];
typedef char EffUnitObject_localMatrix_offset_check[
    ((u32)&((EffUnitObject *)0)->localMatrix == 0) ? 1 : -1];
typedef char EffUnitObject_transform_offset_check[
    ((u32)&((EffUnitObject *)0)->transform == EFF_MATRIX_BYTES) ? 1 : -1];
typedef char EffUnitObject_billboard_offset_check[
    ((u32)&((EffUnitObject *)0)->billboard == 0x80) ? 1 : -1];
typedef char EffUnitObject_resource_offset_check[
    ((u32)&((EffUnitObject *)0)->resource == 0x84) ? 1 : -1];

#endif
