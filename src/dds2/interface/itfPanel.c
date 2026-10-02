#include "common.h"
#include "eff.h"

extern void sdfReleaseResourceAllocation(void *resource);

/* Release the resources held by the primitive's two resource slots. */
void itfPanelReleasePrimitiveResources(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->recordCount != 0) {
            sdfReleaseResourceAllocation(primitive->secondaryResource);
        }
        sdfReleaseResourceAllocation(primitive->primaryResource);
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
    u8 pad20[0xC];
    /* Secondary four-value parameter set, stored before the kind-specific
       D_003B4420 notification; the values' individual roles are unknown. */
    s32 a2C;
    s32 a30;
    s32 a34;
    s32 a38;
    u8 kind; /* 0x3C: index into the handler table */
} PanelDefinition;

extern void (*D_003B43F8[])(void *);

/* `left`/`top`/`right`/`bottom` are the panel rectangle — the call site in
   itfMesManager passes rect[0..3] verbatim; `depth` is the message render value. */
void itfSetPanelLayoutAndNotify(PanelDefinition *panel, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    panel->a0C = depth;
    panel->a10 = left;
    panel->a14 = top;
    panel->a18 = right;
    panel->a1C = bottom;
    if (D_003B43F8[panel->kind] != 0) {
        D_003B43F8[panel->kind](panel->context);
    }
}


void itfAdvancePanelLayoutAndNotify(PanelDefinition *panel, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    void (*handler)(void *, s32, s32, s32, s32) =
        (void (*)(void *, s32, s32, s32, s32))D_003B43F8[panel->kind];

    panel->a10 += left;
    panel->a14 += top;
    panel->a18 += right;
    panel->a1C += bottom;
    panel->a0C += depth;
    if (handler != 0) {
        handler(panel->context, panel->a10, panel->a14, panel->a18, panel->a1C);
    }
}

extern void (*D_003B4420[])(void *);

void itfPanelUpdateValuesAndNotify(PanelDefinition *panel, s32 a2C, s32 a30, s32 a34, s32 a38) {
    panel->a2C = a2C;
    panel->a30 = a30;
    panel->a34 = a34;
    panel->a38 = a38;
    if (D_003B4420[panel->kind] != 0) {
        D_003B4420[panel->kind](panel->context);
    }
}


extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(s32);
extern void itfAppendGsPanelStatePacket(s32);

s32 itfBuildAndSubmitPanelPacket(PanelDefinition *panel, s32 owner) {
    s32 work = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(work);
    itfAppendGsPanelStatePacket(work);
    itfPanelDispatchHandler(panel, work);
    return ((s32 (*)(s32, s32))*(u32 *)(owner + 0x10))(owner, work);
}
