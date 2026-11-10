#include "sdfsnd.h"

/* SDFSDMAN.IRX, the sound driver: it owns the SPU2, plays the .SMG banks and
 * the streamed ADPCM, and takes its orders from the EE over SIF.
 * See docs/sdf-irx-modules.md.
 *
 * The strings stay in asm/irx/SDFSDMAN/rodata.s so .rodata keeps its layout;
 * /tmp aside, their text is listed in docs/sdf-irx-modules.md. */

/* DDS's driver is a later revision of SMT3's. The text and names here are
 * SMT3's, from smt3-decomp, adapted where DDS changed the code: 13 bank kinds
 * instead of 7 (SDF_SND_KINDS), a 0x44-byte sdfSndSlot_t, interrupt locks in
 * the BGM paths, and a stream player with less logging. The functions still in
 * DDS's asm are listed in docs/irx-modules.md. */
extern const char aSdfsdmanIFThre[];
extern const char aSdfsnddrvSOpen[];
extern const char aSdfsndfileread[];
extern const char aSdfsnddrvScems[];
extern const char aHardTimerAlloc[];
extern const char aTimerHandlerRe[];
extern const char aHardTimerSetup[];
extern const char aHardTimerRelea[];
extern const char aHardTimerStart[];
extern const char aHardTimerStopF[];
extern const char aA1[];
extern const char aA2[];
extern const char aAd[];
extern const char aSdfsnddrvScemi[];
extern const char aSdfsnddrvScemi_0[];
extern const char aSdfsnddrvScemi_1[];
extern const char aSdfsnddrvScehs[];
extern const char aSdfsnddrvScemi_2[];
extern const char aSdfsnddrvScemi_3[];
extern const char aSdfsnddrvPlayS[];
extern const char aSdfsnddrvScemi_4[];
extern const char aSdfsnddrvScehs_0[];
extern const char aSdfsnddrvSeSta[];
extern const char aSdfsnddrvFileR[];
extern const char aSdfsnddrvMidi0[];
extern const char aSdfsnddrvSData[];
extern const char aSdfsnddrvDataS[];
extern const char aSdfsnddrvSRead[];
extern const char aSdfsnddrvScehs_1[];
extern const char aSdfsnddrvCreat[];
extern const char aSdfsnddrvWaits[];
extern const char aP2sdirxspuiopm[];
extern const char aP2sdirxspuiopm_0[];
extern const char aSdfsnddrvScesi[];
extern const char aSdfsnddrvIfcou[];
extern const char aSdfsnddrvUnkno[];
extern const char aSdfadpcmAdpcmN[];
extern const char aSdfadpcmCreate[];
extern const char aSdfadpcmAdpcmF[];
extern const char a02xh[];
extern const char aSdfadpcmOnMemo[];

s32 sdfSdManInit(void);
s32 sdfSndIsqrt(s32 v);
s32 sdfSndAtan2Deg(s32 x, s32 y);
s32 sdfSndPostError(s32 code);
s32 sdfSndUpdateVoiceMap(void);
s32 sdfSndFileOpen(const char *path, s32 mode);
s32 sdfSndFileRead(s32 fd, void *buf, s32 len, s32 mode);
s32 sdfSndFileGetSize(s32 fd);
s32 sdfSndFileClose(s32 fd);
u8 *sdfSndGetVolTable(s32 kind, s32 *count);
s32 sdfSndApplyVolume(s32 id, s32 slot, s32 vol);
s32 sdfSndPutMsg(s32 slot);
s32 sdfSndIsSlotMuted(s32 id);
s32 sdfSndMidiCbIgnore1(void);
s32 sdfSndMidiCbIgnore2(void);
s32 sdfSndMidiCbMessage(u32 msg, s32 slot);
s32 sdfSndTimerHandler(sdfSndTimer_t *tm);
s32 sdfSndTimerSetup(sdfSndTimer_t *tm);
s32 sdfSndTimerRelease(sdfSndTimer_t *tm);
s32 sdfSndTimerStart(sdfSndTimer_t *tm);
s32 sdfSndTimerStop(sdfSndTimer_t *tm);
s32 sdfSndInitWorkAreas(void);
void sdfSndSeqThread(void *arg);
s32 sdfSndInitSpu2(void);
s32 sdfSndMidiSelect(s32 slot, s32 seq, s32 bank, void *data, s32 loc);
s32 sdfSndStopAll(s32 which);
s32 sdfSndInitSynth(sdfSndWorkArea_t *work);
s32 sdfSndQuitSynth(void);
s32 sdfSndFindSlotById(s32 idPacked);
s32 sdfSndBgmPlay(sdfSndBgmReq_t *req);
s32 sdfSndSaveResumePoint(s32 slot);
sdfSndResume_t *sdfSndFindResumePoint(s32 id);
s32 sdfSndBgmResume(sdfSndBgmReq_t *req);
s32 sdfSndBgmStop(s32 *req);
s32 sdfSndHsynLoad(s32 bank, s32 slot);
s32 sdfSndSePlay(sdfSndSeReq_t *req);
s32 sdfSndDistanceAtten(s32 dist, s32 radius);
s32 sdfSndUpdate3dVoice(s32 slot);
s32 sdfSndSePlayPositional(s32 id, s32 angle, s32 dist, s32 radius);
s32 sdfSndSetListener(sdfSndListenerReq_t *req);
s32 sdfSndSePlay3d(sdfSndSe3dReq_t *req);
s32 sdfSndIsBankLoaded(s32 kind);
s32 sdfSndLoadSmg(const char *path, s32 mode);
void sdfSndSmgLoadThread(void *arg);
s32 sdfSndSetBankAddr(s32 kind, u32 waveSize);
s32 sdfSndStopByGroup(s32 group);
s32 sdfSndUnloadBank(s32 kind);
s32 p2sdIrxSpuIopMap(void);
s32 sdfSndFadeOutById(s32 *req);
s32 sdfSndRequestBgmSwitch(s32 midi);
s32 sdfSndAllocBgmSlot(void);
s32 sdfSndAllocSeSlot(void);
s32 sdfSndSetSeqVolume(s32 vol);
s32 sdfSndFadeOutAll(void);
s32 sdfSndFadeInAll(void);
s32 sdfSndSifCmdHandler(sdfSndCmd_t *pkt, void *harg);
s32 sdfSndCreateRpcSema(void);
s32 sdfSndLoadVolTables(void);
s32 sdfSndStoreVolTables(void);
void sdfSndKeepAliveThread(void *arg);
s32 sdfSndStartKeepAliveThread(void);
s32 sdfSndDrvRpcInit(void *arg);
s32 sdfSndCallEeRpc(s32 arg, void *buf, s32 len, u32 fno, u32 *reply);
s32 sdfAdpcmFadeTick(void);
s32 sdfAdpcmFadeOut(s32 step);
s32 sdfAdpcmStop(void);
s32 sdfAdpcmUploadBlock(void);
s32 sdfAdpcmSpu2IntrHandler(void *arg);
s32 sdfAdpcmFillBuffer(s32 blocking);
s32 sdfAdpcmSetVolume(s32 vol);
s32 sdfAdpcmStreamPlay(const char *path, s32 vol, s8 onMemory);
void sdfAdpcmStreamInit(void *arg);

