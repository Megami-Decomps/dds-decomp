#ifndef UI_OWNER_REF_H
#define UI_OWNER_REF_H

#include "common.h"

struct ItfMesState;

/* Consumer view: the interface manager reads the owner pointer after 12
 * opaque bytes; this does not claim the full extent of the UiOwnerRef record. */
typedef struct UiOwnerRef {
    u8 opaque00[0xC];
    struct ItfMesState *owner;
} UiOwnerRef;

typedef char UiOwnerRefOwnerOffsetCheck[
    ((u32)&((UiOwnerRef *)0)->owner == 0xC) ? 1 : -1];
typedef char UiOwnerRefViewSizeCheck[(sizeof(UiOwnerRef) == 0x10) ? 1 : -1];

#endif /* UI_OWNER_REF_H */
