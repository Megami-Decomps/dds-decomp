#include "common.h"

/* Circular doubly-linked list node; prev/next at +0x18/+0x1C. */
typedef struct FntNode {
    void *unk0;            /* 0x0: non-NULL while the node is linked */
    u8 unk4[0x14];         /* 0x4 */
    struct FntNode *prev;  /* 0x18 */
    struct FntNode *next;  /* 0x1C */
} FntNode;

/* File group header: two handles to release, plus live-node count and sentinel link. */
typedef struct {
    void *unk0;
    void *unk4;
    u8 pad8[0x10];
    s32 unk18;      /* 0x18: live-node count */
    FntNode *unk1C; /* 0x1C: sentinel link */
} FmGslWork;

extern FmGslWork frFontResourceList;
extern void func_003297C8(void *);

/* Release the group's handles once and clear its active-node flag. */
s32 fmGslReleaseActiveResourceBuffers(void) {
    if (frFontResourceList.unk1C == 0) {
        return 0;
    }
    func_003297C8(frFontResourceList.unk0);
    func_003297C8(frFontResourceList.unk4);
    frFontResourceList.unk1C = 0;
    return 1;
}

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
FntNode *func_0019B7F0(void) {
    FntNode *node = frFontResourceList.unk1C->next;

    if (node->unk0 == NULL) {
        return NULL;
    }
    node->prev->next = node->next;
    frFontResourceList.unk18 -= 1;
    node->next->prev = node->prev;
    return node;
}