s32 sdfSdManInit(void)
{
    iop_thread_t rpc;
    iop_thread_t smg;
    iop_thread_t adpcm;
    s32 tid;
    s32 ret;

    if (sifman_SifSetIOPrcvaddr() == 0) {
        sifman_SifSIF2Init();
    }
    sifcmd_SifInitRpc(0);

    rpc.thread = sdfSndDrvRpcInit;
    rpc.attr = 0;
    rpc.priority = 40;
    rpc.stacksize = 2048;
    rpc.option = 0;
    tid = thbase_CreateThread(&rpc);
    if (tid <= 0) {
        goto fail;
    }
    thbase_StartThread(tid, NULL);

    smg.thread = sdfSndSmgLoadThread;
    smg.attr = 0;
    smg.priority = 41;
    smg.stacksize = 2048;
    smg.option = 0;
    tid = thbase_CreateThread(&smg);
    if (tid <= 0) {
        goto fail;
    }
    thbase_StartThread(tid, NULL);

    adpcm.thread = sdfAdpcmStreamInit;
    adpcm.attr = 0;
    adpcm.priority = 42;
    adpcm.stacksize = 2048;
    adpcm.option = 0;
    tid = thbase_CreateThread(&adpcm);
    if (tid <= 0) {
fail:
        stdio_printf(aSdfsdmanIFThre);
        ret = 1;
    } else {
        thbase_StartThread(tid, NULL);
        ret = 0;
    }
    return ret;
}

/* sdfSdManInit was a translation unit of its own: the linker aligned what
 * follows it to 16, leaving one nop behind. */
ALIGN_TEXT(4);

/* Newton's method; the caller wants the distance between two points. */
s32 sdfSndIsqrt(s32 v)
{
    s32 g, prev;

    if (v <= 0) {
        return -1;
    }
    g = 1;
    do {
        prev = g;
        g = (v / g + g) / 2;
    } while (g != prev && g != prev + 1 && g != prev - 1);
    return g;
}
#ifdef NON_MATCHING
/* The angle of (x, y) in degrees, from a 32 entry table of atan(n/31).
 * Two instructions short: gcc copies x into the negation where the retail
 * code negates its own copy in place. */
s32 sdfSndAtan2Deg(s32 x, s32 y)
{
    s32 ax;
    s32 ay;
    s32 deg;
    s32 idx;

    ax = x;
    if (x < 0) {
        ax = -ax;
    }
    ay = y;
    if (y < 0) {
        ay = -ay;
    }
    if (ax == 0) {
        deg = 0;
    } else if (ay == 0) {
        deg = 90;
    } else if (ax >= ay) {
        idx = 31 * ay / ax;
        deg = 90 - sdfSndAtanTbl[idx];
    } else {
        idx = 31 * ax / ay;
        deg = sdfSndAtanTbl[idx];
    }
    if (y < 0) {
        deg = 180 - deg;
    }
    if (x < 0) {
        deg = -deg;
    }
    return deg;
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndAtan2Deg);
#endif

s32 sdfSndPostError(s32 code)
{
    sdfSndWorkArea->errCode = code;
    sdfSndWorkArea->errCount++;
    return p2sdIrxSpuIopMap();
}

/* Copy the synthesiser voice map into the work area the EE reads: 24 voices
 * per core, -1 where the voice is not in use. */
#ifdef NON_MATCHING
/* One instruction short: gcc fills the branch delay slot with the second
 * arm instead of the address of the slot being written. */
s32 sdfSndUpdateVoiceMap(void)
{
    s32 core;
    s32 base;
    s32 i;
    u8 m;
    u8 v;

    core = 0;
    base = 0;
    do {
        i = 0;
        do {
            m = sdfSndHsynVoiceMap[i + base];
            if ((m & 0x80) != 0) {
                v = m & 0x70;
            } else {
                v = -1;
            }
            sdfSndWorkArea->voiceMap[i + 32 * core] = v;
            i++;
        } while (i < 24);
        core++;
        base += 24;
    } while (core < 2);
    return p2sdIrxSpuIopMap();
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndUpdateVoiceMap);
#endif

/* mode 1 reads through the EE instead of the file library. */
s32 sdfSndFileOpen(const char *path, s32 mode)
{
    s32 fd;

    if (mode == 1) {
        sdfSndCallEeRpc(0, NULL, 0, 3, NULL);
        return 0;
    }
    fd = sdfiopfl_sdfIopFileReadOpen(path);
    if (fd == 0) {
        stdio_printf(aSdfsnddrvSOpen, path);
        sdfSndPostError(1);
        return -1;
    }
    return fd;
}

s32 sdfSndFileRead(s32 fd, void *buf, s32 len, s32 mode)
{
    s32 read;

    if (mode == 1) {
        sdfSndCallEeRpc(0, buf, len, 4, NULL);
        return len;
    }
    read = sdfiopfl_sdfIopFileRead(fd, buf, len);
    if (read != len) {
        stdio_printf(aSdfsndfileread, read);
    }
    return read;
}

s32 sdfSndFileGetSize(s32 fd)
{
    return sdfiopfl_sdfIopFileGetFileSize(fd);
}

s32 sdfSndFileClose(s32 fd)
{
    return sdfiopfl_sdfIopFileClose(fd);
}
INCLUDE_ASM("SDFSDMAN", sdfSndGetVolTable);

#ifdef NON_MATCHING
/* Scale the slot volume by the per sound entry in its volume table, and by
 * the global sequence volume for the three sequence slots.
 * Matches but for the two registers id and vol land in. */
s32 sdfSndApplyVolume(s32 id, s32 slot, s32 vol)
{
    s32 kind;
    u8 *tbl;
    s32 v;
    s32 state;

    kind = sdfSndFindSlotById(id);
    if (kind < 0) {
        return kind;
    }
    tbl = sdfSndGetVolTable(kind, NULL);
    if (tbl != NULL) {
        v = tbl[id & 0x7F];
    } else {
        v = 127;
    }
    v = (vol * v) >> 7;
    if (slot < 3) {
        v = (v * sdfSndSeqVolume) >> 7;
    }
    if (v >= 128) {
        v = 127;
    }
    if (v < 0) {
        v = 0;
    }
    intrman_CpuSuspendIntr(&state);
    modmidi_sceMidi_MidiSetVolume(&sdfSndMidiWork, slot, 255, (u8)v);
    return intrman_CpuResumeIntr(state);
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndApplyVolume);
#endif

/* Reset all 16 channels of a slot: bank select, volume and all notes off. */
s32 sdfSndPutMsg(s32 slot)
{
    s32 i;
    s32 ret;

    for (i = 0; i != 16; i++) {
        if (modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, i | 0x400AB0)) {
            stdio_printf(aSdfsnddrvScems);
        }
        if (modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, i | 0x7F0BB0)) {
            stdio_printf(aSdfsnddrvScems);
        }
        ret = modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, i | 0x4000E0);
        if (ret) {
            ret = stdio_printf(aSdfsnddrvScems);
        }
    }
    return ret;
}

s32 sdfSndIsSlotMuted(s32 id)
{
    s32 kind;
    u8 *tbl;

    kind = sdfSndFindSlotById(id);
    if (kind >= 0) {
        tbl = sdfSndGetVolTable(kind, NULL);
        if (tbl != NULL) {
            if (tbl[id & 0x7F] == 0) {
                return 1;
            }
        }
    }
    return 0;
}

