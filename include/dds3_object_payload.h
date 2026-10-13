#ifndef DDS3_OBJECT_PAYLOAD_H
#define DDS3_OBJECT_PAYLOAD_H

#include "common.h"

struct EffWorldNode;

/* Object data word 8 stores a kind-dependent 32-bit payload. */
void dds3SetObjectPayloadWord8(struct EffWorldNode *object, u32 value);
u32 dds3GetObjectPayloadWord8(struct EffWorldNode *object);

#endif /* DDS3_OBJECT_PAYLOAD_H */
