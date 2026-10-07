#ifndef EFF_EVENT_H
#define EFF_EVENT_H

#include "dds3obj.h"

/* Kind-8 constructors allocate this complete 0x10-byte resource payload.
 * Only the owned resource pointer is initialized or consumed here. */
typedef struct DdsSlotResourceBlock {
    ObjBase *resourceState;
    u8 pad04[0xC];
} DdsSlotResourceBlock;

typedef char DdsSlotResourceBlock_size_must_be_0x10[(sizeof(DdsSlotResourceBlock) == 0x10) ? 1 : -1];

ObjBase *dds3GetResourceOwnerHandle(EffWorldNode *object);

#endif
