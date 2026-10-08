#ifndef DAT_STATE_H
#define DAT_STATE_H

#include "common.h"
#include "dat_affinity.h"

/* The first 0x30 bytes also form compact save metadata; GBWK retains the
 * runtime scene handle at +0x30. */
typedef struct DatStateHeader {
    u8 magic[3];
    u8 version;
    s8 mapGroup;
    s8 mapIndex;
    u8 pad06[2];
    s32 playTicks;
    s16 unk0C;
    s16 transition;
    s8 partyIds[8];
    s8 partyLevels[8];
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    s32 backingAllocation;
    u32 firstTick;
    u32 secondTick;
    s32 currency;
} DatStateHeader;

/* Shared script storage begins at +0x40; +0x388 is integer register 210. */
typedef struct DatScriptGlobals {
    s32 ints[256];
    f32 floats[256];
} DatScriptGlobals;

typedef struct DatModelFlags {
    u32 words[128];
} DatModelFlags;

/* Solar state and the shared field/command flags occupy +0xA40..+0xA5F. */
typedef struct DatWorldState {
    u8 flags;
    u8 phase;
    u8 secondaryPhase;
    u8 overlayFlag;
    f32 phaseTimer;
    u32 unkA48;
    u32 unkA4C;
    s32 score;
    u32 slotFlags;
    u32 fieldFlags;
    u32 updateMode;
} DatWorldState;

/* The active roster and saved template bank share this complete record type. */
typedef struct DatPartyRecord {
    u16 flags;
    u16 affinityTableIndex;
    u16 unitId;
    u16 hp;
    u16 maxHp;
    u16 mp;
    u16 maxMp;
    u16 status;
    u32 totalExp;
    u16 level;
    s8 baseStats[5];
    u8 pad1B;
    u16 hpBonus;
    u16 mpBonus;
    u8 unk20; /* Packed nonzero effectData count written by the enemy constructor. */
    u8 pad21;
    u16 effectData[24];
    u16 menuValue;
    u8 pad54;
#ifdef VERSION_DDS1
    s8 profileId;
    u8 pad56[2];
    union {
        struct {
            u32 skillFlags[76]; /* Eight four-bit states per word. */
            u8 pad188[4];
        };
        u32 skillSnapshotWords[77]; /* Stock merge includes the reserved trailing word. */
    };
    u16 unk18C;
    u16 unk18E;
    u16 unk190;
    u8 pad192[2];
    s32 randomizedValue;
    s32 huntExp; /* 0x198: hunt EP accumulated this battle (shared by ability 0x24C) */
    u8 pad19C[8];
#endif
#ifdef VERSION_DDS2
    u8 profileId;
    u8 pad56[2];
    u32 skillFlags[85];                    /* 0x058: scrClearFlagsTable clears all 0x154 bytes. */
    u16 unk1AC; /* Synced from the active battle record. */
    u16 unk1AE; /* Synced from the active battle record. */
    s16 actionSlot;
    u16 itemId;
    s32 randomizedValue;                  /* 0x1B4: initialized to 0x12 minus a four-way roll. */
    s32 huntExp; /* 0x1B8: hunt EP accumulated this battle */
    u8 pad1BC[8];
#endif
} DatPartyRecord;

typedef struct DatInventory {
#ifdef VERSION_DDS1
    u8 counts[192];
#endif
#ifdef VERSION_DDS2
    u8 counts[256];
#endif
} DatInventory;

/* The field reset constructors prove the 30-byte slot and 64-slot bank. */
typedef struct DatFieldMapSlot {
    union {
        u16 flagBanks[5];
        struct {
            u16 roomModeFlags;
            u16 roomObjectModeFlags;
            u16 roomSceneFlags;
            u16 mapTargetFlags;
            u16 alternateMapTargetFlags;
        };
    };
    u8 values[16];
    u16 trailingFlagBanks[2];
} DatFieldMapSlot;

typedef struct DatFieldMapBank {
    DatFieldMapSlot slots[64];
} DatFieldMapBank;

/* The fifteenth reset word is the first word of the following render region. */
typedef struct DatVrHeader {
    s16 previewX;
    s16 previewY;
    u8 pad04[4];
    u32 words[14];
} DatVrHeader;

typedef struct DatProfileRecord {
    u32 value;
    u32 flags;
} DatProfileRecord;

#ifdef VERSION_DDS2
/* Each unit owns one complete 0x580-byte bank of 176 profile records. */
typedef struct DatProfileBank {
    DatProfileRecord records[176];
} DatProfileBank;
typedef char DatProfileBankSizeCheck[sizeof(DatProfileBank) == 0x580 ? 1 : -1];
#endif

/* Eight-skill output shared by profile builders and reward renderers. */
typedef struct PrfSkillList {
    u32 flags[8];
    s32 count;
    u16 skills[8];
} PrfSkillList;

