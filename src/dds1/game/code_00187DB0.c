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
EffTemplate *func_00187FC0(EffTemplate *src) {
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

INCLUDE_ASM(const s32, "game/code_00187DB0", func_00188150);
