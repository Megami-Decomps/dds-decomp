#ifndef FILE_CONFIG_OPTION_API_H
#define FILE_CONFIG_OPTION_API_H

#include "common.h"

/* Menu indices for the four configuration options persisted in world.slotFlags. */
typedef enum FileConfigOption {
    FILE_CONFIG_OPTION_RUMBLE = 0,
    FILE_CONFIG_OPTION_COMMAND_SELECTION_MEMORY = 1,
    FILE_CONFIG_OPTION_RESTORE_BATTLE_COMMAND = 2,
    FILE_CONFIG_OPTION_REVERSE_FIELD_CAMERA = 3
} FileConfigOption;

u8 fileToggleSlotFlagsBit(FileConfigOption option, u32 *flags);
s32 fileTestSavedSlotFlags(FileConfigOption option);

#endif /* FILE_CONFIG_OPTION_API_H */
