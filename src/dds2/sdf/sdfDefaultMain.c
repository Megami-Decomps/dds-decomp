#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_column_packet_api.h"
#include "kwln_draw_buffer.h"
#include "kwln_frame_draw_buffer.h"
#include "sdf_packet_list.h"
#include "sdf_draw_packet_groups.h"
#include "kwln_frame_draw_bank.h"
#include "kwln_sprite.h"
#include "dds3Admin.h"
#include "kwln.h"
#include "kwln_task_flags.h"
#include "sdf.h"
#include "sdf_pad_mode_api.h"
#include "sdf_thread.h"
#include "sdf_thread_sleep.h"
#include "sdf_linked_packet.h"
#include "sdf_dev_start_load_api.h"
#include "sdf_projection.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"
#include "sdf_packet_alloc.h"
#include "sdf_message.h"

typedef struct SdfThreadNode SdfThreadNode;

extern void __main(void);

extern void sdfInitDeviceSemaphores(void);

extern s32 sceSifSyncIop(void);

extern void sceCdInit(s32);

extern void sceCdMmode(u32 discType);

extern void LoadMcModules(const char *prefix, s32 moduleGroup);

extern void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes);

extern void sdfFileSetHostRoot(const char *path);

extern void sdfFileSetDefaultSrc(s8 worker);

extern void sdfFileCmdStartThread(s32 index);

extern void sdfLoadModulesAndBindNamedResourceRpc(const char *modulePath, const char *optionalModulePath);

extern void sdfStartFileRpcServer(void);

extern void kwlnRunMainLoop(void);

extern const char D_004110A8[];

extern const char D_004110C8[];

extern const char D_004110E8[];

extern const char D_00411108[];

extern const char D_00411118[];

extern const char D_00411130[];

extern const char D_00411140[];

extern const char D_00411158[];

extern const char D_00411178[];

extern const s32 D_00435C0C;

extern u32 sdfDiscType;

extern u8 D_00438A8C;

extern SdfThreadNode kwlnMainLoopThreadNode;

extern u8 D_00439410[];

s32 sdfDefaultMain(void) asm("main");

s32 sdfDefaultMain(void) {
    __main();
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sdfDiscType = D_00435C0C;
    sdfInitDeviceSemaphores();
    while (sceSifRebootIop(D_004110A8) == 0) {
    }
    while (sceSifSyncIop() == 0) {
    }
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    sceFsReset();
    sceCdInit(0);
    sceCdMmode(D_00435C0C);
    while (sceSifLoadModule(D_004110C8, 0, 0) < 0) {
    }
    while (sceSifLoadModule(D_004110E8, 0, 0) < 0) {
    }
    sdfCdImageInit(D_00411108, D_00411118);
    LoadMcModules(D_00411130, 1);
    dds3InitSubsystems(0x17C0000, 0x200000, 0x335000);
    D_00438A8C = 1;
    sdfPadChangeMode(0, SDF_PAD_MODE_DUALSHOCK_LOCKED);
    sdfPadChangeMode(1, SDF_PAD_MODE_DUALSHOCK_LOCKED);
    sdfFileSetHostRoot(D_00411140);
    sdfFileSetDefaultSrc(3);
    sdfFileCmdStartThread(3);
    sdfFileCmdStartThread(0);
    sdfLoadModulesAndBindNamedResourceRpc(D_00411158, D_00411178);
    sdfStartFileRpcServer();
    sdfStartThreadAndWait(&kwlnMainLoopThreadNode, kwlnRunMainLoop, D_00439410, 0x4000, 0x60, 0);
    sdfSetMainThreadPriority();
    return 0;
}

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_004110A8);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_004110C8);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_004110E8);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_00411108);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_00411118);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_00411130);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_00411140);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_00411158);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_00411178);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_00411198);

INCLUDE_SDATA(const s32, "sdf/sdfDefaultMain", D_00435C0C);

