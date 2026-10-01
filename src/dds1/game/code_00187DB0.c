#include "common.h"

INCLUDE_ASM(const s32, "game/code_00187DB0", func_00187DB0);

typedef struct EffTemplateBody {
    u32 words[9];
} EffTemplateBody;

typedef struct EffTemplate {
    EffTemplateBody body;  /* 0x00: copied from the source template */
    u32 resourceWord;      /* 0x24 */
} EffTemplate;

extern void *func_002CFEB8(s32 size);
extern u32 effGetResourceFirstWord(s32 index);

/* Clone an effect template into a fresh allocation. */
EffTemplate *effCloneResourceTemplate(EffTemplate *src) {
    EffTemplate *dst = func_002CFEB8(sizeof(EffTemplate));

    dst->resourceWord = effGetResourceFirstWord(0);
    dst->body = src->body;
    return dst;
}

void func_00188050(void) {
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_00187DB0", func_00188068);

extern void func_00187DB0(void *arg, s32 value, s32 flag);

typedef struct BlurRectWork {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    u8 unk0C[4];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} BlurRectWork;

void func_001880D8(BlurRectWork *w) {
    f32 sc = (f32)w->unk00 * 1.4f;
    s32 a = w->unk04 + 0x1000;
    s32 b = (w->unk08 + 0xE00) >> 1;
    s32 c = (s32)sc;
    s32 d = a + c;
    s32 e;

    a -= c;
    c >>= 1;
    w->unk14 = a;
    e = b + c;
    b -= c;
    w->unk1C = d;
    w->unk18 = b;
    w->unk20 = e;
    func_00187DB0(w->unk0C, w->unk24, 1);
}

typedef struct EffClonedBody {
    u32 w[13];
} EffClonedBody;

typedef struct EffCloned {
    EffClonedBody body; /* 0x00: copied from the source */
    u32 f34;            /* 0x34 */
    u32 f38;            /* 0x38 */
} EffCloned;

extern u32 func_00188738();
extern void func_00188878();
extern void func_00188870();

/* Clone an effect with its own sub-resource attached. */
EffCloned *effTrackPolyCreateWork(EffCloned *src) {
    EffCloned *dst = func_002CFEB8(sizeof(EffCloned));

    dst->body = src->body;
    dst->body.w[7] = 0xC;
    dst->f34 = 0;
    dst->f38 = func_00188738(dst->body.w[6], 0xC);
    func_00188878(dst->f38, &src->body.w[9]);
    func_00188870(dst->f38, src->body.w[8]);
    return dst;
}
