#include "common.h"

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029BC58);

typedef struct {
    u8 pad0[6];
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
} TitleSeq;

typedef struct TitleMenuWork {
    u8 pad00[0x9C];
    TitleSeq **sequence;      /* 0x009C */
    u8 padA0[0xAE08];
    s8 opacityReady;          /* 0xAEA8 */
    u8 padAEA9[7];
    s32 iconResource;         /* 0xAEB0 */
    u32 opacity;              /* 0xAEB4 */
    u8 padAEB8[0x828];
    s32 fadeProgress;         /* 0xB6E0 */
    u8 padB6E4[0x10];
    s32 sequenceMode;         /* 0xB6F4 */
} TitleMenuWork;

extern void mnuRefreshSelectedUnitPanels(TitleSeq *, u8 *);

extern s32 btlAddBaseStats(u8 *, TitleSeq *);

void mnuTitleApplySequenceState(u8 *work) {
    s32 state = ((TitleMenuWork *)work)->sequenceMode;
    TitleSeq *seq = *((TitleMenuWork *)work)->sequence;

    switch (state) {
    case 5:
        break;
    case 4:
        btlAddBaseStats(work + 0x3F4, seq);
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 1:
        seq->unk6 = seq->unk8;
        seq->unkA = seq->unkC;
        seq->unkE = 0;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 2:
        seq->unk6 = seq->unk8;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 3:
        seq->unkA = seq->unkC;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    }
    mnuRefreshSelectedUnitPanels(seq, work);
}

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379C0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379C8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379D0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379D8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379E0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379E8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379F0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379F8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379FC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A00);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A08);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A10);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A18);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A1C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A20);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A28);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A2C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A30);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A34);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A38);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A3C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A40);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A48);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A50);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A58);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A60);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A68);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A70);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A78);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A80);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A88);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A90);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A98);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AA0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AA8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437ACC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437ADC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE4);

