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
#ifdef NON_MATCHING
/* The switch compiles to a jump table in asm/irx/SDFSDMAN/rodata.s, which a
 * C switch cannot reproduce in place. */
/* FUNC sdfSndGetVolTable */
/* The volume table for a bank kind, and how many entries it has. */
u8 *sdfSndGetVolTable(s32 kind, s32 *count)
{
    u8 *tbl;
    s32 unused;

    tbl = NULL;
    if (count == NULL) {
        count = &unused;
    }
    *count = 0;
    if (kind < 0) {
        return NULL;
    }
    switch (kind) {
    case 0:
        tbl = sdfSndVolTblSeq;
        *count = 32;
        break;
    case 1:
        tbl = sdfSndVolTblBgm;
        *count = 128;
        break;
    case 2:
        tbl = sdfSndVolTblSe;
        *count = 128;
        break;
    case 3:
        tbl = sdfSndVolTblVoice;
        *count = 32;
        break;
    case 4:
        tbl = sdfSndVolTblKind4;
        *count = 32;
        break;
    case 5:
        tbl = sdfSndVolTblCore0;
        *count = 2;
        break;
    case 6:
        tbl = sdfSndVolTblCore1;
        *count = 2;
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        tbl = sdfSndVolTblKind7[kind - 7];
        *count = 2;
        break;
    }
    return tbl;
}
/* ENDF sdfSndGetVolTable */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndGetVolTable);
#endif

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
#ifdef NON_MATCHING
/* The synthesiser calls this for every MIDI message. Controller 0 on the
 * pending switch channel picks the next BGM, controller 15 records where the
 * sequence is so a resume can come back to it.
 * Matches but for the register the channel number lands in. */
s32 sdfSndMidiCbMessage(u32 msg, s32 slot)
{
    s32 val;

    val = (msg >> 8) & 0xFF;
    if ((msg & 0xF0) == 0xD0) {
        switch (msg & 0xF) {
        case 0:
            if (sdfSndBgmSwitchReq != 0) {
                if (sdfSndBgmSwitchMidi < 0) {
                    if (val == 127) {
                        sdfSndBgmSwitchReq = 0;
                        break;
                    }
                    sdfSndBgmSwitchMidi = val;
                }
                sdfSndBgmSwitchSlot = slot;
            }
            break;
        case 15:
            sdfSndSlots[slot].location = sdfSndMidiSlotWork[slot].location;
            break;
        }
        return 0;
    }
    return msg;
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndMidiCbMessage);
#endif

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
#ifdef NON_MATCHING
/* 3 instructions short of the retail code. */
/* FUNC sdfSndStopAll */
/* which: 0 every slot, 1 the three sequence slots, anything else the rest. */
s32 sdfSndStopAll(s32 which)
{
    s32 first;
    s32 last;
    s32 i;
    s32 state;

    sdfSndBgmSwitchReq = 0;
    sdfSndBgmSwitchSlot = -1;
    sdfSndBgmSwitchMidi = -1;
    if (which == 0) {
        first = 0;
        last = 16;
    } else if (which == 1) {
        first = 0;
        last = 3;
    } else {
        first = 3;
        last = 16;
    }
    for (i = first; i < last; i++) {
        if ((sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0) {
            if (i < 3) {
                sdfSndSaveResumePoint(i);
            }
            intrman_CpuSuspendIntr(&state);
            modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, i, 0);
            intrman_CpuResumeIntr(state);
            sdfSndSlots[i].vol = 32512;
            sdfSndSlots[i].is3d = 0;
            sdfSndSlots[i].fadeIn = 0;
            sdfSndSlots[i].volStep = 0;
            sdfSndSlots[i].location = -1;
            sdfSndSlots[i].id = -1;
            sdfSndSlots[i].unk34[0] = 0;
        }
    }
}
/* ENDF sdfSndStopAll */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndStopAll);
#endif
#ifdef NON_MATCHING
/* 1 instruction short of the retail code. */
/* FUNC sdfSndInitSynth */
/* Takes the work area the EE allocated, then brings up the sequence thread,
 * its timer, the synthesiser and the MIDI front end. */
