#include "common.h"
#include "eff.h"

extern void func_003297C8(void *arg0);

void func_001A1930(EffPrim *arg0) {
    if (arg0 != NULL) {
        if (arg0->recordCount != 0) {
            func_003297C8(arg0->unk4);
        }
        func_003297C8(arg0->unk0);
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

extern void (*D_003B43F8[])(void *);

void func_001A1980(PanelDefinition *panel, s32 a10, s32 a14, s32 a18, s32 a1C, s32 a0C) {
    panel->a0C = a0C;
    panel->a10 = a10;
    panel->a14 = a14;
    panel->a18 = a18;
    panel->a1C = a1C;
    if (D_003B43F8[panel->kind] != 0) {
        D_003B43F8[panel->kind](panel->context);
    }
}


INCLUDE_ASM(const s32, "interface/itfPanel", func_001A19C8);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A1A50);


extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(s32);
extern void func_001A2D10(s32);

s32 func_001A1A98(PanelDefinition *panel, s32 arg1) {
    s32 work = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(work);
    func_001A2D10(work);
    itfPanelDispatchHandler(panel, work);
    return ((s32 (*)(s32, s32))*(u32 *)(arg1 + 0x10))(arg1, work);
}
