#ifndef EVT_PAD_H
#define EVT_PAD_H

#include "common.h"

/* Shared 0x40-byte event-viewer control state. Unobserved input slots remain
 * opaque; the named bytes are consumed by both titles' editor handlers. */
typedef struct EvtPad {
    u8 pad00[6];
    s8 frameEnd; /* +0x06: jump from group properties to the last key. */
    s8 frameStart; /* +0x07: jump from group properties to the first key. */
    u8 pad08[0x18];
    s8 syncKey; /* 0x20 */
    s8 confirm; /* 0x21 */
    u8 pad22;
    s8 cancel; /* 0x23 */
    s8 decOne; /* 0x24 */
    s8 incOne; /* 0x25 */
    s8 decTen; /* 0x26 */
    s8 incTen; /* 0x27 */
    u8 decHun; /* 0x28 */
    s8 unk29; /* 0x29: coarse decrease in the property editor. */
    u8 incHun; /* 0x2A */
    s8 unk2B; /* 0x2B: coarse increase in the property editor. */
    s8 apply; /* 0x2C */
    s8 unk2D;
    u8 pad2E[0x12]; /* Remaining two-bank, two-port input backing. */
} EvtPad;

typedef char EvtPad_size_must_be_0x40[(sizeof(EvtPad) == 0x40) ? 1 : -1];
typedef char EvtPad_frameEnd_offset_must_be_0x06[
    ((u32)&((EvtPad *)0)->frameEnd == 0x06) ? 1 : -1];
typedef char EvtPad_frameStart_offset_must_be_0x07[
    ((u32)&((EvtPad *)0)->frameStart == 0x07) ? 1 : -1];
typedef char EvtPad_syncKey_offset_must_be_0x20[
    ((u32)&((EvtPad *)0)->syncKey == 0x20) ? 1 : -1];
typedef char EvtPad_confirm_offset_must_be_0x21[
    ((u32)&((EvtPad *)0)->confirm == 0x21) ? 1 : -1];
typedef char EvtPad_cancel_offset_must_be_0x23[
    ((u32)&((EvtPad *)0)->cancel == 0x23) ? 1 : -1];
typedef char EvtPad_decOne_offset_must_be_0x24[
    ((u32)&((EvtPad *)0)->decOne == 0x24) ? 1 : -1];
typedef char EvtPad_incOne_offset_must_be_0x25[
    ((u32)&((EvtPad *)0)->incOne == 0x25) ? 1 : -1];
typedef char EvtPad_decTen_offset_must_be_0x26[
    ((u32)&((EvtPad *)0)->decTen == 0x26) ? 1 : -1];
typedef char EvtPad_incTen_offset_must_be_0x27[
    ((u32)&((EvtPad *)0)->incTen == 0x27) ? 1 : -1];
typedef char EvtPad_decHun_offset_must_be_0x28[
    ((u32)&((EvtPad *)0)->decHun == 0x28) ? 1 : -1];
typedef char EvtPad_incHun_offset_must_be_0x2A[
    ((u32)&((EvtPad *)0)->incHun == 0x2A) ? 1 : -1];
typedef char EvtPad_apply_offset_must_be_0x2C[
    ((u32)&((EvtPad *)0)->apply == 0x2C) ? 1 : -1];

#endif /* EVT_PAD_H */
