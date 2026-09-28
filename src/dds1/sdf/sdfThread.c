#include "common.h"

typedef struct SdfThreadNode {
    struct SdfThreadNode *next;
    s32 threadId;
} SdfThreadNode;

extern s32 D_003BD998;
extern SdfThreadNode *D_003BD99C;

s32 GetThreadId(void);
void WaitSema(s32 arg0);
void SignalSema(s32 arg0);

SdfThreadNode *sdfFindThreadById(s32 threadId) {
    SdfThreadNode *node;

    if (threadId < 0) {
        threadId = GetThreadId();
    }
    WaitSema(D_003BD998);
    node = D_003BD99C;
    while (node != NULL) {
        if (node->threadId == threadId) {
            break;
        }
        node = node->next;
    }
    SignalSema(D_003BD998);
    return node;
}
