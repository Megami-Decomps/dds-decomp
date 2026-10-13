#include "kwln.h"
#include "mnu_list.h"
#include "kwln_task_state.h"
#include "evt_world.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"

extern s32 scrTestEntryFlag(DatPartyRecord *context, u16 entryId, u32 bit);
extern void scrClearEntryFlag(DatPartyRecord *context, u16 entryId, u32 bit);

extern s32 mnuMantraPanelPositionTable;

extern EvtLoadedRecord *mnuMantraNodePositionTable;

extern s32 dspWindowHandle;

extern s8 dspWindowControlState;

extern s32 D_00437888;

extern s8 dspCapturedSoundMode;

extern s8 evtMessageWindowOption;

extern s8 dspWindowStateGate;

extern u32 D_00437890[];

extern s32 scrIsPartyProfileEntryBit15Set(DatPartyRecord *context, u8 entry);

extern s32 scrIsFirstPartyProfileEntryBitZeroSet(DatPartyRecord *context);

/* Return whether bit 15 is set for the supplied entry. */
s32 scrIsPartyProfileEntryBit15Set(DatPartyRecord *context, u8 entry) {
    return scrTestEntryFlag(context, entry, 0xf);
}

void func_0026DB68(void) {
}

/* The consumed context is cleared under the post-call cleanup exception.
 * Retail uses JAL plus epilogue. Qualified cc1 RTL retains this reset through
 * 01.sibling and 02.jump, then deletes it in 03.cse (both entry-zero clears). */
void func_0026DB70(DatPartyRecord *context) {
    scrClearEntryFlag(context, 0, 0);
    context = NULL;
}

/* Return the flag test's result through the native selector-zero wrapper. */
s32 scrIsFirstPartyProfileEntryBitZeroSet(DatPartyRecord *context) {
    return scrTestEntryFlag(context, 0, 0);
}

void func_0026DBB0(void) {
}

/* The consumed context is cleared under the post-call cleanup exception.
 * Retail uses JAL plus epilogue. Qualified cc1 RTL retains this reset through
 * 01.sibling and 02.jump, then deletes it in 03.cse (both entry-zero clears). */
void func_0026DBB8(DatPartyRecord *context) {
    scrClearEntryFlag(context, 0, 1);
    context = NULL;
}

/* Call the existing flag routine with entry zero and selector one; ignore its result. */
void func_0026DBD8(DatPartyRecord *context) {
    scrTestEntryFlag(context, 0, 1);
}

