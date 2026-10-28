#ifndef MNU_STANDARD_PARTNER_REQUIREMENTS_API_H
#define MNU_STANDARD_PARTNER_REQUIREMENTS_API_H

#include "btl_command_id.h"

/* True when a command has standard, non-special partner requirements. */
u8 mnuHasStandardPartnerRequirements(BtlCommandId commandId);

#endif /* MNU_STANDARD_PARTNER_REQUIREMENTS_API_H */
