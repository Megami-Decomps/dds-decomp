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

INCLUDE_ASM(const s32, "game/code_00187DB0", func_001880D8);

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
EffCloned *func_00188150(EffCloned *src) {
    EffCloned *dst = func_002CFEB8(sizeof(EffCloned));

    dst->body = src->body;
    dst->body.w[7] = 0xC;
    dst->f34 = 0;
    dst->f38 = func_00188738(dst->body.w[6], 0xC);
    func_00188878(dst->f38, &src->body.w[9]);
    func_00188870(dst->f38, src->body.w[8]);
    return dst;
}
