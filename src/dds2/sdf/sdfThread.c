#include "sdk_ps2.h"
#include "common.h"
#include "sdf.h"
#include "sdf_thread.h"

s32 sdfThreadSema __attribute__((section(".sbss")));
SdfThreadNode *sdfVblankThreadId __attribute__((section(".sbss")));




/* A negative ID means the calling thread; search under the list semaphore. */
SdfThreadNode *sdfFindThreadNode(s32 threadId) {
    s32 requestedId;
    SdfThreadNode *node;

    requestedId = threadId;
    if (requestedId < 0) {
        requestedId = GetThreadId();
    }
    WaitSema(sdfThreadSema);
    node = sdfVblankThreadId;
    while (node != NULL) {
        if (node->threadId == requestedId) {
            break;
        }
        node = node->next;
    }
    SignalSema(sdfThreadSema);
    return node;
}
