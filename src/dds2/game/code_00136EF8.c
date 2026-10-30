#include "sdk_ps2.h"
#include "fld_item_box_object_api.h"
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
#include "field.h"
#include "fld_resource_resolver.h"
#include "fld_collision_geometry.h"
#include "fld_collision_query_api.h"
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
extern void fldDrawCameraColorScrollingGradient(void);

extern void eglSetOt();

extern void evtSubmitPrimaryGsTest();

extern void evtSubmitPrimaryAlphaBlendMode();

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern sdfTexHandle_t *fldRainTextureReference;

extern FldWallMeshRecord *fldItemBoxObjBuf;

extern collision_s *fldMatterTable;

extern struct sdfMemHandle_t *fldAuxRecordResource;

extern struct sdfMemHandle_t *fldValueRecordResource;


extern struct sdfMemHandle_t *fldRainTextureResource;

extern char D_00413350[];

extern u32 fldRainTextureData;

extern FldCameraSetting *fldCameraSettings;

extern SdfFlagListParams fldCameraColorParameters[];

extern s32 fldValueRecordCount;

extern s32 D_00436188;

extern s32 D_0043617C;

extern f32 fldVecDot(const f32 *, const f32 *);

#ifdef NON_MATCHING
#include "common.h"
#include "fld_area_work.h"
#include "fld.h"
#include "sdf.h"
#include "sdf_textured_rect.h"

extern void eglSetOt(u32 surfaceIndex);
extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte,
                                   s32 ztst);
extern void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);
extern s32 fldComposeFadeColor(s32 fade, s32 color, s32 alpha);
extern u32 fldCameraColorEnabled;
extern SdfFlagListWork *fldCameraColorEffect;
extern sdfTexHandle_t *fldRainTextureReference;
extern FldCameraSetting *fldCameraSettings;
extern FldCameraSetting fldAppliedCameraSettings[];
extern s32 fldCameraColorFade;
extern s32 fldCameraColorLayerScroll;
extern s32 fldCameraColorLayerScrolls[3];

/* One tile of a colour layer, tile (-1, 0 or 1) tiles below the layer's
 * scroll: grey, its alpha the layer's, faded by fldComposeFadeColor by the
 * screen height of its top and of its bottom edge. */
static inline void fldDrawCameraColorTile(s32 x, s32 tile, s32 width, s32 height, s32 textureSize, s32 *layer) {
    u32 top = fldComposeFadeColor(fldCameraColorLayerScroll + tile * height, 0x808080, layer[1]);
    u32 bottom = fldComposeFadeColor(fldCameraColorLayerScroll + tile * height + height, 0x808080, layer[1]);

    evtSubmitTexturedGradientQuad(x, fldCameraColorLayerScroll + tile * height, width, height, 0, 0, textureSize,
                                  textureSize, top, top, bottom, bottom, fldRainTextureReference);
}

/* One column of a colour layer at x: a tile above the layer's scroll, the
 * tile at it, and a tile below. */
static inline void fldDrawCameraColorColumn(s32 x, s32 width, s32 height, s32 textureSize, s32 *layer) {
    fldDrawCameraColorTile(x, -1, width, height, textureSize, layer);
    fldDrawCameraColorTile(x, 0, width, height, textureSize, layer);
    fldDrawCameraColorTile(x, 1, width, height, textureSize, layer);
}

/* The camera colour effect's scrolling layers, each frame. Step their fade
 * (fldCameraColorFade, in percent) toward 0 while the area suppresses the
 * effect and toward 100 otherwise, and update and draw the colour effect
 * unless suppressed, when the camera setting enables it and it exists.
 * Then on surface 0x4F, alpha blended, draw each enabled layer of the
 * applied camera setting (enabled, alpha, speed, flags): tiles 0x100 square,
 * or 0x200 when its flags have any of 0xF00, scrolling down at its speed and
 * across with the camera heading (512 pixels per 180 degrees), in columns at
 * x and x - 512, and for small tiles also x + 256 and x - 256. Upstream:
 * INCLUDE_ASM in DDS1's field/fldFileResolver and DDS2's game/code_00136EF8. */
void fldDrawCameraColorScrollingGradient(void) {
    s32 *layer;
    s32 textureSize;
    s32 size;
    s32 x;
    s32 i;

    if (fldCameraColorEnabled == 0) {
        return;
    }
    if (fldGlobalBase.colorEffectSuppressed != 0) {
        if (fldCameraColorFade > 0) {
            fldCameraColorFade--;
        }
    } else if (fldCameraColorFade < 100) {
        fldCameraColorFade++;
    }
    if (fldGlobalBase.colorEffectSuppressed == 0 && fldCameraSettings->color.enabled != 0 &&
        fldCameraColorEffect != NULL) {
        effUpdateAndDrawSelectionEntries(fldCameraColorEffect);
    }

    /* The heading's place within one 512-pixel period, in [0, 512). */
    x = (s32)(fldGlobalBase.negatedAngle * 2.8444445f);
    x = x / 512 * 512 - x;
    while (x < 0) {
        x += 512;
    }

    eglSetOt(0x4F);
    evtSubmitPrimaryAlphaBlendMode(1);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    textureSize = fldCameraSettings->unk0;
    layer = fldAppliedCameraSettings[0].values[0];
    for (i = 0; i < 3; i++, layer += 4) {
        if (layer[0] == 0) {
            continue;
        }
        size = (layer[3] & 0xF00) ? 0x200 : 0x100;
        fldCameraColorLayerScroll = fldCameraColorLayerScrolls[i] + layer[2];
        fldCameraColorLayerScrolls[i] = fldCameraColorLayerScroll % size;
        fldCameraColorLayerScroll = fldCameraColorLayerScrolls[i];

        fldDrawCameraColorColumn(x, size, size, textureSize, layer);
        if (size < 0x200) {
            fldDrawCameraColorColumn(x + size, size, size, textureSize, layer);
            fldDrawCameraColorColumn(x - 512, size, size, textureSize, layer);
            fldDrawCameraColorColumn(x + size - 512, size, size, textureSize, layer);
        } else {
            fldDrawCameraColorColumn(x - 512, size, size, textureSize, layer);
        }
    }
    evtSubmitPrimaryAlphaBlendMode(0);
}
#else
INCLUDE_ASM(const s32, "game/code_00136EF8", fldDrawCameraColorScrollingGradient);
#endif

void fldInitializeCameraColorResource(void) {
    fldRainTextureResource = sdfDevLoadFileEx(D_00413350, &fldRainTextureData, 0);
    fldRainTextureReference = sdfTexSet((SdfTextureFileHeader *)(fldRainTextureData));
    fldCameraColorEffect = effCreateSelectionFlagListFromWork(fldCameraColorParameters);
    if (fldRainTextureResource != NULL) {
        sdfMemFreeDelayedDirect(fldRainTextureResource);
        fldRainTextureResource = NULL;
    }
    fldUpdateCameraColorEffect(fldCameraSettings);
    fldCameraColorEnabled = 1;
}

