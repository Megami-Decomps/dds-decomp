#include "MODMIDI.h"

extern u8 midiLibraryTable[];

/* MODMIDI.IRX, Sony's MIDI sequencer: it reads a bank's SCEIMidi block
 * and turns it into the channel messages the synthesiser plays.
 * See docs/irx-modules.md.
 *
 * The chunks still in assembly are the ones no C has been written for yet,
 * plus the import stubs and the import and export tables between them. */

/* FUNC modmidi_e26 */
int modmidi_e26(int argc, char **argv)
{
    char *p;
    s32 state;
    s32 res;
    int i;
    int k;

    for (i = 1; i < argc; i++) {
        p = argv[i];
        if (sysclib_strlen(p) == 0) {
            continue;
        }
        while (sysclib_look_ctype_table(*p) & 8) {
            p++;
        }
        for (k = 0; sceMidiOptions[k].len > 0; k++) {
            if (sysclib_strncmp(p, sceMidiOptions[k].name,
                                sceMidiOptions[k].len) == 0) {
                *sceMidiOptions[k].var =
                    sysclib_strtol(p + (sceMidiOptions[k].len + 1), NULL, 10);
                break;
            }
        }
    }
    intrman_CpuSuspendIntr(&state);
    res = loadcore_RegisterLibraryEntries(midiLibraryTable);
    intrman_CpuResumeIntr(state);
    return res != 0;
}
/* ENDF modmidi_e26 */
/* FUNC modmidi_sceMidi_Init */
int modmidi_sceMidi_Init(sceMidiWork_t *work, int interval)
{
    int slotNo;
    u32 *seg;
    sceMidiSeq_t *seq;
    sceMidiVoice_t *voice;
    sceMidiSlot_t *slot;
    int i;

    sceMidiTickScale = interval << 7;
    if (work == NULL) {
        return -1;
    }
    /* The work table is a run of {count, pointer} segments, the sequence
     * slots then the output buffers, and the second is reached by stepping
     * the same pointer rather than through sceMidiTbl_t's later fields. */
    seg = (u32 *)work->tbl;
    if ((int)work->nTblWord < 2) {
        return -1;
    }
    if (seg == NULL) {
        return -1;
    }
    seq = (sceMidiSeq_t *)seg[1];
    if (seg[0] & 1) {
        return -1;
    }
    if (seq == NULL) {
        return -1;
    }
    for (slotNo = 0; slotNo * 2 + 1 < (int)seg[0]; slotNo++) {
        slot = seq[slotNo].slot;
        if (slot != NULL) {
            for (i = 15; i >= 0; i--) {
                slot->chanMask[i] = 1;
            }
            slot->bufMask = 1;
            slot->chMsgCb = NULL;
            slot->metaCb = NULL;
            slot->excCb = NULL;
            slot->loopCb = NULL;
            slot->ctlCb = NULL;
            modmidi_sceMidi_Load(work, slotNo);
        }
    }
    seg += 2;
    voice = (sceMidiVoice_t *)seg[1];
    if (seg[0] == 0) {
        return -1;
    }
    if (voice == NULL) {
        return -1;
    }
    if ((int)seg[0] > 0) {
        slotNo = 1;
        do {
            if (voice->buf != NULL) {
                voice->buf->len = 0;
            }
            voice++;
        } while (slotNo++ < (int)seg[0]);
    }
    return 0;
}
/* ENDF modmidi_sceMidi_Init */
/* FUNC sceMidiTagMatch */
/* Not a parser: a 4+4 character chunk tag comparator, with both tags passed
 * as words and both strings read backwards. */
int sceMidiTagMatch(u32 tag0, u32 tag1, const char *magic, const char *name)
{
    char *p0;
    char *p1;
    u32 i;

    p0 = (char *)&tag0;
    p1 = (char *)&tag1;
    if (sceMidiVerbose != 0) {
        stdio_printf(aCCCCCCCC, p0[3], p0[2], p0[1], p0[0], p1[3], p1[2],
                     p1[1], p1[0]);
    }
    for (i = 0; i < 4; i++) {
        if (p0[i] != magic[3 - i] || p1[i] != name[3 - i]) {
            return 0;
        }
    }
    return 1;
}
/* ENDF sceMidiTagMatch */
/* FUNC modmidi_sceMidi_Load */
#ifdef NON_MATCHING
/* Equivalent, same size. Four pointers land one callee-saved register off
 * retail's, the chExpr fill recomputes trk+i rather than carrying gcc's
 * biased cursor, and gcc threads every failure branch into the epilogue
 * where retail keeps a separate return -1 block before the success path. */
int modmidi_sceMidi_Load(sceMidiWork_t *work, int slotNo)
{
    sceMidiTbl_t *tbl;
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiVers_t *vers;
    sceMidiSequ_t *sequ;
    sceMidiSongChunk_t *song;
    sceMidiMidiChunk_t *midi;
    u32 unit;
    u32 ofs;
    int i;

    if (work == NULL) {
        return -1;
    }
    tbl = work->tbl;
    unit = slotNo * 2 + 1;
    if (unit >= tbl->nSeqWord) {
        return -1;
    }
    slot = tbl->seq[slotNo].slot;
    if (slot == NULL) {
        return -1;
    }
    trk = &slot->trk;
    slot->songNo = -1;
    slot->midiNo = -1;
    slot->tick = 0;
    slot->flags = 0;
    trk->sequ = NULL;
    trk->midi = NULL;
    trk->song = NULL;
    trk->chActive = 0;
    for (i = 15; i >= 0; i--) {
        trk->chExpr[i] = 0x80;
    }
    trk->vol = 0x80;
    vers = work->tbl->seq[slotNo].data;
    if (vers == NULL) {
        return -1;
    }
    if (sceMidiTagMatch(*(u32 *)vers->magic, *(u32 *)vers->name, magic, name) == 0) {
        return -1;
    }
    sequ = (sceMidiSequ_t *)((u8 *)vers + vers->size);
    if (sceMidiTagMatch(*(u32 *)sequ->magic, *(u32 *)sequ->name, magic, aSequ) == 0) {
        return -1;
    }
    ofs = sequ->songOfs;
    song = (sceMidiSongChunk_t *)((u8 *)vers + ofs);
    if (ofs == -1 ||
        sceMidiTagMatch(*(u32 *)song->magic, *(u32 *)song->name, magic, aSong) == 0) {
        song = NULL;
    }
    ofs = sequ->midiOfs;
    if (ofs == -1) {
        return -1;
    }
    midi = (sceMidiMidiChunk_t *)((u8 *)vers + ofs);
    if (sceMidiTagMatch(*(u32 *)midi->magic, *(u32 *)midi->name, magic,
                        (const char *)aMidi) == 0) {
        return -1;
    }
    trk->sequ = sequ;
    trk->midi = midi;
    trk->song = song;
    slot->flags = 1;
    return 0;
}
#else
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_Load);
#endif
/* ENDF modmidi_sceMidi_Load */
/* FUNC modmidi_sceMidi_Unload */
/* Retail passes the unit index, not the slot number, to MidiPlaySwitch,
 * which derives the unit again: the stop never reaches the right slot. */
