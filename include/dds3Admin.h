#ifndef DDS3_ADMIN_H
#define DDS3_ADMIN_H

#include "common.h"

/* Administration flags, value and eight-slot history (0x24); DDS1/2 kernel records. */
typedef struct AdminWork {
    u32 flags;              /* +0x00 */
    u32 value;              /* +0x04: returned by scoped object state accessors */
    s8 unk08;
    s8 unk09;
    u8 historyIndex;        /* +0x0A */
    u8 pad0B;
    s8 signedHistory[8];    /* +0x0C */
    u8 unsignedHistory[8];  /* +0x14 */
    void *unk1C;
    u8 unk20;
    u8 unk21;
    u8 pad22[2];
} AdminWork;

#endif /* DDS3_ADMIN_H */
