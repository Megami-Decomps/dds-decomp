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
EffTemplate *effCloneResourceTemplate(EffTemplate *src) {
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

typedef struct EffClonedBody {
    u32 w[13];
} EffClonedBody;

typedef struct EffCloned {
    EffClonedBody body; /* 0x00: copied from the source */
    u32 f34;            /* 0x34 */
    u32 f38;            /* 0x38 */
} EffCloned;

extern u32 func_00190370();
extern void func_001904B0();
extern void func_001904A8();

/* Clone an effect with its own sub-resource attached. */
EffCloned *effTrackPolyCreateWork(EffCloned *src) {
    EffCloned *dst = func_00328D68(sizeof(EffCloned));

    dst->body = src->body;
    dst->body.w[7] = 0xC;
    dst->f34 = 0;
    dst->f38 = func_00190370(dst->body.w[6], 0xC);
    func_001904B0(dst->f38, &src->body.w[9]);
    func_001904A8(dst->f38, src->body.w[8]);
    return dst;
}
