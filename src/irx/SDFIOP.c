#include "iop.h"

/* SDFIOP.IRX: the EE asks this module, over RPC, to load another IRX off the
 * disc. See docs/sdf-irx-modules.md. */

/* The strings stay in asm/irx/SDFIOP/rodata.s so .rodata keeps its layout. */
extern const char fmt[];                /* "sdfLoadStartModule: %s args %d" */
extern const char aArgS[];              /* " arg %s\n", then "\n" at +0xc */
extern const char aSdfloadstartmo_0[];  /* "...: %s couldn't open.\n" */
extern const char aSdfloadstartmo_1[];  /* "...: %s seek error.\n" */
extern const char aSdfloadstartmo_2[];  /* "...: %s isn't module file.\n" */
extern const char aSdfloadstartmo_3[];  /* "...: memory not enough.\n" */
extern const char aSdfloadstartmo_4[];  /* "...: %s read error.\n" */
extern const char aSdfloadstartmo_5[];  /* "...: LoadModuleBuffer fail.[re = %d].\n" */

/* One 16 byte object: [0] takes the return code the RPC replies with, [1..3]
 * are the result slots modload_StartModule fills in. */
extern s32 sdfIopManagerRpcResult[4];

void sdfIopPrintf(const char *fmt, ...);
s32 sdfLoadStartModule(const char *path, s32 arglen, const char *args, s32 *result);
void *sdfIopManagerRpcHandler(s32 fno, void *data, s32 size);
void sdfIopManagerRpcThread(void *arg);

/* The RPC server data the thread registers; only its size matters here. */
typedef struct {
    s32 unused[6];
} sdf_rpc_data;

s32 msifrpc_e4(s32 arg);
s32 msifrpc_e17(sdf_rpc_data *sd, u32 sid, void *handler, void *arg);

void sdfIopPrintf(const char *format, ...)
{
    char buf[0x400];
    va_list ap;

    va_start(ap, format);
    sysclib_vsprintf(buf, format, ap);
    stdio_printf(buf);
}

s32 sdfLoadStartModule(const char *path, s32 arglen, const char *args, s32 *result)
{
    s32 fd;
    s32 size;
    s32 mid;
    void *buf;

    sdfIopPrintf(fmt, path, arglen, args);
    if (arglen) {
        sdfIopPrintf(aArgS, args);
    } else {
        sdfIopPrintf(aArgS + 0xc);
    }

    fd = ioman_open(path, 1, 0);
    if (fd < 0) {
        sdfIopPrintf(aSdfloadstartmo_0, path);
        return -203;
    }
    size = ioman_lseek(fd, 0, 2);
    if (size < 0 || ioman_lseek(fd, 0, 0) < 0) {
        ioman_close(fd);
        sdfIopPrintf(aSdfloadstartmo_1, path);
        return -204;
    }
    if (size == 0) {
        ioman_close(fd);
        sdfIopPrintf(aSdfloadstartmo_2, path);
        return -201;
    }
    buf = sysmem_AllocSysMemory(1, size, NULL);
    if (buf == NULL) {
        ioman_close(fd);
        sdfIopPrintf(aSdfloadstartmo_3);
        return -400;
    }
    if (ioman_read(fd, buf, size) != size) {
        ioman_close(fd);
        sdfIopPrintf(aSdfloadstartmo_4);  /* the %s has no argument */
        return -204;
    }
    ioman_close(fd);
    mid = modload_LoadModuleBuffer(buf);
    sysmem_FreeSysMemory(buf);
    if (mid < 0) {
        sdfIopPrintf(aSdfloadstartmo_5, mid);
    }
    return modload_StartModule(mid, path, arglen, args, result);
}

/* Call 1 is "load and start the module named in the packet". The packet is
 * {path length, argument length, path, arguments}. */
void *sdfIopManagerRpcHandler(s32 fno, void *data, s32 size)
{
    const char *path;
    s32 arglen;
    const char *args;

    if (fno == 1) {
        arglen = ((s32 *)data)[1];
        path = (const char *)data + 8;
        if (arglen == 0) {
            args = NULL;
        } else {
            args = path + ((s32 *)data)[0] + 1;
        }
        sdfIopManagerRpcResult[0] =
            sdfLoadStartModule(path, arglen, args, &sdfIopManagerRpcResult[1]);
        return sdfIopManagerRpcResult;
    }
    return NULL;
}

void sdfIopManagerRpcThread(void *arg)
{
    sdf_rpc_data sd;

    if (sifman_SifSetIOPrcvaddr() == 0) {
        sifman_SifSIF2Init();
    }
    sifcmd_SifInitRpc(0);
    msifrpc_e4(0);
    msifrpc_e17(&sd, 0x6F496453, sdfIopManagerRpcHandler, NULL);
}

s32 start(s32 argc, char **argv)
{
    iop_thread_t th;
    s32 tid;
    s32 ret;

    th.attr = 0x02000000;
    th.thread = sdfIopManagerRpcThread;
    th.priority = 0x42;
    th.stacksize = 0x800;
    th.option = 0;
    tid = thbase_CreateThread(&th);
    if (tid <= 0) {
        ret = 1;
    } else {
        thbase_StartThread(tid, NULL);
        ret = 0;
    }
    return ret;
}

ALIGN_TEXT(4);
IRX_IMPORT_TABLE(msifrpc, 1, 1);
IRX_IMPORT(msifrpc, e4, 0x4);
IRX_IMPORT(msifrpc, e17, 0x11);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sysmem, 1, 1);
IRX_IMPORT(sysmem, AllocSysMemory, 0x4);
IRX_IMPORT(sysmem, FreeSysMemory, 0x5);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(ioman, 1, 4);
IRX_IMPORT(ioman, open, 0x4);
IRX_IMPORT(ioman, close, 0x5);
IRX_IMPORT(ioman, read, 0x6);
IRX_IMPORT(ioman, lseek, 0x8);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(modload, 1, 6);
IRX_IMPORT(modload, StartModule, 0x8);
IRX_IMPORT(modload, LoadModuleBuffer, 0xa);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sifcmd, 1, 1);
IRX_IMPORT(sifcmd, SifInitRpc, 0xe);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sifman, 1, 1);
IRX_IMPORT(sifman, SifSIF2Init, 0x5);
IRX_IMPORT(sifman, SifSetIOPrcvaddr, 0x1d);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(stdio, 1, 3);
IRX_IMPORT(stdio, printf, 0x4);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sysclib, 1, 3);
IRX_IMPORT(sysclib, vsprintf, 0x2a);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(thbase, 1, 2);
IRX_IMPORT(thbase, CreateThread, 0x4);
IRX_IMPORT(thbase, StartThread, 0x6);
IRX_IMPORT_TABLE_END;
ALIGN_TEXT(4);
