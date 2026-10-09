#include "common.h"
#include "mnu_work.h"
#include "mnu_shooting.h"
#include "mdl.h"

struct WideSlot;
struct CompactSlot;
extern struct WideSlot *itfClaimWideSlotWithTaggedPayload(u32, u32, u32, s8, struct WideSlotPool *);
extern struct CompactSlot *itfClaimCompactSlotWithPayload(u32, u32, struct CompactSlotPool *);
extern s32 func_003242D0(MenuWorkEntry *, s32);
extern f32 mnuEvaluateTimedValue(MenuWorkEntry *);
extern u32 mnuAdvanceWorkEntry(MenuWorkEntry *, s32);
extern void func_0031A830(MnuShootingWork *);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern MenuWorkEntry D_0040ABF8;

void func_0031A288(MenuRuntimeRecord *record, MenuWorkEntry *entry, MnuShootingWork *work) {
    MenuProgressParameters *origin;
    MenuRegistry *registry;
    MenuRegistryParameters *parameters;
    s32 originX;
    s32 originY;
    s32 x;
    s32 score;
    f32 y;

    origin = mnuGetResourceProgressParameters();
    originX = origin->x;
    originY = origin->y;
    if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_MOVEMENT_TABLE) {
        if ((record->state.kind & 0xF) == 3) {
            if (func_003242D0(entry, 0) == 0) {
                work->currentScore += 10000;
                y = mnuEvaluateTimedValue(entry);
                itfClaimWideSlotWithTaggedPayload((s32)(entry->currentX + (f32)originX),
                    (s32)y + originY, 10000, 0, work->spriteWork);
            }
            evtPrintDeveloperConsoleMessage("ITEM GET !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %d\n", (D_0040ABF8.flags >> 15) & 0xF);
        } else if (D_0040ABF8.remaining != 0) {
            mnuAdvanceWorkEntry(entry, 1);
        }
        return;
    }

    registry = mnuGetMenuRecordRegistryEntry(entry->tag);
    mnuGetMenuRegistryParametersByIndex(registry->parameterIndex);
    if (registry->parameterIndex == 0x22) {
        if (entry->flagsBits.modelMotionStarted == 0) {
            if (entry->flagsBits.halfRemainingCountReached == 1) {
                MnuModelNode *node = entry->object.modelNode;
                if (work->round == 0) {
                    node->model->first->frameStep = 1.0f;
                    node->savedModelValue = 1.0f;
                    mdlAddEntryFlaggedEx(node->model, 0, 2, 0.0f, 30.0f);
                } else if (work->round == 2) {
                    node->model->first->frameStep = 1.0f;
                    node->savedModelValue = 1.0f;
                    mdlAddEntryFlaggedEx(node->model, 0, 10, 0.0f, 30.0f);
                }
                entry->flagsBits.modelMotionStarted = 1;
            }
        }
    }
    switch (record->state.kind & 0xF) {
    case 0:
    case 5:
        parameters = mnuGetMenuRegistryParametersByIndex(registry->parameterIndex);
        x = (s32)(record->rotatedOffsetX + record->displacementX + record->baseX);
        y = mnuEvaluateTimedValue(entry);
        y += parameters->hitOffsetY;
        y += parameters->hitHeight;
        itfClaimCompactSlotWithPayload(x + originX, (s32)y + originY, work->tintWork);
        if (entry->flagsBits.finished) {
            if (work->completed == 0) {
                work->progress++;
            }
            func_0031A830(work);
            score = registry->score * work->step;
            work->currentScore += score;
            evtPrintDeveloperConsoleMessage("Add Score[%d]\n", score);
        } else {
            work->currentScore += work->step;
        }
        work->updateCount += 10;
        if (work->updateCount >= 91) {
            work->updateCount = 90;
        }
        break;
    case 4:
        if (entry->flagsBits.finished) {
            if (work->completed == 0) {
                work->progress += registry->progress;
            }
            func_0031A830(work);
            score = registry->score * work->step;
            work->currentScore += score;
            evtPrintDeveloperConsoleMessage("Add Score[%d]\n", score);
        }
        break;
    }
}
