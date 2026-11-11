#ifndef FONT_H
#define FONT_H

#include "common.h"
#include "libsdf.h"

typedef struct frFontUvRect_t frFontUvRect_t;

typedef struct FontMem_t FontMem_t;

typedef struct GsFont_t GsFont_t;

typedef struct frTextHandle_t frTextHandle_t;

typedef struct frFontHandle_t frFontHandle_t;
typedef struct frFontRequest_t frFontRequest_t;

/* Atlas-list nodes use a flattened index; index zero is the sentinel. */
struct frFontUvRect_t {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
};

struct FontMem_t {
    u32 id;
    frFontUvRect_t uv;
    struct GsFont_t *user_data;
    struct FontMem_t *prev;
    struct FontMem_t *next;
};

struct GsFont_t {
    u16 font_no;
    u8 pad02[2];
    u16 used_count;
    u8 pad06[2];
    FontMem_t *font_mem;
};

/* Font items use their own 0x2C-byte pool, separate from parent chains. */
struct frTextHandle_t {
    union {
        u16 glyphCode;
        struct {
            u8 encodedContextByte;
            s8 spacing;
        } byteRoles;
    } glyphCodeOrContext;
    /* Child draw count: setup clears it; nonzero-fade draws increment it, and
     * the fade helper also uses it to select the jitter phase. */
    u16 drawCount;
    s32 x;
    s32 y;
    s32 advance;
    /* The same render-state word is also accessed by its low fade byte. */
    union {
        struct {
            u16 cellAdvance;
            u16 cellHeight;
        } parentDimensions;
        u32 renderWord;
        struct {
            u8 value;
            u8 opaque[3];
        } fadeByte;
    } parentDimensionsOrRenderWord;
    /* Rendering, glyph setup, and message shade paths use distinct byte views. */
    union {
        u32 renderValue;
        struct {
            u8 firstOption;
            u8 fontIndex;
            u8 secondOption;
            u8 sharedFlags;
        } setupBytes;
        struct {
            u8 green;
            u8 red;
            u8 blue;
            u8 opaque;
        } shadeColor;
    } renderValueOrSetupOrShade;
    struct {
        u8 cellWidth;
        u8 cellHeight;
        u8 opaque[2];
    } cellDimensions;
    GsFont_t *cachedItem;
    GsFont_t *sourceItem;
    struct frTextHandle_t *previous;
    struct frTextHandle_t *next;
};

/* Parent chain heads use the native 0x30-byte pool and own child-item links. */
struct frFontHandle_t {
    union {
        u16 glyphCode;
        struct {
            u8 encodedContextByte;
            s8 spacing;
        } byteRoles;
    } glyphCodeOrContext;
    /* Preserved by parent-chain initialization. */
    u16 drawCount;
    s32 x;
    s32 y;
    s32 advance;
    /* Parent glyphs store dimensions; render words also carry a fade byte. */
    union {
        struct {
            u16 cellAdvance;
            u16 cellHeight;
        } parentDimensions;
        u32 renderWord;
        struct {
            u8 value;
            u8 opaque[3];
        } fadeByte;
    } parentDimensionsOrRenderWord;
    /* Rendering, glyph setup, and message shade paths use distinct byte views. */
    union {
        u32 renderValue;
        struct {
            u8 firstOption;
            u8 fontIndex;
            u8 secondOption;
            u8 sharedFlags;
        } setupBytes;
        struct {
            u8 green;
            u8 red;
            u8 blue;
            u8 opaque;
        } shadeColor;
    } renderValueOrSetupOrShade;
    u32 childCount;
    frTextHandle_t *firstChild;
    frTextHandle_t *lastChild;
    struct frFontHandle_t *previous;
    struct frFontHandle_t *next;
    struct frFontHandle_t *chainHead;
    /* DDS only: message timing and lipsync state (0x44 vs SMT3's 0x30). */
    u32 timedControlCode; /* F214/F215 terminal timing control. */
    u32 pendingLipsStopCode; /* F117 until evtLipsStopFunction runs. */
    u32 skipLipsStopWait; /* Set when a timing control follows F117. */
    s32 remainingWaitFrames; /* F215 uses 0xFFFF for title-sound completion. */
    s32 contextModeEnabled;
};

#endif /* FONT_H */
