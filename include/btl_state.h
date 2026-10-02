#ifndef BTL_STATE_H
#define BTL_STATE_H

#include "btl.h"

struct KwlnTask;

/* Full battle-work layout for state users; unit/task-only users include btl.h. */
#ifdef VERSION_DDS1

/* Observed fields of the singleton battle work returned by func_001A17F0.
 * This is one object, not separate script/event/actor-update contexts. Holes
 * remain opaque. DDS2 has a different layout, not a uniform offset shift. */
typedef struct BtlState {
    u8 pad000[0x1C0];
    s16 eventTaskId; /* 0x1C0: -1 when no event task is available */
    u8 pad1C2[2];
    u32 scriptFlags; /* 0x1C4 */
    u32 eventFlags; /* 0x1C8 */
    s16 eventActive; /* 0x1CC */
    u8 pad1CE[2];
    s32 eventAction; /* 0x1D0 */
    s32 eventResult; /* 0x1D4 */
    void *eventRequest; /* 0x1D8 */
    void *eventData; /* 0x1DC */
    BtlUnit *eventUnit; /* 0x1E0 */
    s32 sequenceHandle; /* 0x1E4 */
    s32 scriptHandle; /* 0x1E8 */
    void *eventAssets; /* 0x1EC */
    u8 pad1F0[4];
    u32 battleFlags; /* 0x1F4 */
    u8 pad1F8[4];
    u32 unk_1FC; /* Bit 0x800 bypasses command-block-reason checks. */
    u8 pad200[0x24];
    BtlTask *tasks; /* 0x224 */
    BtlUnit *units; /* 0x228 */
    u8 pad22C[0x14];
    struct BattleModelEntry *modelEntries; /* 0x240 */
    u8 pad244[4];
    u16 turnPhase; /* 0x248 */
    u8 pad24A[2];
    u16 mode; /* 0x24C */
    u8 pad24E[2];
    s32 turnCount; /* 0x250 */
    u8 pad254[4];
    u8 eventReady; /* 0x258 */
    u8 pad259[3];
    u16 phase; /* 0x25C */
    u8 requestMode; /* 0x25E: script sets this to 4 with requestArgument */
    u8 pad25F[0x1D];
    s32 battleMode; /* 0x27C */
    s32 requestArgument; /* 0x280: sign-extended script halfword */
    u8 pad284[0x18];
    struct KwlnTask *scriptOwner; /* 0x29C: parent task; script tasks use its priority minus one */
    s32 scriptTask; /* 0x2A0: scheduler task handle, not another list pointer */
    s32 boundTask; /* 0x2A4: actor-slot binding task */
    u8 pad2A8[0x20C];
    u32 buttonTextureHandle; /* 0x4B4 */
    u8 pad4B8[0xDC];
    void (*bossCleanup)(void); /* 0x594 */
    u8 pad598[0x20];
    s32 unk_5B8;
    u8 pad5BC[0x14];
    void (*cleanup)(void); /* 0x5D0 */
    u8 pad5D4[0x1C];
    void (*updateCallback)(void); /* 0x5F0 */
    u8 pad5F4[0xA0];
    BattleEffectState *effect; /* 0x694 */
    u8 pad698[0xC];
    s32 unk_6A4;
    s32 unk_6A8;
    u8 pad6AC[8];
    s32 unk_6B4;
    s32 unk_6B8;
    u8 pad6BC[8];
    s32 unk_6C4;
    u8 pad6C8[0x10];
    s32 unk_6D8;
    u8 pad6DC[8];
    s32 unk_6E4;
    s32 unk_6E8;
    u8 pad6EC[8];
    s32 unk_6F4;
    u8 pad6F8[0x10];
    s32 unk_708;
    s32 table0[0x20]; /* 0x70C */
    s32 table1[0x180]; /* 0x78C */
    s32 table2[0x20]; /* 0xD8C */
    s8 unk_E0C;
    u8 unk_E0D;
    s16 unk_E0E;
} BtlState;
#endif /* VERSION_DDS1 */

#ifdef VERSION_DDS2

/* Same singleton battle work as DDS1, returned by func_001AA6F8. In particular
 * the script-owner/task pair is +0x2C4/+0x2C8, not DDS1's offsets plus 0x24.
 * Sparse actor-list, effect-context and script-resource views are this object. */
typedef struct BtlState {
    u8 pad000[0x1E4];
    s16 eventTaskId; /* 0x1E4 */
    u8 pad1E6[2];
    u32 scriptFlags; /* 0x1E8 */
    u32 eventFlags; /* 0x1EC */
    s16 eventActive; /* 0x1F0 */
    u8 pad1F2[2];
    s32 eventAction; /* 0x1F4 */
    s32 eventResult; /* 0x1F8 */
    void *eventRequest; /* 0x1FC */
    void *eventData; /* 0x200 */
    BtlUnit *eventUnit; /* 0x204 */
    s32 sequenceHandle; /* 0x208 */
    s32 scriptHandle; /* 0x20C */
    void *eventAssets; /* 0x210 */
    u8 pad214[4];
    u32 battleFlags; /* 0x218 */
    u8 pad21C[4];
    u32 unk220;
    u8 pad224[0x24];
    BtlTask *tasks; /* 0x248 */
    BtlUnit *units; /* 0x24C */
    u8 pad250[0x20];
    u16 mode; /* 0x270 */
    u8 pad272[2];
    s32 turnCount; /* 0x274 */
    u8 pad278[4];
    u8 eventReady; /* 0x27C */
    u8 pad27D[3];
    u16 phase; /* 0x280 */
    u8 requestMode; /* 0x282 */
    u8 pad283[0x1D];
    s32 battleMode; /* 0x2A0 */
    s32 requestArgument; /* 0x2A4 */
    u8 pad2A8[0x1C];
    struct KwlnTask *scriptOwner; /* 0x2C4: parent task; script tasks use its priority minus one */
    s32 scriptTask; /* 0x2C8: also supplies the task passed to scrSetCurrentActor */
    s32 boundTask; /* 0x2CC */
    u8 pad2D0[0x218];
    u32 buttonTextureHandle; /* 0x4E8 */
    u8 pad4EC[0xDC];
    void (*bossCleanup)(void); /* 0x5C8 */
    u8 pad5CC[0x120];
    s32 (*scriptReturnHook)(); /* Optional script-return hook; preserve its unspecified retail prototype. */
    u8 pad6F0[0x28];
    struct BattleLinkedEffectState *effect; /* 0x718 */
    u8 pad71C[0xC];
    s32 unk_728;
    s32 unk_72C;
    u8 pad730[8];
    s32 unk_738;
    s32 unk_73C;
    u8 pad740[8];
    s32 unk_748;
    u8 pad74C[0x10];
    s32 unk_75C;
    u8 pad760[8];
    s32 unk_768;
    s32 unk_76C;
    u8 pad770[8];
    s32 unk_778;
    u8 pad77C[0x10];
    s32 unk_78C;
    s32 table0[0x30]; /* 0x790 */
    s32 table1[0x180]; /* 0x850 */
    s32 table2[0x60]; /* 0xE50 */
    s8 unk_E0C; /* 0xFD0: legacy opaque name, not a DDS2 offset */
    u8 unk_E0D; /* 0xFD1 */
    s16 unk_E0E; /* 0xFD2 */
} BtlState;
#endif /* VERSION_DDS2 */

#endif /* BTL_STATE_H */
