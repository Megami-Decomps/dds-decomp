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

#endif /* DDS3_OWNED_NODE_H */
