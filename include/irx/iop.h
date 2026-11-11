#ifndef IRX_IOP_H
#define IRX_IOP_H

#include "spu2.h"

/* The IOP library calls the three modules import. Every module links its own
 * stubs, so a name declared here only resolves in the modules that import it.
 * The stub chunks live in asm/irx/<module>/text/. */

#include "irx.h"

typedef struct {
    u32 attr;
    u32 option;
    void *thread;
    s32 stacksize;
    s32 priority;
    s32 unused;
} iop_thread_t;

typedef struct {
    u32 attr;
    u32 option;
    s32 initial;
    s32 max;
} iop_sema_t;

/* sceSifClientData: the EE side fills in server once the bind completes. */
typedef struct {
    void *pkt_addr;
    u32 rpc_id;
    s32 sema_id;
    u32 mode;
    u32 command;
    void *buff;
    void *cbuff;
    void (*end_function)(void *);
    void *end_param;
    void *server;
} SifRpcClientData_t;


/* ioman */
s32 ioman_open(const char *path, s32 flags, s32 mode);
s32 ioman_close(s32 fd);
s32 ioman_read(s32 fd, void *buf, s32 len);
s32 ioman_lseek(s32 fd, s32 offset, s32 whence);

/* sysmem */
void *sysmem_AllocSysMemory(s32 place, s32 size, void *addr);
s32 sysmem_FreeSysMemory(void *addr);

/* modload */
s32 modload_LoadModuleBuffer(void *buf);
s32 modload_StartModule(s32 mid, const char *path, s32 arglen, const char *args, s32 *result);

/* stdio, sysclib */
s32 stdio_printf(const char *fmt, ...);
s32 sysclib_vsprintf(char *buf, const char *fmt, va_list ap);
void *sysclib_memcpy(void *dst, const void *src, s32 len);
void *sysclib_memset(void *dst, s32 c, s32 len);
s32 sysclib_strlen(const char *s);
s32 sysclib_strncmp(const char *a, const char *b, s32 n);
s32 sysclib_strtol(const char *s, char **end, s32 base);
s32 sysclib_look_ctype_table(s32 c);

/* thbase */
s32 thbase_CreateThread(iop_thread_t *param);
s32 thbase_StartThread(s32 tid, void *arg);
s32 thbase_ExitThread(void);
s32 thbase_SleepThread(void);
s32 thbase_iWakeupThread(s32 tid);
s32 thbase_iCancelWakeupThread(s32 tid);
s32 thbase_DelayThread(u32 usec);
s32 thbase_USec2SysClock(u32 usec, u32 *sysclock);
s32 thbase_GetSystemTime(u32 *sysclock);
s32 thbase_SysClock2USec(u32 *sysclock, u32 *sec, u32 *usec);
s32 thbase_e43(void);

/* thsemap */
s32 thsemap_CreateSema(iop_sema_t *param);
s32 thsemap_SignalSema(s32 sid);
s32 thsemap_iSignalSema(s32 sid);
s32 thsemap_WaitSema(s32 sid);

/* sifcmd, sifman */
s32 sifcmd_SifInitRpc(s32 mode);
s32 sifcmd_SifBindRpc(SifRpcClientData_t *client, u32 sid, u32 mode);
s32 sifcmd_SifCallRpc(SifRpcClientData_t *client, u32 fno, u32 mode,
                      void *send, s32 ssize, void *recv, s32 rsize,
                      void (*end_function)(void *), void *end_param);
s32 sifman_SifSIF2Init(void);
s32 sifman_SifSetIOPrcvaddr(void);

/* loadcore */
s32 loadcore_RegisterLibraryEntries(void *lib);
void loadcore_FlushDcache(void);

/* intrman */
s32 intrman_CpuSuspendIntr(s32 *state);
s32 intrman_CpuResumeIntr(s32 state);

/* libsd, the SPU2 library. Declared here rather than beside the sound
 * driver because Sony's MODHSYN imports it too. */
typedef struct {
    s32 core;
    u32 mode;
    u16 depth_L;
    u16 depth_R;
    s32 delay;
    s32 feedback;
} sceSdEffectAttr_t;

s32 libsd_sceSdInit(s32 flag);
u16 libsd_sceSdSetParam(u16 entry, u16 value);
u16 libsd_sceSdGetParam(u16 entry);
void libsd_sceSdSetSwitch(u16 entry, u32 value);
u32 libsd_sceSdGetSwitch(u16 entry);
void libsd_sceSdSetAddr(u16 entry, u32 value);
u32 libsd_sceSdGetAddr(u16 entry);
u16 libsd_sceSdSetCoreAttr(u16 entry, u16 value);
u16 libsd_sceSdNote2Pitch(u16 center, u16 fine, u16 note, s16 bend);
u16 libsd_sceSdPitch2Note(u16 center, u16 fine, u16 pitch);
s32 libsd_sceSdProcBatch(u32 *cmds, u32 *rets, u32 count);
s32 libsd_sceSdProcBatchEx(u32 *cmds, u32 *rets, u32 count, u32 flags);
s32 libsd_sceSdVoiceTrans(s16 chan, u16 mode, u8 *iopaddr, u32 spuaddr, u32 size);
s32 libsd_sceSdBlockTrans(s16 chan, u16 mode, u8 *iopaddr, u32 size);
s32 libsd_sceSdVoiceTransStatus(s16 chan, s16 flag);
s32 libsd_sceSdBlockTransStatus(s16 chan, s16 flag);
s32 libsd_sceSdSetEffectAttr(s32 core, sceSdEffectAttr_t *attr);
void libsd_sceSdGetEffectAttr(s32 core, sceSdEffectAttr_t *attr);
s32 libsd_sceSdClearEffectWorkArea(s32 core, s32 channel, s32 effectMode);
void *libsd_sceSdSetSpu2IntrHandler(void *func, void *arg);

#endif /* IRX_IOP_H */
