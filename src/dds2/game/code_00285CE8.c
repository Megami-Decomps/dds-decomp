#include "sdk_ps2.h"
#include "common.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "mdl_flag_api.h"


extern void evtPrintDeveloperConsoleMessage();




extern void mnuClearCampResourceFlagEntries(void);

extern void mnuSynchronizeMantraModelFlags(s32 mode);

extern void mtrRecomputeMantraFlagsForPartyUnit(DatPartyRecord *record);

extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);




typedef struct MtrCell {
    s16 x;
    s16 y;
    s16 remainingFrames;
    s16 durationFrames;
    s8 direction;
    s8 state;
    u8 remainingDepth;
    u8 unk0B;
} MtrCell;

typedef char MtrCellRemainingFramesOffsetCheck[
    ((u32)&((MtrCell *)0)->remainingFrames == 0x04) ? 1 : -1];
typedef char MtrCellDurationFramesOffsetCheck[
    ((u32)&((MtrCell *)0)->durationFrames == 0x06) ? 1 : -1];
typedef char MtrCellSizeCheck[sizeof(MtrCell) == 0x0C ? 1 : -1];

/* Native cell storage and callback; geometry is shared with the draw callback. */
typedef struct MtrGrid {
    u8 pad00[4];
    MtrCell *cells;
    s32 count;
    s16 originX;
    s16 originY;
    s16 width;
    s16 height;
    s16 unk14;
    s16 unk16;
    s16 remainingDelay;
    s16 delayRange;
    s16 cellWidth;
    s16 cellHeight;
    s16 depthStep;
    u8 initialDepth;
    s8 axisSwap;
    void (*draw)(s32, s32, s32, s32, struct MtrGrid *, s32, s32);
} MtrGrid;

typedef char MtrGrid_cellWidth_offset_must_be_0x1C[
    ((u32)&((MtrGrid *)0)->cellWidth == 0x1C) ? 1 : -1];
typedef char MtrGrid_cellHeight_offset_must_be_0x1E[
    ((u32)&((MtrGrid *)0)->cellHeight == 0x1E) ? 1 : -1];

extern void mtrSpawnGridPropagationCells(MtrGrid *grid, MtrCell *source, s8 mode);
extern void mtrAdvanceGridCellPropagation(MtrGrid *grid);

void mtrUpdateGridPropagation(MtrGrid *grid) {
    if (grid->remainingDelay == 0) {
        if (grid->unk14 < grid->unk16) {
            mtrSpawnGridPropagationCells(grid, NULL, 1);
        }
        grid->remainingDelay = grid->delayRange * sdfGetRandFloat(0) + 1.0f;
    } else {
        grid->remainingDelay--;
    }
    mtrAdvanceGridCellPropagation(grid);
}

void mtrDrawGridPropagationCells(s32 x, s32 y, s32 z, s32 alpha, MtrGrid *grid, s32 flags, s32 context) {
    MtrCell *cell = grid->cells;
    s32 i;

    for (i = 0; i < grid->count; i++, cell++) {
        if (cell->state != 0) {
            f32 fade = sdfSinPoly((f32)cell->remainingFrames / (f32)cell->durationFrames * 3.141592503f);
            grid->draw(x + cell->x, y + cell->y, z, (s32)((f32)alpha * fade), grid, flags, context);
        }
    }
}

typedef struct MtrDirections { u8 offsets[6][2]; } MtrDirections;
extern const MtrDirections D_00425DD8;

MtrCell *mtrCreateGridPropagationCell(MtrGrid *grid, MtrCell *source, s8 mode) {
    MtrDirections directionTable = D_00425DD8;
    MtrCell *cell = NULL;
    s32 count;
    s32 i;

    count = grid->count;
    for (i = 0; i < count; i++) {
        if (grid->cells[i].state == 0) {
            cell = &grid->cells[i];
            break;
        }
    }
    if (cell == NULL) {
        return NULL;
    }
    {
        if (source != NULL) {
            s8 direction = source->direction + mode;
            s32 countdown;

            if (direction < 0) {
                direction += 6;
            }
            if (direction >= 6) {
                direction -= 6;
            }
            if (grid->axisSwap == 0) {
                cell->x = source->x + (s8)directionTable.offsets[direction][0] * grid->cellWidth;
                cell->y = source->y + (s8)directionTable.offsets[direction][1] * grid->cellHeight;
            } else {
                cell->x = source->x + (s8)directionTable.offsets[direction][1] * grid->cellWidth;
                cell->y = source->y + (s8)directionTable.offsets[direction][0] * grid->cellHeight;
            }
            cell->remainingDepth = source->remainingDepth - 1;
            cell->direction = direction;
            countdown = (u16)grid->delayRange - (u16)grid->depthStep;
            cell->remainingFrames = cell->durationFrames = countdown;
        } else {
            s32 countdown;
            f32 random = sdfGetRandFloat(NULL);
            cell->x = grid->originX + (s32)((f32)(grid->width / grid->cellWidth) * random) * grid->cellWidth;
            random = sdfGetRandFloat(NULL);
            cell->y = grid->originY + (s32)((f32)(grid->height / grid->cellHeight) * random) * grid->cellHeight;
            cell->remainingDepth = grid->initialDepth;
            cell->direction = 0;
            countdown = (u16)grid->delayRange;
            cell->remainingFrames = cell->durationFrames = countdown;
        }
    }
    return cell;
}

