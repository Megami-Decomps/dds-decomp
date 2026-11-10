#include "MODHSYN.h"

extern u8 hsynLibraryTable[];

/* MODHSYN.IRX, Sony's SPU2 synthesiser: it owns the voices and the envelopes,
 * and a .SMG's bank is loaded into it. See docs/irx-modules.md.
 *
 * The chunks still in assembly are the ones no C has been written for yet,
 * plus the import stubs and the import and export tables between them. */

/* FUNC modhsyn_e30 */
int modhsyn_e30()
{
    s32 state;
    s32 res;

    intrman_CpuSuspendIntr(&state);
    res = loadcore_RegisterLibraryEntries(hsynLibraryTable);
    intrman_CpuResumeIntr(state);
    return res != 0;
}
/* ENDF modhsyn_e30 */
ALIGN_TEXT(4);
/* FUNC modhsyn_sceHSyn_Init */
int modhsyn_sceHSyn_Init(sceHSynWork_t *work, int interval)
{
    sceHSynTbl_t *tbl;
    sceHSynEntry_t *entry;
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynBank_t *bank;
    u32 *mask;
    u32 n;
    int slotNo;
    int i;
    int j;

    sceHsynStat = NULL;
    sceHsynTickScale = ((u32)interval << 8) / 1000;
    if (work == NULL) {
        return -1;
    }
    tbl = work->tbl;
    if (work->nTblWord == 0) {
        return -1;
    }
    if (tbl == NULL) {
        return -1;
    }
    n = tbl->nEntryWord;
    entry = tbl->entry;
    if ((n & 1) != 0) {
        return -1;
    }
    if (entry == NULL) {
        return -1;
    }
    for (slotNo = 0; slotNo * 2 + 1 < (int)tbl->nEntryWord; slotNo++) {
        if (entry[slotNo].seq == NULL) {
            continue;
        }
        slot = entry[slotNo].slot;
        if (slot == NULL) {
            continue;
        }
        slot->maxVoice = 0x30;
        slot->voiceBase = 0;
        slot->flags = 0;
        slot->path = 0;
        slot->unk04 = 0;
        slot->unk08 = 0;
        slot->unk0C = 0;
        slot->unk10 = NULL;
        set = (sceHSynChanSet_t *)&slot->unk14;
        set->unk00 = 0x100;
        set->nActive = 0;
        for (i = 0; i < 16; i++) {
            bank = &set->bank[i];
            bank->head = NULL;
            bank->prog = NULL;
            bank->sset = NULL;
            bank->smpl = NULL;
            bank->vagi = NULL;
            bank->setb = NULL;
            bank->spuAddr = 0;
        }
        for (j = 0; j < 16; j++) {
            sceHsynResetState(&set->chan[j], slot);
        }
    }
    sceHsynInitCores();
    sceHsynLastKeyOnVoice = 0xFF;
    mask = sceHsynVoiceMask;
    mask[1] = 0;
    mask[0] = 0;
    sceHsynEnabled = 0;
    sceHsynSdSyncFlag = 1;
    sceHsynModDisable = 0;
    sceHsynRelCount = 0;
    sceHsynActiveCount = 0;
    return 0;
}
/* ENDF modhsyn_sceHSyn_Init */
/* FUNC modhsyn_e7 */
int modhsyn_e7(s16 core, u8 *iopAddr, u32 spuAddr, u32 size)
{
    u32 end;
    int done;

    end = (size >> 6) << 6;
    if ((size & 0x3F) != 0) {
        end += 0x40;
    }
    sceHsynSdVoiceTransStatus(core, 1);
    done = sceHsynSdVoiceTrans(core, 0, iopAddr, spuAddr, size);
    sceHsynSdVoiceTransStatus(core, 1);
    return (done == end) ? 0 : -1;
}
/* ENDF modhsyn_e7 */
ALIGN_TEXT(4);
/* FUNC sceHsynTagMatch */
/* Not a loader: a 4+4 character chunk tag comparator, with both tags passed
 * as words and both strings read backwards. */
int sceHsynTagMatch(u32 tag0, u32 tag1, const char *magic, const char *name)
{
    char *p0;
    char *p1;
    u32 i;

    p0 = (char *)&tag0;
    p1 = (char *)&tag1;
    for (i = 0; i < 4; i++) {
        if (p0[i] != magic[3 - i] || p1[i] != name[3 - i]) {
            return 0;
        }
    }
    return 1;
}
/* ENDF sceHsynTagMatch */
#ifdef NON_MATCHING
/* Six instructions short: gcc cross-jumps the two identical bank-clearing
 * blocks that retail keeps twice. */
/* FUNC modhsyn_sceHSyn_Load */
int modhsyn_sceHSyn_Load(sceHSynWork_t *work, int slotNo, u32 spuAddr, sceSmgVers_t *img, u32 bankIdx)
{
    sceHSynTbl_t *tbl;
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynBank_t *bank;
    sceSmgHead_t *head;
    u32 unit;

    if (work == NULL || bankIdx >= 0x10) {
        return -1;
    }
    tbl = work->tbl;
    unit = slotNo * 2 + 1;
    if (unit >= tbl->nEntryWord) {
        return -1;
    }
    slot = *(sceHSynSlot_t **)((u8 *)tbl->entry + (unit << 3) + 4);
    set = (sceHSynChanSet_t *)&slot->unk14;
    if (slot->path == 1) {
        bankIdx = 0;
    }
    bank = &set->bank[bankIdx];
    if (spuAddr != 0 && img != NULL) {
        goto load;
    }
    bank->head = NULL;
    bank->prog = NULL;
    bank->sset = NULL;
    bank->smpl = NULL;
    bank->vagi = NULL;
    bank->setb = NULL;
    bank->spuAddr = 0;
    return 0;
load:
    bank->head = (sceSmgHead_t *)((u8 *)img + 0x10);
    if (bank->head->progOfs != -1) {
        bank->prog = (sceSmgProg_t *)((u8 *)img + bank->head->progOfs);
    }
    if (bank->head->ssetOfs != -1) {
        bank->sset = (sceSmgSset_t *)((u8 *)img + bank->head->ssetOfs);
    }
    if (bank->head->smplOfs != -1) {
        bank->smpl = (sceSmgSmpl_t *)((u8 *)img + bank->head->smplOfs);
    }
    if (bank->head->vagiOfs != -1) {
        bank->vagi = (sceSmgVagi_t *)((u8 *)img + bank->head->vagiOfs);
    }
    if (img->version >= 2) {
        if (bank->head->setbOfs != -1) {
            bank->setb = (sceSmgSetb_t *)((u8 *)img + bank->head->setbOfs);
        }
    }
    if (sceHsynTagMatch(*(u32 *)img->magic, *(u32 *)img->name, magic, name) == 0) {
        goto fail;
    }
    head = bank->head;
    if (sceHsynTagMatch(*(u32 *)head->magic, *(u32 *)head->name, magic, aHead) == 0) {
        goto fail;
    }
    if (bank->prog != NULL) {
        if (sceHsynTagMatch(*(u32 *)bank->prog->magic, *(u32 *)bank->prog->name, magic, aProg) == 0) {
            goto fail;
        }
    }
    if (bank->sset != NULL) {
        if (sceHsynTagMatch(*(u32 *)bank->sset->magic, *(u32 *)bank->sset->name, magic, aSset) == 0) {
            goto fail;
        }
    }
    if (bank->smpl != NULL) {
        if (sceHsynTagMatch(*(u32 *)bank->smpl->magic, *(u32 *)bank->smpl->name, magic, aSmpl) == 0) {
            goto fail;
        }
    }
    if (bank->vagi != NULL) {
        if (sceHsynTagMatch(*(u32 *)bank->vagi->magic, *(u32 *)bank->vagi->name, magic, aVagi) == 0) {
            goto fail;
        }
    }
    if (bank->setb == NULL) {
        goto ok;
    }
    if (sceHsynTagMatch(*(u32 *)bank->setb->magic, *(u32 *)bank->setb->name, magic, aSetb) != 0) {
        goto ok;
    }
fail:
    bank->head = NULL;
    bank->prog = NULL;
    bank->sset = NULL;
    bank->smpl = NULL;
    bank->vagi = NULL;
    bank->setb = NULL;
    bank->spuAddr = 0;
    return -1;
ok:
    bank->spuAddr = spuAddr;
    return 0;
}
#else
INCLUDE_ASM("MODHSYN", modhsyn_sceHSyn_Load);
#endif
/* ENDF modhsyn_sceHSyn_Load */
/* FUNC modhsyn_e28 */
int modhsyn_e28(sceHSynWork_t *work, int slotNo, u32 bankIdx)
{
    sceHSynTbl_t *tbl;
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynBank_t *bank;
    u32 unit;
    int i;
    int ofs;

    if (work == NULL || bankIdx >= 0x10) {
        return -1;
    }
    tbl = work->tbl;
    unit = slotNo * 2 + 1;
    if (unit >= tbl->nEntryWord) {
        return -1;
    }
    slot = (sceHSynSlot_t *)((void **)tbl->entry)[unit * 2 + 1];
    if (slot == NULL) {
        return -1;
    }
    set = (sceHSynChanSet_t *)&slot->unk14;
    if (slot->path == 1) {
        bankIdx = 0;
    }
    bank = &set->bank[bankIdx];
    bank->head = NULL;
    bank->prog = NULL;
    bank->sset = NULL;
    bank->smpl = NULL;
    bank->vagi = NULL;
    bank->setb = NULL;
    bank->spuAddr = 0;
    for (i = 0, ofs = 4; i < 16; i++, ofs += 0x38) {
        sceHsynResetState((sceHSynChannel_t *)((char *)set + ofs), slot);
    }
    return 0;
}
/* ENDF modhsyn_e28 */
ALIGN_TEXT(4);
/* FUNC sceHsynResetState */
void sceHsynResetState(sceHSynChannel_t *chan, sceHSynSlot_t *slot)
{
    sceHSynVoice_t *v;

    v = (sceHSynVoice_t *)((char *)chan - 8);
    chan->onPrev = v;
    chan->onNext = v;
    v = (sceHSynVoice_t *)((char *)chan + 8);
    chan->offPrev = v;
    chan->offNext = v;
    chan->relPrev = (sceHSynVoice_t *)chan;
    chan->relNext = (sceHSynVoice_t *)chan;
    chan->prog = NULL;
    chan->slot = slot;
    chan->bank = NULL;
    chan->unk32 = 0;
    chan->unk26 = 0;
    chan->bendRange = 0;
    chan->unk28 = -1;
    chan->bankIdx = 0;
    chan->vol = 0x80;
    chan->expression = 0x80;
    chan->unk2D = 0;
    chan->unk2F = 0xFF;
    chan->unk30 = -1;
    chan->unk2E = 0;
    chan->pan = 0;
    chan->portaNote = 0xFF;
    chan->unk37 = 0xFF;
    chan->unk36 = 0xFF;
    chan->unk35 = 0xFF;
}
/* ENDF sceHsynResetState */
/* FUNC sceHsynChannelSetProgram */
void sceHsynChannelSetProgram(sceHSynChanSet_t *set, sceHSynChannel_t *chan, u8 prog)
{
    sceSmgProgEnt_t *ent;
    sceHSynBank_t *bank;
    sceSmgProg_t *p;

    ent = NULL;
    bank = NULL;
    if (chan->bankIdx < 16) {
        bank = &set->bank[chan->bankIdx];
        p = bank->prog;
        if (p != NULL && p->nEntry >= prog && p->ofs[prog] != -1) {
            ent = (sceSmgProgEnt_t *)((char *)p + p->ofs[prog]);
            if (ent->nKey == 0) {
                ent = NULL;
            }
        }
    }
    chan->prog = ent;
    if (ent != NULL) {
        chan->bank = bank;
    } else {
        chan->bank = NULL;
    }
}
/* ENDF sceHsynChannelSetProgram */
#ifdef NON_MATCHING
/* Two instructions swapped: gcc schedules the head pointer ahead of the
 * saved bend value. */
/* FUNC sceHsynChannelSetPitchBend */
void sceHsynChannelSetPitchBend(sceHSynChannel_t *chan, u8 hi, u8 lo)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    s16 bend;
    int i;
    sceHSynVoice_t **head;

    bend = (hi << 7) + lo - 0x2000;
    if (chan->unk26 == bend) {
        return;
    }
    chan->unk26 = bend;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            sceHsynSdSetParamPitch(v, bend);
        }
    }
}
/* ENDF sceHsynChannelSetPitchBend */
#else
INCLUDE_ASM("MODHSYN", sceHsynChannelSetPitchBend);
#endif
/* FUNC sceHsynChannelApplyVolume */
void sceHsynChannelApplyVolume(sceHSynChannel_t *chan, int copyRelease)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;
    sceHSynVoice_t **head;

    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            if (copyRelease != 0) {
                v->unk54 = chan->expression;
            }
            sceHsynSdSetParamEnvelope(v);
        }
    }
}
/* ENDF sceHsynChannelApplyVolume */
/* FUNC sceHsynChannelSetVolume */
void sceHsynChannelSetVolume(sceHSynChannel_t *chan, u8 val)
{
    u8 n;

    n = (val << 7) / 127;
    if (chan->vol != n) {
        chan->vol = n;
        sceHsynChannelApplyVolume(chan, 0);
    }
}
/* ENDF sceHsynChannelSetVolume */
/* FUNC sceHsynChannelSetExpression */
void sceHsynChannelSetExpression(sceHSynChannel_t *chan, u8 val)
{
    u8 n;

    n = (val << 7) / 127;
    if (chan->expression != n) {
        chan->expression = n;
        sceHsynChannelApplyVolume(chan, 1);
    }
}
/* ENDF sceHsynChannelSetExpression */
/* FUNC sceHsynVoiceSetVolumeLeft */
void sceHsynVoiceSetVolumeLeft(sceHSynChannel_t *chan, u8 val)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;
    sceHSynVoice_t **head;
    u16 n;

    n = (val << 7) / 127;
    if (chan->unk2D == n) {
        return;
    }
    chan->unk2D = n;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            sceHsynSdSetParamVolumeLeft(v, n);
        }
    }
}
/* ENDF sceHsynVoiceSetVolumeLeft */
/* FUNC sceHsynVoiceSetVolumeRight */
void sceHsynVoiceSetVolumeRight(sceHSynChannel_t *chan, u8 val)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;
    sceHSynVoice_t **head;
    u16 n;

    n = (val << 7) / 127;
    if (chan->unk2E == n) {
        return;
    }
    chan->unk2E = n;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            sceHsynSdSetParamVolumeRight(v, n);
        }
    }
}
/* ENDF sceHsynVoiceSetVolumeRight */
/* FUNC sceHsynChannelSetBendRange */
int sceHsynChannelSetBendRange(sceHSynChannel_t *chan, u8 semitones)
{
    chan->bendRange = semitones * 20;
}
/* ENDF sceHsynChannelSetBendRange */
/* FUNC sceHsynChannelSetPortamento */
void sceHsynChannelSetPortamento(sceHSynChannel_t *chan, u8 on)
{
    if (on != 0) {
        if (chan->portaNote == 0xFF) {
            chan->portaNote = 0x80;
            chan->unk34 = 0x80;
        }
    } else {
        chan->portaNote = 0xFF;
    }
}
/* ENDF sceHsynChannelSetPortamento */
/* FUNC sceHsynChannelSetPortamentoNote */
void sceHsynChannelSetPortamentoNote(sceHSynChannel_t *chan, u8 val)
{
    chan->unk34 = val;
}
/* ENDF sceHsynChannelSetPortamentoNote */
#ifdef NON_MATCHING
/* Two instructions swapped, as in sceHsynChannelSetPitchBend: gcc schedules the head
 * pointer ahead of the saved value the loop passes on. */
/* FUNC sceHsynChannelSetPan */
void sceHsynChannelSetPan(sceHSynChannel_t *chan, u8 val)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;
    sceHSynVoice_t **head;
    s8 n;

    n = val - 0x40;
    if (n < -0x3F) {
        n = -0x3F;
    }
    if (chan->pan == n) {
        return;
    }
    chan->pan = n;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            sceHsynVoiceSetPan(v, n);
        }
    }
}
/* ENDF sceHsynChannelSetPan */
#else
INCLUDE_ASM("MODHSYN", sceHsynChannelSetPan);
#endif
#ifdef NON_MATCHING
/* Same instructions, two registers swapped: retail keeps the clamped pan in
   $2 and the sign-extended test value in the argument register. */
/* FUNC sceHsynChannelSetPanDirect */
void sceHsynChannelSetPanDirect(sceHSynChannel_t *chan, u8 ch, u32 tag, int val)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int all;
    int pan;
    int i;

    val -= 0x40;
    pan = val;
    if ((s8)val < -0x3F) {
        pan = -0x3F;
    }
    all = ch & 0x80;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chNext; v != end; v = v->chNext) {
            if (tag != 0x7F && v->tag != tag) {
                continue;
            }
            if (all == 0 && v->ch != ch) {
                continue;
            }
            sceHsynVoiceSetPan(v, (s8)pan);
        }
    }
}
/* ENDF sceHsynChannelSetPanDirect */
#else
INCLUDE_ASM("MODHSYN", sceHsynChannelSetPanDirect);
#endif
#ifdef NON_MATCHING
/* One instruction short: retail forms the unsigned high half of the divide
   with a signed multiply plus a correction, where gcc here uses multu. */
/* FUNC sceHsynChannelSetExpressionDirect */
void sceHsynChannelSetExpressionDirect(sceHSynChannel_t *chan, u8 ch, u32 tag, u8 val)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    u32 scaled;
    int all;
    int i;

    all = ch & 0x80;
    heads[1] = (sceHSynVoice_t *)chan;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    scaled = ((u32)val << 8) / 254;
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chNext; v != end; v = v->chNext) {
            if (tag != 0x7F && v->tag != tag) {
                continue;
            }
            if (all == 0 && v->ch != ch) {
                continue;
            }
            v->unk54 = scaled;
            sceHsynSdSetParamEnvelope(v);
        }
    }
}
/* ENDF sceHsynChannelSetExpressionDirect */
#else
INCLUDE_ASM("MODHSYN", sceHsynChannelSetExpressionDirect);
#endif
/* FUNC sceHsynVoiceSetPitchDirect */
void sceHsynVoiceSetPitchDirect(sceHSynChannel_t *chan, u8 ch, u32 tag, u8 hi, u8 lo)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    s16 bend;
    int i;

    bend = (hi << 7) + lo - 0x2000;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chNext; v != end; v = v->chNext) {
            if (tag != 0x7F && v->tag != tag) {
                continue;
            }
            if ((ch & 0x80) == 0 && v->ch != ch) {
                continue;
            }
            sceHsynSdSetParamPitch(v, bend);
        }
    }
}
/* ENDF sceHsynVoiceSetPitchDirect */
ALIGN_TEXT(4);
/* FUNC sceHsynChannelResetControllers */
void sceHsynChannelResetControllers(sceHSynChannel_t *chan)
{
    sceHsynChannelSetHold(chan, 0);
    sceHsynChannelSetPitchBend(chan, 0x40, 0);
    sceHsynVoiceSetVolumeLeft(chan, 0);
    sceHsynVoiceSetVolumeRight(chan, 0);
    sceHsynChannelSetVolume(chan, 0x7F);
    sceHsynChannelSetPan(chan, 0x40);
    sceHsynChannelSetExpression(chan, 0x7F);
    sceHsynChannelSetPortamento(chan, 0);
}
/* ENDF sceHsynChannelResetControllers */
#ifdef NON_MATCHING
/* The switch on the controller number compiles to a jump table, and the
 * assembly .rodata already holds its case labels, so a C body here will not
 * link. */
/* FUNC sceHsynVoiceEventDispatch */
void sceHsynVoiceEventDispatch(int set, sceHSynChannel_t *chan, u8 cc, u8 val)
{
    switch (cc) {
    case 0:
        chan->bankIdx = val;
        break;
    case 1:
        sceHsynVoiceSetVolumeRight(chan, val);
        break;
    case 2:
        sceHsynVoiceSetVolumeLeft(chan, val);
        break;
    case 5:
        sceHsynChannelSetBendRange(chan, val);
        break;
    case 7:
        sceHsynChannelSetVolume(chan, val);
        break;
    case 10:
        sceHsynChannelSetPan(chan, val);
        break;
    case 11:
        sceHsynChannelSetExpression(chan, val);
        break;
    case 64:
        sceHsynChannelSetHold(chan, val);
        break;
    case 65:
        sceHsynChannelSetPortamento(chan, val);
        break;
    case 84:
        sceHsynChannelSetPortamentoNote(chan, val);
        break;
    case 120:
        sceHsynChannelAllSoundOff(chan);
        break;
    case 121:
        sceHsynChannelResetControllers(chan);
        break;
    case 123:
        sceHsynChannelAllNotesOff(chan);
        break;
    case 99:
        chan->unk35 = val;
        chan->unk37 = 0xFF;
        chan->unk36 = 0xFF;
        break;
    case 98:
        chan->unk36 = val;
        break;
    case 6:
        chan->unk37 = val;
        if (chan->unk36 != 0xFF) {
            if (chan->unk35 == 2) {
                sceHsynEffectAttrRead(chan->unk36, chan->unk37);
            } else if (chan->unk35 == 3) {
                sceHsynEffectAttrWrite(chan->unk36, chan->unk37);
            }
        }
        chan->unk37 = 0xFF;
        chan->unk36 = 0xFF;
        chan->unk35 = 0xFF;
        break;
    }
}
/* ENDF sceHsynVoiceEventDispatch */
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceEventDispatch);
#endif
#ifdef NON_MATCHING
/* One instruction short: retail copies the note argument out of its argument
   register so the next argument's load can reuse it. */
