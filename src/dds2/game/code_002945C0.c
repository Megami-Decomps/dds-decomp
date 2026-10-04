#include "common.h"

extern u32 D_00435E70;
extern u64 itfDrawBankTextWithLayoutFlags(u64, u64, u64, u16, u32, u64);

typedef struct ItfGlyphInfo {
    u32 unk0;
    u32 code;
    u32 unk8;
    s32 kind;
} ItfGlyphInfo;

typedef struct ItfGlyphData {
    u8 pad00[0x60];
    ItfGlyphInfo info;
} ItfGlyphData;

typedef struct ItfGlyphEntry {
    u8 pad00[0x1C];
    ItfGlyphData *data;
    s32 active;
} ItfGlyphEntry;

typedef struct ItfGlyphList {
    u8 pad00[0x18];
    ItfGlyphEntry *selected;
} ItfGlyphList;

typedef struct ItfGlyphDisplayContext {
    u8 pad00[0x80];
    ItfGlyphList *glyphList;
} ItfGlyphDisplayContext;

void func_002945C0(ItfGlyphDisplayContext *context, u64 unused, u64 parentGlyph,
                  u64 color, u64 glyphAttribute) {
    ItfGlyphEntry *entry;
    ItfGlyphInfo *data;
    u64 glyph;
    s32 y;
    u32 code;

    entry = context->glyphList->selected;
    if (entry->active != 0) {
        data = &entry->data->info;
        code = data->code;
        if (data->kind == 1 || data->kind == 3) {
            y = 0xBD0;
        } else {
            y = 0xB08;
        }
        glyph = itfDrawBankTextWithLayoutFlags(0x600, y, 1, code, D_00435E70, parentGlyph);
        frFontSetChildColors(glyph, color);
        func_0019D550(glyph, 1, glyphAttribute);
        frFontQueueGlyphInSelectedSlot(glyph);
        return;
    }
}

extern void func_00306CD0(s32, s32, s32, s32, s32, void *, s32, s32);

typedef struct BlendDispatchWork {
    u8 pad00[8];
    u32 mode;      /* 0x08: must be 2 */
    u8 pad0C[0x64];
    void *first;   /* 0x70 */
    void *second;  /* 0x74 */
} BlendDispatchWork;

void func_00294680(BlendDispatchWork *w, s32 a1, s32 a2) {
    void *p;

    if (w->mode != 2) {
        return;
    }
    func_00306CD0(0xE30, 0x610, 0, a1, 0, w->first, 1, a2);
    p = *(void **)((u8 *)w->second + 0x18);
    *(f32 *)((u8 *)p + 0xC4) = 90.0f;
    func_00306CD0(0x9F0, 0x610, 0, a1, 2, w->second, 1, a2);
}
