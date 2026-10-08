#include "common.h"
#include "mnu_result.h"

extern const char *D_003D62F0[6];
extern char (*D_00435E48)[17];
extern char (*D_00435E5C)[25];
struct EffRandState;
extern u32 effMiscRand(struct EffRandState *state);
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 mnuSelectEventFlagCode(void);
extern s32 dspStartEntry(s32);
extern void evtStageTestQueueMotion(s32, u32);
extern void evtStageTestSetPendingEffect(u32);

void func_0029BC58(BrsSkillPackageWork *work) {
    DatPartyRecord *unit = work->selectedRewardRow->unit;
    s32 mode = work->rewardMode;
    s32 rewardIndex;

    switch (mode) {
    case 4: {
        rewardIndex = work->rewardIndex;
        work->statGains[rewardIndex]++;
        evtCopyEntryStringToActiveWindow(1, (s32)D_003D62F0[rewardIndex]);
    }
        /* Fall through to display the rewarded unit. */
    case 1:
    case 2:
    case 3:
        evtCopyEntryStringToActiveWindow(0, (s32)D_00435E48[unit->unitId]);
        dspStartEntry(work->rewardMode + 25);
        break;
    case 5: {
        s32 roll;
        rewardIndex = unit->unitId * 4 - 4;
        roll = effMiscRand(NULL) & 3;

        evtCopyEntryStringToActiveWindow(0, (s32)D_00435E48[unit->unitId]);
        evtCopyEntryStringToActiveWindow(1, (s32)D_00435E5C[work->earnedItem]);
        switch (mnuSelectEventFlagCode()) {
        case 1:
            dspStartEntry(rewardIndex + roll + 30);
            break;
        case 5:
            dspStartEntry(rewardIndex + roll + 62);
            break;
        case 2:
            dspStartEntry(rewardIndex + roll + 94);
            break;
        case 8:
            dspStartEntry(rewardIndex + roll + 126);
            break;
        }
        break;
    }
    }

    switch (work->rewardMode) {
    case 1:
    case 2:
    case 3:
        evtStageTestQueueMotion(2, 1);
        evtStageTestSetPendingEffect(0);
        return;
    case 4:
        evtStageTestQueueMotion(2, 1);
        return;
    case 5:
        evtStageTestQueueMotion(2, 2);
        break;
    }
}


extern void mnuRefreshSelectedUnitPanels(DatPartyRecord *, BrsSkillPackageWork *);

extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);

void mnuTitleApplySequenceState(BrsSkillPackageWork *work) {
    s32 state = work->rewardMode;
    DatPartyRecord *seq = work->selectedRewardRow->unit;

    switch (state) {
    case 5:
        break;
    case 4:
        btlAddBaseStats(work->statGains, seq);
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 1:
        seq->hp = seq->maxHp;
        seq->mp = seq->maxMp;
        seq->status = 0;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 2:
        seq->hp = seq->maxHp;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 3:
        seq->mp = seq->maxMp;
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

INCLUDE_SDATA(const s32, "game/code_0029BC58", mnuTitleSoundTask);

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

INCLUDE_SDATA(const s32, "game/code_0029BC58", mnuMovieMenuState);

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

INCLUDE_SDATA(const s32, "game/code_0029BC58", mnuMovieWork);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", mnuMovieDrawTask);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437ADC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE4);