/* FUNC sceHsynVoiceUpdatePass */
void sceHsynVoiceUpdatePass(sceHSynSlot_t *slot, int status, char sel, u8 note, u8 vel)
{
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    int cmd;

    set = (sceHSynChanSet_t *)&slot->unk14;
    chan = &set->chan[sel & 0xF];
    cmd = sel & 0xF0;
    switch (cmd) {
    case 0x80:
        sceHsynChannelNoteOff(chan, note, 0xFF);
        break;
    case 0x90:
        if (vel != 0) {
            sceHsynChannelNoteOn(slot, chan, status, note, vel, 0xFF);
        } else {
            sceHsynChannelNoteOff(chan, note, 0xFF);
        }
        break;
    case 0xA0:
        break;
    case 0xB0:
        sceHsynVoiceEventDispatch((int)set, chan, note, vel);
        break;
    case 0xC0:
        sceHsynChannelSetProgram(set, chan, note);
        break;
    case 0xD0:
        break;
    case 0xE0:
        sceHsynChannelSetPitchBend(chan, vel, note);
        break;
    }
}
/* ENDF sceHsynVoiceUpdatePass */
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceUpdatePass);
#endif
/* FUNC sceHsynVoiceRefreshHook */
void sceHsynVoiceRefreshHook(sceHSynSlot_t *slot, u8 *data, int len)
{
}
/* ENDF sceHsynVoiceRefreshHook */
/* FUNC sceHsynVoiceDirectPass */
void sceHsynVoiceDirectPass(sceHSynSlot_t *slot, int status, u8 sel, u8 *p)
{
    sceHSynChannel_t *chan;
    u8 a;
    u8 b;
    u8 tag;
    u32 note;
    int val;
    int vel;

    a = *p++;
    b = *p++;
    val = *p++;
    chan = (sceHSynChannel_t *)((u8 *)slot + (b & 0xF) * sizeof(sceHSynChannel_t) + 0x18);
    switch (sel) {
    case 0xF9:
        switch (a) {
        case 0:
            chan->unk2F = val;
            break;
        case 1:
            chan->unk30 = val;
            break;
        case 2:
            chan->unk28 = val + (*p << 7);
            break;
        }
        break;
    case 0xFD:
        note = val;
        tag = *p++;
        val = *p++;
        if ((b & 0x10) != 0) {
            note |= 0x80;
        }
        switch (a) {
        case 0x10:
            vel = val & 0xFF;
            note &= 0x7F;
            if (vel != 0) {
                sceHsynChannelNoteOn(slot, chan, status, note, vel, tag);
            } else {
                sceHsynChannelNoteOff(chan, note, tag);
            }
            break;
        case 0:
            sceHsynChannelSetExpressionDirect(chan, note, tag, val);
            break;
        case 1:
            sceHsynChannelSetPanDirect(chan, note, tag, val);
            break;
        case 2:
            sceHsynVoiceSetPitchDirect(chan, note, tag, *p, val);
            break;
        }
        break;
    }
}
/* ENDF sceHsynVoiceDirectPass */
/* FUNC sceHsynVoiceRefresh */
#ifdef NON_MATCHING
/* Four bytes short: retail shares one `p < end` test block, placed before
 * the body with the function entry jumping over it, and each continue pays a
 * `j` plus the compare; gcc keeps the test at the bottom of the loop. */
void sceHsynVoiceRefresh(sceHSynSlot_t *slot, int idx, u8 *p, u8 *end)
{
    u8 *start;
    u8 c;
    u8 d1;
    int d2;
    int len;
    int step;

    do {
        c = *p++;
        if ((c & 0x80) == 0) {
            continue;
        }
        if (c < 0xF0) {
            if (p >= end) {
                return;
            }
            d2 = 0;
            d1 = *p++;
            if (sceHsynStatusFlags[(c - 0x80) >> 4] != 0) {
                if (p >= end) {
                    return;
                }
                d2 = *p++;
            }
            sceHsynVoiceUpdatePass(slot, idx, c, d1, d2);
            continue;
        }
        step = 4;
        if (c == 0xF0) {
            if (p >= end) {
                return;
            }
            start = p;
            len = 0;
            while (*p++ != 0xF7) {
                if (p >= end) {
                    return;
                }
                len++;
            }
            sceHsynVoiceRefreshHook(slot, start, len);
            continue;
        }
        if (c != 0xF9) {
            if (c != 0xFD) {
                continue;
            }
            step = 6;
        }
        p += step;
        if (p > end) {
            continue;
        }
        sceHsynVoiceDirectPass(slot, idx, c, p);
    } while (p < end);
}
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceRefresh);
#endif
/* ENDF sceHsynVoiceRefresh */
#ifdef NON_MATCHING
/* Two instructions short: gcc bases the entry loads on entry+0xC
 * instead of keeping the +8 segment pointer live. */
/* FUNC sceHsynServiceTick */
void sceHsynServiceTick(sceHSynEntry_t *entry, int nEntryWord)
{
    sceHSynSeq_t *seq;
    sceHSynSlot_t *slot;
    u32 *p;
    u32 *seg;
    u8 *data;
    u8 *end;
    int i;
    int w;
    int n;

    i = 0;
    p = (u32 *)entry;
    for (w = 1; w < nEntryWord; w += 2) {
        seg = p + 2;
        seq = (sceHSynSeq_t *)p[1];
        slot = (sceHSynSlot_t *)seg[1];
        if (seq != NULL && slot != NULL) {
            data = seq->data;
            n = seq->len + 8;
            end = (u8 *)seq + n;
            if (data < end) {
                seq->len = 0;
                switch (slot->flags & 1) {
                case 0:
                    sceHsynVoiceRefresh((sceHSynSlot_t *)seg[1], i, data, end);
                    break;
                case 1:
                    sceHsynCmdDispatch((sceHSynSlot_t *)seg[1], i, data, end);
                    break;
                }
            }
        }
        p += 4;
        i++;
    }
}
/* ENDF sceHsynServiceTick */
#else
INCLUDE_ASM("MODHSYN", sceHsynServiceTick);
#endif
/* FUNC sceHsynServiceAll */
int sceHsynServiceAll(sceHSynWork_t *work)
{
    sceHSynTbl_t *tbl;
    u32 vm;
    u32 inv;
    int core;
    int nFree;
    int nActive;

    if (sceHsynLastKeyOnVoice != 0xFF) {
        if (sceHsynSpu2SettleDelay() != 0) {
            sceHsynLastKeyOnVoice = 0xFF;
        }
    }
    for (core = 0; core < 2; core++) {
        inv = ~sceHsynVoiceMask[core];
        sceHsynMixMaskL[core][0] &= inv;
        sceHsynMixMaskL[core][1] &= inv;
        sceHsynMixMaskR[core][0] &= inv;
        sceHsynMixMaskR[core][1] &= inv;
        vm = sceHsynVoiceMask[core];
        if (vm != 0) {
            sceHsynMixMaskL[core][0] |= sceHsynSdGetSwitch(core | 0x1800) & vm;
            sceHsynMixMaskL[core][1] |= sceHsynSdGetSwitch(core | 0x1A00) & vm;
            sceHsynMixMaskR[core][0] |= sceHsynSdGetSwitch(core | 0x1900) & vm;
            sceHsynMixMaskR[core][1] |= sceHsynSdGetSwitch(core | 0x1B00) & vm;
        }
    }
    tbl = work->tbl;
    sceHsynServiceTick(tbl->entry, tbl->nEntryWord);
    nFree = sceHsynServiceVoices();
    nActive = sceHsynUpdateVoiceState();
    if (sceHsynStat != NULL) {
        sceHsynStat->nFree = nFree;
        sceHsynStat->nActive = nActive;
    }
    return 0;
}
/* ENDF sceHsynServiceAll */
/* FUNC modhsyn_e8 */
int modhsyn_e8(int *masks)
{
    sceHSynVoice_t *freeEnd;
    sceHSynVoice_t *relEnd;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    u8 *cnt;
    u32 mask;
    int i;

    for (i = 0; i < 2; i++) {
        freeEnd = (sceHSynVoice_t *)sceHsynFreeList[i];
        relEnd = (sceHSynVoice_t *)sceHsynRelList[i];
        cnt = &sceHsynFreeCount[i];
        mask = masks[i];
        sceHsynVoiceMask[i] = mask;
        for (v = relEnd->next; v != relEnd; v = relEnd->next) {
            sceHsynVoiceUnlink(v);
            sceHsynVoiceInsertBefore(v, freeEnd);
            (*cnt)++;
        }
        for (v = freeEnd->next; v != freeEnd; v = v->next) {
            if (mask & (1 << v->index)) {
                next = v->prev;
                sceHsynVoiceUnlink(v);
                sceHsynVoiceInsertAfter(v, relEnd);
                (*cnt)--;
                v = next;
            }
        }
    }
    return 0;
}
/* ENDF modhsyn_e8 */
/* FUNC modhsyn_e26 */
int modhsyn_e26(u32 *mask)
{
    mask[0] = sceHsynVoiceMask[0];
    mask[1] = sceHsynVoiceMask[1];
    return 0;
}
/* ENDF modhsyn_e26 */
#ifdef NON_MATCHING
/* Three registers apart: retail keeps the unit index in the dead slotNo
 * register, gcc reuses the one holding slotNo * 2. */
/* FUNC sceHsynParamSlotResolve */
int sceHsynParamSlotResolve(sceHSynWork_t *work, int slotNo, sceHSynSlot_t **pslot, sceHSynChanSet_t **pset)
{
    sceHSynTbl_t *tbl;
    sceHSynSlot_t *slot;
    u32 unit;

    if (work != NULL) {
        tbl = work->tbl;
        /* nEntryWord counts eight-byte units, so a slot owns units 2n and
         * 2n+1 and its sceHSynSlot_t pointer is the odd one's second word. */
        unit = slotNo * 2 + 1;
        if (unit < tbl->nEntryWord) {
            slot = (sceHSynSlot_t *)((void **)tbl->entry)[unit * 2 + 1];
            *pslot = slot;
            *pset = (sceHSynChanSet_t *)&slot->unk14;
            return 1;
        }
    }
    return 0;
}
/* ENDF sceHsynParamSlotResolve */
#else
INCLUDE_ASM("MODHSYN", sceHsynParamSlotResolve);
#endif
/* FUNC modhsyn_e10 */
int modhsyn_e10(sceHSynWork_t *work, int slotNo, s16 val)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    set->unk00 = val;
    return 0;
}
/* ENDF modhsyn_e10 */
/* FUNC modhsyn_e11 */
int modhsyn_e11(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return 0;
    }
    return set->unk00;
}
/* ENDF modhsyn_e11 */
/* FUNC modhsyn_e12 */
int modhsyn_e12(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    int i;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        sceHsynChannelAllNotesOff(&set->chan[i]);
    }
    return 0;
}
/* ENDF modhsyn_e12 */
/* FUNC modhsyn_e13 */
int modhsyn_e13(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    int i;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        sceHsynChannelAllSoundOff(&set->chan[i]);
    }
    return 0;
}
/* ENDF modhsyn_e13 */
/* FUNC modhsyn_e14 */
int modhsyn_e14(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    int i;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        sceHsynChannelResetControllers(&set->chan[i]);
    }
    return 0;
}
/* ENDF modhsyn_e14 */
/* FUNC modhsyn_e9 */
int modhsyn_e9(sceHSynCore_t *c)
{
    sceSdEffectAttr_t attr;
    int on;

    attr.core = c->core;
    attr.depth_L = c->depthL;
    attr.depth_R = c->depthR;
    attr.delay = c->delay;
    attr.feedback = c->feedback;
    attr.mode = sceHsynChannelMap[c->mode & 0xF];
    on = attr.mode != 0;
    if ((c->mode & 0x100) != 0) {
        attr.mode |= 0x100;
    }
    sceHsynSdSetEffectAttr(c->core, &attr);
    sceHsynSdSetCoreAttr((u16)c->core | 2, on);
    sceHsynSdSetParam((u16)c->core | 0xB80, c->volL);
    sceHsynSdSetParam((u16)c->core | 0xC80, c->volR);
    return 0;
}
/* ENDF modhsyn_e9 */
/* FUNC modhsyn_e15 */
int modhsyn_e15(sceHSynStat_t *stat)
{
    sceHsynStat = stat;
    return 0;
}
/* ENDF modhsyn_e15 */
/* FUNC sceHsynEffectAttrRead */
void sceHsynEffectAttrRead(u8 sel, u8 mode)
{
    sceSdEffectAttr_t attr;
    int core;
    int t;

    if (mode >= 10) {
        return;
    }
    if (sel != 0 && sel != 0x10) {
        return;
    }
    t = sel & 0x10;
    core = t != 0;
    sceHsynSdGetEffectAttr(core, &attr);
    attr.mode = mode | 0x100;
    sceHsynSdSetEffectAttr(core, &attr);
}
/* ENDF sceHsynEffectAttrRead */
/* FUNC sceHsynEffectAttrWrite */
void sceHsynEffectAttrWrite(u8 sel, u8 val)
{
    sceSdEffectAttr_t attr;
    int core;
    int v;
    int t;

    if ((sel & 0xE0) != 0) {
        return;
    }
    t = sel & 0x10;
    core = t != 0;
    sceHsynSdGetEffectAttr(core, &attr);
    switch (sel & 0xF) {
    case 0:
        v = (val & 0xFF) << 8;
        attr.depth_R = v;
        attr.depth_L = v;
        break;
    case 1:
        v = -((val & 0xFF) << 8);
        attr.depth_R = v;
        attr.depth_L = v;
        break;
    case 2:
        attr.delay = val & 0xFF;
        break;
    case 3:
        attr.feedback = val & 0xFF;
        break;
    }
    sceHsynSdSetEffectAttr(core, &attr);
}
/* ENDF sceHsynEffectAttrWrite */
/* FUNC modhsyn_e17 */
int modhsyn_e17(sceHSynWork_t *work, int slotNo, u8 *out)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    u8 flags;
    int on;
    int i;
    int ofs;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    for (i = 0, ofs = 4; i < 16; i++, ofs += 0x38) {
        chan = (sceHSynChannel_t *)((char *)set + ofs);
        on = (char *)chan->onNext != (char *)chan - 8;
        flags = on;
        if ((char *)chan->relNext != (char *)chan) {
            flags = on | 2;
        }

        if ((char *)chan->offNext != (char *)chan + 8) {
            flags |= 4;
        }
        out[i] = flags;
    }
    return 0;
}
/* ENDF modhsyn_e17 */
/* FUNC modhsyn_e18 */
int modhsyn_e18(int mode)
{
    switch (mode) {
    case 0:
        sceHsynEnabled = 1;
        break;
    case 1:
        sceHsynEnabled = 0;
        break;
    default:
        return -1;
    }
    return 0;
}
/* ENDF modhsyn_e18 */
/* FUNC modhsyn_e27 */
int modhsyn_e27(void)
{
    return (sceHsynEnabled ^ 1) != 0;
}
/* ENDF modhsyn_e27 */
/* FUNC sceHsynCmdReset */
int sceHsynCmdReset(u8 **pp)
{
    u32 acc;
    u8 b;

    acc = 0;
    do {
        b = *(*pp)++;
        acc = (acc << 7) + (b & 0x7F);
    } while (b & 0x80);
    return acc;
}
/* ENDF sceHsynCmdReset */
/* FUNC sceHsynCmdProgramChange */
void sceHsynCmdProgramChange(int base, int unused, int rate, int state, u8 note, u8 ch, u8 vol, u32 tag)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    if ((*(u8 *)(base + 2) & 1) != 0) {
        heads[0] = (sceHSynVoice_t *)(base + 16);
        heads[1] = (sceHSynVoice_t *)(base + 32);
        for (i = 0; i < 2; i++) {
            end = heads[i];
            for (v = end->chPrev; v != end; v = v->chPrev) {
                if (v->tag != tag) {
                    continue;
                }
                if (v->unk58 != state) {
                    continue;
                }
                if (v->note != note) {
                    continue;
                }
                if (v->ch != ch) {
                    continue;
                }
                sceHsynVoiceSetVolumeRamp(v, rate, vol);
            }
        }
    }
}
/* ENDF sceHsynCmdProgramChange */
/* FUNC sceHsynCmdSetParamRamp */
void sceHsynCmdSetParamRamp(int base, int unused, u32 a3, int a4, int state, u8 note, u8 ch, u8 a8, s8 a9, int a10, u32 tag)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    if ((*(u8 *)(base + 2) & 1) != 0) {
        heads[0] = (sceHSynVoice_t *)(base + 16);
        heads[1] = (sceHSynVoice_t *)(base + 32);
        for (i = 0; i < 2; i++) {
            end = heads[i];
            for (v = end->chPrev; v != end; v = v->chPrev) {
                if (v->tag != tag) {
                    continue;
                }
                if (v->unk58 != state) {
                    continue;
                }
                if (v->note != note) {
                    continue;
                }
                if (v->ch != ch) {
                    continue;
                }
                sceHsynVoiceSetParamRamp(v, a3, a4, a8, a9, a10);
            }
        }
    }
}
/* ENDF sceHsynCmdSetParamRamp */
/* FUNC sceHsynCmdChannelPressure */
void sceHsynCmdChannelPressure(int base, int unused, int dir, int rate, int state, u8 note, u8 ch, u16 depth, u32 tag)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    if ((*(u8 *)(base + 2) & 1) != 0) {
        heads[0] = (sceHSynVoice_t *)(base + 16);
        heads[1] = (sceHSynVoice_t *)(base + 32);
        for (i = 0; i < 2; i++) {
            end = heads[i];
            for (v = end->chPrev; v != end; v = v->chPrev) {
                if (v->tag != tag) {
                    continue;
                }
                if (v->unk58 != state) {
                    continue;
                }
                if (v->note != note) {
                    continue;
                }
                if (v->ch != ch) {
                    continue;
                }
                sceHsynPitchBendApply(v, rate, depth, dir);
            }
        }
    }
}
/* ENDF sceHsynCmdChannelPressure */
/* FUNC sceHsynCmdNoteOff */
void sceHsynCmdNoteOff(int sel, int base, int unused, int state, u8 note, u8 ch, s16 val, u32 tag)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    if ((*(u8 *)(base + 2) & 1) != 0) {
        heads[0] = (sceHSynVoice_t *)(base + 16);
        heads[1] = (sceHSynVoice_t *)(base + 32);
        for (i = 0; i < 2; i++) {
            end = heads[i];
            for (v = end->chPrev; v != end; v = v->chPrev) {
                if (v->tag != tag) {
                    continue;
                }
                if (v->unk58 != state) {
                    continue;
                }
                if (v->note != note) {
                    continue;
                }
                if (v->ch != ch) {
                    continue;
                }
                switch (sel) {
                case 0:
                    v->unk20 = val;
                    break;
                case 1:
                    v->unk4F = val;
                    break;
                }
            }
        }
    }
}
/* ENDF sceHsynCmdNoteOff */
/* FUNC sceHsynCmdNoteOn */
void sceHsynCmdNoteOn(int sel, int base, int unused, int state, u8 note, u8 ch, s16 val, u32 tag)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    if ((*(u8 *)(base + 2) & 1) != 0) {
        heads[0] = (sceHSynVoice_t *)(base + 16);
        heads[1] = (sceHSynVoice_t *)(base + 32);
        for (i = 0; i < 2; i++) {
            end = heads[i];
            for (v = end->chPrev; v != end; v = v->chPrev) {
                if (v->tag != tag) {
                    continue;
                }
                if (v->unk58 != state) {
                    continue;
                }
                if (v->note != note) {
                    continue;
                }
                if (v->ch != ch) {
                    continue;
                }
                switch (sel) {
                case 0:
                    v->unk22 = val;
                    break;
                case 1:
                    v->unk50 = val;
                    break;
                }
            }
        }
    }
}
/* ENDF sceHsynCmdNoteOn */
/* FUNC sceHsynCmdVoiceAlloc */
#ifdef NON_MATCHING
/* Eight bytes short: gcc hoists the 0x10000 dividend and the
 * (a7 << 8) + half sum out of the voice walk, where retail rebuilds both
 * inside it. */
void sceHsynCmdVoiceAlloc(int kind, sceHSynSlot_t *slot, int a3, int a4, u8 note,
                       u8 ch, u16 a7, u32 tag)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    u32 scale;
    u32 half;
    u32 t;
    int one;
    int i;

    if ((slot->flags & 1) == 0) {
        return;
    }
    heads[0] = (sceHSynVoice_t *)((char *)&slot->chan[0].onNext - 8);
    heads[1] = (sceHSynVoice_t *)((char *)&slot->chan[0].offNext - 8);
    one = 1;
    scale = sceHsynTickScale;
    half = scale >> one;
    for (i = 0; i < 2; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = v->chPrev) {
            if (v->tag != tag) {
                continue;
            }
            if (v->unk58 != a4) {
                continue;
            }
            if (v->note != note) {
                continue;
            }
            if (v->ch != ch) {
                continue;
            }
            t = a7 << 8;
            t += half;
            if (kind == 0) {
                v->envA.rate = 0x10000 / (t / scale);
            } else if (kind == one) {
                v->envB.rate = 0x10000 / (t / scale);
            }
        }
    }
}
#else
INCLUDE_ASM("MODHSYN", sceHsynCmdVoiceAlloc);
#endif
/* ENDF sceHsynCmdVoiceAlloc */
#ifdef NON_MATCHING
/* The switch on the system-exclusive command byte compiles to a jump table,
 * and the assembly .rodata already holds its case labels, so a C body here
 * will not link. */