int modmidi_sceMidi_Unload(sceMidiWork_t *work, int slotNo)
{
    sceMidiTbl_t *tbl;
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    u32 unit;
    int ch;

    if (work == NULL) {
        return -1;
    }
    tbl = work->tbl;
    unit = slotNo * 2 + 1;
    if (unit >= tbl->nSeqWord) {
        return -1;
    }
    slot = (sceMidiSlot_t *)((void **)tbl->seq)[unit * 2 + 1];
    if (slot == NULL) {
        return -1;
    }
    if (slot->flags & 2) {
        modmidi_sceMidi_MidiPlaySwitch(work, unit, 0);
    }
    trk = &slot->trk;
    slot->songNo = -1;
    slot->midiNo = -1;
    slot->flags = 0;
    slot->tick = 0;
    trk->sequ = NULL;
    trk->vol = 0x80;
    trk->midi = NULL;
    trk->song = NULL;
    trk->smfHdr = NULL;
    trk->smfData = NULL;
    trk->tick = 0;
    trk->frac = 0;
    trk->usPerTick = 0;
    trk->runStatus = 0;
    trk->deltaHeld = 0;
    trk->ctlLoopId = 0xFF;
    trk->ctlNrpn = 0xFF;
    trk->ctlLoopCnt = 0xFF;
    trk->ctlRpn = 0xFF;
    trk->chActive = 0;
    for (ch = 0; ch < 16; ch++) {
        trk->chVol[ch] = 0;
        trk->chExpr[ch] = 0;
        trk->ch[ch].prog = 0xFF;
        trk->ch[ch].bank = 0xFF;
        trk->ch[ch].cc1 = 0xFF;
        trk->ch[ch].cc2 = 0xFF;
        trk->ch[ch].cc5 = 0xFF;
        trk->ch[ch].vol = 0xFF;
        trk->ch[ch].pan = 0xFF;
        trk->ch[ch].expr = 0xFF;
        trk->ch[ch].sustain = 0xFF;
        trk->ch[ch].portamento = 0xFF;
        trk->ch[ch].bend = 0xFFFF;
    }
    return 0;
}
/* ENDF modmidi_sceMidi_Unload */
ALIGN_TEXT(4);
/* FUNC modmidi_e5 */
int modmidi_e5(sceMidiWork_t *work)
{
    sceMidiTbl_t *tbl;
    sceMidiVoiceTbl_t *vt;
    sceMidiSlot_t *slot;
    int unit;

    tbl = work->tbl;
    vt = (sceMidiVoiceTbl_t *)&tbl->nVoiceWord;
    for (unit = 1; unit < tbl->nSeqWord; unit += 2) {
        slot = (sceMidiSlot_t *)((void **)tbl->seq)[unit * 2 + 1];
        if (slot == NULL) {
            continue;
        }
        if (slot->flags & 2) {
            sceMidiTrackServiceTick(slot, vt);
        }
        if (slot->flags & 0x100) {
            sceMidiSongServiceTick(work, unit / 2, slot);
        }
    }
    return 0;
}
/* ENDF modmidi_e5 */
ALIGN_TEXT(4);
/* FUNC sceMidiLoopFind */
sceMidiLoop_t *sceMidiLoopFind(sceMidiLoop_t *tbl, u8 id)
{
    int i;

    for (i = 7; i != -1; i--, tbl++) {
        if (tbl->id == id) {
            return tbl;
        }
    }
    return NULL;
}
/* ENDF sceMidiLoopFind */
/* FUNC sceMidiScanLoops */
#ifdef NON_MATCHING
/* 40 bytes long: gcc keeps the four control-change arms and the many
 * `r++` exits as separate blocks where retail cross-jumps them, so the
 * two switches come out one layer flatter. */
void sceMidiScanLoops(sceMidiSlot_t *slot, sceMidiTrack_t *trk, u8 status, u8 cc,
                   u8 val, sceMidiVoiceTbl_t *vt)
{
    sceMidiLoop_t *e;
    int r;
    int i;
    u8 cnt;

    switch (cc) {
    case 0x62:
        trk->ctlRpn = val;
        trk->ctlLoopCnt = 0xFF;
        trk->ctlLoopId = 0xFF;
        break;
    case 0x63:
        trk->ctlNrpn = val;
        trk->ctlLoopCnt = 0xFF;
        trk->ctlLoopId = 0xFF;
        break;
    case 6:
        trk->ctlLoopId = val;
        break;
    case 0x26:
        trk->ctlLoopCnt = val;
        break;
    default:
        return;
    }
    r = 0;
    switch (trk->ctlNrpn) {
    case 0:
        if (trk->ctlLoopId == 0xFF) {
            break;
        }
        e = sceMidiLoopFind(trk->loop, trk->ctlLoopId);
        if (e == NULL) {
            e = trk->loop;
            for (i = 7; i != -1; i--, e++) {
                if (e->id == 0xFF) {
                    break;
                }
            }
        } else if (e->count != 0xFF && sceMidiDebug != 0) {
            stdio_printf(aLoopStartDuple, trk->ctlLoopId);
        }
        if (e != NULL) {
            e->count = 0xFF;
            e->id = trk->ctlLoopId;
            e->unk02 = trk->runStatus;
            e->unk03 = trk->deltaHeld;
            e->time = trk->tick;
            e->ptr = trk->cur;
        } else if (sceMidiDebug != 0) {
            stdio_printf(aLoopTableOverf);
        }
        r++;
        break;
    case 1:
        if (trk->ctlLoopId == 0xFF || trk->ctlLoopCnt == 0xFF) {
            break;
        }
        if ((slot->flags & 8) || vt == NULL) {
            r++;
            break;
        }
        e = sceMidiLoopFind(trk->loop, trk->ctlLoopId);
        if (e == NULL) {
            if (sceMidiDebug != 0) {
                stdio_printf(aNotFoundLoopSt, trk->ctlLoopId);
            }
            r++;
            break;
        }
        if (e->count == 0xFF) {
            e->count = trk->ctlLoopCnt;
        }
        cnt = trk->ctlLoopCnt;
        if (cnt != 0 && e->count == 0) {
            e->count = 0xFF;
            r++;
            break;
        }
        if (slot->loopCb != NULL) {
            sceMidiLoopInfo.total = cnt;
            sceMidiLoopInfo.pass = trk->ctlLoopCnt - e->count + 1;
            sceMidiLoopInfo.id = e->id;
            if (slot->loopCb(&sceMidiLoopInfo, slot->loopArg) == 0) {
                e->count = 0xFF;
                break;
            }
        }
        trk->cur = e->ptr;
        slot->tick = e->time;
        trk->tick = e->time;
        trk->deltaHeld = e->unk03;
        trk->runStatus = e->unk02;
        if (trk->ctlLoopCnt != 0) {
            e->count = e->count - 1;
        }
        r++;
        break;
    case 0x10:
        if (trk->ctlRpn == 0) {
            goto sendId;
        }
        if (trk->ctlRpn == 1) {
            goto sendIdCount;
        }
        break;
    case 0x11:
    sendId:
        if (trk->ctlLoopId == 0xFF) {
            break;
        }
        if (vt == NULL || slot->ctlCb == NULL) {
            r++;
            break;
        }
        slot->ctlCb(status & 0xF, val, (u32)trk->cur, slot->ctlArg);
        r++;
        break;
    case 0x12:
    sendIdCount:
        if (trk->ctlLoopId == 0xFF || trk->ctlLoopCnt == 0xFF) {
            break;
        }
        if (vt == NULL || slot->ctlCb == NULL) {
            r++;
            break;
        }
        slot->ctlCb(status & 0xF,
                    ((trk->ctlLoopId & 0x7F) << 7) | (trk->ctlLoopCnt & 0x7F),
                    (u32)trk->cur, slot->ctlArg);
        r++;
        break;
    case 0x7F:
        if (trk->ctlRpn == 0xFF) {
            r = 1;
        }
        break;
    }
    if (r != 0) {
        trk->ctlLoopCnt = 0xFF;
        trk->ctlLoopId = 0xFF;
        trk->ctlRpn = 0xFF;
        trk->ctlNrpn = 0xFF;
    }
}
#else
INCLUDE_ASM("MODMIDI", sceMidiScanLoops);
#endif
/* ENDF sceMidiScanLoops */
/* FUNC sceMidiTrackSeekAbsolute */
u32 sceMidiTrackSeekAbsolute(sceMidiTrack_t *trk)
{
    u32 us;

    us = (trk->tempo << 7) / trk->division;
    us = (us << 8) / trk->speed;
    trk->usPerTick = us;
    return us;
}
/* ENDF sceMidiTrackSeekAbsolute */
/* FUNC sceMidiReadVarLen */
int sceMidiReadVarLen(sceMidiTrack_t *trk)
{
    u8 *p;
    int v;
    u8 c;

    p = trk->cur;
    v = 0;
    do {
        c = *p++;
        v = (v << 7) + (c & 0x7F);
    } while (c & 0x80);
    trk->cur = p;
    return v;
}
/* ENDF sceMidiReadVarLen */
/* FUNC sceMidiTrackReset */
int sceMidiTrackReset(sceMidiTrack_t *trk)
{
    u8 *p;
    int v;
    u8 c;

    v = 0;
    if (trk->deltaHeld != 0) {
        trk->deltaHeld = 0;
    } else {
        p = trk->cur;
        do {
            c = *p++;
            v = (v << 7) + (c & 0x7F);
        } while (c & 0x80);
        trk->cur = p;
        trk->tick += v;
    }
    return v;
}
/* ENDF sceMidiTrackReset */
/* FUNC sceMidiReadDeltaTime */
int sceMidiReadDeltaTime(sceMidiTrack_t *trk)
{
    int v;

    trk->cur--;
    v = sceMidiReadVarLen(trk);
    trk->cur += v;
    return v;
}
/* ENDF sceMidiReadDeltaTime */
/* FUNC sceMidiParseMetaEvent */
#ifdef NON_MATCHING
/* Four bytes short: gcc cross-jumps the `trk->cur += len` tail the
 * callback-handled path and the default both end in, and hoists the
 * `return 1` out of the compare chain; retail keeps two copies. */
