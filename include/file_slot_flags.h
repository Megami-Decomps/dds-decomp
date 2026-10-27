#ifndef FILE_SLOT_FLAGS_H
#define FILE_SLOT_FLAGS_H

#include "common.h"

/* Bits in each FileReqEntry slot-flags word.  Known writers set the
 * save-read and delete-before-write bits together, while consumers test them
 * for different operations. */
enum FileReqSlotFlag {
    FILE_REQ_SLOT_CAN_READ_SAVE = 0x1,
    FILE_REQ_SLOT_DIRECTORY_READY = 0x2,
    FILE_REQ_SLOT_DELETE_BEFORE_WRITE = 0x8,
    FILE_REQ_SLOT_DIRECTORY_DELETE_MASK = 0xA,
    FILE_REQ_SLOT_SELECTABLE_SAVE_MASK = 0xB
};

/* The sixteen-word array holds derived ten-slot menu display states, separate
 * from request flags. Zero also means not yet scanned; state two is a ready
 * directory without a recognized save. */
typedef u32 FileSlotDisplayState;
enum FileSlotDisplayState {
    FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE = 0,
    FILE_SLOT_DISPLAY_SAVE_PRESENT = 1,
    FILE_SLOT_DISPLAY_DIRECTORY_WITHOUT_SAVE = 2
};

extern FileSlotDisplayState fileSlotDisplayStates[16];

void fileReqClearSlotFlags(s32 request, s32 slot);
void fileReqSetSlotFlags(s32 request, s32 slot, s32 mask);
u32 fileReqGetSlotFlags(s32 request, s32 slot);

#endif
