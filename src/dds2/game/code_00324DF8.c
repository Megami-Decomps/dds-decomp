#include "common.h"
#include "dds_nested_resource.h"
#include "mnu_callback_list.h"
#include "sdf_resource.h"

extern void (*sdfTickCallback)(void);

/* Reset the first resource list in a two-list owner. */
void func_00324DF8(u32 *lists, u32 option) {
    func_00320CE0((MnuCallbackList *)*lists, 0, option);
}

u32 mnuInsertResourceHandleAfterMatchingId(u32 *pair, u32 key, u32 value) {
    SdfListNode *node = mnuFindResourceNodeById((MnuCallbackList *)pair[0], key);
    if (node) {
        return (u32)func_00320D80((MnuCallbackList *)pair[0], node, 0, value);
    }
    return 0;
}

void mnuRemoveMatchedNodesFromLinkedResourceLists(u32 *pair, u32 key) {
    SdfListNode *node = mnuFindResourceNodeById((MnuCallbackList *)pair[0], key);
    if (node == 0) {
        return;
    }
    dds3RemoveListNodeAndNotify((MnuCallbackList *)pair[1],
                                mnuFindResourceNodeByHandle((MnuCallbackList *)pair[1], (u32)node->value));
    dds3RemoveListNodeAndNotify((MnuCallbackList *)pair[0], node);
}

extern s32 mnuClearResourceList(MnuCallbackList *);

s64 mnuClearOwnedResourceListPair(u32 *pair) {
    mnuClearResourceList((MnuCallbackList *)pair[0]);
    return mnuClearResourceList((MnuCallbackList *)pair[1]);
}

void func_00324F20(MnuCallbackList **list, u32 handle) {
    mnuFindResourceNodeByHandle(*list, handle);
}

void func_00324F38(MnuCallbackList **list, u32 id) {
    mnuFindResourceNodeById(*list, id);
}
INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389BC);

INCLUDE_SDATA(const s32, "game/code_00324DF8", D_004389C0);

INCLUDE_SDATA(const s32, "game/code_00324DF8", sdfTickCallback);

