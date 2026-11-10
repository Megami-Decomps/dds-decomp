#include "common.h"
#include "sdf_vu0.h"
#include "sdf_chip.h"
#include "eff_param.h"
#include "eff_param_operations.h"
#include "eff_thunder_vector.h"
#include "par_cell_api.h"
#include "sdf_heap_api.h"
#include "btl_state.h"
#include "eff.h"
#include "eff_battle_color_keyframe.h"
#include "eff_battle_transition_duration.h"
#include "ee_mmi.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "pcp_vu0.h"
#include "eff_matrix4.h"
#include "mdl.h"
#include "btl_grouped_entity_api.h"
#include "bill_object_api.h"

extern u16 effPartModelSerial;

/* Allocate the copied head and its trailing cells as one block, then create
 * the cell system with native arguments groupDivisor=0 and kind=4.
 * Only three words per cell are zeroed here; vector/range storage is untouched. */
EffThunderVectorWork *effPCPThunderSpherePolyCreate(EffThunderAlphaParams *source) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(source->cellCount * sizeof(EffThunderVectorCell) + sizeof(EffThunderVectorWork));
    EffThunderVectorWork *work = (EffThunderVectorWork *)sdfMemLock(allocation);
    u32 cellIndex;

    work->head.alpha = *source;
    work->cells = (EffThunderVectorCell *)(work + 1);
    work->baseRadiusScale = source->radiusScale;
    work->baseHeightScale = source->heightScale;
    work->allocationHandle = allocation;
    work->cellSystem = dds3TrackPolyCreate(work->head.alpha.cellCount, work->head.alpha.perCell, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->cellSystem, work->head.alpha.centerAlphaWord, work->head.alpha.middleAlphaWord, work->head.alpha.edgeAlphaWord);
    dds3TrackPolySetMode(work->cellSystem, work->head.alpha.systemParam);
    for (cellIndex = 0; cellIndex < work->head.alpha.cellCount; cellIndex++) {
        work->cells[cellIndex].delayFrames = 0;
        work->cells[cellIndex].activeFrames = 0;
        work->cells[cellIndex].color = 0;
    }
    work->tintColor = 0x80808080;
    return work;
}

