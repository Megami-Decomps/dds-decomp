#include "kwln.h"
#include "mnu_list.h"
#include "kwln_task_state.h"
#include "evt_world.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"

extern s32 scrTestEntryFlag(DatPartyRecord *context, u16 entryId, u32 bit);

extern s32 mnuMantraPanelPositionTable;

extern EvtLoadedRecord *mnuMantraNodePositionTable;

extern s32 dspWindowHandle;

extern s8 dspWindowControlState;

extern s32 D_00437888;

extern s8 dspCapturedSoundMode;

extern s8 evtMessageWindowOption;

extern s8 dspWindowStateGate;

extern u32 D_00437890[];

extern s32 func_0026DB48(DatPartyRecord *context, u8 entry);

extern s32 func_0026DB90(DatPartyRecord *context);

/* Return whether bit 15 is set for the supplied entry. */
s32 func_0026DB48(DatPartyRecord *context, u8 entry) {
    return scrTestEntryFlag(context, entry, 0xf);
}

void func_0026DB68(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DB48", func_0026DB70);

/* Return the flag test's result through the native selector-zero wrapper. */
s32 func_0026DB90(DatPartyRecord *context) {
    return scrTestEntryFlag(context, 0, 0);
}

void func_0026DBB0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DB48", func_0026DBB8);

/* Call the existing flag routine with entry zero and selector one; ignore its result. */
void func_0026DBD8(DatPartyRecord *context) {
    scrTestEntryFlag(context, 0, 1);
}

