#ifndef DAT_STATE_H
#define DAT_STATE_H

#include "common.h"

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
    u8 pad20[2];
    u16 effectData[24];
    u16 menuValue;
    u8 pad54;
#ifdef VERSION_DDS1
    s8 profileId;
    u8 pad56[2];
    u32 skillFlags[76];                    /* 0x058: eight four-bit skill states per word. */
    u8 pad188[0xC];
    s32 randomizedValue;
    s32 link;
    u8 pad19C[8];
#endif
#ifdef VERSION_DDS2
    u8 profileId;
    u8 pad56[2];
    u32 skillFlags[85];                    /* 0x058: scrClearFlagsTable clears all 0x154 bytes. */
    u8 pad1AC[6];
    u16 itemId;
    s32 randomizedValue;                  /* 0x1B4: initialized to 0x12 minus a four-way roll. */
    s32 link;
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
    u16 flagBanks[5];
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
    u8 pad1294[8];
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
    u8 pad1334[8];
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
    DatProfileRecord profileRecords[16][176]; /* 0x17210 */
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
typedef char DatGameStateSizeCheck[sizeof(DatGameState) == 0x1E840 ? 1 : -1];
#endif

extern DatGameState *datGameState;

#endif /* DAT_STATE_H */
