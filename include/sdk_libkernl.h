#ifndef SDK_LIBKERNL_H
#define SDK_LIBKERNL_H

/* SDK kernel: threads, semaphores, interrupts, alarms, file io and SIF (libkernl). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"
#include "sce_io.h"
#include "sdf_kernel_thread_param.h"

#ifndef SDK_SKIP_AddSbusIntcHandler
s32 AddSbusIntcHandler(s32 cause, s32 handler);
#endif
#ifndef SDK_SKIP_CancelWakeupThread
s32 CancelWakeupThread(s32 threadId);
#endif
#ifndef SDK_SKIP_ChangeThreadPriority
s32 ChangeThreadPriority(s32 threadId, s32 priority);
#endif
#ifndef SDK_SKIP_CreateSema
s32 CreateSema(void *param);
#endif
#ifndef SDK_SKIP_CreateThread
s32 CreateThread(SdfKernelThreadParam *param);
#endif
#ifndef SDK_SKIP_DIntr
s32 DIntr();
#endif
#ifndef SDK_SKIP_DeleteThread
s32 DeleteThread(s32 threadId);
#endif
#ifndef SDK_SKIP_EIntr
s32 EIntr(void);
#endif
#ifndef SDK_SKIP_EnableIntc
void EnableIntc(s32 cause);
#endif
#ifndef SDK_SKIP_Exit
void Exit(s32 status);
#endif
#ifndef SDK_SKIP_ExitDeleteThread
void ExitDeleteThread(void);
#endif
#ifndef SDK_SKIP_FlushCache
void FlushCache(s32 mode);
#endif
#ifndef SDK_SKIP_GetThreadId
s32 GetThreadId(void);
#endif
#ifndef SDK_SKIP_LoadExecPS2
void LoadExecPS2(const char *path, s32 argc, char **argv);
#endif
#ifndef SDK_SKIP_RemoveDmacHandler
s32 RemoveDmacHandler(s32 channel, s32 handlerId);
#endif
#ifndef SDK_SKIP_RemoveIntcHandler
s32 RemoveIntcHandler(s32 cause, s32 handlerId);
#endif
#ifndef SDK_SKIP_RemoveSbusIntcHandler
s32 RemoveSbusIntcHandler(s32 cause);
#endif
#ifndef SDK_SKIP_SetAlarm
s32 SetAlarm(u16 delay, void (*callback)(s32 alarmId, u16 time, void *common), void *common);
#endif
#ifndef SDK_SKIP_SignalSema
s32 SignalSema(s32 semaphoreId);
#endif
#ifndef SDK_SKIP_SleepThread
s32 SleepThread(void);
#endif
#ifndef SDK_SKIP_TerminateThread
s32 TerminateThread(s32 threadId);
#endif
#ifndef SDK_SKIP_WaitSema
s32 WaitSema(s32 semaphoreId);
#endif
#ifndef SDK_SKIP_WakeupThread
s32 WakeupThread(s32 threadId);
#endif
#ifndef SDK_SKIP__StartThread
s32 _StartThread(s32 threadId, s32 arg);
#endif
#ifndef SDK_SKIP_iSignalSema
s32 iSignalSema(s32 semaphoreId);
#endif
#ifndef SDK_SKIP_iWakeupThread
s32 iWakeupThread(s32 threadId);
#endif
#ifndef SDK_SKIP_sceClose
s32 sceClose(s32 fd);
#endif
#ifndef SDK_SKIP_sceDclose
s32 sceDclose(s32 directory);
#endif
#ifndef SDK_SKIP_sceDopen
s32 sceDopen(const char *path);
#endif
#ifndef SDK_SKIP_sceDread
s32 sceDread(s32 directory, void *entry);
#endif
#ifndef SDK_SKIP_sceFsReset
void sceFsReset(void);
#endif
#ifndef SDK_SKIP_sceGetstat
s32 sceGetstat(const char *path, SceIoStat *stat);
#endif
#ifndef SDK_SKIP_sceOpen
s32 sceOpen(const char *path, s32 flags, ...);
#endif
#ifndef SDK_SKIP_sceSifAllocIopHeap
s32 sceSifAllocIopHeap(s32 size);
#endif
#ifndef SDK_SKIP_sceSifDmaStat
s32 sceSifDmaStat(s32 transferId);
#endif
#ifndef SDK_SKIP_sceSifGetSreg
s32 sceSifGetSreg(s32 index);
#endif
#ifndef SDK_SKIP_sceSifInitIopHeap
s32 sceSifInitIopHeap(void);
#endif
#ifndef SDK_SKIP_sceSifInitRpc
void sceSifInitRpc(s32 mode);
#endif
#ifndef SDK_SKIP_sceSifLoadFileReset
void sceSifLoadFileReset(void);
#endif
#ifndef SDK_SKIP_sceSifLoadModule
s32 sceSifLoadModule(const char *path, s32 argLen, const char *args);
#endif
#ifndef SDK_SKIP_sceSifRebootIop
s32 sceSifRebootIop(const char *iopImagePath);
#endif
#ifndef SDK_SKIP_sceSifRegisterRpc
void sceSifRegisterRpc(void *serverData, s32 id, void *function, void *buffer, s32 bufferSize, s32 unk, void *queue);
#endif
#ifndef SDK_SKIP_sceSifRpcLoop
void sceSifRpcLoop(void *queue);
#endif
#ifndef SDK_SKIP_sceSifSendCmd
s32 sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 extraSize);
#endif
#ifndef SDK_SKIP_sceSifSetDma
s32 sceSifSetDma(void *transfer, s32 count);
#endif
#ifndef SDK_SKIP_sceSifSetRpcQueue
void sceSifSetRpcQueue(void *queue, s32 threadId);
#endif
#ifndef SDK_SKIP_sceSync
s32 sceSync(const char *device, s32 flag);
#endif
#ifndef SDK_SKIP_sceWrite
s32 sceWrite(s32 fd, const void *data, s32 size);
#endif
#ifndef SDK_SKIP_sdfDevSeekFile
s32 sdfDevSeekFile(s32 fileHandle, s32 offset, s32 origin);
#endif
#ifndef SDK_SKIP_DisableDmac
s32 DisableDmac(s32 channel);
#endif
#ifndef SDK_SKIP_EnableDmac
s32 EnableDmac(s32 channel);
#endif

#ifdef VERSION_DDS2
#ifndef SDK_SKIP_sdfDevReadFileBytes
s32 sdfDevReadFileBytes(s32 fileHandle, s32 buffer, s32 byteCount);
#endif
#endif

#endif
