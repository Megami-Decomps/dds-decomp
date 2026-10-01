#include "common.h"
#include "sdf.h"

extern s32 sdfTrackedThreadSemaphore;

extern SdfThreadNode *sdfTrackedThreadHead;

s32 GetThreadId(void);

void WaitSema(s32 arg0);

void SignalSema(s32 arg0);

/* A negative ID means the calling thread; search under the list semaphore. */
SdfThreadNode *sdfFindThreadNode(s32 threadId) {
    s32 requestedId;
    SdfThreadNode *node;

    requestedId = threadId;
    if (requestedId < 0) {
        requestedId = GetThreadId();
    }
    WaitSema(sdfTrackedThreadSemaphore);
    node = sdfTrackedThreadHead;
    while (node != NULL) {
        if (node->threadId == requestedId) {
            break;
        }
        node = node->next;
    }
    SignalSema(sdfTrackedThreadSemaphore);
    return node;
}
