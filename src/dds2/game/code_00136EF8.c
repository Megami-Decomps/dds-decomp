#include "fld_area_work.h"
#include "common.h"
#include "fld_stop_entry.h"
#include "sdf_textured_rect.h"
#include "sdf_chip.h"
#include "sdf_dev_state.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "fld_waypoint.h"
#include "fld_npc_motion.h"
#include "fld_inf.h"
#include "fld_title_banner_menu.h"
#include "kwln_task_lifecycle.h"
#include "sdf_vu_matrix.h"
#include "kwln_fade.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "evt_world_source_transform.h"
#include "fld.h"
#include "fld_resource_resolver.h"
#include "fld_collision_geometry.h"
#include "evt_world.h"
#include "scr.h"
#include "sdf.h"
#include "sdf_texture_file.h"
#include "dds3_world_primary.h"
#include "mdl_flag_api.h"
#include "dds3_world_secondary.h"
#include "fld_trigger_shape.h"
#include "fld_room_state.h"
#include "fld_action_row.h"
#include "dds3_world_counter.h"
/* Declared before first use (were implicit int). */
extern void func_00136EF8(void);

extern void evtSetDrawSurfaceIndex();

extern void evtSubmitPrimaryGsTest();

extern void evtSubmitPrimaryAlphaBlendMode();

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern SdfTex *fldRainTextureReference;

extern u32 fldAuxRecordBuffer;

extern u32 fldValueRecords;

extern u32 fldAuxRecordResource;

extern u32 fldValueRecordResource;

extern void *memset(void *s, s32 c, u32 n);

extern s32 fldRainTextureResource;

extern char D_00413350[];

extern u32 fldRainTextureData;

extern FldCameraSetting *fldCameraSettings;

extern SdfFlagListParams fldCameraColorParameters[];

extern s32 fldValueRecordCount;

extern s32 D_00436188;

extern s32 D_0043617C;

extern f32 fldDotVector(f32 *, f32 *);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00136EF8);

void fldInitializeCameraColorResource(void) {
    fldRainTextureResource = sdfReadNamedResource(D_00413350, &fldRainTextureData, 0);
    fldRainTextureReference = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(fldRainTextureData));
    fldCameraColorEffect = effCreateSelectionFlagListFromWork(fldCameraColorParameters);
    if (fldRainTextureResource != 0) {
        sdfMemFreeDelayedDirect((struct SdfMemBlock *)fldRainTextureResource);
        fldRainTextureResource = 0;
    }
    fldUpdateCameraColorEffect(fldCameraSettings);
    fldCameraColorEnabled = 1;
}

void func_00137888(void) {
    func_00136EF8();
}

void fldReleaseCameraColorEffect(void) {
    fldCameraColorEnabled = 0;
    if (fldRainTextureReference != 0) {
        sdfTexReleaseReferenceViaHandler(fldRainTextureReference);
        fldRainTextureReference = 0;
    }
    if (fldCameraColorEffect != 0) {
        effReleaseSelectionFlagList(fldCameraColorEffect);
        fldCameraColorEffect = 0;
    }
}

extern FldCameraSetting fldAppliedCameraSettings[];

void fldCopyCameraSetting(FldCameraSetting *destination) {
    *destination = *fldAppliedCameraSettings;
}