/* Global flag APIs use 32-bit words in each unit's bitmap. */
typedef struct DatMantraBitmap {
#ifdef VERSION_DDS1
    u32 words[7];
#endif
#ifdef VERSION_DDS2
    u32 words[12];
#endif
} DatMantraBitmap;

typedef struct DatGameState {
    DatStateHeader header;                 /* 0x00000 */
    DatScriptGlobals script;               /* 0x00040 */
    DatModelFlags modelFlags;              /* 0x00840 */
    DatWorldState world;                   /* 0x00A40 */
    DatPartyRecord party[5];               /* 0x00A60 */
#ifdef VERSION_DDS1
    u8 partyOrder[5];                      /* 0x01294: ordered indices into party[]. */
    u8 pad1299[3];
    s32 partyCount;                        /* 0x0129C */
    DatInventory inventory;               /* 0x012A0 */
    s32 unk1360;
    u16 unk1364; /* read with lhu by the battle test menu (func_00215FF8) */
    u8 pad1366[0xA];
    DatFieldMapBank maps[40];              /* 0x01370 */
    u64 areaFlags[13][64];                 /* 0x13F70 */
    u8 activationFlags[32];                /* 0x15970 */
    DatVrHeader vr;                        /* 0x15990 */
    u8 pad159D0[0x19000];
    u32 scriptFlags[3];                    /* 0x2E9D0 */
    u32 battleFlags[3];                    /* 0x2E9DC */
    u8 pad2E9E8[8];
    DatMantraBitmap mantraBits[16];        /* 0x2E9F0 */
    DatProfileRecord profileRecords[16][96]; /* 0x2EBB0 */
    DatPartyRecord templates[16];          /* 0x31BB0: the snapshot copies sixteen records. */
    u8 pad335F0[0x10];
#endif
#ifdef VERSION_DDS2
    u8 partyOrder[5];                      /* 0x01334: ordered indices into party[]. */
    u8 pad1339[3];
    s32 partyCount;                        /* 0x0133C */
    DatInventory inventory;               /* 0x01340 */
    s32 unk1440;
    s16 unk1444;
    u8 pad1446[0xA];
    DatFieldMapBank maps[31];              /* 0x01450 */
    u64 areaFlags[10][64];                 /* 0x0FCD0 */
    u8 activationFlags[32];                /* 0x110D0 */
    DatVrHeader vr;                        /* 0x110F0 */
    u8 pad11130[0x5DC0];
    u32 scriptFlags[4];                    /* 0x16EF0 */
    u32 battleFlags[4];                    /* 0x16F00 */
    DatMantraBitmap mantraBits[16];        /* 0x16F10 */
    DatProfileBank profileBanks[16]; /* 0x17210 */
    DatPartyRecord templates[16];          /* 0x1CA10: the initializer clears sixteen records. */
    u32 savedCurrency;                    /* 0x1E650 */
    s32 progressTotal;                    /* 0x1E654 */
    s32 progressSlot;                     /* 0x1E658 */
    u8 pad1E65C[4];
    u32 highScore;                        /* 0x1E660 */
    u8 pad1E664[0xC];
    u8 itemStatBonuses[64][5];             /* 0x1E670; item IDs 0xC0..0xFF */
    u8 itemRequirementCounts[64];          /* 0x1E7B0 */
    u8 itemBlockedFlags[64];               /* 0x1E7F0 */
    u8 pad1E830[0x10];
#endif
} DatGameState;

typedef char DatStateHeaderSizeCheck[sizeof(DatStateHeader) == 0x40 ? 1 : -1];
typedef char DatFieldMapBankSizeCheck[sizeof(DatFieldMapBank) == 0x780 ? 1 : -1];
#ifdef VERSION_DDS1
typedef char DatPartyRecordSizeCheck[sizeof(DatPartyRecord) == 0x1A4 ? 1 : -1];
typedef char DatGameStateSizeCheck[sizeof(DatGameState) == 0x33600 ? 1 : -1];
#endif
#ifdef VERSION_DDS2
typedef char DatPartyRecordSizeCheck[sizeof(DatPartyRecord) == 0x1C4 ? 1 : -1];
typedef char DatPartyRecordActionSlotOffsetCheck[((u32)&((DatPartyRecord *)0)->actionSlot == 0x1B0) ? 1 : -1];
typedef char DatPartyRecordItemIdOffsetCheck[((u32)&((DatPartyRecord *)0)->itemId == 0x1B2) ? 1 : -1];
typedef char DatGameStateSizeCheck[sizeof(DatGameState) == 0x1E840 ? 1 : -1];
#endif

/* The 0x4C-byte enemy table supplies initial party vitals/stats, skills and
 * rewards (DDS1 001A1990, DDS2 001AA898 copy the leading values). */
