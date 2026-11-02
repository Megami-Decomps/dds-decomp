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

extern void sceDevctl(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

extern u8 D_003BD460[];

extern u8 D_003BD468[];

extern u8 sdfDev9Ready;

extern s32 sceSifLoadStartModule(const char *modulePath, s32 flags, void *moduleArguments, s32 *resident);

void sdfHddPowerOffTh(s32 semaphore) {
    s32 status;

    for (;;) {
        WaitSema(semaphore);
        sceDevctl(D_003BD460, 0x5003, 0, 0, 0, 0);
        sceDevctl(D_003BD468, 0x4806, 0, 0, 0, 0);
        sceCdPowerOff(&status);
    }
}

void sdfHddPowerOffHandler(void *semaphore) {
    iSignalSema((s32)semaphore);
}

extern u8 sdfHddValid;

extern char sdfDevHddModuleArguments[];

extern char sdfDevPfsModuleArguments[];

extern char D_003FA800[];

extern char D_003BD470[];

extern void sdfDev9LoadModule(void);

extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);

typedef void (*SdfPowerOffCallback)(void *);

extern SdfPowerOffCallback sceCdPOffCallback(SdfPowerOffCallback, void *);

extern s32 sceFsInit(void);

extern s32 sceMount(const char *, const char *, s32, const void *, s32);

/* Load the HDD/PFS dev modules and mount path before enabling PFS debug mode. */
void sdfHddInit(char *path) {
    char buffer[0x100];
    s32 resident;
    s32 result;
    s32 semaphore;
    s32 thread;

    sdfHddValid = 0;
    sdfDev9LoadModule();
    if (sdfDev9Ready == 0) {
        return;
    }
    result = sceSifLoadStartModule("cdrom0:\\IRX\\ATAD.IRX;1", 0, NULL, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    result = sceSifLoadStartModule("cdrom0:\\IRX\\HDD.IRX;1", 0xC, sdfDevHddModuleArguments, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    result = sceSifLoadStartModule("cdrom0:\\IRX\\PFS.IRX;1", 0x12, sdfDevPfsModuleArguments, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    semaphore = sdfCreateSema(0, 1, 0);
    thread = CreateThreadSimple(sdfHddPowerOffTh, D_003FA800, 0x800, 1);
    _StartThread(thread, semaphore);
    sceCdPOffCallback(sdfHddPowerOffHandler, (void *)semaphore);
    sceFsInit();
    sprintf(buffer, "hdd0:%s,", path);
    sceMount(D_003BD470, buffer, 0, 0, 0);
    sdfHddValid = 1;
}
INCLUDE_SDATA(const s32, "sdf/sdfHdd", D_003BD460);

INCLUDE_SDATA(const s32, "sdf/sdfHdd", D_003BD468);

INCLUDE_SDATA(const s32, "sdf/sdfHdd", D_003BD470);

INCLUDE_SDATA(const s32, "sdf/sdfHdd", sdfDev9Ready);

INCLUDE_SDATA(const s32, "sdf/sdfHdd", sdfCdClock);