s32 sdfSndMidiCbIgnore1(void)
{
    return 1;
}

s32 sdfSndMidiCbIgnore2(void)
{
    return 1;
}
INCLUDE_ASM("SDFSDMAN", sdfSndMidiCbMessage);

s32 sdfSndTimerHandler(sdfSndTimer_t *tm)
{
    s32 thid;

    thid = tm->thid;
    thbase_iCancelWakeupThread(tm->thid);
    thbase_iWakeupThread(thid);
    return tm->interval;
}

/* One hard timer ticking at 240Hz wakes the sequence thread. */
s32 sdfSndTimerSetup(sdfSndTimer_t *tm)
{
    u32 sysclock[2];
    u32 clk;
    s32 id;

    thbase_USec2SysClock(0x1047, sysclock);
    clk = sysclock[0];
    tm->interval = clk;
    id = timrman_AllocHardTimer(1, 32, 1);
    tm->timerId = id;
    if (id <= 0) {
        stdio_printf(aHardTimerAlloc);
        return 1;
    }
    if (timrman_SetTimerHandler(id, clk, (s32 (*)(void *))sdfSndTimerHandler, tm) != 0) {
        stdio_printf(aTimerHandlerRe);
        return 1;
    }
    if (timrman_SetupHardTimer(id, 1, 0, 1) != 0) {
        stdio_printf(aHardTimerSetup);
        return 1;
    }
    return 0;
}

s32 sdfSndTimerRelease(sdfSndTimer_t *tm)
{
    s32 id;

    id = tm->timerId;
    if (id > 0) {
        if (timrman_FreeHardTimer(id) != 0) {
            stdio_printf(aHardTimerRelea);
            return 1;
        }
        tm->timerId = 0;
    }
    return 0;
}

s32 sdfSndTimerStart(sdfSndTimer_t *tm)
{
    if (timrman_StartHardTimer(tm->timerId) != 0) {
        stdio_printf(aHardTimerStart, tm->timerId);
        return 1;
    }
    return 0;
}

s32 sdfSndTimerStop(sdfSndTimer_t *tm)
{
    s32 ret;

    ret = timrman_StopHardTimer(tm->timerId);
    if (ret != 0 && ret != -156) {
        stdio_printf(aHardTimerStopF);
        return 1;
    }
    return 0;
}

#ifdef NON_MATCHING
/* Same instructions, but gcc picks a different base register for the
 * synthesiser work area writes. */
/* Point the synthesiser and MIDI tables at the 16 sequence and slot work
 * areas, then take the per bank voice counts from the work area. */
s32 sdfSndInitWorkAreas(void)
{
    s32 i;
    u32 bank;
    sceMSInBuf_t *hseq;
    sceHSynSlot_t *hslot;
    sceMidiSlot_t *mslot;
    sceMSInBuf_t *voice;

    sdfSndHsynWork.unk00 = 1;
    sdfSndHsynWork.unk10 = 0;
    sdfSndHsynWork.unk0C = 0;
    sdfSndHsynWork.tbl = &sdfSndHsynTbl;
    sdfSndHsynTbl.nEntry = 32;
    sdfSndHsynTbl.entry = sdfSndHsynEntry;
    hseq = sdfSndMsInBuf;
    hslot = sdfSndHsynSlotWork;
    i = 0;
    do {
        sdfSndHsynEntry[i].unk00 = 0;
        sdfSndHsynEntry[i].buf = hseq;
        sdfSndHsynEntry[i].unk08 = 0;
        sdfSndHsynEntry[i].slot = hslot;
        hseq->size = 1032;
        hseq->len = 0;
        hseq++;
        hslot++;
        i++;
    } while (i < 16);

    bank = 0;
    if (sdfSndWorkArea->bankCount != 0) {
        i = 0;
        do {
            sdfSndHsynSlotWork[i].unk00 = sdfSndWorkArea->banks[bank].unk00;
            sdfSndHsynSlotWork[i].unk01 = sdfSndWorkArea->banks[bank].unk01;
            bank++;
            i++;
        } while (bank < sdfSndWorkArea->bankCount);
    }

    sdfSndMidiWork.unk00 = 2;
    sdfSndMidiWork.unk10 = 0;
    sdfSndMidiWork.unk0C = 0;
    sdfSndMidiWork.tbl = &sdfSndMidiTbl;
    sdfSndMidiTbl.nSeq = 32;
    sdfSndMidiTbl.seq = sdfSndMidiSeq;
    mslot = sdfSndMidiSlotWork;
    i = 0;
    do {
        sdfSndMidiSeq[i].slot = mslot;
        sdfSndMidiSeq[i].unk00 = 0;
        sdfSndMidiSeq[i].data = NULL;
        sdfSndMidiSeq[i].unk08 = 0;
        mslot++;
        i++;
    } while (i < 16);

    sdfSndMidiTbl.nVoice = 16;
    sdfSndMidiTbl.voice = sdfSndMidiVoice;
    voice = sdfSndMsInBuf;
    i = 0;
    do {
        sdfSndMidiVoice[i].buf = voice;
        sdfSndMidiVoice[i].unk00 = 0;
        voice++;
        i++;
    } while (i < 16);
    return sdfSndSetBankAddr(-1, 0);
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndInitWorkAreas);
#endif
INCLUDE_ASM("SDFSDMAN", sdfSndSeqThread);


#ifdef NON_MATCHING
/* Same instructions in a different order: gcc fills the call delay slot
 * with the argument truncation the retail code schedules earlier. */
s32 sdfSndInitSpu2(void)
{
    s32 core;
    sceSdEffectAttr_t *attr;
    u32 addr;

    libsd_sceSdInit(0);
    core = 0;
    attr = sdfSndEffectAttr;
    do {
        addr = 0x1FFFFF;
        if (core == 1) {
            addr = 0x1FFFF;
        }
        libsd_sceSdSetAddr(core | 0x1D00, addr);
        libsd_sceSdSetEffectAttr(core, attr);
        libsd_sceSdSetCoreAttr(core | 2, 1);
        libsd_sceSdSetParam(core | 0xB80, 0x4FFF);
        libsd_sceSdSetParam(core | 0xC80, 0x4FFF);
        libsd_sceSdSetParam(core | 0x980, 0x3FFF);
        libsd_sceSdSetParam(core | 0xA80, 0x3FFF);
        core++;
        attr++;
    } while (core < 2);
    sdfSndBgmSwitchReq = 0;
    sdfSndBgmSwitchSlot = -1;
    sdfSndBgmSwitchMidi = -1;
    return -1;
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndInitSpu2);
#endif

#ifdef NON_MATCHING
/* 7 instructions short of the retail code. */
/* FUNC sdfSndMidiSelect */
/* Load a sequence into a MIDI slot, point its callbacks back here and park
 * it at the requested location. */
