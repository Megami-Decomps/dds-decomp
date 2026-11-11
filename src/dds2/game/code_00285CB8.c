#include "common.h"
#include "sdf_heap_api.h"

void mnuReleaseOptionalResourceSlot(u32 *resourceSlot) {
    if (resourceSlot != NULL) {
        sdfMemFreeDirect((struct sdfMemHandle_t *)(u32)(*resourceSlot));
        return;
    }
}
