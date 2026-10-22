#ifndef MNU_MANTRA_DRAW_H
#define MNU_MANTRA_DRAW_H

#include "common.h"

/* Draw processes of the mantra menu (code_0026DBF8): the draw-pool item,
 * the selection list's transition nodes and the unit panel's state. */

typedef struct MantraDisplayNode {
    s32 frame;
    u32 fromValue;
    u32 toValue;
    s16 transitionKind;
    u8 pad0E[2];
    struct MantraDisplayNode *next;
} MantraDisplayNode;

/* One draw process: construction supplies data, update requests completion,
 * draw runs each active frame, and release receives the item record. */
typedef struct MantraDrawItem {
    u32 kind;
    union {
        u32 flags;
        struct {
            u32 active : 1;
            u32 unkBits : 2;
            u32 state : 8;
            u32 released : 1;
            u32 category : 8;
            u32 unkHigh : 12;
        } bits;
    };
    u32 (*create)();
    void (*release)(struct MantraDrawItem *);
    s32 (*update)();
    void (*draw)();
    s16 startDelay;    /* 0x18: delay before construction */
    s16 endDelay;      /* 0x1A: delay before release */
    u32 createArg;     /* 0x1C: constructor argument */
    void *data;       /* 0x20: constructor result */
} MantraDrawItem;

typedef struct MantraListState {
    u32 unk0;
    MantraDisplayNode *head;
    u32 entries[8];
    s16 count;
    s16 index;
    u32 unk2C;
} MantraListState;

typedef struct MantraLampState {
    /* 0x00 */ u16 state;
    /* 0x02 */ s16 status; /* mnuSetMantraUnitPanelValue; 1..5 pick status icons 0x47..0x4B */
    /* 0x04 */ u16 timer;
    /* 0x06 */ u16 clock;
    /* 0x08 */ f32 value;
    /* 0x0C */ MantraListState list;
    /* 0x3C */ union {
        u32 bits;
        struct {
            u16 reverse : 1;    /* hide/show requested mid-fade: end hidden/shown */
            u16 countdown : 15; /* frames of reduced alpha after such a request */
            u16 : 16;
        };
    };
} MantraLampState;

typedef char MantraDisplayNode_size[(sizeof(MantraDisplayNode) == 0x14) ? 1 : -1];
typedef char MantraDrawItem_size[(sizeof(MantraDrawItem) == 0x24) ? 1 : -1];
typedef char MantraListState_size[(sizeof(MantraListState) == 0x30) ? 1 : -1];
typedef char MantraLampState_size[(sizeof(MantraLampState) == 0x40) ? 1 : -1];

#endif /* MNU_MANTRA_DRAW_H */