int sceMidiParseMetaEvent(sceMidiSlot_t *slot, sceMidiTrack_t *trk, u8 type)
{
    int len;

    len = sceMidiReadVarLen(trk);
    if (slot->metaCb != NULL) {
        if (slot->metaCb(type, trk->cur, len, slot->metaArg) == 0) {
            trk->cur += len;
            return type != 0x2F;
        }
    }
    switch (type) {
    case 0x51:
        if (len != 3) {
            if (sceMidiDebug != 0) {
                stdio_printf(aParsemetaevent, len);
            }
            return 0;
        }
        trk->tempo = trk->cur[0] << 16;
        trk->tempo += trk->cur[1] << 8;
        trk->tempo += trk->cur[2];
        if (sceMidiVerbose != 0) {
            stdio_printf(aTempoD, 60000000 / trk->tempo);
        }
        trk->cur += 3;
        sceMidiTrackSeekAbsolute(trk);
        return 1;
    case 0x2F:
        slot->flags |= 4;
        return 0;
    }
    trk->cur += len;
    return 1;
}
#else
INCLUDE_ASM("MODMIDI", sceMidiParseMetaEvent);
#endif
/* ENDF sceMidiParseMetaEvent */
/* FUNC sceMidiVoiceLookup */
sceMidiVoice_t *sceMidiVoiceLookup(u32 index, sceMidiVoiceTbl_t *vt)
{
    if (index >= vt->nVoiceWord) {
        return NULL;
    }
    return &vt->voice[index];
}
/* ENDF sceMidiVoiceLookup */
/* FUNC sceMidiParseExcEvent */
void sceMidiParseExcEvent(sceMidiSlot_t *slot, sceMidiTrack_t *trk,
                       sceMidiVoiceTbl_t *vt)
{
    sceMidiVoice_t *v;
    sceMidiBuf_t *buf;
    u8 *p;
    u8 *src;
    u32 mask;
    int len;
    int index;
    int shift;
    int n;
    u32 total;

    len = sceMidiReadVarLen(trk);
    if (slot->excCb == NULL ||
        slot->excCb(trk->cur, len, slot->excArg) != 0) {
        if (vt != NULL) {
            mask = slot->bufMask;
            if (mask != 0) {
                index = 0;
                do {
                    shift = 1;
                    if (mask & shift) {
                        v = sceMidiVoiceLookup(index, vt);
                        if (v != NULL) {
                            buf = v->buf;
                            if (buf != NULL) {
                                if (buf->size >= buf->len + len + 9) {
                                    p = (u8 *)buf + (buf->len + 8);
                                    src = trk->cur;
                                    total = buf->len + 1;
                                    buf->len = total + len;
                                    *p = 0xF0;
                                    p++;
                                    n = len - 1;
                                    while (n != -1) {
                                        *p++ = *src++;
                                        n--;
                                    }
                                } else if (sceMidiDebug != 0) {
                                    stdio_printf(aPaesexceventBu);
                                }
                            }
                        }
                    } else if ((mask & 0xF) != 0) {
                        /* Retail: sceMidiSendChMsg skips four bits when the
                         * nibble is empty, this one when it is not. */
                        shift = 4;
                    }
                    mask >>= shift;
                    index += shift;
                } while (mask != 0);
            }
        }
    }
    trk->cur += len;
}
/* ENDF sceMidiParseExcEvent */
/* FUNC sceMidiSendChMsg */
void sceMidiSendChMsg(sceMidiSlot_t *slot, sceMidiTrack_t *trk,
                   sceMidiVoiceTbl_t *vt, u32 msg, int len)
{
    sceMidiVoice_t *v;
    sceMidiBuf_t *buf;
    u8 *p;
    u32 w;
    u32 mask;
    u32 total;
    int ch;
    int index;
    int shift;
    int n;

    msg &= 0x7F7FFF;
    if (slot->chMsgCb != NULL) {
        msg = slot->chMsgCb(msg, slot->chMsgArg);
        if (msg == 0) {
            return;
        }
    }
    ch = msg & 0xF;
    trk->chActive |= sceMidiChBit[ch];
    if (vt == NULL) {
        return;
    }
    mask = slot->chanMask[ch];
    if (mask == 0) {
        return;
    }
    index = 0;
    shift = 1;
    do {
        shift = 1;
        if (mask & shift) {
            v = sceMidiVoiceLookup(index, vt);
            if (v != NULL) {
                buf = v->buf;
                if (buf != NULL) {
                    total = buf->len + len;
                    if (buf->size < total + 8) {
                        if (sceMidiDebug != 0) {
                            stdio_printf(aSendchmsgBuffe);
                        }
                    } else {
                        w = msg;
                        p = (u8 *)buf + (buf->len + 8);
                        buf->len = total;
                        n = len - 1;
                        while (n != -1) {
                            *p++ = w;
                            w >>= 8;
                            n--;
                        }
                    }
                }
            }
        } else if ((mask & 0xF) == 0) {
            shift = 4;
        }
        mask >>= shift;
        index += shift;
    } while (mask != 0);
}
/* ENDF sceMidiSendChMsg */
/* FUNC sceMidiChannelReset */
int sceMidiChannelReset(sceMidiTrack_t *trk, u8 ch)
{
    u32 prod;
    u32 v;

    prod = trk->chVol[ch] * trk->chExpr[ch] * trk->vol;
    v = prod >> 14;
    if (v >= 0x80) {
        v = 0x7F;
    }
    return v;
}
/* ENDF sceMidiChannelReset */
/* FUNC sceMidiPlayEnv */
#ifdef NON_MATCHING
/* Cannot match while .rodata is assembly: the controller switch compiles to
 * a jump table, and the table in the assembly .rodata already holds this
 * body's case labels, so a table emitted from the C file will not link. */