void fldUpdateCameraColorEffect(FldCameraSetting *setting) {
    FldColorParams *color = &setting->color;

    if (color->enabled != 0) {
        fldCameraColorParameters->color.finalColor = fldCameraColorParameters->color.initialColor =
            (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
        fldCameraColorParameters->alpha.alpha = color->vectorY;
        switch (color->mode) {
        case 0:
            fldCameraColorParameters->alpha.surfaceIndex = 1;
            break;
        case 1:
            fldCameraColorParameters->alpha.surfaceIndex = 2;
            break;
        default:
            fldCameraColorParameters->alpha.surfaceIndex = 3;
            break;
        }
        fldCameraColorParameters->count = color->slotIndex;
        fldCameraColorParameters->speed = color->vectorZ;
        itfCopyColorFields(fldCameraColorEffect, fldCameraColorParameters);
    }
    *fldAppliedCameraSettings = *setting;
}

/* Keep both the resource handles and retained addresses: callers use the
 * retained storage, whereas the handles are needed at release time. */
void fldHitAlloc(void) {
    struct SdfMemBlock *allocation = sdfMemAllocDirect(FIELD_VALUE_RECORD_STORAGE_SIZE);
    void *storage;

    fldValueRecordResource = (u32)allocation;
    storage = (void *)sdfMemLock(allocation);
    fldValueRecords = (u32)storage;
    memset(storage, 0, FIELD_VALUE_RECORD_STORAGE_SIZE);
    allocation = sdfMemAllocDirect(FIELD_AUX_RECORD_STORAGE_SIZE);
    fldAuxRecordResource = (u32)allocation;
    storage = (void *)sdfMemLock(allocation);
    fldAuxRecordBuffer = (u32)storage;
    memset(storage, 0, FIELD_AUX_RECORD_STORAGE_SIZE);
}

/* Release both retained resources and clear the usable buffer addresses. */
void fldHitFree(void) {
    sdfMemUnlock((struct SdfMemBlock *)fldValueRecordResource);
    sdfMemFreeDelayedDirect((struct SdfMemBlock *)fldValueRecordResource);
    fldValueRecords = 0;
    sdfMemUnlock((struct SdfMemBlock *)fldAuxRecordResource);
    sdfMemFreeDelayedDirect((struct SdfMemBlock *)fldAuxRecordResource);
    fldAuxRecordBuffer = 0;
}

/* Dot product of XYZ only; any fourth vector component is ignored. */
float fldDotVector(float *left, float *right) {
    return *left * *right + left[1] * right[1] + left[2] * right[2];
}

/* Return the Euclidean XYZ length without normalizing the input vector. */
f32 fldCalculateVectorLength(const f32 *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

/* Build a normalized plane from three vertices spaced four floats apart.
 * Snap near-zero normal components before deriving D from the third vertex.
 * Degenerate triangles retain the native divide-by-zero behavior. */
void fldCalcTrianglePlane(f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant) {
    f32 firstPoint[4];
    f32 secondPoint[4];
    f32 thirdPoint[4];
    f32 inverseLength;

    firstPoint[0] = vertices[0];
    firstPoint[1] = vertices[1];
    firstPoint[2] = vertices[2];
    secondPoint[0] = vertices[4];
    secondPoint[1] = vertices[5];
    secondPoint[2] = vertices[6];
    thirdPoint[0] = vertices[8];
    thirdPoint[1] = vertices[9];
    thirdPoint[2] = vertices[10];
    *normalX = (secondPoint[1] - firstPoint[1]) * (thirdPoint[2] - secondPoint[2]) - (secondPoint[2] - firstPoint[2]) * (thirdPoint[1] - secondPoint[1]);
    *normalY = (secondPoint[2] - firstPoint[2]) * (thirdPoint[0] - secondPoint[0]) - (secondPoint[0] - firstPoint[0]) * (thirdPoint[2] - secondPoint[2]);
    *normalZ = (secondPoint[0] - firstPoint[0]) * (thirdPoint[1] - secondPoint[1]) - (secondPoint[1] - firstPoint[1]) * (thirdPoint[0] - secondPoint[0]);
    inverseLength = 1.0f / fsqrtf(*normalX * *normalX + *normalY * *normalY + *normalZ * *normalZ);
    *normalX = *normalX * inverseLength;
    *normalY = *normalY * inverseLength;
    *normalZ = *normalZ * inverseLength;
    if (*normalX > -0.0001f && *normalX < 0.0001f) {
        *normalX = 0.0f;
    }
    if (*normalY > -0.0001f && *normalY < 0.0001f) {
        *normalY = 0.0f;
    }
    if (*normalZ > -0.0001f && *normalZ < 0.0001f) {
        *normalZ = 0.0f;
    }
    *planeConstant = -(*normalX * thirdPoint[0] + *normalY * thirdPoint[1] + *normalZ * thirdPoint[2]);
}

/* Return the first matching record's world node; NULL also denotes a missing ID. */
struct EffWorldNode *fldHitGetMatter(s32 id) {
    s32 index = 0;

    if (fldValueRecordCount > 0) {
        FldValueRecord *record = (FldValueRecord *)fldValueRecords;
        do {
            if (record->id == id) {
                return record->worldNode;
            }
            index++;
            record++;
        } while (index < fldValueRecordCount);
    }
    return 0;
}

/* Update every matching ID, including duplicates; leave other records intact. */
void fldHitLinkMatter(s32 id, struct EffWorldNode *worldNode) {
    s32 index;

    for (index = 0; index < fldValueRecordCount; index++) {
        if (((FldValueRecord *)fldValueRecords)[index].id == id) {
            ((FldValueRecord *)fldValueRecords)[index].worldNode = worldNode;
        }
    }
}

void fldClrCollision(void) {
    s32 index;
    for (index = 0; index < fldValueRecordCount; index++) {
        ((FldValueRecord *)fldValueRecords)[index].worldNode = 0;
    }
    fldValueRecordCount = 0;
    fldAreaState.unkA0 = -1;
    fldAreaState.unkA4 = -1;
    fldAreaState.unkAC = -1;
    D_00436188 = 0;
    D_0043617C = 0;
    if (fldValueRecords != 0) {
        fldHitFree();
    }
}

void fldAddCollisionPoly(FldCollisionDescriptor *geometry, f32 *translation,
                   u32 id, s32 sceneFlag, s32 ownerIndex) {
    f32 vertices[4][4];
    f32 copiedVertices[4][4];
    /* Inferred four-point capacity from related native workspace maps;
     * this operation populates only its three defining points. */
    f32 edgeTriangle[4][4];
    f32 center[3];
    f32 capturedPlane[4];
    f32 minimum[2];
    f32 maximum[2];
    FldValueRecord *record;
    s32 pass;
    s32 remaining;
    f32 (*sourceVertices)[4];
    void *stopData;
    s32 trianglesRemaining;
    FldCollisionFace *face;
    f32 *planeX;
    f32 *planeY;
    f32 *planeZ;
    f32 *limit0;
    f32 *limit1;
    f32 *limit2;
    f32 *limit3;
    f32 absX;
    f32 absY;
    f32 absZ;
    f32 runningMinimum;
    s32 i;
    s32 component;
    if (fldValueRecords == 0) {
        fldHitAlloc();
    }
    for (pass = 0; pass < 5; pass++) {
        s32 faceCount;
        FldCollisionFace *firstFace;
        sourceVertices = geometry->vertices;
        faceCount = geometry->faceCount;
        firstFace = geometry->faces;
        stopData = geometry->stopData;
        trianglesRemaining = geometry->triangleCount;
        if (faceCount > 0) {
            face = firstFace;
            remaining = faceCount;
            for (; remaining != 0; remaining--, face++, trianglesRemaining--) {
                u32 vertexIndex;
                record = &((FldValueRecord *)fldValueRecords)[fldValueRecordCount];
                record->count = trianglesRemaining > 0 ? 3 : 4;
                vertexIndex = face->vertexIndices[0];
                vertices[0][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[0][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[0][2] = sourceVertices[vertexIndex][2] + translation[2];
                vertexIndex = face->vertexIndices[1];
                vertices[1][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[1][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[1][2] = sourceVertices[vertexIndex][2] + translation[2];
                vertexIndex = face->vertexIndices[2];
                vertices[2][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[2][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[2][2] = sourceVertices[vertexIndex][2] + translation[2];
                if (record->count == 3) {
                    for (i = 0; i < 3; i++) {
                        copiedVertices[i][0] = vertices[i][0];
                        copiedVertices[i][1] = vertices[i][1];
                        copiedVertices[i][2] = vertices[i][2];
                    }
                } else {
                    vertexIndex = face->vertexIndices[3];
                    vertices[3][0] = sourceVertices[vertexIndex][0] + translation[0];
                    vertices[3][1] = sourceVertices[vertexIndex][1] + translation[1];
                    vertices[3][2] = sourceVertices[vertexIndex][2] + translation[2];
                    for (i = 0; i < 4; i++) {
                        copiedVertices[i][0] = vertices[i][0];
                        copiedVertices[i][1] = vertices[i][1];
                        copiedVertices[i][2] = vertices[i][2];
                    }
                }
                fldCalcTrianglePlane(copiedVertices[0], &capturedPlane[0],
                                     &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                for (i = 0; i < 3; i++) {
                    record->normal[i] = capturedPlane[i];
                }
                record->normal[3] = 0.0f;
                for (i = 0; i < record->count; i++) {
                    for (component = 0; component < 3; component++) {
                        record->vertices[i][component] = copiedVertices[i][component];
                    }
                    record->vertices[i][3] = 1.0f;
                }
                record->planeConstant = fldDotVector(record->normal, record->vertices[0]);
                absX = record->normal[0];
                absX = ffabsf(absX);
                absY = ffabsf(record->normal[1]);
                absZ = ffabsf(record->normal[2]);
                if (pass == 0) {
                    if (!(absX < absY) || !(absZ < absY)) continue;
                    if (record->normal[1] > 0.0f) continue;
                    if (record->normal[1] > -0.98f) continue;
                } else if (pass == 1) {
                    if (!(absX < absY) || !(absZ < absY)) continue;
                    if (record->normal[1] > 0.0f) continue;
                    if (record->normal[1] <= -0.98f) continue;
                } else if (pass == 2) {
                    if (!(absY <= absX) || !(absZ <= absX)) continue;
                } else if (pass == 3) {
                    if (!(absX < absZ) || !(absY <= absZ)) continue;
                } else if (pass == 4) {
                    if (!(absX <= absY) || !(absZ <= absY)) continue;
                    if (record->normal[1] <= 0.0f) continue;
                }
                if (absY <= absX && absZ <= absX) {
                    record->mode = 0;
                    runningMinimum = vertices[0][1];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][2];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->count; i++) {
                        if (vertices[i][1] < runningMinimum) {
                            runningMinimum = vertices[i][1];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][2] < minimum[1]) minimum[1] = vertices[i][2];
                        if (vertices[i][1] > maximum[0]) maximum[0] = vertices[i][1];
                        if (vertices[i][2] > maximum[1]) maximum[1] = vertices[i][2];
                    }
                    record->bound[0] = runningMinimum;
                    record->bound[1] = minimum[1];
                    record->bound[2] = maximum[0];
                    record->bound[3] = maximum[1];
                    if (record->count == 3) {
                        center[0] = 0.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1]) / 3.0f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2]) / 3.0f;
                    } else {
                        center[0] = 0.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1] + vertices[3][1]) * 0.25f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2] + vertices[3][2]) * 0.25f;
                    }
                    edgeTriangle[0][0] = 0.0f;
                    edgeTriangle[0][1] = vertices[0][1];
                    edgeTriangle[0][2] = vertices[0][2];
                    edgeTriangle[1][0] = 0.0f;
                    edgeTriangle[1][1] = vertices[1][1];
                    edgeTriangle[1][2] = vertices[1][2];
                    edgeTriangle[2][0] = 10.0f;
                    edgeTriangle[2][1] = vertices[1][1];
                    edgeTriangle[2][2] = vertices[1][2];
                    planeX = &record->plane[0][0];
                    planeY = &record->plane[0][1];
                    planeZ = &record->plane[0][2];
                    limit0 = &record->limit[0];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    if (fldDotVector(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = 0.0f;
                    edgeTriangle[0][1] = vertices[1][1];
                    edgeTriangle[0][2] = vertices[1][2];
                    edgeTriangle[1][0] = 0.0f;
                    edgeTriangle[1][1] = vertices[2][1];
                    edgeTriangle[1][2] = vertices[2][2];
                    edgeTriangle[2][0] = 10.0f;
                    edgeTriangle[2][1] = vertices[2][1];
                    edgeTriangle[2][2] = vertices[2][2];
                    planeX = &record->plane[1][0];
                    planeY = &record->plane[1][1];
                    planeZ = &record->plane[1][2];
                    limit1 = &record->limit[1];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    if (fldDotVector(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    }
                    if (record->count == 3) {
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[3][1];
                        edgeTriangle[1][2] = vertices[3][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[3][1];
                        edgeTriangle[2][2] = vertices[3][2];
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[3][1];
                        edgeTriangle[0][2] = vertices[3][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->plane[3][0];
                        planeY = &record->plane[3][1];
                        planeZ = &record->plane[3][2];
                        limit3 = &record->limit[3];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    }
                }
                else if (absX <= absY && absZ <= absY) {
                    record->mode = 1;
                    runningMinimum = vertices[0][0];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][2];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->count; i++) {
                        if (vertices[i][0] < runningMinimum) {
                            runningMinimum = vertices[i][0];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][2] < minimum[1]) minimum[1] = vertices[i][2];
                        if (vertices[i][0] > maximum[0]) maximum[0] = vertices[i][0];
                        if (vertices[i][2] > maximum[1]) maximum[1] = vertices[i][2];
                    }
                    record->bound[0] = runningMinimum;
                    record->bound[1] = minimum[1];
                    record->bound[2] = maximum[0];
                    record->bound[3] = maximum[1];
                    if (record->count == 3) {
                        center[1] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0]) / 3.0f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2]) / 3.0f;
                    } else {
                        center[1] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0] + vertices[3][0]) * 0.25f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2] + vertices[3][2]) * 0.25f;
                    }
                    edgeTriangle[0][0] = vertices[0][0];
                    edgeTriangle[0][1] = 0.0f;
                    edgeTriangle[0][2] = vertices[0][2];
                    edgeTriangle[1][0] = vertices[1][0];
                    edgeTriangle[1][1] = 0.0f;
                    edgeTriangle[1][2] = vertices[1][2];
                    edgeTriangle[2][0] = vertices[1][0];
                    edgeTriangle[2][1] = 10.0f;
                    edgeTriangle[2][2] = vertices[1][2];
                    planeX = &record->plane[0][0];
                    planeY = &record->plane[0][1];
                    planeZ = &record->plane[0][2];
                    limit0 = &record->limit[0];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    if (fldDotVector(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = vertices[1][0];
                    edgeTriangle[0][1] = 0.0f;
                    edgeTriangle[0][2] = vertices[1][2];
                    edgeTriangle[1][0] = vertices[2][0];
                    edgeTriangle[1][1] = 0.0f;
                    edgeTriangle[1][2] = vertices[2][2];
                    edgeTriangle[2][0] = vertices[2][0];
                    edgeTriangle[2][1] = 10.0f;
                    edgeTriangle[2][2] = vertices[2][2];
                    planeX = &record->plane[1][0];
                    planeY = &record->plane[1][1];
                    planeZ = &record->plane[1][2];
                    limit1 = &record->limit[1];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    if (fldDotVector(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    }
                    if (record->count == 3) {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = vertices[3][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[3][2];
                        edgeTriangle[2][0] = vertices[3][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[3][2];
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = vertices[3][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[3][2];
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->plane[3][0];
                        planeY = &record->plane[3][1];
                        planeZ = &record->plane[3][2];
                        limit3 = &record->limit[3];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    }
                }
                else {
                    record->mode = 2;
                    runningMinimum = vertices[0][0];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][1];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->count; i++) {
                        if (vertices[i][0] < runningMinimum) {
                            runningMinimum = vertices[i][0];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][1] < minimum[1]) minimum[1] = vertices[i][1];
                        if (vertices[i][0] > maximum[0]) maximum[0] = vertices[i][0];
                        if (vertices[i][1] > maximum[1]) maximum[1] = vertices[i][1];
                    }
                    record->bound[0] = runningMinimum;
                    record->bound[1] = minimum[1];
                    record->bound[2] = maximum[0];
                    record->bound[3] = maximum[1];
                    if (record->count == 3) {
                        center[2] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0]) / 3.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1]) / 3.0f;
                    } else {
                        center[2] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0] + vertices[3][0]) * 0.25f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1] + vertices[3][1]) * 0.25f;
                    }
                    edgeTriangle[0][0] = vertices[0][0];
                    edgeTriangle[0][1] = vertices[0][1];
                    edgeTriangle[0][2] = 0.0f;
                    edgeTriangle[1][0] = vertices[1][0];
                    edgeTriangle[1][1] = vertices[1][1];
                    edgeTriangle[1][2] = 0.0f;
                    edgeTriangle[2][0] = vertices[1][0];
                    edgeTriangle[2][1] = vertices[1][1];
                    edgeTriangle[2][2] = 10.0f;
                    planeX = &record->plane[0][0];
                    planeY = &record->plane[0][1];
                    planeZ = &record->plane[0][2];
                    limit0 = &record->limit[0];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    if (fldDotVector(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = vertices[1][0];
                    edgeTriangle[0][1] = vertices[1][1];
                    edgeTriangle[0][2] = 0.0f;
                    edgeTriangle[1][0] = vertices[2][0];
                    edgeTriangle[1][1] = vertices[2][1];
                    edgeTriangle[1][2] = 0.0f;
                    edgeTriangle[2][0] = vertices[2][0];
                    edgeTriangle[2][1] = vertices[2][1];
                    edgeTriangle[2][2] = 10.0f;
                    planeX = &record->plane[1][0];
                    planeY = &record->plane[1][1];
                    planeZ = &record->plane[1][2];
                    limit1 = &record->limit[1];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    if (fldDotVector(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    }
                    if (record->count == 3) {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[3][0];
                        edgeTriangle[1][1] = vertices[3][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[3][0];
                        edgeTriangle[2][1] = vertices[3][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = vertices[3][0];
                        edgeTriangle[0][1] = vertices[3][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->plane[3][0];
                        planeY = &record->plane[3][1];
                        planeZ = &record->plane[3][2];
                        limit3 = &record->limit[3];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    }
                }
                record->unkC8 = D_00436188;
                record->id = id;
                record->worldNode = 0;
                record->face = face;
                record->stopData = stopData;
                record->sceneFlag = sceneFlag;
                record->ownerIndex = ownerIndex;
                record->unkCA = 0;
                fldValueRecordCount++;
            }
        }
    }
    D_00436188++;
}

s32 fldClassifyPositionInZoneWithMargin(f32 *out, s32 mode, s32 count, f32 margin, f32 *pos, FldValueRecord *zone) {
    f32 probe[3];
    f32 planar[2];
    f32 best = margin;
    f32 dist;
    f32 slack;
    s32 inside = 0;
    s32 planeIndex;

    if (mode == 0) {
        probe[0] = 0.0f;
        probe[1] = pos[1];
        probe[2] = pos[2];
        planar[0] = pos[1];
        planar[1] = pos[2];
    } else if (mode == 1) {
        probe[0] = pos[0];
        probe[1] = 0.0f;
        probe[2] = pos[2];
        planar[0] = pos[0];
        planar[1] = pos[2];
    } else {
        probe[0] = pos[0];
        probe[1] = pos[1];
        probe[2] = 0.0f;
        planar[0] = pos[0];
        planar[1] = pos[1];
    }
    for (planeIndex = 0; planeIndex < count; planeIndex++) {
        dist = fldDotVector(probe, zone->plane[planeIndex]) - zone->limit[planeIndex];
        slack = dist + margin;
        if (slack < 0.0f) {
            *out = -1.0f;
            return -1;
        }
        if (planar[0] < zone->bound[0] - margin || planar[1] < zone->bound[1] - margin ||
            zone->bound[2] + margin < planar[0] || zone->bound[3] + margin < planar[1]) {
            *out = -1.0f;
            return -1;
        }
        if (dist < 0.0f) {
            /* The mode-1 plane uses a fixed 45-unit inset, unlike the
               other projections, which use the caller's margin. */
            if (mode == 1) {
                best = dist + 45.0f;
            } else {
                best = slack;
            }
            inside = 1;
        }
    }
    if (best < 0.001f) {
        best = -1.0f;
    }
    *out = best;
    return inside;
}

extern f32 D_0038BBF0[3];

extern s32 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit);

