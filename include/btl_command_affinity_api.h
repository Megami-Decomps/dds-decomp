#ifndef BTL_COMMAND_AFFINITY_API_H
#define BTL_COMMAND_AFFINITY_API_H

#include "btl_command_partner_output.h"

s32 btlHasRequiredCommandAffinityPartners(BtlUnit *unit, s32 command);
s32 btlResolveCommandAffinityPartners(BtlUnit *unit, s32 command,
                                     BtlCommandPartnerOutput *output);

#endif