extern MtrCell *mtrCreateGridPropagationCell(MtrGrid *, MtrCell *, s8);

void mtrSpawnGridPropagationCells(MtrGrid *grid, MtrCell *source, s8 mode) {
    MtrCell *cell;
    f32 random;
    s32 i;

    switch (mode) {
    case 1:
        cell = mtrCreateGridPropagationCell(grid, NULL, 0);
        if (cell != NULL) {
            cell->state = 4;
        }
        break;
    case 2:
        random = sdfGetRandFloat(0);
        if (random < 0.8f) {
            cell = mtrCreateGridPropagationCell(grid, source, 0);
            if (cell == NULL) {
                return;
            }
            cell->state = mode;
        }
        if (random < 0.9f) {
            cell = mtrCreateGridPropagationCell(grid, source, 1);
            if (cell != NULL) {
                cell->state = 3;
            }
        }
        break;
    case 3:
        if (sdfGetRandFloat(0) < 0.8f) {
            cell = mtrCreateGridPropagationCell(grid, source, 0);
            if (cell != NULL) {
                cell->state = mode;
            }
        }
        break;
    case 4:
        for (i = 0; i < 6; i++) {
            cell = mtrCreateGridPropagationCell(grid, source, i);
            if (cell == NULL) {
                break;
            }
            cell->state = 2;
        }
        break;
    }
}

void mnuDrawCellScaledGrid(s32 x, s32 y, s32 z, s32 alpha, MtrGrid *grid, s32 flags, s32 context) {
    uiDrawUniformColorRect(x << 4, y << 3, z, grid->cellWidth << 4, grid->cellHeight << 3, alpha | 0x80808000, context);
}

void mtrAdvanceGridCellPropagation(MtrGrid *grid) {
    MtrCell *cell = grid->cells;
    s32 i;

    for (i = 0; i < grid->count; i++, cell++) {
        if (cell->state != 0) {
            cell->remainingFrames--;
            if (cell->remainingFrames == cell->durationFrames - 5 && cell->remainingDepth != 0) {
                mtrSpawnGridPropagationCells(grid, cell, cell->state);
            }
            if (cell->remainingFrames == 0) {
                cell->state = 0;
            }
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00285CE8", D_00425DD8);

struct SdfMemBlock *mtrMantraEventBitPush(void) {
    s8 *data;
    struct SdfMemBlock *allocation;
    s32 i;

    allocation = sdfMemAllocDirect(0x76);
    data = (s8 *)sdfMemoryGetBlockAddress(allocation);
    memset(data, 0, 0x76);
    for (i = 0; i < 0x70; i++) {
        if (mdlFlagTest(i + 0x920)) {
            *data = 1;
        }
        data++;
    }
    for (i = 0; i < 6; i++) {
        if (mdlFlagTest(i + 0x9A0)) {
            *data = 1;
        }
        data++;
    }
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraEventBitPush()]*****************\n");
    return allocation;
}


void mtrMantraEventBitPop(struct SdfMemBlock *allocation) {
    s32 i;
    s8 *data = (s8 *)sdfMemoryGetBlockAddress(allocation);

    for (i = 0; i < 0x70; i++) {
        if (*data != 0) {
            mdlFlagSet(i + 0x920);
        } else {
            mdlFlagClear(i + 0x920);
        }
        data++;
    }
    for (i = 0; i < 6; i++) {
        if (*data != 0) {
            mdlFlagSet(i + 0x9A0);
        } else {
            mdlFlagClear(i + 0x9A0);
        }
        data++;
    }
    sdfMemFreeDirect(allocation);
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraEventBitPop()]*****************\n");
}

extern void scrSetEntryLowFlags(DatPartyRecord *context, u16 entryId, u16 lowFlags);
void mtrMantraBitResetUnit(DatPartyRecord *record) {
    s32 i;

    for (i = 0; i < 0xB0; i++) {
        scrSetEntryLowFlags(record, i, 0);
    }
    evtPrintDeveloperConsoleMessage(
        "*****************[mtrMantraBitReset_Unit():[0x%x]]*****************\n", record->unitId);
}

void mtrMantraBitReset(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (datGameState->party[i].flags & 1) {
            mtrMantraBitResetUnit(&datGameState->party[i]);
        }
    }
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraBitReset()]*****************\n");
}

