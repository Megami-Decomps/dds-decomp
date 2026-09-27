#include "common.h"

typedef struct SdfThreadNode {
    struct SdfThreadNode *next;
    s32 unk4;
} SdfThreadNode;

extern s32 D_004390F8;

extern SdfThreadNode *D_004390FC;

s32 GetThreadId(void);

void WaitSema(s32 arg0);

void SignalSema(s32 arg0);

SdfThreadNode *func_00328A60(s32 arg0) {
    s32 tid;
    SdfThreadNode *node;

    tid = arg0;
    if (tid < 0) {
        tid = GetThreadId();
    }
    WaitSema(D_004390F8);
    node = D_004390FC;
    while (node != NULL) {
        if (node->unk4 == tid) {
            break;
        }
        node = node->next;
    }
    SignalSema(D_004390F8);
    return node;
}