s32 sdfSndInitSynth(sdfSndWorkArea_t *work)
{
    iop_thread_t thp;
    s32 param[2];
    s32 i;

    for (i = 0; i < 16; i++) {
        sdfSndSlots[i].vol = 32512;
        sdfSndSlots[i].volStep = 0;
        sdfSndSlots[i].fadeIn = 0;
        sdfSndSlots[i].id = -1;
        sdfSndSlots[i].location = -1;
        sdfSndSlots[i].serial = 0;
        sdfSndSlots[i].unk34[0] = 0;
    }
    sdfSndResumeIdx = 0;
    for (i = 0; i < 32; i++) {
        sdfSndResume[i].id = -1;
        sdfSndResume[i].location = -1;
    }
    sdfSndListenerAngle = 0;
    sdfSndListenerZ = 0;
    sdfSndListenerY = 0;
    sdfSndListenerX = 0;
    sdfSndListenerDirty = 0;
    sdfSndWorkArea = work;
    work->errCount = 0;
    work->errCode = -1;
    sdfSndInitSpu2();

    thp.attr = 0x2000000;
    thp.thread = sdfSndSeqThread;
    thp.priority = 30;
    thp.stacksize = 2048;
    thp.option = 0;
    sdfSndSeqThreadId = thbase_CreateThread(&thp);
    thbase_StartThread(sdfSndSeqThreadId, NULL);
    sdfSndTimer.thid = sdfSndSeqThreadId;
    sdfSndTimerSetup(&sdfSndTimer);
    sdfSndInitWorkAreas();
    if (modhsyn_sceHSyn_Init(&sdfSndHsynWork, 4167) != 0) {
        stdio_printf(aSdfsnddrvScehs);
        return sdfSndPostError(9);
    } else {
        param[0] = 3;
        param[1] = 0;
        modhsyn_e8(param);
        modhsyn_e18(1);
        sdfSndSeqVolume = 127;
        if (modmidi_sceMidi_Init(&sdfSndMidiWork, 4167) != 0) {
            stdio_printf(aSdfsnddrvScemi_2);
            return sdfSndPostError(10);
        } else {
            return sdfSndTimerStart(&sdfSndTimer);
        }
    }
}
/* ENDF sdfSndInitSynth */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndInitSynth);
#endif

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
#ifdef NON_MATCHING
/* 9 instructions short of the retail code. */
/* FUNC sdfSndBgmPlay */
/* Start a sequence in one of the three BGM slots. A volume of 0x100 means
 * fade in from silence, anything over 127 plays at full. */
s32 sdfSndBgmPlay(sdfSndBgmReq_t *req)
{
    s32 id;
    s32 bank;
    s32 seq;
    s32 slot;
    s32 vol;
    s32 ret;
    s32 i;
    s32 state;

    id = req->id;
    bank = sdfSndFindSlotById(req->id);
    if ((u32)(bank - 1) < 2) {
        sdfSndBankAltSlot = bank;
    }
    for (i = 0; i < 16; i++) {
        if (sdfSndSlots[i].id == id) {
            ret = sdfSndSlots[i].volStep;
            if (ret < 0) {
                sdfSndSlots[i].volStep = 32;
            }
            return ret;
        }
    }
    ret = sdfSndIsSlotMuted(id);
    if (ret != 0) {
        return ret;
    }
    seq = (u16)req->id;
    if (bank < 0) {
        return sdfSndPostError(18);
    }
    slot = sdfSndAllocBgmSlot();
    if (slot < 0) {
        return -1;
    }
    sdfSndBgmSwitchReq = 0;
    sdfSndBgmSwitchSlot = -1;
    sdfSndBgmSwitchMidi = -1;
    intrman_CpuSuspendIntr(&state);
    if ((sdfSndMidiWork.tbl->seq[slot].slot->flags & 2) != 0) {
        if (modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, slot, 0) != 0) {
            intrman_CpuResumeIntr(state);
            return stdio_printf(aSdfsnddrvScemi_3);
        }
    }
    if (sdfSndMidiSelect(slot, seq, bank, sdfSndWorkArea->progData[bank].ptr,
                         req->location) != 0) {
        return intrman_CpuResumeIntr(state);
    }
    ret = modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, slot, 1);
    sdfSndPutMsg(slot);
    if (ret != 0) {
        intrman_CpuResumeIntr(state);
        stdio_printf(aSdfsnddrvPlayS);
        return sdfSndPostError(12);
    }
    sdfSndSlots[slot].volStep = 0;
    sdfSndSlots[slot].fadeIn = 0;
    sdfSndSlots[slot].is3d = 0;
    sdfSndSlots[slot].location = -1;
    sdfSndSlots[slot].id = id;
    sdfSndSlots[slot].group = bank;
    sdfSndSlots[slot].unk34[0] = 0;
    sdfSndSlots[slot].serial = sdfSndSlotSerialNext;
    vol = req->vol;
    sdfSndSlotSerialNext++;
    if (vol >= 128) {
        if ((vol & 0x100) != 0) {
            sdfSndSlots[slot].volStep = 64;
            vol = 0;
        } else {
            vol = 127;
        }
    }
    sdfSndSlots[slot].vol = vol << 8;
    sdfSndApplyVolume(id, slot, vol);
    return intrman_CpuResumeIntr(state);
}
/* ENDF sdfSndBgmPlay */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndBgmPlay);
#endif
#ifdef NON_MATCHING
/* 3 instructions short of the retail code. */
/* FUNC sdfSndSaveResumePoint */
/* Remember where a sequence stopped, so a later resume can pick it up. The
 * table holds 32 entries and wraps. */
