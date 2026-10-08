#ifndef DDS3_OWNED_NODE_H
#define DDS3_OWNED_NODE_H

#include "common.h"

struct FrFontGlyph;

typedef struct Dds3IntrusiveNode Dds3IntrusiveNode;
typedef struct Dds3IntrusiveNodeCallbacks Dds3IntrusiveNodeCallbacks;

/* Shared prefix used by the font node and by L2D rectangles. */
struct Dds3IntrusiveNode {
    Dds3IntrusiveNode *prev;
    Dds3IntrusiveNode *next;
    s32 value;
    Dds3IntrusiveNodeCallbacks *vtable;
};

/* The list object contains the head and tail words at its native address. */
typedef struct Dds3IntrusiveNodeList {
    Dds3IntrusiveNode *head;
    Dds3IntrusiveNode *tail;
} Dds3IntrusiveNodeList;

/* Callback tables are shared by owners with different payloads. */
struct Dds3IntrusiveNodeCallbacks {
    void (*destroy)(void *owner);
    void (*update)(void *owner);
};

/* The font-owned node's payload is specifically a retained glyph pointer. */
typedef struct Dds3FontNode {
    Dds3IntrusiveNode owner;
    struct FrFontGlyph *glyph;
} Dds3FontNode;

/* The L2D callback owner extends the intrusive prefix with a rectangle. */
typedef struct Dds3L2dRectangle {
    Dds3IntrusiveNode owner;
    s32 left;
    s32 top;
    s32 width;
    s32 height;
    s32 depth;
    s32 color;
} Dds3L2dRectangle;

extern Dds3IntrusiveNodeList dds3OwnedNodeListHead;
extern Dds3IntrusiveNodeCallbacks dds3FontNodeVTable;

void dds3AppendIntrusiveNode(Dds3IntrusiveNodeList *list,
    Dds3IntrusiveNode *node, s32 linkOffset);
void dds3UnlinkNodeFromList(Dds3IntrusiveNodeList *list,
    Dds3IntrusiveNode *node, s32 linkOffset);
void dds3RegisterOwnedIntrusiveNode(Dds3IntrusiveNode *node,
    Dds3IntrusiveNodeCallbacks *vtable);
void dds3DestroyLinkedNode(Dds3IntrusiveNode *node);
void dds3DestroyAllOwnedIntrusiveNodes(void);
void dds3SetLinkedNodeValue(Dds3IntrusiveNode *node, u32 value);
void dds3DestroyNodesWithValue(s32 value);
void dds3UpdateLinkedNodes(void);

Dds3FontNode *dds3CreateFontNode(s32 x, s32 y, void *text);
void itfConfigureOwnedGlyphChainFlag(Dds3FontNode *node, u8 value);
void frFontSubmitAndFreeGlyphOwner(void *owner);
void frFontDrawOwnedGlyph(void *owner);
void frFontReleaseOwnerStorage(void *owner);

typedef char Dds3IntrusiveNodeSizeCheck[sizeof(Dds3IntrusiveNode) == 0x10 ? 1 : -1];
typedef char Dds3IntrusiveNodeListSizeCheck[sizeof(Dds3IntrusiveNodeList) == 0x08 ? 1 : -1];
typedef char Dds3IntrusiveNodeCallbacksSizeCheck[sizeof(Dds3IntrusiveNodeCallbacks) == 0x08 ? 1 : -1];
typedef char Dds3FontNodeGlyphOffsetCheck[((u32)&((Dds3FontNode *)0)->glyph == 0x10) ? 1 : -1];
typedef char Dds3FontNodeSizeCheck[sizeof(Dds3FontNode) == 0x14 ? 1 : -1];
typedef char Dds3L2dRectangleLayoutCheck[
    ((u32)&((Dds3L2dRectangle *)0)->left == 0x10 &&
     (u32)&((Dds3L2dRectangle *)0)->top == 0x14 &&
     (u32)&((Dds3L2dRectangle *)0)->width == 0x18 &&
     (u32)&((Dds3L2dRectangle *)0)->height == 0x1C &&
     (u32)&((Dds3L2dRectangle *)0)->depth == 0x20 &&
     (u32)&((Dds3L2dRectangle *)0)->color == 0x24 &&
     sizeof(Dds3L2dRectangle) == 0x28) ? 1 : -1];

#endif /* DDS3_OWNED_NODE_H */
