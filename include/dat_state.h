#ifndef DAT_STATE_H
#define DAT_STATE_H

#include "common.h"

/* GBWK allocates this state, retaining its scene handle at +0x30. */
typedef struct DatStateHeader {
    u8 magic[3];
    u8 version;
    u8 pad04[4];
    u32 unk08;
    s16 unk0C;
    s16 transition;
    u8 name10[8];
    u8 name18[8];
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

/* Five active records; the same record type is used by the eight templates. */
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
    u8 profileId;
#ifdef VERSION_DDS1
    u8 pad56[0x13E];
    s32 randomizedValue;
    s32 link;
    u8 pad19C[8];
#endif
#ifdef VERSION_DDS2
    u8 pad56[0x15C];
    u16 itemId;
    u8 pad1B4[4];
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
    s16 unk1364;
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
    DatPartyRecord templates[8];           /* 0x31BB0 */
    u8 pad328D0[0xD30];
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
    DatPartyRecord templates[8];           /* 0x1CA10 */
    u8 pad1D830[0xE20];
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
