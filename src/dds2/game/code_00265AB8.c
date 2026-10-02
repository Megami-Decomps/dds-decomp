#include "common.h"

void dspStartActivePresetEntry(void) {
    dspSetActive(1);
    dspStartEntry(0xe);
}
