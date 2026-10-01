#include "common.h"

INCLUDE_ASM(const s32, "game/code_0018F9E8", func_0018F9E8);

typedef struct EffTemplateBody {
    u32 words[9];
} EffTemplateBody;

typedef struct EffTemplate {
    EffTemplateBody body;  /* 0x00: copied from the source template */
    u32 resourceWord;      /* 0x24 */
} EffTemplate;

extern void *func_00328D68(s32 size);
extern u32 effGetResourceFirstWord(s32 index);

/* Clone an effect template into a fresh allocation. */
EffTemplate *func_0018FBF8(EffTemplate *src) {
    EffTemplate *dst = func_00328D68(sizeof(EffTemplate));

    dst->resourceWord = effGetResourceFirstWord(0);
    dst->body = src->body;
    return dst;
}

void func_0018FC88(void) {
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_0018F9E8", func_0018FCA0);

INCLUDE_ASM(const s32, "game/code_0018F9E8", func_0018FD10);

INCLUDE_ASM(const s32, "game/code_0018F9E8", func_0018FD88);
