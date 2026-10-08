#include "common.h"
#include "fr_font.h"
#include "sdf_resource.h"

/* Release the group's handles once and clear its active-node flag. */
s32 fmGslReleaseActiveResourceBuffers(void) {
    if (frFontResourceList.head == 0) {
        return 0;
    }
    sdfReleaseResourceAllocation(frFontResourceList.nodeAllocation);
    sdfReleaseResourceAllocation(frFontResourceList.recordStorageAllocation);
    frFontResourceList.head = 0;
    return 1;
}

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
FntNode *frFontDetachFirstResourceNode(void) {
    FntNode *node = frFontResourceList.head->next;

    if (node->nodeIndex == 0) {
        return NULL;
    }
    node->prev->next = node->next;
    frFontResourceList.count -= 1;
    node->next->prev = node->prev;
    return node;
}
