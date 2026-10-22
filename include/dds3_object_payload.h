#ifndef DDS3_OBJECT_PAYLOAD_H
#define DDS3_OBJECT_PAYLOAD_H

#include "common.h"

struct dds3Basic_t;

/* Object data word 8 stores a kind-dependent 32-bit payload. */
void dds3MatterObjectBasicSetAlpMode(struct dds3Basic_t *object, u32 value);
u32 dds3MatterObjectBasicGetAlpMode(const struct dds3Basic_t *object);

#endif /* DDS3_OBJECT_PAYLOAD_H */
