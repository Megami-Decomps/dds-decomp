#ifndef EFF_RESOURCE_RECORDS_H
#define EFF_RESOURCE_RECORDS_H

#include "common.h"

struct sdfMemHandle_t;

/* Each mapped record owns status storage expanded while its resource is loaded. */
typedef struct EffMappedRecord {
    u8 pad00[0x14];
    u32 category;
    u32 statusBytes;
    u8 pad1C[4];
    u8 *status;
} EffMappedRecord;

/* Timed states borrow records through EffTimedState.source. Keep this
 * resource alive until those states no longer reference its records.
 */
typedef struct EffMappedResource {
    s32 count;
    struct sdfMemHandle_t *allocation;
    EffMappedRecord *records;
} EffMappedResource;

typedef char EffMappedRecord_size_must_be_0x24[(sizeof(EffMappedRecord) == 0x24) ? 1 : -1];
typedef char EffMappedResource_size_must_be_0x0C[(sizeof(EffMappedResource) == 0x0C) ? 1 : -1];

/* Header for count 0x6C-byte records; allocation is retained as a resource handle. */
typedef struct EffPayload {
    struct sdfMemHandle_t *allocation;
    u32 count;
    u8 *records;
} EffPayload;

#endif /* EFF_RESOURCE_RECORDS_H */