s32 sdfSndSaveResumePoint(s32 slot)
{
    s32 id;
    s32 i;
    s32 loc;
    s32 idx;
    s32 ret;

    ret = 0xBF0000;
    if (slot >= 0) {
        id = sdfSndSlots[slot].id;
        ret = (u16)id;
        if ((u32)(id - 0x400000) <= 0xBFFFFF) {
            ret = (u16)sdfSndSlots[slot].id < 0x10;
            if (ret) {
                for (i = 0; i < 32; i++) {
                    ret = sdfSndResume[i].id;
                    if (ret == id) {
                        loc = sdfSndSlots[slot].location;
                        if (loc >= 0) {
                            sdfSndResume[i].location = loc;
                        } else {
                            ret = sdfSndMidiSlotWork[slot].location;
                            sdfSndResume[i].location = ret;
                        }
                        return ret;
                    }
                }
                idx = sdfSndResumeIdx;
                sdfSndResume[idx].id = sdfSndSlots[slot].id;
                loc = sdfSndSlots[slot].location;
                if (loc < 0) {
                    loc = sdfSndMidiSlotWork[slot].location;
                }
                sdfSndResume[idx].location = loc;
                ret = (sdfSndResumeIdx + 1) & 0x1F;
                sdfSndResumeIdx = ret;
            }
        }
    }
    return ret;
}
/* ENDF sdfSndSaveResumePoint */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSaveResumePoint);
#endif

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
#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndBgmResume */
s32 sdfSndBgmResume(sdfSndBgmReq_t *req)
{
    sdfSndResume_t *rp;
    s32 ret;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (sdfSndSlots[i].id == req->id) {
            ret = sdfSndSlots[i].volStep;
            if (ret < 0) {
                sdfSndSlots[i].volStep = 32;
            }
            return ret;
        }
    }
    rp = sdfSndFindResumePoint(req->id);
    if (rp != NULL && rp->location >= 0) {
        req->location = rp->location;
    } else {
        req->location = 0;
    }
    req->vol = 256;
    return sdfSndBgmPlay(req);
}
/* ENDF sdfSndBgmResume */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndBgmResume);
#endif
#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndBgmStop */
s32 sdfSndBgmStop(s32 *req)
{
    s32 i;
    s32 state;

    for (i = 0; i < 16; i++) {
        if (sdfSndSlots[i].id == *req &&
            (sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0) {
            if (i < 3) {
                sdfSndSaveResumePoint(i);
            }
            intrman_CpuSuspendIntr(&state);
            modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, i, 0);
            intrman_CpuResumeIntr(state);
            sdfSndSlots[i].is3d = 0;
            sdfSndSlots[i].fadeIn = 0;
            sdfSndSlots[i].volStep = 0;
            sdfSndSlots[i].location = -1;
            sdfSndSlots[i].id = -1;
            sdfSndSlots[i].unk34[0] = 0;
        }
    }
}
/* ENDF sdfSndBgmStop */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndBgmStop);
#endif
/* FUNC sdfSndHsynLoad */
/* Hand the synthesiser the wave data for a bank. */
s32 sdfSndHsynLoad(s32 bank, s32 slot)
{
    sceHSynSlot_t *hs;
    s32 state;

    hs = &sdfSndHsynSlotWork[slot];
    hs->unk00 = 0;
    hs->unk04 = 0;
    hs->unk08 = 0;
    hs->unk0C = 0;
    hs->unk10 = 0;
    hs->unk02 = 0;
    hs->unk03 = 0;
    intrman_CpuSuspendIntr(&state);
    if (modhsyn_sceHSyn_Load(&sdfSndHsynWork, slot, sdfSndWorkArea->banks[bank].spuAddr,
                             sdfSndWorkArea->banks[bank].data, 0) != 0 ||
        modhsyn_sceHSyn_Load(&sdfSndHsynWork, slot, sdfSndWorkArea->banks[bank].spuAddr,
                             sdfSndWorkArea->banks[bank].data, 1) != 0) {
        intrman_CpuResumeIntr(state);
        return stdio_printf(aSdfsnddrvScehs_0);
    }
    modhsyn_e15(&sdfSndHsynLoadParam);
    return intrman_CpuResumeIntr(state);
}
/* ENDF sdfSndHsynLoad */
#ifdef NON_MATCHING
/* 12 instructions short of the retail code. */
/* FUNC sdfSndSePlay */
/* Start a sound effect in one of the 13 effect slots and return the slot. */
s32 sdfSndSePlay(sdfSndSeReq_t *req)
{
    s32 id;
    s32 bank;
    s32 pan;
    s32 slot;
    s32 ret;
    u32 vol;
    s32 state;

    id = req->id;
    bank = sdfSndFindSlotById(req->id);
    pan = req->pan & 0x7F;
    if (sdfSndIsSlotMuted(id) != 0) {
        return -1;
    }
    if (bank < 0) {
        sdfSndPostError(18);
        return -1;
    }
    slot = sdfSndAllocSeSlot();
    if (slot < 0) {
        return -1;
    }
    if (sdfSndMidiSelect(slot, (u16)id, bank, sdfSndWorkArea->progData[bank].ptr, 0) != 0) {
        return -1;
    }
    intrman_CpuSuspendIntr(&state);
    ret = modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, slot, 1);
    sdfSndPutMsg(slot);
    if (ret != 0) {
        intrman_CpuResumeIntr(state);
        stdio_printf(aSdfsnddrvSeSta);
        return -1;
    }
    sdfSndSlots[slot].volStep = 0;
    sdfSndSlots[slot].fadeIn = 0;
    sdfSndSlots[slot].is3d = 0;
    sdfSndSlots[slot].location = -1;
    sdfSndSlots[slot].id = id;
    sdfSndSlots[slot].group = bank;
    sdfSndSlots[slot].unk34[0] = 0;
    sdfSndSlots[slot].serial = sdfSndSlotSerialNext;
    sdfSndSlotSerialNext++;
    modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, (pan << 16) | 0xAB0);
    vol = req->vol;
    if (vol >= 0x80) {
        if ((vol & 0x100) != 0) {
            sdfSndSlots[slot].volStep = 64;
            vol = 0;
        } else {
            vol = 127;
        }
    }
    sdfSndSlots[slot].vol = vol << 8;
    sdfSndApplyVolume(id, slot, vol);
    intrman_CpuResumeIntr(state);
    return slot;
}
/* ENDF sdfSndSePlay */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSePlay);
#endif

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
#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndUpdate3dVoice */
/* Re-pan and re-attenuate a positional sound after the listener moved. */
s32 sdfSndUpdate3dVoice(s32 slot)
{
    s32 dx;
    s32 dz;
    s32 dist;
    s32 angle;
    s32 sign;
    s32 level;
    s32 pan;
    s32 vol;
    s32 state;

    if (sdfSndSlots[slot].is3d == 0) {
        return sdfSndSlots[slot].is3d;
    }
    dx = sdfSndSlots[slot].posX - sdfSndListenerX;
    dz = sdfSndSlots[slot].posZ - sdfSndListenerZ;
    dist = sdfSndIsqrt(dx * dx +
                       (sdfSndSlots[slot].posY - sdfSndListenerY) *
                       (sdfSndSlots[slot].posY - sdfSndListenerY) + dz * dz);
    angle = (sdfSndAtan2Deg(-dx, dz) - sdfSndListenerAngle) % 360;
    if (angle >= 181) {
        angle -= 360;
    }
    if (angle < -180) {
        angle += 360;
    }
    sign = 1;
    if (angle < 0) {
        sign = -1;
        angle = -angle;
    }
    level = 100;
    if (angle >= 45) {
        if (angle >= 91) {
            pan = 63 - (180 - angle) * 70 * sign / 100;
            level = 75 - ((angle - 90) >> 2);
        } else {
            level = 100 - (angle - 45) / 2;
            pan = 63 - angle * 70 * sign / 100;
        }
    } else {
        pan = 63 - angle * 70 * sign / 100;
    }
    if (dist < 11) {
        pan = ((pan - 63) >> 1) + 63;
    } else {
        level = level * sdfSndDistanceAtten(dist, sdfSndSlots[slot].radius) / 100;
    }
    vol = 127 * level / 100;
    if (vol >= 127) {
        vol = 127;
    }
    sdfSndSlots[slot].vol = vol << 8;
    sdfSndSlots[slot].volStep = 0;
    sdfSndApplyVolume(sdfSndSlots[slot].id, slot, vol);
    intrman_CpuSuspendIntr(&state);
    modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, ((pan & 0x7F) << 16) | 0xAB0);
    return intrman_CpuResumeIntr(state);
}
/* ENDF sdfSndUpdate3dVoice */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndUpdate3dVoice);
#endif

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
#ifdef NON_MATCHING
/* 1 instruction longer than the retail code. */
/* FUNC sdfSndSePlay3d */
/* Play a sound effect at a point in the world. */
s32 sdfSndSePlay3d(sdfSndSe3dReq_t *req)
{
    s32 dx;
    s32 dz;
    s32 dist;
    s32 angle;
    s32 slot;

    dx = req->x - sdfSndListenerX;
    dz = req->z - sdfSndListenerZ;
    dist = sdfSndIsqrt(dx * dx + (req->y - sdfSndListenerY) * (req->y - sdfSndListenerY) +
                       dz * dz);
    angle = sdfSndAtan2Deg(-dx, dz);
    slot = sdfSndSePlayPositional(req->id, angle - sdfSndListenerAngle, dist, req->radius);
    if (slot >= 0) {
        sdfSndSlots[slot].is3d = 1;
        sdfSndSlots[slot].posX = req->x;
        sdfSndSlots[slot].posY = req->y;
        sdfSndSlots[slot].posZ = req->z;
        sdfSndSlots[slot].radius = req->radius;
    }
}
/* ENDF sdfSndSePlay3d */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSePlay3d);
#endif

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
#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndSetBankAddr */
/* kind < 0 lays the whole bank table out again from the size and offset
 * tables; otherwise it just records one bank's wave size. */