/* FUNC sceHsynCmdDispatch */
void sceHsynCmdDispatch(sceHSynSlot_t *slot, int idx, u8 *p, u8 *end)
{
    u8 *cur;
    u8 *q;
    u32 tag;
    u32 w0;
    u32 w1;
    int val;
    int ch;
    int cmd;
    int sub;
    int b0;
    int b1;
    int b2;
    int b3;
    int n;
    int c;
    int lo;
    int hi;

    cur = p;
    do {
        c = *cur++;
        if (c != 0xFE) {
            do {
                if (cur >= end) {
                    return;
                }
                c = *cur++;
            } while (c != 0xFE);
        }
        q = cur;
        if (q >= end) {
            return;
        }
        if (q + 6 >= end) {
            return;
        }
        tag = q[1] | (q[2] << 8) | (q[3] << 16) | (q[4] << 24);
        sub = q[5];
        ch = sub & 0xF;
        cur = q + 6;
        switch (sub & 0xF0) {
        case 0x90:
        case 0xA0:
            n = (q[0] != 0) ? 6 : 5;
            if (cur + n > end) {
                break;
            }
            b0 = *cur++;
            b1 = *cur++;
            b2 = *cur++;
            b3 = -0x80;
            if (q[0] != 0) {
                b3 = *cur++;
            }
            lo = *cur++;
            hi = *cur++;
            val = lo | (hi << 7);
            if ((sub & 0xF0) == 0x90) {
                sceHsynVoiceStart(slot, idx, ch, b0, b1, b2, (s8)b3, val, tag);
            } else if (b2 == 0) {
                sceHsynChannelNoteOffMatch((int)slot, idx, ch, b0, b1, 0, tag);
            } else {
                sceHsynVoiceStart(slot, idx, ch, b0, b1, b2, (s8)b3, val, tag);
            }
            break;
        case 0xB0:
            if (q + 9 >= end) {
                break;
            }
            b0 = q[6];
            b1 = q[7];
            cmd = q[8];
            cur = q + 9;
            switch (cmd) {
            case 0:
            case 1:
            case 3:
            case 4:
                val = sceHsynCmdReset(&cur);
                lo = *cur++;
                hi = *cur++;
                sceHsynCmdSetParamRamp((int)slot, idx, cmd, val, ch, b0, b1, lo, hi, 0, tag);
                break;
            case 2:
                sceHsynCmdSetParamRamp((int)slot, idx, cmd, 0, ch, b0, b1, 0, 0, 0, tag);
                break;
            case 5:
                lo = cur[0];
                cur += 2;
                sceHsynCmdSetParamRamp((int)slot, idx, cmd, 0, ch, b0, b1, 0, lo, 0, tag);
                break;
            case 7:
                val = sceHsynCmdReset(&cur);
                lo = cur[0];
                cur += 2;
                sceHsynCmdProgramChange((int)slot, idx, val, ch, b0, b1, lo, tag);
                break;
            case 10:
                sceHsynCmdReset(&cur);
                cur += 2;
                break;
            case 12:
            case 13:
                val = sceHsynCmdReset(&cur);
                lo = cur[0];
                cur += 2;
                sceHsynCmdSetParamRamp((int)slot, idx, cmd, val, ch, b0, b1, 0, lo, 0, tag);
                break;
            case 14:
            case 15:
                val = sceHsynCmdReset(&cur);
                lo = *cur++;
                hi = *cur++;
                sceHsynCmdChannelPressure((int)slot, idx, cmd, val, ch, b0, b1,
                                       lo | (hi << 8), tag);
                break;
            case 16:
                lo = *cur++;
                hi = *cur++;
                sceHsynCmdNoteOff(0, (int)slot, idx, ch, b0, b1, lo | (hi << 8), tag);
                break;
            case 17:
                lo = *cur++;
                hi = *cur++;
                sceHsynCmdNoteOn(0, (int)slot, idx, ch, b0, b1, lo | (hi << 8), tag);
                break;
            case 18:
                val = sceHsynCmdReset(&cur);
                sceHsynCmdVoiceAlloc(0, slot, idx, ch, b0, b1, val, tag);
                break;
            case 32:
                lo = *cur++;
                hi = *cur++;
                sceHsynCmdNoteOff(1, (int)slot, idx, ch, b0, b1, lo | (hi << 8), tag);
                break;
            case 33:
                lo = *cur++;
                hi = *cur++;
                sceHsynCmdNoteOn(1, (int)slot, idx, ch, b0, b1, lo | (hi << 8), tag);
                break;
            case 34:
                val = sceHsynCmdReset(&cur);
                sceHsynCmdVoiceAlloc(1, slot, idx, ch, b0, b1, val, tag);
                break;
            default:
                stdio_printf(aSesqUnknownCom);
                break;
            }
            break;
        case 0xC0:
            q = cur;
            cmd = *cur++;
            switch (cmd) {
            case 0x1E:
                w0 = q[1] | (q[2] << 8) | (q[3] << 16) | (q[4] << 24);
                w1 = q[5] | (q[6] << 8) | (q[7] << 16) | (q[8] << 24);
                cur = q + 9;
                sceHsynChannelKeyOffByTagMaskEntry((int)slot, tag, w0, w1);
                break;
            case 0x1F:
                sceHsynChannelKeyOffByTagEntry((int)slot, tag);
                break;
            case 0x00:
            case 0x11:
            case 0x12:
                cur = q + 3;
                break;
            case 0x10:
                cur = q + 6;
                break;
            case 0x13:
                cur = q + 4;
                break;
            case 0x20:
            case 0x30:
                cur = q + 2;
                break;
            case 0x31:
                sceHsynCmdReset(&cur);
                cur += 2;
                break;
            }
            break;
        }
    } while (cur < end);
}
/* ENDF sceHsynCmdDispatch */
#else
INCLUDE_ASM("MODHSYN", sceHsynCmdDispatch);
#endif
/* FUNC modhsyn_e19 */
int modhsyn_e19(u8 val)
{
    if (val >= 0x31) {
        return -1;
    }
    sceHsynModDisable = val;
    return 0;
}
/* ENDF modhsyn_e19 */
/* FUNC modhsyn_e20 */
int modhsyn_e20(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    sceHsynChannelKeyOffAll(&set->chan[0]);
    return 0;
}
/* ENDF modhsyn_e20 */
/* FUNC modhsyn_e21 */
int modhsyn_e21(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    sceHsynChannelStopVoices(&set->chan[0]);
    return 0;
}
/* ENDF modhsyn_e21 */
/* FUNC modhsyn_e22 */
int modhsyn_e22(sceHSynWork_t *work, int slotNo, u32 tag, u8 *out, s8 max)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int n;
    int i;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    n = 0;
    chan = set->chan;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = v->chPrev) {
            if (v->tag == tag) {
                out[n] = v->index + v->core * 24;
                n++;
                if (n >= max) {
                    return n;
                }
            }
        }
    }
    return n;
}
/* ENDF modhsyn_e22 */
/* FUNC modhsyn_e25 */
int modhsyn_e25(sceHSynWork_t *work, int slotNo, u32 *out, int max)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int n;
    int i;
    int j;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    n = 0;
    chan = set->chan;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = v->chPrev) {
            for (j = 0; j < n; j++) {
                if (out[j] == v->tag) {
                    break;
                }
            }
            if (j < n) {
                continue;
            }
            out[n] = v->tag;
            n++;
            if (n >= max) {
                return n;
            }
        }
    }
    return n;
}
/* ENDF modhsyn_e25 */
/* FUNC modhsyn_e23 */
int modhsyn_e23(sceHSynWork_t *work, int slotNo, u8 tag, u8 *out, s8 max)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t **head;
    sceHSynVoice_t **base;
    int n;
    int ch;
    int i;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if (sceHsynStat == NULL) {
        return -1;
    }
    if ((slot->flags & 1) != 0) {
        return -1;
    }
    n = 0;
    ch = 0;
    base = heads;
    for (; ch < 16; ch++) {
        chan = &set->chan[ch];
        heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
        heads[1] = (sceHSynVoice_t *)chan;
        heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
        for (i = 0, head = base; i < 3; i++, head++) {
            end = *head;
            for (v = end->chPrev; v != end; v = v->chPrev) {
                if (v->tag == tag) {
                    out[n] = sceHsynStat->state[v->core][v->index];
                    n++;
                    if (n >= max) {
                        return n;
                    }
                }
            }
        }
    }
    return n;
}
/* ENDF modhsyn_e23 */
#ifdef NON_MATCHING
/* Same size: two independent preheader instructions, the cached stat
 * pointer and the hoisted tag mask, come out in the other order. */
/* FUNC modhsyn_e24 */
int modhsyn_e24(sceHSynWork_t *work, int slotNo, u8 tag, u16 *out, s8 max)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynChanSet_t *sp;
    sceHSynStat_t *stat;
    sceHSynChannel_t *chan;
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t **head;
    sceHSynVoice_t **base;
    int n;
    int ch;
    int i;

    if (sceHsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if (sceHsynStat == NULL) {
        return -1;
    }
    if ((slot->flags & 1) != 0) {
        return -1;
    }
    n = 0;
    ch = 0;
    sp = set;
    base = heads;
    stat = sceHsynStat;
    for (; ch < 16; ch++) {
        chan = &sp->chan[ch];
        heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
        heads[1] = (sceHSynVoice_t *)chan;
        heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
        for (i = 0, head = base; i < 3; i++, head++) {
            end = *head;
            for (v = end->chPrev; v != end; v = v->chPrev) {
                if (v->tag == tag) {
                    out[n] = stat->unk38[v->core][v->index];
                    n++;
                    if (n >= max) {
                        return n;
                    }
                }
            }
        }
    }
    return n;
}
/* ENDF modhsyn_e24 */
#else
INCLUDE_ASM("MODHSYN", modhsyn_e24);
#endif
#ifdef NON_MATCHING
/* Four instructions out of place: gcc schedules the outer loop's preheader
 * (the hoisted 0xFF and the induction-variable init) after the list-head
 * stores, where retail interleaves it with the register saves. */
/* FUNC sceHsynInitCores */
void sceHsynInitCores(void)
{
    sceHSynVoice_t *a;
    sceHSynVoice_t *b;
    sceHSynVoice_t *v;
    u32 *p0;
    u32 *p1;
    u32 *p2;
    u32 *p3;
    u32 *p4;
    u8 *cnt;
    int core;
    int i;

    a = (sceHSynVoice_t *)sceHsynRelList[0];
    b = (sceHSynVoice_t *)sceHsynRelList[1];
    sceHsynRelList[0][1] = a;
    sceHsynRelList[0][0] = a;
    sceHsynRelList[1][1] = b;
    sceHsynRelList[1][0] = b;
    a = (sceHSynVoice_t *)sceHsynCoreList[0];
    b = (sceHSynVoice_t *)sceHsynCoreList[1];
    sceHsynCoreList[0][1] = a;
    sceHsynCoreList[0][0] = a;
    sceHsynCoreList[1][1] = b;
    sceHsynCoreList[1][0] = b;
    a = (sceHSynVoice_t *)sceHsynActiveList[0];
    b = (sceHSynVoice_t *)sceHsynActiveList[1];
    sceHsynActiveList[0][1] = a;
    sceHsynActiveList[0][0] = a;
    sceHsynActiveList[1][1] = b;
    sceHsynActiveList[1][0] = b;
    a = (sceHSynVoice_t *)sceHsynFreeList[0];
    b = (sceHSynVoice_t *)sceHsynFreeList[1];
    sceHsynFreeList[0][1] = a;
    sceHsynFreeList[0][0] = a;
    sceHsynFreeList[1][1] = b;
    sceHsynFreeList[1][0] = b;
    for (core = 0; core < 2; core++) {
        for (i = 0; i < 24; i++) {
            v = &sceHsynVoices[core][i];
            v->unk28 = -1;
            v->flags = 0;
            v->slotp = NULL;
            v->chan = NULL;
            v->prog = NULL;
            v->key = NULL;
            v->tone = NULL;
            v->vag = NULL;
            v->spuAddr = 0;
            v->unk12 = 0;
            v->unk38 = 0;
            v->vnum = 0;
            v->core = core;
            v->index = i;
            v->unk55 = 0xFF;
            v->vol = -0x80;
            v->tag = 0xFF;
            v->unk57 = 0xFF;
            v->unk58 = 0xFF;
            v->unk2C = 0;
            sceHsynCoreStateGet(v);
            sceHsynCoreStateSet(v);
            sceHsynChannelStateGet(v);
            sceHsynVoiceInsertBefore(v, (sceHSynVoice_t *)sceHsynFreeList[core]);
        }
    }
    cnt = sceHsynFreeCount;
    cnt[1] = 24;
    cnt[0] = 24;
    p0 = sceHsynKeyOffPending;
    p0[1] = 0;
    p0[0] = 0;
    p1 = sceHsynKeyOnPending;
    p1[1] = 0;
    p1[0] = 0;
    p2 = sceHsynNoiseMask;
    p2[1] = 0;
    p2[0] = 0;
    p3 = (u32 *)sceHsynMixMaskL;
    p3[3] = 0;
    p3[2] = 0;
    p3[1] = 0;
    p3[0] = 0;
    p4 = (u32 *)sceHsynMixMaskR;
    p4[3] = 0;
    p4[2] = 0;
    p4[1] = 0;
    p4[0] = 0;
}
#else
INCLUDE_ASM("MODHSYN", sceHsynInitCores);
#endif
/* ENDF sceHsynInitCores */
/* FUNC sceHsynVoiceInsertAfter */
void sceHsynVoiceInsertAfter(sceHSynVoice_t *v, sceHSynVoice_t *at)
{
    sceHSynVoice_t *p;

    v->prev = at;
    p = at->next;
    v->next = p;
    at->next = v;
    p->prev = v;
}
/* ENDF sceHsynVoiceInsertAfter */
/* FUNC sceHsynVoiceInsertBefore */
void sceHsynVoiceInsertBefore(sceHSynVoice_t *v, sceHSynVoice_t *at)
{
    sceHSynVoice_t *p;

    v->next = at;
    p = at->prev;
    v->prev = p;
    at->prev = v;
    p->next = v;
}
/* ENDF sceHsynVoiceInsertBefore */
/* FUNC sceHsynVoiceUnlink */
void sceHsynVoiceUnlink(sceHSynVoice_t *v)
{
    v->next->prev = v->prev;
    v->prev->next = v->next;
}
/* ENDF sceHsynVoiceUnlink */
/* FUNC sceHsynVoiceChLink */
void sceHsynVoiceChLink(sceHSynVoice_t *v, sceHSynVoice_t **head)
{
    sceHSynVoice_t *n;

    /* head points at the chNext/chPrev pair of the list's sentinel. */
    v->chPrev = (sceHSynVoice_t *)((u8 *)head - 8);
    n = head[0];
    v->chNext = n;
    head[0] = v;
    n->chPrev = v;
}
/* ENDF sceHsynVoiceChLink */
INCLUDE_ASM("MODHSYN", D_00003734);
/* FUNC sceHsynVoiceChUnlink */
void sceHsynVoiceChUnlink(sceHSynVoice_t *v)
{
    v->chNext->chPrev = v->chPrev;
    v->chPrev->chNext = v->chNext;
}
/* ENDF sceHsynVoiceChUnlink */
/* FUNC sceHsynServiceVoices */
int sceHsynServiceVoices(void)
{
    int n;
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    u32 m;
    int core;

    n = 0;
    for (core = 0; core < 2; core++) {
        m = sceHsynKeyOffPending[core];
        if (m != 0) {
            sceHsynSdSetSwitch(core | 0x1600, m);
            sceHsynKeyOffPending[core] = 0;
        }
    }
    for (core = 0; core < 2; core++) {
        end = (sceHSynVoice_t *)sceHsynActiveList[core];
        for (v = end->prev; v != end; v = v->prev) {
            if (v->unk57 == 1 && v->ch == 0xFF) {
                goto skip;
            }
            if ((u16)sceHsynSdGetParam(core | (v->index << 1) | 0x500) >= 0x3E8) {
                goto skip2;
            }
            next = v->next;
            switch (v->path) {
            case 0:
                sceHsynVoiceKeyOnPathA(v);
                break;
            case 1:
                sceHsynVoiceKeyOnPathB(v);
                break;
            }
            sceHsynVoiceUnlink(v);
            sceHsynVoiceInsertAfter(v, (sceHSynVoice_t *)sceHsynCoreList[core]);
            v = next;
            continue;
        skip2:
            n++;
            continue;
        skip:
            n++;
        }
        m = sceHsynKeyOnPending[core];
        if (m != 0) {
            sceHsynSdSetSwitch(core | 0x1500, m);
            sceHsynKeyOnPending[core] = 0;
        }
    }
    return n;
}
/* ENDF sceHsynServiceVoices */
/* FUNC sceHsynEnvRateLookup */
s16 sceHsynEnvRateLookup(s16 delta, s32 base, s32 bias)
{
    s32 v;

    v = base + bias + delta;
    if (v > 0x7FFF) {
        v = 0x7FFF;
    } else if (v < -0x8000) {
        v = -0x8000;
    }
    return v;
}
/* ENDF sceHsynEnvRateLookup */
/* FUNC sceHsynEnvLevelLookup */
s8 sceHsynEnvLevelLookup(s8 delta, s32 base, s32 bias)
{
    s32 v;

    v = base + bias + delta;
    if (v >= 0x80) {
        v = 0x7F;
    } else if (v < -0x80) {
        v = -0x80;
    }
    return v;
}
/* ENDF sceHsynEnvLevelLookup */
#ifdef NON_MATCHING
/* Seven registers apart: the key delta and the mask land in the other
 * order. */
/* FUNC sceHsynAdjustFieldByKey */
u16 sceHsynAdjustFieldByKey(u16 field, s8 scale, int keyDelta, u16 mask, int shift)
{
    int delta;
    int v;

    delta = keyDelta * scale / 12;
    if (delta != 0) {
        v = ((field >> shift) & mask) + delta;
        if (v < 0) {
            v = 0;
        } else if (v > mask) {
            v = mask;
        }
        field = (field & ~(mask << shift)) | (v << shift);
    }
    return field;
}
/* ENDF sceHsynAdjustFieldByKey */
#else
INCLUDE_ASM("MODHSYN", sceHsynAdjustFieldByKey);
#endif
/* FUNC sceHsynBuildAdsr1 */
u16 sceHsynBuildAdsr1(int note, sceSmgSmplEnt_t *tone)
{
    u16 v;

    v = tone->adsr1;
    if (tone->arScale != 0) {
        v = sceHsynAdjustFieldByKey(v, tone->arScale, note - tone->arKey, 0x7F, 8);
    }
    if (tone->drScale != 0) {
        v = sceHsynAdjustFieldByKey(v, tone->drScale, note - tone->drKey, 0xF, 4);
    }
    if (tone->slScale != 0) {
        v = sceHsynAdjustFieldByKey(v, tone->slScale, note - tone->slKey, 0xF, 0);
    }
    return v;
}
/* ENDF sceHsynBuildAdsr1 */
/* FUNC sceHsynBuildAdsr2 */
u16 sceHsynBuildAdsr2(int note, sceSmgSmplEnt_t *tone)
{
    u16 v;

    v = tone->adsr2;
    if (tone->srScale != 0) {
        v = sceHsynAdjustFieldByKey(v, tone->srScale, note - tone->srKey, 0x7F, 6);
    }
    if (tone->rrScale != 0) {
        v = sceHsynAdjustFieldByKey(v, tone->rrScale, note - tone->rrKey, 0x1F, 0);
    }
    return v;
}
/* ENDF sceHsynBuildAdsr2 */
/* FUNC sceHsynVoiceUpdatePathA */
void sceHsynVoiceUpdatePathA(sceHSynVoice_t *v)
{
    sceSmgProgEnt_t *prog;
    sceSmgProgKey_t *key;
    sceSmgSmplEnt_t *tone;
    sceHSynChannel_t *chan;
    sceHSynSlot_t *slot;
    int base;
    int bias;
    int t;
    int t2;
    int bend;
    s8 pan;
    u8 e;
    s16 pitch;

    chan = v->chan;
    prog = v->prog;
    tone = v->tone;
    key = v->key;
    slot = chan->slot;
    sceHsynLfoSetupA(&v->envA, (int)prog, (u8 *)slot->unk08, slot->unk04,
                      tone->unk28 & 7, tone->unk20, tone->unk22,
                      sceHsynTickScale, 0);
    bias = ((v->ch - key->unk0B) * key->unk0A) / 12;
    base = sceHsynSdSetParamVolume(v, (s8)tone->unk05, tone->flags, tone->unk07);
    v->unk24 = sceHsynEnvRateLookup(prog->unk18, bias, base);
    v->unk26 = sceHsynEnvRateLookup(prog->unk1A, bias, base);
    sceHsynLfoSetupA(&v->envB, (int)prog, (u8 *)slot->unk08, slot->unk04,
                      tone->unk28 & 0x70, tone->unk24, tone->unk26,
                      sceHsynTickScale, 1);
    bias = ((v->ch - key->unk0D) * key->unk0C) / 12;
    base = sceHsynSdSetParamVolume(v, (s8)tone->unk08, tone->unk09, tone->unk0A);
    v->unk51 = sceHsynEnvLevelLookup(prog->unk20, bias, base);
    v->unk52 = sceHsynEnvLevelLookup(prog->unk21, bias, base);
    sceHsynSdSetParamVolumeLeft(v, chan->unk2D);
    sceHsynSdSetParamVolumeRight(v, chan->unk2E);
    base = sceHsynVolumeScale(v->vel, v->unk4A, slot->unk10, slot->unk0C);
    t = (u8)prog->unk06 * key->unk10;
    t2 = t * tone->unk10;
    bend = (t2 * base) / 16384;
    v->unk16 = (u32)(bend * sceHsynSdSetParamBend(v)) >> 16;
    bias = (v->ch - key->unk0F) * key->unk0E;
    bias = bias + (v->ch - prog->unk0B) * prog->unk0A;
    v->unk4E = prog->unk0C & 1;
    sceHsynSdSetParamAdsr(v, bias / 12);
    if ((v->vol & 0x80) != 0) {
        pan = (u8)chan->pan;
    } else {
        pan = (u8)v->vol - 0x40;
        if (pan < -0x3F) {
            pan = -0x3F;
        }
    }
    sceHsynVoiceSetPan(v, pan);
    if (v->unk55 != 0xFF) {
        e = v->unk55;
    } else {
        e = chan->expression;
    }
    v->unk54 = e;
    sceHsynSdSetParamEnvelope(v);
    v->unk1C = 0xFFFF;
    v->unk1A = 0xFFFF;
    if (v->vag != NULL) {
        v->unk30 = 0x7FFFFFFF;
        v->unk3C = ((u32)*(u16 *)&v->vag->unk04 << 16) / 48000;
        if ((v->unk28 & 0x8000) != 0) {
            pitch = chan->unk26;
        } else {
            pitch = (u16)v->unk28 - 0x2000;
        }
        sceHsynSdSetParamPitch(v, pitch);
    }
}
/* ENDF sceHsynVoiceUpdatePathA */
/* FUNC sceHsynVoiceKeyOnPathA */
void sceHsynVoiceKeyOnPathA(sceHSynVoice_t *v)
{
    sceSmgSmplEnt_t *tone;
    u32 clk[2];
    u32 bit;
    u16 ent;
    int core;
    int idx;
    u32 note;
    u8 *st;

    idx = v->index;
    core = v->core;
    tone = v->tone;
    ent = core | (idx << 1);
    bit = 1 << idx;
    sceHsynVoiceVolumeCommit(v);
    if (v->vag != NULL) {
        sceHsynNoiseMask[v->core] &= ~bit;
        sceHsynVoicePitchCommit(v);
        sceHsynSdSetAddr(ent | 0x2040, v->spuAddr + v->vag->spuOfs);
        sceHsynSdSetParam(ent | 0x300, sceHsynBuildAdsr1(v->ch, tone));
        sceHsynSdSetParam(ent | 0x400, sceHsynBuildAdsr2(v->ch, tone));
        if ((*((u8 *)v->vag + 6) & 1) == 0) {
            v->flags |= 1;
        }
    } else {
        note = v->ch;
        if (note >= 0x40) {
            note = 0x3F;
        }
        sceHsynSdSetCoreAttr(v->core | 8, note);
        sceHsynNoiseMask[v->core] |= bit;
    }
    sceHsynSdSetSwitch(v->core | 0x1400, sceHsynNoiseMask[v->core]);
    if ((tone->allocHint & 1) != 0) {
        sceHsynMixMaskL[v->core][0] |= bit;
    } else {
        sceHsynMixMaskL[v->core][0] &= ~bit;
    }
    if ((tone->allocHint & 2) != 0) {
        sceHsynMixMaskL[v->core][1] |= bit;
    } else {
        sceHsynMixMaskL[v->core][1] &= ~bit;
    }
    if ((tone->allocHint & 4) != 0) {
        sceHsynMixMaskR[v->core][0] |= bit;
    } else {
        sceHsynMixMaskR[v->core][0] &= ~bit;
    }
    if ((tone->allocHint & 8) != 0) {
        sceHsynMixMaskR[v->core][1] |= bit;
    } else {
        sceHsynMixMaskR[v->core][1] &= ~bit;
    }
    sceHsynSdSetSwitch(v->core | 0x1800, sceHsynMixMaskL[v->core][0]);
    sceHsynSdSetSwitch(v->core | 0x1A00, sceHsynMixMaskL[v->core][1]);
    sceHsynSdSetSwitch(v->core | 0x1900, sceHsynMixMaskR[v->core][0]);
    sceHsynSdSetSwitch(v->core | 0x1B00, sceHsynMixMaskR[v->core][1]);
    sceHsynKeyOnPending[v->core] |= bit;
    sceHsynLastKeyOnCore = v->core;
    sceHsynLastKeyOnVoice = v->index;
    thbase_GetSystemTime(clk);
    thbase_SysClock2USec(clk, &sceHsynSettleTimeHi, &sceHsynSettleTimeLo);
    v->state = 3;
    v->flags = v->flags & 0xFFF7;
    if (sceHsynStat != NULL) {
        st = sceHsynStat->state[v->core];
        st[v->index] = (st[v->index] & 0x8F) | 0x20;
    }
}
/* ENDF sceHsynVoiceKeyOnPathA */
/* FUNC sceHsynVoicePitchCommit */
void sceHsynVoicePitchCommit(sceHSynVoice_t *v)
{
    u32 prod;
    u32 r;
    int lvl;
    int pitch;
    u32 note;
    int fine;
    int center;
    int tmp;
    int d;

    lvl = v->envA.level;
    if (lvl >= 0) {
        pitch = v->unk20;
    } else {
        pitch = v->unk22;
    }
    pitch = (pitch * lvl) / 16384;
    pitch = pitch + (v->unk34 + v->unk38);
    if (v->unk57 == 1) {
        if (v->lfoA.unk00 != 0) {
            pitch += (s16)v->lfoA.unk02;
        }
    }
    if (pitch == v->unk30) {
        return;
    }
    if (v->unk2C != 0 && (v->flags & 0x400) == 0) {
        pitch = v->unk2C;
        v->unk30 = pitch;
    } else {
        v->unk30 = pitch;
        switch (v->path) {
        case 1:
            center = v->ch;
            note = center + v->prog->unk05;
            fine = v->prog->unk06;
            tmp = fine;
            break;
        case 0:
        default:
            center = v->tone->unk0B;
            tmp = v->tone->unk0C;
            note = (v->ch + v->prog->unk08) + v->key->unk12;
            fine = (v->prog->unk09 + tmp) + v->key->unk13;
            break;
        }
        if (v->unk2C != 0) {
            r = sceHsynSdPitch2Note(center, tmp, v->unk2C);
            r &= 0xFFFF;
            note = r >> 8;
            fine = r & 0xFF;
        }
        pitch += fine;
        if (pitch < 0) {
            d = pitch / 128;
            note += d;
            pitch -= d * 128;
            if (pitch != 0) {
                pitch += 128;
                note--;
            }
        }
        pitch = (u16)sceHsynSdNote2Pitch(center, 0, note, pitch);
        prod = pitch * v->unk3C;
        pitch = prod >> 16;
    }
    if (pitch >= 0x4000) {
        pitch = 0x3FFF;
    }
    sceHsynSdSetParam(v->core | (v->index << 1) | 0x200, pitch);
}
/* ENDF sceHsynVoicePitchCommit */
/* FUNC sceHsynSdSetParamPitch */
void sceHsynSdSetParamPitch(sceHSynVoice_t *v, s16 bend)
{
    sceSmgProgKey_t *key;
    u16 depth;

    key = v->key;
    if (key == NULL) {
        return;
    }
    if (bend >= 0) {
        depth = key->unk08;
    } else {
        depth = key->unk06;
    }
    v->unk34 = depth * bend / 8192;
}
/* ENDF sceHsynSdSetParamPitch */
#ifdef NON_MATCHING
/* One instruction short: gcc keeps envB.level live into the second arm where
 * retail loads it again. */
