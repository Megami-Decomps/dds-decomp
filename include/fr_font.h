#ifndef FR_FONT_H
#define FR_FONT_H

#include "common.h"
#include "font.h"

struct frFontHandle_t;
struct MemNode;
struct sdfMemHandle_t;
struct ItfRelativeSegmentResourceView;

typedef struct FontGsControl_t {
    struct sdfMemHandle_t *mh;
    struct sdfMemHandle_t *ud_mh;
    s32 unit_width; /* 0x08 */
    s32 unit_height; /* 0x0C */
    s32 unit_size; /* 0x10 */
    s32 unit_num_max; /* 0x14 */
    u32 unit_nums; /* 0x18 */
    FontMem_t *root; /* 0x1C */
} FontGsControl_t;

/* Serialized resource header; DDS2 moved the lookup offset by eight bytes. */
typedef struct frFontHeader_t {
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
} frFontHeader_t;

typedef struct frFontData_t {
    struct sdfMemHandle_t *allocation;
    frFontHeader_t *resourceHeader;
    s32 metricByteCount;
    s32 valueByteCount;
    s8 *prop;
    void *ysht;
    GsFont_t **trans;
    struct ItfRelativeSegmentResourceView *resource;
    u32 data;
} frFontData_t;

/* frFontWork + 0x160; the small texture label aliases this owned subobject. */
typedef struct frFontAtlas_t {
    s32 width;
    u32 widthExponent;
    s32 height;
    u32 heightExponent;
    u32 bufferBase;
    u32 bufferWidth;
} frFontAtlas_t;

typedef struct frFontSystem_t {
    frFontData_t entries[9];
    s32 cachedItemCount;
    s32 itemCount;
    s32 glyphCount;
    struct MemNode *itemPool;
    struct MemNode *glyphPool;
    struct sdfGsMemSlice_t *textureHead0;
    struct sdfGsMemSlice_t *textureHead1;
    frFontAtlas_t atlas;
    u32 imageBuffers[6];
    u8 pad190[4];
    struct frFontHandle_t *glyphSlots[2];
} frFontSystem_t;

/* The SDK DMA tag is accessed as a quadword and as its four command words. */
typedef union frFontDmaTag_t {
    u128 value;
    u32 words[4];
} frFontDmaTag_t;

typedef struct frFontGsWrite_t {
    u64 value;
    u64 reg;
} frFontGsWrite_t;

typedef struct frFontGsPacket_t {
    frFontDmaTag_t dma;
    u64 gifTag;
    u64 gifRegisters;
    frFontGsWrite_t writes[6];
} frFontGsPacket_t;

typedef struct frFontSpriteCorner_t {
    u64 uv;
    u64 rgbaq;
    u64 xyz;
} frFontSpriteCorner_t;

typedef struct frFontSpritePacket_t {
    frFontDmaTag_t dma;
    u64 setupTag;
    u64 setupRegisters;
    frFontGsWrite_t tex0;
    frFontGsWrite_t tex1;
    frFontGsWrite_t clamp;
    u64 spriteTag;
    u64 spriteRegisters;
    frFontSpriteCorner_t corners[2];
} frFontSpritePacket_t;

extern frFontSystem_t frFontWork;
extern FontGsControl_t gFGC;

/* Decode encoded text into glyph nodes, optionally extending an existing chain. */
#ifdef VERSION_DDS1
/* DDS1 keeps the legacy placeholder spelling here: declaring the real name in
 * this header perturbs vendor-cc1 symbol-name CSE hashing in
 * field/fldFileResolver.c (fldQuerySelectedActorMotionState grows 8 bytes). */
struct frFontHandle_t *func_001951C8(const char *text, s8 fontIndex,
    s8 firstOption, s8 secondOption, struct frFontHandle_t *existingGlyph);
#endif
#ifdef VERSION_DDS2
struct frFontHandle_t *frSetFontString(const char *text, s8 fontIndex,
    s8 firstOption, s8 secondOption, struct frFontHandle_t *existingGlyph);
#endif
struct frFontHandle_t *frLinkFontString(const char *text, s8 fontIndex,
    s8 firstOption, s8 secondOption, struct frFontHandle_t *previousGlyph);
/* Update the first-option byte of every child on this glyph chain. */
void frSetFontColorNo(struct frFontHandle_t *glyph, u8 firstOption);
/* Replace each child render word along the borrowed glyph chain. */
void frSetFontColor(struct frFontHandle_t *parentNode, u32 colorWord);

/* Decode encoded text and append the resulting glyph to an optional parent. */
struct frFontHandle_t *itfCreateConvertedTextGlyph(s32 x, s32 y, s32 depth,
    u32 colors, const u8 *text, struct frFontHandle_t *parent);
struct frFontHandle_t *itfCreateFont3TextGlyph(s32 x, s32 y, s32 depth,
    u32 colors, const char *text, struct frFontHandle_t *parent);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct frFontHandle_t *glyph);

#ifdef VERSION_DDS1
struct frFontHandle_t *itfCreateMeasuredTextGlyph(s32 x, s32 y, s32 depth, s32 colors,
    const char *text, struct frFontHandle_t *parent);
#endif
#ifdef VERSION_DDS2
struct frFontHandle_t *itfCreateMeasuredTextGlyph(s32 x, s32 y, s32 depth, s32 colors,
    const char *text, struct frFontHandle_t *parent);
#endif

void frAddFontStyle(u8 slotIndex, u8 *resourceBytes,
    struct sdfMemHandle_t *allocation);

void frFontDrawAtlasGlyph(s32 x, s32 y, s32 width, s32 height, u8 style,
    u32 color, u32 depth, s32 filtered, const frFontUvRect_t *uv,
    const frFontAtlas_t *atlas, s32 surface);

typedef char FontGsControlSizeCheck[sizeof(FontGsControl_t) == 0x20 ? 1 : -1];
typedef char FrFontEntrySizeCheck[sizeof(frFontData_t) == 0x24 ? 1 : -1];
typedef char FrFontEntryAllocationOffsetCheck[((u32)&((frFontData_t *)0)->allocation == 0x00) ? 1 : -1];
typedef char FrFontAtlasSizeCheck[sizeof(frFontAtlas_t) == 0x18 ? 1 : -1];
typedef char FrFontSystemSizeCheck[sizeof(frFontSystem_t) == 0x19C ? 1 : -1];
typedef char FrFontSpritePacketSizeCheck[sizeof(frFontSpritePacket_t) == 0x90 ? 1 : -1];

#endif /* FR_FONT_H */
