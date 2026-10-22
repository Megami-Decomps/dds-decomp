#include "dsp_entry_api.h"
#include "dsp_active_api.h"
#include "common.h"

void dspStartActivePresetEntry(void) {
    dspSetActive(1);
    fclStartMessage(0xe);
}
