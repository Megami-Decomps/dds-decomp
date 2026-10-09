#ifndef EFF_RESOURCE_BROWSER_H
#define EFF_RESOURCE_BROWSER_H

#include "sdf.h"

/* Directory list roots and filename nodes have distinct allocation extents. */
typedef struct EffResourceListNode {
    s32 type;
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


/* Complete 0x44-byte browser owner returned by effCreateResourceListDescriptor. */
typedef struct EffResourceDescriptor {
    s32 x;
    s32 y;
    s32 result;
    s32 resourceCount;
    u32 word10;
    s32 selectedIndex;
    s32 visibleIndex;
    s32 previewActive;
    u32 repeatDelay;
    u32 drawSurface;
    u32 borderColor;
    u32 fillColor;
    EffResourceListNode *firstVisibleEntry;
    EffResourceListNode *selectedEntry;
    EffResourceListNode *cachedEntry;
    SdfTex *textureHandle;
    EffResourceList *resourceList;
} EffResourceDescriptor;
typedef char EffResourceDescriptor_size_must_be_0x44[
    sizeof(EffResourceDescriptor) == 0x44 ? 1 : -1];


EffResourceDescriptor *effCreateResourceListDescriptor(EffResourceList *list);
void effFreeWorkList(EffResourceList *list);
void effFreeWork(EffResourceDescriptor *work);
void effSetMsgHeader(EffResourceDescriptor *work, s32 x, s32 y);
u32 effGetWorkParam(EffResourceDescriptor *work);
u32 effGetWorkLink(EffResourceDescriptor *work);
u32 effFormatMsgNames(EffResourceDescriptor *work, void *destination);
void effSetWorkFirst(EffResourceDescriptor *work, u32 delay);
void effSetWorkSecond(EffResourceDescriptor *work, u32 surface);
void effSetMsgPair(EffResourceDescriptor *work, u32 borderColor, u32 fillColor);
void effSetWorkTextureResource(EffResourceDescriptor *work, const char *textureResource);
#ifdef VERSION_DDS2
EffResourceList *func_00194BD0(char *path, s32 flags);
s32 func_001950F0(EffResourceDescriptor *work);
void effDrawResourceTexturePreview(EffResourceDescriptor *overlay);
#else
EffResourceList *func_0018CF98(char *path, s32 flags);
s32 func_0018D4B8(EffResourceDescriptor *work);
void func_0018DA70(EffResourceDescriptor *work);
#endif

#endif
