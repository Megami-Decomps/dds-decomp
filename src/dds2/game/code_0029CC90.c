#include "common.h"

extern u32 func_0029D790();

extern u8 *D_00435DD0;

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

extern s32 btlAddBaseStats(u8 *, TitleSeq *);

extern u32 func_00309138(u32, u32, s32);

void itfUpdateFadeColor(u8 *work);

extern struct { s32 v[6]; } D_003D6500;

extern void mnuCampDrawMenuIconLayer(s32, s32, s32, u32, u8 *, s32, u8 *);

extern void func_0029CB70(s32, s32, s32, u32, u8 *, s32, u8 *);

extern void func_0029C880(s32, s32, s32, u32, u8 *, s32, u8 *);

void mnuTitleDrawFadeMenuEntries(u8 *work) {
    u8 *res = work + 0x5C;
    u32 color = func_00309138(0xFFF06480, 0xFFF06400, 0x100 - ((TitleMenuWork *)work)->fadeProgress);

    mnuCampDrawMenuIconLayer(D_003D6500.v[0], D_003D6500.v[1], 0, color, res, 0x53, work);
    func_0029CB70(D_003D6500.v[2], D_003D6500.v[3], 0, color, res, 0x53, work);
    func_0029C880(D_003D6500.v[4], D_003D6500.v[5], 0, color, res, 0x53, work);
}

void mnuTitleRenderFadeAndPanels(u8 *work) {
    s32 remaining = 0x100 - ((TitleMenuWork *)work)->fadeProgress;

    itfUpdateFadeColor(work);
    func_0029C618(work);
    func_0029C120(work);
    func_0029C3F0(work);
    func_0029DB58(0x1D0, 0x3B8, 0, remaining, work + 0x408, 0x53);
    mnuTitleDrawFadeMenuEntries(work);
}

void brsDecaySharedAnimCounter(void) {
    func_0029C810();
}

extern u8 D_003D9D58[];

/* Read the value paired with the highest of three thresholds not above input. */
u8 brsGetLevelStepForValue(s32 value) {
    s32 i;

    for (i = 2; i >= 0; i--) {
        if (value >= D_003D9D58[i * 2]) {
            return D_003D9D58[i * 2 + 1];
        }
    }
    return D_003D9D58[1];
}

u8 brsGetLevelStepCrossedBy(s32 position, s32 increment) {
    u8 *table = D_003D9D58;
    s32 i = 2;
    u8 *limit = table + 4;
    s32 nextPosition = position + increment;
    do {
        if (position < *limit && nextPosition >= *limit) {
            return limit[1];
        }
        limit -= 2;
    } while (--i >= 0);
    return 0;
}

u32 func_0029CE80(void) {
    return 1;
}

u32 func_0029CE88(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", ptyComputeTotalExp);

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029CF00);

#define BRS_ACTIVE_PARTY_FLAG 2
#define BRS_HALF_EXP_SKILL 0x23F
#define BRS_FULL_EXP_SKILL 0x240

/* Same party-unit reward header layout as DDS1, including AP status. */
typedef struct BrsExpUnit {
    u16 flags;          /* 0x00: bit 1 means active party member */
    u8 pad02[0xC];
    u16 apStatus;       /* 0x0E */
} BrsExpUnit;

/* Active party members take full EXP; benched members need the half/full
 * EXP skills (0x23F/0x240 respectively). The third caller arg is unused. */
s32 brsCalcExpGain(u8 *unit, s32 exp, s32 unused) {
    s32 result;

    if ((((BrsExpUnit *)unit)->flags & BRS_ACTIVE_PARTY_FLAG) != 0) {
        result = exp;
    } else {
        result = 0;
        if (ptyHasSkill(unit, BRS_HALF_EXP_SKILL) != 0) {
            result = exp / 2;
        }
        if (ptyHasSkill(unit, BRS_FULL_EXP_SKILL) != 0) {
            result = exp;
        }
    }
    return result;
}

u32 func_0029D000(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029D008);

INCLUDE_ASM(const s32, "game/code_0029CC90", ptyCalcLevelUps);

s32 mnuCountAdvancingTitleAnimations(void) {
    s32 offset = 0;
    s32 count = 0;
    s32 remaining = 4;
    do {
        s32 step = ptyCalcLevelUps(D_00435DD0 + 0xa60 + offset);
        count += step > 0;
        offset += 0x1c4;
    } while (--remaining >= 0);
    return count;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029D2D8);

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029D3D8);

s32 mnuAdvanceTitleEntryAnimation(u8 *entry) {
    s32 step = ptyCalcLevelUps(entry);
    *(u16 *)(entry + 0x14) += step;
    ptyRecomputeMaxHpMp(entry);
    return step;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", btlAddBaseStats);

INCLUDE_ASM(const s32, "game/code_0029CC90", ptyAccumulateStatGains);

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029D790);

void mnuTitleInitFourParameters(u32 *state, u32 first, u32 second, u32 third, u32 fourth) {
    memset(state, 0, 0x10);
    state[0] = first;
    state[1] = second;
    state[2] = third;
    state[3] = fourth;
}

extern s32 ptyCalcLevelUps(u8 *);

extern s32 ptyComputeTotalExp(u8 *, s32);

extern u32 func_00314728(u8 *, u32);

extern s32 func_00314C10(s32);

extern u32 func_00314690(u16);

/* Match the progress-row layout in DDS1 game/code_002653A0.c. */
typedef struct BrsUnitExp {
    u8 pad00[0x10];
    s32 totalExp;           /* 0x10 */
} BrsUnitExp;

typedef struct BrsProgressRow {
    u8 pad00[8];
    u32 unit;               /* 0x08 */
    u32 levelProgress[4];   /* 0x0C */
    u32 profileProgress[4]; /* 0x1C */
} BrsProgressRow;

/* Set up the level and profile progress bars for one party member. */
void brsBuildUnitProgressRow(u8 *state, u8 *entry) {
    s32 levelDelta;
    s32 profilePoints;

    memset(state, 0, 0x2C);
    ((BrsProgressRow *)state)->unit = (u32)entry;
    levelDelta = ptyCalcLevelUps(entry);
    mnuTitleInitFourParameters(((BrsProgressRow *)state)->levelProgress, 0x6E0, 0x50,
        ((BrsUnitExp *)entry)->totalExp - ptyComputeTotalExp(entry, levelDelta),
        ptyComputeTotalExp(entry, levelDelta + 1) - ptyComputeTotalExp(entry, levelDelta));
    profilePoints = func_00314728(entry, 0);
    mnuTitleInitFourParameters(((BrsProgressRow *)state)->profileProgress, 0x3C0, 0x50, profilePoints,
        func_00314690(func_00314C10((s32)entry) & 0xFFFF));
}

u32 func_0029DA58(u32 a, u32 b, u32 c, s32 blend, u8 *resource) {
    func_00314C10(*(u32 *)(resource + 8));
    return func_00309138(0x80808080, 0x80808000, blend);
}

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029DA98);

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029DB58);

INCLUDE_RODATA(const s32, "game/code_0029CC90", D_00428550);

