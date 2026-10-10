#ifndef MNU_MANTRA_GRID_H
#define MNU_MANTRA_GRID_H

#include "dat_state.h"

#ifndef VERSION_DDS2

/* Result bits shared by the mantra-entry flag producer and its draw/update readers. */
enum MnuMantraDisplayFlag {
    MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH = 0x01,
    MNU_MANTRA_DISPLAY_FLAG_AT_CAP = 0x02,
    MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_1 = 0x04,
    MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_2 = 0x08,
    MNU_MANTRA_DISPLAY_FLAG_REQUIREMENT_PAIR_CLEAR_FALLBACK = 0x10,
    MNU_MANTRA_DISPLAY_FLAG_REQUIREMENT_PAIR_SET_FALLBACK = 0x20,
};

typedef struct MnuMantraGridPulse {
    u16 timer;
    s16 duration;
    u32 mode;
} MnuMantraGridPulse;

/* Branch outcomes produced by the two profile checks and the profile-0x4E exception. */
enum MnuMantraGridEntryState {
    MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG = 1,
    MNU_MANTRA_GRID_ENTRY_FIRST_CHECK_OR_SPECIAL_PROFILE = 2,
    MNU_MANTRA_GRID_ENTRY_FIRST_PATH_REJECTED = 3,
};

/* The profile-selection grid allocates and passes this 0x58-byte entry to
 * the mantra display and pulse callbacks. */
typedef struct MnuMantraGridEntry {
    s32 frame;
    s32 value;
    s32 cap;
    u16 sceneId;
    u16 requiredLevel;
    u32 value05;
    s32 state;
    PrfSkillList skills;
    MnuMantraGridPulse pulse;
    s8 profileFlag;
    u8 pad55[3];
} MnuMantraGridEntry;

typedef char MnuMantraGridEntryLayoutAssert[
    (sizeof(MnuMantraGridPulse) == 8 &&
     sizeof(MnuMantraGridEntry) == 0x58 &&
     (u32)&((MnuMantraGridEntry *)0)->value == 4 &&
     (u32)&((MnuMantraGridEntry *)0)->cap == 8 &&
     (u32)&((MnuMantraGridEntry *)0)->sceneId == 0x0C &&
     (u32)&((MnuMantraGridEntry *)0)->requiredLevel == 0x0E &&
     (u32)&((MnuMantraGridEntry *)0)->value05 == 0x10 &&
     (u32)&((MnuMantraGridEntry *)0)->state == 0x14 &&
     (u32)&((MnuMantraGridEntry *)0)->skills == 0x18 &&
     (u32)&((MnuMantraGridEntry *)0)->pulse == 0x4C &&
     (u32)&((MnuMantraGridEntry *)0)->profileFlag == 0x54)
        ? 1 : -1];

typedef struct MantraPrerequisiteRecord {
    s32 kind;
    s16 x;
    s16 y;
    s8 ids[4];
    u8 flags[4];
} MantraPrerequisiteRecord;

extern MantraPrerequisiteRecord D_0036AE80[89];

typedef char MantraPrerequisiteRecordLayoutAssert[
    (sizeof(MantraPrerequisiteRecord) == 0x10 &&
     (u32)&((MantraPrerequisiteRecord *)0)->x == 0x04 &&
     (u32)&((MantraPrerequisiteRecord *)0)->y == 0x06 &&
     (u32)&((MantraPrerequisiteRecord *)0)->ids == 0x08 &&
     (u32)&((MantraPrerequisiteRecord *)0)->flags == 0x0C)
        ? 1 : -1];

#endif

#endif
