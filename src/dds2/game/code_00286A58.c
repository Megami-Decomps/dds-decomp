#include "common.h"
#include "dat_state.h"
#include "mnu_mantra.h"

extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern s32 mdlFlagTest(s32);
extern void func_00286BA8(DatPartyRecord *);
extern void mtrMantraBitResetUnit(DatPartyRecord *);
extern MantraFlagResource *evtAllocateMantraSelectionWork(DatPartyRecord *, s32);
extern void evtReleaseMantraSelectionWork(MantraFlagResource *);
extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);
extern s32 *ptyGetProfileRecordPointer(DatPartyRecord *, u16);
extern s32 ptyGetProfileRecordCap(u16);
extern void func_00314868(DatPartyRecord *, u16);
extern void scrSetEntryLowFlags(DatPartyRecord *, u16, u16);

const char D_00425F30[] = "*****************[mtrMantraSetBit():[0x%x]]*****************\n";

void func_00286A58(DatPartyRecord *record) {
    MantraFlagResource *work;
    MantraNodePos *node;
    s32 i;

    evtPrintDeveloperConsoleMessage(D_00425F30, record->unitId);
    if (mdlFlagTest(0xBA0) != 0 && mdlFlagTest(0x80E) == 0) {
        func_00286BA8(record);
    } else {
        mtrMantraBitResetUnit(record);
        work = evtAllocateMantraSelectionWork(record, 1);
        node = mnuGetMantraNodePositionRecord(0);
        for (i = 0; i < 176; i++, node++) {
            scrSetEntryLowFlags(record, i, work->flags[i]);
            if ((node->selector.packed & 0x10F) == 0x102 && ((work->flags[i] >> 8) & 1)) {
                *ptyGetProfileRecordPointer(record, node->id) = ptyGetProfileRecordCap(node->id);
                func_00314868(record, node->id);
            }
        }
        evtReleaseMantraSelectionWork(work);
        work = NULL;
    }
}