s32 sdfSndSetBankAddr(s32 kind, u32 waveSize)
{
    s32 i;
    u8 *base;

    if (kind >= 0) {
        sdfSndWorkArea->banks[kind].waveSize = waveSize;
        return kind;
    }
    for (i = 0; i < SDF_SND_KINDS; i++) {
        sdfSndWorkArea->banks[i].spuAddr = sdfSndBankSizeTbl[i];
        sdfSndWorkArea->banks[i].waveSize = sdfSndBankAddrTbl[i];
    }
    if (sdfSndBankBase == NULL) {
        sdfSndBankBase = sdfSndWorkArea->banks[0].data;
    }
    base = sdfSndBankBase;
    for (i = 0; i < SDF_SND_KINDS + 1; i++) {
        sdfSndWorkArea->banks[i].data = &base[sdfSndBankOffTbl[i]];
        sdfSndWorkArea->banks[i].size = sdfSndBankFlagTbl[i];
    }
    return -1;
}
/* ENDF sdfSndSetBankAddr */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSetBankAddr);
#endif
#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndStopByGroup */
s32 sdfSndStopByGroup(s32 group)
{
    s32 i;
    s32 state;

    for (i = 0; i < 16; i++) {
        if (sdfSndSlots[i].id >= 0 && sdfSndSlots[i].group == group &&
            (sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0) {
            if (i < 3) {
                sdfSndSaveResumePoint(i);
            }
            sdfSndSlots[i].is3d = 0;
            sdfSndSlots[i].fadeIn = 0;
            sdfSndSlots[i].volStep = 0;
            sdfSndSlots[i].location = -1;
            sdfSndSlots[i].id = -1;
            sdfSndSlots[i].unk34[0] = 0;
            intrman_CpuSuspendIntr(&state);
            modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, i, 0);
            modhsyn_e13(&sdfSndHsynWork, i);
            intrman_CpuResumeIntr(state);
        }
    }
}
/* ENDF sdfSndStopByGroup */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndStopByGroup);
#endif
/* FUNC sdfSndUnloadBank */
s32 sdfSndUnloadBank(s32 kind)
{
    if (kind >= 0) {
        sdfSndStopByGroup(kind);
        sdfSndWorkArea->banks[kind].waveSize = 0;
        sdfSndWorkArea->banks[kind].size = 0;
        sdfSndWorkArea->stat[kind].id = -1;
        if (kind == 1) {
            sdfSndBank1Loaded = 0;
        }
    }
}
/* ENDF sdfSndUnloadBank */
#ifdef NON_MATCHING
/* 1 instruction short of the retail code. */
/* FUNC p2sdIrxSpuIopMap */
/* Push the work area up to the EE by DMA and wait for it to land. */
s32 p2sdIrxSpuIopMap(void)
{
    SifDmaTransfer_t sdd;
    s32 state;
    s32 tries;
    u32 id;
    u32 start;
    u32 now;
    s32 stat;
    s32 ret;

    sdd.src = sdfSndWorkArea;
    sdd.dest = sdfSndWorkArea->eeAddr;
    tries = 5;
    sdd.attr = 0;
    sdd.size = sdfSndWorkArea->size;
    loadcore_FlushDcache();
    while (1) {
        intrman_CpuSuspendIntr(&state);
        id = sifman_sceSifSetDma(&sdd, 1);
        intrman_CpuResumeIntr(state);
        if (id != 0) {
            break;
        }
        thbase_DelayThread(1000);
        if (--tries == 0) {
            tries = 10000;
            stdio_printf(aP2sdirxspuiopm);
        }
    }
    start = thbase_e43();
    while (1) {
        intrman_CpuSuspendIntr(&state);
        stat = sifman_sceSifDmaStat(id);
        ret = intrman_CpuResumeIntr(state);
        if (stat < 0) {
            break;
        }
        thbase_DelayThread(1000);
        now = thbase_e43();
        if (now - start > 0x1F78A40) {
            stdio_printf(aP2sdirxspuiopm_0, stat);
            start = now;
        }
    }
    return ret;
}
/* ENDF p2sdIrxSpuIopMap */
#else
INCLUDE_ASM("SDFSDMAN", p2sdIrxSpuIopMap);
#endif