extern s32 fldTestMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit);

void fldHitChkUnitElvOnly(f32 *input) {
    f32 position[4];
    f32 previous[4];
    f32 savedPosition[4];
    f32 savedPrevious[4];
    f32 absoluteDelta[4];
    f32 clearance;
    FldValueRecord *record;
    ObjectTransform *transform;
    ObjectTransform *anchor;
    f32 distance;
    s32 relative;
    s32 inside;
    s32 index;
    f32 margin;

    previous[1] = D_0038BBF0[1];
    position[1] = input[1] - 35.0f;
    position[0] = input[0];
    position[2] = input[2];
    previous[0] = D_0038BBF0[0];
    previous[2] = D_0038BBF0[2];
    absoluteDelta[0] = ffabsf(input[0] - D_0038BBF0[0]);
    absoluteDelta[2] = ffabsf(input[2] - D_0038BBF0[2]);
    margin = 45.0f;
    for (index = 0; index < fldValueRecordCount; index++) {
        if (!(((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x80)) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldAreaState.area, fldAreaState.floor + 1,
                                 ((FldValueRecord *)fldValueRecords)[index].sceneFlag)) {
            continue;
        }
        if (((FldValueRecord *)fldValueRecords)[index].ownerIndex > 0) {
            if (fldTestMapSlotAuxiliaryFlag(fldAreaState.area, fldAreaState.floor + 1,
                                            ((FldValueRecord *)fldValueRecords)[index].ownerIndex)) {
                continue;
            }
        }
        savedPosition[0] = position[0];
        savedPosition[1] = position[1];
        savedPosition[2] = position[2];
        savedPrevious[0] = previous[0];
        savedPrevious[1] = previous[1];
        savedPrevious[2] = previous[2];
        if (((FldValueRecord *)fldValueRecords)[index].worldNode != 0) {
            transform = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
            relative = 1;
            position[0] -= transform->position[0];
            position[1] -= transform->position[1];
            position[2] -= transform->position[2];
            previous[0] -= transform->position[0];
            previous[1] -= transform->position[1];
            previous[2] -= transform->position[2];
        } else {
            relative = 0;
        }
        distance = fldDotVector(position, ((FldValueRecord *)fldValueRecords)[index].normal);
        distance -= ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        if (ffabsf(distance) < margin) {
            record = &((FldValueRecord *)fldValueRecords)[index];
            fldClassifyPositionInZoneWithMargin(&clearance, record->mode, record->count, 45.0f, position, record);
            inside = clearance < 0.0f ? 0 : 1;
            if (inside == 1) {
                if (((FldValueRecord *)fldValueRecords)[index].worldNode != 0) {
                    if (fldAreaState.unk90 == -1 ||
                        ((FldValueRecord *)fldValueRecords)[fldAreaState.unk90].worldNode->key ==
                            ((FldValueRecord *)fldValueRecords)[index].worldNode->key) {
                        fldAreaState.unk90 = index;
                    } else {
                        fldAreaState.unk94 = index;
                    }
                    anchor = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[0] = anchor->position[0];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[1] = anchor->position[1];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[2] = anchor->position[2];
                }
            }
        }
        if (relative) {
            transform = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
            position[0] += transform->position[0];
            position[1] += transform->position[1];
            position[2] += transform->position[2];
            previous[0] += transform->position[0];
            previous[1] += transform->position[1];
            previous[2] += transform->position[2];
        }
    }
}

