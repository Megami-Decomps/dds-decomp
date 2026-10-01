#include "common.h"
#include "eff.h"

extern void func_003297C8(void *resource);

/* Release the resources held by the primitive's two resource slots. */
void itfPanelReleasePrimitiveResources(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->recordCount != 0) {
            func_003297C8(primitive->unk4);
        }
        func_003297C8(primitive->unk0);
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

void itfSetPanelLayoutAndNotify(PanelDefinition *panel, s32 a10, s32 a14, s32 a18, s32 a1C, s32 a0C) {
    panel->a0C = a0C;
    panel->a10 = a10;
    panel->a14 = a14;
    panel->a18 = a18;
    panel->a1C = a1C;
    if (D_003B43F8[panel->kind] != 0) {
        D_003B43F8[panel->kind](panel->context);
    }
}


void itfAdvancePanelLayoutAndNotify(PanelDefinition *panel, s32 a10, s32 a14, s32 a18, s32 a1C, s32 a0C) {
    void (*handler)(void *, s32, s32, s32, s32) =
        (void (*)(void *, s32, s32, s32, s32))D_003B43F8[panel->kind];

    panel->a10 += a10;
    panel->a14 += a14;
    panel->a18 += a18;
    panel->a1C += a1C;
    panel->a0C += a0C;
    if (handler != 0) {
        handler(panel->context, panel->a10, panel->a14, panel->a18, panel->a1C);
    }
}

extern void (*D_003B4420[])(void *);

void func_001A1A50(PanelDefinition *panel, s32 a2C, s32 a30, s32 a34, s32 a38) {
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
