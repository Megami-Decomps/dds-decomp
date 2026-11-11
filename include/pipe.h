#ifndef PIPE_H
#define PIPE_H

#include "common.h"
#include "libsdf.h"
#include "model.h"

struct pipeTextureFile_t;

typedef struct tagPipeTexture {
    sdfTexHandle_t *th;
    sdfMemHandle_t *mh;
    struct pipeTextureFile_t *mp;
    u32 unk0C;
} tagPipeTexture;

typedef struct tagpipeBasic {
    void *base;
    mdlObj_t *mdl;
    u8 unk08[4];
    s32 enabled;
    f32 pos[4];
    f32 rot[4];
    f32 scl[4];
} tagpipeBasic;

typedef struct tagPipeObject {
    s32 offset;
    f32 w;
    f32 pos[2];
    f32 vpos[2];
    u8 id;
    u8 action;
    u8 enabled;
    u8 mdl;
    s16 count;
    u16 flags;
} tagPipeObject;

typedef struct tagPipePosData {
    f32 position[3];
    u32 opaque0C;
    f32 direction[3];
    u32 opaque1C;
} tagPipePosData;

typedef struct tagPipePosTable {
    sdfMemHandle_t *aHandle;
    s32 tSize;
    tagPipePosData *points;
    u32 opaque0C;
} tagPipePosTable;

typedef struct tagPipeTable {
    s32 tabMax;
    sdfMemHandle_t *aHandle;
    void *tabPtr;
    void *data;
} tagPipeTable;

typedef struct tagPipeLinkData {
    u16 id;
    s16 partsID;
    struct tagPipeLinkData *next;
    struct tagPipeLinkData *prev;
    u8 unk0C[4];
} tagPipeLinkData;


typedef struct tagPipeLinkInfo {
    sdfMemHandle_t *aHandle;
    tagPipeLinkData *head;
    s32 maxNum;
    s32 nextId;
} tagPipeLinkInfo;

typedef struct tagPipePartsType {
    f32 radius;
    f32 length;
    u32 frame;
} tagPipePartsType;


typedef struct tagPipePartsInfo {
    u16 partsID;
    u8 inWay;
    u8 goWay;
    tagPipePartsType *data;
} tagPipePartsInfo;

typedef struct {
    s32 count;
    void *data;
} pipeLinkSource_t;

#endif
