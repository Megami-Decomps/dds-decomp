#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "sdf_file_existence_api.h"
#include "sdf_console_packet.h"
#include "sif_rpc_client_prefix.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_alarm.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "sdf_timer.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf_draw.h"
#include "sdf_packet_alloc.h"
#include "sdf_sif_command.h"
#include "sdf_dev_event.h"
#include "sdf_dev_protocol.h"
#include "sdf_dev_state.h"
#include "sdf_dev_path_prefix.h"
#include "sdf_dev_worker.h"
#include "sdf_dev_start_load_api.h"
#include "sdf_disc_file_record.h"
#include "btl.h"
#include "sdf_debug_api.h"

extern u8 sdfFileCmdThreadPrioCheckCounter;

extern s32 sdfDiscRequestPending;

extern char sdfFileHostRootPath[];

extern s32 sdfFileCmdThreadPrio;

extern u8 D_00438B50[];

extern u8 D_00438B58[];

extern u32 sdfCdTrayMode;

extern u16 sdfFileCmdDefaultCreateMode;

extern s32 sdfFileSemaId;

extern char D_0042E3A0[]; /* "cdrom0:\\IRX\\DEV9.IRX;1 resident fail.\n", followed by padding no C emits */

extern u8 sdfDev9LoadFlag;

extern u8 sdfDev9Ready;

extern s32 sceSifLoadStartModule(const char *modulePath, s32 flags, void *moduleArguments, s32 *resident);

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_00438AF0[];

extern s8 sdfPrintIopFlag;

extern char D_00438AC0[];

extern char sdfCdDirectPrefix[]; /* cdrom */

extern char sdfDiscPathPrefix[];

typedef struct Bytes6 {
    s8 b[6];
} Bytes6;

extern Bytes6 sdfPfsPathPrefix[];

extern char D_00438B00[]; /* cdrom */

extern u8 sdfFileDefaultSrcType;

extern u8 sdfFileCmdDefaultDevice;

extern sdfFileCmdHandle_t *D_00438B08;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdFreeCount;

extern sdfFileCmdHandle_t *D_00438B14;

extern sdfFileCmdHandle_t *D_00438B18;

extern s16 sdfFileCmdCount;

extern char D_00438B28[]; /* cdrom0: */

extern char D_00438B30[]; /* host0: */

extern char D_00438B38[]; /* pfs0: */

extern char D_00438B40[]; /* /movie/ */

extern char D_00438B48[]; /* cdimg: */

extern char D_00438B60[];

extern void sdfDev9LoadModule(void);

void sdfDev9LoadModule(void) {
    s32 resident;
    if (sdfDev9LoadFlag == 0) {
        sdfDev9Ready = 0;
        if (sceSifLoadStartModule("cdrom0:\\IRX\\DEV9.IRX;1", 0, NULL, &resident) < 0) {
            printf("cdrom0:\\IRX\\DEV9.IRX;1 could't load.\n");
            return;
        }
        if (resident != 0) {
            printf(D_0042E3A0);
            return;
        }
        sdfDev9Ready = 1;
        sdfDev9LoadFlag = 1;
    }
}

INCLUDE_RODATA(const s32, "sdf/sdfDev9", D_0042E3A0);