/* FUNC sceHsynVoiceVolumeCommit */
void sceHsynVoiceVolumeCommit(sceHSynVoice_t *v)
{
    u32 e;
    u32 l;
    u32 r;
    u32 m;
    int vol;
    int g;
    int pol;

    vol = v->unk18;
    if (v->unk57 == 1) {
        if (v->lfoB.unk00 != 0) {
            vol += (s16)v->lfoB.unk02;
        }
    }
    vol = (vol * v->slotp->unk00) / 256;
    if (v->envB.level >= 0) {
        g = (vol * ((v->envB.level * v->unk4F) / 128 + 0x4000)) / 16384;
    } else {
        g = (vol * ((v->envB.level * v->unk50) / 128 + 0x4000)) / 16384;
    }
    if (g < 0) {
        g = 0;
    }
    m = v->unk4C;
    if (v->unk57 == 1 && *(u32 *)&v->param[0] != 0) {
        m = *(u32 *)&v->param[44];
    }
    l = (m * g) >> 7;
    if (l >= 0x4000) {
        l = 0x3FFF;
    }
    m = v->unk4D;
    if (v->unk57 == 1 && *(u32 *)&v->param[0] != 0) {
        m = *(u32 *)&v->param[48];
    }
    r = (m * g) >> 7;
    if (r >= 0x4000) {
        r = 0x3FFF;
    }
    if (l != v->unk1A || r != v->unk1C) {
        pol = v->unk53;
        e = v->core | (v->index << 1);
        v->unk1A = l;
        v->unk1C = r;
        if (v->unk57 == 1 && *(u32 *)&v->param[0] != 0) {
            pol = v->param[52];
        }
        if (pol != 0) {
            sceHsynSdSetParam(e, l);
            sceHsynSdSetParam(e | 0x100, r);
        } else {
            sceHsynSdSetParam(e, -l & 0x7FFF);
            sceHsynSdSetParam(e | 0x100, -r & 0x7FFF);
        }
    }
}
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceVolumeCommit);
#endif
/* ENDF sceHsynVoiceVolumeCommit */
/* ENDF sceHsynVoiceVolumeCommit */
/* FUNC sceHsynSdSetParamEnvelope */
void sceHsynSdSetParamEnvelope(sceHSynVoice_t *voice)
{
    u32 v;

    v = voice->chan->vol * voice->unk54;
    v = v * voice->unk16;
    voice->unk18 = v >> 14;
}
/* ENDF sceHsynSdSetParamEnvelope */
/* FUNC sceHsynSdSetParamVolume */
s32 sceHsynSdSetParamVolume(sceHSynVoice_t *v, s8 scale, u8 a, u8 b)
{
    sceHSynSlot_t *slot;

    slot = v->chan->slot;
    return sceHsynVolumeCurveLookup(v->vel, a, b, slot->unk10, slot->unk0C) * scale
        / 65536;
}
/* ENDF sceHsynSdSetParamVolume */
/* FUNC sceHsynSdSetParamVolumeLeft */
void sceHsynSdSetParamVolumeLeft(sceHSynVoice_t *v, u8 scale)
{
    int t;

    t = (scale * v->prog->unk22) / 128;
    t += v->unk51;
    if (t >= 0x80) {
        t = 0x7F;
    } else if (t < -0x80) {
        t = -0x80;
    }
    v->unk4F = t;
    t = (scale * v->prog->unk23) / 128;
    t += v->unk52;
    if (t >= 0x80) {
        t = 0x7F;
    } else if (t < -0x80) {
        t = -0x80;
    }
    v->unk50 = t;
}
/* ENDF sceHsynSdSetParamVolumeLeft */
/* FUNC sceHsynSdSetParamVolumeRight */
void sceHsynSdSetParamVolumeRight(sceHSynVoice_t *v, u8 scale)
{
    int t;

    t = (scale * v->prog->unk1C) / 128;
    t += v->unk24;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk20 = t;
    t = (scale * v->prog->unk1E) / 128;
    t += v->unk26;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk22 = t;
}
/* ENDF sceHsynSdSetParamVolumeRight */
#ifdef NON_MATCHING
/* Two instructions short: gcc enters the second wrap loop at its
 * bottom test where retail emits a guard with an unfilled delay slot. */
/* FUNC sceHsynSdSetParamAdsr */
void sceHsynSdSetParamAdsr(sceHSynVoice_t *v, s32 bias)
{
    int p;
    int k;
    int t;
    int lo;
    int hi;
    int x;

    p = v->prog->unk07;
    k = v->key->unk11;
    t = v->tone->unk0D;
    if (v->unk4E != 0) {
        lo = p - 0x80;
        hi = k + t;
        hi += bias;
        x = lo + hi;
        while (x >= 0x80) {
            x -= 0xFF;
        }
        while (x < -0x80) {
            x += 0xFF;
        }
        v->unk1E = x;
        return;
    }
    if (p < 0) {
        p = -p;
    }
    if (k < 0) {
        k = -k;
    }
    if (t < 0) {
        t = -t;
    }
    lo = p - 0x80;
    hi = k + t;
    hi += bias;
    x = lo + hi;
    if (x < 0) {
        x = 0;
    } else if (x >= 0x80) {
        x = 0x7F;
    }
    v->unk53 = 1;
    v->unk1E = x;
}
/* ENDF sceHsynSdSetParamAdsr */
#else
INCLUDE_ASM("MODHSYN", sceHsynSdSetParamAdsr);
#endif
/* FUNC sceHsynVoiceSetPan */
#ifdef NON_MATCHING
/* Same size and the same instructions: gcc fills the sceHsynEnabled branch's
 * delay slot with the sign extension and retail with the `p = pan` copy, so
 * the four instructions of the clamp come out in a different order. */
void sceHsynVoiceSetPan(sceHSynVoice_t *v, s8 pan)
{
    s8 p;
    int n;
    int sign;

    v->unk4D = 0x80;
    v->unk4C = 0x80;
    v->unk5A = 0;
    if (sceHsynEnabled != 0) {
        return;
    }
    p = pan;
    if (pan >= 0x40) {
        p = 0x3F;
    } else {
        if (pan < -0x3F) {
            p = -0x3F;
        }
    }
    if (v->unk4E != 0) {
        n = v->unk1E;
        sign = 1;
        if (n < 0) {
            n = -n;
            sign = 0;
        }
        n += p;
        if (n < 0) {
            sign ^= 1;
            n = -n;
        } else if (n >= 0x80) {
            sign ^= 1;
            n = 0x100 - n;
        }
        v->unk53 = sign;
        n -= 0x40;
    } else {
        n = p + v->unk1E - 0x40;
    }
    if (n > 0) {
        if (n >= 0x3F) {
            v->unk4C = 0;
        } else {
            v->unk4C = ((0x3F - n) << 7) / 63;
        }
    } else if (n < 0) {
        if (n < -0x3E) {
            v->unk4D = 0;
        } else {
            v->unk4D = ((n + 0x3F) << 7) / 63;
        }
    }
    v->unk5A = n;
}
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceSetPan);
#endif
/* ENDF sceHsynVoiceSetPan */
/* FUNC sceHsynSdSetParamBend */
u32 sceHsynSdSetParamBend(sceHSynVoice_t *v)
{
    sceSmgSmplEnt_t *tone;
    sceSmgProgKey_t *key;
    u32 r;
    u32 prod;
    int t;
    int m;
    int num;

    tone = v->tone;
    key = v->key;
    r = 0x10000;
    if (v->vel >= tone->unk03) {
        m = tone->velHigh;
        m &= 0x80;
        if (m != 0) {
            t = tone->velHigh & 0x7F;
            if (t != tone->unk03) {
                t++;
                num = t - v->vel;
                t = t - tone->unk03;
                r = (u32)(num << 16) / (u32)t;
            }
        }
    } else {
        m = tone->velLow;
        m &= 0x80;
        if (m != 0) {
            t = tone->velLow & 0x7F;
            if (t != tone->unk03) {
                t--;
                num = v->vel - t;
                t = tone->unk03 - t;
                r = (u32)(num << 16) / (u32)t;
            }
        }
    }
    if (v->ch >= key->unk03) {
        m = key->keyHigh;
        m &= 0x80;
        if (m != 0) {
            t = key->keyHigh & 0x7F;
            if (t != key->unk03) {
                t++;
                num = t - v->ch;
                prod = r * num;
                t = t - key->unk03;
                r = prod / (u32)t;
            }
        }
    } else {
        m = key->keyLow;
        m &= 0x80;
        if (m != 0) {
            t = key->keyLow & 0x7F;
            if (t != key->unk03) {
                t--;
                num = v->ch - t;
                prod = r * num;
                t = key->unk03 - t;
                r = prod / (u32)t;
            }
        }
    }
    return r;
}
/* ENDF sceHsynSdSetParamBend */
/* FUNC sceHsynUpdateVoiceState */
int sceHsynUpdateVoiceState(void)
{
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    u8 *st;
    u32 sw;
    int envx;
    int core;
    int n;
    int t;
    u16 f;
    u8 state;

    n = 0;
    for (core = 0; core < 2; core++) {
        end = (sceHSynVoice_t *)sceHsynCoreList[core];
        sw = sceHsynSdGetSwitch(core | 0x1700);
        for (v = end->prev; v != end; v = v->prev) {
            f = v->flags;
            if (sceHsynSdSyncFlag != 0) {
                envx = sceHsynSdGetParam(core | (v->index << 1) | 0x500);
                if ((f & 0x300) != 0) {
                    state = v->state;
                    if (state != 0) {
                        if ((sw & (1 << v->index)) == 0) {
                            v->state = 0;
                        } else {
                            v->state = state - 1;
                        }
                        envx = 0x7FFF;
                        goto post;
                    }
                    if ((sw & (1 << v->index)) == 0) {
                        goto post;
                    }
                    if ((u16)envx == 0) {
                        goto release;
                    }
                    goto post;
                }
                if ((u16)envx != 0) {
                    goto post;
                }
            release:
                next = v->next;
                sceHsynVoiceChFree(v);
                if (sceHsynStat != NULL) {
                    st = sceHsynStat->state[v->core];
                    st[v->index] &= 0x8F;
                }
                v = next;
                continue;
            post:
                if (sceHsynStat != NULL) {
                    sceHsynStat->unk38[v->core][v->index] = envx;
                }
            }
            if (v->vag != NULL) {
                if ((f & 6) != 0) {
                    if ((f & 2) != 0) {
                        t = v->unk38 + v->unk2A;
                        v->unk38 = t;
                        if (t >= 0) {
                            v->unk38 = 0;
                            v->flags = v->flags & 0xFFFD;
                        }
                    } else {
                        t = v->unk38 - v->unk2A;
                        v->unk38 = t;
                        if (t <= 0) {
                            v->unk38 = 0;
                            v->flags = v->flags & 0xFFFB;
                        }
                    }
                }
                sceHsynLfoStep(&v->envA);
                if (v->unk57 == 1) {
                    sceHsynEffectDepthSet(&v->lfoA);
                }
                sceHsynVoicePitchCommit(v);
            }
            sceHsynLfoStep(&v->envB);
            if (v->unk57 == 1) {
                sceHsynVoiceParamRampStep(v);
                sceHsynEffectFeedbackSet(v);
            }
            sceHsynVoiceVolumeCommit(v);
            n++;
        }
    }
    sceHsynSdSyncFlag = 1;
    return n;
}
/* ENDF sceHsynUpdateVoiceState */
/* FUNC sceHsynVoiceKeyOff */
int sceHsynVoiceKeyOff(void)
{
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    u8 *st;
    u32 sw;
    int envx;
    int core;
    int ret;
    u16 f;
    u8 state;

    ret = 0;
    for (core = 0; core < 2; core++) {
        end = (sceHSynVoice_t *)sceHsynCoreList[core];
        sw = sceHsynSdGetSwitch(core | 0x1700);
        for (v = end->prev; v != end; v = v->prev) {
            f = v->flags;
            envx = sceHsynSdGetParam(core | (v->index << 1) | 0x500);
            v->unk12 = envx;
            if ((f & 0x300) != 0) {
                state = v->state;
                if (state != 0) {
                    if ((sw & (1 << v->index)) == 0) {
                        v->state = 0;
                    } else {
                        v->state = state - 1;
                    }
                    v->unk12 = 0x7FFF;
                    continue;
                }
                if ((sw & (1 << v->index)) == 0) {
                    continue;
                }
                if ((u16)envx == 0) {
                    goto release;
                }
                continue;
            }
            if ((u16)envx != 0) {
                continue;
            }
        release:
            next = v->next;
            sceHsynVoiceChFree(v);
            if (sceHsynStat != NULL) {
                st = sceHsynStat->state[v->core];
                st[v->index] &= 0x8F;
            }
            v = next;
            ret = 1;
        }
    }
    return ret;
}
/* ENDF sceHsynVoiceKeyOff */
/* FUNC sceHsynVoiceStateClear */
int sceHsynVoiceStateClear(sceHSynVoice_t *v)
{
    int x;
    u32 q;
    int t;

    x = (v->chan->portaNote - v->ch) << 7;
    v->unk38 = x;
    if (x < 0) {
        x = -x;
        v->flags |= 2;
    } else {
        v->flags |= 4;
    }
    q = (x * sceHsynTickScale) / v->chan->bendRange;
    t = (int)q / 256;
    v->unk2A = t;
    return t;
}
/* ENDF sceHsynVoiceStateClear */
#ifdef NON_MATCHING
/* One instruction short: gcc keeps v->vag live across the two halfword stores
 * where retail loads it again. */
/* FUNC sceHsynVoiceUpdatePathB */
void sceHsynVoiceUpdatePathB(sceHSynVoice_t *v)
{
    sceSmgProgEnt_t *prog;
    sceHSynChannel_t *chan;
    sceHSynSlot_t *slot;
    int base;
    int t;
    s8 pan;
    u8 sc;

    prog = v->prog;
    chan = v->chan;
    slot = chan->slot;
    sceHsynLfoSetupB(&v->envA, (int)prog, (u8 *)slot->unk08, slot->unk04,
                      prog->unk31 & 7, *(u16 *)&prog->unk22, prog->unk26,
                      sceHsynTickScale, 0);
    base = sceHsynSdSetParamVolume(v, prog->unk2A, prog->unk2C, prog->unk2E);
    v->unk24 = sceHsynEnvRateLookup(prog->unk1C, 0, base);
    v->unk26 = sceHsynEnvRateLookup(prog->unk1E, 0, base);
    sceHsynLfoSetupB(&v->envB, (int)prog, (u8 *)slot->unk08, slot->unk04,
                      prog->unk31 & 0x70, prog->unk24, prog->unk28,
                      sceHsynTickScale, 1);
    base = sceHsynSdSetParamVolume(v, prog->unk2B, prog->unk2D, prog->unk2F);
    v->unk51 = sceHsynEnvLevelLookup(prog->unk20, 0, base);
    v->unk52 = sceHsynEnvLevelLookup(prog->unk21, 0, base);
    sceHsynEnvAttackStep(v, chan->unk2D);
    sceHsynEnvDecayStep(v, chan->unk2E);
    sc = sceHsynVolumeScale(v->vel, v->unk4A, slot->unk10, slot->unk0C);
    v->unk16 = *((u8 *)prog + 3) * sc;
    v->unk4E = prog->unk0C & 1;
    sceHsynVoiceSetPanBase(v);
    t = (u8)v->vol;
    if (t == 0x80) {
        pan = 0;
    } else {
        pan = t - 0x40;
        if (pan < -0x3F) {
            pan = -0x3F;
        }
    }
    sceHsynVoiceSetPan(v, pan);
    v->unk54 = chan->expression;
    sceHsynSdSetParamEnvelope(v);
    v->unk1C = 0xFFFF;
    v->unk1A = 0xFFFF;
    if (v->vag != NULL) {
        v->unk30 = 0x7FFFFFFF;
        v->unk34 = 0;
        v->unk3C = ((u32)*(u16 *)&v->vag->unk04 << 16) / 48000;
    }
}
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceUpdatePathB);
#endif
/* ENDF sceHsynVoiceUpdatePathB */
/* FUNC sceHsynVoiceKeyOnPathB */
void sceHsynVoiceKeyOnPathB(sceHSynVoice_t *v)
{
    sceSmgProgEnt_t *prog;
    u32 clk[2];
    u32 bit;
    u16 ent;
    int core;
    int idx;
    u32 note;
    u8 *st;

    idx = v->index;
    core = v->core;
    prog = v->prog;
    ent = core | (idx << 1);
    bit = 1 << idx;
    sceHsynVoiceVolumeCommit(v);
    if (v->vag != NULL) {
        sceHsynNoiseMask[v->core] &= ~bit;
        sceHsynVoicePitchCommit(v);
        sceHsynSdSetAddr(ent | 0x2040, v->spuAddr + v->vag->spuOfs);
        sceHsynSdSetParam(ent | 0x300, *(u16 *)&prog->unk0E);
        sceHsynSdSetParam(ent | 0x400, *(u16 *)&prog->unk10[0]);
        if ((*((u8 *)v->vag + 6) & 1) == 0) {
            v->flags |= 1;
        }
    } else {
        note = v->ch;
        if (note >= 0x40) {
            note = 0x3F;
        }
        sceHsynSdSetCoreAttr(v->core | 8, note);
        sceHsynNoiseMask[v->core] |= bit;
    }
    sceHsynSdSetSwitch(v->core | 0x1400, sceHsynNoiseMask[v->core]);
    if ((prog->unk0D & 1) != 0) {
        sceHsynMixMaskL[v->core][0] |= bit;
    } else {
        sceHsynMixMaskL[v->core][0] &= ~bit;
    }
    if ((prog->unk0D & 2) != 0) {
        sceHsynMixMaskL[v->core][1] |= bit;
    } else {
        sceHsynMixMaskL[v->core][1] &= ~bit;
    }
    if ((prog->unk0D & 4) != 0) {
        sceHsynMixMaskR[v->core][0] |= bit;
    } else {
        sceHsynMixMaskR[v->core][0] &= ~bit;
    }
    if ((prog->unk0D & 8) != 0) {
        sceHsynMixMaskR[v->core][1] |= bit;
    } else {
        sceHsynMixMaskR[v->core][1] &= ~bit;
    }
    sceHsynSdSetSwitch(v->core | 0x1800, sceHsynMixMaskL[v->core][0]);
    sceHsynSdSetSwitch(v->core | 0x1A00, sceHsynMixMaskL[v->core][1]);
    sceHsynSdSetSwitch(v->core | 0x1900, sceHsynMixMaskR[v->core][0]);
    sceHsynSdSetSwitch(v->core | 0x1B00, sceHsynMixMaskR[v->core][1]);
    sceHsynKeyOnPending[v->core] |= bit;
    sceHsynLastKeyOnCore = v->core;
    sceHsynLastKeyOnVoice = v->index;
    thbase_GetSystemTime(clk);
    thbase_SysClock2USec(clk, &sceHsynSettleTimeHi, &sceHsynSettleTimeLo);
    v->state = 3;
    v->flags = v->flags & 0xFFF7;
    if (sceHsynStat != NULL) {
        st = sceHsynStat->state[v->core];
        st[v->index] = (st[v->index] & 0x8F) | 0x20;
    }
}
/* ENDF sceHsynVoiceKeyOnPathB */
/* FUNC sceHsynEnvAttackStep */
void sceHsynEnvAttackStep(sceHSynVoice_t *v, u8 scale)
{
    sceSmgProgEnt_t *p;
    int t;

    p = v->prog;
    t = (scale * p->unk20) / 128;
    t += v->unk51;
    if (t >= 0x80) {
        t = 0x7F;
    } else if (t < -0x80) {
        t = -0x80;
    }
    v->unk4F = t;
    t = (scale * p->unk21) / 128;
    t += v->unk52;
    if (t >= 0x80) {
        t = 0x7F;
    } else if (t < -0x80) {
        t = -0x80;
    }
    v->unk50 = t;
}
/* ENDF sceHsynEnvAttackStep */
/* FUNC sceHsynEnvDecayStep */
void sceHsynEnvDecayStep(sceHSynVoice_t *v, u8 scale)
{
    sceSmgProgEnt_t *p;
    int t;

    p = v->prog;
    t = (scale * p->unk1C) / 128;
    t += v->unk24;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk20 = t;
    t = (scale * p->unk1E) / 128;
    t += v->unk26;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk22 = t;
}
/* ENDF sceHsynEnvDecayStep */
/* FUNC sceHsynUnrefEnvDecayStep */
void sceHsynUnrefEnvDecayStep(sceHSynVoice_t *v, u16 scale)
{
    sceSmgProgEnt_t *p;
    int t;

    p = v->prog;
    t = (scale * p->unk1C) / 128;
    t += v->unk24;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk20 = t;
    t = (scale * p->unk1E) / 128;
    t += v->unk26;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk22 = t;
}
/* ENDF sceHsynUnrefEnvDecayStep */
#ifdef NON_MATCHING
/* Two instructions short: the same second-wrap-loop shape as
 * sceHsynSdSetParamAdsr. */
