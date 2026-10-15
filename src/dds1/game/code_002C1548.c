#include "sdf_gs_register_submit_api.h"
#include "sdf_gs_header.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "fpu.h"
#include "eff.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "itf_grid_text.h"
#include "itf_draw_grid.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"

void sdfDispatchSurfaceWithPreparedTexturePacket(s32 surfaceIndex);

void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind);

/* Dispatch prepared texture data and configure the menu background surface. */
void uiPrepareMenuBackgroundSurface(s32 unused, s32 surface) {
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    sdfSubmitGsTestOneRegisterPacket(0x50000, surface);
    sdfSubmitGsAlphaOneRegisterPacket(0x54, surface);
}
