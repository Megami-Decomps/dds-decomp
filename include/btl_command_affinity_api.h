#ifndef BTL_COMMAND_AFFINITY_API_H
#define BTL_COMMAND_AFFINITY_API_H

#include "btl_command_id.h"
#include "btl_command_partner_output.h"

typedef s32 BtlCommandAffinityPartnerAvailability;
enum {
    BTL_COMMAND_AFFINITY_PARTNERS_UNAVAILABLE = 0,
    BTL_COMMAND_AFFINITY_PARTNERS_AVAILABLE = 1,
};

typedef s32 BtlCommandAffinityPartnerResolution;
enum {
    BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED = 0,
    BTL_COMMAND_AFFINITY_PARTNERS_RESOLVED = 1,
};

BtlCommandAffinityPartnerAvailability btlHasRequiredCommandAffinityPartners(
    BtlUnit *unit, BtlCommandId commandId);
BtlCommandAffinityPartnerResolution btlResolveCommandAffinityPartners(
    BtlUnit *unit, BtlCommandId commandId, BtlCommandPartnerOutput *output);

#endif
