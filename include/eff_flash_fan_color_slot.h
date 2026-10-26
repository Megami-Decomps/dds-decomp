#ifndef EFF_FLASH_FAN_COLOR_SLOT_H
#define EFF_FLASH_FAN_COLOR_SLOT_H

#include "eff.h"

/* Writable packed RGBA values paired with the five vertices in a fan record. */
typedef struct EffFlashFanColorSlot {
    u32 color[5];
} EffFlashFanColorSlot;

typedef char EffFlashFanColorSlotSizeCheck[
    (sizeof(EffFlashFanColorSlot) == 0x14) ? 1 : -1];

/* Return the unchecked, writable five-color slot for one fan group. */
EffFlashFanColorSlot *dds3FlashPolyListGetVC(dds3FlashPolyList_t *pool, s32 groupIndex);

#endif /* EFF_FLASH_FAN_COLOR_SLOT_H */