/* FUNC sdfSndFadeOutById */
s32 sdfSndFadeOutById(s32 *req)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (sdfSndSlots[i].id == *req &&
            (sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0) {
            sdfSndSlots[i].volStep = -32;
            sdfSndSlots[i].fadeIn = 0;
            sdfSndSlots[i].unk34[0] = 0;
        }
    }
}
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
#ifdef NON_MATCHING
/* 2 instructions short of the retail code. */
/* FUNC sdfSndAllocSeSlot */
s32 sdfSndAllocSeSlot(void)
{
    s32 i;
    s32 slot;
    u32 serial;
    s32 state;

    for (i = 3; i < 16; i++) {
        if ((sdfSndMidiWork.tbl->seq[i].slot->flags & 2) == 0 && i != sdfSndLastSeSlot) {
            sdfSndLastSeSlot = i;
            return i;
        }
    }
    serial = -1;
    slot = -1;
    for (i = 3; i < 16; i++) {
        if (sdfSndSlots[i].serial < serial && i != sdfSndLastSeSlot &&
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
    sdfSndLastSeSlot = slot;
    return slot;
}
/* ENDF sdfSndAllocSeSlot */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndAllocSeSlot);
#endif
#ifdef NON_MATCHING
/* 1 instruction short of the retail code. */
/* FUNC sdfSndSetSeqVolume */
/* The EE sends 0..15; the driver keeps it in eighths with the odd steps
 * rounded up to the next full one. */
s32 sdfSndSetSeqVolume(s32 vol)
{
    s32 v;
    s32 i;

    v = 8 * vol;
    if ((vol & 1) != 0) {
        v |= 7;
    }
    sdfSndSeqVolume = v;
    for (i = 0; i < 3; i++) {
        if ((sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0) {
            sdfSndApplyVolume(sdfSndSlots[i].id, i, sdfSndSeqVolume);
        }
    }
}
/* ENDF sdfSndSetSeqVolume */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSetSeqVolume);
#endif

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
/* FUNC sdfAdpcmStop */
s32 sdfAdpcmStop(void)
{
    u32 voices;

    sdfAdpcm.playing = 0;
    voices = (1 << sdfAdpcm.voiceL) | (1 << sdfAdpcm.voiceR);
    libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 0);
    libsd_sceSdSetParam(0xF80, 0);
    libsd_sceSdSetParam(0x1080, 0);
    libsd_sceSdSetSwitch((s8)sdfAdpcm.coreId | 0x1600, voices);
    return thsemap_SignalSema(sdfAdpcmIntrSema);
}
/* ENDF sdfAdpcmStop */

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
#ifdef NON_MATCHING
/* 4 instructions longer than the retail code. */
/* FUNC sdfAdpcmFillBuffer */
/* Read the next 512 bytes of the stream from the EE and write the ADPCM
 * block flags: 6 on the first block, 2 while it runs, 3 on the last. */