void fldDrawCameraColorLayerPass(void) {
    fldDrawCameraColorScrollingGradient();
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

void fldUpdateCameraColorEffect(const FldCameraSetting *setting) {
    const FldColorParams *color = &setting->color;

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
void fldAllocPlaneTable(void) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(FIELD_VALUE_RECORD_STORAGE_SIZE);
    void *storage;

    fldValueRecordResource = allocation;
    storage = (void *)sdfMemLock(allocation);
    fldMatterTable = storage;
    memset(storage, 0, FIELD_VALUE_RECORD_STORAGE_SIZE);
    allocation = sdfMemAllocDirect(FIELD_AUX_RECORD_STORAGE_SIZE);
    fldAuxRecordResource = allocation;
    storage = (void *)sdfMemLock(allocation);
    fldItemBoxObjBuf = storage;
    memset(storage, 0, FIELD_AUX_RECORD_STORAGE_SIZE);
}

/* Release both retained resources and clear the usable buffer addresses. */
void fldFreeMatterTable(void) {
    sdfMemUnlock(fldValueRecordResource);
    sdfMemFreeDelayedDirect(fldValueRecordResource);
    fldMatterTable = NULL;
    sdfMemUnlock(fldAuxRecordResource);
    sdfMemFreeDelayedDirect(fldAuxRecordResource);
    fldItemBoxObjBuf = NULL;
}

/* Dot product of XYZ only; any fourth vector component is ignored. */
float fldVecDot(const float *left, const float *right) {
    return *left * *right + left[1] * right[1] + left[2] * right[2];
}

/* Return the Euclidean XYZ length without normalizing the input vector. */
f32 MATH_VectorLength(const f32 *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

/* Build a normalized plane from three vertices spaced four floats apart.
 * Snap near-zero normal components before deriving D from the third vertex.
 * Degenerate triangles retain the native divide-by-zero behavior. */
void fldMakeEveHitPlanes(const f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant) {
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
struct dds3Basic_t *fldFindWorldNodeForRecordId(s32 recordId) {
    s32 index = 0;

    if (fldValueRecordCount > 0) {
        collision_s *record = fldMatterTable;
        do {
            if (record->resID == recordId) {
                return record->matter;
            }
            index++;
            record++;
        } while (index < fldValueRecordCount);
    }
    return 0;
}

/* Update every matching ID, including duplicates; leave other records intact. */
void fldBindWorldNodeToRecordId(s32 recordId, struct dds3Basic_t *worldNode) {
    s32 index;

    for (index = 0; index < fldValueRecordCount; index++) {
        if (fldMatterTable[index].resID == recordId) {
            fldMatterTable[index].matter = worldNode;
        }
    }
}

void fldClrCollision(void) {
    s32 index;
    for (index = 0; index < fldValueRecordCount; index++) {
        fldMatterTable[index].matter = 0;
    }
    fldValueRecordCount = 0;
    fldGlobalBase.unkA0 = -1;
    fldGlobalBase.unkA4 = -1;
    fldGlobalBase.unkAC = -1;
    D_00436188 = 0;
    D_0043617C = 0;
    if (fldMatterTable != NULL) {
        fldFreeMatterTable();
    }
}

void fldMakePlane(const FldCollisionDescriptor *geometry, const f32 *translation,
                   u32 id, s32 sceneFlag,
                   FldMapSlotAuxiliaryFlagSelector auxiliaryFlagSelector) {
    f32 vertices[4][4];
    f32 copiedVertices[4][4];
    /* Inferred four-point capacity from related native workspace maps;
     * this operation populates only its three defining points. */
    f32 edgeTriangle[4][4];
    f32 center[3];
    f32 capturedPlane[4];
    f32 minimum[2];
    f32 maximum[2];
    collision_s *record;
    s32 pass;
    s32 remaining;
    f32 (*sourceVertices)[4];
    struct FldStopEntry *stopData;
    s32 trianglesRemaining;
    fldCollisionFace_t *face;
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
    if (fldMatterTable == NULL) {
        fldAllocPlaneTable();
    }
    for (pass = 0; pass < 5; pass++) {
        s32 faceCount;
        fldCollisionFace_t *firstFace;
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
                record = &fldMatterTable[fldValueRecordCount];
                record->numedges = trianglesRemaining > 0 ? 3 : 4;
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
                if (record->numedges == 3) {
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
                fldMakeEveHitPlanes(copiedVertices[0], &capturedPlane[0],
                                     &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                for (i = 0; i < 3; i++) {
                    record->normal[i] = capturedPlane[i];
                }
                record->normal[3] = 0.0f;
                for (i = 0; i < record->numedges; i++) {
                    for (component = 0; component < 3; component++) {
                        record->point[i][component] = copiedVertices[i][component];
                    }
                    record->point[i][3] = 1.0f;
                }
                record->num = fldVecDot(record->normal, record->point[0]);
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
                    record->mode2D = FLD_COLLISION_AXIS_X;
                    runningMinimum = vertices[0][1];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][2];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->numedges; i++) {
                        if (vertices[i][1] < runningMinimum) {
                            runningMinimum = vertices[i][1];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][2] < minimum[1]) minimum[1] = vertices[i][2];
                        if (vertices[i][1] > maximum[0]) maximum[0] = vertices[i][1];
                        if (vertices[i][2] > maximum[1]) maximum[1] = vertices[i][2];
                    }
                    record->min[0] = runningMinimum;
                    record->min[1] = minimum[1];
                    record->min[2] = maximum[0];
                    record->min[3] = maximum[1];
                    if (record->numedges == 3) {
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
                    planeX = &record->e_normal[0][0];
                    planeY = &record->e_normal[0][1];
                    planeZ = &record->e_normal[0][2];
                    limit0 = &record->e_num[0];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    if (fldVecDot(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldVecDot(planeX, edgeTriangle[0]);
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
                    planeX = &record->e_normal[1][0];
                    planeY = &record->e_normal[1][1];
                    planeZ = &record->e_normal[1][2];
                    limit1 = &record->e_num[1];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    if (fldVecDot(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    }
                    if (record->numedges == 3) {
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
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
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
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
                        planeX = &record->e_normal[3][0];
                        planeY = &record->e_normal[3][1];
                        planeZ = &record->e_normal[3][2];
                        limit3 = &record->e_num[3];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    }
                }
                else if (absX <= absY && absZ <= absY) {
                    record->mode2D = FLD_COLLISION_AXIS_Y;
                    runningMinimum = vertices[0][0];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][2];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->numedges; i++) {
                        if (vertices[i][0] < runningMinimum) {
                            runningMinimum = vertices[i][0];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][2] < minimum[1]) minimum[1] = vertices[i][2];
                        if (vertices[i][0] > maximum[0]) maximum[0] = vertices[i][0];
                        if (vertices[i][2] > maximum[1]) maximum[1] = vertices[i][2];
                    }
                    record->min[0] = runningMinimum;
                    record->min[1] = minimum[1];
                    record->min[2] = maximum[0];
                    record->min[3] = maximum[1];
                    if (record->numedges == 3) {
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
                    planeX = &record->e_normal[0][0];
                    planeY = &record->e_normal[0][1];
                    planeZ = &record->e_normal[0][2];
                    limit0 = &record->e_num[0];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    if (fldVecDot(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldVecDot(planeX, edgeTriangle[0]);
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
                    planeX = &record->e_normal[1][0];
                    planeY = &record->e_normal[1][1];
                    planeZ = &record->e_normal[1][2];
                    limit1 = &record->e_num[1];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    if (fldVecDot(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    }
                    if (record->numedges == 3) {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
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
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
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
                        planeX = &record->e_normal[3][0];
                        planeY = &record->e_normal[3][1];
                        planeZ = &record->e_normal[3][2];
                        limit3 = &record->e_num[3];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    }
                }
                else {
                    record->mode2D = FLD_COLLISION_AXIS_Z;
                    runningMinimum = vertices[0][0];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][1];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->numedges; i++) {
                        if (vertices[i][0] < runningMinimum) {
                            runningMinimum = vertices[i][0];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][1] < minimum[1]) minimum[1] = vertices[i][1];
                        if (vertices[i][0] > maximum[0]) maximum[0] = vertices[i][0];
                        if (vertices[i][1] > maximum[1]) maximum[1] = vertices[i][1];
                    }
                    record->min[0] = runningMinimum;
                    record->min[1] = minimum[1];
                    record->min[2] = maximum[0];
                    record->min[3] = maximum[1];
                    if (record->numedges == 3) {
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
                    planeX = &record->e_normal[0][0];
                    planeY = &record->e_normal[0][1];
                    planeZ = &record->e_normal[0][2];
                    limit0 = &record->e_num[0];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    if (fldVecDot(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldVecDot(planeX, edgeTriangle[0]);
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
                    planeX = &record->e_normal[1][0];
                    planeY = &record->e_normal[1][1];
                    planeZ = &record->e_normal[1][2];
                    limit1 = &record->e_num[1];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    if (fldVecDot(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    }
                    if (record->numedges == 3) {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
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
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
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
                        planeX = &record->e_normal[3][0];
                        planeY = &record->e_normal[3][1];
                        planeZ = &record->e_normal[3][2];
                        limit3 = &record->e_num[3];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    }
                }
                record->blockID = D_00436188;
                record->resID = id;
                record->matter = 0;
                record->pFace = face;
                record->pTome = stopData;
                record->hojiID = sceneFlag;
                record->npcID = auxiliaryFlagSelector;
                record->hitflag = 0;
                fldValueRecordCount++;
            }
        }
    }
    D_00436188++;
}

s32 HitPositionInside2(f32 *out, FldCollisionAxis axis, s32 count, f32 margin, const f32 *pos,
                        const collision_s *zone) {
    f32 probe[3];
    f32 planar[2];
    f32 best = margin;
    f32 dist;
    f32 slack;
    s32 inside = 0;
    s32 planeIndex;

    if (axis == FLD_COLLISION_AXIS_X) {
        probe[0] = 0.0f;
        probe[1] = pos[1];
        probe[2] = pos[2];
        planar[0] = pos[1];
        planar[1] = pos[2];
    } else if (axis == FLD_COLLISION_AXIS_Y) {
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
        dist = fldVecDot(probe, zone->e_normal[planeIndex]) - zone->e_num[planeIndex];
        slack = dist + margin;
        if (slack < 0.0f) {
            *out = -1.0f;
            return -1;
        }
        if (planar[0] < zone->min[0] - margin || planar[1] < zone->min[1] - margin ||
            zone->min[2] + margin < planar[0] || zone->min[3] + margin < planar[1]) {
            *out = -1.0f;
            return -1;
        }
        if (dist < 0.0f) {
            /* The mode-1 plane uses a fixed 45-unit inset, unlike the
               other projections, which use the caller's margin. */
            if (axis == FLD_COLLISION_AXIS_Y) {
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

extern f32 fldResolvedPosition[3];

extern u8 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit);

extern u8 fldTestMapSlotAuxiliaryFlag(
    s32 mapId, u32 slotIndex, FldMapSlotAuxiliaryFlagSelector bit);

void fldHitChkUnitElvOnly(const f32 *input) {
    f32 position[4];
    f32 previous[4];
    f32 savedPosition[4];
    f32 savedPrevious[4];
    f32 absoluteDelta[4];
    f32 clearance;
    collision_s *record;
    dds3PoseSet_t *transform;
    dds3PoseSet_t *anchor;
    f32 distance;
    s32 relative;
    s32 inside;
    s32 index;
    f32 margin;

    previous[1] = fldResolvedPosition[1];
    position[1] = input[1] - 35.0f;
    position[0] = input[0];
    position[2] = input[2];
    previous[0] = fldResolvedPosition[0];
    previous[2] = fldResolvedPosition[2];
    absoluteDelta[0] = ffabsf(input[0] - fldResolvedPosition[0]);
    absoluteDelta[2] = ffabsf(input[2] - fldResolvedPosition[2]);
    margin = 45.0f;
    for (index = 0; index < fldValueRecordCount; index++) {
        if (!(fldMatterTable[index].pFace->attributes & 0x80)) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                 fldMatterTable[index].hojiID)) {
            continue;
        }
        if (fldMatterTable[index].npcID > 0) {
            if (fldTestMapSlotAuxiliaryFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                            fldMatterTable[index].npcID)) {
                continue;
            }
        }
        savedPosition[0] = position[0];
        savedPosition[1] = position[1];
        savedPosition[2] = position[2];
        savedPrevious[0] = previous[0];
        savedPrevious[1] = previous[1];
        savedPrevious[2] = previous[2];
        if (fldMatterTable[index].matter != 0) {
            transform = fldMatterTable[index].matter->poseset;
            relative = 1;
            position[0] -= transform->pos[0];
            position[1] -= transform->pos[1];
            position[2] -= transform->pos[2];
            previous[0] -= transform->pos[0];
            previous[1] -= transform->pos[1];
            previous[2] -= transform->pos[2];
        } else {
            relative = 0;
        }
        distance = fldVecDot(position, fldMatterTable[index].normal);
        distance -= fldMatterTable[index].num;
        if (ffabsf(distance) < margin) {
            record = &fldMatterTable[index];
            HitPositionInside2(&clearance, record->mode2D, record->numedges, 45.0f, position, record);
            inside = clearance < 0.0f ? 0 : 1;
            if (inside == 1) {
                if (fldMatterTable[index].matter != 0) {
                    if (fldGlobalBase.primaryMovingMatterIndex == -1 ||
                        fldMatterTable[fldGlobalBase.primaryMovingMatterIndex].matter->key ==
                            fldMatterTable[index].matter->key) {
                        fldGlobalBase.primaryMovingMatterIndex = index;
                    } else {
                        fldGlobalBase.secondaryMovingMatterIndex = index;
                    }
                    anchor = fldMatterTable[index].matter->poseset;
                    fldMatterTable[index].previousPosition[0] = anchor->pos[0];
                    fldMatterTable[index].previousPosition[1] = anchor->pos[1];
                    fldMatterTable[index].previousPosition[2] = anchor->pos[2];
                }
            }
        }
        if (relative) {
            transform = fldMatterTable[index].matter->poseset;
            position[0] += transform->pos[0];
            position[1] += transform->pos[1];
            position[2] += transform->pos[2];
            previous[0] += transform->pos[0];
            previous[1] += transform->pos[1];
            previous[2] += transform->pos[2];
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
    fldGlobalBase.positionPending = FLD_POSITION_UPDATE_NONE;
    if (fldGlobalBase.primaryMovingMatterIndex == -1) {
        fldHitChkUnitElvOnly(position);
    }
    index = fldGlobalBase.primaryMovingMatterIndex;
    if (index != -1) {
        if (fldMatterTable[index].matter != 0) {
            dds3PoseSet_t *transform;

            transform = fldMatterTable[index].matter->poseset;
            movement[0] = transform->pos[0] - fldMatterTable[index].previousPosition[0];
            movement[1] = transform->pos[1] - fldMatterTable[index].previousPosition[1];
            movement[2] = transform->pos[2] - fldMatterTable[index].previousPosition[2];
            fldMatterTable[index].previousPosition[0] = transform->pos[0];
            fldMatterTable[index].previousPosition[1] = transform->pos[1];
            fldMatterTable[index].previousPosition[2] = transform->pos[2];
        }
        if (fldTestRoomSceneFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                               fldMatterTable[index].hojiID) == 0) {
            index = fldGlobalBase.secondaryMovingMatterIndex;
            if (index != -1 && fldTestRoomSceneFlag(fldGlobalBase.area,
                        fldGlobalBase.floor + 1,
                        fldMatterTable[index].hojiID) == 0) {
                if (fldMatterTable[index].matter != 0) {
                    dds3PoseSet_t *transform;

                    transform = fldMatterTable[index].matter->poseset;
                    movement[0] += transform->pos[0] - fldMatterTable[index].previousPosition[0];
                    movement[1] += transform->pos[1] - fldMatterTable[index].previousPosition[1];
                    movement[2] += transform->pos[2] - fldMatterTable[index].previousPosition[2];
                    fldMatterTable[index].previousPosition[0] = transform->pos[0];
                    fldMatterTable[index].previousPosition[1] = transform->pos[1];
                    fldMatterTable[index].previousPosition[2] = transform->pos[2];
                }
            }
            newPosition[0] = position[0] + movement[0];
            newPosition[1] = position[1] + movement[1];
            newPosition[2] = position[2] + movement[2];
            previous[0] = fldResolvedPosition[0];
            previous[1] = fldResolvedPosition[1];
            previous[2] = fldResolvedPosition[2];
            fldGlobalBase.positionPending = FLD_POSITION_UPDATE_PENDING;
            fldGlobalBase.targetX = newPosition[0];
            fldGlobalBase.targetY = newPosition[1];
            fldGlobalBase.targetZ = newPosition[2];
            position[0] = newPosition[0];
            position[1] = newPosition[1];
            position[2] = newPosition[2];
            fldResolvedPosition[0] = newPosition[0];
            fldResolvedPosition[1] = newPosition[1];
            fldResolvedPosition[2] = newPosition[2];
        }
    }
}

/* Non-owning native alias of fldGlobalBase.terrainAttritionKind (FldAreaWork + 0x11C). */
extern FldTerrainAttritionKind D_0038988C[];

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
    fldAreaWork_t *areaState;
    FldCollisionAxis selectedAxis;
    s32 pass;
    s32 recordIndex;

    D_0038988C[0] = 0;

    /* Capture both snapshots before lowering the current probe for testing. */
    point[0] = position[0];
    point[1] = position[1];
    point[2] = position[2];
    reference[0] = fldResolvedPosition[0];
    reference[1] = fldResolvedPosition[1];
    reference[2] = fldResolvedPosition[2];
    point[1] -= 35.0f;
    distanceVector[0] = point[0] - reference[0];
    distanceVector[1] = point[1] - reference[1];
    distanceVector[2] = point[2] - reference[2];

    if (MATH_VectorLength(distanceVector) < 0.1f) {
        position[0] = reference[0];
        position[1] = reference[1];
        position[2] = reference[2];
        return;
    }

    axisDistance[0] = ffabsf(point[0] - reference[0]);
    axisDistance[2] = ffabsf(point[2] - reference[2]);
    selectedAxis = axisDistance[2] < axisDistance[0] ? FLD_COLLISION_AXIS_X : FLD_COLLISION_AXIS_Z;

    fldHitChkUnitElvOnly(position);

    /* Linked-node translations accumulate across records and both passes. */
    for (pass = 0; pass < 2; pass++) {
        recordIndex = 0;
        if (fldValueRecordCount > 0) {
            areaState = &fldGlobalBase;
            do {
                dds3Basic_t *node;

                margin = fldMatterTable[recordIndex].mode2D == FLD_COLLISION_AXIS_Y ? 45.0f : 60.0f;
                if (pass == 0) {
                    if (fldMatterTable[recordIndex].mode2D == selectedAxis) {
                        continue;
                    }
                } else if (fldMatterTable[recordIndex].mode2D != selectedAxis) {
                    continue;
                }

                if (fldTestRoomSceneFlag(areaState->area, areaState->floor + 1, fldMatterTable[recordIndex].hojiID) != 0) {
                    continue;
                }

                savedPoint[0] = point[0];
                savedPoint[1] = point[1];
                savedPoint[2] = point[2];
                savedReference[0] = reference[0];
                savedReference[1] = reference[1];
                savedReference[2] = reference[2];

                node = fldMatterTable[recordIndex].matter;
                if (node != 0) {
                    point[0] = savedPoint[0] - node->poseset->pos[0];
                    point[1] = savedPoint[1] - node->poseset->pos[1];
                    point[2] = savedPoint[2] - node->poseset->pos[2];
                    reference[0] = savedReference[0] - node->poseset->pos[0];
                    reference[1] = savedReference[1] - node->poseset->pos[1];
                    reference[2] = savedReference[2] - node->poseset->pos[2];
                }

                planeDistance = fldVecDot(point, fldMatterTable[recordIndex].normal);
                {
                    u32 faceAttributes = fldMatterTable[recordIndex].pFace->attributes;
                    planeDistance = ffabsf(planeDistance - fldMatterTable[recordIndex].num);
                    if ((faceAttributes & 0x4000) == 0) {
                        continue;
                    }
                    {
                        s32 accepted = 0;
                        if (!(planeDistance < margin)) {
                            continue;
                        }
                        HitPositionInside2(
                            &classifiedMargin,
                            fldMatterTable[recordIndex].mode2D,
                            fldMatterTable[recordIndex].numedges,
                            margin, point,
                            &fldMatterTable[recordIndex]);
                        if (!(classifiedMargin < 0.0f)) {
                            accepted = 1;
                        }
                        if (accepted == 1) {
                            fldCollisionFace_t *face = fldMatterTable[recordIndex].pFace;
                            s32 soundClass = face->sound & 0xF0;
                            if (soundClass == 0x20) {
                                areaState->terrainAttritionKind = FLD_TERRAIN_ATTRITION_SOUND_CLASS_20;
                            } else if (soundClass == 0x30) {
                                areaState->terrainAttritionKind = FLD_TERRAIN_ATTRITION_SOUND_CLASS_30;
                            } else {
                                areaState->terrainAttritionKind = FLD_TERRAIN_ATTRITION_STANDARD;
                            }
                        }
                    }
                }
            } while (++recordIndex < fldValueRecordCount);
        }
    }
}

#include "common.h"
#include "fld_area_work.h"
#include "fld.h"
#include "fpu.h"
#include "dds3obj.h"

extern f32 MATH_VectorLength(const f32 *vector);





extern void fldAutoMapNameD(s32 labelIndex);
extern void fldAutoMapNameU(s32 labelIndex);
extern void fldSetCurrentFloorFlag(s32 flagNumber);
extern void fldTransitionSkyLightSet(s32 lightSetIndex, s32 mode);
extern void fldApplyPendingSceneValueWithSpeed(s32 speed);
extern void rsetOtOfs(u32 displayIndex);
extern void rsetTest(s32 flag0, s32 flag1, s32 dataOffset, s32 field12, s32 flag14, s32 flag15, s32 unused,
                               s32 field17);
extern void rsetAlpha(s32 mode);
extern collision_s *fldMatterTable;
extern s32 fldValueRecordCount;
extern u32 fldRunDanteGunFlag[];
extern s8 dds3PadButtonState[2][2][16];
extern f32 fldResolvedPosition[3];

/* Move the field position toward input against the room's collision faces.
 * Carry it by the motion of a moving support it stood on; stop at the last
 * resolved position (fldResolvedPosition) when it moves less than 0.1. Else
 * test each face in two passes (faces off the major axis of movement first,
 * skipping those a room scene flag or the owner's map slot flag hides) and
 * apply each face the position lies within its margin of (45, or 60 off the
 * floor axis): map labels, encounter and sound zones, specials (sky light,
 * height, colour effect), automap flags, the support record, and for a solid
 * face (a floor, or a wall met from its front) a push out along its normal,
 * which becomes the target. Then start the sky light change and write the
 * result to input and fldResolvedPosition. With fldRunDanteGunFlag[6] set
 * it first sets up display buffer 0x5E for debug drawing;
 * fldRunDanteGunFlag[19] with the first pad's button 11 held lets the
 * position through solid faces. DDS1 has a different FldAreaWork layout and
 * is qualified separately; do not treat the twins as identical.
 * Retail oddities kept: it copies the position before making it relative to a
 * moving face and never reads the copy; a special face leaves the position
 * relative to its moving face; the final movement length is computed and
 * discarded. The small contact-flag bindings reconstruct allocation, not
 * original-source spelling; see docs/compiler-trace-area-refresh.md. */
void fldRefreshAreaStateFromPosition(f32 *input) {
    f32 position[4];
    f32 previous[4];
    f32 projected[4];
    f32 delta[4];
    f32 savedPosition[4];
    f32 savedPrevious[4];
    f32 absoluteDelta[4];
    f32 carried[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    f32 proposedX, proposedY, proposedZ;
    f32 clearance;
    f32 contactInset;
    f32 margin;
    f32 distance;
    f32 absoluteDistance;
    s32 majorAxis;
    s32 floorRecord;
    s32 pending;
    s32 index;
    s32 pass;
    s32 relative;
    s32 inside;
    s32 display;
    s32 sound;
    u32 attributes;
    dds3PoseSet_t *transform;

    pending = -1;
    floorRecord = -1;
    display = fldRunDanteGunFlag[6] != 0;
    fldGlobalBase.unkA0 = -1;
    fldGlobalBase.overrideSupportRecordIndex = -1;
    fldGlobalBase.unkA4 = -1;
    fldAutoMapNameD(-1);
    fldGlobalBase.unk8C = 0;
    if (fldGlobalBase.primaryMovingMatterIndex != -1) {
        dds3PoseSet_t *carrier;

        index = fldGlobalBase.primaryMovingMatterIndex;
        if (fldMatterTable[index].matter != NULL) {
            carrier = fldMatterTable[index].matter->poseset;
            carried[0] = carrier->pos[0] - fldMatterTable[index].previousPosition[0];
            carried[1] = carrier->pos[1] - fldMatterTable[index].previousPosition[1] + 10.0f;
            carried[2] = carrier->pos[2] - fldMatterTable[index].previousPosition[2];
        }
        fldGlobalBase.primaryMovingMatterIndex = -1;
        fldGlobalBase.secondaryMovingMatterIndex = -1;
    }
    fldGlobalBase.primaryMovingMatterIndex = -1;
    fldGlobalBase.secondaryMovingMatterIndex = -1;
    if (display) {
        rsetOtOfs(0x5E);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
        rsetAlpha(0);
    }
    previous[0] = fldResolvedPosition[0];
    previous[1] = fldResolvedPosition[1];
    previous[2] = fldResolvedPosition[2];
    proposedX = input[0];
    proposedY = input[1];
    proposedZ = input[2];
    proposedX += carried[0];
    proposedY += carried[1];
    proposedZ += carried[2];
    proposedY -= 35.0f;
    /* Commit carried horizontal coordinates before the adjusted height. */
    position[0] = proposedX;
    position[2] = proposedZ;
    position[1] = proposedY;
    delta[0] = position[0] - previous[0];
    delta[1] = position[1] - previous[1];
    delta[2] = position[2] - previous[2];
    if (MATH_VectorLength(delta) < 0.09999999404f) {
        input[0] = previous[0];
        input[1] = previous[1];
        input[2] = previous[2];
        return;
    }
    for (index = 0; index < fldValueRecordCount; index++) {
        fldMatterTable[index].hitflag = 0;
    }
    fldGlobalBase.unk98 = -1;
    fldGlobalBase.colorEffectSuppressed = 0;
    fldGlobalBase.encounterOverrideId = 0;
    absoluteDelta[0] = ffabsf(position[0] - previous[0]);
    absoluteDelta[2] = ffabsf(position[2] - previous[2]);
    majorAxis = 0;
    if (!(absoluteDelta[2] < absoluteDelta[0])) {
        majorAxis = 2;
    }
    fldHitChkUnitElvOnly(input);
    for (pass = 0; pass < 2; pass++) {
        for (index = 0; index < fldValueRecordCount; index++) {
            if (fldMatterTable[index].mode2D == 1) {
                margin = 45.0f;
            } else {
                margin = 60.0f;
            }
            if (pass == 0) {
                if (fldMatterTable[index].mode2D == majorAxis) {
                    continue;
                }
            } else if (fldMatterTable[index].mode2D != majorAxis) {
                continue;
            }
            if (fldTestRoomSceneFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, fldMatterTable[index].hojiID)) {
                continue;
            }
            if (fldMatterTable[index].npcID > 0 &&
                fldTestMapSlotAuxiliaryFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                            fldMatterTable[index].npcID)) {
                continue;
            }
            savedPosition[0] = position[0];
            savedPosition[1] = position[1];
            savedPosition[2] = position[2];
            savedPrevious[0] = previous[0];
            savedPrevious[1] = previous[1];
            savedPrevious[2] = previous[2];
            if (fldMatterTable[index].matter != NULL) {
                transform = fldMatterTable[index].matter->poseset;
                position[0] -= transform->pos[0];
                position[1] -= transform->pos[1];
                position[2] -= transform->pos[2];
                previous[0] -= transform->pos[0];
                previous[1] -= transform->pos[1];
                previous[2] -= transform->pos[2];
                relative = 1;
            } else {
                relative = 0;
            }
            distance = fldVecDot(position, fldMatterTable[index].normal);
            distance -= fldMatterTable[index].num;
            absoluteDistance = ffabsf(distance);
            attributes = fldMatterTable[index].pFace->attributes;
            if (attributes & 0x40) {
                if (absoluteDistance < margin) {
                    HitPositionInside2(&clearance, fldMatterTable[index].mode2D,
                                                        fldMatterTable[index].numedges, margin, position,
                                                        &fldMatterTable[index]);
                    inside = clearance < 0.0f ? 0 : 1;
                    if (inside == 1) {
                        fldAutoMapNameD((fldMatterTable[index].pFace->moveFloor & 0xF) * 10 +
                                                         (fldMatterTable[index].pFace->moveFloor >> 4));
                    }
                }
            } else if (attributes & 0x1000) {
                if (absoluteDistance < margin) {
                    HitPositionInside2(&clearance, fldMatterTable[index].mode2D,
                                                        fldMatterTable[index].numedges, margin, position,
                                                        &fldMatterTable[index]);
                    inside = clearance < 0.0f ? 0 : 1;
                    if (inside == 1) {
                        fldAutoMapNameD(fldMatterTable[index].pFace->place);
                    }
                }
            } else if (attributes & 0x2000) {
                if (absoluteDistance < margin) {
                    HitPositionInside2(&clearance, fldMatterTable[index].mode2D,
                                                        fldMatterTable[index].numedges, margin, position,
                                                        &fldMatterTable[index]);
                    inside = clearance < 0.0f ? 0 : 1;
                    if (inside == 1) {
                        fldGlobalBase.encounterOverrideId = fldMatterTable[index].pFace->encounter;
                    }
                }
            } else if (attributes & 0x200) {
                if (absoluteDistance < margin) {
                    HitPositionInside2(&clearance, fldMatterTable[index].mode2D,
                                                        fldMatterTable[index].numedges, margin, position,
                                                        &fldMatterTable[index]);
                    inside = clearance < 0.0f ? 0 : 1;
                    if (inside == 1) {
                        sound = fldMatterTable[index].pFace->sound;
                        fldGlobalBase.unk8C = sound;
                        if (sound == 3) {
                            fldGlobalBase.unk98 = index;
                        }
                    }
                }
            } else if (attributes & 0x8000) {
                if (absoluteDistance < margin) {
                    HitPositionInside2(&clearance, fldMatterTable[index].mode2D,
                                                        fldMatterTable[index].numedges, margin, position,
                                                        &fldMatterTable[index]);
                    inside = clearance < 0.0f ? 0 : 1;
                    if (inside == 1) {
                        switch (fldMatterTable[index].pFace->special[0]) {
                        case 1:
                            pending = fldMatterTable[index].pFace->special[1];
                            break;
                        case 2:
                            break;
                        case 3:
                            fldGlobalBase.unk9C = fldMatterTable[index].point[0][1];
                            break;
                        case 5:
                            fldGlobalBase.colorEffectSuppressed = 1;
                            break;
                        }
                    }
                }
            } else if (attributes & 0x400) {
                continue;
            } else if (attributes & 0x800) {
                if (absoluteDistance < margin) {
                    HitPositionInside2(&clearance, fldMatterTable[index].mode2D,
                                                        fldMatterTable[index].numedges, margin, position,
                                                        &fldMatterTable[index]);
                    inside = clearance < 0.0f ? 0 : 1;
                    if (inside == 1) {
                        fldSetCurrentFloorFlag(fldMatterTable[index].pFace->automap[0]);
                        if (fldMatterTable[index].pFace->automap[1] != 0) {
                            fldAutoMapNameU(fldMatterTable[index].pFace->automap[1]);
                        }
                    }
                }
            } else if (attributes & 0x4000) {
                continue;
            } else if (attributes & 0x100) {
                if (absoluteDistance < margin) {
                    HitPositionInside2(&clearance, fldMatterTable[index].mode2D,
                                                        fldMatterTable[index].numedges, margin, position,
                                                        &fldMatterTable[index]);
                    inside = clearance < 0.0f ? 0 : 1;
                    if (inside == 1) {
                        fldGlobalBase.overrideSupportRecordIndex = index;
                    }
                }
            } else {
                if (absoluteDistance < margin) {
                    HitPositionInside2(&clearance, fldMatterTable[index].mode2D,
                                                        fldMatterTable[index].numedges, margin, position,
                                                        &fldMatterTable[index]);
                    contactInset = clearance;
                    inside = contactInset < 0.0f ? 0 : 1;
                    if (inside == 1 && !(fldRunDanteGunFlag[19] != 0 && dds3PadButtonState[1][0][11] != 0)) {
                        projected[0] = position[0] - fldMatterTable[index].normal[0] * distance;
                        projected[1] = position[1] - fldMatterTable[index].normal[1] * distance;
                        projected[2] = position[2] - fldMatterTable[index].normal[2] * distance;
                        if (fldMatterTable[index].normal[1] < -0.75f) {
                            /* A floor: stand on it, a unit below the face. */
                            f32 resolvedClearance;
                            fldGlobalBase.unkA4 = index;
                            if (floorRecord >= 0 && contactInset < absoluteDistance) {
                                clearance = absoluteDistance;
                            }
                            /* One resolved distance owns this vector reconstruction. */
                            resolvedClearance = clearance;
                            position[0] = projected[0] + fldMatterTable[index].normal[0] * resolvedClearance;
                            position[1] = projected[1] + fldMatterTable[index].normal[1] * resolvedClearance - 1.0f;
                            position[2] = projected[2] + fldMatterTable[index].normal[2] * resolvedClearance;
                            if (resolvedClearance == margin) {
                                floorRecord = index;
                            }
                            if (fldMatterTable[index].matter != NULL) {
                                transform = fldMatterTable[index].matter->poseset;
                                position[0] += transform->pos[0];
                                position[1] += transform->pos[1];
                                position[2] += transform->pos[2];
                                previous[0] += transform->pos[0];
                                previous[1] += transform->pos[1];
                                previous[2] += transform->pos[2];
                                relative = 0;
                            }
                            {
                                /* Keep this real contact-store constant independent of
                                 * the loop classifier's live 1. Qualified compiler
                                 * traces otherwise reuse that value instead of the
                                 * native local definition. No call crosses this
                                 * binding; the halfword store is its only consumer.
                                 * Allocation reconstruction, not original spelling.
                                 * See docs/compiler-trace-area-refresh.md. */
                                register u32 contactActive __asm__("$2") = 1;
                                fldMatterTable[index].hitflag = contactActive;
                            }
                            fldGlobalBase.targetX = position[0];
                            fldGlobalBase.targetY = position[1];
                            fldGlobalBase.targetZ = position[2];
                            fldGlobalBase.positionPending = 1;
                            fldGlobalBase.unkAC = index;
                        } else if (!(distance < 0.0f)) {
                            /* A wall, approached from its front. */
                            f32 resolvedClearance;
                            fldGlobalBase.unkA0 = index;
                            if (contactInset < absoluteDistance) {
                                clearance = absoluteDistance;
                            }
                            /* One resolved distance owns this vector reconstruction. */
                            resolvedClearance = clearance;
                            position[0] = projected[0] + fldMatterTable[index].normal[0] * resolvedClearance;
                            position[1] = projected[1] + fldMatterTable[index].normal[1] * resolvedClearance;
                            position[2] = projected[2] + fldMatterTable[index].normal[2] * resolvedClearance;
                            if (fldMatterTable[index].matter != NULL) {
                                transform = fldMatterTable[index].matter->poseset;
                                position[0] += transform->pos[0];
                                position[1] += transform->pos[1];
                                position[2] += transform->pos[2];
                                previous[0] += transform->pos[0];
                                previous[1] += transform->pos[1];
                                previous[2] += transform->pos[2];
                                relative = 0;
                            }
                            {
                                /* Keep this real contact-store constant independent of
                                 * the loop classifier's live 1. Qualified compiler
                                 * traces otherwise reuse that value instead of the
                                 * native local definition. No call crosses this
                                 * binding; the halfword store is its only consumer.
                                 * Allocation reconstruction, not original spelling.
                                 * See docs/compiler-trace-area-refresh.md. */
                                register u32 contactActive __asm__("$2") = 1;
                                fldMatterTable[index].hitflag = contactActive;
                            }
                            fldGlobalBase.positionPending = inside;
                            fldGlobalBase.targetX = position[0];
                            fldGlobalBase.targetY = position[1];
                            fldGlobalBase.targetZ = position[2];
                            fldGlobalBase.unkAC = index;
                        }
                    }
                }
                if (relative) {
                    transform = fldMatterTable[index].matter->poseset;
                    position[0] += transform->pos[0];
                    position[1] += transform->pos[1];
                    position[2] += transform->pos[2];
                    previous[0] += transform->pos[0];
                    previous[1] += transform->pos[1];
                    previous[2] += transform->pos[2];
                }
            }
        }
    }
    if (pending != -1) {
        fldTransitionSkyLightSet(pending, 5);
    } else {
        fldApplyPendingSceneValueWithSpeed(5);
    }
    delta[0] = position[0] - previous[0];
    delta[1] = position[1] - previous[1];
    delta[2] = position[2] - previous[2];
    MATH_VectorLength(delta);
    input[0] = position[0];
    input[1] = position[1];
    input[2] = position[2];
    fldResolvedPosition[0] = position[0];
    fldResolvedPosition[1] = position[1];
    fldResolvedPosition[2] = position[2];
}

void func_0013AA78(void) {
}

void func_0013AA80(void) {
}

void func_0013AA88(void) {
}

/* Project away the selected axis and reject points outside expanded bounds.
 * Return the last negative plane's margin-adjusted distance, not a minimum;
 * no negative plane leaves margin unchanged. Reject results below 0.001. */
f32 HitPositionInside3(FldCollisionAxis axis, s32 planeCount, f32 margin, const f32 *position,
                       const collision_s *zone) {
    f32 projectedPosition[3];
    f32 planarPosition[2];
    f32 clearance = margin;
    f32 planeDistance;
    f32 marginDistance;
    s32 planeIndex;

    if (axis == FLD_COLLISION_AXIS_X) {
        projectedPosition[0] = 0.0f;
        projectedPosition[1] = position[1];
        projectedPosition[2] = position[2];
        planarPosition[0] = position[1];
        planarPosition[1] = position[2];
    } else if (axis == FLD_COLLISION_AXIS_Y) {
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
        planeDistance = fldVecDot(projectedPosition, zone->e_normal[planeIndex]) - zone->e_num[planeIndex];
        marginDistance = planeDistance + margin;
        if (marginDistance < 0.0f || planarPosition[0] < zone->min[0] - margin || planarPosition[1] < zone->min[1] - margin ||
            zone->min[2] + margin < planarPosition[0] || zone->min[3] + margin < planarPosition[1]) {
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

extern void fldToggleWorldNodeState(s64 enabled);

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value);

FldValueRecordIndex fldHitChkCamera(const f32 start[3], const f32 end[3], f32 hitPoint[3]) {
    f32 origin[4];
    f32 target[4];
    f32 delta[4];
    f32 probeStart[4];
    f32 probeEnd[4];
    f32 anchor[4];
    dds3PoseSet_t *transform;
    dds3Basic_t *secondary;
    f32 startDistance;
    f32 endDistance;
    f32 total;
    f32 ratio;
    f32 clearance;
    FldValueRecordIndex found;
    FldStopEntry *entry;
    s32 relative;
    FldValueRecordIndex index;

    found = FLD_VALUE_RECORD_INDEX_NO_HIT;
    secondary = dds3GetControlScene();
    if (!((u32)(fldGlobalBase.area - 200) < 300)) {
        fldToggleWorldNodeState(0);
    }
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    target[0] = end[0];
    target[1] = end[1];
    target[2] = end[2];
    for (index = 0; index < fldValueRecordCount; index++) {
        if (fldMatterTable[index].mode2D == FLD_COLLISION_AXIS_Y) {
            if (!(fldMatterTable[index].pFace->attributes & 0x100)) {
                continue;
            }
        }
        if (fldMatterTable[index].pFace->attributes & 0xA01) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                 fldMatterTable[index].hojiID)) {
            continue;
        }
        if (fldMatterTable[index].matter != 0) {
            relative = 1;
            transform = fldMatterTable[index].matter->poseset;
            origin[0] -= transform->pos[0];
            origin[1] -= transform->pos[1];
            origin[2] -= transform->pos[2];
            target[0] -= transform->pos[0];
            target[1] -= transform->pos[1];
            target[2] -= transform->pos[2];
        } else {
            relative = 0;
        }
        probeStart[0] = origin[0];
        probeStart[1] = origin[1];
        probeStart[2] = origin[2];
        probeEnd[0] = target[0];
        probeEnd[1] = target[1];
        probeEnd[2] = target[2];
        startDistance = fldVecDot(probeStart, fldMatterTable[index].normal) -
                        fldMatterTable[index].num;
        endDistance = fldVecDot(probeEnd, fldMatterTable[index].normal) -
                      fldMatterTable[index].num;
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
            clearance = HitPositionInside3(fldMatterTable[index].mode2D, fldMatterTable[index].numedges, 2.0f, probeStart, &fldMatterTable[index]);
            if (0.0f <= clearance) {
                if (fldMatterTable[index].pFace->attributes & 0x400) {
                    entry = &((FldStopEntry *)fldMatterTable[index].pTome)
                                [fldMatterTable[index].pFace->stop];
                    dds3MatterObjectBasicSetAlpMode(dds3SceneBasicSearchNameAndType(secondary, 6, entry->name), 4);
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
            transform = fldMatterTable[index].matter->poseset;
            origin[0] += transform->pos[0];
            origin[1] += transform->pos[1];
            origin[2] += transform->pos[2];
            target[0] += transform->pos[0];
            target[1] += transform->pos[1];
            target[2] += transform->pos[2];
        }
    }
    hitPoint[0] = origin[0];
    hitPoint[1] = origin[1];
    hitPoint[2] = origin[2];
    return found;
}

FldValueRecordIndex fldHitChkEnemy(const f32 start[3], const f32 end[3], f32 hitPoint[3]) {
    f32 origin[4];
    f32 target[4];
    f32 delta[4];
    f32 probeStart[4];
    f32 probeEnd[4];
    f32 anchor[4];
    dds3PoseSet_t *transform;
    f32 startDistance;
    f32 endDistance;
    f32 total;
    f32 ratio;
    f32 clearance;
    FldValueRecordIndex found;
    s32 relative;
    FldValueRecordIndex index;

    found = FLD_VALUE_RECORD_INDEX_NO_HIT;
    dds3GetControlScene();
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    target[0] = end[0];
    target[1] = end[1];
    target[2] = end[2];
    for (index = 0; index < fldValueRecordCount; index++) {
        if (fldMatterTable[index].mode2D == FLD_COLLISION_AXIS_Y) {
            if (!(fldMatterTable[index].pFace->attributes & 0x100)) {
                continue;
            }
        }
        if (fldMatterTable[index].pFace->attributes & 0xA01) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                 fldMatterTable[index].hojiID)) {
            continue;
        }
        if (!(fldMatterTable[index].pFace->attributes & 0x8000)) {
            continue;
        }
        if (fldMatterTable[index].pFace->special[0] != 4) {
            continue;
        }
        if (fldMatterTable[index].matter != 0) {
            relative = 1;
            transform = fldMatterTable[index].matter->poseset;
            origin[0] -= transform->pos[0];
            origin[1] -= transform->pos[1];
            origin[2] -= transform->pos[2];
            target[0] -= transform->pos[0];
            target[1] -= transform->pos[1];
            target[2] -= transform->pos[2];
        } else {
            relative = 0;
        }
        probeStart[0] = origin[0];
        probeStart[1] = origin[1];
        probeStart[2] = origin[2];
        probeEnd[0] = target[0];
        probeEnd[1] = target[1];
        probeEnd[2] = target[2];
        startDistance = fldVecDot(probeStart, fldMatterTable[index].normal) -
                        fldMatterTable[index].num;
        endDistance = fldVecDot(probeEnd, fldMatterTable[index].normal) -
                      fldMatterTable[index].num;
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
            clearance = HitPositionInside3(fldMatterTable[index].mode2D, fldMatterTable[index].numedges, 2.0f, probeStart, &fldMatterTable[index]);
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
            transform = fldMatterTable[index].matter->poseset;
            origin[0] += transform->pos[0];
            origin[1] += transform->pos[1];
            origin[2] += transform->pos[2];
            target[0] += transform->pos[0];
            target[1] += transform->pos[1];
            target[2] += transform->pos[2];
        }
    }
    hitPoint[0] = origin[0];
    hitPoint[1] = origin[1];
    hitPoint[2] = origin[2];
    return found;
}

/* Add the four vertical faces of an axis-aligned field collision box. */
void fldAddItemBoxObject(f32 x, f32 y, f32 z, f32 extent, f32 height,
                         FldMapSlotAuxiliaryFlagSelector auxiliaryFlagSelector) {
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
    fldItemBoxObjBuf[index].geometry.vertexCount = 8;
    fldItemBoxObjBuf[index].geometry.faceCount = 4;
    fldItemBoxObjBuf[index].geometry.triangleCount = 0;
    fldItemBoxObjBuf[index].geometry.vertices = fldItemBoxObjBuf[index].vertices;
    fldItemBoxObjBuf[index].geometry.faces = fldItemBoxObjBuf[index].faces;
    fldItemBoxObjBuf[index].geometry.stopData = NULL;
    fldItemBoxObjBuf[index].vertices[0][0] = minX;
    fldItemBoxObjBuf[index].vertices[0][1] = minY;
    fldItemBoxObjBuf[index].vertices[0][2] = maxZ;
    fldItemBoxObjBuf[index].vertices[0][3] = 1.0f;
    fldItemBoxObjBuf[index].vertices[1][0] = maxX;
    fldItemBoxObjBuf[index].vertices[1][1] = minY;
    fldItemBoxObjBuf[index].vertices[1][2] = maxZ;
    fldItemBoxObjBuf[index].vertices[1][3] = 1.0f;
    fldItemBoxObjBuf[index].vertices[2][0] = minX;
    fldItemBoxObjBuf[index].vertices[2][1] = minY;
    fldItemBoxObjBuf[index].vertices[2][2] = minZ;
    fldItemBoxObjBuf[index].vertices[2][3] = 1.0f;
    fldItemBoxObjBuf[index].vertices[3][0] = maxX;
    fldItemBoxObjBuf[index].vertices[3][1] = minY;
    fldItemBoxObjBuf[index].vertices[3][2] = minZ;
    fldItemBoxObjBuf[index].vertices[3][3] = 1.0f;
    fldItemBoxObjBuf[index].vertices[4][0] = minX;
    fldItemBoxObjBuf[index].vertices[4][1] = y;
    fldItemBoxObjBuf[index].vertices[4][2] = maxZ;
    fldItemBoxObjBuf[index].vertices[4][3] = 1.0f;
    fldItemBoxObjBuf[index].vertices[5][0] = maxX;
    fldItemBoxObjBuf[index].vertices[5][1] = y;
    fldItemBoxObjBuf[index].vertices[5][2] = maxZ;
    fldItemBoxObjBuf[index].vertices[5][3] = 1.0f;
    fldItemBoxObjBuf[index].vertices[6][0] = minX;
    fldItemBoxObjBuf[index].vertices[6][1] = y;
    fldItemBoxObjBuf[index].vertices[6][2] = minZ;
    fldItemBoxObjBuf[index].vertices[6][3] = 1.0f;
    fldItemBoxObjBuf[index].vertices[7][0] = maxX;
    fldItemBoxObjBuf[index].vertices[7][1] = y;
    fldItemBoxObjBuf[index].vertices[7][2] = minZ;
    fldItemBoxObjBuf[index].vertices[7][3] = 1.0f;
    fldItemBoxObjBuf[index].faces[0].attributes = 128;
    fldItemBoxObjBuf[index].faces[0].moveFloor = 0;
    fldItemBoxObjBuf[index].faces[0].sound = 0;
    fldItemBoxObjBuf[index].faces[0].stop = 0;
    fldItemBoxObjBuf[index].faces[0].place = 0;
    fldItemBoxObjBuf[index].faces[0].automap[0] = 0;
    fldItemBoxObjBuf[index].faces[0].automap[1] = 0;
    fldItemBoxObjBuf[index].faces[0].vertexIndices[0] = 0;
    fldItemBoxObjBuf[index].faces[0].vertexIndices[1] = 1;
    fldItemBoxObjBuf[index].faces[0].vertexIndices[2] = 5;
    fldItemBoxObjBuf[index].faces[0].vertexIndices[3] = 4;
    fldItemBoxObjBuf[index].faces[0].encounterType = 0;
    fldItemBoxObjBuf[index].faces[0].encounter = 0;
    fldItemBoxObjBuf[index].faces[0].special[0] = 0;
    fldItemBoxObjBuf[index].faces[0].special[1] = 0;
    fldItemBoxObjBuf[index].faces[1].attributes = 128;
    fldItemBoxObjBuf[index].faces[1].moveFloor = 0;
    fldItemBoxObjBuf[index].faces[1].sound = 0;
    fldItemBoxObjBuf[index].faces[1].stop = 0;
    fldItemBoxObjBuf[index].faces[1].place = 0;
    fldItemBoxObjBuf[index].faces[1].automap[0] = 0;
    fldItemBoxObjBuf[index].faces[1].automap[1] = 0;
    fldItemBoxObjBuf[index].faces[1].vertexIndices[0] = 1;
    fldItemBoxObjBuf[index].faces[1].vertexIndices[1] = 3;
    fldItemBoxObjBuf[index].faces[1].vertexIndices[2] = 7;
    fldItemBoxObjBuf[index].faces[1].vertexIndices[3] = 5;
    fldItemBoxObjBuf[index].faces[1].encounterType = 0;
    fldItemBoxObjBuf[index].faces[1].encounter = 0;
    fldItemBoxObjBuf[index].faces[1].special[0] = 0;
    fldItemBoxObjBuf[index].faces[1].special[1] = 0;
    fldItemBoxObjBuf[index].faces[2].attributes = 128;
    fldItemBoxObjBuf[index].faces[2].moveFloor = 0;
    fldItemBoxObjBuf[index].faces[2].sound = 0;
    fldItemBoxObjBuf[index].faces[2].stop = 0;
    fldItemBoxObjBuf[index].faces[2].place = 0;
    fldItemBoxObjBuf[index].faces[2].automap[0] = 0;
    fldItemBoxObjBuf[index].faces[2].automap[1] = 0;
    fldItemBoxObjBuf[index].faces[2].vertexIndices[0] = 3;
    fldItemBoxObjBuf[index].faces[2].vertexIndices[1] = 2;
    fldItemBoxObjBuf[index].faces[2].vertexIndices[2] = 6;
    fldItemBoxObjBuf[index].faces[2].vertexIndices[3] = 7;
    fldItemBoxObjBuf[index].faces[2].encounterType = 0;
    fldItemBoxObjBuf[index].faces[2].encounter = 0;
    fldItemBoxObjBuf[index].faces[2].special[0] = 0;
    fldItemBoxObjBuf[index].faces[2].special[1] = 0;
    fldItemBoxObjBuf[index].faces[3].attributes = 128;
    fldItemBoxObjBuf[index].faces[3].moveFloor = 0;
    fldItemBoxObjBuf[index].faces[3].sound = 0;
    fldItemBoxObjBuf[index].faces[3].stop = 0;
    fldItemBoxObjBuf[index].faces[3].place = 0;
    fldItemBoxObjBuf[index].faces[3].automap[0] = 0;
    fldItemBoxObjBuf[index].faces[3].automap[1] = 0;
    fldItemBoxObjBuf[index].faces[3].vertexIndices[0] = 2;
    fldItemBoxObjBuf[index].faces[3].vertexIndices[1] = 0;
    fldItemBoxObjBuf[index].faces[3].vertexIndices[2] = 4;
    fldItemBoxObjBuf[index].faces[3].vertexIndices[3] = 6;
    fldItemBoxObjBuf[index].faces[3].encounterType = 0;
    fldItemBoxObjBuf[index].faces[3].encounter = 0;
    fldItemBoxObjBuf[index].faces[3].special[0] = 0;
    fldItemBoxObjBuf[index].faces[3].special[1] = 0;
    fldMakePlane(&fldItemBoxObjBuf[index].geometry,
                  origin, dds3AllocObjectId(), 0, auxiliaryFlagSelector);
    D_0043617C++;
}

void evtMessageExtraActionNoOp(void) {
}
INCLUDE_SDATA(const s32, "game/code_00136EF8", fldCameraColorLayerScroll);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldItemBoxObjBuf);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_0043617C);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldMatterTable);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldValueRecordCount);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436188);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldAuxRecordResource);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldValueRecordResource);