typedef struct DatEnemyRecord {
    u32 flags;            /* 0x00 */
    u8 pad04;
    u8 level;             /* 0x05 */
    u16 hp;               /* 0x06 */
    u16 maxHp;            /* 0x08 */
    u16 mp;               /* 0x0A */
    u16 maxMp;            /* 0x0C */
    u8 pad0E[2];
    u8 baseStats[5];      /* 0x10 */
    u8 unk15;            /* Copied to the command actor's actionNumber on model change. */
    u8 pad16[2];
    u16 skills[8];        /* 0x18 */
    s32 money;            /* 0x28 */
    u16 unk2C;
    u16 experience;       /* 0x2E */
    u16 huntExperience;   /* 0x30 */
    u8 pad32[4];
    u16 huntPenaltyFlags; /* 0x36: hunt's adverse-outcome mask. */
    u16 huntPenaltyChance; /* 0x38: base percentage before status modifiers. */
    u8 pad3A[4];
    u8 unk3E[2];          /* The battle status accessor tests both adjacent bytes. */
    u8 actionChances[2];  /* 0x40: bucket thresholds for the two unk3E actions. */
    u16 overrideFlag;     /* 0x42: model flag gating the override action. */
    u8 overrideAction;    /* 0x44: action selected before the ordinary slots. */
    u8 overrideChance;    /* 0x45: bucket threshold for the override action. */
    s8 unk46;            /* Signed indexed-value accessor. */
    u8 pad47;
    u8 unk48;            /* Enemy display-byte accessor. */
    u8 pad49[3];
} DatEnemyRecord;

typedef char DatEnemyRecordSizeCheck[sizeof(DatEnemyRecord) == 0x4C ? 1 : -1];
typedef char DatEnemyActionChancesOffsetCheck[((u32)&((DatEnemyRecord *)0)->actionChances == 0x40) ? 1 : -1];
typedef char DatEnemyOverrideFlagOffsetCheck[((u32)&((DatEnemyRecord *)0)->overrideFlag == 0x42) ? 1 : -1];
typedef char DatEnemyOverrideActionOffsetCheck[((u32)&((DatEnemyRecord *)0)->overrideAction == 0x44) ? 1 : -1];
typedef char DatEnemyOverrideChanceOffsetCheck[((u32)&((DatEnemyRecord *)0)->overrideChance == 0x45) ? 1 : -1];

/* One 0x28-byte battle scene record; datBattleSceneRecords points at the 0x400-entry table. */
typedef struct DatBattleSceneRecord {
    s8 unk00;            /* 0x00: tested for nonzero (scene color/mode selection) */
    u8 unk01;            /* 0x01: id handed to the scene-entry loader, unk02 times */
    u8 unk02;            /* 0x02 */
    u8 pad03;
    u16 serialScene;     /* 0x04: forced follow-up encounter, zero selects the weighted policy. */
    u16 unitModes[11];   /* 0x06: enemy unit modes; zero marks an empty slot */
    u16 unk1C;           /* 0x1C: copied into the battle state with unk1E when both are set */
    u16 unk1E;           /* 0x1E */
    u16 flags;           /* 0x20: 0x8000, 0x800 and 0x400 are tested */
    u8 pad22[2];
    u16 unk24;           /* 0x24: overrides the scene's sound selection when nonzero */
    u16 eventId;         /* 0x26: signed event number; zero disables */
} DatBattleSceneRecord;

typedef char DatBattleSceneRecordSizeCheck[sizeof(DatBattleSceneRecord) == 0x28 ? 1 : -1];

#ifdef VERSION_DDS1
/* One 0x20C-byte encounter adjustment record. Sound selection shares the
 * header with condition/variant data; four complete weighted groups follow. */
typedef struct BattleAdjustmentEntry {
    u16 sceneIndex;
    u16 weight;
    s8 value;
    u8 unk05;
} BattleAdjustmentEntry;

typedef struct BattleAdjustmentGroup {
    s32 interval;
    BattleAdjustmentEntry entries[20];
} BattleAdjustmentGroup;

typedef struct BattleAdjustmentRecord {
    u8 pad00[4];
    u16 streamSelection; /* 0x04: default stream selector used by 001F3278. */
    u8 pad06[2];
    u32 conditions[3];
    u8 variantCodes[8];
    BattleAdjustmentGroup groups[4];
} BattleAdjustmentRecord;

typedef char BattleAdjustmentEntrySizeCheck[sizeof(BattleAdjustmentEntry) == 6 ? 1 : -1];
typedef char BattleAdjustmentGroupSizeCheck[sizeof(BattleAdjustmentGroup) == 0x7C ? 1 : -1];
typedef char BattleAdjustmentRecordSizeCheck[sizeof(BattleAdjustmentRecord) == 0x20C ? 1 : -1];
typedef char BattleAdjustmentStreamOffsetCheck[(u32)&((BattleAdjustmentRecord *)0)->streamSelection == 4 ? 1 : -1];
typedef char BattleAdjustmentGroupsOffsetCheck[(u32)&((BattleAdjustmentRecord *)0)->groups == 0x1C ? 1 : -1];

extern BattleAdjustmentRecord *D_003BAA3C;
#endif

extern DatBattleSceneRecord *datBattleSceneRecords;

extern DatGameState *datGameState;

#endif /* DAT_STATE_H */
