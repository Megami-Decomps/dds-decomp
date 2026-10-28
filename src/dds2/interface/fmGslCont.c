#include "common.h"
#include "fr_font.h"
#include "font_mem_control_api.h"
#include "sdf_resource.h"

/* Release the group's handles once and clear its active-node flag. */
FntMemControlDestroyResult fmDestroyFontMemControl(void) {
    if (gFGC.head == 0) {
        return FNT_MEM_CONTROL_DESTROY_ALREADY_INACTIVE;
    }
    sdfMemFreeDirect(gFGC.nodeAllocation);
    sdfMemFreeDirect(gFGC.recordStorageAllocation);
    gFGC.head = 0;
    return FNT_MEM_CONTROL_DESTROY_RELEASED;
}

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
FntNode *fmRentFontMem(void) {
    FntNode *resourceNode = gFGC.head->next;

    if (resourceNode->nodeIndex == 0) {
        return NULL;
    }
    resourceNode->prev->next = resourceNode->next;
    gFGC.count -= 1;
    resourceNode->next->prev = resourceNode->prev;
    return resourceNode;
}
