#include "common.h"
#include "dds_nested_resource.h"
#include "mnu_callback_list.h"
#include "sdf_resource.h"
#include "sdf_handler_node.h"

extern void (*sdfTickCallback)(void);

/* Append a value to the first resource list in a two-list owner. */
void dds3AppendFirstResourceListValue(MnuCallbackListPair *lists, u32 option) {
    dds3AppendIndexedListNode(lists->first, 0, option);
}

u32 mnuInsertResourceHandleAfterMatchingId(MnuCallbackListPair *pair, u32 key, u32 value) {
    SdfListNode *node = mnuFindResourceNodeById(pair->first, key);
    if (node) {
        return (u32)dds3InsertIndexedListNodeAfter(pair->first, node, 0, value);
    }
    return 0;
}

void mnuRemoveMatchedNodesFromLinkedResourceLists(MnuCallbackListPair *pair, u32 key) {
    SdfListNode *node = mnuFindResourceNodeById(pair->first, key);
    if (node == 0) {
        return;
    }
    dds3RemoveListNodeAndNotify(pair->second,
                                mnuFindResourceNodeByHandle(pair->second, (u32)node->value));
    dds3RemoveListNodeAndNotify(pair->first, node);
}

extern s32 mnuClearResourceList(MnuCallbackList *);

s64 mnuClearOwnedResourceListPair(MnuCallbackListPair *pair) {
    mnuClearResourceList(pair->first);
    return mnuClearResourceList(pair->second);
}

void func_00324F20(MnuCallbackList **list, u32 handle) {
    mnuFindResourceNodeByHandle(*list, handle);
}

void func_00324F38(MnuCallbackList **list, u32 id) {
    mnuFindResourceNodeById(*list, id);
}
SdfHandlerNode *sdfHandlerNodeListHead __attribute__((section(".sdata"))) = NULL;

/* Workspace bytes passed to the console DMA setup allocation. */
s32 D_004389C0 __attribute__((section(".sdata"))) = 0x100000;

void (*sdfTickCallback)(void) __attribute__((section(".sdata"))) = NULL;

