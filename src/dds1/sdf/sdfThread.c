#include "common.h"

typedef struct SdfThreadNode {
    struct SdfThreadNode *next;
    s32 unk4;
} SdfThreadNode;

extern s32 D_003BD998;
extern SdfThreadNode *D_003BD99C;

s32 GetThreadId(void);
void WaitSema(s32 arg0);
void SignalSema(s32 arg0);

SdfThreadNode *func_002CFBB0(s32 arg0) {
    s32 tid;
    SdfThreadNode *node;

    tid = arg0;
    if (tid < 0) {
        tid = GetThreadId();
    }
    WaitSema(D_003BD998);
    node = D_003BD99C;
    while (node != NULL) {
        if (node->unk4 == tid) {
            break;
        }
        node = node->next;
    }
    SignalSema(D_003BD998);
    return node;
}
