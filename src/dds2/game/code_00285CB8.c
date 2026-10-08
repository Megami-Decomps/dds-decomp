#include "common.h"
#include "sdf_resource.h"

void mnuReleaseOptionalResourceSlot(u32 *resourceSlot) {
    if (resourceSlot != NULL) {
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(*resourceSlot));
        return;
    }
}
