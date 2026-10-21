#ifndef BTL_TUTORIAL_DIALOG_WORK_H
#define BTL_TUTORIAL_DIALOG_WORK_H

#include "common.h"

/* Shared by battle notice and tutorial tasks, which allocate 0x18 bytes. */
typedef struct BtlTutorialDialogWork {
    s8 phase;
    u8 pad01[3];
    s32 counter;
    u16 alpha;
    u8 pad0A[0xA];
    s32 itemIndex;
} BtlTutorialDialogWork;

typedef char BtlTutorialDialogWorkSize[
    (sizeof(BtlTutorialDialogWork) == 0x18) ? 1 : -1];
typedef char BtlTutorialDialogWorkCounterOffset[
    ((unsigned long)&((BtlTutorialDialogWork *)0)->counter == 0x04) ? 1 : -1];
typedef char BtlTutorialDialogWorkAlphaOffset[
    ((unsigned long)&((BtlTutorialDialogWork *)0)->alpha == 0x08) ? 1 : -1];
typedef char BtlTutorialDialogWorkItemIndexOffset[
    ((unsigned long)&((BtlTutorialDialogWork *)0)->itemIndex == 0x14) ? 1 : -1];

#endif
