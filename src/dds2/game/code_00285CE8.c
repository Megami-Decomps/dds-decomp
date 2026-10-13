#include "common.h"
#include "eff_random.h"
#include "sdf_resource.h"
#include "dat_state.h"


extern void evtPrintDeveloperConsoleMessage();

extern void mdlFlagClear(s32 flag);

extern void mdlFlagSet(s32 flag);

extern s32 mdlFlagTest(s32 flag);

extern void mnuClearCampResourceFlagEntries(void);

extern void mnuSynchronizeMantraModelFlags(s32 arg);

extern void func_00286BA8(void *record);

extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);




typedef struct MtrCell {
    s16 x;
    s16 y;
    s16 unk04;
    s16 unk06;
    s8 direction;
    s8 state;
    u8 remainingDepth;
    u8 unk0B;
} MtrCell;

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
    s16 unk1C;
    s16 unk1E;
    s16 depthStep;
    u8 initialDepth;
    s8 axisSwap;
    void (*draw)(s32, s32, s32, s32, struct MtrGrid *, s32, s32);
} MtrGrid;

extern void mtrSpawnGridPropagationCells(MtrGrid *grid, MtrCell *source, s8 mode);
extern void mtrAdvanceGridCellPropagation(MtrGrid *grid);

void mtrUpdateGridPropagation(MtrGrid *grid) {
    if (grid->remainingDelay == 0) {
        if (grid->unk14 < grid->unk16) {
            mtrSpawnGridPropagationCells(grid, NULL, 1);
        }
        grid->remainingDelay = grid->delayRange * effMiscRandUnitFloat(0) + 1.0f;
    } else {
        grid->remainingDelay--;
    }
    mtrAdvanceGridCellPropagation(grid);
}
extern f32 sdfSinPoly(f32);

void mtrDrawGridPropagationCells(s32 x, s32 y, s32 z, s32 alpha, MtrGrid *grid, s32 flags, s32 context) {
    MtrCell *cell = grid->cells;
    s32 i;

    for (i = 0; i < grid->count; i++, cell++) {
        if (cell->state != 0) {
            f32 fade = sdfSinPoly((f32)cell->unk04 / (f32)cell->unk06 * 3.141592503f);
            grid->draw(x + cell->x, y + cell->y, z, (s32)((f32)alpha * fade), grid, flags, context);
        }
    }
}

MtrCell *mtrCreateGridPropagationCell(MtrGrid *grid, MtrCell *source, s8 mode) {
    typedef struct MtrDirections { u8 offsets[6][2]; } MtrDirections;
    extern const MtrDirections D_00425DD8;
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
                cell->x = source->x + (s8)directionTable.offsets[direction][0] * grid->unk1C;
                cell->y = source->y + (s8)directionTable.offsets[direction][1] * grid->unk1E;
            } else {
                cell->x = source->x + (s8)directionTable.offsets[direction][1] * grid->unk1C;
                cell->y = source->y + (s8)directionTable.offsets[direction][0] * grid->unk1E;
            }
            cell->remainingDepth = source->remainingDepth - 1;
            cell->direction = direction;
            countdown = (u16)grid->delayRange - (u16)grid->depthStep;
            cell->unk04 = cell->unk06 = countdown;
        } else {
            s32 countdown;
            f32 random = effMiscRandUnitFloat(NULL);
            cell->x = grid->originX + (s32)((f32)(grid->width / grid->unk1C) * random) * grid->unk1C;
            random = effMiscRandUnitFloat(NULL);
            cell->y = grid->originY + (s32)((f32)(grid->height / grid->unk1E) * random) * grid->unk1E;
            cell->remainingDepth = grid->initialDepth;
            cell->direction = 0;
            countdown = (u16)grid->delayRange;
            cell->unk04 = cell->unk06 = countdown;
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
        random = effMiscRandUnitFloat(0);
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
        if (effMiscRandUnitFloat(0) < 0.8f) {
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

void mnuDrawCellScaledGrid(s32 arg0, s32 arg1, s32 arg2, s32 arg3, MtrGrid *arg4, s32 arg5, s32 arg6) {
    uiDrawUniformColorRect(arg0 << 4, arg1 << 3, arg2, arg4->unk1C << 4, arg4->unk1E << 3, arg3 | 0x80808000, arg6);
}

void mtrAdvanceGridCellPropagation(MtrGrid *grid) {
    MtrCell *cell = grid->cells;
    s32 i;

    for (i = 0; i < grid->count; i++, cell++) {
        if (cell->state != 0) {
            cell->unk04--;
            if (cell->unk04 == cell->unk06 - 5 && cell->remainingDepth != 0) {
                mtrSpawnGridPropagationCells(grid, cell, cell->state);
            }
            if (cell->unk04 == 0) {
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

    allocation = sdfAllocGeneralBlock(0x76);
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
    sdfReleaseResourceAllocation(allocation);
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

void func_00286618(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 2;
    func_00286BA8(&record);
    mnuSynchronizeMantraModelFlags(0);
    if (mdlFlagTest(0x9A0)) {
        mdlFlagSet(0x9A1);
    }
}

void func_00286670(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 1;
    func_00286BA8(&record);
    mnuSynchronizeMantraModelFlags(1);
    if (mdlFlagTest(0x9A1)) {
        mdlFlagSet(0x9A0);
    }
}

void mtrRefreshMantraStateForPartyUnit(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 3;
    func_00286BA8(&record);
}

void func_00286700(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 7;
    func_00286BA8(&record);
}

INCLUDE_ASM(const s32, "game/code_00285CE8", mtrSynchronizeMantraFlagsAcrossParty);
