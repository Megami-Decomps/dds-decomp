#ifndef EVT_LIPS_LINK_H
#define EVT_LIPS_LINK_H

#include "common.h"

struct EvtUnit;
struct MdlCtx;

/* Consumer view of the model owner reached from an event lip-sync link. */
typedef struct EvtLipsMh {
    u8 opaque00[0x0C];
    struct MdlCtx *model; /* 0x0C */
} EvtLipsMh;

/* Consumer view of a lip-sync entry in the world's unit list. */
typedef struct EvtLipsLink {
    u8 opaque00[0x08];
    struct EvtUnit *unit; /* 0x08 */
    EvtLipsMh *mh; /* 0x0C */
} EvtLipsLink;

typedef char EvtLipsMhSizeCheck[(sizeof(EvtLipsMh) == 0x10) ? 1 : -1];
typedef char EvtLipsMhOpaqueOffsetCheck[
    ((u32)&((EvtLipsMh *)0)->opaque00 == 0x00) ? 1 : -1];
typedef char EvtLipsMhModelOffsetCheck[
    ((u32)&((EvtLipsMh *)0)->model == 0x0C) ? 1 : -1];
typedef char EvtLipsLinkSizeCheck[(sizeof(EvtLipsLink) == 0x10) ? 1 : -1];
typedef char EvtLipsLinkOpaqueOffsetCheck[
    ((u32)&((EvtLipsLink *)0)->opaque00 == 0x00) ? 1 : -1];
typedef char EvtLipsLinkUnitOffsetCheck[
    ((u32)&((EvtLipsLink *)0)->unit == 0x08) ? 1 : -1];
typedef char EvtLipsLinkMhOffsetCheck[
    ((u32)&((EvtLipsLink *)0)->mh == 0x0C) ? 1 : -1];

#endif /* EVT_LIPS_LINK_H */