s32 sdfSndMidiSelect(s32 slot, s32 seq, s32 bank, void *data, s32 loc)
{
    sceMidiSlot_t *ms;
    s32 i;
    s32 code;
    s32 state;

    intrman_CpuSuspendIntr(&state);
    sdfSndMidiWork.tbl->seq[slot].data = data;
    sdfSndHsynLoad(bank, slot);
    if (modmidi_sceMidi_Load(&sdfSndMidiWork, slot) != 0) {
        intrman_CpuResumeIntr(state);
        stdio_printf(aSdfsnddrvScemi);
        code = 17;
        goto fail;
    }
    ms = &sdfSndMidiSlotWork[slot];
    ms->cbB = sdfSndMidiCbIgnore1;
    ms->msgCb = sdfSndMidiCbMessage;
    ms->cbBArg = slot;
    ms->msgCbArg = slot;
    ms->cbC = sdfSndMidiCbIgnore2;
    ms->cbCArg = slot;
    ms->slotMask = 1 << slot;
    for (i = 15; i >= 0; i--) {
        ms->chanMask[i] = 1 << slot;
    }
    if (seq < 0) {
        if (modmidi_sceMidi_SelectMidi(&sdfSndMidiWork, slot, 0) != 0) {
            goto selectFailed;
        }
    } else if (modmidi_sceMidi_SelectMidi(&sdfSndMidiWork, slot, seq) != 0) {
selectFailed:
        intrman_CpuResumeIntr(state);
        stdio_printf(aSdfsnddrvScemi_0);
        code = 7;
        goto fail;
    }
    if (modmidi_sceMidi_MidiSetLocation(&sdfSndMidiWork, slot, loc) == 0) {
        intrman_CpuResumeIntr(state);
        return 0;
    }
    intrman_CpuResumeIntr(state);
    stdio_printf(aSdfsnddrvScemi_1);
    code = 8;
fail:
    sdfSndPostError(code);
    return 1;
}
/* ENDF sdfSndMidiSelect */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndMidiSelect);
#endif
INCLUDE_ASM("SDFSDMAN", sdfSndStopAll);
INCLUDE_ASM("SDFSDMAN", sdfSndInitSynth);

/* FUNC sdfSndQuitSynth */
s32 sdfSndQuitSynth(void)
{
    s32 ret;

    ret = (s32)sdfSndWorkArea;
    if (sdfSndWorkArea != NULL) {
        sdfSndTimerStop(&sdfSndTimer);
        sdfSndTimerRelease(&sdfSndTimer);
        sdfSndStopAll(0);
        sysmem_FreeSysMemory(sdfSndWorkArea->xferBuf);
        sysmem_FreeSysMemory(sdfSndWorkArea->banks[0].data);
        sysmem_FreeSysMemory(sdfSndWorkArea->progData[0].ptr);
        ret = sysmem_FreeSysMemory(sdfSndWorkArea);
        sdfSndWorkArea = NULL;
    }
    return ret;
}
/* ENDF sdfSndQuitSynth */
#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndFindSlotById */
/* The bank slot a packed sound id belongs to, or -1. */
s32 sdfSndFindSlotById(s32 idPacked)
{
    s32 i;
    s32 id;

    id = idPacked >> 16;
    if (idPacked < 0) {
        return -1;
    }
    for (i = 0; i < SDF_SND_KINDS; i++) {
        if (sdfSndWorkArea->stat[i].id == id) {
            return i;
        }
    }
    return -1;
}
/* ENDF sdfSndFindSlotById */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndFindSlotById);
#endif
INCLUDE_ASM("SDFSDMAN", sdfSndBgmPlay);
INCLUDE_ASM("SDFSDMAN", sdfSndSaveResumePoint);

/* FUNC sdfSndFindResumePoint */
sdfSndResume_t *sdfSndFindResumePoint(s32 id)
{
    s32 i;

    if (id < 0) {
        return NULL;
    }
    for (i = 0; i < 32; i++) {
        if (sdfSndResume[i].id == id) {
            return &sdfSndResume[i];
        }
    }
    return NULL;
}
/* ENDF sdfSndFindResumePoint */
INCLUDE_ASM("SDFSDMAN", sdfSndBgmResume);
INCLUDE_ASM("SDFSDMAN", sdfSndBgmStop);
INCLUDE_ASM("SDFSDMAN", sdfSndHsynLoad);
INCLUDE_ASM("SDFSDMAN", sdfSndSePlay);

#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndDistanceAtten */
/* Volume percentage for a sound `dist` away with the given audible radius. */
s32 sdfSndDistanceAtten(s32 dist, s32 radius)
{
    s32 outer;
    s32 atten;

    outer = 4 * radius;
    if (radius < 3) {
        radius = 3;
        outer = 12;
    }
    atten = 10 * (2 * (outer + radius) - (dist - 10)) / radius;
    if (atten < 0) {
        return 0;
    }
    if (atten >= 101) {
        return 100;
    }
    return atten;
}
/* ENDF sdfSndDistanceAtten */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndDistanceAtten);
#endif
INCLUDE_ASM("SDFSDMAN", sdfSndUpdate3dVoice);

#ifdef NON_MATCHING
/* 4 instructions short of the retail code. */
/* FUNC sdfSndSePlayPositional */
/* Turn an angle and a distance into a pan and a volume, then play. */
s32 sdfSndSePlayPositional(s32 id, s32 angle, s32 dist, s32 radius)
{
    sdfSndSeReq_t req;
    s32 a;
    s32 sign;
    s32 level;

    a = angle % 360;
    req.id = id;
    if (a >= 181) {
        a -= 360;
    }
    if (a < -180) {
        a += 360;
    }
    sign = 1;
    if (a < 0) {
        sign = -1;
        a = -a;
    }
    level = 100;
    if (a >= 45) {
        if (a >= 91) {
            req.pan = 63 - (180 - a) * 70 * sign / 100;
            level = 75 - ((a - 90) >> 2);
        } else {
            level = 100 - (a - 45) / 2;
            req.pan = 63 - a * 70 * sign / 100;
        }
    } else {
        req.pan = 63 - a * 70 * sign / 100;
    }
    if (dist < 11) {
        req.pan = ((req.pan - 63) >> 1) + 63;
    } else {
        level = level * sdfSndDistanceAtten(dist, radius) / 100;
    }
    req.vol = 127 * level / 100;
    return sdfSndSePlay(&req);
}
/* ENDF sdfSndSePlayPositional */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSePlayPositional);
#endif

#ifdef NON_MATCHING
/* 1 instruction longer than the retail code. */
/* FUNC sdfSndSetListener */
/* Remember where the listener is and which way it faces; the sequence thread
 * re-pans the positional sounds on the next tick. */
s32 sdfSndSetListener(sdfSndListenerReq_t *req)
{
    s32 angle;
    s32 posZ;
    s32 atZ;
    s32 posY;
    s32 dx;

    if (sdfSndListenerX == req->posX && sdfSndListenerY == req->posY &&
        sdfSndListenerZ == req->posZ) {
        angle = sdfSndAtan2Deg(req->atX - sdfSndListenerX, sdfSndListenerZ - req->atZ);
        if (sdfSndListenerAngle == angle) {
            return angle;
        }
    }
    posZ = req->posZ;
    atZ = req->atZ;
    posY = req->posY;
    dx = req->atX - req->posX;
    sdfSndListenerX = req->posX;
    sdfSndListenerY = posY;
    sdfSndListenerZ = posZ;
    sdfSndListenerAngle = sdfSndAtan2Deg(dx, posZ - atZ);
    sdfSndListenerDirty = 1;
    return 1;
}
/* ENDF sdfSndSetListener */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSetListener);
#endif
INCLUDE_ASM("SDFSDMAN", sdfSndSePlay3d);

