#ifndef FLD_COLLISION_GEOMETRY_H
#define FLD_COLLISION_GEOMETRY_H

#include "fld.h"

/* Six-word runtime view used by both file-loaded and generated geometry. */
typedef struct FldCollisionDescriptor {
    s32 vertexCount;
    s32 faceCount;
    s32 triangleCount;
    f32 (*vertices)[4];
    FldCollisionFace *faces;
    void *stopData;
} FldCollisionDescriptor;

/* Temporary axis-aligned walls own their vertex/face arrays inline. */
typedef struct FldWallMeshRecord {
    f32 vertices[8][4];
    FldCollisionFace faces[4];
    FldCollisionDescriptor geometry;
} FldWallMeshRecord;

typedef char FldCollisionDescriptor_size_must_be_0x18[
    (sizeof(FldCollisionDescriptor) == 0x18) ? 1 : -1];
typedef char FldCollisionDescriptor_vertices_offset_must_be_0x0C[
    ((u32)&((FldCollisionDescriptor *)0)->vertices == 0x0C) ? 1 : -1];
typedef char FldCollisionDescriptor_faces_offset_must_be_0x10[
    ((u32)&((FldCollisionDescriptor *)0)->faces == 0x10) ? 1 : -1];
typedef char FldCollisionDescriptor_stopData_offset_must_be_0x14[
    ((u32)&((FldCollisionDescriptor *)0)->stopData == 0x14) ? 1 : -1];
typedef char FldWallMeshRecord_size_must_be_0x128[
    (sizeof(FldWallMeshRecord) == 0x128) ? 1 : -1];
typedef char FldWallMeshRecord_faces_offset_must_be_0x80[
    ((u32)&((FldWallMeshRecord *)0)->faces == 0x80) ? 1 : -1];
typedef char FldWallMeshRecord_geometry_offset_must_be_0x110[
    ((u32)&((FldWallMeshRecord *)0)->geometry == 0x110) ? 1 : -1];

#endif /* FLD_COLLISION_GEOMETRY_H */