void mtrMantraEventBitReset(void) {
    s32 i;

    for (i = 0; i < 0x70; i++) {
        mdlFlagClear(i + 0x920);
    }
    for (i = 0; i < 6; i++) {
        mdlFlagClear(i + 0x9A0);
    }
    mnuClearCampResourceFlagEntries();
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraEventBitReset()]*****************\n");
}

void mtrRefreshMantraStateForUnit2(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 2;
    mtrRecomputeMantraFlagsForPartyUnit(&record);
    mnuSynchronizeMantraModelFlags(0);
    if (mdlFlagTest(0x9A0)) {
        mdlFlagSet(0x9A1);
    }
}

void mtrRefreshMantraStateForUnit1(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 1;
    mtrRecomputeMantraFlagsForPartyUnit(&record);
    mnuSynchronizeMantraModelFlags(1);
    if (mdlFlagTest(0x9A1)) {
        mdlFlagSet(0x9A0);
    }
}

void mtrRefreshMantraStateForPartyUnit(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 3;
    mtrRecomputeMantraFlagsForPartyUnit(&record);
}

void mtrRefreshMantraStateForUnit7(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 7;
    mtrRecomputeMantraFlagsForPartyUnit(&record);
}

#include "mnu_mantra.h"
#include "mnu_mantra_position_api.h"
extern u16 scrGetEntryLowFlags(DatPartyRecord *, u16);
extern u32 scrGetSelectedScriptEntryId(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16);
extern s32 mnuGetActiveMantraModelFlagState(void);
extern u32 ptyGetProfileRecordValue(DatPartyRecord *work, u16 scriptId);

void mtrSynchronizeMantraFlagsAcrossParty(void) {
    DatPartyRecord first;
    DatPartyRecord second;
    DatPartyRecord merged;
    u16 firstId = 0;
    u16 secondId = 0;
    u32 i;
    s32 maxState;
    u16 flags;

    maxState = mnuGetActiveMantraModelFlagState();
    memset(&first, 0, sizeof(first));
    memset(&second, 0, sizeof(second));
    memset(&merged, 0, sizeof(merged));
    first.unitId = 1;
    second.unitId = 2;
    merged.unitId = 8;
    for (i = 0; i < 16; i++) {
        if (datGameState->templates[i].unitId == 2) {
            secondId = scrGetSelectedScriptEntryId(&datGameState->templates[i]);
        } else if (datGameState->templates[i].unitId == 1) {
            firstId = scrGetSelectedScriptEntryId(&datGameState->templates[i]);
        }
    }
    if (firstId == secondId) {
        secondId = 0;
    }
    for (i = 1; (s32)i < 0xB0; i++) {
        u16 entryId = i;
        u16 other = scrGetEntryLowFlags(&second, entryId);
        u16 firstFlags = scrGetEntryLowFlags(&first, entryId);

        if (firstFlags == 0) {
            scrSetEntryLowFlags(&merged, entryId, 0);
        } else {
            u32 firstLow = firstFlags & 0xF;
            u32 low = other & 0xF;

            if (firstLow < low) {
                low = firstLow;
            }
            flags = low | (firstFlags & 0xF0);

            if ((u16)(((firstFlags & 0xFF00) >> 8) & 1) || (u16)((other >> 8) & 1)) {
                flags = (u16)(flags | 0x100);
            }
            if ((((firstFlags & 0xFF00) >> 8) & 2) || ((other >> 8) & 2)) {
                flags = (u16)(flags | 0x200);
            }
            if (((firstFlags & 0xFF00) >> 8) & 4) {
                flags = (u16)(flags | 0x400);
            }
            if ((((firstFlags & 0xFF00) >> 8) & 8) || ((other >> 8) & 8)) {
                flags = (u16)(flags | 0x800);
            }
            scrSetEntryLowFlags(&merged, entryId, flags);
        }
    }
    if (ptyGetProfileRecordValue(&second, secondId) == ptyGetProfileRecordCap(secondId)) {
        struct MantraNodePos *node;


        scrSetEntryLowFlags(&merged, secondId, (scrGetEntryLowFlags(&merged, secondId) & 0xFFF0) | 0x301);
        node = mnuGetMantraNodePositionRecord((s16)secondId);
        for (i = 0; (s32)i < 6; i++) {
            struct MantraNodePos *neighbor = node->neighbors[i];
            u32 kind;

            if (neighbor == NULL) {
                continue;
            }
            if (neighbor->modelFlagState > maxState) {
                continue;
            }
            kind = neighbor->selector.packed & 0xF;
            if (kind == 1 || kind == 4) {
                flags = scrGetEntryLowFlags(&merged, (u16)neighbor->id);

                if ((flags & 0xF) == 2) {
                    scrSetEntryLowFlags(&merged, node->neighbors[i]->id, (flags & 0xFFF0) | 1);
                }
            }
        }
    }
    mtrRefreshMantraStateForUnit2();
}