/* FUNC sdfSndIsBankLoaded */
s32 sdfSndIsBankLoaded(s32 kind)
{
    s32 i;

#ifdef FIX_BUGS
    for (i = 0; i < SDF_SND_KINDS; i++) {
        s32 id;

        /* Past the last stat retail reads eeAddr, then size and unk1D0 as ids. */
        if (kind + i < SDF_SND_KINDS) {
            id = sdfSndWorkArea->stat[kind + i].id;
        } else if (kind + i == SDF_SND_KINDS) {
            id = (s32)sdfSndWorkArea->eeAddr;
        } else {
            id = (s32)sdfSndWorkArea->unk1D0;
        }
        if (id >= 0) {
            return 1;
        }
    }
    return 0;
#else
    for (i = 0; i < SDF_SND_KINDS; i++) {
        if (sdfSndWorkArea->stat[kind + i].id >= 0) {
            return 1;
        }
    }
    return 0;
#endif
}
/* ENDF sdfSndIsBankLoaded */
INCLUDE_ASM("SDFSDMAN", sdfSndLoadSmg);

/* FUNC sdfSndSmgLoadThread */
/* Waits on its semaphore and loads whatever path the EE last set. */
void sdfSndSmgLoadThread(void *arg)
{
    s32 ret;

    sdfSndSmgSemaParam.initial = 0;
    sdfSndSmgSemaParam.max = 1;
    sdfSndSmgSemaParam.option = 0;
    sdfSndSmgSema = thsemap_CreateSema(&sdfSndSmgSemaParam);
    if (sdfSndSmgSema <= 0) {
        stdio_printf(aSdfsnddrvCreat);
        thbase_ExitThread();
    }
    while (1) {
        ret = thsemap_WaitSema(sdfSndSmgSema);
        if (ret != 0) {
            stdio_printf(aSdfsnddrvWaits, ret);
        }
        sdfSndLoadSmg(sdfSndSmgPath, 0);
    }
}
/* ENDF sdfSndSmgLoadThread */
INCLUDE_ASM("SDFSDMAN", sdfSndSetBankAddr);
INCLUDE_ASM("SDFSDMAN", sdfSndStopByGroup);
INCLUDE_ASM("SDFSDMAN", sdfSndUnloadBank);
INCLUDE_ASM("SDFSDMAN", p2sdIrxSpuIopMap);

/* FUNC sdfSndFadeOutById */
INCLUDE_ASM("SDFSDMAN", sdfSndFadeOutById);
/* ENDF sdfSndFadeOutById */

/* FUNC sdfSndRequestBgmSwitch */
/* The next controller 0 on the switch channel starts this sequence. */
s32 sdfSndRequestBgmSwitch(s32 midi)
{
    if (midi >= 0) {
        sdfSndBgmSwitchMidi = midi;
    }
    sdfSndBgmSwitchReq = 1;
    return 1;
}
/* ENDF sdfSndRequestBgmSwitch */

/* FUNC sdfSndAllocBgmSlot */
/* An idle one of the three sequence slots, else the one playing the oldest
 * sound. The slot used last time is skipped so a restart is audible. */
s32 sdfSndAllocBgmSlot(void)
{
    s32 i;
    s32 slot;
    u32 serial;
    s32 state;

    slot = -1;
    serial = -1;
    for (i = 0; i < 3; i++) {
        if ((sdfSndMidiWork.tbl->seq[i].slot->flags & 2) == 0 && i != sdfSndLastBgmSlot &&
            serial >= sdfSndSlots[i].serial) {
            serial = sdfSndSlots[i].serial;
            slot = i;
        }
    }
    if (slot >= 0) {
        sdfSndLastBgmSlot = slot;
        return slot;
    }
    serial = -1;
    slot = -1;
    for (i = 0; i < 3; i++) {
        if (sdfSndSlots[i].serial < serial && i != sdfSndLastBgmSlot &&
            sdfSndSlots[i].is3d == 0) {
            serial = sdfSndSlots[i].serial;
            slot = i;
        }
    }
    if (slot < 0) {
        return -1;
    }
    intrman_CpuSuspendIntr(&state);
    modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, slot, 0);
    intrman_CpuResumeIntr(state);
    sdfSndLastBgmSlot = slot;
    return slot;
}
/* ENDF sdfSndAllocBgmSlot */
INCLUDE_ASM("SDFSDMAN", sdfSndAllocSeSlot);
INCLUDE_ASM("SDFSDMAN", sdfSndSetSeqVolume);

/* FUNC sdfSndFadeOutAll */
s32 sdfSndFadeOutAll(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (sdfSndSlots[i].id >= 0 &&
            (sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0 &&
            sdfSndSlots[i].volStep >= 0) {
            sdfSndSlots[i].volStep = -64;
            sdfSndSlots[i].fadeIn = 1;
            sdfSndSlots[i].unk34[0] = 1;
        }
    }
}
/* ENDF sdfSndFadeOutAll */

/* FUNC sdfSndFadeInAll */
s32 sdfSndFadeInAll(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (sdfSndSlots[i].id >= 0 &&
            (sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0 &&
            sdfSndSlots[i].unk34[0] == 1) {
            sdfSndSlots[i].volStep = 64;
            sdfSndSlots[i].fadeIn = 0;
            sdfSndSlots[i].unk34[0] = 0;
        }
    }
}
/* ENDF sdfSndFadeInAll */

#ifdef NON_MATCHING
/* 2 instructions longer than the retail code. */
/* FUNC sdfSndSifCmdHandler */
/* Copy an EE command packet into the ring and wake the RPC thread. A full
 * ring drops the oldest entry and counts the overflow. */
s32 sdfSndSifCmdHandler(sdfSndCmd_t *pkt, void *harg)
{
    s32 ret;

    sysclib_memcpy(&sdfSndCmdRing[(u8)(sdfSndCmdHead + sdfSndCmdCount)], pkt,
                   16 * ((pkt->opt >> 28) + 1));
    ret = thsemap_iSignalSema(sdfSndCmdSema);
    if (ret == 0) {
        return ++sdfSndCmdCount;
    }
    if (ret == -420) {
        sdfSndCmdHead = (u8)(sdfSndCmdHead + 1);
        sdfSndCmdOverflow++;
        return sdfSndCmdHead;
    }
    return -420;
}
/* ENDF sdfSndSifCmdHandler */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSifCmdHandler);
#endif

#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndCreateRpcSema */
s32 sdfSndCreateRpcSema(void)
{
    s32 sema;

    sdfSndRpcSemaParam.initial = 1;
    sdfSndRpcSemaParam.max = 2;
    sdfSndRpcSemaParam.option = 0;
    sema = thsemap_CreateSema(&sdfSndRpcSemaParam);
    sdfSndRpcSema = sema;
    if (sema <= 0) {
        stdio_printf(aSdfsnddrvCreat);
        return thbase_ExitThread();
    }
    return sema;
}
/* ENDF sdfSndCreateRpcSema */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndCreateRpcSema);
#endif

