#include "sdk_ps2.h"
#include "common.h"
#include "sdf.h"
#include "sdf_thread.h"

s32 sdfTrackedThreadSemaphore __attribute__((section(".sbss")));
SdfThreadNode *sdfTrackedThreadHead __attribute__((section(".sbss")));


/* A negative ID means the calling thread; search under the list semaphore. */
SdfThreadNode *sdfFindThreadNode(s32 threadId) {
    SdfThreadNode *node;

    if (threadId < 0) {
        threadId = GetThreadId();
    }
    WaitSema(sdfTrackedThreadSemaphore);
    node = sdfTrackedThreadHead;
    while (node != NULL) {
        if (node->threadId == threadId) {
            break;
        }
        node = node->next;
    }
    SignalSema(sdfTrackedThreadSemaphore);
    return node;
}