/* Indexed fields retain index-first addresses; see docs/idioms.md,
 * "Trace-justified indexed field addresses: relative zone motion". */
void fldHitChkUnitElv(f32 *position) {
    f32 newPosition[3];
    f32 previous[3];
    f32 movement[4];
    s32 index;

    memset(movement, 0, sizeof(movement));
    movement[3] = 1.0f;
    fldAreaState.positionPending = 0;
    if (fldAreaState.unk90 == -1) {
        fldHitChkUnitElvOnly(position);
    }
    index = fldAreaState.unk90;
    if (index != -1) {
        if (((FldValueRecord *)fldValueRecords)[index].worldNode != 0) {
            ObjectTransform *transform;

            transform = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
            movement[0] = transform->position[0] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[0];
            movement[1] = transform->position[1] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[1];
            movement[2] = transform->position[2] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[2];
            ((FldValueRecord *)fldValueRecords)[index].previousPosition[0] = transform->position[0];
            ((FldValueRecord *)fldValueRecords)[index].previousPosition[1] = transform->position[1];
            ((FldValueRecord *)fldValueRecords)[index].previousPosition[2] = transform->position[2];
        }
        if (fldTestRoomSceneFlag(fldAreaState.area, fldAreaState.floor + 1,
                               ((FldValueRecord *)fldValueRecords)[index].sceneFlag) == 0) {
            index = fldAreaState.unk94;
            if (index != -1 && fldTestRoomSceneFlag(fldAreaState.area,
                        fldAreaState.floor + 1,
                        ((FldValueRecord *)fldValueRecords)[index].sceneFlag) == 0) {
                if (((FldValueRecord *)fldValueRecords)[index].worldNode != 0) {
                    ObjectTransform *transform;

                    transform = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
                    movement[0] += transform->position[0] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[0];
                    movement[1] += transform->position[1] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[1];
                    movement[2] += transform->position[2] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[2];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[0] = transform->position[0];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[1] = transform->position[1];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[2] = transform->position[2];
                }
            }
            newPosition[0] = position[0] + movement[0];
            newPosition[1] = position[1] + movement[1];
            newPosition[2] = position[2] + movement[2];
            previous[0] = D_0038BBF0[0];
            previous[1] = D_0038BBF0[1];
            previous[2] = D_0038BBF0[2];
            fldAreaState.positionPending = 1;
            fldAreaState.targetX = newPosition[0];
            fldAreaState.targetY = newPosition[1];
            fldAreaState.targetZ = newPosition[2];
            position[0] = newPosition[0];
            position[1] = newPosition[1];
            position[2] = newPosition[2];
            D_0038BBF0[0] = newPosition[0];
            D_0038BBF0[1] = newPosition[1];
            D_0038BBF0[2] = newPosition[2];
        }
    }
}

