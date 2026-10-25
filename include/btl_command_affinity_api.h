#ifndef BTL_COMMAND_AFFINITY_API_H
#define BTL_COMMAND_AFFINITY_API_H

#include "btl_command_partner_output.h"

typedef s32 BtlCommandAffinityPartnerAvailability;
enum {
    BTL_COMMAND_AFFINITY_PARTNERS_UNAVAILABLE = 0,
    BTL_COMMAND_AFFINITY_PARTNERS_AVAILABLE = 1,
};

BtlCommandAffinityPartnerAvailability btlHasRequiredCommandAffinityPartners(BtlUnit *unit, s32 command);
s32 btlResolveCommandAffinityPartners(BtlUnit *unit, s32 command,
                                     BtlCommandPartnerOutput *output);

#endif
