#ifndef DAT_CURRENCY_REQUIREMENT_API_H
#define DAT_CURRENCY_REQUIREMENT_API_H

#include "common.h"

typedef u8 DatCurrencyRequirementResult;
enum {
    DAT_CURRENCY_REQUIREMENT_UNMET = 0,
    DAT_CURRENCY_REQUIREMENT_MET = 1
};

/* Compare the signed balance with the amount; negative amounts use the same comparison. */
DatCurrencyRequirementResult datHasEnoughCurrency(s32 requiredAmount);

#endif