int sceMidiPlayEnv(sceMidiSlot_t *slot, sceMidiTrack_t *trk, sceMidiVoiceTbl_t *vt)
{
    u8 *p;
    u8 status;
    u8 d1;
    u8 d2;
    u32 msg;
    u32 idx;
    int len;
    int ch;

    while (slot->tick >= trk->tick) {
        p = trk->cur;
        status = p[0];
        trk->cur = p + 1;
        if (status & 0x80) {
            d1 = p[1];
            trk->cur = p + 2;
        } else {
            d1 = status;
            if (trk->runStatus == 0) {
                if (sceMidiDebug != 0) {
                    stdio_printf(aPlayenvRunning, status, trk->runStatus);
                }
                return 0;
            }
            status = trk->runStatus;
        }
        if (status >= 0xF0) {
            trk->deltaHeld = 0;
            switch (status) {
            case 0xF0:
                sceMidiParseExcEvent(slot, trk, vt);
                break;
            case 0xF7:
                sceMidiReadDeltaTime(trk);
                break;
            case 0xFF:
                if (sceMidiParseMetaEvent(slot, trk, d1) == 0) {
                    return 0;
                }
                break;
            default:
                if (sceMidiDebug != 0) {
                    stdio_printf(aPlayenvUnknown, status);
                }
                return 0;
            }
        } else {
            msg = status | (d1 << 8);
            trk->runStatus = status;
            ch = status & 0xF;
            d2 = 0;
            len = 2;
            switch (status & 0xF0) {
            case 0x80:
            case 0xD0:
                break;
            case 0xC0:
                trk->ch[ch].prog = d1 & 0x7F;
                break;
            case 0x90:
            case 0xE0:
                p = trk->cur;
                d2 = p[0];
                trk->cur = p + 1;
                msg |= d2 << 16;
                len = 3;
                if (status >= 0xE0) {
                    trk->ch[ch].bend = msg >> 8;
                }
                break;
            case 0xA0:
                if (trk->smfHdr == NULL) {
                    p = trk->cur;
                    d2 = p[0];
                    trk->cur = p + 1;
                    msg |= d2 << 16;
                    len = 3;
                    break;
                }
                idx = (ch | (d1 & 0xF0)) << 1;
                len = 3;
                if (idx >= *(u16 *)(trk->smfHdr + 2)) {
                    len = 0;
                    break;
                }
                msg = trk->smfHdr[idx + 4] | (trk->smfHdr[idx + 5] << 8);
                msg |= (((d1 & 0xF) << 3) | 7) << 16;
                break;
            case 0xB0:
                p = trk->cur;
                d2 = p[0];
                trk->cur = p + 1;
                msg |= d2 << 16;
                len = 3;
                switch (d1) {
                case 0:
                    trk->ch[ch].bank = d2 & 0x7F;
                    break;
                case 1:
                    trk->ch[ch].cc1 = d2 & 0x7F;
                    break;
                case 2:
                    trk->ch[ch].cc2 = d2 & 0x7F;
                    break;
                case 5:
                    trk->ch[ch].cc5 = d2 & 0x7F;
                    break;
                case 7:
                    trk->chVol[ch] = d2 & 0x7F;
                    trk->ch[ch].vol = d2 & 0x7F;
                    d2 = (d2 & 0x80) | sceMidiChannelReset(trk, ch);
                    msg = (msg & 0xFFFF) | (d2 << 16);
                    break;
                case 10:
                    trk->ch[ch].pan = d2 & 0x7F;
                    break;
                case 11:
                    trk->ch[ch].expr = d2 & 0x7F;
                    break;
                case 32:
                    slot->chanMask[ch] = 1 << (d2 & 0xF);
                    break;
                case 64:
                    trk->ch[ch].sustain = d2 & 0x7F;
                    break;
                case 65:
                    trk->ch[ch].portamento = d2 & 0x7F;
                    break;
                case 6:
                case 38:
                case 98:
                case 99:
                    sceMidiScanLoops(slot, trk, status, d1, d2 & 0x7F, vt);
                    break;
                }
                break;
            default:
                if (sceMidiDebug != 0) {
                    stdio_printf(aPlayenvUnknown, status);
                }
                return 0;
            }
            if (len != 0) {
                sceMidiSendChMsg(slot, trk, vt, msg, len);
            }
            trk->deltaHeld = (d1 | d2) & 0x80;
        }
        sceMidiTrackReset(trk);
    }
    return 1;
}
#else
INCLUDE_ASM("MODMIDI", sceMidiPlayEnv);
#endif
/* ENDF sceMidiPlayEnv */
/* FUNC sceMidiStopAllNotes */
int sceMidiStopAllNotes(sceMidiSlot_t *slot, sceMidiVoiceTbl_t *vt)
{
    sceMidiTrack_t *trk;
    u32 ch;

    trk = &slot->trk;
    for (ch = 0; ch < 16; ch++) {
        if (trk->chActive & sceMidiChBit[ch]) {
            sceMidiSendChMsg(slot, trk, vt, ch | 0x40B0, 3);
            sceMidiSendChMsg(slot, trk, vt, ch | 0x7BB0, 3);
        }
    }
    trk->chActive = 0;
}
/* ENDF sceMidiStopAllNotes */
#ifdef NON_MATCHING
/* Two instructions swapped: gcc schedules frac = 0 ahead of the vt argument
 * copy in the block that stops the track. */
/* FUNC sceMidiTrackServiceTick */
int sceMidiTrackServiceTick(sceMidiSlot_t *slot, sceMidiVoiceTbl_t *vt)
{
    sceMidiTrack_t *trk;
    u32 frac;

    trk = &slot->trk;
    frac = trk->frac + sceMidiTickScale;
    for (;;) {
        if (sceMidiPlayEnv(slot, trk, vt) == 0) {
            frac = 0;
            slot->flags &= ~2;
            sceMidiStopAllNotes(slot, vt);
            break;
        }
        if (frac < trk->usPerTick) {
            break;
        }
        frac -= trk->usPerTick;
        slot->tick++;
    }
    trk->frac = frac;
}
/* ENDF sceMidiTrackServiceTick */
#else
INCLUDE_ASM("MODMIDI", sceMidiTrackServiceTick);
#endif
/* FUNC sceMidiApplyPlayState */
void sceMidiApplyPlayState(sceMidiSlot_t *slot, sceMidiVoiceTbl_t *vt)
{
    sceMidiTrack_t *trk;
    u32 ch;
    u8 t;
    u8 v;
    u16 w;

    trk = &slot->trk;
    for (ch = 0; ch < 16; ch++) {
        t = trk->ch[ch].bank;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].bank << 16;
            msg |= 0xB0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
        t = trk->ch[ch].prog;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].prog << 8;
            msg |= 0xC0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 2);
        }
        t = trk->ch[ch].cc1;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].cc1 << 16;
            msg |= 0x1B0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
        t = trk->ch[ch].cc2;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].cc2 << 16;
            msg |= 0x2B0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
        t = trk->ch[ch].cc5;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].cc5 << 16;
            msg |= 0x5B0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
        v = trk->ch[ch].vol;
        if ((v & 0x80) == 0) {
            trk->chVol[ch] = v;
            sceMidiSendChannelVolume(slot, trk, vt, ch);
        }
        t = trk->ch[ch].pan;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].pan << 16;
            msg |= 0xAB0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
        t = trk->ch[ch].expr;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].expr << 16;
            msg |= 0xBB0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
        t = trk->ch[ch].sustain;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].sustain << 16;
            msg |= 0x40B0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
        t = trk->ch[ch].portamento;
        t &= 0x80;
        if (t == 0) {
            u32 msg = trk->ch[ch].portamento << 16;
            msg |= 0x41B0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
        w = trk->ch[ch].bend;
        w &= 0x8080;
        if (w == 0) {
            u32 msg = trk->ch[ch].bend << 8;
            msg |= 0xE0;
            sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
        }
    }
}
/* ENDF sceMidiApplyPlayState */
#ifdef NON_MATCHING
/* Three registers apart: retail clears the bit in place in the register the
 * shared store uses, gcc loads into a second temp. */
