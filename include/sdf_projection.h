#ifndef SDF_PROJECTION_H
#define SDF_PROJECTION_H

#include "common.h"

/* Native 0x90-byte camera/viewport owner shared by both projection builders. */
typedef struct SdfCamera {
    u32 flags;       // 0x00: 1 half-height, 2 field-of-view projection
    f32 aspect;      // 0x04
    f32 scale;       // 0x08
    f32 fov;         // 0x0C
    f32 offsetX;     // 0x10
    f32 offsetY;     // 0x14
    f32 width;       // 0x18
    f32 height;      // 0x1C
    f32 top;         // 0x20
    f32 bottom;      // 0x24
    f32 nearZ;       // 0x28
    f32 farZ;        // 0x2C
    u8 matrix[0x40]; // 0x30
    f32 halfWidth;   // 0x70
    f32 halfHeight;  // 0x74
    f32 centerY;     // 0x78
    f32 one;         // 0x7C
    f32 originX;     // 0x80
    f32 originY;     // 0x84
    f32 bottomY;     // 0x88
    u32 zero;        // 0x8C
} SdfCamera;

typedef char SdfCamera_size_must_be_0x90[(sizeof(SdfCamera) == 0x90) ? 1 : -1];

/* Complete static projection record, including the inverse-origin cache. */
typedef struct SdfProjectionRecord {
    SdfCamera camera;
    f32 inverseOrigin[4]; /* 0x90 */
} SdfProjectionRecord;

typedef char SdfProjectionRecord_size_must_be_0xA0[(sizeof(SdfProjectionRecord) == 0xA0) ? 1 : -1];
typedef char SdfProjectionRecord_inverseOrigin_must_be_0x90[
    ((u32)&((SdfProjectionRecord *)0)->inverseOrigin == 0x90) ? 1 : -1];

extern SdfProjectionRecord sdfSceneProjectionParameters;
#ifdef VERSION_DDS1
extern SdfProjectionRecord D_003247B0;
extern SdfProjectionRecord D_00324980;
#elif defined(VERSION_DDS2)
extern SdfProjectionRecord D_0037F7B0;
extern SdfProjectionRecord D_0037F980;
#endif
extern void sdfCameraBuildProjection(SdfCamera *);
struct ConsMatrixPacket;
extern void sdfConsBuildMatrixPacket(struct ConsMatrixPacket *, SdfProjectionRecord *, void *);
extern void sdfConsCacheTransformedNode(SdfProjectionRecord *, void *);

#endif /* SDF_PROJECTION_H */
