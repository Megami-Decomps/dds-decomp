#ifndef MNU_CALLBACK_LIST_H
#define MNU_CALLBACK_LIST_H

#include "common.h"
#include "sdf_list_node.h"

/* Callback-bearing indexed-list owner allocated as 0x18 bytes. */
typedef struct MnuCallbackList {
    u32 count;                    /* 0x00 */
    SdfListNode *head;            /* 0x04 */
    SdfListNode *tail;            /* 0x08 */
    u32 userData;                 /* 0x0C */
    void (*onRemove)();           /* 0x10: invoked for each removed node */
    void (*onDestroy)();          /* 0x14: invoked after clearing the list */
} MnuCallbackList;

typedef char MnuCallbackList_size_must_be_0x18[(sizeof(MnuCallbackList) == 0x18) ? 1 : -1];
typedef char MnuCallbackList_head_offset_must_be_4[((u32)&((MnuCallbackList *)0)->head == 0x04) ? 1 : -1];
typedef char MnuCallbackList_tail_offset_must_be_8[((u32)&((MnuCallbackList *)0)->tail == 0x08) ? 1 : -1];
typedef char MnuCallbackList_userData_offset_must_be_C[((u32)&((MnuCallbackList *)0)->userData == 0x0C) ? 1 : -1];
typedef char MnuCallbackList_onRemove_offset_must_be_10[((u32)&((MnuCallbackList *)0)->onRemove == 0x10) ? 1 : -1];
typedef char MnuCallbackList_onDestroy_offset_must_be_14[((u32)&((MnuCallbackList *)0)->onDestroy == 0x14) ? 1 : -1];

MnuCallbackList *mnuCreateCallbackNode(u32 userData);
void dds3DestroyCallbackNodeAfterLastNotification(MnuCallbackList *list);
void dds3SetCallbackNodeFirstListener(MnuCallbackList *list, void (*callback)());
void dds3SetCallbackNodeLastListener(MnuCallbackList *list, void (*callback)());

SdfListNode *dds3DetachIndexedListNodeAndRenumber(MnuCallbackList *list, SdfListNode *node);
SdfListNode *dds3RemoveListNodeAndNotify(MnuCallbackList *list, SdfListNode *node);
SdfListNode *func_00320CE0(MnuCallbackList *list, s32 key, u32 valueWord);
SdfListNode *func_00320D80(MnuCallbackList *list, SdfListNode *afterNode, s32 key, u32 valueWord);
SdfListNode *mnuFindResourceNodeByValue(MnuCallbackList *list, u32 value);
SdfListNode *mnuFindResourceNodeById(MnuCallbackList *list, u32 id);
SdfListNode *mnuFindResourceNodeByHandle(MnuCallbackList *list, u32 handle);

#endif
