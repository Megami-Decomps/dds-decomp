#include "common.h"

/* Small work area: indirect table plus an object released on cleanup. */
typedef struct {
    u8   pad_0x00[0x20]; /* 0x00 */
    u32  *unk20;         /* 0x20: table written by func_00189B90 */
    u8   pad_0x24[0x10]; /* 0x24 */
    void *unk34;         /* 0x34: released by func_001893C0 */
} EffMagatuhiWork; /* 0x38 */

/* Mid-size variant holding the pairs freed by func_0018B1D0/func_0018C2A8. */
typedef struct {
    u8   pad_0x000[0x120]; /* 0x000 */
    void *unk120;          /* 0x120 */
    void *unk124;          /* 0x124 */
    void *unk128;          /* 0x128 */
    void *unk12C;          /* 0x12C */
} EffMagatuhiMidWork; /* 0x130 */

/* Large variant holding the triple freed by func_00189E60/func_0018A800. */
typedef struct {
    u8   pad_0x000[0x180]; /* 0x000 */
    void *unk180;          /* 0x180 */
    void *unk184;          /* 0x184 */
    void *unk188;          /* 0x188 */
    void *unk18C;          /* 0x18C */
    void *unk190;          /* 0x190 */
} EffMagatuhiBigWork; /* 0x194 */

void func_001893C0(EffMagatuhiWork *work) {
    func_002D0918(work->unk34);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001893D8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189818);

void func_00189B90(EffMagatuhiWork *work, s32 index, u32 value) {
    work->unk20[index] = value;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189BA8);

void func_00189C80(void) {
    func_001891C0();
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189C98);

void func_00189E60(EffMagatuhiBigWork *work) {
    func_0018DF90(work->unk180);
    func_00189178(work->unk18C);
    func_002D0918(work->unk190);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189E98);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A098);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A610);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A678);

void func_0018A800(EffMagatuhiBigWork *work) {
    func_0018DF90(work->unk184);
    func_00189178(work->unk188);
    func_002D0918(work->unk18C);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A838);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AB40);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AD58);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018ADD8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AFE8);

void func_0018B1D0(EffMagatuhiMidWork *work) {
    func_00189178(work->unk120);
    func_002D0918(work->unk128);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B200);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B348);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B600);

void func_0018B618(EffMagatuhiMidWork *work, void *value) {
    work->unk124 = value;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B620);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B648);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B850);

void func_0018BA00(EffMagatuhiMidWork *work) {
    func_00189178(work->unk124);
    func_002D0918(work->unk12C);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BA30);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BB88);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BE50);

void func_0018BE68(EffMagatuhiMidWork *work, void *value) {
    work->unk128 = value;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BE70);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BE98);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C0C0);

void func_0018C2A8(EffMagatuhiMidWork *work) {
    func_00189178(work->unk124);
    func_002D0918(work->unk128);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C2D8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C4C8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C7A8);

void func_0018C7C0(EffMagatuhiMidWork *work, void *value) {
    work->unk120 = value;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C7C8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C7F0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018CA30);