/* Non-owning native alias of fldAreaState.terrainAttritionKind (FldAreaWork + 0x11C). */
extern s32 D_0038988C[];

/* Classify flagged zone faces after checking the current movement. */
void fldHitChkUnitDamage(f32 *position) {
    f32 point[3];
    f32 reference[3];
    f32 distanceVector[3];
    f32 savedPoint[3];
    f32 savedReference[3];
    f32 axisDistance[3];
    f32 margin;
    f32 planeDistance;
    f32 classifiedMargin;
    FldAreaWork *areaState;
    s32 selectedAxis;
    s32 pass;
    s32 recordIndex;

    D_0038988C[0] = 0;

    /* Capture both snapshots before lowering the current probe for testing. */
    point[0] = position[0];
    point[1] = position[1];
    point[2] = position[2];
    reference[0] = D_0038BBF0[0];
    reference[1] = D_0038BBF0[1];
    reference[2] = D_0038BBF0[2];
    point[1] -= 35.0f;
    distanceVector[0] = point[0] - reference[0];
    distanceVector[1] = point[1] - reference[1];
    distanceVector[2] = point[2] - reference[2];

    if (fldCalculateVectorLength(distanceVector) < 0.1f) {
        position[0] = reference[0];
        position[1] = reference[1];
        position[2] = reference[2];
        return;
    }

    axisDistance[0] = ffabsf(point[0] - reference[0]);
    axisDistance[2] = ffabsf(point[2] - reference[2]);
    selectedAxis = axisDistance[2] < axisDistance[0] ? 0 : 2;

    fldHitChkUnitElvOnly(position);

    /* Linked-node translations accumulate across records and both passes. */
    for (pass = 0; pass < 2; pass++) {
        recordIndex = 0;
        if (fldValueRecordCount > 0) {
            areaState = &fldAreaState;
            do {
                EffWorldNode *node;

                margin = ((FldValueRecord *)fldValueRecords)[recordIndex].mode == 1 ? 45.0f : 60.0f;
                if (pass == 0) {
                    if (((FldValueRecord *)fldValueRecords)[recordIndex].mode == selectedAxis) {
                        continue;
                    }
                } else if (((FldValueRecord *)fldValueRecords)[recordIndex].mode != selectedAxis) {
                    continue;
                }

                if (fldTestRoomSceneFlag(areaState->area, areaState->floor + 1, ((FldValueRecord *)fldValueRecords)[recordIndex].sceneFlag) != 0) {
                    continue;
                }

                savedPoint[0] = point[0];
                savedPoint[1] = point[1];
                savedPoint[2] = point[2];
                savedReference[0] = reference[0];
                savedReference[1] = reference[1];
                savedReference[2] = reference[2];

                node = ((FldValueRecord *)fldValueRecords)[recordIndex].worldNode;
                if (node != 0) {
                    point[0] = savedPoint[0] - node->inner->position[0];
                    point[1] = savedPoint[1] - node->inner->position[1];
                    point[2] = savedPoint[2] - node->inner->position[2];
                    reference[0] = savedReference[0] - node->inner->position[0];
                    reference[1] = savedReference[1] - node->inner->position[1];
                    reference[2] = savedReference[2] - node->inner->position[2];
                }

                planeDistance = fldDotVector(point, ((FldValueRecord *)fldValueRecords)[recordIndex].normal);
                {
                    u32 faceAttributes = ((FldValueRecord *)fldValueRecords)[recordIndex].face->attributes;
                    planeDistance = ffabsf(planeDistance - ((FldValueRecord *)fldValueRecords)[recordIndex].planeConstant);
                    if ((faceAttributes & 0x4000) == 0) {
                        continue;
                    }
                    {
                        s32 accepted = 0;
                        if (!(planeDistance < margin)) {
                            continue;
                        }
                        fldClassifyPositionInZoneWithMargin(
                            &classifiedMargin,
                            ((FldValueRecord *)fldValueRecords)[recordIndex].mode,
                            ((FldValueRecord *)fldValueRecords)[recordIndex].count,
                            margin, point,
                            &((FldValueRecord *)fldValueRecords)[recordIndex]);
                        if (!(classifiedMargin < 0.0f)) {
                            accepted = 1;
                        }
                        if (accepted == 1) {
                            FldCollisionFace *face = ((FldValueRecord *)fldValueRecords)[recordIndex].face;
                            s32 soundClass = face->sound & 0xF0;
                            if (soundClass == 0x20) {
                                areaState->terrainAttritionKind = 3;
                            } else if (soundClass == 0x30) {
                                areaState->terrainAttritionKind = 2;
                            } else {
                                areaState->terrainAttritionKind = accepted;
                            }
                        }
                    }
                }
            } while (++recordIndex < fldValueRecordCount);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00136EF8", fldRefreshAreaStateFromPosition);

void func_0013AA78(void) {
}

void func_0013AA80(void) {
}

void func_0013AA88(void) {
}

/* Project away the selected axis and reject points outside expanded bounds.
 * Return the last negative plane's margin-adjusted distance, not a minimum;
 * no negative plane leaves margin unchanged. Reject results below 0.001. */
f32 fldGetPositionZoneClearance(s32 axisMode, s32 planeCount, f32 margin, f32 *position, FldValueRecord *zone) {
    f32 projectedPosition[3];
    f32 planarPosition[2];
    f32 clearance = margin;
    f32 planeDistance;
    f32 marginDistance;
    s32 planeIndex;

    if (axisMode == 0) {
        projectedPosition[0] = 0.0f;
        projectedPosition[1] = position[1];
        projectedPosition[2] = position[2];
        planarPosition[0] = position[1];
        planarPosition[1] = position[2];
    } else if (axisMode == 1) {
        projectedPosition[0] = position[0];
        projectedPosition[1] = 0.0f;
        projectedPosition[2] = position[2];
        planarPosition[0] = position[0];
        planarPosition[1] = position[2];
    } else {
        projectedPosition[0] = position[0];
        projectedPosition[1] = position[1];
        projectedPosition[2] = 0.0f;
        planarPosition[0] = position[0];
        planarPosition[1] = position[1];
    }
    for (planeIndex = 0; planeIndex < planeCount; planeIndex++) {
        planeDistance = fldDotVector(projectedPosition, zone->plane[planeIndex]) - zone->limit[planeIndex];
        marginDistance = planeDistance + margin;
        if (marginDistance < 0.0f || planarPosition[0] < zone->bound[0] - margin || planarPosition[1] < zone->bound[1] - margin ||
            zone->bound[2] + margin < planarPosition[0] || zone->bound[3] + margin < planarPosition[1]) {
            return -1.0f;
        }
        if (planeDistance < 0.0f) {
            clearance = marginDistance;
        }
    }
    if (clearance < 0.001f) {
        clearance = -1.0f;
    }
    return clearance;
}

extern void fldToggleWorldNodeState(s32 enabled);

extern void dds3SetObjectPayloadWord8(EffWorldNode *object, u32 value);

s32 fldHitChkCamera(f32 *start, f32 *end, f32 *hitPoint) {
    f32 origin[4];
    f32 target[4];
    f32 delta[4];
    f32 probeStart[4];
    f32 probeEnd[4];
    f32 anchor[4];
    ObjectTransform *transform;
    EffWorldNode *secondary;
    f32 startDistance;
    f32 endDistance;
    f32 total;
    f32 ratio;
    f32 clearance;
    s32 found;
    FldStopEntry *entry;
    s32 relative;
    s32 index;

    found = -1;
    secondary = dds3GetWorldSecondaryObject();
    if (!((u32)(fldAreaState.area - 200) < 300)) {
        fldToggleWorldNodeState(0);
    }
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    target[0] = end[0];
    target[1] = end[1];
    target[2] = end[2];
    for (index = 0; index < fldValueRecordCount; index++) {
        if (((FldValueRecord *)fldValueRecords)[index].mode == 1) {
            if (!(((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x100)) {
                continue;
            }
        }
        if (((FldValueRecord *)fldValueRecords)[index].face->attributes & 0xA01) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldAreaState.area, fldAreaState.floor + 1,
                                 ((FldValueRecord *)fldValueRecords)[index].sceneFlag)) {
            continue;
        }
        if (((FldValueRecord *)fldValueRecords)[index].worldNode != 0) {
            relative = 1;
            transform = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
            origin[0] -= transform->position[0];
            origin[1] -= transform->position[1];
            origin[2] -= transform->position[2];
            target[0] -= transform->position[0];
            target[1] -= transform->position[1];
            target[2] -= transform->position[2];
        } else {
            relative = 0;
        }
        probeStart[0] = origin[0];
        probeStart[1] = origin[1];
        probeStart[2] = origin[2];
        probeEnd[0] = target[0];
        probeEnd[1] = target[1];
        probeEnd[2] = target[2];
        startDistance = fldDotVector(probeStart, ((FldValueRecord *)fldValueRecords)[index].normal) -
                        ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        endDistance = fldDotVector(probeEnd, ((FldValueRecord *)fldValueRecords)[index].normal) -
                      ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        if ((startDistance < 0.0f && 0.0f <= endDistance) || (endDistance < 0.0f && 0.0f <= startDistance)) {
            startDistance = ffabsf(startDistance);
            endDistance = ffabsf(endDistance);
            total = startDistance + endDistance;
            ratio = startDistance / total;
            anchor[0] = probeStart[0];
            anchor[1] = probeStart[1];
            anchor[2] = probeStart[2];
            delta[0] = probeEnd[0] - probeStart[0];
            delta[1] = probeEnd[1] - probeStart[1];
            delta[2] = probeEnd[2] - probeStart[2];
            probeStart[0] += delta[0] * ratio;
            probeStart[1] += delta[1] * ratio;
            probeStart[2] += delta[2] * ratio;
            clearance = fldGetPositionZoneClearance(((FldValueRecord *)fldValueRecords)[index].mode, ((FldValueRecord *)fldValueRecords)[index].count, 2.0f, probeStart, &((FldValueRecord *)fldValueRecords)[index]);
            if (0.0f <= clearance) {
                if (((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x400) {
                    entry = &((FldStopEntry *)((FldValueRecord *)fldValueRecords)[index].stopData)
                                [((FldValueRecord *)fldValueRecords)[index].face->stop];
                    dds3SetObjectPayloadWord8(dds3FindIndexedObjectChainNodeByName(secondary, 6, entry->name), 4);
                } else {
                found = index;
                ratio = (startDistance + clearance) / total;
                delta[0] = probeEnd[0] - anchor[0];
                delta[1] = probeEnd[1] - anchor[1];
                delta[2] = probeEnd[2] - anchor[2];
                anchor[0] += delta[0] * ratio;
                anchor[1] += delta[1] * ratio;
                anchor[2] += delta[2] * ratio;
                probeStart[0] = anchor[0];
                probeStart[1] = anchor[1];
                probeStart[2] = anchor[2];
                origin[0] = anchor[0];
                origin[1] = anchor[1];
                origin[2] = anchor[2];
                }
            }
        }
        if (relative) {
            transform = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
            origin[0] += transform->position[0];
            origin[1] += transform->position[1];
            origin[2] += transform->position[2];
            target[0] += transform->position[0];
            target[1] += transform->position[1];
            target[2] += transform->position[2];
        }
    }
    hitPoint[0] = origin[0];
    hitPoint[1] = origin[1];
    hitPoint[2] = origin[2];
    return found;
}

s32 fldHitChkEnemy(f32 *start, f32 *end, f32 *hitPoint) {
    f32 origin[4];
    f32 target[4];
    f32 delta[4];
    f32 probeStart[4];
    f32 probeEnd[4];
    f32 anchor[4];
    ObjectTransform *transform;
    f32 startDistance;
    f32 endDistance;
    f32 total;
    f32 ratio;
    f32 clearance;
    s32 found;
    s32 relative;
    s32 index;

    found = -1;
    dds3GetWorldSecondaryObject();
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    target[0] = end[0];
    target[1] = end[1];
    target[2] = end[2];
    for (index = 0; index < fldValueRecordCount; index++) {
        if (((FldValueRecord *)fldValueRecords)[index].mode == 1) {
            if (!(((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x100)) {
                continue;
            }
        }
        if (((FldValueRecord *)fldValueRecords)[index].face->attributes & 0xA01) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldAreaState.area, fldAreaState.floor + 1,
                                 ((FldValueRecord *)fldValueRecords)[index].sceneFlag)) {
            continue;
        }
        if (!(((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x8000)) {
            continue;
        }
        if (((FldValueRecord *)fldValueRecords)[index].face->special[0] != 4) {
            continue;
        }
        if (((FldValueRecord *)fldValueRecords)[index].worldNode != 0) {
            relative = 1;
            transform = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
            origin[0] -= transform->position[0];
            origin[1] -= transform->position[1];
            origin[2] -= transform->position[2];
            target[0] -= transform->position[0];
            target[1] -= transform->position[1];
            target[2] -= transform->position[2];
        } else {
            relative = 0;
        }
        probeStart[0] = origin[0];
        probeStart[1] = origin[1];
        probeStart[2] = origin[2];
        probeEnd[0] = target[0];
        probeEnd[1] = target[1];
        probeEnd[2] = target[2];
        startDistance = fldDotVector(probeStart, ((FldValueRecord *)fldValueRecords)[index].normal) -
                        ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        endDistance = fldDotVector(probeEnd, ((FldValueRecord *)fldValueRecords)[index].normal) -
                      ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        if ((startDistance < 0.0f && 0.0f <= endDistance) || (endDistance < 0.0f && 0.0f <= startDistance)) {
            startDistance = ffabsf(startDistance);
            endDistance = ffabsf(endDistance);
            total = startDistance + endDistance;
            ratio = startDistance / total;
            anchor[0] = probeStart[0];
            anchor[1] = probeStart[1];
            anchor[2] = probeStart[2];
            delta[0] = probeEnd[0] - probeStart[0];
            delta[1] = probeEnd[1] - probeStart[1];
            delta[2] = probeEnd[2] - probeStart[2];
            probeStart[0] += delta[0] * ratio;
            probeStart[1] += delta[1] * ratio;
            probeStart[2] += delta[2] * ratio;
            clearance = fldGetPositionZoneClearance(((FldValueRecord *)fldValueRecords)[index].mode, ((FldValueRecord *)fldValueRecords)[index].count, 2.0f, probeStart, &((FldValueRecord *)fldValueRecords)[index]);
            if (0.0f <= clearance) {
                found = index;
                ratio = (startDistance + clearance) / total;
                delta[0] = probeEnd[0] - anchor[0];
                delta[1] = probeEnd[1] - anchor[1];
                delta[2] = probeEnd[2] - anchor[2];
                anchor[0] += delta[0] * ratio;
                anchor[1] += delta[1] * ratio;
                anchor[2] += delta[2] * ratio;
                probeStart[0] = anchor[0];
                probeStart[1] = anchor[1];
                probeStart[2] = anchor[2];
                origin[0] = anchor[0];
                origin[1] = anchor[1];
                origin[2] = anchor[2];
            }
        }
        if (relative) {
            transform = ((FldValueRecord *)fldValueRecords)[index].worldNode->inner;
            origin[0] += transform->position[0];
            origin[1] += transform->position[1];
            origin[2] += transform->position[2];
            target[0] += transform->position[0];
            target[1] += transform->position[1];
            target[2] += transform->position[2];
        }
    }
    hitPoint[0] = origin[0];
    hitPoint[1] = origin[1];
    hitPoint[2] = origin[2];
    return found;
}

/* Add the four vertical faces of an axis-aligned field collision box. */
void fldHitProgramAdd(s32 id, f32 x, f32 y, f32 z, f32 extent, f32 height) {
    f32 origin[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 index;
    f32 minX, minY, minZ, maxX, maxZ;

    index = D_0043617C;
    minX = x - extent;
    maxX = x + extent;
    minY = y - height;
    /* Build the front boundary before the back boundary, matching the
     * native corner ordering. The traced sched1 death notes then place
     * the final Z/extent use on minZ; local allocation keeps Z with maxZ. */
    maxZ = z + extent;
    minZ = z - extent;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.vertexCount = 8;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.faceCount = 4;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.triangleCount = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.vertices = ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.faces = ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.stopData = NULL;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[0][0] = minX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[0][1] = minY;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[0][2] = maxZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[0][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[1][0] = maxX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[1][1] = minY;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[1][2] = maxZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[1][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[2][0] = minX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[2][1] = minY;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[2][2] = minZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[2][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[3][0] = maxX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[3][1] = minY;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[3][2] = minZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[3][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[4][0] = minX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[4][1] = y;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[4][2] = maxZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[4][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[5][0] = maxX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[5][1] = y;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[5][2] = maxZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[5][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[6][0] = minX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[6][1] = y;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[6][2] = minZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[6][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[7][0] = maxX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[7][1] = y;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[7][2] = minZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[7][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].attributes = 128;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].moveFloor = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].sound = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].stop = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].place = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].automap[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].automap[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].vertexIndices[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].vertexIndices[1] = 1;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].vertexIndices[2] = 5;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].vertexIndices[3] = 4;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].encounterType = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].encounter = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].special[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].special[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].attributes = 128;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].moveFloor = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].sound = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].stop = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].place = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].automap[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].automap[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].vertexIndices[0] = 1;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].vertexIndices[1] = 3;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].vertexIndices[2] = 7;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].vertexIndices[3] = 5;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].encounterType = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].encounter = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].special[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].special[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].attributes = 128;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].moveFloor = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].sound = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].stop = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].place = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].automap[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].automap[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].vertexIndices[0] = 3;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].vertexIndices[1] = 2;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].vertexIndices[2] = 6;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].vertexIndices[3] = 7;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].encounterType = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].encounter = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].special[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].special[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].attributes = 128;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].moveFloor = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].sound = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].stop = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].place = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].automap[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].automap[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].vertexIndices[0] = 2;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].vertexIndices[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].vertexIndices[2] = 4;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].vertexIndices[3] = 6;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].encounterType = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].encounter = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].special[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].special[1] = 0;
    fldAddCollisionPoly(&((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry,
                  origin, dds3AdvanceWorldCounter(), 0, id);
    D_0043617C++;
}

void func_0013B810(void) {
}
INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436174);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldAuxRecordBuffer);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_0043617C);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldValueRecords);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldValueRecordCount);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436188);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldAuxRecordResource);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldValueRecordResource);