/* FUNC sceHsynVoiceSetPanBase */
/* The +4 byte of the program entry is read signed here and unsigned as a key
 * count in sceHsynChannelSetProgram, so the cast stands in for a field the struct
 * cannot hold both ways. */
void sceHsynVoiceSetPanBase(sceHSynVoice_t *v)
{
    int x;

    x = *(s8 *)&v->prog->nKey;
    if (v->unk4E != 0) {
        while (x >= 0x80) {
            x -= 0xFF;
        }
        while (x < -0x80) {
            x += 0xFF;
        }
        v->unk1E = x;
        return;
    }
    if (x < 0) {
        x = -x;
    }
    if (x < 0) {
        x = 0;
    } else if (x >= 0x80) {
        x = 0x7F;
    }
    v->unk53 = 1;
    v->unk1E = x;
}
/* ENDF sceHsynVoiceSetPanBase */
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceSetPanBase);
#endif
ALIGN_TEXT(4);
/* FUNC sceHsynSpu2SettleDelay */
int sceHsynSpu2SettleDelay()
{
    u32 clk[2];
    u32 sec;
    int usec;
    int t;

    thbase_GetSystemTime(clk);
    thbase_SysClock2USec(clk, &sec, (u32 *)&usec);
    if (sec == sceHsynSettleTimeHi) {
        usec -= sceHsynSettleTimeLo;
    } else {
        sec--;
        if (sec == sceHsynSettleTimeHi) {
            return 1;
        }
        t = usec + 1000000;
        usec = t - sceHsynSettleTimeLo;
    }
    return usec >= 0x15;
}
/* ENDF sceHsynSpu2SettleDelay */
/* FUNC sceHsynChannelNoteOff */
void sceHsynChannelNoteOff(sceHSynChannel_t *chan, u8 ch, u32 tag)
{
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    u8 *st;
    u16 flags;

    if (chan->portaNote < 0x81) {
        chan->portaNote = ch;
    }
    end = (sceHSynVoice_t *)((char *)chan - 8);
    for (v = end->chPrev; v != end; v = v->chPrev) {
        if (v->ch != ch) {
            continue;
        }
        if (v->tag != tag && tag != 0xFF) {
            continue;
        }
        next = v->chNext;
        sceHsynVoiceChUnlink(v);
        flags = v->flags;
        v->flags = flags & 0xFCFF;
        if (chan->unk32 != 0) {
            v->flags = (flags & 0xFCFF) | 0x100;
            sceHsynVoiceChLink(v, &chan->relNext);
        } else if ((flags & 8) != 0) {
            sceHsynVoiceFree(v);
        } else {
            sceHsynLfoKeyOff(&v->envA);
            sceHsynLfoKeyOff(&v->envB);
            sceHsynKeyOffPending[v->core] |= 1 << v->index;
            sceHsynVoiceChLink(v, &chan->offNext);
            if (sceHsynStat != NULL) {
                st = &sceHsynStat->state[v->core][v->index];
                *st = (*st & 0x8F) | 0x30;
            }
        }
        v = next;
    }
}
/* ENDF sceHsynChannelNoteOff */
/* FUNC sceHsynVoiceFree */
int sceHsynVoiceFree(sceHSynVoice_t *v)
{
    sceHSynChanSet_t *set;
    u8 *st;
    int n;

    set = v->slotp;
    v->flags = 0;
    n = set->nActive;
    if (n != 0) {
        set->nActive = n - 1;
    }
    if (v->unk57 == 0) {
        n = sceHsynActiveCount;
        if (n != 0) {
            sceHsynActiveCount = n - 1;
        }
    } else if (v->unk57 == 1) {
        n = sceHsynRelCount;
        if (n != 0) {
            sceHsynRelCount = n - 1;
        }
    }
    sceHsynVoiceUnlink(v);
    sceHsynFreeCount[v->core]++;
    sceHsynVoiceInsertAfter(v, (sceHSynVoice_t *)sceHsynFreeList[v->core]);
    if (sceHsynStat != NULL) {
        st = &sceHsynStat->state[v->core][v->index];
        *st &= 0x8F;
    }
    v->tag = 0xFF;
    v->unk57 = 0xFF;
    v->unk58 = 0xFF;
    return 0xFF;
}
/* ENDF sceHsynVoiceFree */
/* FUNC sceHsynChannelSetHold */
void sceHsynChannelSetHold(sceHSynChannel_t *chan, u8 val)
{
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    u8 *st;

    if (val != 0) {
        chan->unk32 = val;
    } else if (chan->unk32 != 0) {
        chan->unk32 = val;
        end = (sceHSynVoice_t *)chan;
        for (v = end->chNext; v != end; v = end->chNext) {
            sceHsynVoiceChUnlink(v);
            if ((v->flags & 8) != 0) {
                sceHsynVoiceFree(v);
            } else {
                sceHsynLfoKeyOff(&v->envA);
                sceHsynLfoKeyOff(&v->envB);
                sceHsynKeyOffPending[v->core] |= 1 << v->index;
                sceHsynVoiceChLink(v, &chan->offNext);
                v->flags &= 0xFCFF;
                if (sceHsynStat != NULL) {
                    st = &sceHsynStat->state[v->core][v->index];
                    *st = (*st & 0x8F) | 0x30;
                }
            }
        }
    }
}
/* ENDF sceHsynChannelSetHold */
/* FUNC sceHsynChannelAllNotesOff */
void sceHsynChannelAllNotesOff(sceHSynChannel_t *chan)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    u8 *st;
    u16 flags;
    int i;

    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    for (i = 0; i < 2; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = end->chPrev) {
            sceHsynVoiceChUnlink(v);
            flags = v->flags;
            if ((flags & 8) != 0) {
                sceHsynVoiceFree(v);
            } else {
                v->flags = flags & 0xFCFF;
                sceHsynKeyOffPending[v->core] |= 1 << v->index;
                sceHsynVoiceChLink(v, &chan->offNext);
                if (sceHsynStat != NULL) {
                    st = &sceHsynStat->state[v->core][v->index];
                    *st = (*st & 0x8F) | 0x30;
                }
            }
        }
    }
}
/* ENDF sceHsynChannelAllNotesOff */
/* FUNC sceHsynChannelAllSoundOff */
void sceHsynChannelAllSoundOff(sceHSynChannel_t *chan)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = end->chPrev) {
            sceHsynKeyOffPending[v->core] |= 1 << v->index;
            sceHsynSdSetParam((v->core | (v->index << 1)) | 0x400, 0xC021);
            sceHsynVoiceChFree(v);
        }
    }
}
/* ENDF sceHsynChannelAllSoundOff */
/* FUNC sceHsynVoiceChFree */
int sceHsynVoiceChFree(sceHSynVoice_t *v)
{
    sceHSynChanSet_t *set;
    u8 *st;
    int n;

    sceHsynVoiceChUnlink(v);
    set = v->slotp;
    v->flags = 0;
    n = set->nActive;
    if (n != 0) {
        set->nActive = n - 1;
    }
    if (v->unk57 == 0) {
        n = sceHsynActiveCount;
        if (n != 0) {
            sceHsynActiveCount = n - 1;
        }
    } else if (v->unk57 == 1) {
        n = sceHsynRelCount;
        if (n != 0) {
            sceHsynRelCount = n - 1;
        }
    }
    sceHsynVoiceUnlink(v);
    sceHsynFreeCount[v->core]++;
    sceHsynVoiceInsertAfter(v, (sceHSynVoice_t *)sceHsynFreeList[v->core]);
    if (sceHsynStat != NULL) {
        st = &sceHsynStat->state[v->core][v->index];
        *st &= 0x8F;
    }
    v->tag = 0xFF;
    v->unk57 = 0xFF;
    v->unk58 = 0xFF;
    v->lfoA.unk00 = 0;
    v->lfoB.unk00 = 0;
    *(u32 *)v->param = 0;
    return 0xFF;
}
/* ENDF sceHsynVoiceChFree */
/* FUNC sceHsynVoiceStealScan */
int sceHsynVoiceStealScan(sceHSynChanSet_t *cs, u16 vnum)
{
    sceHSynVoice_t *best;
    sceHSynVoice_t *chan;
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    best = NULL;
    for (i = 0; i < 16; i++) {
        chan = (sceHSynVoice_t *)&cs->chan[i];
        end = (sceHSynVoice_t *)((char *)chan + 8);
        for (v = end->chNext; v != end; v = v->chNext) {
            if (v->vnum <= vnum) {
                best = v;
                vnum = v->vnum;
            }
        }
    }
    if (best == NULL) {
        for (i = 0; i < 16; i++) {
            chan = (sceHSynVoice_t *)&cs->chan[i];
            end = (sceHSynVoice_t *)((char *)chan - 8);
            for (v = end->chNext; v != end; v = v->chNext) {
                if (v->vnum <= vnum) {
                    best = v;
                    vnum = v->vnum;
                }
            }
            end = chan;
            for (v = end->chNext; v != end; v = v->chNext) {
                if (v->vnum <= vnum) {
                    best = v;
                    vnum = v->vnum;
                }
            }
        }
        if (best == NULL) {
            return 0;
        }
    }
    sceHsynKeyOffPending[best->core] |= 1 << best->index;
    sceHsynSdSetParam((best->core | (best->index << 1)) | 0x400, 0xC021);
    sceHsynVoiceChFree(best);
    return 1;
}
/* ENDF sceHsynVoiceStealScan */
#ifdef NON_MATCHING
/* Equivalent but not matching: the retail body spills a dozen values to the
 * frame and a C body of this shape keeps them in registers. */
/* FUNC sceHsynChannelNoteOn */
void sceHsynChannelNoteOn(sceHSynSlot_t *slot, sceHSynChannel_t *chan, int status, u8 note, u8 vel, u32 tag)
{
    sceHSynChanSet_t *set;
    sceSmgProgEnt_t *prog;
    sceHSynBank_t *bank;
    sceSmgProgKey_t *key;
    sceSmgSset_t *sset;
    sceSmgSmpl_t *smpl;
    sceSmgVagi_t *vagi;
    sceSmgSsetEnt_t *se;
    sceSmgSmplEnt_t *tone;
    sceSmgVagiEnt_t *vag;
    sceHSynVoice_t *v;
    u8 *st;
    int nKey;
    int i;
    int vnum;
    int started;
    u8 stateByte;

    set = (sceHSynChanSet_t *)&slot->unk14;
    prog = chan->prog;
    bank = chan->bank;
    if (prog == NULL) {
        return;
    }
    started = 0;
    nKey = prog->nKey;
    key = (sceSmgProgKey_t *)((u8 *)prog + prog->keyOfs);
    sset = bank->sset;
    smpl = bank->smpl;
    vagi = bank->vagi;
    if (nKey <= 0) {
        return;
    }
    stateByte = status | 0x90;
    do {
        if (note < (key->keyLow & 0x7F) || (key->keyHigh & 0x7F) < note) {
            goto nextKey;
        }
        if (sset->nEntry < key->ssetIdx) {
            goto nextKey;
        }
        se = (sceSmgSsetEnt_t *)((u8 *)sset + sset->ofs[key->ssetIdx]);
        if (vel < se->velLow || se->velHigh < vel) {
            goto nextKey;
        }
        if (se->nSmpl == 0) {
            goto nextKey;
        }
        for (i = 0; i < se->nSmpl; i++) {
            if (smpl->nEntry < se->smplIdx[i]) {
                continue;
            }
            tone = (sceSmgSmplEnt_t *)((u8 *)smpl + smpl->ofs[se->smplIdx[i]]);
            if (vel < (tone->velLow & 0x7F) || (tone->velHigh & 0x7F) < vel) {
                continue;
            }
            vag = NULL;
            if (vagi->nEntry >= tone->vagiIdx) {
                vag = (sceSmgVagiEnt_t *)((u8 *)vagi + vagi->ofs[tone->vagiIdx]);
                if (vag->spuOfs == -1) {
                    continue;
                }
            }
            if (tone->pitch != 0) {
                sceHsynChannelKeyOffByPitch(chan, tone->pitch);
            }
            vnum = tone->voiceOfs + slot->voiceBase;
            if (set->nActive >= slot->maxVoice) {
                if (sceHsynVoiceStealScan(set, vnum) == 0) {
                    return;
                }
            }
            v = sceHsynVoiceAlloc(tone->allocHint);
            if (v == NULL) {
                if (sceHsynSdSyncFlag != 0) {
                    sceHsynSdSyncFlag = 0;
                    if (sceHsynVoiceKeyOff() != 0) {
                        v = sceHsynVoiceAlloc(tone->allocHint);
                    }
                }
                if (v == NULL) {
                    v = sceHsynVoiceStealByEnvPhase(vnum, tone->allocHint);
                    if (v == NULL) {
                        v = sceHsynVoiceStealByVnumActive(vnum, tone->allocHint);
                        if (v == NULL) {
                            continue;
                        }
                    }
                }
            }
            v->flags = 0x208;
            v->slotp = set;
            v->chan = chan;
            v->prog = prog;
            v->tone = tone;
            v->vag = vag;
            v->key = key;
            v->unk12 = 0;
            v->unk38 = 0;
            v->vnum = vnum;
            v->ch = note;
            v->tag = tag;
            v->unk2C = 0;
            v->spuAddr = bank->spuAddr;
            v->vel = vel;
            v->pitch = tone->pitch;
            v->unk4A = se->unk00;
            v->unk28 = chan->unk28;
            v->unk55 = chan->unk2F;
            v->unk57 = 0;
            v->vol = chan->unk30;
            v->path = slot->path;
            sceHsynCoreStateGet(v);
            sceHsynCoreStateSet(v);
            sceHsynChannelStateGet(v);
            set->nActive++;
            sceHsynActiveCount++;
            sceHsynVoiceChLink(v, (sceHSynVoice_t **)chan);
            started = 1;
            if ((chan->portaNote & 0x80) == 0 && chan->bendRange != 0) {
                sceHsynVoiceStateClear(v);
            }
            sceHsynVoiceUpdatePathA(v);
            sceHsynVoiceInsertAfter(v, (sceHSynVoice_t *)sceHsynActiveList[v->core]);
            if (sceHsynStat != NULL) {
                st = sceHsynStat->state[v->core];
                st[v->index] = stateByte;
            }
        }
    nextKey:
        key++;
        nKey--;
    } while (nKey > 0);
    if (started != 0) {
        chan->unk28 = -1;
        chan->unk2F = 0xFF;
        chan->unk30 = -1;
    }
}
/* ENDF sceHsynChannelNoteOn */
#else
INCLUDE_ASM("MODHSYN", sceHsynChannelNoteOn);
#endif
/* FUNC sceHsynVoiceAlloc */
sceHSynVoice_t *sceHsynVoiceAlloc(u8 allocHint)
{
    sceHSynVoice_t *v;
    int idx;
    int n;

    switch (allocHint & 0x30) {
    case 0x10:
        idx = 0;
        break;
    case 0x20:
        idx = 1;
        break;
    default:
        idx = sceHsynFreeCount[0] < sceHsynFreeCount[1];
        break;
    }
    n = sceHsynFreeCount[idx];
    if (n == 0) {
        return NULL;
    }
    sceHsynFreeCount[idx] = n - 1;
    v = sceHsynFreeList[idx][1];
    sceHsynVoiceUnlink(v);
    return v;
}
/* ENDF sceHsynVoiceAlloc */
/* FUNC sceHsynVoiceStealByEnvPhase */
sceHSynVoice_t *sceHsynVoiceStealByEnvPhase(u16 vnum, s8 coreMask)
{
    sceHSynVoice_t *best;
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynChanSet_t *set;
    u8 sel;
    u16 cur;
    u16 best300;
    u16 unk12;
    u16 f;
    int i;
    int n;

    sel = coreMask;
    i = 1;
    best = NULL;
    best300 = 0x300;
    unk12 = 0xFFFF;
    cur = vnum;
    if ((coreMask & 0x30) == 0) {
        sel = 0x30;
    }
    do {
        if ((sel & sceHsynCoreBit[i]) != 0) {
            end = (sceHSynVoice_t *)sceHsynCoreList[i];
            for (v = end->prev; v != end; v = v->prev) {
                f = v->flags & 0x300;
                if (v->unk57 != 0) {
                    continue;
                }
                if (f < best300) {
                    if (vnum >= v->vnum) {
                        cur = v->vnum;
                        best300 = f;
                        unk12 = v->unk12;
                        best = v;
                    }
                } else if (f == best300) {
                    if (v->vnum == cur) {
                        if (v->unk12 < unk12) {
                            unk12 = v->unk12;
                            best = v;
                        }
                    } else if (v->vnum < cur) {
                        cur = v->vnum;
                        unk12 = v->unk12;
                        best = v;
                    }
                }
            }
        }
        i--;
    } while (i != -1);
    if (best != NULL) {
        sceHsynKeyOffPending[best->core] |= 1 << best->index;
        set = best->slotp;
        n = set->nActive;
        if (n != 0) {
            set->nActive = n - 1;
        }
        if (best->unk57 == 0) {
            n = sceHsynActiveCount;
            if (n != 0) {
                sceHsynActiveCount = n - 1;
            }
        } else if (best->unk57 == 1) {
            n = sceHsynRelCount;
            if (n != 0) {
                sceHsynRelCount = n - 1;
            }
        }
        sceHsynVoiceChUnlink(best);
        sceHsynVoiceUnlink(best);
        best->flags = 0;
        best->tag = 0xFF;
        best->unk57 = 0xFF;
        best->unk58 = 0xFF;
    }
    return best;
}
/* ENDF sceHsynVoiceStealByEnvPhase */
/* FUNC sceHsynVoiceStealByVnumActive */
sceHSynVoice_t *sceHsynVoiceStealByVnumActive(s16 vnum0, s8 coreMask)
{
    sceHSynVoice_t *best;
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynChanSet_t *set;
    u8 sel;
    u16 vnum;
    int i;
    int n;

    sel = coreMask;
    i = 1;
    best = NULL;
    vnum = vnum0 + 1;
    if ((coreMask & 0x30) == 0) {
        sel = 0x30;
    }
    do {
        if ((sel & sceHsynCoreBit[i]) != 0) {
            end = (sceHSynVoice_t *)sceHsynActiveList[i];
            for (v = end->prev; v != end; v = v->prev) {
                if (v->unk57 == 0 && v->vnum < vnum) {
                    vnum = v->vnum;
                    best = v;
                }
            }
        }
        i--;
    } while (i != -1);
    if (best != NULL) {
        set = best->slotp;
        n = set->nActive;
        if (n != 0) {
            set->nActive = n - 1;
        }
        if (best->unk57 == 0) {
            n = sceHsynActiveCount;
            if (n != 0) {
                sceHsynActiveCount = n - 1;
            }
        } else if (best->unk57 == 1) {
            n = sceHsynRelCount;
            if (n != 0) {
                sceHsynRelCount = n - 1;
            }
        }
        sceHsynVoiceChUnlink(best);
        sceHsynVoiceUnlink(best);
        best->flags = 0;
        best->tag = 0xFF;
        best->unk57 = 0xFF;
        best->unk58 = 0xFF;
    }
    return best;
}
/* ENDF sceHsynVoiceStealByVnumActive */
/* FUNC sceHsynChannelKeyOffByPitch */
void sceHsynChannelKeyOffByPitch(sceHSynChannel_t *chan, u8 pitch)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    int i;

    heads[1] = (sceHSynVoice_t *)chan;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = v->chPrev) {
            if (v->pitch != pitch) {
                continue;
            }
            next = v->chNext;
            if ((v->flags & 8) != 0) {
                sceHsynVoiceChUnlink(v);
                sceHsynVoiceFree(v);
            } else {
                if (i < 2) {
                    sceHsynKeyOffPending[v->core] |= 1 << v->index;
                }
                sceHsynSdSetParam((v->core | (v->index << 1)) | 0x400, 0xC021);
                sceHsynVoiceChFree(v);
            }
            v = next;
        }
    }
}
/* ENDF sceHsynChannelKeyOffByPitch */
/* FUNC sceHsynVoiceKeyOffOne */
sceHSynVoice_t *sceHsynVoiceKeyOffOne(sceHSynVoice_t *v, int keyOff)
{
    sceHSynVoice_t *next;

    next = v->chNext;
    if ((v->flags & 8) != 0) {
        sceHsynVoiceChUnlink(v);
        sceHsynVoiceFreeEntry(v);
        return next;
    }
    if (keyOff == 1) {
        sceHsynKeyOffPending[v->core] |= keyOff << v->index;
    }
    sceHsynSdSetParam((v->core | (v->index << 1)) | 0x400, 0xC021);
    sceHsynVoiceChFreeEntry(v);
    return next;
}
/* ENDF sceHsynVoiceKeyOffOne */
/* FUNC sceHsynChannelNoteOffMatch */
void sceHsynChannelNoteOffMatch(int base, int unused1, int state, u8 note, u8 ch, int unused2, u32 tag)
{
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    u8 *st;
    u16 flags;
    int chanp;

    end = (sceHSynVoice_t *)(base + 16);
    chanp = base + 24;
    for (v = end->chPrev; v != end; v = v->chPrev) {
        if (v->tag != tag && tag != 0xFF) {
            continue;
        }
        if (v->unk58 != state) {
            continue;
        }
        if (v->note != note) {
            continue;
        }
        if (v->ch != ch) {
            continue;
        }
        next = v->chNext;
        sceHsynVoiceChUnlink(v);
        flags = v->flags;
        v->flags = flags & 0xFCFF;
        if ((flags & 8) != 0) {
            sceHsynVoiceFreeEntry(v);
        } else {
            sceHsynLfoKeyOff(&v->envA);
            sceHsynLfoKeyOff(&v->envB);
            sceHsynKeyOffPending[v->core] |= 1 << v->index;
            sceHsynVoiceChLink(v, (sceHSynVoice_t **)(chanp + 16));
            if (sceHsynStat != NULL) {
                st = &sceHsynStat->state[v->core][v->index];
                *st = (*st & 0x8F) | 0x30;
            }
        }
        v = next;
    }
}
/* ENDF sceHsynChannelNoteOffMatch */
/* FUNC sceHsynVoiceFreeEntry */
int sceHsynVoiceFreeEntry(sceHSynVoice_t *v)
{
    return sceHsynVoiceFree(v);
}
/* ENDF sceHsynVoiceFreeEntry */
/* FUNC sceHsynChannelKeyOffAll */
void sceHsynChannelKeyOffAll(sceHSynChannel_t *chan)
{
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    u8 *st;
    u16 flags;

    end = (sceHSynVoice_t *)((char *)chan - 8);
    for (v = end->chPrev; v != end; v = end->chPrev) {
        sceHsynVoiceChUnlink(v);
        flags = v->flags;
        if ((flags & 8) != 0) {
            sceHsynVoiceFreeEntry(v);
        } else {
            v->flags = flags & 0xFCFF;
            sceHsynKeyOffPending[v->core] |= 1 << v->index;
            sceHsynVoiceChLink(v, &chan->offNext);
            if (sceHsynStat != NULL) {
                st = &sceHsynStat->state[v->core][v->index];
                *st = (*st & 0x8F) | 0x30;
            }
        }
    }
}
/* ENDF sceHsynChannelKeyOffAll */
/* FUNC sceHsynChannelKeyOffByTag */
void sceHsynChannelKeyOffByTag(int base, u32 tag)
{
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    u8 *st;
    u16 flags;

    end = (sceHSynVoice_t *)(base - 8);
    for (v = end->chPrev; v != end; v = v->chPrev) {
        if (v->tag != tag) {
            continue;
        }
        next = v->chNext;
        sceHsynVoiceChUnlink(v);
        flags = v->flags;
        v->flags = flags & 0xFCFF;
        if ((flags & 8) != 0) {
            sceHsynVoiceFreeEntry(v);
        } else {
            sceHsynLfoKeyOff(&v->envA);
            sceHsynLfoKeyOff(&v->envB);
            sceHsynKeyOffPending[v->core] |= 1 << v->index;
            sceHsynVoiceChLink(v, (sceHSynVoice_t **)(base + 16));
            if (sceHsynStat != NULL) {
                st = &sceHsynStat->state[v->core][v->index];
                *st = (*st & 0x8F) | 0x30;
            }
        }
        v = next;
    }
}
/* ENDF sceHsynChannelKeyOffByTag */
/* FUNC sceHsynChannelKeyOffByTagEntry */
void sceHsynChannelKeyOffByTagEntry(int chan, u32 tag)
{
    sceHsynChannelKeyOffByTag(chan + 0x18, tag);
}
/* ENDF sceHsynChannelKeyOffByTagEntry */
/* FUNC sceHsynChannelKeyOffByTagMask */
void sceHsynChannelKeyOffByTagMask(int base, int unused, u32 tag, u32 mask)
{
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynVoice_t *next;
    u8 *st;
    u16 flags;
    u32 key;

    end = (sceHSynVoice_t *)(base - 8);
    key = tag & mask;
    for (v = end->chPrev; v != end; v = v->chPrev) {
        if ((v->tag & mask) != key) {
            continue;
        }
        next = v->chNext;
        sceHsynVoiceChUnlink(v);
        flags = v->flags;
        v->flags = flags & 0xFCFF;
        if ((flags & 8) != 0) {
            sceHsynVoiceFreeEntry(v);
        } else {
            sceHsynLfoKeyOff(&v->envA);
            sceHsynLfoKeyOff(&v->envB);
            sceHsynKeyOffPending[v->core] |= 1 << v->index;
            sceHsynVoiceChLink(v, (sceHSynVoice_t **)(base + 16));
            if (sceHsynStat != NULL) {
                st = &sceHsynStat->state[v->core][v->index];
                *st = (*st & 0x8F) | 0x30;
            }
        }
        v = next;
    }
}
/* ENDF sceHsynChannelKeyOffByTagMask */
/* FUNC sceHsynChannelKeyOffByTagMaskEntry */
void sceHsynChannelKeyOffByTagMaskEntry(int chan, int unused, u32 tag, u32 mask)
{
    sceHsynChannelKeyOffByTagMask(chan + 0x18, unused, tag, mask);
}
/* ENDF sceHsynChannelKeyOffByTagMaskEntry */
/* FUNC sceHsynChannelStopVoices */
void sceHsynChannelStopVoices(sceHSynChannel_t *chan)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 2; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = end->chPrev) {
            sceHsynKeyOffPending[v->core] |= 1 << v->index;
            sceHsynSdSetParam((v->core | (v->index << 1)) | 0x400, 0xC021);
            sceHsynVoiceChFreeEntry(v);
        }
    }
}
/* ENDF sceHsynChannelStopVoices */
/* FUNC sceHsynVoiceChFreeEntry */
int sceHsynVoiceChFreeEntry(sceHSynVoice_t *v)
{
    return sceHsynVoiceChFree(v);
}
/* ENDF sceHsynVoiceChFreeEntry */
/* FUNC sceHsynVoiceStealFromChannel */
int sceHsynVoiceStealFromChannel(int chan, u16 vnum)
{
    sceHSynVoice_t *best;
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;

    end = (sceHSynVoice_t *)(chan + 12);
    best = NULL;
    for (v = end->chNext; v != end; v = v->chNext) {
        if (v->vnum <= vnum) {
            best = v;
            vnum = v->vnum;
        }
    }
    if (best == NULL) {
        end = (sceHSynVoice_t *)(chan - 4);
        for (v = end->chNext; v != end; v = v->chNext) {
            if (v->vnum <= vnum) {
                best = v;
                vnum = v->vnum;
            }
        }
        if (best == NULL) {
            return 0;
        }
    }
    sceHsynKeyOffPending[best->core] |= 1 << best->index;
    sceHsynSdSetParam((best->core | (best->index << 1)) | 0x400, 0xC021);
    sceHsynVoiceChFreeEntry(best);
    return 1;
}
/* ENDF sceHsynVoiceStealFromChannel */
#ifdef NON_MATCHING
/* Equivalent but not matching: the retail body spills a dozen values to the
 * frame and a C body of this shape keeps them in registers. */
