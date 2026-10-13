#ifndef DDS_SDF_MESSAGE_H
#define DDS_SDF_MESSAGE_H

#include "common.h"

/* Shared message written by the model provider and consumed by pool cleanup. */
typedef struct SdfMsg {
    s32 firstWord;
    u32 *work;
    s32 thirdWord;
    s32 fourthWord;
} SdfMsg; /* 0x10 */

typedef char SdfMsg_size_check[(sizeof(SdfMsg) == 0x10) ? 1 : -1];
typedef char SdfMsg_work_offset_check[
    ((u32)&((SdfMsg *)0)->work == 0x4) ? 1 : -1];
typedef char SdfMsg_third_word_offset_check[
    ((u32)&((SdfMsg *)0)->thirdWord == 0x8) ? 1 : -1];
typedef char SdfMsg_fourth_word_offset_check[
    ((u32)&((SdfMsg *)0)->fourthWord == 0xC) ? 1 : -1];

void sdfInstallPoolNodeReleaseCallbacks(u32 *work);
void sdfStoreMessageWordsAndNotifyConsumer(SdfMsg *message, s32 firstWord, u32 *work, s32 thirdWord, s32 fourthWord);

#endif