/* FUNC modmidi_sceMidi_MidiPlaySwitch */
int modmidi_sceMidi_MidiPlaySwitch(sceMidiWork_t *work, int slotNo, int on)
{
    sceMidiTbl_t *tbl;
    sceMidiVoiceTbl_t *vt;
    sceMidiSlot_t *slot;
    u32 flags;
    u32 unit;

    if (work == NULL) {
        return -1;
    }
    tbl = work->tbl;
    unit = slotNo * 2 + 1;
    if (unit >= tbl->nSeqWord) {
        return -1;
    }
    slot = (sceMidiSlot_t *)((void **)tbl->seq)[unit * 2 + 1];
    if (slot == NULL) {
        return -1;
    }
    vt = (sceMidiVoiceTbl_t *)&tbl->nVoiceWord;
    if (on != 0) {
        if (on != 1) {
            return 0;
        }
        if ((slot->flags & 5) != on || slot->midiNo == -1) {
            return -1;
        }
        sceMidiApplyPlayState(slot, vt);
        flags = slot->flags | 2;
    } else {
        if ((slot->flags & 2) == 0) {
            return 0;
        }
        sceMidiStopAllNotes(slot, vt);
        flags = slot->flags & ~2;
    }
    slot->flags = flags;
    return 0;
}
/* ENDF modmidi_sceMidi_MidiPlaySwitch */
#else
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_MidiPlaySwitch);
#endif
/* FUNC sceMidiSelectTrack */
int sceMidiSelectTrack(sceMidiSlot_t *slot)
{
    sceMidiTrack_t *trk;
    int i;

    trk = &slot->trk;
    slot->tick = 0;
    trk->tick = 0;
    trk->frac = 0;
    trk->usPerTick = 0;
    trk->runStatus = 0;
    trk->deltaHeld = 0;
    trk->ctlLoopId = 0xFF;
    trk->ctlNrpn = 0xFF;
    trk->ctlLoopCnt = 0xFF;
    trk->ctlRpn = 0xFF;
    trk->cur = trk->smfData;
    for (i = 0; i < 16; i++) {
        trk->chVol[i] = 0x7F;
        trk->ch[i].prog = 0xFF;
        trk->ch[i].bank = 0xFF;
        trk->ch[i].cc1 = 0xFF;
        trk->ch[i].cc2 = 0xFF;
        trk->ch[i].cc5 = 0xFF;
        trk->ch[i].vol = 0xFF;
        trk->ch[i].pan = 0xFF;
        trk->ch[i].expr = 0xFF;
        trk->ch[i].sustain = 0xFF;
        trk->ch[i].portamento = 0xFF;
        trk->ch[i].bend = 0xFFFF;
    }
    sceMidiTrackSeekAbsolute(trk);
    sceMidiTrackReset(trk);
    return 1;
}
/* ENDF sceMidiSelectTrack */
/* FUNC modmidi_sceMidi_SelectMidi */
int modmidi_sceMidi_SelectMidi(sceMidiWork_t *work, int slotNo, u32 midiNo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiMidiChunk_t *midi;
    sceMidiSmfHdr_t *hdr;
    int i;
    u32 div;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    midi = trk->midi;
    if (slot->flags & 2) {
        modmidi_sceMidi_MidiPlaySwitch(work, slotNo, 0);
    }
    slot->flags &= ~6;
    slot->midiNo = -1;
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    if (midi == NULL) {
        return -1;
    }
    if (midi->nTrack < midiNo) {
        return -1;
    }
    hdr = (sceMidiSmfHdr_t *)((u8 *)midi + midi->trackOfs[midiNo]);
    trk->smfHdr = (u8 *)&hdr->format;
    if (hdr->format != 1 || *(u16 *)(trk->smfHdr + 2) == 0 ||
        hdr->size < 0xC) {
        trk->smfHdr = NULL;
    }
    for (i = 0; i < 8; i++) {
        trk->loop[i].id = 0xFF;
        trk->loop[i].count = 0xFF;
    }
    trk->smfData = (u8 *)hdr + hdr->size;
    div = hdr->division;
    trk->tempo = 500000;
    trk->speed = 0x100;
    trk->division = div;
    if (sceMidiSelectTrack(slot) == 0) {
        return -1;
    }
    slot->midiNo = midiNo;
    return 0;
}
/* ENDF modmidi_sceMidi_SelectMidi */
/* FUNC modmidi_e16 */
int modmidi_e16(sceMidiWork_t *work, int slotNo, u32 tick)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    u32 t;
    int r;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    if (tick < slot->tick) {
        if (sceMidiSelectTrack(slot) == 0) {
            return -1;
        }
    }
    while (slot->tick < tick) {
        if (slot->flags & 4) {
            return -1;
        }
        t = trk->tick;
        if (t >= tick) {
            slot->tick = tick;
            break;
        }
        slot->tick = t;
        if (sceMidiPlayEnv(slot, trk, NULL) == 0) {
            break;
        }
    }
    r = 0;
    if (slot->tick != tick) {
        r = -1;
    }
    return r;
}
/* ENDF modmidi_e16 */
/* FUNC sceMidiSendChannelVolume */
void sceMidiSendChannelVolume(sceMidiSlot_t *slot, sceMidiTrack_t *trk, sceMidiVoiceTbl_t *vt, u32 ch)
{
    u32 vol;
    u32 msg;

    if (slot->flags & 2) {
        vol = sceMidiChannelReset(trk, ch);
        msg = (vol << 16) | 0x7B0;
        sceMidiSendChMsg(slot, trk, vt, ch | msg, 3);
    }
}
/* ENDF sceMidiSendChannelVolume */
#ifdef NON_MATCHING
/* The two return -1 paths come out as one shared tail where retail has
 * two, so the branch arrangement differs. */
/* FUNC modmidi_sceMidi_MidiSetVolume */
int modmidi_sceMidi_MidiSetVolume(sceMidiWork_t *work, int slotNo, u8 ch, u8 vol)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiVoiceTbl_t *vt;
    u8 i;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    vt = (sceMidiVoiceTbl_t *)&work->tbl->nVoiceWord;
    if (ch < 16) {
        trk->chExpr[ch] = vol;
        sceMidiSendChannelVolume(slot, trk, vt, ch);
        return 0;
    }
    if (ch != 0xFF) {
        return -1;
    }
    trk->vol = vol;
    if (slot->flags & 2) {
        for (i = 0; i < 16; i++) {
            sceMidiSendChannelVolume(slot, trk, vt, i);
        }
    }
    return 0;
}
/* ENDF modmidi_sceMidi_MidiSetVolume */
#else
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_MidiSetVolume);
#endif
#ifdef NON_MATCHING
/* Two instructions longer: gcc reloads trk from its stack slot after the
 * chVol store instead of keeping it in a register. */