/* FUNC sdfSndLoadVolTables */
/* The EE writes the volume tables into the work area; take a copy. */
s32 sdfSndLoadVolTables(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        sdfSndVolTblSeq[i] = sdfSndWorkArea->volTable[0][i];
    }
    for (i = 0; i < 128; i++) {
        sdfSndVolTblBgm[i] = sdfSndWorkArea->volTable[1][i];
        sdfSndVolTblSe[i] = sdfSndWorkArea->volTable[2][i];
    }
    for (i = 0; i < 32; i++) {
        sdfSndVolTblVoice[i] = sdfSndWorkArea->volTable[3][i];
        sdfSndVolTblKind4[i] = sdfSndWorkArea->volTable[4][i];
    }
    for (i = 0; i < 2; i++) {
        sdfSndVolTblCore0[i] = sdfSndWorkArea->volTable[5][i];
        sdfSndVolTblCore1[i] = sdfSndWorkArea->volTable[6][i];
        sdfSndVolTblKind7[0][i] = sdfSndWorkArea->volTable[7][i];
        sdfSndVolTblKind7[1][i] = sdfSndWorkArea->volTable[8][i];
        sdfSndVolTblKind7[2][i] = sdfSndWorkArea->volTable[9][i];
        sdfSndVolTblKind7[3][i] = sdfSndWorkArea->volTable[10][i];
        sdfSndVolTblKind7[4][i] = sdfSndWorkArea->volTable[11][i];
        sdfSndVolTblKind7[5][i] = sdfSndWorkArea->volTable[12][i];
    }
}
/* ENDF sdfSndLoadVolTables */

/* FUNC sdfSndStoreVolTables */
s32 sdfSndStoreVolTables(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        sdfSndWorkArea->volTable[0][i] = sdfSndVolTblSeq[i];
    }
    for (i = 0; i < 128; i++) {
        sdfSndWorkArea->volTable[1][i] = sdfSndVolTblBgm[i];
        sdfSndWorkArea->volTable[2][i] = sdfSndVolTblSe[i];
    }
    for (i = 0; i < 32; i++) {
        sdfSndWorkArea->volTable[3][i] = sdfSndVolTblVoice[i];
        sdfSndWorkArea->volTable[4][i] = sdfSndVolTblKind4[i];
    }
    for (i = 0; i < 2; i++) {
        sdfSndWorkArea->volTable[5][i] = sdfSndVolTblCore0[i];
        sdfSndWorkArea->volTable[6][i] = sdfSndVolTblCore1[i];
        sdfSndWorkArea->volTable[7][i] = sdfSndVolTblKind7[0][i];
        sdfSndWorkArea->volTable[8][i] = sdfSndVolTblKind7[1][i];
        sdfSndWorkArea->volTable[9][i] = sdfSndVolTblKind7[2][i];
        sdfSndWorkArea->volTable[10][i] = sdfSndVolTblKind7[3][i];
        sdfSndWorkArea->volTable[11][i] = sdfSndVolTblKind7[4][i];
        sdfSndWorkArea->volTable[12][i] = sdfSndVolTblKind7[5][i];
    }
    return p2sdIrxSpuIopMap();
}
/* ENDF sdfSndStoreVolTables */

/* FUNC sdfSndKeepAliveThread */
/* The IOP idles its clock without a running thread, which upsets the timing
 * of everything else; this one never sleeps. */
void sdfSndKeepAliveThread(void *arg)
{
    while (1) {
        thbase_DelayThread(10000);
    }
}
/* ENDF sdfSndKeepAliveThread */

/* FUNC sdfSndStartKeepAliveThread */
s32 sdfSndStartKeepAliveThread(void)
{
    iop_thread_t thp;
    s32 tid;

    thp.attr = 0x2000000;
    thp.thread = sdfSndKeepAliveThread;
    thp.priority = 1;
    thp.stacksize = 2048;
    tid = thbase_CreateThread(&thp);
    return thbase_StartThread(tid, NULL);
}
/* ENDF sdfSndStartKeepAliveThread */

#ifdef NON_MATCHING
/* The command switch compiles to the jump table at jpt_4188 in
 * asm/irx/SDFSDMAN/rodata.s, which a C switch cannot reproduce in place. */
/* FUNC sdfSndDrvRpcInit */
/* The driver's main loop: wait for a command packet from the EE, run it and
 * acknowledge it. Command 1 (quit) is the only way out. */
s32 sdfSndDrvRpcInit(void *arg)
{
    sdfSndCmd_t *pkt;
    u32 opt;
    s32 arg4;
    s32 bank;
    s32 vol;
    u16 depth;
    s32 state;

    sdfSndStartKeepAliveThread();
    sdfSndCreateRpcSema();
    sdfSndCmdSemaParam.initial = 0;
    sdfSndCmdSemaParam.max = 256;
    sdfSndCmdSemaParam.option = 0;
    sdfSndCmdSema = thsemap_CreateSema(&sdfSndCmdSemaParam);
    if (sdfSndCmdSema <= 0) {
        stdio_printf(aSdfsnddrvCreat);
        thbase_ExitThread();
    }
    sifcmd_SifInitCmd();
    intrman_CpuSuspendIntr(&state);
    sifcmd_SifSetCmdBuffer(sdfSndSifCmdBuffer, 10);
    sifcmd_SifAddCmdHandler(0, (void *(*)(void *, void *))sdfSndSifCmdHandler, NULL);
    intrman_CpuResumeIntr(state);
    sdfSndAckPacket.code = 31;
    sdfSndAckPacket.arg = 1;
    sifcmd_SifSendCmd(0x80000001, &sdfSndAckPacket, 24, NULL, NULL, 0);
    sdfSndEeRpcSema = thsemap_CreateSema(&sdfSndEeRpcSemaParam);
    if (sifcmd_SifBindRpc(&sdfSndEeRpcClient, 0x54524E53, 0) != 0) {
        stdio_printf(aSdfsnddrvScesi);
    }
    while (1) {
        do {
            thsemap_WaitSema(sdfSndCmdSema);
            if ((s16)sdfSndCmdCount > 0) {
                break;
            }
            stdio_printf(aSdfsnddrvIfcou, (s16)sdfSndCmdCount);
        } while (1);
        pkt = &sdfSndCmdRing[(s16)sdfSndCmdHead];
        opt = pkt->opt;
        arg4 = (opt >> 16) & 0xF;
        switch ((u8)(opt >> 20)) {
        case 0:
            sdfSndInitSynth(*(sdfSndWorkArea_t **)pkt->data);
            break;
        case 1:
            sdfSndQuitSynth();
            return 0;
        case 2:
            sdfSndBgmPlay((sdfSndBgmReq_t *)pkt->data);
            break;
        case 3:
            sdfSndBgmStop((s32 *)pkt->data);
            break;
        case 4:
            sdfSndStopAll(0);
            break;
        case 9:
            sdfSndSePlay((sdfSndSeReq_t *)pkt->data);
            break;
        case 0xA:
            sdfSndLoadSmg((const char *)pkt->data, 0);
            break;
        case 0xB:
            bank = sdfSndFindSlotById(*(s32 *)pkt->data);
            if (bank < 0) {
                break;
            }
            sdfSndUnloadBank(bank);
            p2sdIrxSpuIopMap();
            break;
        case 0xC0:
            p2sdIrxSpuIopMap();
            break;
        case 0xD:
            sdfSndFadeOutById((s32 *)pkt->data);
            break;
        case 0xE:
            sdfSndRequestBgmSwitch(arg4 - 1);
            break;
        case 0xF:
            vol = 8 * arg4;
            if ((vol & 8) != 0) {
                vol |= 7;
            }
            sdfAdpcmStreamPlay((const char *)pkt->data, vol, 0);
            break;
        case 0x10:
            sdfAdpcmStop();
            break;
        case 0x11:
            vol = 8 * arg4;
            if ((vol & 8) != 0) {
                vol |= 7;
            }
            sdfAdpcmSetVolume(vol);
            break;
        case 0x12:
            vol = 8 * arg4;
            if ((vol & 8) != 0) {
                vol |= 7;
            }
            sdfAdpcmStreamPlay((const char *)pkt->data, vol, 1);
            break;
        case 0x13:
            sdfSndBgmResume((sdfSndBgmReq_t *)pkt->data);
            break;
        case 0x14:
            depth = arg4 << 10;
            if ((opt & 0x10000) != 0) {
                depth |= 0x3FF;
            }
            libsd_sceSdSetParam(0x980, depth);
            libsd_sceSdSetParam(0xA80, depth);
            libsd_sceSdSetParam(0x981, depth);
            libsd_sceSdSetParam(0xA81, depth);
            break;
        case 0x15:
            sdfSndSetSeqVolume(arg4);
            break;
        case 0x16:
            sdfSndSetListener((sdfSndListenerReq_t *)pkt->data);
            break;
        case 0x17:
            sdfSndSePlay3d((sdfSndSe3dReq_t *)pkt->data);
            break;
        case 0x18:
            sdfSndFadeOutAll();
            break;
        case 0x19:
            sdfSndFadeInAll();
            break;
        case 0x1A:
            sdfSndStopAll(1);
            break;
        case 0x1B:
            sdfSndLoadSmg(sdfSndSmgPathPtr, 1);
            break;
        case 0x1C:
            intrman_CpuSuspendIntr(&state);
            modhsyn_e18(arg4 == 0);
            intrman_CpuResumeIntr(state);
            break;
        case 0x1D:
            sdfAdpcmFadeOut(arg4 + 1);
            break;
        case 0x1F:
            sdfSndLoadVolTables();
            break;
        case 0x20:
            sdfSndStoreVolTables();
            break;
        case 0x21:
            sdfSndStopAll(2);
            break;
        default:
            stdio_printf(aSdfsnddrvUnkno, (u8)(opt >> 20), (u16)pkt->opt);
            sdfSndPostError(14);
            break;
        }
        sdfSndAckPacket.code = 30;
        sdfSndAckPacket.arg = pkt->tag;
        intrman_CpuSuspendIntr(&state);
        sdfSndCmdCount--;
        sdfSndCmdHead = (u8)(sdfSndCmdHead + 1);
        intrman_CpuResumeIntr(state);
        sifcmd_SifSendCmd(0x80000001, &sdfSndAckPacket, 24, NULL, NULL, 0);
    }
}
/* ENDF sdfSndDrvRpcInit */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndDrvRpcInit);
#endif

