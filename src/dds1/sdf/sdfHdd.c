#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

extern s32 iSignalSema(s32 aSemaId);

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

extern s32 (*p2sdSmgBuf)(void);

extern u8 sdfHddValid;

extern u8 sdfDev9Ready;

extern char sdfHddDevPfs[];

extern char sdfHddDevHdd[];

extern char sdfHddMountPoint[];

extern char sdfHddIrxArgs[];

extern char sdfHddPfsIrxArgs[];

extern u8 sdfHddPowerOffStack[];

#include "sdk_libc.h"

extern s32 WaitSema(s32 aSemaId);

#include "sdk_libkernl.h"

extern s32 sceCdPowerOff(s32 *result);

extern void sdfDev9LoadModule(void);

extern s32 sceSifLoadStartModule(const char *path, s32 arglen, char *args, s32 *result);

extern s32 sdfCreateSema(s32 initial, s32 max, s32 option);

extern s32 CreateThreadSimple(void *entry, void *stack, s32 stackSize, s32 priority);

typedef void (*cdvdPowerCallback_t)(void *argument);
extern cdvdPowerCallback_t sceCdPOffCallback(cdvdPowerCallback_t handler, void *argument);

extern s32 sceFsInit(void);

extern s32 sceMount(char *mountPoint, char *device, s32 flag, void *arg, s32 arglen);

void sdfHddPowerOffHandler(void *aArg);

/* DDS has no printf here. */
void sdfHddPowerOffTh(s32 aSemaId)
{
    s32 result;

    for (;;) {
        WaitSema(aSemaId);
        sceDevctl(sdfHddDevPfs, 0x5003, NULL, 0, NULL, 0);
        sceDevctl(sdfHddDevHdd, 0x4806, NULL, 0, NULL, 0);
        sceCdPowerOff(&result);
    }
}

void sdfHddPowerOffHandler(void *aArg)
{
    iSignalSema(EE_ADDR_S(aArg));
}

/* DDS takes the mount path and has no printf. */
void sdfHddInit(char *aPath)
{
    char buffer[0x100];
    s32 resident;
    s32 result;
    s32 sema;
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
    result = sceSifLoadStartModule("cdrom0:\\IRX\\HDD.IRX;1", 0xC, sdfHddIrxArgs, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    result = sceSifLoadStartModule("cdrom0:\\IRX\\PFS.IRX;1", 0x12, sdfHddPfsIrxArgs, &resident);
    if (result < 0 || resident != 0) {
        return;
    }
    sema = sdfCreateSema(0, 1, 0);
    thread = CreateThreadSimple(sdfHddPowerOffTh, sdfHddPowerOffStack, 0x800, 1);
    _StartThread(thread, (void *)sema);
    sceCdPOffCallback(sdfHddPowerOffHandler, (void *)sema);
    sceFsInit();
    sprintf(buffer, "hdd0:%s,", aPath);
    sceMount(sdfHddMountPoint, buffer, 0, NULL, 0);
    sdfHddValid = 1;
}

INCLUDE_SDATA(const s32, "sdf/sdfHdd", sdfHddDevPfs);

INCLUDE_SDATA(const s32, "sdf/sdfHdd", sdfHddDevHdd);

INCLUDE_SDATA(const s32, "sdf/sdfHdd", sdfHddMountPoint);

