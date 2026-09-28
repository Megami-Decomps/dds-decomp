#include "common.h"

typedef struct SdfThreadNode {
    struct SdfThreadNode *next;
    s32 threadId;
} SdfThreadNode;

extern s32 D_004390F8;

extern SdfThreadNode *D_004390FC;

s32 GetThreadId(void);

void WaitSema(s32 arg0);

void SignalSema(s32 arg0);

SdfThreadNode *func_00328A60(s32 threadId) {
    s32 requestedId;
    SdfThreadNode *node;

    requestedId = threadId;
    if (requestedId < 0) {
        requestedId = GetThreadId();
    }
    WaitSema(D_004390F8);
    node = D_004390FC;
    while (node != NULL) {
        if (node->threadId == requestedId) {
            break;
        }
        node = node->next;
    }
    SignalSema(D_004390F8);
    return node;
}