ALIGN_TEXT(4);
#ifdef NON_MATCHING
/* 8 instructions short of the retail code. */
/* FUNC sdfSndCallEeRpc */
/* Ask the EE to do something for us: read a block of the stream, open a
 * file, and so on. */
s32 sdfSndCallEeRpc(s32 arg, void *buf, s32 len, u32 fno, u32 *reply)
{
    sdfSndEeReq_t send;
    u32 recv[4];
    u32 a;
    u32 b;
    u32 c;

    thsemap_WaitSema(sdfSndEeRpcSema);
    send.buf = buf;
    send.arg = arg;
    send.len = len;
    while (sifcmd_SifCheckStatRpc(&sdfSndEeRpcClient)) {
    }
    sifcmd_SifCallRpc(&sdfSndEeRpcClient, fno, 0, &send, 16, recv, 16, NULL, NULL);
    if (reply != NULL) {
        a = recv[1];
        b = recv[2];
        c = recv[3];
        reply[0] = recv[0];
        reply[1] = a;
        reply[2] = b;
        reply[3] = c;
    }
    return thsemap_SignalSema(sdfSndEeRpcSema);
}
/* ENDF sdfSndCallEeRpc */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndCallEeRpc);
#endif

ALIGN_TEXT(4);
/* FUNC sdfAdpcmFadeTick */
/* One step of the stream fade out, called once per sequence tick. */
s32 sdfAdpcmFadeTick(void)
{
    s32 frac;

    if (sdfAdpcm.playing == 0 || sdfAdpcm.fadeStep == 0) {
        return sdfAdpcm.playing;
    }
    frac = sdfAdpcm.fadeFrac + sdfAdpcm.fadeStep;
    sdfAdpcm.fadeFrac = frac;
    if (frac >= 16) {
        sdfAdpcm.fadeFrac = frac & 0xF;
        sdfAdpcm.volume -= (frac >> 4) & 0xF;
    }
    if (sdfAdpcm.volume > 0) {
        return sdfAdpcmSetVolume(sdfAdpcm.volume);
    }
    return sdfAdpcmStop();
}
/* ENDF sdfAdpcmFadeTick */

/* FUNC sdfAdpcmFadeOut */
s32 sdfAdpcmFadeOut(s32 step)
{
    if (sdfAdpcm.playing) {
        sdfAdpcm.fadeStep = step;
    }
}
/* ENDF sdfAdpcmFadeOut */
INCLUDE_ASM("SDFSDMAN", sdfAdpcmStop);

#ifdef NON_MATCHING
/* 1 instruction short of the retail code. */
/* FUNC sdfAdpcmUploadBlock */
/* Send the block the reader just filled to the SPU2 and point the voice at
 * it. Stereo splits the 512 bytes into two 256 byte halves. */
s32 sdfAdpcmUploadBlock(void)
{
    u32 spuAddr;
    u16 entry;
    s32 off;
    s32 next;

    if (sdfAdpcm.stereo) {
        while (libsd_sceSdVoiceTrans(sdfAdpcm.core, 0,
                                     (u8 *)(sdfAdpcm.iopBuf + ((s8)sdfAdpcm.xferIdx << 9)),
                                     sdfAdpcm.spuAddr + (sdfAdpcm.xferBlk << 8), 0x100) < 0) {
        }
        libsd_sceSdVoiceTransStatus(sdfAdpcm.core, 1);
        while (libsd_sceSdVoiceTrans(sdfAdpcm.core, 0,
                                     (u8 *)(sdfAdpcm.iopBuf + ((s8)sdfAdpcm.xferIdx << 9) + 256),
                                     sdfAdpcm.spuAddr + (sdfAdpcm.xferBlk << 8) + 768,
                                     0x100) < 0) {
        }
        libsd_sceSdVoiceTransStatus(sdfAdpcm.core, 1);
        spuAddr = sdfAdpcm.spuAddr;
        entry = (s8)sdfAdpcm.coreId | 0x1F00;
        off = sdfAdpcm.xferBlk << 8;
    } else {
        while (libsd_sceSdVoiceTrans(sdfAdpcm.core, 0,
                                     (u8 *)(sdfAdpcm.iopBuf + ((s8)sdfAdpcm.xferIdx << 9)),
                                     sdfAdpcm.spuAddr + (sdfAdpcm.xferBlk << 9), 0x200) < 0) {
        }
        libsd_sceSdVoiceTransStatus(sdfAdpcm.core, 1);
        spuAddr = sdfAdpcm.spuAddr;
        entry = (s8)sdfAdpcm.coreId | 0x1F00;
        off = sdfAdpcm.xferBlk << 9;
    }
    libsd_sceSdSetAddr(entry, spuAddr + off);
    next = sdfAdpcm.xferBlk + 1;
    sdfAdpcm.xferIdx = (sdfAdpcm.xferIdx + 1) & 1;
    sdfAdpcm.xferBlk = next % 3;
}
/* ENDF sdfAdpcmUploadBlock */
#else
INCLUDE_ASM("SDFSDMAN", sdfAdpcmUploadBlock);
#endif

