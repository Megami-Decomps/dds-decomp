#include "common.h"
#include "sdf_vu0.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "pcp_vu0.h"
#include "sdf_projection.h"
#include "sdf_vu_matrix.h"

typedef struct {
    f32 components[4];
} CameraVector;


extern void dds3FreePoseSet(dds3Basic_t *node);
extern void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *base);

/* Release the camera's inner node, base handle, and owned data block. */
void dds3DestroyCameraData(dds3Basic_t *camera) {
    CameraData *cameraData;

    dds3FreePoseSet(camera);
    cameraData = ((CameraData *)camera->data);
    dds3ObjectBaseFreeControlWorkData(cameraData->handle);
    sdfChipFreeDirect(cameraData);
}
#include "dds3_world_secondary.h"
u8 dds3CheckPoseSetFlag(dds3PoseSet_t *node, u32 flags);
void dds3OffPoseSetFlag(dds3PoseSet_t *node, u32 flags);
extern void dds3SetRadiusPoseSet(dds3PoseSet_t *inner);
extern void MakeCamera(dds3Basic_t *camera);
extern f32 aViewTarget[4];
extern f32 sdfViewTargetVector[4];
extern f32 aViewDown[4];

/* Rebuild dirty vectors and publish only the active world's camera; return 1.
 * A pending field-of-view update is consumed only while this camera is active. */
s32 dds3UpdateCameraObject(dds3Basic_t *camera) {
    dds3PoseSet_t *inner;
    CameraData *data;

    inner = camera->inner;
    data = ((CameraData *)camera->data);
    if (dds3CheckPoseSetFlag(inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) == 1) {
        dds3OffPoseSetFlag(inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        MakeCamera(camera);
        dds3SetRadiusPoseSet(inner);
    }
    if (dds3GetActiveCameraSceneBasic(dds3GetControlScene()) == camera) {
        SDF_VU0_COPYVECTOR(sdfViewTargetVector, &inner->position);
        SDF_VU0_COPYVECTOR(aViewTarget, &data->worldEye);
        SDF_VU0_COPYVECTOR(aViewDown, &data->worldUp);
        if (data->fovUpdatePending & 1) {
            dds3SetScreenFovy(data->fieldOfView);
            data->fovUpdatePending &= ~1;
        }
    }
    return 1;
}

/* The camera draw callback has no geometry to submit. */
s32 dds3DrawCameraObject(dds3Basic_t *camera) {
    return 1;
}


/* Transform the local eye/up vectors through the inner node and rebuild the
 * world-space look-at basis. */
void MakeCamera(dds3Basic_t *camera) {
    CameraData *data = camera->data;
    dds3PoseSet_t *inner = camera->inner;

    SDF_VU0_LOADVECTOR(&inner->rotation);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_LOADVECTOR(&data->localUp);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&data->worldUp);
    if (data->eyeIsRelative == 1) {
        SDF_VU0_LOADVECTOR(&data->localEyeOffset);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&inner->position);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&data->worldEye);
    }
    LookAtMatrix((const f32 *)&data->worldEye, inner->position, (const f32 *)&data->worldUp);
    VU0_STORE_MATRIX(data->matrix);
}

/* Return the base handle owned by the camera data. */
dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *camera) {
    return ((CameraData *)camera->data)->handle;
}


extern void dds3SetRadiusPoseSet(dds3PoseSet_t *inner);
extern void MakeCamera(dds3Basic_t *camera);
extern CameraVector D_00412878;
extern CameraVector D_00412888;


/* Append a camera-kind node, set its key, and ensure slot data. */
dds3Basic_t *dds3AllocCameraObjectBasic(s32 key) {
    dds3Basic_t *obj = dds3AddSceneBasicData(4);

    obj->key = key;
    dds3ObjectBaseCreateMover(obj);
    return obj;
}

