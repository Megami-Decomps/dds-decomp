#include "common.h"

extern s32 D_003BB140;
extern u32 D_003BB144;
extern u32 D_003BB148;

/* Work area for the event-effect helpers in this TU. */
typedef struct {
    u8   pad_0x00[0x04]; /* 0x00 */
    void *unk04;         /* 0x04: owner passed to func_00190238/func_00190328 */
    u8   unk08[0x28];    /* 0x08: init block handed to func_00190238 */
    u32  unk30;          /* 0x30 */
    void *unk34;         /* 0x34 */
    u8   pad_0x38[0x48]; /* 0x38 */
    u8   unk80;          /* 0x80 */
    u8   pad_0x81[0x03]; /* 0x81 */
    void *unk84;         /* 0x84: released by func_00190CD0 */
} EffEventWork; /* 0x88 */

void func_00190208(EffEventWork *work) {
    func_00160B00(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190238);

INCLUDE_ASM(const s32, "effect/effEvent", func_001902A0);

void func_00190308(EffEventWork *work) {
    func_00161588(work->unk34);
}

void func_00190320(EffEventWork *work, u32 value) {
    work->unk30 = value;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190328);

INCLUDE_ASM(const s32, "effect/effEvent", func_00190640);

INCLUDE_ASM(const s32, "effect/effEvent", func_00190708);

INCLUDE_ASM(const s32, "effect/effEvent", func_00190748);

void func_00190810(EffEventWork *work) {
    func_00190328(work->unk04);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190828);

void func_00190880(EffEventWork *work, void *value) {
    work->unk34 = value;
    func_00190238(work->unk04, work->unk08);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_001908A0);

INCLUDE_RODATA(const s32, "effect/effEvent", D_003A12E0);

INCLUDE_ASM(const s32, "effect/effEvent", func_00190AD8);

void func_00190CD0(EffEventWork *work) {
    D_003BB140 = D_003BB140 - 1;
    if (D_003BB140 == 0) {
        func_00151F00(D_003BB144);
        func_00151F00(D_003BB148);
    }
    func_002D0918(work->unk84);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190D18);

INCLUDE_ASM(const s32, "effect/effEvent", func_001910C8);

INCLUDE_ASM(const s32, "effect/effEvent", func_001914E0);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192018);

void func_00192028(EffEventWork *work, u8 value) {
    work->unk80 = value;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00192030);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192110);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192230);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192250);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192420);



INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB140);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB144);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB148);


INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB14C);