/* FUNC modmidi_e18 */
int modmidi_e18(sceMidiWork_t *work, int slotNo, u8 ch, s8 vol)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiVoiceTbl_t *vt;
    u8 v;
    u8 i;

    v = vol;
    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    vt = (sceMidiVoiceTbl_t *)&work->tbl->nVoiceWord;
    if (vol < 0) {
        v = 0x7F;
    }
    if (ch < 16) {
        trk->chVol[ch] = v;
        trk->ch[ch].vol = v;
        sceMidiSendChannelVolume(slot, trk, vt, ch);
        return 0;
    }
    if (ch != 0xFF) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        trk->chVol[i] = v;
        trk->ch[i].vol = v;
        sceMidiSendChannelVolume(slot, trk, vt, i);
    }
    return 0;
}
/* ENDF modmidi_e18 */
#else
INCLUDE_ASM("MODMIDI", modmidi_e18);
#endif
/* FUNC modmidi_e19 */
int modmidi_e19(sceMidiWork_t *work, int slotNo, u8 bpm)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->tempo = 60000000U / bpm;
    sceMidiTrackSeekAbsolute(trk);
    return 0;
}
/* ENDF modmidi_e19 */
/* FUNC modmidi_e20 */
int modmidi_e20(sceMidiWork_t *work, int slotNo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    u32 bpm;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0 || slot->midiNo == -1 ||
        trk->tempo == 0) {
        return 0;
    }
    bpm = 60000000 / trk->tempo;
    if (bpm < 20) {
        bpm = 20;
    } else if (bpm > 255) {
        bpm = 255;
    }
    return (u8)bpm;
}
/* ENDF modmidi_e20 */
/* FUNC modmidi_sceMidi_MidiSetAbsolute */
int modmidi_sceMidi_MidiSetAbsolute(sceMidiWork_t *work, int slotNo, u16 speed)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->speed = speed;
    sceMidiTrackSeekAbsolute(trk);
    return 0;
}
/* ENDF modmidi_sceMidi_MidiSetAbsolute */
/* FUNC modmidi_e22 */
int modmidi_e22(sceMidiWork_t *work, int slotNo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return 0;
    }
    if (slot->midiNo == -1) {
        return 0;
    }
    return (u16)trk->speed;
}
/* ENDF modmidi_e22 */
/* FUNC modmidi_e23 */
int modmidi_e23(sceMidiWork_t *work, int slotNo, int tempo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->tempo = tempo;
    sceMidiTrackSeekAbsolute(trk);
    return 0;
}
/* ENDF modmidi_e23 */
/* FUNC modmidi_e24 */
int modmidi_e24(sceMidiWork_t *work, int slotNo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return 0;
    }
    if (slot->midiNo == -1) {
        return 0;
    }
    return trk->tempo;
}
/* ENDF modmidi_e24 */
/* FUNC sceMidiSongStopNotes */
void sceMidiSongStopNotes(sceMidiWork_t *work, int slotNo, u32 msg, int len)
{
    sceMidiTbl_t *tbl;
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiVoiceTbl_t *vt;
    u32 *seg;
    u32 ofs;
    u32 ch;

    tbl = work->tbl;
    /* The pair at sceMidiSeq_t+8 is a {count, pointer} segment of its own. */
    ofs = (slotNo << 4) | 8;
    seg = (u32 *)(ofs + (u32)tbl->seq);
    slot = (sceMidiSlot_t *)seg[1];
    if (slot != NULL) {
        trk = &slot->trk;
        vt = (sceMidiVoiceTbl_t *)&tbl->nVoiceWord;
        msg &= 0xFFFFF0;
        for (ch = 0; ch < 16; ch++) {
            sceMidiSendChMsg(slot, trk, vt, msg | ch, len);
        }
    }
}
/* ENDF sceMidiSongStopNotes */
ALIGN_TEXT(4);
/* FUNC sceMidiSongApplyVolume */
#ifdef NON_MATCHING
/* Two instructions longer: gcc keeps trk->fadeLeft in $a0 and has to save
 * work in another register. */
void sceMidiSongApplyVolume(sceMidiWork_t *work, int slotNo, sceMidiTrack_t *trk)
{
    int v;
    int t;

    if (trk->fadeLeft != 0) {
        v = (trk->curMidi - trk->nextMidi) * trk->fadeLeft / trk->fadeTicks +
            trk->nextMidi;
    } else {
        v = trk->curMidi;
    }
    t = v * trk->songVol;
    v = t / 128;
    if (trk->unk12F != v) {
        modmidi_sceMidi_MidiSetVolume(work, slotNo, 0xFF, v);
        trk->unk12F = v;
    }
}
#else
INCLUDE_ASM("MODMIDI", sceMidiSongApplyVolume);
#endif
/* ENDF sceMidiSongApplyVolume */
/* FUNC sceMidiSongLookup */
int sceMidiSongLookup(sceMidiWork_t *work, int slotNo, sceMidiSlot_t **pslot, sceMidiTrack_t **ptrk)
{
    sceMidiTbl_t *tbl;
    sceMidiSlot_t *slot;
    u32 unit;

    if (work != NULL) {
        tbl = work->tbl;
        /* nSeqWord counts eight-byte units, so a slot owns units 2n and
         * 2n+1 and its sceMidiSlot_t pointer is the odd one's second word. */
        unit = slotNo * 2 + 1;
        if (unit < tbl->nSeqWord) {
            slot = (sceMidiSlot_t *)((void **)tbl->seq)[unit * 2 + 1];
            *pslot = slot;
            if (pslot == NULL) {
                return 0;
            }
            *ptrk = &slot->trk;
            return 1;
        }
    }
    return 0;
}
/* ENDF sceMidiSongLookup */
/* FUNC sceMidiSongSelect */
void sceMidiSongSelect(sceMidiSlot_t *slot, u8 **songCur)
{
    int i;

    slot->flags &= ~0x700;
    songCur[1] = songCur[0];
    /* songCur is &trk->songData, so index 7 + 2 * i is repeat[i].cursor. */
    for (i = 7; i >= 0; i--) {
        songCur[7 + i * 2] = NULL;
    }
}
/* ENDF sceMidiSongSelect */
/* FUNC modmidi_sceMidi_SelectSong */
int modmidi_sceMidi_SelectSong(sceMidiWork_t *work, int slotNo, u32 songNo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiSongChunk_t *song;
    sceMidiSongChunk_t *cur;
    u32 ofs;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    song = trk->song;
    if (song == NULL) {
        return -1;
    }
    if (song->nSong < songNo) {
        return -1;
    }
    if (song->songOfs[songNo] == -1) {
        return -1;
    }
    if (slot->flags & 0x100) {
        modmidi_e8(work, slotNo, 0);
    }
    cur = trk->song;
    ofs = cur->songOfs[songNo];
    trk->fadeTicks = 0;
    trk->fadeLeft = 0;
    trk->songData = (u8 *)cur + ofs;
    trk->curMidi = 0x80;
    trk->songVol = 0x80;
    trk->unk12F = 0xFF;
    trk->songFlags = 0;
    trk->loopCount = 0x40;
    trk->unk133 = 0x78;
    trk->songSpeed = 0x100;
    slot->songNo = songNo;
    sceMidiSongSelect(slot, &trk->songData);
    return 0;
}
/* ENDF modmidi_sceMidi_SelectSong */
/* FUNC sceMidiSongSetVolume */
void sceMidiSongSetVolume(sceMidiTrack_t *trk, int val)
{
    if (val < 0) {
        val = 0;
    } else if (val > 0x80) {
        val = 0x80;
    }
    trk->curMidi = val;
}
/* ENDF sceMidiSongSetVolume */
/* FUNC sceMidiSongSetLoopCount */
void sceMidiSongSetLoopCount(sceMidiTrack_t *trk, int val)
{
    if (val < 0) {
        val = 0;
    } else if (val > 0x7F) {
        val = 0x7F;
    }
    trk->loopCount = val;
}
/* ENDF sceMidiSongSetLoopCount */
/* FUNC sceMidiSongSetBpm */
void sceMidiSongSetBpm(sceMidiTrack_t *trk, int val)
{
    if (val < 0xA) {
        val = 0xA;
    } else if (val > 0xFF) {
        val = 0xFF;
    }
    trk->unk133 = val;
}
/* ENDF sceMidiSongSetBpm */
/* FUNC sceMidiSongApplyState */
int sceMidiSongApplyState(sceMidiWork_t *work, int slotNo, sceMidiSlot_t *slot, u32 midiNo)
{
    sceMidiTrack_t *trk;

    trk = &slot->trk;
    if (modmidi_sceMidi_SelectMidi(work, slotNo, midiNo) != 0) {
        return 0;
    }
    modmidi_sceMidi_MidiSetAbsolute(work, slotNo, trk->songSpeed);
    modmidi_e19(work, slotNo, trk->unk133);
    if (modmidi_sceMidi_MidiPlaySwitch(work, slotNo, 1) != 0) {
        return 0;
    }
    sceMidiSongApplyVolume(work, slotNo, trk);
    sceMidiSongStopNotes(work, slotNo, (trk->loopCount << 16) | 0xAB0, 3);
    slot->flags |= 0x200;
    return 1;
}
/* ENDF sceMidiSongApplyState */
/* FUNC sceMidiSongRepeat */
#ifdef NON_MATCHING
/* 16 bytes long: gcc cross-jumps the four `return 1` exits into one block
 * at the end, where retail sets the 1 in each exit's branch delay slot. */
