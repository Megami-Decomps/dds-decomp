#ifndef DDS_FLD_MAP_RESOURCE_H
#define DDS_FLD_MAP_RESOURCE_H

#include "common.h"

struct SdfTex;
struct SdfMemBlock;

typedef struct MapResource {
    struct SdfTex *texture;            /* 0x00: retained texture returned by sdfTexAcquireResourceTexture */
    struct SdfMemBlock *allocation;    /* 0x04: named-resource allocation queued for release */
    u32 resourceAddress;               /* 0x08: address word written by sdfReadNamedResource */
    u32 unknown0C;                     /* 0x0C: untouched by the paired loader/release providers */
} MapResource;

typedef char MapResource_size_must_be_0x10[(sizeof(MapResource) == 0x10) ? 1 : -1];
typedef char MapResource_texture_offset_must_be_0x00[
    ((u32)&((MapResource *)0)->texture == 0x00) ? 1 : -1];
typedef char MapResource_allocation_offset_must_be_0x04[
    ((u32)&((MapResource *)0)->allocation == 0x04) ? 1 : -1];
typedef char MapResource_resourceAddress_offset_must_be_0x08[
    ((u32)&((MapResource *)0)->resourceAddress == 0x08) ? 1 : -1];
typedef char MapResource_unknown0C_offset_must_be_0x0C[
    ((u32)&((MapResource *)0)->unknown0C == 0x0C) ? 1 : -1];

extern MapResource fldLocalMapNameTextures[10];
extern MapResource fldLocalMapAuxTextureResource;
extern MapResource fldLocalMapTextureResource;

s32 fldLoadMapResource(const char *name, MapResource *record);
u32 fldReleaseMapResource(struct SdfTex **texture);

#endif /* DDS_FLD_MAP_RESOURCE_H */
