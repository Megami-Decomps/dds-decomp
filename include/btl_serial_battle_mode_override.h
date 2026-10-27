#ifndef BTL_SERIAL_BATTLE_MODE_OVERRIDE_H
#define BTL_SERIAL_BATTLE_MODE_OVERRIDE_H

#include "common.h"

/* Return -1 to cancel; otherwise return the nonnegative next battle mode. */
typedef s32 BtlSerialBattleModeOverrideResult;
typedef BtlSerialBattleModeOverrideResult (*BtlSerialBattleModeOverrideCallback)(void);

enum {
    BTL_SERIAL_BATTLE_MODE_OVERRIDE_CANCEL = -1,
};

#endif /* BTL_SERIAL_BATTLE_MODE_OVERRIDE_H */
