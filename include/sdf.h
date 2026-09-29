#ifndef SDF_H
#define SDF_H

#include "common.h"

/* Reference-counted texture handle (0x8); DDS1 sdf/sdfTex.c and DDS1/2 SdfTex owners. */
typedef struct SdfTexRef {
    void *unk0;
    s32 refCount;
} SdfTexRef;

/* Texture buffer GPU command words (0x38); DDS1/2 game/code_002D10B0/00329F60.c. */
typedef struct SdfTexBuf {
    s32 unk0;
    u8 pad4[0xC];
    u64 unk10;
    u64 unk18;
    u64 unk20;
    u64 unk28;
    u64 unk30;
} SdfTexBuf;

/* Texture resource word at +0xC (0x10); DDS1/2 game/code_002D10B0/00329F60.c via SdfTex. */
typedef struct {
    u8 pad00[0xC];
    u32 word;
} SdfTexResource;

/* Linked texture and its two buffers/resources (0x40); DDS1/2 sdf/sdfTex.c and game texture units. */
typedef struct SdfTex {
    struct SdfTex *next;
    struct SdfTex *prev;
    SdfTexRef *reference;
    s16 unkC;
    s16 unkE;
    SdfTexResource *primaryResource;
    SdfTexResource *secondaryResource;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u16 unk1C;
    u8 unk1E;
    u8 unk1F;
    s32 unk20;
    s32 unk24;
    SdfTexBuf *unk28;
    SdfTexBuf *unk2C;
    u8 *data;
    s32 dataSize;
    s32 unk38;
    void *unk3C;
} SdfTex;

/* Semaphore ID and attached work pointers (0x14); DDS1/2 game/code_002D10B0/00329F60.c. */
typedef struct SdfSemaObj {
    s32 unk0; /* Semaphore ID. */
    void *unk4;
    void *unk8;
    void *unkC;
    s32 unk10;
} SdfSemaObj;

/* DMA packet list cursors and endpoints (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfListHead {
    u32 unk0;
    u32 first;
    u32 last;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
} SdfListHead;

/* Four-doubleword DMA packet payload (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfPacket {
    u64 unk0;
    u64 unk8;
    u64 unk10;
    u64 unk18;
} SdfPacket;

/* DMA source header and trailing 64-bit field (0x10); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfDmaSrc {
    u16 unk0;
    u8 pad2[6];
    u64 unk8;
} SdfDmaSrc;

/* DMA node with a 128-bit command (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfDmaNode {
    u64 unk0;
    u64 unk8;
    int __attribute__((mode(TI))) unk10;
} SdfDmaNode;

/* Resource entry with word at +0xC (0x10); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfResEntry {
    u8 pad00[0xC];
    u32 unk0C;
} SdfResEntry;

/* Large DMA packet fields at +0x30/+0x80 (0x88); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfBigPacket {
    u8 pad00[8];
    s32 unk08;
    u8 pad0C[0x24];
    u64 unk30;
    u8 pad38[0x48];
    u64 unk80;
} SdfBigPacket;

/* Two-slot packet builder and source/mode state (0x60); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfPacketBuilder {
    u8 pad00[4];
    void (*prepare)(void);
    u8 pad08[8];
    SdfPacket packets[2];
    s32 source;
    s32 data;
    s32 region;
    s32 mode;
} SdfPacketBuilder;

/* Linked named resource (0x24); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfResource {
    u32 unk00;
    struct SdfResource *next;
    u8 pad08[0x18];
    s32 id;
} SdfResource;

/* Graphics packet header (0x10); DDS1/2 game/code_002D9748/003325F8.c. */
typedef struct SdfNode {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} SdfNode;

/* Asset holding texture and resource entries (0x48); DDS1/2 game/code_002D9748/003325F8.c. */
typedef struct SdfAsset {
    u8 pad00[8];
    void *entries[2];
    u32 unk10;
    u32 unk14;
    u32 unk18;
    f32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    SdfTex *unk2C;
    u8 pad30[8];
    void *third;
    void *fourth;
    f32 unk40;
    f32 unk44;
} SdfAsset;

/* Asset entry with packed payload words (0x50); DDS1/2 game/code_002D9748/003325F8.c. */
typedef struct SdfAssetEntry {
    u32 pad00;
    u32 unk04;
    u32 unk08;
    u32 pad0C;
    u32 unk10;
    u32 unk14;
    u32 pad18;
    f32 unk1C;
    u8 pad20[0x18];
    u64 unk38;
    u64 unk40;
    u64 unk48;
} SdfAssetEntry;

#endif /* SDF_H */
