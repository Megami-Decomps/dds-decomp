#ifndef SDF_PAC_RELOC_H
#define SDF_PAC_RELOC_H

#include "common.h"

/* Prefix on relocated PAC payloads: tableBytes is cleared after fixups. */
typedef struct PacReloc {
    s32 tableOffset; /* 0x00: relocation table displacement from payload */
    s32 tableBytes; /* 0x04: relocation table byte count */
    u8 pad08[8];
    u8 payload[1]; /* 0x10: relocated payload */
} PacReloc;

typedef char PacReloc_tableOffset_must_be_at_0[
    ((u32)&((PacReloc *)0)->tableOffset == 0) ? 1 : -1];
typedef char PacReloc_tableBytes_must_be_at_4[
    ((u32)&((PacReloc *)0)->tableBytes == 4) ? 1 : -1];
typedef char PacReloc_payload_must_be_at_0x10[
    ((u32)&((PacReloc *)0)->payload == 0x10) ? 1 : -1];

#endif /* SDF_PAC_RELOC_H */
