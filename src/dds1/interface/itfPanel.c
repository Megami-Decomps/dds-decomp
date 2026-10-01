#include "common.h"
#include "eff.h"

extern void func_002D0918(void *resource);

/* Release the resources held by the primitive's two resource slots. */
void itfPanelReleasePrimitiveResources(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->recordCount != 0) {
            func_002D0918(primitive->unk4);
        }
        func_002D0918(primitive->unk0);
    }
}

/* Panel definition: a handler table is indexed by `kind` (0x3C) and the entry
   is called with the panel's own context (0x08). */
typedef struct PanelDefinition {
    u8 pad00[8];
    void *context; /* 0x08: passed as $4 to the table entry */
    s32 a0C;
    s32 a10;
    s32 a14;
    s32 a18;
    s32 a1C;
    u8 pad20[0x1C];
    u8 kind; /* 0x3C: index into the handler table */
} PanelDefinition;

typedef struct PanelDefinition2 {
    u8 pad00[8];
    void *context; /* 0x08 */
    u8 pad0C[0x20];
    s32 a2C;
    s32 a30;
    s32 a34;
    s32 a38;
    u8 kind; /* 0x3C */
} PanelDefinition2;

extern void (*D_00357A00[])(void *);
extern void (*D_00357A28[])(void *);

void itfSetPanelLayoutAndNotify(PanelDefinition *panel, s32 a10, s32 a14, s32 a18, s32 a1C, s32 a0C) {
    panel->a0C = a0C;
    panel->a10 = a10;
    panel->a14 = a14;
    panel->a18 = a18;
    panel->a1C = a1C;
    if (D_00357A00[panel->kind] != 0) {
        D_00357A00[panel->kind](panel->context);
    }
}

void itfAdvancePanelLayoutAndNotify(PanelDefinition *panel, s32 a10, s32 a14, s32 a18, s32 a1C, s32 a0C) {
    void (*handler)(void *, s32, s32, s32, s32) =
        (void (*)(void *, s32, s32, s32, s32))D_00357A00[panel->kind];

    panel->a10 += a10;
    panel->a14 += a14;
    panel->a18 += a18;
    panel->a1C += a1C;
    panel->a0C += a0C;
    if (handler != 0) {
        handler(panel->context, panel->a10, panel->a14, panel->a18, panel->a1C);
    }
}

void func_00199A20(PanelDefinition2 *panel, s32 a2C, s32 a30, s32 a34, s32 a38) {
    panel->a2C = a2C;
    panel->a30 = a30;
    panel->a34 = a34;
    panel->a38 = a38;
    if (D_00357A28[panel->kind] != 0) {
        D_00357A28[panel->kind](panel->context);
    }
}

/* The packet owner's callback occupies its 0x10 dispatch slot. */
typedef struct PanelPacketDispatch {
    u8 pad00[0x10];
    s32 (*handler)(s32 owner, s32 packet);
} PanelPacketDispatch;

s32 itfBuildAndSubmitPanelPacket(PanelDefinition *panel, s32 owner) {
    s32 work = sdfAllocPacketAligned(0x20, owner);

    sdfInitPacketList(work);
    itfAppendGsPanelStatePacket(work);
    itfPanelDispatchHandler(panel, work);
    return ((PanelPacketDispatch *)owner)->handler(owner, work);
}