int sceMidiSongRepeat(sceMidiSlot_t *slot, u8 count)
{
    sceMidiTrack_t *trk;
    sceMidiRepeat_t *rep;
    u8 *p;
    u8 *cur;
    u32 delta;
    u32 hi;
    u32 lo;
    int i;
    int c;

    trk = &slot->trk;
    p = trk->songCur;
    if (p[0] != 0xA1) {
        if (sceMidiDebug != 0) {
            stdio_printf(aSongrepeatStat, p[0]);
        }
        return 0;
    }
    trk->songCur = p + 1;
    hi = p[1];
    trk->songCur = p + 2;
    lo = p[2];
    trk->songCur = p + 3;
    sceMidiRepeatInfo.total = count;
    sceMidiRepeatInfo.id = (u32)trk->songCur;
    delta = hi << 8;
    delta += lo;
    if (count != 0) {
        cur = trk->songCur;
        rep = trk->repeat;
        for (i = 8; i != 0; i--, rep++) {
            if (rep->cursor != cur) {
                continue;
            }
            c = rep->count - 1;
            rep->count = c;
            if ((u8)c == 0) {
                rep->cursor = NULL;
                return 1;
            }
            if (slot->loopCb == NULL) {
                break;
            }
            sceMidiRepeatInfo.pass = count - c + 1;
            if (slot->loopCb(&sceMidiRepeatInfo, slot->loopArg) != 0) {
                break;
            }
            rep->cursor = NULL;
            return 1;
        }
        if (i == 0) {
            rep = trk->repeat;
            for (i = 8; i != 0; i--, rep++) {
                if (rep->cursor != NULL) {
                    continue;
                }
                sceMidiRepeatInfo.pass = 1;
                if (slot->loopCb != NULL &&
                    slot->loopCb(&sceMidiRepeatInfo, slot->loopArg) == 0) {
                    return 1;
                }
                rep->cursor = cur;
                rep->count = count;
                break;
            }
            if (i == 0) {
                if (sceMidiDebug != 0) {
                    stdio_printf(aSongrepeatRepe);
                }
                return 0;
            }
        }
    } else if (slot->loopCb != NULL) {
        sceMidiRepeatInfo.pass = 0;
        if (slot->loopCb(&sceMidiRepeatInfo, slot->loopArg) == 0) {
            return 1;
        }
    }
    trk->songCur = trk->songData + delta;
    return 1;
}
#else
INCLUDE_ASM("MODMIDI", sceMidiSongRepeat);
#endif
/* ENDF sceMidiSongRepeat */
/* FUNC sceMidiPlaySong */
#ifdef NON_MATCHING
/* Cannot match while .rodata is assembly: the switch compiles to a jump
 * table, and the table in the assembly .rodata already holds this body's
 * case labels, so a table emitted from the C file will not link. */
void sceMidiPlaySong(sceMidiWork_t *work, int slotNo, sceMidiSlot_t *slot)
{
    sceMidiTrack_t *trk;
    u8 *p;
    u8 c;
    u8 cmd;
    u8 arg;

    trk = &slot->trk;
    while (slot->flags & 0x100) {
        p = trk->songCur;
        c = p[0];
        trk->songCur = p + 1;
        if (c != 0xA0) {
            if (sceMidiDebug != 0) {
                stdio_printf(aPlaysongUnknow, c);
            }
            modmidi_e8(work, slotNo, 0);
            return;
        }
        cmd = p[1];
        trk->songCur = p + 2;
        arg = p[2];
        trk->songCur = p + 3;
        switch (cmd) {
        case 0:
            if (sceMidiSongApplyState(work, slotNo, slot, arg) != 0) {
                return;
            }
            trk->songFlags &= 0xFB;
            break;
        case 1:
            sceMidiSongSetVolume(trk, arg);
            break;
        case 2:
            sceMidiSongSetVolume(trk, arg + trk->curMidi);
            break;
        case 3:
            sceMidiSongSetVolume(trk, trk->curMidi - arg);
            break;
        case 4:
            sceMidiSongSetLoopCount(trk, arg);
            break;
        case 5:
            sceMidiSongSetLoopCount(trk, arg + trk->loopCount);
            break;
        case 6:
            sceMidiSongSetLoopCount(trk, trk->loopCount - arg);
            break;
        case 7:
            trk->fadeTicks = arg * 12800000 / sceMidiTickScale;
            break;
        case 8:
            if (trk->curMidi == arg) {
                trk->fadeLeft = 0;
            } else {
                trk->fadeLeft = trk->fadeTicks;
            }
            if (trk->fadeLeft != 0) {
                trk->nextMidi = arg;
            } else {
                sceMidiSongSetVolume(trk, arg);
            }
            break;
        case 0x11:
            if (trk->songFlags & 4) {
                p = trk->songCur;
                c = p[0];
                if (c != 0xA1) {
                    if (sceMidiDebug != 0) {
                        stdio_printf(aPlaysongLoopOf, c);
                    }
                    modmidi_e8(work, slotNo, 0);
                    return;
                }
                trk->songCur = p + 3;
            } else if (sceMidiSongRepeat(slot, arg) == 0) {
                modmidi_e8(work, slotNo, 0);
                return;
            }
            break;
        case 0x21:
            sceMidiSongSetBpm(trk, arg);
            break;
        case 0x22:
            sceMidiSongSetBpm(trk, arg + trk->unk133);
            break;
        case 0x23:
            sceMidiSongSetBpm(trk, trk->unk133 - arg);
            break;
        case 0x7F:
            switch (arg) {
            case 0:
                modmidi_sceMidi_MidiPlaySwitch(work, slotNo, 0);
                sceMidiSongStopNotes(work, slotNo, 0x78B0, 3);
                break;
            case 1:
                modmidi_sceMidi_MidiPlaySwitch(work, slotNo, 0);
                break;
            case 0x7F:
                slot->flags = (slot->flags & ~0x300) | 0x400;
                break;
            default:
                if (sceMidiDebug != 0) {
                    stdio_printf(aPlaysongUnknow_0, arg);
                }
                modmidi_e8(work, slotNo, 0);
                return;
            }
            break;
        default:
            if (sceMidiDebug != 0) {
                stdio_printf(aPlaysongUnknow_1, cmd);
            }
            modmidi_e8(work, slotNo, 0);
            return;
        }
    }
}
#else
INCLUDE_ASM("MODMIDI", sceMidiPlaySong);
#endif
/* ENDF sceMidiPlaySong */
/* FUNC modmidi_e8 */
#ifdef NON_MATCHING
/* Four bytes long: gcc reloads the slot pointer for the shared
 * slot->flags store where retail keeps it in a register, and cross-jumps
 * case 1's MidiPlaySwitch call with the one in case 2. */
