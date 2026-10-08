#ifndef MNU_SCENE_LIST_H
#define MNU_SCENE_LIST_H

#include "common.h"

/* One 0x14-byte node allocation backs both scene transition and timed-draw
 * lists. Payload words vary by list use; the link is always at +0x10. */
typedef struct MnuSceneListNode {
    s32 frame; /* 0x00: elapsed frame for timed entries */
    union {
        struct {
            s32 fromValue; /* 0x04 */
            s32 toValue;   /* 0x08 */
            s16 mode;      /* 0x0C */
            u8 pad0E[2];
        } transition;
        struct {
            u32 value04;       /* 0x04: not consumed by the timed-draw path */
            s32 recordAddress; /* 0x08 */
            s16 kind;          /* 0x0C */
            u8 pad0E[2];
        } timedDraw;
    } payload;
    struct MnuSceneListNode *next; /* 0x10 */
} MnuSceneListNode;

typedef struct MnuSceneListHead {
    s32 frameCount;                  /* 0x00 */
    u32 value04;                     /* 0x04 */
    MnuSceneListNode *first;         /* 0x08 */
} MnuSceneListHead;

typedef char MnuSceneListNodeLayoutAssert[
    (sizeof(MnuSceneListNode) == 0x14 &&
     (u32)&((MnuSceneListNode *)0)->payload.transition.fromValue == 4 &&
     (u32)&((MnuSceneListNode *)0)->payload.transition.toValue == 8 &&
     (u32)&((MnuSceneListNode *)0)->payload.transition.mode == 0xC &&
     (u32)&((MnuSceneListNode *)0)->payload.timedDraw.recordAddress == 8 &&
     (u32)&((MnuSceneListNode *)0)->payload.timedDraw.kind == 0xC &&
     (u32)&((MnuSceneListNode *)0)->next == 0x10)
        ? 1 : -1];
typedef char MnuSceneListHeadLayoutAssert[
    (sizeof(MnuSceneListHead) == 0x0C &&
     (u32)&((MnuSceneListHead *)0)->first == 8)
        ? 1 : -1];

MnuSceneListNode *mnuAllocateMenuListNode(void);
MnuSceneListNode *mnuAllocateDisplayListNode(void);
MnuSceneListNode *mnuAppendDisplayListNode(MnuSceneListHead *head);
MnuSceneListNode *mnuAppendNodeToDisplayList(MnuSceneListHead *head);
MnuSceneListNode *mnuFreeMenuListNodeAndGetNext(MnuSceneListNode *node);
MnuSceneListNode *mnuReleaseDisplayListNodeAndGetNext(MnuSceneListNode *node);
void mnuReleaseDisplayListNodes(MnuSceneListHead *head);
void mnuReleaseListNodes(MnuSceneListHead *head);
s32 mnuAdvanceDisplayList(MnuSceneListHead *head, s32 amount, s32 drawContext);
void mnuDrawMantraCostAfterListAdvance(s32 recordAddress,
                                       MnuSceneListHead *head, s32 amount,
                                       s32 drawContext);

#endif
