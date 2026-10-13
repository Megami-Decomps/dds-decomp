#ifndef BTL_COMMAND_PARTNER_OUTPUT_H
#define BTL_COMMAND_PARTNER_OUTPUT_H

#include "btl.h"

typedef struct BtlCommandPartnerOutput {
    u16 commandId; /* 0x00 */
    u8 opaque02[2];
    void *firstPartner; /* 0x04 */
    void *secondPartner; /* 0x08 */
} BtlCommandPartnerOutput;

typedef char BtlCommandPartnerOutputSizeCheck[
    sizeof(BtlCommandPartnerOutput) == 0x0C ? 1 : -1];

#endif
