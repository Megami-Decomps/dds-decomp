#ifndef EFF_H
#define EFF_H

#include "common.h"

/* Type-indexed effect dispatch object and its handler tables. */
typedef struct EffWork {
    u32 type;
    void *unk4;
    u32 unk8;
    u8 unkC[8];
    u32 unk14;
    u8 unk18[8];
    u32 unk20;
    u32 unk24;
    u8 unk28[0x10];
    void *unk38;
    u32 unk3C; /* Sound handle. */
} EffWork;

typedef struct EffHandler {
    void (*handler)(void *);
    u8 unk4[0x14];
} EffHandler;

typedef struct EffHandler32 {
    s32 (*handler)(s32);
    u8 unk4[0x14];
} EffHandler32;

typedef struct EffResult {
    s32 unk0; /* Input selector. */
    s32 unk4; /* Handler result. */
} EffResult;

typedef struct EffSub {
    u8 unk0[0x20];
    u8 unk20;
    u8 unk21[0x1F];
    u8 unk40;
    u8 unk41[0xF];
    u8 unk50;
} EffSub;

/* Resource records used by the effect message and slot allocator. */
typedef struct EffMsg {
    s32 unk0;
    s32 unk4;
    u8 unk8[0x20];
    u32 unk28;
    u32 unk2C;
    u8 unk30[4];
    u32 *unk34; /* Length-prefixed name at +4. */
    u8 unk38[8];
    u32 *unk40; /* Length-prefixed name at +4. */
} EffMsg;

typedef struct EffSlot38 {
    u8 unk0[0x30];
    s32 unk30;
    f32 unk34;
} EffSlot38;

typedef struct EffArrHdr {
    void *unk0; /* Slot array base. */
    u32 unk4;   /* Slot count. */
    void *unk8; /* Allocation handle. */
} EffArrHdr;

/* Billboard instance and its type-selecting dispatch entry. */
typedef struct BillObj {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u32 unk24;
    void (*unk28)();
    u16 unk2C;          /* Kind: child (0) or entry list (1). */
    u16 unk2E;
    void *unk30;
    u8 pad34[8];
    u16 unk3C;
    u8 pad3E[10];
    u32 unk48;
    u32 unk4C;
    u16 unk50;
    u8 pad52[6];
    u32 unk58;
    s32 entryCount;
    void *unk60;
} BillObj;

typedef struct BillData {
    u8 pad[8];
    s32 unk8;
    s32 entryCount;
    u8 pad10[4];
    s32 unk14;
    u8 pad18[24];
} BillData;

typedef struct {
    void *(*func)();
    void (*unk4)();
    u32 unk8;
} BillDispatch;

/* Resource-indexed effect buffer and its owning allocation. */
typedef struct EffectConfig {
    s16 unk00;
    u8 pad02[10];
} EffectConfig;

typedef struct EffectBufferRecord {
    u8 pad00[0x20];
    s32 unk20;
    s32 unk24;
    u8 pad28[0x18];
} EffectBufferRecord;

typedef struct EffectBufferTail {
    s32 allocation;
    EffectBufferRecord *records;
} EffectBufferTail;

#endif /* EFF_H */
