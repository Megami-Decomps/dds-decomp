#ifndef MNU_MANTRA_GRID_ENTRY_STATE_H
#define MNU_MANTRA_GRID_ENTRY_STATE_H

/* Grid snapshots keep absent slots at zero and copy these entry outcomes. */
typedef enum MnuMantraGridEntryState {
    MNU_MANTRA_GRID_ENTRY_ABSENT = 0,
    MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG = 1,
    MNU_MANTRA_GRID_ENTRY_FIRST_CHECK_OR_SPECIAL_PROFILE = 2,
    MNU_MANTRA_GRID_ENTRY_FIRST_PATH_REJECTED = 3,
} MnuMantraGridEntryState;

#endif /* MNU_MANTRA_GRID_ENTRY_STATE_H */
