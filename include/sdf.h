#ifndef SDF_H
#define SDF_H

#include "common.h"

/* Texture handle, optional resources and double-buffered GPU command data. */
typedef struct SdfTexRef {
    void *unk0;
    s32 refCount;
} SdfTexRef;

typedef struct SdfTexBuf {
    s32 unk0;
    u8 pad4[0xC];
    u64 unk10;
    u64 unk18;
    u64 unk20;
    u64 unk28;
    u64 unk30;
} SdfTexBuf;

typedef struct {
    u8 pad00[0xC];
    u32 word;
} SdfTexResource;

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

typedef struct SdfSemaObj {
    s32 unk0; /* Semaphore ID. */
    void *unk4;
    void *unk8;
    void *unkC;
    s32 unk10;
} SdfSemaObj;

/* DMA packet list and its builders. */
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

typedef struct SdfPacket {
    u64 unk0;
    u64 unk8;
    u64 unk10;
    u64 unk18;
} SdfPacket;

typedef struct SdfDmaSrc {
    u16 unk0;
    u8 pad2[6];
    u64 unk8;
} SdfDmaSrc;

typedef struct SdfDmaNode {
    u64 unk0;
    u64 unk8;
    int __attribute__((mode(TI))) unk10;
} SdfDmaNode;

typedef struct SdfResEntry {
    u8 pad00[0xC];
    u32 unk0C;
} SdfResEntry;

typedef struct SdfBigPacket {
    u8 pad00[8];
    s32 unk08;
    u8 pad0C[0x24];
    u64 unk30;
    u8 pad38[0x48];
    u64 unk80;
} SdfBigPacket;

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

typedef struct SdfResource {
    u32 unk00;
    struct SdfResource *next;
    u8 pad08[0x18];
    s32 id;
} SdfResource;

/* Graphics packet header (not the generic SdfNodeCursor linked-list node). */
typedef struct SdfNode {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} SdfNode;

/* Asset and resource entry layout shared by both game versions. */
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
