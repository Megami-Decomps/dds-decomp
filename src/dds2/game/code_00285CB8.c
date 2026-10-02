#include "common.h"

void mnuReleaseOptionalResourceSlot(u32 *resourceSlot) {
    if (resourceSlot != NULL) {
        sdfReleaseResourceAllocation(*resourceSlot);
        return;
    }
}