/* FUNC sceHsynVoiceStartPathA */
int sceHsynVoiceStartPathA(sceHSynSlot_t *slot, int a2, u32 a3, u8 a4, u8 a5, int a6, s8 a7, int a8, u32 a9)
{
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    sceHSynBank_t *bank;
    sceSmgProg_t *prog;
    sceSmgProgEnt_t *ent;
    sceSmgProgKey_t *key;
    sceSmgSset_t *sset;
    sceSmgSmpl_t *smpl;
    sceSmgVagi_t *vagi;
    sceSmgSsetEnt_t *se;
    sceSmgSmplEnt_t *tone;
    sceSmgVagiEnt_t *vag;
    sceHSynVoice_t *v;
    u8 *st;
    u32 ofs;
    int nKey;
    int i;
    int vnum;
    int started;
    u8 stateByte;

    set = (sceHSynChanSet_t *)&slot->unk14;
    chan = slot->chan;
    bank = &set->bank[a3];
    ent = NULL;
    prog = bank->prog;
    if (prog != NULL && prog->nEntry >= a4) {
        ofs = prog->ofs[a4];
        if (ofs != -1) {
            ent = (sceSmgProgEnt_t *)((u8 *)prog + ofs);
            if (ent->nKey == 0) {
                ent = NULL;
            }
        }
    }
    if (ent == NULL) {
        return -1;
    }
    started = 0;
    chan->prog = ent;
    nKey = ent->nKey;
    key = (sceSmgProgKey_t *)((u8 *)ent + ent->keyOfs);
    sset = bank->sset;
    smpl = bank->smpl;
    vagi = bank->vagi;
    if (nKey <= 0) {
        goto tail;
    }
    stateByte = a2 | 0x90;
    do {
        if (a5 < (key->keyLow & 0x7F) || (key->keyHigh & 0x7F) < a5) {
            goto nextKey;
        }
        if (sset->nEntry < key->ssetIdx) {
            goto nextKey;
        }
        se = (sceSmgSsetEnt_t *)((u8 *)sset + sset->ofs[key->ssetIdx]);
        if (a6 < se->velLow || se->velHigh < a6) {
            goto nextKey;
        }
        if (se->nSmpl == 0) {
            goto nextKey;
        }
        for (i = 0; i < se->nSmpl; i++) {
            if (smpl->nEntry < se->smplIdx[i]) {
                continue;
            }
            tone = (sceSmgSmplEnt_t *)((u8 *)smpl + smpl->ofs[se->smplIdx[i]]);
            if (a6 < (tone->velLow & 0x7F) || (tone->velHigh & 0x7F) < a6) {
                continue;
            }
            vag = NULL;
            if (vagi->nEntry >= tone->vagiIdx) {
                vag = (sceSmgVagiEnt_t *)((u8 *)vagi + vagi->ofs[tone->vagiIdx]);
                if (vag->spuOfs == -1) {
                    continue;
                }
            }
            if (tone->pitch != 0) {
                sceHsynChannelKeyOffByPitch(chan, tone->pitch);
            }
            vnum = tone->voiceOfs + slot->voiceBase;
            if (set->nActive >= slot->maxVoice) {
                if (sceHsynVoiceStealFromChannel((int)set, vnum) == 0) {
                    return -1;
                }
            }
            if (sceHsynModDisable != 0 && sceHsynRelCount >= sceHsynModDisable) {
                v = sceHsynVoiceStealByEnvPhaseEx(vnum, tone->allocHint, 1, 1);
            } else {
                v = sceHsynVoiceAlloc(tone->allocHint);
                if (v == NULL) {
                    if (sceHsynSdSyncFlag != 0) {
                        sceHsynSdSyncFlag = 0;
                        if (sceHsynVoiceKeyOff() != 0) {
                            v = sceHsynVoiceAlloc(tone->allocHint);
                        }
                    }
                    if (v == NULL) {
                        v = (sceHSynVoice_t *)sceHsynVoiceSteal(vnum, tone->allocHint);
                        if (v == NULL) {
                            v = sceHsynVoiceStealByVnum(vnum, tone->allocHint);
                        }
                    }
                }
            }
            if (v == NULL) {
                continue;
            }
            v->flags = 0x208;
            if ((slot->flags & 2) != 0) {
                v->flags = 0x608;
            }
            v->slotp = set;
            v->chan = chan;
            v->prog = ent;
            v->key = key;
            v->tone = tone;
            v->vag = vag;
            v->unk12 = 0;
            v->unk38 = 0;
            v->vnum = vnum;
            v->note = a4;
            v->ch = a5;
            v->tag = a9;
            v->vel = a6;
            v->unk2C = a8;
            v->spuAddr = bank->spuAddr;
            v->pitch = tone->pitch;
            v->unk4A = se->unk00;
            v->unk28 = chan->unk28;
            v->unk55 = chan->unk2F;
            v->vol = a7;
            v->unk57 = 1;
            v->unk58 = a3;
            v->path = slot->path;
            sceHsynCoreStateGet(v);
            sceHsynCoreStateSet(v);
            sceHsynChannelStateGet(v);
            set->nActive++;
            sceHsynRelCount++;
            sceHsynVoiceChLink(v, (sceHSynVoice_t **)chan);
            started = 1;
            if ((chan->portaNote & 0x80) == 0 && chan->bendRange != 0) {
                sceHsynVoiceStateClear(v);
            }
            sceHsynVoiceUpdatePathA(v);
            sceHsynVoiceInsertAfter(v, (sceHSynVoice_t *)sceHsynActiveList[v->core]);
            if (sceHsynStat != NULL) {
                st = sceHsynStat->state[v->core];
                st[v->index] = stateByte;
            }
        }
    nextKey:
        key++;
        nKey--;
    } while (nKey > 0);
tail:
    if (started != 0) {
        chan->unk28 = -1;
        chan->unk2F = 0xFF;
        chan->unk30 = -1;
    }
    return 0;
}
/* ENDF sceHsynVoiceStartPathA */
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceStartPathA);
#endif
#ifdef NON_MATCHING
/* Equivalent but not matching: the retail body keeps its narrow parameters in
 * frame slots and a C body of this shape keeps them in registers. */
/* FUNC sceHsynVoiceStartPathB */
int sceHsynVoiceStartPathB(sceHSynSlot_t *slot, int a2, u32 a3, u8 a4, int a5, int a6, s8 a7, int a8, u32 a9)
{
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    sceHSynBank_t *bank;
    sceSmgSetb_t *setb;
    sceSmgVagi_t *vagi;
    sceSmgVagiEnt_t *vag;
    sceHSynVoice_t *v;
    u8 *grp;
    u8 *ent;
    u8 *st;
    u32 ofs;
    int lo;
    int vnum;

    set = (sceHSynChanSet_t *)&slot->unk14;
    chan = slot->chan;
    bank = &set->bank[0];
    if (bank == NULL) {
        return -1;
    }
    setb = bank->setb;
    vagi = bank->vagi;
    if (setb == NULL || vagi == NULL) {
        return -1;
    }
    if (setb->nEntry < a3) {
        return -1;
    }
    ofs = setb->ofs[a3];
    if (ofs == -1) {
        return -1;
    }
    grp = (u8 *)setb + ofs;
    if (*(u32 *)grp < a4) {
        return -1;
    }
    ent = (u8 *)setb + *(u32 *)(grp + a4 * 4 + 4);
    lo = ent[5];
    if (a5 < lo || ent[6] < a5) {
        return -1;
    }
    ent = (u8 *)ent + *(u32 *)ent + ent[4] * ((a5 - lo) & 0xFF);
    vag = NULL;
    if (vagi->nEntry >= *(u16 *)ent) {
        vag = (sceSmgVagiEnt_t *)((u8 *)vagi + vagi->ofs[*(u16 *)ent]);
        if (vag->spuOfs == -1) {
            return -1;
        }
    }
    if (ent[7] != 0) {
        sceHsynChannelLimitByPitch((int)chan, ent[7], ent[8]);
    }
    vnum = ent[9] + slot->voiceBase;
    if (set->nActive >= slot->maxVoice) {
        if (sceHsynVoiceStealFromChannel((int)set, vnum) == 0) {
            return -1;
        }
    }
    v = sceHsynVoiceAllocEntry(ent[0xD]);
    if (v == NULL) {
        if (sceHsynSdSyncFlag != 0) {
            sceHsynSdSyncFlag = 0;
            if (sceHsynVoiceKeyOff() != 0) {
                v = sceHsynVoiceAllocEntry(ent[0xD]);
            }
        }
        if (v == NULL) {
            v = (sceHSynVoice_t *)sceHsynVoiceSteal(vnum, ent[0xD]);
            if (v == NULL) {
                v = sceHsynVoiceStealByVnum(vnum, ent[0xD]);
                if (v == NULL) {
                    return -1;
                }
            }
        }
    }
    v->flags = 0x208;
    if ((slot->flags & 2) != 0) {
        v->flags = 0x608;
    }
    v->slotp = set;
    v->chan = chan;
    v->prog = (sceSmgProgEnt_t *)ent;
    v->key = NULL;
    v->tone = NULL;
    v->vag = vag;
    v->unk12 = 0;
    v->unk38 = 0;
    v->vnum = vnum;
    v->note = a4;
    v->ch = a5;
    v->tag = a9;
    v->vel = a6;
    v->unk2C = a8;
    v->spuAddr = bank->spuAddr;
    v->pitch = ent[7];
    v->unk28 = -1;
    v->unk55 = 0xFF;
    v->vol = a7;
    v->unk57 = 1;
    v->unk4A = ent[2];
    v->unk58 = a3;
    v->path = slot->path;
    sceHsynCoreStateGet(v);
    sceHsynCoreStateSet(v);
    sceHsynChannelStateGet(v);
    set->nActive++;
    sceHsynRelCount++;
    sceHsynVoiceChLink(v, (sceHSynVoice_t **)chan);
    sceHsynVoiceUpdatePathB(v);
    sceHsynVoiceInsertAfter(v, (sceHSynVoice_t *)sceHsynActiveList[v->core]);
    if (sceHsynStat != NULL) {
        st = sceHsynStat->state[v->core];
        st[v->index] = a2 | 0x90;
    }
    return 0;
}
/* ENDF sceHsynVoiceStartPathB */
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceStartPathB);
#endif
#ifdef NON_MATCHING
/* Byte-identical apart from one register pair: gcc gives a8 the lower
 * call-clobbered register where retail gives it to a7. */
