#include "common.h"
#include "dds3obj.h"
#include "pcp_vu0.h"

typedef struct {
    f32 components[4];
} CameraVector;


extern void effObjFreeInner(EffWorldNode *node);
extern void dds3DestroyObjectBase(ObjBase *base);
extern void dds3RemoveWorldObjectNode(EffWorldNode *node);
extern void sdfReleaseChipBlock(void *block);

/* Release the camera's inner node, base handle, and owned data block. */
void dds3DestroyCameraData(EffWorldNode *camera) {
    CameraData *data;

    effObjFreeInner(camera);
    data = ((CameraData *)camera->data);
    dds3DestroyObjectBase(data->handle);
    sdfReleaseChipBlock(data);
}

EffWorldNode *dds3GetWorldCameraObject(EffWorldNode *world);
void *dds3GetWorldSecondaryObject(void);
u8 effObjTestNodeFlags(ObjectTransform *node, u32 flags);
void effObjClearNodeFlags(ObjectTransform *node, u32 flags);
extern void effObjInnerVecBackup(ObjectTransform *inner);
extern void dds3RebuildCameraBasis(EffWorldNode *obj);
extern void func_001063A8(f32 value);
extern f32 sdfViewEyeVector[4];
extern f32 sdfViewTargetVector[4];
extern f32 sdfViewUpVector[4];

/* Rebuild dirty vectors and publish only the active world's camera; return 1.
 * A pending field-of-view update is consumed only while this camera is active. */
s32 dds3UpdateCameraObject(EffWorldNode *camera) {
    ObjectTransform *inner;
    CameraData *data;

    inner = camera->inner;
    data = ((CameraData *)camera->data);
    if (effObjTestNodeFlags(inner, 1) == 1) {
        effObjClearNodeFlags(inner, 1);
        dds3RebuildCameraBasis(camera);
        effObjInnerVecBackup(inner);
    }
    if (dds3GetWorldCameraObject(dds3GetWorldSecondaryObject()) == camera) {
        PCP_COPY_VECTOR(sdfViewTargetVector, &inner->position);
        PCP_COPY_VECTOR(sdfViewEyeVector, &data->worldEye);
        PCP_COPY_VECTOR(sdfViewUpVector, &data->worldUp);
        if (data->fovUpdatePending & 1) {
            func_001063A8(data->fieldOfView);
            data->fovUpdatePending &= ~1;
        }
    }
    return 1;
}

/* The camera draw callback has no geometry to submit. */
s32 dds3DrawCameraObject(EffWorldNode *camera) {
    return 1;
}

extern void effMiscQuaternionToMatrixVU(void);
extern void sdfVuBuildLookAtBasis(void *target, void *origin, void *up);

/* Transform the local eye/up vectors through the inner node and rebuild the
 * world-space look-at basis. */
void dds3RebuildCameraBasis(EffWorldNode *obj) {
    CameraData *data = obj->data;
    ObjectTransform *inner = obj->inner;

    VU0_LOAD_VF(vf10, &inner->rotation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, &data->localUp);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, &data->worldUp);
    if (data->eyeIsRelative == 1) {
        VU0_LOAD_VF(vf10, &data->localEyeOffset);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, &inner->position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, &data->worldEye);
    }
    sdfVuBuildLookAtBasis(&data->worldEye, &inner->position, &data->worldUp);
    VU0_STORE_MATRIX(data->matrix);
}

/* Return the base handle owned by the camera data. */
ObjBase *dds3GetCameraHandle(EffWorldNode *camera) {
    return ((CameraData *)camera->data)->handle;
}

extern EffWorldNode *dds3AppendWorldObjectNode(s32 kind);

extern void dds3EnsureSlotData(void *object);
extern void effObjSetInnerFirstVec(EffWorldNode *obj, u128 *vec);
extern void effObjSetInnerSecondVec(EffWorldNode *obj, u128 *vec);
extern void effObjInnerVecBackup(ObjectTransform *inner);
extern void dds3RebuildCameraBasis(EffWorldNode *obj);
extern CameraVector D_00412878;
extern CameraVector D_00412888;


/* Append a camera-kind node, set its key, and ensure slot data. */
EffWorldNode *dds3CreateCameraObjectWithSlotData(s32 key) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(4);

    obj->key = key;
    dds3EnsureSlotData(obj);
    return obj;
}

