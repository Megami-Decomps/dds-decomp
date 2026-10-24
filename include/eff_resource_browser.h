#ifndef EFF_RESOURCE_BROWSER_H
#define EFF_RESOURCE_BROWSER_H

#include "sdf.h"

typedef enum EffResourceKind {
    EFF_RESOURCE_KIND_NONE = 0,
    EFF_RESOURCE_KIND_TMX = 1,
    EFF_RESOURCE_KIND_P2A = 2,
    EFF_RESOURCE_KIND_D3P = 4,
    EFF_RESOURCE_KIND_BED = 8,
    EFF_RESOURCE_KIND_PB = 0x10,
    EFF_RESOURCE_KIND_PCF = 0x20
} EffResourceKind;

/* Directory list roots and filename nodes have distinct allocation extents. */
typedef struct EffResourceListNode {
    EffResourceKind kind;
    char name[0x30];
    struct EffResourceListNode *previous;
    struct EffResourceListNode *next;
} EffResourceListNode;

typedef struct EffResourceList {
    s32 resourceCount;
    char *directoryPath;
    EffResourceListNode *head;
} EffResourceList;
typedef char EffResourceList_size_must_be_12[(sizeof(EffResourceList) == 12) ? 1 : -1];
typedef char EffResourceListNode_size_must_be_60[(sizeof(EffResourceListNode) == 60) ? 1 : -1];


/* Complete 0x44-byte browser owner returned by dds3EffectFileListWindowCreate. */
typedef struct EffResourceDescriptor {
    s32 x;
    s32 y;
    s32 result;
    s32 resourceCount;
    u32 word10;
    s32 selectedIndex;
    s32 visibleIndex;
    u32 previewActive; /* 0x1C: set when a drawable preview is active */
    u32 repeatDelay;
    u32 drawSurface;
    u32 borderColor;
    u32 fillColor;
    EffResourceListNode *firstVisibleEntry;
    EffResourceListNode *selectedEntry;
    EffResourceListNode *cachedEntry;
    sdfTexHandle_t *textureHandle;
    EffResourceList *resourceList;
} EffResourceDescriptor;
typedef char EffResourceDescriptor_size_must_be_0x44[
    sizeof(EffResourceDescriptor) == 0x44 ? 1 : -1];


EffResourceDescriptor *dds3EffectFileListWindowCreate(EffResourceList *list);
void dds3EffectFileListDelete(EffResourceList *list);
void dds3EffectFileListWindowDelete(EffResourceDescriptor *work);
void dds3EffectFileListWindowSetPos(EffResourceDescriptor *work, s32 x, s32 y);
u32 effGetWorkParam(EffResourceDescriptor *work);
u32 dds3EffectFileListWindowGetState(EffResourceDescriptor *work);
u32 dds3EffectFileListWindowGetFilename(EffResourceDescriptor *work, char *destination);
void dds3EffectFileListWindowSetOt(EffResourceDescriptor *work, u32 delay);
void dds3EffectFileListWindowSetPadWait(EffResourceDescriptor *work, u32 surface);
void dds3EffectFileListWindowSetWindowColor(EffResourceDescriptor *work, u32 borderColor, u32 fillColor);
void dds3EffectListSetTexture(EffResourceDescriptor *work, const char *textureResource);
EffResourceList *dds3EffectFileListCreate(const char *path, s32 flags);
s32 dds3EffectFileListWindowDraw(EffResourceDescriptor *work);
void dds3EffectFileListWindowDrawSprite(EffResourceDescriptor *overlay);

#endif
