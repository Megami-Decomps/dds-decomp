#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "sdf_file_existence_api.h"
#include "sdf_console_packet.h"
#include "sif_rpc_client_prefix.h"
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
#include "sdf_thread_workspace_api.h"
#include "sdf_dev_start_load_api.h"
#include "sdf_disc_file_record.h"
#include "btl.h"
#include "sdf_debug_api.h"

extern char D_003B4690[]; /* "cdrom0:\\IRX\\DEV9.IRX;1 resident fail.\n", followed by padding no C emits */

extern u8 sdfFileCmdThreadPrioCheckCounter;

extern u8 sdfFileCmdDefaultDevice;

extern u8 sdfFileDefaultSrcType;

extern s16 sdfFileCmdCount;

extern sdfFileCmdHandle_t *D_003BD424;

extern sdfFileCmdHandle_t *D_003BD428;

extern s32 sdfFileCmdThreadPrio;

extern s32 sdfFileSemaId;

extern s32 sdfDiscRequestPending;

extern char sdfFileHostRootPath[];

extern u8 sdfPfsPathPrefix[];

extern u32 sdfCdTrayMode;

extern u8 D_003BD460[];

extern u8 D_003BD468[];

extern u16 sdfFileCmdDefaultCreateMode;

extern u8 sdfDev9LoadFlag;

extern u8 sdfDev9Ready;

extern s32 sceSifLoadStartModule(const char *modulePath, s32 flags, void *moduleArguments, s32 *resident);

extern s8 sdfPrintIopFlag;

extern char D_003BD3D0[];

extern char D_003BD3E0[]; /* cdrom */

extern char sdfDiscPathPrefix[];

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BD400[];

extern char D_003BD410[]; 

extern sdfFileCmdHandle_t *D_003BD418;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdFreeCount;

extern char D_003BD438[]; /* cdrom0: */

extern char D_003BD440[]; /* host0: */

extern char D_003BD448[]; /* pfs0: */

extern char D_003BD450[]; /* /movie/ */

extern char D_003BD458[]; /* cdimg: */

extern char D_003BD470[];

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
            printf(D_003B4690);
            return;
        }
        sdfDev9Ready = 1;
        sdfDev9LoadFlag = 1;
    }
}

INCLUDE_RODATA(const s32, "sdf/sdfDev9", D_003B4690);

