#include "common.h"
#include "fr_font.h"
#include "sdf_resource.h"

/* Release the group's handles once and clear its active-node flag. */
s32 fmGslReleaseActiveResourceBuffers(void) {
    if (frFontResourceList.head == 0) {
        return 0;
    }
    sdfMemFreeDirect(frFontResourceList.nodeAllocation);
    sdfMemFreeDirect(frFontResourceList.recordStorageAllocation);
    frFontResourceList.head = 0;
    return 1;
}

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
FntNode *frFontDetachFirstResourceNode(void) {
    FntNode *resourceNode = frFontResourceList.head->next;

    if (resourceNode->nodeIndex == 0) {
        return NULL;
    }
    resourceNode->prev->next = resourceNode->next;
    frFontResourceList.count -= 1;
    resourceNode->next->prev = resourceNode->prev;
    return resourceNode;
}
