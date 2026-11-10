#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"

extern u32 sdfDefaultCdMode;

extern u32 sdfCdTrayMode;

extern u8 sdfPadManagerMode;

extern sdfThCB_t dds3KernelThread;

extern u8 dds3KernelStack[];

extern void __main(void);

extern void sceSifInitRpc(s32 mode);

extern s32 sceSifInitIopHeap(void);

extern s32 sdfCdInit(void);

extern s32 sceSifRebootIop(const char *image);

extern s32 sceSifSyncIop(void);

extern s32 sceSifLoadFileReset(void);

extern void sceFsReset(void);

extern s32 sceCdInit(s32 aMode);

extern s32 sceCdMmode(s32 aMode);

extern void sdfCdImageInit(const char *imageFile, const char *dirFile);

extern s32 sceSifLoadModule(const char *path, s32 argSize, const char *args);

extern void LoadMcModules(const char *prefix, s32 moduleGroup);

extern void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes);

extern void sdfPadChangeMode(s32 aPadNum, u8 mode);

extern void sdfFileSetHostRoot(char *aRootPath);

extern void sdfFileSetDefaultSrc(s32 aType);

extern void sdfFileCmdStartThread(s32 deviceType);

extern void SdfIopInit(const char *driverPath, const char *modulePath);

extern void sdfStartFileRpcServer(void);


extern void sdfSetMainThreadPriority(void);

extern void dds3KernelMain(void);

/* Named around `main` so cc1 does not emit the __main call it adds to a
   function of that name. */
s32 sdfDefaultMain(void) FAKEMATCH_ASM_NAME("main");

/* DDS loads the pad drivers before the disc image; DDS1 boots another IOP
   image and the packet buffer differs per game. */
s32 sdfDefaultMain(void)
{
    __main();
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sdfCdTrayMode = sdfDefaultCdMode;
    sdfCdInit();
    while (sceSifRebootIop("cdrom0:\\IRX\\IOPRP250.IMG;1") == 0) {
    }
    while (sceSifSyncIop() == 0) {
    }
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    sceFsReset();
    sceCdInit(0);
    sceCdMmode(sdfDefaultCdMode);
    while (sceSifLoadModule("cdrom0:\\IRX\\SIO2MAN.IRX;1", 0, NULL) < 0) {
    }
    while (sceSifLoadModule("cdrom0:\\IRX\\PADMAN.IRX;1", 0, NULL) < 0) {
    }
    sdfCdImageInit("\\DDS3.IMG;1", "cdrom0:\\DDS3.DDT;1");
    LoadMcModules("cdrom0:\\IRX", 1);
    dds3InitSubsystems(0x17C0000, 0x200000, 0x335000);
    sdfPadManagerMode = 1;
    sdfPadChangeMode(0, 3);
    sdfPadChangeMode(1, 3);
    sdfFileSetHostRoot("../../../dds3data");
    sdfFileSetDefaultSrc(3);
    sdfFileCmdStartThread(3);
    sdfFileCmdStartThread(0);
    SdfIopInit("cdrom0:\\USERIRX\\SDFIOP.IRX;1", "cdrom0:\\IRX\\MSIFRPC.IRX;1");
    sdfStartFileRpcServer();
    sdfStartThreadAndWait(&dds3KernelThread, dds3KernelMain, dds3KernelStack, 0x4000, 0x60, 0);
    sdfSetMainThreadPriority();
    return 0;
}

INCLUDE_RODATA(const s32, "sdf/sdfDefaultMain", D_00411198);

INCLUDE_SDATA(const s32, "sdf/sdfDefaultMain", sdfDefaultCdMode);

