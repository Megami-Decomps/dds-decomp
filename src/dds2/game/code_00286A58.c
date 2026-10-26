#include "common.h"
#include "dat_state.h"
#include "mnu_mantra.h"
#include "mnu_mantra_position_api.h"
#include "prf_profile_cap.h"
#include "mdl_flag_api.h"

extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void mtrRecomputeMantraFlagsForPartyUnit(datUnitWork_t *);
extern void mtrMantraBitResetUnit(datUnitWork_t *);
extern MantraFlagResource *evtAllocateMantraSelectionWork(datUnitWork_t *, s32);
extern void evtReleaseMantraSelectionWork(MantraFlagResource *);
extern DatProfileRecord *ptyGetProfileRecordPointer(const datUnitWork_t *, u16);
extern void ptyApplyProfile(datUnitWork_t *, u16);
extern void scrSetEntryLowFlags(datUnitWork_t *, u16, u16);

const char D_00425F30[] = "*****************[mtrMantraSetBit():[0x%x]]*****************\n";

void mtrMantraSetBit(datUnitWork_t *record) {
    MantraFlagResource *work;
    MantraNodePos *node;
    s32 i;

    evtPrintDeveloperConsoleMessage(D_00425F30, record->unitId);
    if (evtBitCheck(0xBA0) != 0 && evtBitCheck(0x80E) == 0) {
        mtrRecomputeMantraFlagsForPartyUnit(record);
    } else {
        mtrMantraBitResetUnit(record);
        work = evtAllocateMantraSelectionWork(record, 1);
        node = mnuGetMantraNodePositionRecord(0);
        for (i = 0; i < 176; i++, node++) {
            scrSetEntryLowFlags(record, i, work->flags[i]);
            if ((node->selector.packed & 0x10F) == 0x102 && ((work->flags[i] >> 8) & 1)) {
                ptyGetProfileRecordPointer(record, node->id)->value = ptyGetProfileRecordCap(node->id);
                ptyApplyProfile(record, node->id);
            }
        }
        evtReleaseMantraSelectionWork(work);
        work = NULL;
    }
}
