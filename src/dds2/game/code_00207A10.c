#include "common.h"

extern s32 btlRepositionPartyAroundBattleCenter(void);

/* Persona 4 func_001789d0 @ 001789D0 (src/promoted/code1_0017.c), recompiled unchanged */
s32 btlCommandRecenterParty(void)
{
    btlRepositionPartyAroundBattleCenter();
    btlArrangeOpponentFormationSlots();
    return 1;
}