int modmidi_e8(sceMidiWork_t *work, int slotNo, int mode)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    u32 f;
    u32 g;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    if (slot->songNo == -1) {
        return -1;
    }
    switch (mode) {
    case 0:
        slot->flags &= ~0x700;
        trk->songFlags = 0;
        modmidi_sceMidi_MidiPlaySwitch(work, slotNo, 0);
        if (trk->fadeLeft != 0) {
            trk->fadeLeft = 0;
            trk->curMidi = trk->nextMidi;
        }
        sceMidiSongSelect(slot, &trk->songData);
        return 0;
    case 1:
        slot->flags &= ~0x100;
        trk->songFlags = 0;
        modmidi_sceMidi_MidiPlaySwitch(work, slotNo, 0);
        return 0;
    case 2:
        if (slot->flags & 0x100) {
            return 0;
        }
        trk->songFlags &= 0xFC;
        f = slot->flags;
        if (f & 0x400) {
            g = f & ~0x300;
        } else if ((f & 0x200) != 0) {
            slot->flags = f | 0x100;
            modmidi_sceMidi_MidiPlaySwitch(work, slotNo, 1);
            return 0;
        } else {
            g = f | 0x100;
        }
        slot->flags = g;
        return 0;
    }
    return -1;
}
#else
INCLUDE_ASM("MODMIDI", modmidi_e8);
#endif
/* ENDF modmidi_e8 */
/* FUNC sceMidiSongServiceTick */
void sceMidiSongServiceTick(sceMidiWork_t *work, int slotNo, sceMidiSlot_t *slot)
{
    sceMidiTrack_t *trk;
    u32 flags;
    u32 keep;
    u8 songFlags;

    if ((slot->flags & 0x100) == 0) {
        return;
    }
    trk = &slot->trk;
    if (trk->fadeLeft != 0) {
        trk->fadeLeft--;
        if (trk->fadeLeft == 0) {
            trk->curMidi = trk->nextMidi;
        }
        sceMidiSongApplyVolume(work, slotNo, trk);
    }
    flags = slot->flags;
    if ((flags & 0x202) == 0x202) {
        return;
    }
    songFlags = trk->songFlags;
    if (songFlags & 1) {
        keep = flags & 0x700;
        if (sceMidiSearchPlay(slot, trk, trk->searchId, songFlags & 2) == 0) {
            return;
        }
        slot->flags |= keep;
    }
    sceMidiPlaySong(work, slotNo, slot);
}
/* ENDF sceMidiSongServiceTick */
/* FUNC sceMidiSongSetMasterVolume */
int sceMidiSongSetMasterVolume(sceMidiWork_t *work, int slotNo, u8 vol)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->songVol = vol;
    if (slot->flags & 0x200) {
        sceMidiSongApplyVolume(work, slotNo, trk);
    }
    return 0;
}
/* ENDF sceMidiSongSetMasterVolume */
/* FUNC modmidi_e10 */
int modmidi_e10(sceMidiWork_t *work, int slotNo, u8 val)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    sceMidiSongSetVolume(trk, val);
    if (slot->flags & 0x200) {
        sceMidiSongApplyVolume(work, slotNo, trk);
    }
    return 0;
}
/* ENDF modmidi_e10 */
/* FUNC modmidi_e11 */
int modmidi_e11(sceMidiWork_t *work, int slotNo, u8 val)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    sceMidiSongSetBpm(trk, val);
    if (slot->flags & 0x200) {
        modmidi_e19(work, slotNo, trk->unk133);
    }
    return 0;
}
/* ENDF modmidi_e11 */
/* FUNC modmidi_sceMidi_SongSetAbsolute */
int modmidi_sceMidi_SongSetAbsolute(sceMidiWork_t *work, int slotNo, u16 speed)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->songSpeed = speed;
    if (slot->flags & 0x200) {
        modmidi_sceMidi_MidiSetAbsolute(work, slotNo, trk->songSpeed);
    }
    return 0;
}
/* ENDF modmidi_sceMidi_SongSetAbsolute */
/* FUNC sceMidiSearchPlay */
#ifdef NON_MATCHING
/* Equivalent, same size: gcc materialises the 0x7F and 0xA1 compare
 * constants inside the loop where retail hoists all four into caller-saved
 * registers, and keeps the two debug printf calls apart where retail
 * cross-jumps them. */
int sceMidiSearchPlay(sceMidiSlot_t *slot, sceMidiTrack_t *trk, u32 target, int flag)
{
    u8 *p;
    u8 *save;
    u32 count;
    u8 c;
    u8 v;

    count = 0;
    sceMidiSongSelect(slot, &trk->songData);
    save = trk->songCur;
    trk->songFlags = 4;
    for (;;) {
        p = trk->songCur;
        c = p[0];
        trk->songCur = p + 1;
        if (c != 0xA0) {
            if (sceMidiDebug != 0) {
                stdio_printf(aSearchplayUnkn);
            }
            goto fail;
        }
        c = p[1];
        trk->songCur = p + 2;
        v = p[2];
        trk->songCur = p + 3;
        switch (c) {
        case 0:
            count++;
            if (target < count) {
                if (flag != 0) {
                    trk->songCur = save;
                } else {
                    trk->songCur = p;
                }
                trk->songFlags = 0;
                return 1;
            }
            save = p + 3;
            break;
        case 0x11:
            if (p[3] != 0xA1) {
                if (sceMidiDebug != 0) {
                    stdio_printf(aSearchplayLoop);
                }
                goto fail;
            }
            trk->songCur = p + 6;
            break;
        case 0x7F:
            if (v == c) {
                goto fail;
            }
            break;
        }
    }
fail:
    slot->flags = (slot->flags & ~0x300) | 0x400;
    trk->songFlags = 0;
    return 0;
}
#else
INCLUDE_ASM("MODMIDI", sceMidiSearchPlay);
#endif
/* ENDF sceMidiSearchPlay */
/* FUNC modmidi_e13 */
int modmidi_e13(sceMidiWork_t *work, int slotNo, u32 a3, u32 a4)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    u32 f;

    if (sceMidiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    if (slot->songNo == -1) {
        return -1;
    }
    f = slot->flags & 0x700;
    if (slot->flags & 0x100) {
        if (a4 & 1) {
            trk->songFlags = (a4 & 2) ? 1 : 3;
            trk->searchId = a3;
            return 0;
        }
        modmidi_sceMidi_MidiPlaySwitch(work, slotNo, 0);
        if (trk->fadeLeft != 0) {
            trk->fadeLeft = 0;
            trk->curMidi = trk->nextMidi;
        }
    }
    if (sceMidiSearchPlay(slot, trk, a3, ((a4 >> 1) ^ 1) & 1) == 0) {
        return -1;
    }
    if (f & 0x100) {
        return modmidi_e8(work, slotNo, 2);
    }
    return 0;
}
/* ENDF modmidi_e13 */
INCLUDE_ASM("MODMIDI", midiLibraryTable);
/* FUNC modmidi_e3 */
void modmidi_e3(void)
{
}
/* ENDF modmidi_e3 */
ALIGN_TEXT(4);
IRX_IMPORT_TABLE(intrman, 1, 2);
IRX_IMPORT(intrman, CpuSuspendIntr, 0x11);
IRX_IMPORT(intrman, CpuResumeIntr, 0x12);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(loadcore, 1, 3);
IRX_IMPORT(loadcore, RegisterLibraryEntries, 0x6);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(stdio, 1, 3);
IRX_IMPORT(stdio, printf, 0x4);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sysclib, 1, 3);
IRX_IMPORT(sysclib, look_ctype_table, 0x8);
IRX_IMPORT(sysclib, strlen, 0x1b);
IRX_IMPORT(sysclib, strncmp, 0x1d);
IRX_IMPORT(sysclib, strtol, 0x24);
IRX_IMPORT_TABLE_END;
