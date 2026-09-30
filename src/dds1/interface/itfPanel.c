#include "common.h"
#include "eff.h"

extern void func_002D0918(void *arg0);

void func_00199900(EffPrim *arg0) {
    if (arg0 != NULL) {
        if (arg0->recordCount != 0) {
            func_002D0918(arg0->unk4);
        }
        func_002D0918(arg0->unk0);
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

void func_00199950(PanelDefinition *panel, s32 a10, s32 a14, s32 a18, s32 a1C, s32 a0C) {
    panel->a0C = a0C;
    panel->a10 = a10;
    panel->a14 = a14;
    panel->a18 = a18;
    panel->a1C = a1C;
    if (D_00357A00[panel->kind] != 0) {
        D_00357A00[panel->kind](panel->context);
    }
}

INCLUDE_ASM(const s32, "interface/itfPanel", func_00199998);

void func_00199A20(PanelDefinition2 *panel, s32 a2C, s32 a30, s32 a34, s32 a38) {
    panel->a2C = a2C;
    panel->a30 = a30;
    panel->a34 = a34;
    panel->a38 = a38;
    if (D_00357A28[panel->kind] != 0) {
        D_00357A28[panel->kind](panel->context);
    }
}

s32 func_00199A68(PanelDefinition *panel, s32 arg1) {
    s32 work = sdfAllocPacketAligned(0x20, arg1);

    sdfInitPacketList(work);
    func_0019ACE0(work);
    itfPanelDispatchHandler(panel, work);
    return ((s32 (*)(s32, s32))*(u32 *)(arg1 + 0x10))(arg1, work);
}