/* FUNC sceHsynVoiceStart */
int sceHsynVoiceStart(sceHSynSlot_t *slot, int a2, u32 a3, u8 a4, u8 a5, u8 a6, int a7, u16 a8, u32 a9)
{
    switch (slot->path) {
    case 0:
        return sceHsynVoiceStartPathA(slot, a2, a3, a4, a5, a6, a7, a8, a9);
    case 1:
        return sceHsynVoiceStartPathB(slot, a2, a3, a4, a5, a6, a7, a8, a9);
    }
    return -1;
}
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceStart);
#endif
/* ENDF sceHsynVoiceStart */
/* FUNC sceHsynVoiceAllocEntry */
sceHSynVoice_t *sceHsynVoiceAllocEntry(u8 allocHint)
{
    return sceHsynVoiceAlloc(allocHint);
}
/* ENDF sceHsynVoiceAllocEntry */
/* FUNC sceHsynVoiceStealByEnvPhaseEx */
sceHSynVoice_t *sceHsynVoiceStealByEnvPhaseEx(u16 vnum, u8 coreMask, int on, int done)
{
    sceHSynVoice_t *best;
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynChanSet_t *set;
    u8 sel;
    u16 cur;
    u16 best300;
    u16 unk12;
    u16 f;
    int i;
    int n;

    sel = coreMask;
    i = 1;
    best = NULL;
    best300 = 0x300;
    unk12 = 0xFFFF;
    cur = vnum;
    if ((coreMask & 0x30) == 0) {
        sel = 0x30;
    }
    do {
        if ((sel & sceHsynCoreBit[i]) != 0) {
            end = (sceHSynVoice_t *)sceHsynCoreList[i];
            for (v = end->prev; v != end; v = v->prev) {
                f = v->flags & 0x300;
                if (on != 0) {
                    if (done != 0) {
                        if (v->unk57 != 1) {
                            continue;
                        }
                    } else if (v->unk57 == 1) {
                        continue;
                    }
                }
                if (f < best300) {
                    if (vnum >= v->vnum) {
                        cur = v->vnum;
                        best300 = f;
                        unk12 = v->unk12;
                        best = v;
                    }
                } else if (f == best300) {
                    if (v->vnum == cur) {
                        if (v->unk12 < unk12) {
                            unk12 = v->unk12;
                            best = v;
                        }
                    } else if (v->vnum < cur) {
                        cur = v->vnum;
                        unk12 = v->unk12;
                        best = v;
                    }
                }
            }
        }
        i--;
    } while (i != -1);
    if (best != NULL) {
        sceHsynKeyOffPending[best->core] |= 1 << best->index;
        set = best->slotp;
        n = set->nActive;
        if (n != 0) {
            set->nActive = n - 1;
        }
        if (best->unk57 == 0) {
            n = sceHsynActiveCount;
            if (n != 0) {
                sceHsynActiveCount = n - 1;
            }
        } else if (best->unk57 == 1) {
            n = sceHsynRelCount;
            if (n != 0) {
                sceHsynRelCount = n - 1;
            }
        }
        sceHsynVoiceChUnlink(best);
        sceHsynVoiceUnlink(best);
        best->flags = 0;
        best->tag = 0xFF;
        best->unk57 = 0xFF;
        best->unk58 = 0xFF;
    }
    return best;
}
/* ENDF sceHsynVoiceStealByEnvPhaseEx */
/* FUNC sceHsynVoiceSteal */
int sceHsynVoiceSteal(u16 a, u8 b)
{
    u8 limit;
    int on;
    int done;

    on = 1;
    done = 0;
    limit = sceHsynModDisable;
    if (limit == 0) {
        on = 0;
    } else if (sceHsynRelCount >= limit) {
        done = 1;
    }
    return (int)sceHsynVoiceStealByEnvPhaseEx(a, b, on, done);
}
/* ENDF sceHsynVoiceSteal */
/* FUNC sceHsynVoiceStealByVnum */
sceHSynVoice_t *sceHsynVoiceStealByVnum(s16 vnum0, char allocHint)
{
    sceHSynVoice_t *best;
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    sceHSynChanSet_t *set;
    u8 sel;
    u16 vnum;
    int i;
    int n;

    sel = allocHint;
    i = 1;
    best = NULL;
    vnum = vnum0 + 1;
    if ((allocHint & 0x30) == 0) {
        sel = 0x30;
    }
    do {
        if ((sel & sceHsynCoreBit[i]) != 0) {
            end = (sceHSynVoice_t *)sceHsynActiveList[i];
            for (v = end->prev; v != end; v = v->prev) {
                if (v->vnum < vnum) {
                    vnum = v->vnum;
                    best = v;
                }
            }
        }
        i--;
    } while (i != -1);
    if (best != NULL) {
        set = best->slotp;
        n = set->nActive;
        if (n != 0) {
            set->nActive = n - 1;
        }
        if (best->unk57 == 0) {
            n = sceHsynActiveCount;
            if (n != 0) {
                sceHsynActiveCount = n - 1;
            }
        } else if (best->unk57 == 1) {
            n = sceHsynRelCount;
            if (n != 0) {
                sceHsynRelCount = n - 1;
            }
        }
        sceHsynVoiceChUnlink(best);
        sceHsynVoiceUnlink(best);
        best->flags = 0;
        best->tag = 0xFF;
        best->unk57 = 0xFF;
        best->unk58 = 0xFF;
    }
    return best;
}
/* ENDF sceHsynVoiceStealByVnum */
/* FUNC sceHsynChannelLimitByPitch */
int sceHsynChannelLimitByPitch(int chan, u8 pitch, u8 min)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int n;
    int i;

    n = 0;
    heads[0] = (sceHSynVoice_t *)(chan + 8);
    heads[1] = (sceHSynVoice_t *)(chan - 8);
    for (i = 0; i < 2; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = v->chPrev) {
            if (v->pitch == pitch) {
                n++;
            }
        }
    }
    if (min >= n) {
        return 0;
    }
    for (i = 0; i < 2; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = v->chPrev) {
            if (v->pitch == pitch) {
                v = sceHsynVoiceKeyOffOne(v, i == 1);
            }
        }
    }
    return 1;
}
/* ENDF sceHsynChannelLimitByPitch */
ALIGN_TEXT(4);
/* FUNC sceHsynUnrefParamGetA */
int sceHsynUnrefParamGetA(u8 a, u8 b)
{
    return ((a - b) << 16) / 126;
}
/* ENDF sceHsynUnrefParamGetA */
/* FUNC sceHsynUnrefParamGetB */
int sceHsynUnrefParamGetB(u8 a, u8 b)
{
    return ((b - a) << 16) / 126;
}
/* ENDF sceHsynUnrefParamGetB */
/* FUNC sceHsynUnrefParamSetA */
int sceHsynUnrefParamSetA(u8 a, u8 b)
{
    int x;
    int y;

    x = ((a - 1) << 15) / 126;
    x = x * x;
    y = ((b - 1) << 15) / 126;
    y = y * y;
    return (x - y) / 16384;
}
/* ENDF sceHsynUnrefParamSetA */
/* FUNC sceHsynUnrefParamSetB */
int sceHsynUnrefParamSetB(u8 a, u8 b)
{
    int x;
    int y;

    x = ((a - 1) << 15) / 126;
    x = 0x10000 - x * x / 16384;
    y = ((b - 1) << 15) / 126;
    y = 0x10000 - y * y / 16384;
    return x - y;
}
/* ENDF sceHsynUnrefParamSetB */
/* FUNC sceHsynUnrefParamSetC */
int sceHsynUnrefParamSetC(u8 a, u8 b)
{
    int x;
    int y;

    x = 0x10000 - ((a - 1) << 15) / 126;
    x = 0x10000 - x * x / 16384;
    y = 0x10000 - ((b - 1) << 15) / 126;
    y = 0x10000 - y * y / 16384;
    return x - y;
}
/* ENDF sceHsynUnrefParamSetC */
/* FUNC sceHsynUnrefParamSetD */
int sceHsynUnrefParamSetD(u8 a, u8 b)
{
    int x;
    int y;

    x = 0x10000 - ((a - 1) << 15) / 126;
    x = x * x / 16384;
    y = 0x10000 - ((b - 1) << 15) / 126;
    y = y * y / 16384;
    return x - y;
}
/* ENDF sceHsynUnrefParamSetD */
/* FUNC sceHsynCurveSpanLinear */
int sceHsynCurveSpanLinear(u8 from, u8 to)
{
    int n;

    if (from == to) {
        return 0;
    }
    if (to < from) {
        n = 0x7F - to;
    } else {
        n = to - 1;
    }
    return ((from - to) << 16) / n;
}
/* ENDF sceHsynCurveSpanLinear */
/* FUNC sceHsynCurveSpanLinearInv */
int sceHsynCurveSpanLinearInv(u8 from, u8 to)
{
    int n;

    if (from == to) {
        return 0;
    }
    if (to < from) {
        n = 0x7F - to;
    } else {
        n = to - 1;
    }
    return ((to - from) << 16) / n;
}
/* ENDF sceHsynCurveSpanLinearInv */
/* FUNC sceHsynCurveSpanSquare */
int sceHsynCurveSpanSquare(u8 from, u8 to)
{
    int d;
    int q;

    if (from == to) {
        return 0;
    }
    d = (from - to) << 15;
    if (to < from) {
        q = d / (0x7F - to);
        return (q * q) / 16384;
    }
    q = d / (to - 1);
    return -((q * q) / 16384);
}
/* ENDF sceHsynCurveSpanSquare */
#ifdef NON_MATCHING
/* Same size: the two `0x8000 - q` subtractions land in q's register
 * rather than the constant's. */
/* FUNC sceHsynCurveSpanSquareInv */
int sceHsynCurveSpanSquareInv(u8 from, u8 to)
{
    int d;
    int t;
    int q;

    if (from == to) {
        return 0;
    }
    d = (from - to) << 15;
    if (to < from) {
        t = 0x8000 - d / (0x7F - to);
        return 0x10000 - (t * t) / 16384;
    }
    t = d / (to - 1) + 0x8000;
    return (t * t) / 16384 - 0x10000;
}
/* ENDF sceHsynCurveSpanSquareInv */
#else
INCLUDE_ASM("MODHSYN", sceHsynCurveSpanSquareInv);
#endif
#ifdef NON_MATCHING
/* Same size and instructions: gcc coalesces `curve` into the dying `sel`
 * register where retail gives it one of its own. */
/* FUNC sceHsynVolumeCurveLookup */
int sceHsynVolumeCurveLookup(u8 a, u8 b, u8 sel, u8 *tbl, int nTbl)
{
    u32 curve;
    int idx;

    idx = sel >> 4;
    if (a == 0) {
        a = 1;
    }
    idx--;
    if (idx != -1 && idx < nTbl) {
        tbl += idx * 126;
        a = tbl[a - 1];
        b = tbl[b - 1];
    }
    curve = sel & 0xF;
    if (curve >= 11) {
        curve = 0;
    }
    return sceHsynCurveEval2[curve](a, b);
}
/* ENDF sceHsynVolumeCurveLookup */
#else
INCLUDE_ASM("MODHSYN", sceHsynVolumeCurveLookup);
#endif
/* FUNC sceHsynUnrefStubA */
u8 sceHsynUnrefStubA(s32 v)
{
    return v;
}
/* ENDF sceHsynUnrefStubA */
/* FUNC sceHsynUnrefStubB */
u8 sceHsynUnrefStubB(u8 v)
{
    return 0x80 - v;
}
/* ENDF sceHsynUnrefStubB */
/* FUNC sceHsynTableEntryFill */
int sceHsynTableEntryFill(u8 x)
{
    u32 v;
    int r;

    r = 1;
    v = x;
    v = v * v / 127;
    if (v != 0) {
        r = v & 0xFF;
    }
    return r;
}
/* ENDF sceHsynTableEntryFill */
/* FUNC sceHsynUnrefTableSetA */
u8 sceHsynUnrefTableSetA(u8 x)
{
    u32 r;

    r = sceHsynTableEntryFill(x);
    return 0x80 - r;
}
/* ENDF sceHsynUnrefTableSetA */
/* FUNC sceHsynUnrefTableSetB */
u8 sceHsynUnrefTableSetB(u8 x)
{
    int c;

    c = 0x80;
    return c - sceHsynTableEntryFill(c - x);
}
/* ENDF sceHsynUnrefTableSetB */
/* FUNC sceHsynUnrefTableSetC */
u8 sceHsynUnrefTableSetC(u8 x)
{
    return sceHsynTableEntryFill(0x80 - x);
}
/* ENDF sceHsynUnrefTableSetC */
#ifdef NON_MATCHING
/* Five registers apart: gcc keeps the curve index in the sel register where
 * retail uses a fresh one. */
/* FUNC sceHsynVolumeScale */
u8 sceHsynVolumeScale(u8 val, u8 sel, u8 *tbl, int n)
{
    int idx;
    u32 curve;

    idx = sel >> 4;
    if (val == 0) {
        val = 1;
    }
    /* Separate from the shift: folded into one expression gcc rewrites the
     * -1 test as a test of sel >> 4 against zero. */
    idx--;
    if (idx != -1 && idx < n) {
        tbl += idx * 126;
        val = tbl[val - 1];
    }
    curve = sel & 0xF;
    if (curve >= 11) {
        curve = 0;
    }
    return sceHsynCurveEval1[curve](val);
}
/* ENDF sceHsynVolumeScale */
#else
INCLUDE_ASM("MODHSYN", sceHsynVolumeScale);
#endif
/* FUNC sceHsynLfoSawUp */
int sceHsynLfoSawUp(sceHSynEnv_t *env, int wrapped)
{
    return (s16)((env->phase - 0x8000) >> 1);
}
/* ENDF sceHsynLfoSawUp */
/* FUNC sceHsynLfoSawDown */
int sceHsynLfoSawDown(sceHSynEnv_t *env, int wrapped)
{
    u32 t;

    t = 0xFFFF - env->phase;
    return (s16)((t - 0x8000) >> 1);
}
/* ENDF sceHsynLfoSawDown */
/* FUNC sceHsynLfoTriangle */
int sceHsynLfoTriangle(sceHSynEnv_t *env, int wrapped)
{
    int v;

    v = env->phase;
    if (v > 0xBFFF) {
        v -= 0x10000;
    } else if (v >= 0x4001) {
        v = 0x8000 - v;
    }
    return (s16)v;
}
/* ENDF sceHsynLfoTriangle */
/* FUNC sceHsynLfoSquare */
int sceHsynLfoSquare(sceHSynEnv_t *env, int wrapped)
{
    int v;

    v = -0x4000;
    if ((int)env->phase > 0x7FFF) {
        v = 0x4000;
    }
    return v;
}
/* ENDF sceHsynLfoSquare */
/* FUNC sceHsynLfoSine */
int sceHsynLfoSine(sceHSynEnv_t *env, int wrapped)
{
    u32 *mask;
    u32 n;
    u32 prod;
    u32 i;
    s16 *t;
    int a;
    int b;

    n = env->nTbl;
    prod = env->phase * n;
    t = env->tbl;
    i = prod >> 16;
    a = t[i];
    i++;
    if (i >= n) {
        i = 0;
    }
    b = t[i];
    a += ((b - a) * (int)(prod & 0xFFFF)) >> 16;
    return (s16)(a / 2);
}
/* ENDF sceHsynLfoSine */
/* FUNC sceHsynLfoNoise */
int sceHsynLfoNoise(sceHSynEnv_t *env, int wrapped)
{
    int v;

    if (wrapped != 0) {
        v = (s16)sceHsynRand() >> 2;
    } else {
        v = env->level;
    }
    return v;
}
/* ENDF sceHsynLfoNoise */
#ifdef NON_MATCHING
/* The switch on the state byte compiles to a jump table, and the assembly
 * .rodata already holds its case labels, so a C body here will not link. */
/* FUNC sceHsynLfoStep */
int sceHsynLfoStep(sceHSynEnv_t *env)
{
    u32 ph;
    int state;
    int lvl;
    int wrapped;
    int n;
    int t;

    wrapped = 0;
    lvl = 0;
    state = env->unk00;
    if (state == 0 || state >= 6) {
        goto out;
    }
    if (state >= 2) {
        ph = env->phase + env->rate;
        if ((int)ph > 0xFFFF) {
            ph -= 0x10000;
            wrapped = 1;
        }
        lvl = env->wave(env, wrapped);
        env->phase = ph;
    }
    switch (state) {
    case 1:
        env->unk04--;
        if (env->unk04 != 0) {
            break;
        }
        if (env->unk0C != 0) {
            env->unk08 = 0;
            env->unk00 = 2;
        } else {
            env->unk00 = 3;
        }
        break;
    case 2:
        n = env->unk08;
        lvl = ((s16)lvl * n) / 0x10000;
        t = n + env->unk0C;
        env->unk08 = t;
        if (t > 0xFFFF) {
            env->unk00 = 3;
        }
        break;
    case 3:
        break;
    case 4:
        env->unk04--;
        if (env->unk04 != 0) {
            break;
        }
        if (env->unk0C != 0) {
            env->unk08 = 0x10000;
            env->unk00 = 5;
        } else {
            env->unk00 = 6;
        }
        break;
    case 5:
        n = env->unk08;
        lvl = ((s16)lvl * n) / 0x10000;
        t = n - env->unk0C;
        env->unk08 = t;
        if (t <= 0) {
            env->unk00 = 6;
        }
        break;
    }
out:
    env->level = lvl;
    return (s16)lvl;
}
/* ENDF sceHsynLfoStep */
#else
INCLUDE_ASM("MODHSYN", sceHsynLfoStep);
#endif
/* FUNC sceHsynLfoStart */
void sceHsynLfoStart(sceHSynEnv_t *lfo)
{
    if (lfo->unk02 != 0) {
        lfo->unk00 = 1;
        lfo->unk04 = lfo->unk02;
    } else if (lfo->unk0C != 0) {
        lfo->unk00 = 2;
        lfo->unk08 = 0;
    } else {
        lfo->unk00 = 3;
    }
}
/* ENDF sceHsynLfoStart */
#ifdef NON_MATCHING
/* The switch on the mode byte compiles to a jump table, and the assembly
 * .rodata already holds its case labels, so a C body here will not link. */
/* FUNC sceHsynLfoSetupA */
void sceHsynLfoSetupA(sceHSynEnv_t *env, int prog, u8 *seg, int n, char sel, u16 period, u16 depth, unsigned int tick, int which)
{
    u8 *b;
    int i;
    int k;
    int w;
    int step;
    int d;
    int ph;

    b = (u8 *)prog;
    env->unk01 = 0;
    env->unk00 = 0;
    env->level = 0;
    if ((sel & 0x77) == 0) {
        return;
    }
    env->tbl = NULL;
    if (which != 0) {
        w = b[0xF];
    } else {
        w = b[0xE];
    }
    k = w & 0x80;
    if (k == 0) {
        switch (w & 0xFF) {
        case 1:
            env->wave = sceHsynLfoSawUp;
            break;
        case 2:
            env->wave = sceHsynLfoSawDown;
            break;
        case 3:
            env->wave = sceHsynLfoTriangle;
            break;
        case 4:
            env->wave = sceHsynLfoSquare;
            break;
        case 5:
            env->wave = sceHsynLfoNoise;
            break;
        case 6:
            env->tbl = sceHsynSineTable;
            env->nTbl = 200;
            env->wave = sceHsynLfoSine;
            break;
        default:
            return;
        }
    } else {
        for (i = 0; i < n; i++, seg += 8) {
            if (seg[0] == k) {
                env->tbl = *(s16 **)(seg + 4);
                env->nTbl = *(u16 *)(seg + 2);
                env->wave = sceHsynLfoSine;
                break;
            }
        }
        if (env->tbl == NULL && env->nTbl == 0) {
            return;
        }
    }
    env->unk02 = ((period << 8) + tick / 2) / tick;
    d = ((depth << 8) + tick / 2) / tick;
    if (d != 0) {
        d = 0x10000 / d;
        if (d == 0) {
            d = 1;
        }
    }
    env->unk0C = d;
    if (which != 0) {
        env->phase = b[0x13] << 8;
        ph = (int)(env->phase * (s16)sceHsynRand()) / 0x10000;
        env->phase = ph;
        ph = ph + (b[0x11] << 8);
        env->phase = ph;
        step = *(u16 *)(b + 0x16) << 8;
    } else {
        env->phase = b[0x12] << 8;
        ph = (int)(env->phase * (s16)sceHsynRand()) / 0x10000;
        env->phase = ph;
        ph = ph + (b[0x10] << 8);
        env->phase = ph;
        step = *(u16 *)(b + 0x14) << 8;
    }
    ph = env->phase;
    if (ph < 0) {
        env->phase = ph + 0x10000;
    } else if (ph > 0xFFFF) {
        env->phase = ph - 0x10000;
    }
    env->rate = 0x10000 / ((step + tick / 2) / tick);
    if ((sel & 0x66) != 0) {
        env->unk01 = 1;
    }
    if ((sel & 0x55) != 0) {
        sceHsynLfoStart(env);
    }
}
/* ENDF sceHsynLfoSetupA */
#else
INCLUDE_ASM("MODHSYN", sceHsynLfoSetupA);
#endif
/* FUNC sceHsynLfoKeyOff */
void sceHsynLfoKeyOff(sceHSynEnv_t *env)
{
    if (env->unk01 == 0) {
        return;
    }
    switch (env->unk00) {
    case 0:
        sceHsynLfoStart(env);
        break;
    case 1:
        env->unk00 = 6;
        break;
    case 2:
    case 3:
        if (env->unk02 != 0) {
            env->unk00 = 4;
            env->unk04 = env->unk02;
        } else if (env->unk0C != 0) {
            env->unk00 = 5;
            env->unk08 = 0x10000;
        } else {
            env->unk00 = 6;
        }
        break;
    default:
        env->unk00 = 6;
        break;
    }
}
/* ENDF sceHsynLfoKeyOff */
#ifdef NON_MATCHING
/* The switch on the mode byte compiles to a jump table, and the assembly
 * .rodata already holds its case labels, so a C body here will not link. */
/* FUNC sceHsynLfoSetupB */
void sceHsynLfoSetupB(sceHSynEnv_t *env, int prog, u8 *seg, int n, char sel, u16 period, u16 depth, unsigned int tick, int which)
{
    u8 *b;
    int i;
    int k;
    int w;
    int step;
    int d;
    int ph;

    b = (u8 *)prog;
    env->unk01 = 0;
    env->unk00 = 0;
    env->level = 0;
    if ((sel & 0x77) == 0) {
        return;
    }
    env->tbl = NULL;
    if (which != 0) {
        w = b[0x13];
    } else {
        w = b[0x12];
    }
    k = w & 0x80;
    if (k == 0) {
        switch (w & 0xFF) {
        case 1:
            env->wave = sceHsynLfoSawUp;
            break;
        case 2:
            env->wave = sceHsynLfoSawDown;
            break;
        case 3:
            env->wave = sceHsynLfoTriangle;
            break;
        case 4:
            env->wave = sceHsynLfoSquare;
            break;
        case 5:
            env->wave = sceHsynLfoNoise;
            break;
        case 6:
            env->tbl = sceHsynSineTable;
            env->nTbl = 200;
            env->wave = sceHsynLfoSine;
            break;
        default:
            return;
        }
    } else {
        for (i = 0; i < n; i++, seg += 8) {
            if (seg[0] == k) {
                env->tbl = *(s16 **)(seg + 4);
                env->nTbl = *(u16 *)(seg + 2);
                env->wave = sceHsynLfoSine;
                break;
            }
        }
        if (env->tbl == NULL && env->nTbl == 0) {
            return;
        }
    }
    env->unk02 = ((period << 8) + tick / 2) / tick;
    d = ((depth << 8) + tick / 2) / tick;
    if (d != 0) {
        d = 0x10000 / d;
        if (d == 0) {
            d = 1;
        }
    }
    env->unk0C = d;
    if (which != 0) {
        env->phase = b[0x17] << 8;
        ph = (int)(env->phase * (s16)sceHsynRand()) / 0x10000;
        env->phase = ph;
        ph = ph + (b[0x15] << 8);
        env->phase = ph;
        step = *(u16 *)(b + 0x1A) << 8;
    } else {
        env->phase = b[0x16] << 8;
        ph = (int)(env->phase * (s16)sceHsynRand()) / 0x10000;
        env->phase = ph;
        ph = ph + (b[0x14] << 8);
        env->phase = ph;
        step = *(u16 *)(b + 0x18) << 8;
    }
    ph = env->phase;
    if (ph < 0) {
        env->phase = ph + 0x10000;
    } else if (ph > 0xFFFF) {
        env->phase = ph - 0x10000;
    }
    env->rate = 0x10000 / ((step + tick / 2) / tick);
    if ((sel & 0x66) != 0) {
        env->unk01 = 1;
    }
    if ((sel & 0x55) != 0) {
        sceHsynLfoStart(env);
    }
}
/* ENDF sceHsynLfoSetupB */
#else
INCLUDE_ASM("MODHSYN", sceHsynLfoSetupB);
#endif
ALIGN_TEXT(4);
/* FUNC sceHsynRand */
int sceHsynRand()
{
    int seed;

    seed = sceHsynRandSeed * 0x41C64E6D + 0x3039;
    sceHsynRandSeed = seed;
    return seed >> 16;
}
/* ENDF sceHsynRand */
ALIGN_TEXT(4);
#ifdef NON_MATCHING
/* One nop long: gas will not move the second half of the `sw w, sym` macro
 * into the branch delay slot, where the retail assembler put it. */