/* Create a camera with the default local eye/up vectors and relative eye mode. */
dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 key, void *targetPosition, void *rotation) {
    CameraVector initialUp;
    CameraVector initialEyeOffset;
    dds3Basic_t *camera;
    CameraData *data;

    initialUp = D_00412878;
    initialEyeOffset = D_00412888;
    camera = dds3AllocCameraObjectBasic(key);
    data = ((CameraData *)camera->data);
    data->eyeIsRelative = 1;
    data->fieldOfView = 0.6283185f;
    dds3SetRotPoseSet(camera, rotation);
    dds3SetPosPoseSet(camera, targetPosition);
    dds3SetRadiusPoseSet(camera->inner);
    SDF_VU0_COPYVECTOR(&data->localUp, &initialUp);
    SDF_VU0_COPYVECTOR(&data->localEyeOffset, &initialEyeOffset);
    MakeCamera(camera);
    return camera;
}

/* Create a camera with an explicit world eye and an up vector rotated by its node. */
dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(s32 key, void *targetPosition, u128 *worldEye, u128 *localUp) {
    dds3Basic_t *obj = dds3AllocCameraObjectBasic(key);
    CameraData *data = obj->data;

    data->fieldOfView = 0.6283185f;
    data->eyeIsRelative = 0;
    dds3SetPosPoseSet(obj, targetPosition);
    dds3SetRadiusPoseSet(obj->inner);
    SDF_VU0_COPYVECTOR(&data->localUp, localUp);
    SDF_VU0_COPYVECTOR(&data->worldEye, worldEye);
    MakeCamera(obj);
    return obj;
}

/* Store the eye vector as a local offset; flag 0 also seeds the world eye.
 * The basis rebuild transforms that offset only when eyeIsRelative is 1. */
dds3Basic_t *dds3AddCameraObjectBasic(s32 key, f32 fieldOfView, void *targetPosition, u128 *eyeVector, u128 *localUp, s32 eyeIsRelative) {
    dds3Basic_t *obj = dds3AllocCameraObjectBasic(key);
    CameraData *data = obj->data;

    data->fieldOfView = fieldOfView;
    data->eyeIsRelative = eyeIsRelative;
    dds3SetPosPoseSet(obj, targetPosition);
    dds3SetRadiusPoseSet(obj->inner);
    SDF_VU0_COPYVECTOR(&data->localUp, localUp);
    SDF_VU0_COPYVECTOR(&data->localEyeOffset, eyeVector);
    if (eyeIsRelative == 0) {
        SDF_VU0_COPYVECTOR(&data->worldEye, eyeVector);
    }
    MakeCamera(obj);
    return obj;
}

void dds3DeleteCameraObjectBasic(dds3Basic_t *node) {
    dds3DeleteSceneBasicData(node);
}

/* Load the owned look-at matrix into vf28-vf31. */
void dds3LoadObjectMatrixPointerIntoVu(dds3Basic_t *camera) {
    VU0_LOAD_MATRIX(((CameraData *)camera->data)->matrix);
}

/* Copy the supplied vector to the camera's world-space eye position. */
void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, u128 *worldEye) {
    SDF_VU0_COPYVECTOR(&((CameraData *)camera->data)->worldEye, worldEye);
}

/* Return the camera's world-space eye vector in vf10. */
void dds3CameraObjectBasicGetTargetPos(dds3Basic_t *camera) {
    u128 *eye = &((CameraData *)camera->data)->worldEye;

    SDF_VU0_LOADVECTOR_BARRIER(eye);
}

/* Cache a field of view in radians and request its next active-camera update. */
void dds3CameraObjectBasicSetFovy(dds3Basic_t *camera, f32 fieldOfView) {
    CameraData *state = camera->data;
    state->fieldOfView = fieldOfView;
    state->fovUpdatePending |= 1;
}

/* Return the cached field of view in radians. */
f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *camera) {
    return ((CameraData *)camera->data)->fieldOfView;
}


/* vu0 routine: update world up and the relative eye, then copy world eye and
 * target position into the four-component output buffers. */
void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *obj, f32 *worldEyeOut, f32 *targetPositionOut) {
    CameraData *data = obj->data;
    dds3PoseSet_t *inner = obj->inner;

    SDF_VU0_LOADVECTOR(&inner->rotation);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_LOADVECTOR(&data->localUp);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&data->worldUp);
    if (data->eyeIsRelative == 1) {
        SDF_VU0_LOADVECTOR(&data->localEyeOffset);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&inner->position);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&data->worldEye);
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