/* FUNC sdfAdpcmSpu2IntrHandler */
s32 sdfAdpcmSpu2IntrHandler(void *arg)
{
    libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 0);
    sdfAdpcm.intrState = 2;
    thsemap_iSignalSema(sdfAdpcmIntrSema);
    libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 1);
    return 0;
}
/* ENDF sdfAdpcmSpu2IntrHandler */
INCLUDE_ASM("SDFSDMAN", sdfAdpcmFillBuffer);
INCLUDE_ASM("SDFSDMAN", sdfAdpcmSetVolume);

/* FUNC sdfAdpcmStreamPlay */
s32 sdfAdpcmStreamPlay(const char *path, s32 vol, s8 onMemory)
{
    if (sdfAdpcm.playing) {
        stdio_printf(aSdfadpcmAdpcmN);
    } else {
        sdfAdpcm.volume = vol;
        sdfAdpcm.fadeStep = 0;
        sdfAdpcm.onMemory = onMemory;
        thsemap_SignalSema(sdfAdpcmPlaySema);
    }
}
/* ENDF sdfAdpcmStreamPlay */
INCLUDE_ASM("SDFSDMAN", sdfAdpcmStreamInit);

ALIGN_TEXT(4);
IRX_IMPORT_TABLE(libsd, 1, 5);
IRX_IMPORT(libsd, sceSdInit, 0x4);
IRX_IMPORT(libsd, sceSdSetParam, 0x5);
IRX_IMPORT(libsd, sceSdSetSwitch, 0x7);
IRX_IMPORT(libsd, sceSdGetSwitch, 0x8);
IRX_IMPORT(libsd, sceSdSetAddr, 0x9);
IRX_IMPORT(libsd, sceSdSetCoreAttr, 0xb);
IRX_IMPORT(libsd, sceSdVoiceTrans, 0x11);
IRX_IMPORT(libsd, sceSdVoiceTransStatus, 0x13);
IRX_IMPORT(libsd, sceSdSetEffectAttr, 0x17);
IRX_IMPORT(libsd, sceSdSetSpu2IntrHandler, 0x1b);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(modhsyn, 1, 4);
IRX_IMPORT(modhsyn, sceHSyn_Init, 0x4);
IRX_IMPORT(modhsyn, e5, 0x5);
IRX_IMPORT(modhsyn, sceHSyn_Load, 0x6);
IRX_IMPORT(modhsyn, sceHSyn_VoiceTrans, 0x7);
IRX_IMPORT(modhsyn, e8, 0x8);
IRX_IMPORT(modhsyn, e13, 0xd);
IRX_IMPORT(modhsyn, e15, 0xf);
IRX_IMPORT(modhsyn, e18, 0x12);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(modmidi, 1, 5);
IRX_IMPORT(modmidi, sceMidi_Init, 0x4);
IRX_IMPORT(modmidi, e5, 0x5);
IRX_IMPORT(modmidi, sceMidi_Load, 0x6);
IRX_IMPORT(modmidi, sceMidi_SelectMidi, 0xe);
IRX_IMPORT(modmidi, sceMidi_MidiPlaySwitch, 0xf);
IRX_IMPORT(modmidi, sceMidi_MidiSetLocation, 0x10);
IRX_IMPORT(modmidi, sceMidi_MidiSetVolume, 0x11);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(modmsin, 1, 1);
IRX_IMPORT(modmsin, sceMSIn_PutMsg, 0x7);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sdfiopfl, 1, 1);
IRX_IMPORT(sdfiopfl, sdfIopFileReadOpen, 0x4);
IRX_IMPORT(sdfiopfl, sdfIopFileGetFileSize, 0x5);
IRX_IMPORT(sdfiopfl, sdfIopFileRead, 0x6);
IRX_IMPORT(sdfiopfl, sdfIopFileClose, 0x7);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sysmem, 1, 1);
IRX_IMPORT(sysmem, FreeSysMemory, 0x5);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(intrman, 1, 2);
IRX_IMPORT(intrman, CpuSuspendIntr, 0x11);
IRX_IMPORT(intrman, CpuResumeIntr, 0x12);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(loadcore, 1, 3);
IRX_IMPORT(loadcore, FlushDcache, 0x5);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sifcmd, 1, 1);
IRX_IMPORT(sifcmd, SifInitCmd, 0x4);
IRX_IMPORT(sifcmd, SifSetCmdBuffer, 0x8);
IRX_IMPORT(sifcmd, SifAddCmdHandler, 0xa);
IRX_IMPORT(sifcmd, SifSendCmd, 0xc);
IRX_IMPORT(sifcmd, SifInitRpc, 0xe);
IRX_IMPORT(sifcmd, SifBindRpc, 0xf);
IRX_IMPORT(sifcmd, SifCallRpc, 0x10);
IRX_IMPORT(sifcmd, SifCheckStatRpc, 0x12);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sifman, 1, 1);
IRX_IMPORT(sifman, SifSIF2Init, 0x5);
IRX_IMPORT(sifman, sceSifSetDma, 0x7);
IRX_IMPORT(sifman, sceSifDmaStat, 0x8);
IRX_IMPORT(sifman, SifSetIOPrcvaddr, 0x1d);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(stdio, 1, 3);
IRX_IMPORT(stdio, printf, 0x4);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sysclib, 1, 3);
IRX_IMPORT(sysclib, memcpy, 0xc);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(thbase, 1, 2);
IRX_IMPORT(thbase, CreateThread, 0x4);
IRX_IMPORT(thbase, StartThread, 0x6);
IRX_IMPORT(thbase, ExitThread, 0x8);
IRX_IMPORT(thbase, SleepThread, 0x18);
IRX_IMPORT(thbase, iWakeupThread, 0x1a);
IRX_IMPORT(thbase, iCancelWakeupThread, 0x1c);
IRX_IMPORT(thbase, DelayThread, 0x21);
IRX_IMPORT(thbase, USec2SysClock, 0x27);
IRX_IMPORT(thbase, e43, 0x2b);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(thsemap, 1, 1);
IRX_IMPORT(thsemap, CreateSema, 0x4);
IRX_IMPORT(thsemap, SignalSema, 0x6);
IRX_IMPORT(thsemap, iSignalSema, 0x7);
IRX_IMPORT(thsemap, WaitSema, 0x8);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(timrman, 1, 3);
IRX_IMPORT(timrman, AllocHardTimer, 0x4);
IRX_IMPORT(timrman, FreeHardTimer, 0x6);
IRX_IMPORT(timrman, SetTimerHandler, 0x14);
IRX_IMPORT(timrman, SetupHardTimer, 0x16);
IRX_IMPORT(timrman, StartHardTimer, 0x17);
IRX_IMPORT(timrman, StopHardTimer, 0x18);
IRX_IMPORT_TABLE_END;
ALIGN_TEXT(4);
