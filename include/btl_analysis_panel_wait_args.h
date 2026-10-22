#ifndef BTL_ANALYSIS_PANEL_WAIT_ARGS_H
#define BTL_ANALYSIS_PANEL_WAIT_ARGS_H

#include "common.h"

/* Eight-byte callback arguments allocated by btlAllocTask(8). The first word
 * is forwarded as the analysis-panel entry to btlCreateAnalysisPanelTask. The
 * second word counts callback updates before panel creation. */
typedef struct BtlAnalysisPanelWaitArgs {
    s32 panelEntry;
    u32 elapsedTicks;
} BtlAnalysisPanelWaitArgs;

typedef char BtlAnalysisPanelWaitArgs_size_must_be_0x08[
    (sizeof(BtlAnalysisPanelWaitArgs) == 0x08) ? 1 : -1];
typedef char BtlAnalysisPanelWaitArgs_elapsedTicks_offset_must_be_0x04[
    ((u32)&((BtlAnalysisPanelWaitArgs *)0)->elapsedTicks == 0x04) ? 1 : -1];

#endif