/* FUNC modhsyn_e16 */
int modhsyn_e16(sceHSynSdLog_t *w)
{
    sceHsynSdWorkArea = w;
    if (w != NULL) {
        w->index = 0;
        w->unk04 = 0;
    }
    return 0;
}
/* ENDF modhsyn_e16 */
#else
INCLUDE_ASM("MODHSYN", modhsyn_e16);
#endif
/* FUNC sceHsynSdLogNext */
/* The work area is a ring of 28-byte call records; +0 holds the opcode with
 * bit 31 set while the call is outstanding, +4 its result. */
sceHSynSdLogEnt_t *sceHsynSdLogNext()
{
    sceHSynSdLog_t *w;
    u32 i;
    sceHSynSdLogEnt_t *e;

    w = sceHsynSdWorkArea;
    if (w == NULL) {
        return NULL;
    }
    i = w->index;
    e = &w->entry[i];
    w->index = i + 1;
    if (i + 1 >= w->size) {
        w->index = 0;
    }
    return e;
}
/* ENDF sceHsynSdLogNext */
/* FUNC sceHsynSdLogCall */
void sceHsynSdLogCall(sceHSynSdLogEnt_t *e, int op, u32 a0, u32 a1, u32 a2, u32 a3, u32 a4)
{
    if (e != NULL) {
        e->op = op | 0x80000000;
        e->arg[0] = a0;
        e->arg[1] = a1;
        e->arg[2] = a2;
        e->arg[3] = a3;
        e->arg[4] = a4;
    }
}
/* ENDF sceHsynSdLogCall */
/* FUNC sceHsynSdLogResult */
void sceHsynSdLogResult(sceHSynSdLogEnt_t *e, int result)
{
    if (e != NULL) {
        e->result = result;
        e->op &= 0x7FFFFFFF;
    }
}
/* ENDF sceHsynSdLogResult */
/* FUNC sceHsynSdInit */
int sceHsynSdInit(u32 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 0, flag, 0, 0, 0, 0);
    ret = libsd_sceSdInit(flag);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdInit */
/* FUNC sceHsynSdSetParam */
int sceHsynSdSetParam(u16 entry, u16 value)
{
    sceHSynSdLogEnt_t *e;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 1, entry, value, 0, 0, 0);
    libsd_sceSdSetParam(entry, value);
    sceHsynSdLogResult(e, 0);
}
/* ENDF sceHsynSdSetParam */
/* FUNC sceHsynSdGetParam */
int sceHsynSdGetParam(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 2, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetParam(entry);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdGetParam */
/* FUNC sceHsynSdSetSwitch */
int sceHsynSdSetSwitch(u16 entry, u32 value)
{
    sceHSynSdLogEnt_t *e;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 3, entry, value, 0, 0, 0);
    libsd_sceSdSetSwitch(entry, value);
    sceHsynSdLogResult(e, 0);
}
/* ENDF sceHsynSdSetSwitch */
/* FUNC sceHsynSdGetSwitch */
u32 sceHsynSdGetSwitch(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u32 ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 4, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetSwitch(entry);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdGetSwitch */
/* FUNC sceHsynSdSetAddr */
int sceHsynSdSetAddr(u16 entry, u32 value)
{
    sceHSynSdLogEnt_t *e;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 5, entry, value, 0, 0, 0);
    libsd_sceSdSetAddr(entry, value);
    sceHsynSdLogResult(e, 0);
}
/* ENDF sceHsynSdSetAddr */
/* FUNC sceHsynSdGetAddr */
u32 sceHsynSdGetAddr(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u32 ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 6, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetAddr(entry);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdGetAddr */
/* FUNC sceHsynSdSetCoreAttr */
int sceHsynSdSetCoreAttr(u16 entry, u16 value)
{
    sceHSynSdLogEnt_t *e;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 7, entry, value, 0, 0, 0);
    libsd_sceSdSetCoreAttr(entry, value);
    sceHsynSdLogResult(e, 0);
}
/* ENDF sceHsynSdSetCoreAttr */
/* FUNC sceHsynSdNote2Pitch */
int sceHsynSdNote2Pitch(u16 center, u16 fine, u16 note, s16 bend)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 9, center, fine, note, bend, 0);
    ret = libsd_sceSdNote2Pitch(center, fine, note, bend);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdNote2Pitch */
/* FUNC sceHsynSdPitch2Note */
int sceHsynSdPitch2Note(u16 center, u16 fine, u16 pitch)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 10, center, fine, pitch, 0, 0);
    ret = libsd_sceSdPitch2Note(center, fine, pitch);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdPitch2Note */
/* FUNC sceHsynSdProcBatch */
int sceHsynSdProcBatch(u32 *cmds, u32 *rets, u32 count)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 11, (u32)cmds, (u32)rets, count, 0, 0);
    ret = libsd_sceSdProcBatch(cmds, rets, count);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdProcBatch */
/* FUNC sceHsynSdProcBatchEx */
int sceHsynSdProcBatchEx(u32 *cmds, u32 *rets, u32 count, u32 flags)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 12, (u32)cmds, (u32)rets, count, flags, 0);
    ret = libsd_sceSdProcBatchEx(cmds, rets, count, flags);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdProcBatchEx */
/* FUNC sceHsynSdVoiceTrans */
int sceHsynSdVoiceTrans(s16 chan, u16 mode, u8 *iopAddr, u32 spuAddr, u32 size)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 13, chan, mode, (u32)iopAddr, spuAddr, size);
    ret = libsd_sceSdVoiceTrans(chan, mode, iopAddr, spuAddr, size);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdVoiceTrans */
/* FUNC sceHsynSdBlockTrans */
int sceHsynSdBlockTrans(s16 chan, u16 mode, u8 *iopAddr, u32 size)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 14, chan, mode, (u32)iopAddr, size, 0);
    ret = libsd_sceSdBlockTrans(chan, mode, iopAddr, size);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdBlockTrans */
/* FUNC sceHsynSdVoiceTransStatus */
int sceHsynSdVoiceTransStatus(s16 chan, s16 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 15, chan, flag, 0, 0, 0);
    ret = libsd_sceSdVoiceTransStatus(chan, flag);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdVoiceTransStatus */
/* FUNC sceHsynSdBlockTransStatus */
int sceHsynSdBlockTransStatus(s16 chan, s16 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 16, chan, flag, 0, 0, 0);
    ret = libsd_sceSdBlockTransStatus(chan, flag);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdBlockTransStatus */
/* FUNC sceHsynSdSetEffectAttr */
int sceHsynSdSetEffectAttr(int core, sceSdEffectAttr_t *attr)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 19, core, (u32)attr, 0, 0, 0);
    ret = libsd_sceSdSetEffectAttr(core, attr);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdSetEffectAttr */
/* FUNC sceHsynSdGetEffectAttr */
void sceHsynSdGetEffectAttr(int core, sceSdEffectAttr_t *attr)
{
    sceHSynSdLogEnt_t *e;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 20, core, (u32)attr, 0, 0, 0);
    libsd_sceSdGetEffectAttr(core, attr);
    sceHsynSdLogResult(e, 0);
}
/* ENDF sceHsynSdGetEffectAttr */
/* FUNC sceHsynSdClearEffectWorkArea */
int sceHsynSdClearEffectWorkArea(int core, int channel, int effectMode)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = sceHsynSdLogNext();
    sceHsynSdLogCall(e, 21, core, channel, effectMode, 0, 0);
    ret = libsd_sceSdClearEffectWorkArea(core, channel, effectMode);
    sceHsynSdLogResult(e, ret);
    return ret;
}
/* ENDF sceHsynSdClearEffectWorkArea */
ALIGN_TEXT(4);
#ifdef NON_MATCHING
/* One instruction short: retail copies the counter before incrementing it,
 * gcc folds the copy into the add. */
/* FUNC sceHsynEffectDepthSet */
int sceHsynEffectDepthSet(sceHSynLfo_t *lfo)
{
    u32 *mask;
    u32 n;

    if (lfo->unk00 == 1) {
        n = lfo->unk10;
        if (n < lfo->unk0C) {
            lfo->unk10 = n + 1;
            lfo->unk02 = lfo->unk04 + (lfo->unk08 * n >> 8);
        } else {
            lfo->unk00 = 2;
            lfo->unk02 = lfo->unk04 + lfo->unk06;
            lfo->unk10 = lfo->unk10 + 1;
        }
    }
    return 0;
}
/* ENDF sceHsynEffectDepthSet */
#else
INCLUDE_ASM("MODHSYN", sceHsynEffectDepthSet);
#endif
/* FUNC sceHsynCoreStateGet */
int sceHsynCoreStateGet(sceHSynVoice_t *v)
{
    v->lfoA.unk00 = 0;
    v->lfoA.unk04 = 0;
    v->lfoA.unk02 = 0;
    v->lfoA.unk06 = 0;
    v->lfoA.unk08 = 0;
    v->lfoA.unk0C = 0;
    v->lfoA.unk10 = 0;
    return 0;
}
/* ENDF sceHsynCoreStateGet */
/* FUNC sceHsynPitchBendApply */
int sceHsynPitchBendApply(sceHSynVoice_t *v, int rate, u16 depth, int dir)
{
    int period;
    int step;
    int d;
    u16 e;

    period = (rate << 8) / sceHsynTickScale;
    d = (depth << 7) / 100;
    period--;
    e = d;
    if (period <= 0) {
        period = 1;
    }
    step = (e << 8) / period;
    if (dir == 0xF) {
        step = -step;
        e = -d;
    }
    v->lfoA.unk08 = step;
    v->lfoA.unk04 = v->lfoA.unk02;
    v->lfoA.unk10 = 1;
    v->lfoA.unk00 = 1;
    v->lfoA.unk06 = e;
    v->lfoA.unk0C = period;
    return 0;
}
/* ENDF sceHsynPitchBendApply */
ALIGN_TEXT(4);
#ifdef NON_MATCHING
/* One instruction short: retail copies the counter before incrementing it,
 * gcc folds the copy into the add. */
/* FUNC sceHsynEffectFeedbackSet */
int sceHsynEffectFeedbackSet(sceHSynVoice_t *voice)
{
    sceHSynLfo_t *lfo;
    u32 *mask;
    u32 n;

    lfo = &voice->lfoB;
    if (lfo->unk00 == 1) {
        n = lfo->unk10;
        if (n < lfo->unk0C) {
            lfo->unk10 = n + 1;
            lfo->unk02 = lfo->unk04 + (lfo->unk08 * n >> 16);
        } else {
            lfo->unk02 = lfo->unk04 + lfo->unk06;
            lfo->unk00 = 2;
            lfo->unk10 = lfo->unk10 + 1;
        }
    }
    return 0;
}
/* ENDF sceHsynEffectFeedbackSet */
#else
INCLUDE_ASM("MODHSYN", sceHsynEffectFeedbackSet);
#endif
/* FUNC sceHsynCoreStateSet */
int sceHsynCoreStateSet(sceHSynVoice_t *v)
{
    v->lfoB.unk00 = 0;
    v->lfoB.unk04 = 0;
    v->lfoB.unk02 = 0;
    v->lfoB.unk06 = 0;
    v->lfoB.unk08 = 0;
    v->lfoB.unk0C = 0;
    v->lfoB.unk10 = 0;
    return 0;
}
/* ENDF sceHsynCoreStateSet */
#ifdef NON_MATCHING
/* One nop long: gcc hoists the lfoB phase load above the volume
 * product's mflo, where retail schedules it after the shift. */
/* FUNC sceHsynVoiceSetVolumeRamp */
int sceHsynVoiceSetVolumeRamp(sceHSynVoice_t *v, int rate, u8 vol)
{
    sceHSynChannel_t *chan;
    sceHSynSlot_t *slot;
    int period;
    u32 prod;
    int level;
    int target;
    int step;

    period = (rate << 8) / sceHsynTickScale;
    chan = v->chan;
    slot = chan->slot;
    period--;
    if (period <= 0) {
        period = 1;
    }
    prod = vol * sceHsynVolumeScale(vol, v->unk4A, slot->unk10, slot->unk0C);
    prod = prod * (chan->vol * chan->expression);
    level = prod >> 14;
    level = (u16)level;
    target = v->unk18 + *(s16 *)&v->lfoB.unk02;
    target = level - target;
    step = (target << 16) / period;
    v->lfoB.unk0C = period;
    v->lfoB.unk04 = v->lfoB.unk02;
    v->lfoB.unk10 = 1;
    v->lfoB.unk00 = 1;
    v->lfoB.unk06 = target;
    v->lfoB.unk08 = step;
    return 0;
}
/* ENDF sceHsynVoiceSetVolumeRamp */
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceSetVolumeRamp);
#endif
ALIGN_TEXT(4);
/* FUNC sceHsynPanSplit */
void sceHsynPanSplit(u32 *out, int wrap, s8 pan)
{
    int x;
    int en;

    en = sceHsynEnabled;
    out[1] = 0x80;
    out[0] = 0x80;
    if (en != 0) {
        return;
    }
    x = pan;
    if (wrap != 0) {
        if (x < -0x80) {
            x += 0xFF;
        } else if (x >= 0x80) {
            x -= 0xFF;
        }
        if (x < 0) {
            x = -x;
        }
    } else {
        if (x >= 0x80) {
            x = 0x7F;
        }
        if (x < -0x80) {
            x = -0x80;
        }
    }
    if (x == 0) {
        out[1] = 0;
        return;
    }
    if (x >= 0x7F) {
        out[0] = 0;
        return;
    }
    if (x < 0x40) {
        out[1] = (x << 1) & 0xFF;
        return;
    }
    if (x > 0x40) {
        out[0] = ((0x80 - x) << 1) & 0xFF;
    }
}
/* ENDF sceHsynPanSplit */
#ifdef NON_MATCHING
/* One instruction short: retail copies the pan value into the shift temp
   before shifting it in place, which no form of the expression reproduces. */
/* FUNC sceHsynVoiceParamRampStep */
int sceHsynVoiceParamRampStep(sceHSynVoice_t *v)
{
    int *p;
    int state;
    int n;
    int x;

    state = *(int *)v->param;
    p = (int *)v->param;
    if (state == 1) {
        if ((u32)p[9] >= (u32)p[6]) {
            n = p[7];
            p[9] = 0;
            if (n != 0) {
                n--;
                p[7] = n;
                if (n == 0) {
                    if (p[8] != 0) {
                        p[7] = 1;
                        p[8] = 0;
                        p[6] = p[10];
                    } else {
                        p[7] = -1;
                        p[9] = p[6];
                    }
                }
            }
            if ((u32)p[9] >= (u32)p[6]) {
                goto stop;
            }
        }
        x = p[3] + p[5] * p[9];
        if (x > 0x7F000) {
            x -= 0xFF000;
        } else if (x < -0x7FFFF) {
            x += 0xFF000;
        }
        p[2] = x;
        x = x / 4096;
        goto done;
stop:
        x = p[4];
        p[0] = 2;
        p[2] = x << 12;
done:
        p[9]++;
        sceHsynPanSplit((u32 *)&p[11], v->unk4E, x);
    }
    return 0;
}
/* ENDF sceHsynVoiceParamRampStep */
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceParamRampStep);
#endif
/* FUNC sceHsynChannelStateGet */
int sceHsynChannelStateGet(sceHSynVoice_t *v)
{
    *(u32 *)&v->param[0] = 0;
    *(u32 *)&v->param[4] = -1;
    *(u32 *)&v->param[8] = 0;
    *(u32 *)&v->param[12] = 0;
    *(u32 *)&v->param[16] = 0;
    *(u32 *)&v->param[20] = 0;
    *(u32 *)&v->param[24] = 0;
    *(u32 *)&v->param[28] = 0;
    *(u32 *)&v->param[32] = 0;
    *(u32 *)&v->param[36] = 0;
    *(u32 *)&v->param[40] = 0;
    *(u32 *)&v->param[44] = 0x80;
    *(u32 *)&v->param[48] = 0x80;
    *(u32 *)&v->param[52] = 1;
    return 0;
}
/* ENDF sceHsynChannelStateGet */
#ifdef NON_MATCHING
/* The switch on the parameter selector compiles to a jump table, and the
 * assembly .rodata already holds its case labels, so a C body here will not
 * link. */
/* FUNC sceHsynVoiceSetParamRamp */
int sceHsynVoiceSetParamRamp(sceHSynVoice_t *v, u32 a3, int a4, u8 a8, s8 a9, int a10)
{
    int *p;
    int rate;
    int step;
    int apply;
    int cur;
    int tgt;
    int base;
    int from;
    int to;
    int d;
    int lim;
    int t;

    step = 1;
    rate = 1;
    apply = 1;
    p = (int *)v->param;
    if (a3 >= 14) {
        return -1;
    }
    base = (v->unk5A + 0x40) << 12;
    cur = (u8)v->unk5A + 0x40;
    if ((u32)(p[0] - 1) < 2) {
        base = p[2];
        cur = (int)((u32)(base < 0 ? base + 0xFFF : base) >> 12);
    }
    tgt = cur;
    if (a4 != 0) {
        step = (a4 << 8) / sceHsynTickScale;
        rate = 0xFF000 / step;
    }
    switch (a3) {
    case 1:
        rate = -rate;
        /* fall through */
    case 0:
        p[7] = a8;
        p[8] = 0;
        break;
    case 2:
        apply = 0;
        p[7] = 1;
        p[8] = 0;
        p[4] = v->unk5A + 0x40;
        break;
    case 3:
        p[7] = a8;
        tgt = (u8)a9;
        from = (s8)cur;
        to = (s8)tgt;
        if (from < 0) {
            from += 0x100;
        }
        if (to < 0) {
            to += 0x100;
        }
        d = to - from;
        if (d < 0) {
            d += 0x100;
        } else if (d == 0) {
            d = 0x100;
        }
        t = (d << 12) / rate;
        p[8] = 1;
        p[10] = t;
        break;
    case 4:
        p[7] = a8;
        tgt = (u8)a9;
        from = (s8)cur;
        to = (s8)tgt;
        if (from < 0) {
            from += 0x100;
        }
        if (to < 0) {
            to += 0x100;
        }
        d = to - from;
        if (d < 0) {
            d += 0x100;
        } else if (d == 0) {
            d = 0x100;
        }
        d = 0x100 - d;
        t = (d << 12) / rate;
        rate = -rate;
        p[8] = 1;
        p[10] = t;
        break;
    case 12:
        p[7] = 1;
        tgt = (u8)a9;
        from = (s8)cur;
        to = (s8)tgt;
        if (from < 0) {
            from += 0x100;
        }
        if (to < 0) {
            to += 0x100;
        }
        d = to - from;
        if (d < 0) {
            d += 0x100;
        } else if (d == 0) {
            d = 0x100;
        }
        rate = (d << 12) / step;
        p[10] = step;
        p[8] = 0;
        break;
    case 13:
        p[7] = 1;
        tgt = (u8)a9;
        from = (s8)cur;
        to = (s8)tgt;
        if (from < 0) {
            from += 0x100;
        }
        if (to < 0) {
            to += 0x100;
        }
        d = to - from;
        if (d < 0) {
            d += 0x100;
        }
        d = 0x100 - d;
        if (d == 0) {
            d = 0x100;
        }
        rate = -((d << 12) / step);
        p[10] = step;
        p[8] = 0;
        break;
    case 5:
        apply = 0;
        from = (s8)v->unk5A + 0x40;
        if (from < 0) {
            from = (s8)v->unk5A + 0x140;
        }
        to = (s8)a9;
        if (to < 0) {
            to += 0x100;
        }
        d = to - from;
        if (d < 0) {
            d += 0x100;
        } else if (d == 0) {
            d = 0x100;
        }
        lim = p[1];
        rate = p[5];
        if ((lim & 2) != 0) {
            d = 0x100 - d;
            if (d == 0) {
                d = 0x100;
            }
            rate = -rate;
        }
        t = (d << 12) / rate;
        p[4] = (s8)a9;
        if ((u32)p[10] >= (u32)t) {
            p[10] = t;
        } else {
            p[6] = t;
            p[8] = 0;
        }
        p[7] = 1;
        break;
    default:
        return -1;
    }
    if (apply == 0) {
        return 0;
    }
    p[0] = 1;
    p[3] = base;
    p[4] = (s8)tgt;
    p[5] = rate;
    p[6] = step;
    p[9] = 0;
    p[1] = sceHsynSdParamTable[a3];
    p[11] = v->unk4C;
    p[12] = v->unk4D;
    p[13] = (s8)v->unk53;
    return 0;
}
/* ENDF sceHsynVoiceSetParamRamp */
#else
INCLUDE_ASM("MODHSYN", sceHsynVoiceSetParamRamp);
#endif
INCLUDE_ASM("MODHSYN", hsynLibraryTable);
/* FUNC sceHsynRetnStub */
void sceHsynRetnStub(void)
{
}
/* ENDF sceHsynRetnStub */
ALIGN_TEXT(4);
IRX_IMPORT_TABLE(libsd, 1, 5);
IRX_IMPORT(libsd, sceSdInit, 0x4);
IRX_IMPORT(libsd, sceSdSetParam, 0x5);
IRX_IMPORT(libsd, sceSdGetParam, 0x6);
IRX_IMPORT(libsd, sceSdSetSwitch, 0x7);
IRX_IMPORT(libsd, sceSdGetSwitch, 0x8);
IRX_IMPORT(libsd, sceSdSetAddr, 0x9);
IRX_IMPORT(libsd, sceSdGetAddr, 0xa);
IRX_IMPORT(libsd, sceSdSetCoreAttr, 0xb);
IRX_IMPORT(libsd, sceSdNote2Pitch, 0xd);
IRX_IMPORT(libsd, sceSdPitch2Note, 0xe);
IRX_IMPORT(libsd, sceSdProcBatch, 0xf);
IRX_IMPORT(libsd, sceSdProcBatchEx, 0x10);
IRX_IMPORT(libsd, sceSdVoiceTrans, 0x11);
IRX_IMPORT(libsd, sceSdBlockTrans, 0x12);
IRX_IMPORT(libsd, sceSdVoiceTransStatus, 0x13);
IRX_IMPORT(libsd, sceSdBlockTransStatus, 0x14);
IRX_IMPORT(libsd, sceSdSetEffectAttr, 0x17);
IRX_IMPORT(libsd, sceSdGetEffectAttr, 0x18);
IRX_IMPORT(libsd, sceSdClearEffectWorkArea, 0x19);
IRX_IMPORT_TABLE_END;
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
IRX_IMPORT_TABLE(thbase, 1, 2);
IRX_IMPORT(thbase, GetSystemTime, 0x22);
IRX_IMPORT(thbase, SysClock2USec, 0x28);
IRX_IMPORT_TABLE_END;
ALIGN_TEXT(4);
