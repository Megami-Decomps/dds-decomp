#include "common.h"

void mnuReleaseOptionalResourceSlot(u32 *resourceSlot) {
    if (resourceSlot != NULL) {
        func_003297C8(*resourceSlot);
        return;
    }
}