s32 sdfAdpcmFillBuffer(s32 blocking)
{
    u32 reply[4];
    u8 *buf;
    u8 *left;
    u8 *right;
    s32 i;
    s32 count;
    s32 next;

    sdfAdpcm.intrState = 0;
    if (sdfAdpcm.playing == 0) {
        return sdfAdpcm.playing;
    }
    if (sdfAdpcm.bytesLeft == 0) {
        return sdfAdpcmStop();
    }
    buf = (u8 *)(sdfAdpcm.iopBuf + ((s8)sdfAdpcm.fillIdx << 9));
    do {
        if (sdfAdpcm.onMemory) {
            sdfSndCallEeRpc(0, buf, 512, 7, reply);
        } else {
            sdfSndCallEeRpc(0, buf, 512, 1, reply);
        }
    } while (blocking && (reply[0] & 0x80000000) != 0);

    if (sdfAdpcm.fadeState == 1) {
        if ((reply[0] & 0x80000000) != 0) {
            sdfAdpcm.fadeState = 2;
            sdfAdpcmSetVolume(0);
        }
    } else if (sdfAdpcm.fadeState == 2) {
        if ((reply[0] & 0x80000000) == 0) {
            sdfAdpcm.fadeState = 1;
            sdfAdpcmSetVolume(sdfAdpcm.volume);
        }
    }

    left = buf + 1;
    right = buf + 257;
    if (sdfAdpcm.fillBlk != 0) {
        i = 0;
    } else {
        left = buf + 17;
        right = buf + 273;
        i = 1;
        buf[1] = 6;
        buf[257] = 6;
    }
    count = 31;
    if (sdfAdpcm.stereo) {
        count = 15;
    }
    for (; i < count; i++) {
        *right = 2;
        *left = 2;
        left += 16;
        right += 16;
    }
    if (sdfAdpcm.fillBlk == 2) {
        *right = 3;
        *left = 3;
    } else {
        *right = 2;
        *left = 2;
    }
    if ((reply[0] & 0x80000000) == 0 && sdfAdpcm.bytesLeft != 0) {
        if (sdfAdpcm.bytesLeft >= 0x201) {
            sdfAdpcm.bytesLeft -= 512;
        } else {
            sdfAdpcm.bytesLeft = 0;
        }
    }
    next = sdfAdpcm.fillBlk + 1;
    sdfAdpcm.fillIdx = (sdfAdpcm.fillIdx + 1) & 1;
    sdfAdpcm.fillBlk = next % 3;
}
/* ENDF sdfAdpcmFillBuffer */
#else
INCLUDE_ASM("SDFSDMAN", sdfAdpcmFillBuffer);
#endif
/* FUNC sdfAdpcmSetVolume */
s32 sdfAdpcmSetVolume(s32 vol)
{
    s32 entry;
    s32 entryR;

    vol = vol << 7;
    if ((vol & 0x80) != 0) {
        vol |= 0xFF;
    }
    entry = 2 * sdfAdpcm.voiceL + (s8)sdfAdpcm.coreId;
    entryR = 2 * sdfAdpcm.voiceR + (s8)sdfAdpcm.coreId;
    libsd_sceSdSetParam(entry, vol);
    libsd_sceSdSetParam(entry | 0x100, 0);
    libsd_sceSdSetParam(entryR, 0);
    libsd_sceSdSetParam(entryR | 0x100, vol);
}
/* ENDF sdfAdpcmSetVolume */

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
