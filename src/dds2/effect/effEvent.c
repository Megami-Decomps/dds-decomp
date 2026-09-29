#include "common.h"
#include "pcp_vu0.h"

extern s32 D_00436530;

extern u32 D_00436534;

extern u32 D_00436538;

typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

/* Event work shared by resource setup, teardown and state updates. */
typedef struct EffEventWork {
    u8 pad00[4];
    u32 owner;              /* 0x04 */
    u8 initBlock[0x28];    /* 0x08: file-record header source */
    u32 state;              /* 0x30 */
    u32 effect;             /* 0x34 */
    u8 pad38[0x48];
    u8 flag;                /* 0x80 */
    u8 pad81[3];
    u32 resource;           /* 0x84 */
} EffEventWork;

extern void func_00198710();

extern u8 D_003B2D20[];

/* Release the attached effect before freeing the event work. */
void effEventReleaseNode(EffEventWork *work) {
    func_001686F0(work->effect);
    func_00328E48(work);
}

void effEventCopyFileRecordHeader(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00197ED8);

void func_00197F40(EffEventWork *work) {
    func_00169168(work->effect);
}

void effEventSetState(EffEventWork *work, u32 state) {
    work->state = state;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00197F60);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198278);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198340);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198380);

void func_00198448(EffEventWork *work) {
    func_00197F60(work->owner);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00198460);

/* Attach the effect and copy the initial file-record header to its owner. */
void effEventBindEffect(EffEventWork *work, u32 effect) {
    work->effect = effect;
    effEventCopyFileRecordHeader(work->owner, work->initBlock);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_001984D8);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00414A00);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198710);

void effEventReleaseSharedResources(EffEventWork *work) {
    D_00436530 = D_00436530 - 1;
    if (D_00436530 == 0) {
        billDispatchByKind(D_00436534);
        billDispatchByKind(D_00436538);
    }
    func_003297C8(work->resource);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00198950);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198D00);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199118);

void func_00199C50(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00199C60(EffEventWork *work, u8 flag) {
    work->flag = flag;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00199C68);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199D48);

void func_00199E68(void) {
    func_00198710(D_003B2D20);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00199E88);

INCLUDE_ASM(const s32, "effect/effEvent", func_0019A058);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436530);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436534);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436538);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043653C);