/* Create a camera with the default local eye/up vectors and relative eye mode. */
EffWorldNode *dds3CreateCameraObject(s32 key, void *targetPosition, void *rotation) {
    CameraVector initialUp;
    CameraVector initialEyeOffset;
    EffWorldNode *camera;
    CameraData *data;

    initialUp = D_00412878;
    initialEyeOffset = D_00412888;
    camera = dds3CreateCameraObjectWithSlotData(key);
    data = ((CameraData *)camera->data);
    data->eyeIsRelative = 1;
    data->fieldOfView = 0.6283185f;
    effObjSetInnerSecondVec(camera, rotation);
    effObjSetInnerFirstVec(camera, targetPosition);
    effObjInnerVecBackup(camera->inner);
    PCP_COPY_VECTOR(&data->localUp, &initialUp);
    PCP_COPY_VECTOR(&data->localEyeOffset, &initialEyeOffset);
    dds3RebuildCameraBasis(camera);
    return camera;
}

/* Create a camera with an explicit world eye and an up vector rotated by its node. */
EffWorldNode *dds3CreateConfiguredCameraObject(s32 key, void *targetPosition, u128 *worldEye, u128 *localUp) {
    EffWorldNode *obj = dds3CreateCameraObjectWithSlotData(key);
    CameraData *data = obj->data;

    data->fieldOfView = 0.6283185f;
    data->eyeIsRelative = 0;
    effObjSetInnerFirstVec(obj, targetPosition);
    effObjInnerVecBackup(obj->inner);
    PCP_COPY_VECTOR(&data->localUp, localUp);
    PCP_COPY_VECTOR(&data->worldEye, worldEye);
    dds3RebuildCameraBasis(obj);
    return obj;
}

/* Store the eye vector as a local offset; flag 0 also seeds the world eye.
 * The basis rebuild transforms that offset only when eyeIsRelative is 1. */
EffWorldNode *dds3CreateCameraObjectWithVectors(s32 key, f32 fieldOfView, void *targetPosition, u128 *eyeVector, u128 *localUp, s32 eyeIsRelative) {
    EffWorldNode *obj = dds3CreateCameraObjectWithSlotData(key);
    CameraData *data = obj->data;

    data->fieldOfView = fieldOfView;
    data->eyeIsRelative = eyeIsRelative;
    effObjSetInnerFirstVec(obj, targetPosition);
    effObjInnerVecBackup(obj->inner);
    PCP_COPY_VECTOR(&data->localUp, localUp);
    PCP_COPY_VECTOR(&data->localEyeOffset, eyeVector);
    if (eyeIsRelative == 0) {
        PCP_COPY_VECTOR(&data->worldEye, eyeVector);
    }
    dds3RebuildCameraBasis(obj);
    return obj;
}

void dds3ReleaseCameraWorldNode(EffWorldNode *node) {
    dds3RemoveWorldObjectNode(node);
}

/* Load the owned look-at matrix into vf28-vf31. */
void dds3LoadObjectMatrixPointerIntoVu(EffWorldNode *camera) {
    VU0_LOAD_MATRIX(((CameraData *)camera->data)->matrix);
}

/* Copy the supplied vector to the camera's world-space eye position. */
void dds3SetCameraVector(EffWorldNode *camera, u128 *worldEye) {
    PCP_COPY_VECTOR(&((CameraData *)camera->data)->worldEye, worldEye);
}

/* Return the camera's world-space eye vector in vf10. */
void dds3LoadCameraVectorVU(EffWorldNode *camera) {
    u128 *eye = &((CameraData *)camera->data)->worldEye;

    VU0_LOAD_VF_MEMORY(vf10, eye);
}

/* Cache a field of view in radians and request its next active-camera update. */
void dds3SetCameraFieldOfView(EffWorldNode *camera, f32 fieldOfView) {
    CameraData *state = camera->data;
    state->fieldOfView = fieldOfView;
    state->fovUpdatePending |= 1;
}

/* Return the cached field of view in radians. */
f32 dds3GetCameraFieldOfView(EffWorldNode *camera) {
    return ((CameraData *)camera->data)->fieldOfView;
}

extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: update world up and the relative eye, then copy world eye and
 * target position into the four-component output buffers. */
void dds3TransformCameraVectorsByInnerRotation(EffWorldNode *obj, f32 *worldEyeOut, f32 *targetPositionOut) {
    CameraData *data = obj->data;
    ObjectTransform *inner = obj->inner;

    VU0_LOAD_VF(vf10, &inner->rotation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, &data->localUp);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, &data->worldUp);
    if (data->eyeIsRelative == 1) {
        VU0_LOAD_VF(vf10, &data->localEyeOffset);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, &inner->position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, &data->worldEye);
    }
    worldEyeOut[0] = ((f32 *)&data->worldEye)[0];
    worldEyeOut[1] = ((f32 *)&data->worldEye)[1];
    worldEyeOut[2] = ((f32 *)&data->worldEye)[2];
    worldEyeOut[3] = ((f32 *)&data->worldEye)[3];
    targetPositionOut[0] = inner->position[0];
    targetPositionOut[1] = inner->position[1];
    targetPositionOut[2] = inner->position[2];
    targetPositionOut[3] = inner->position[3];
}

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412878);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412888);

