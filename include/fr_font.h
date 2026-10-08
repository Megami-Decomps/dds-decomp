#ifndef FR_FONT_H
#define FR_FONT_H

#include "common.h"

struct FrFontGlyph;
struct MemNode;
struct FrFontRecord;

/* The cached node owns the four GS UV coordinates following its first word. */
typedef struct FrFontUvRect {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} FrFontUvRect;

typedef struct FntNode {
    void *unk0;
    FrFontUvRect uv;
    struct FrFontRecord *item;
    struct FntNode *prev;
    struct FntNode *next;
} FntNode;

typedef struct FrFontRecord {
    u16 id;
    u8 pad02[2];
    u16 refs;
    u8 pad06[2];
    FntNode *list;
} FrFontRecord;

typedef struct FntList {
    u8 unk0[0x18];
    s32 count;
    FntNode *head;
} FntList;

/* Serialized resource header; DDS2 moved the lookup offset by eight bytes. */
typedef struct FrFontHeader {
    u32 tableOffset;
    u8 pad04[6];
    u8 tableCount;
    u8 pad0B[3];
    u16 widthCount;
    u16 cellWidth;
    u16 cellHeight;
    u8 pad14[2];
    u8 hasExtra;
    u8 pad17;
#ifdef VERSION_DDS2
    u8 pad18[8];
#endif
    u32 lookupOffset;
} FrFontHeader;

typedef struct FrFontEntry {
    void *buffer;
    FrFontHeader *resourceHeader;
    s32 metricByteCount;
    s32 valueByteCount;
    s8 *flagBytes;
    void *unk14;
    FrFontRecord **slots;
    void *resource;
    u32 unk20;
} FrFontEntry;

/* frFontWork + 0x160; the small texture label aliases this owned subobject. */
typedef struct FrFontAtlas {
    s32 width;
    u32 widthExponent;
    s32 height;
    u32 heightExponent;
    u32 bufferBase;
    u32 bufferWidth;
} FrFontAtlas;

typedef struct FrFontSystem {
    FrFontEntry entries[9];
    s32 cachedItemCount;
    s32 itemCount;
    s32 glyphCount;
    struct MemNode *itemPool;
    struct MemNode *glyphPool;
    void *textureHead0;
    void *textureHead1;
    FrFontAtlas atlas;
    s32 imageBuffers[6];
    u8 pad190[4];
    struct FrFontGlyph *glyphSlots[2];
} FrFontSystem;

/* The SDK DMA tag is accessed as a quadword and as its four command words. */
typedef union FrFontDmaTag {
    u128 value;
    u32 words[4];
} FrFontDmaTag;

typedef struct FrFontGsWrite {
    u64 value;
    u64 reg;
} FrFontGsWrite;

typedef struct FrFontGsPacket {
    FrFontDmaTag dma;
    u64 gifTag;
    u64 gifRegisters;
    FrFontGsWrite writes[6];
} FrFontGsPacket;

typedef struct FrFontSpriteVertex {
    u64 uv;
    u64 rgbaq;
    u64 xyz;
} FrFontSpriteVertex;

typedef struct FrFontSpritePacket {
    FrFontDmaTag dma;
    u64 setupTag;
    u64 setupRegisters;
    FrFontGsWrite tex0;
    FrFontGsWrite tex1;
    FrFontGsWrite clamp;
    u64 spriteTag;
    u64 spriteRegisters;
    FrFontSpriteVertex corners[2];
} FrFontSpritePacket;

extern FrFontSystem frFontWork;
extern FntList frFontResourceList;


#ifdef VERSION_DDS1
void func_00193D70(s32 x, s32 y, s32 width, s32 halfHeight, u8 style,
    u32 color, u32 depth, s32 enabled, const FrFontUvRect *uv,
    const FrFontAtlas *atlas, s32 drawFlags);
#endif
#ifdef VERSION_DDS2
void func_0019BA00(s32 x, s32 y, s32 width, s32 halfHeight, u8 style,
    u32 color, u32 depth, s32 enabled, const FrFontUvRect *uv,
    const FrFontAtlas *atlas, s32 drawFlags);
#endif

typedef char FntNodeSizeCheck[sizeof(FntNode) == 0x20 ? 1 : -1];
typedef char FrFontRecordSizeCheck[sizeof(FrFontRecord) == 0x0C ? 1 : -1];
typedef char FrFontEntrySizeCheck[sizeof(FrFontEntry) == 0x24 ? 1 : -1];
typedef char FrFontAtlasSizeCheck[sizeof(FrFontAtlas) == 0x18 ? 1 : -1];
typedef char FrFontSystemSizeCheck[sizeof(FrFontSystem) == 0x19C ? 1 : -1];
typedef char FrFontSpritePacketSizeCheck[sizeof(FrFontSpritePacket) == 0x90 ? 1 : -1];

#endif /* FR_FONT_H */
