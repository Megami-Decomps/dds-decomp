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
#include "sdf_linked_packet.h"
#include "sdf_dev_start_load_api.h"
#include "kwln.h"
#include "kwln_task_flags.h"
#include "sdf.h"
#include "sdf_pad_mode_api.h"
#include "sdf_thread.h"
#include "sdf_thread_sleep.h"
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

extern void sceCdMmode(u32);

extern void LoadMcModules(const char *prefix, s32 moduleGroup);

extern void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes);

extern void sdfFileSetHostRoot(const char *path);

extern void sdfFileSetDefaultSrc(s32 worker);

extern void sdfFileCmdStartThread(s32 index);

extern void sdfLoadModulesAndBindNamedResourceRpc(const char *modulePath, const char *optionalModulePath);

extern void sdfStartFileRpcServer(void);

extern void kwlnRunMainLoop(void);

extern char D_0039DF28[]; /* "cdrom0:\IRX\IOPRP255.IMG;1" */

extern char D_0039DF48[]; /* "cdrom0:\IRX\SIO2MAN.IRX;1" */

extern char D_0039DF68[]; /* "cdrom0:\IRX\PADMAN.IRX;1" */

extern char D_0039DF88[]; /* "\DDS3.IMG;1" */

extern char D_0039DF98[]; /* "cdrom0:\DDS3.DDT;1" */

extern char D_0039DFB0[]; /* "cdrom0:\IRX" */

extern char D_0039DFC0[]; /* "../../../dds3data" */

extern char D_0039DFD8[]; /* "cdrom0:\USERIRX\SDFIOP.IRX;1" */

extern char D_0039DFF8[]; /* "cdrom0:\IRX\MSIFRPC.IRX;1" */

extern const s32 D_003BA83C;

extern u32 sdfDiscType;

extern u8 D_003BD39C;

extern SdfThreadNode kwlnMainLoopThreadNode;

extern u8 D_003BDC90[];

s32 sdfDefaultMain(void) asm("main");

s32 sdfDefaultMain(void) {
    __main();
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sdfDiscType = D_003BA83C;
    sdfInitDeviceSemaphores();
    while (sceSifRebootIop(D_0039DF28) == 0) {
    }
    while (sceSifSyncIop() == 0) {
    }
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    sceFsReset();
    sceCdInit(0);
    sceCdMmode(D_003BA83C);
    while (sceSifLoadModule(D_0039DF48, 0, 0) < 0) {
    }
    while (sceSifLoadModule(D_0039DF68, 0, 0) < 0) {
    }
    sdfCdImageInit(D_0039DF88, D_0039DF98);
    LoadMcModules(D_0039DFB0, 1);
    dds3InitSubsystems(0x17C0000, 0x200000, 0x380000);
    D_003BD39C = 1;
    sdfPadChangeMode(0, SDF_PAD_MODE_DUALSHOCK_LOCKED);
    sdfPadChangeMode(1, SDF_PAD_MODE_DUALSHOCK_LOCKED);
    sdfFileSetHostRoot(D_0039DFC0);
    sdfFileSetDefaultSrc(3);
    sdfFileCmdStartThread(3);
    sdfFileCmdStartThread(0);
    sdfLoadModulesAndBindNamedResourceRpc(D_0039DFD8, D_0039DFF8);
    sdfStartFileRpcServer();
    sdfStartThreadAndWait(&kwlnMainLoopThreadNode, kwlnRunMainLoop, D_003BDC90, 0x4000, 0x60, 0);
    sdfSetMainThreadPriority();
    return 0;
}

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DF28);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DF48);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DF68);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DF88);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DF98);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DFB0);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DFC0);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DFD8);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039DFF8);

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_0039E018);

INCLUDE_SDATA(const s32, "sdf/sdfDefaultMain", D_003BA83C);

