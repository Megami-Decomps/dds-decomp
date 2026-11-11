#include "common.h"
#include "libsdf.h"

/* Declared here because no header carries these. */
extern void *memcpy(void *a, const void *b, size_t c);
extern void *memset(void *a, s32 b, size_t c);

extern void *sdfChipAllocDirect(s32 aSize);
extern void *sdfChipAllocZeroDirect(s32 aSize);
extern void sdfChipFreeDirect(void *aPtr);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
extern sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 aSize);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr);

extern sdfTexHandle_t *sdfTexSet(void *aTex);
extern sdfTexHandle_t *sdfTexSetDirect(void *aTex);
extern sdfItemArray_t *sdfTexListNew(s32 aInitCapa);

extern void sdfDecompInit(struct sdfDecomp_t *cb, u8 *dst);
extern s32 sdfDecomp(struct sdfDecomp_t *cb, const u8 *src, s32 length);
extern void sdfAddressResolver(s32 *aDataBody, saddr_t aOffset, u8 *aAddrInfoPtr, s32 aAddrInfoLeng);

s32 sdfPacStdCallback(sdfPacDecodeCb_t *cb, s32 cause, sdfPacStdBlockHeader_t *hdr);
void sdfPacNewBlock(sdfPacDecodeCb_t *cb);
void sdfPacEndBlock(sdfPacDecodeCb_t *cb);
sdfPacDecodeResrc_t *sdfPacNewResrc(sdfPacDecodeCb_t *cb, const sdfPacStdBlockHeader_t *hdr);
void sdfPacMemDecoderFunc(sdfPacDecodeCb_t *cb);
void sdfPacMemDecompDecoderFunc(sdfPacDecodeCb_t *cb);
void sdfPacMemStayDecoderFunc(sdfPacDecodeCb_t *cb);
void sdfPacMemDecoderFuncInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);
void sdfPacMemDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);
void sdfPacModelDecoderEndFunc(sdfPacDecodeCb_t *cb);
void sdfPacModelDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);
void sdfPacMotionDecoderEndFunc(sdfPacDecodeCb_t *cb);
void sdfPacMotionDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);
void sdfPacTmxDecoderFunc(sdfPacDecodeCb_t *cb);
void sdfPacTmxDecompDecoderFunc(sdfPacDecodeCb_t *cb);
void sdfPacTmxStayDecoderFunc(sdfPacDecodeCb_t *cb);
void sdfPacTmxDecoderInitFunc(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr,
                              sdfPacTmxDecodeCb_t *tmxCb);
void sdfPacTmxDecoderEndFunc(sdfPacDecodeCb_t *cb);
void sdfPacTmxDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);
void sdfPacTexListDecoderTmxBodyEndFunc(sdfPacDecodeCb_t *cb);
void sdfPacTexListDecoder1(sdfPacDecodeCb_t *cb);
void sdfPacTexListDecoder0(sdfPacDecodeCb_t *cb);
void sdfPacTexListDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);


void sdfPacMarkPhaseFinished(sdfPacDecodeCb_t *aState);
void sdfPacResetOutputToAllocationEntry(sdfPacDecodeCb_t *aState);
void sdfPacTexListDecoderTmxHeaderEndFunc(sdfPacDecodeCb_t *cb);
extern void sdfTexListAdd(sdfItemArray_t *aList, sdfTexHandle_t *aTexHandle);

void sdfPacInit(sdfPacDecodeCb_t *cb, sdfPacCallbackFunc callback)
{
    memset(cb, 0, sizeof(*cb));
    cb->currBlockNum = -1;
    if (callback != NULL) {
        cb->callback = callback;
    } else {
        cb->callback = sdfPacStdCallback;
    }
}

void sdfPacSetStay(sdfPacDecodeCb_t *cb)
{
    cb->flag |= SDF_PAC_STATE_STAY;
}

void sdfPacSetFromTail(sdfPacDecodeCb_t *cb)
{
    cb->flag |= SDF_PAC_STATE_FROMTAIL;
}

void sdfPacFree(sdfPacDecodeCb_t *cb)
{
    sdfPacDecodeResrc_t *cursor = cb->resrcHead;
    sdfPacDecodeResrc_t *current = cursor;

    while (cursor != NULL) {
        cursor = cursor->next;
        sdfChipFreeDirect(current);
        current = cursor;
    }
}

void sdfPacMarkPhaseFinished(sdfPacDecodeCb_t *aState)
{
    aState->stat = -1;
}

void sdfPacUseSrc(sdfPacDecodeCb_t *cb, s32 size)
{
    cb->srcData += size;
    cb->srcLength -= size;
    cb->srcTotal += size;
}

void sdfPacNewBlock(sdfPacDecodeCb_t *cb)
{
    sdfPacStdBlockHeader_t *packet;
    u32 alignmentOffset;
    u32 packetBytes;
    u32 headerBytes;
    u32 payloadBytes;
    s32 (*callback)();
    s32 result;

    alignmentOffset = cb->srcTotal & 0x3F;
    if (alignmentOffset != 0) {
        cb->stat = 1;
        cb->rest = 0x40 - alignmentOffset;
        return;
    }
    packet = (sdfPacStdBlockHeader_t *)cb->srcData;
    sdfPacUseSrc(cb, 0x10);
    packetBytes = packet->size;
    cb->currBlockNum++;
    headerBytes = (packet->flags & SDF_PAC_EXTRA_MASK) + 0x10;
    payloadBytes = packetBytes - headerBytes;
    cb->rest = payloadBytes;
    callback = (s32 (*)())cb->callback;
    result = callback(cb, 0, packet, payloadBytes);
    if (result == 1) {
        sdfPacMarkPhaseFinished(cb);
        return;
    }
    if (result == 2) {
        cb->stat = 1;
        cb->rest = (cb->rest + 0x3F) & ~0x3F;
    } else {
        cb->stat = 2;
    }
}

void sdfPacEndBlock(sdfPacDecodeCb_t *cb)
{
    s32 result;
    sdfPacDecodeResrc_t *tail;
    sdfPacDecodeResrc_t *scan;
    sdfPacDecodeResrc_t *next;

    result = cb->callback(cb, 1, (sdfPacStdBlockHeader_t *)cb->resrcTail->header);
    if (result == 1) {
        sdfPacMarkPhaseFinished(cb);
        return;
    }
    if (result == 4) {
        tail = cb->resrcTail;
        scan = cb->resrcHead;
        if (scan == tail) {
            cb->resrcTail = NULL;
            cb->resrcHead = NULL;
        } else {
            while ((next = scan->next) != tail) {
                scan = next;
            }
            /* Retail clears the link but leaves the multi-item tail unchanged. */
            scan->next = NULL;
        }
        sdfChipFreeDirect(tail);
    }
    cb->stat = 0;
}

s32 sdfPacDecode(sdfPacDecodeCb_t *cb, const u8 *data, s32 length)
{
    cb->srcData = data;
    cb->srcLength = length;
    while (cb->srcLength > 0) {
        switch ((s8)cb->stat) {
        case -1:
            return -1;
        case 0:
            sdfPacNewBlock(cb);
            break;
        case 1: {
            s32 len = cb->rest;

            if (cb->srcLength < len) {
                len = cb->srcLength;
            }
            sdfPacUseSrc(cb, len);
            cb->rest -= len;
            if (cb->rest == 0) {
                cb->stat = 0;
            }
            break;
        }
        case 2:
            cb->decoder(cb);
            break;
        }
    }
    return (s8)cb->stat;
}

sdfPacDecodeResrc_t *sdfPacNewResrc(sdfPacDecodeCb_t *cb, const sdfPacStdBlockHeader_t *hdr)
{
    s32 extra = hdr->flags & SDF_PAC_EXTRA_MASK;
    sdfPacDecodeResrc_t *aWork = sdfChipAllocZeroDirect(extra + sizeof(sdfPacDecodeResrc_t));

    aWork->cb = cb;
    memcpy(aWork->header, hdr, extra + sizeof(*hdr));
    if (cb->resrcTail == NULL) {
        cb->resrcHead = aWork;
    } else {
        cb->resrcTail->next = aWork;
    }
    cb->resrcTail = aWork;
    return aWork;
}

sdfPacDecodeResrc_t *sdfPacFreeResrc(sdfPacDecodeResrc_t *resrc)
{
    sdfPacDecodeCb_t *aState = resrc->cb;
    sdfPacDecodeResrc_t **link = &aState->resrcHead;
    sdfPacDecodeResrc_t *current = aState->resrcHead;
    sdfPacDecodeResrc_t *previous = NULL;
    sdfPacDecodeResrc_t *next;

    if (current != resrc) {
        do {
            previous = current;
            current = previous->next;
            link = &previous->next;
        } while (current != resrc);
    }
    next = resrc->next;
    *link = next;
    if (aState->resrcTail == resrc) {
        aState->resrcTail = previous;
    }
    sdfChipFreeDirect(resrc);
    return next;
}

s32 sdfPacStdCallback(sdfPacDecodeCb_t *cb, s32 cause, sdfPacStdBlockHeader_t *hdr)
{
    if (cause == 0) {
        switch (hdr->fileType) {
        case DDS3_FT_EFFECT:
            sdfPacMemDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_TMX:
            sdfPacTmxDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_MODEL:
            sdfPacModelDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_MOTION:
            sdfPacMotionDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_TEXLIST:
            sdfPacTexListDecoderInit(cb, hdr);
            return 0;
        case DDS3_FT_END:
            return 1;
        default:
            return 3;
        }
    }
    return 0;
}

char *sdfPacGetResrcFileName(sdfPacDecodeResrc_t *resrc)
{
    sdfPacStdBlockHeader_t *packet = (sdfPacStdBlockHeader_t *)resrc->header;
    s32 extra = packet->flags & SDF_PAC_EXTRA_MASK;

    if (extra <= 0) {
        return NULL;
    }
    return (char *)(packet + 1);
}

void sdfPacMemDecoderFunc(sdfPacDecodeCb_t *cb)
{
    s32 len = cb->rest;

    if (cb->srcLength < len) {
        len = cb->srcLength;
    }
    if (len != 0) {
        memcpy(cb->dst, cb->srcData, len);
        sdfPacUseSrc(cb, len);
        cb->dst += len;
        cb->rest -= len;
        if (cb->rest == 0) {
            cb->onEnd(cb);
        }
    }
}

void sdfPacMemDecompDecoderFunc(sdfPacDecodeCb_t *cb)
{
    s32 len = cb->srcLength;
    s32 done = sdfDecomp(cb->decomp, cb->srcData, len);

    sdfPacUseSrc(cb, len - cb->decomp->remain);
    if (done != 0) {
        sdfChipFreeDirect(cb->decomp);
        cb->onEnd(cb);
    }
}

void sdfPacMemStayDecoderFunc(sdfPacDecodeCb_t *cb)
{
    s32 len = cb->rest;

    if (cb->srcLength < len) {
        len = cb->srcLength;
    }
    if (len != 0) {
        sdfPacUseSrc(cb, len);
        cb->rest -= len;
        if (cb->rest == 0) {
            cb->onEnd(cb);
        }
    }
}

void sdfPacMemDecoderFuncInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    s32 size = hdr->realSize;
    sdfPacDecodeResrc_t *aWork;

    if (size == 0) {
        size = hdr->size + (hdr->flags & SDF_PAC_EXTRA_MASK) - sizeof(*hdr);
    }
    if (cb->flag & SDF_PAC_STATE_STAY) {
        aWork = sdfPacNewResrc(cb, hdr);
        aWork->data = (u8 *)(hdr + 1);
        cb->decoder = sdfPacMemStayDecoderFunc;
        return;
    }
    cb->stat = 2;
    aWork = sdfPacNewResrc(cb, hdr);
    if (cb->flag & SDF_PAC_STATE_FROMTAIL) {
        aWork->resrc = sdfMemAllocFromTailDirect(size);
    } else {
        aWork->resrc = sdfMemAllocDirect(size);
    }
    aWork->data = sdfMemLock(aWork->resrc);
    cb->dst = aWork->data;
    switch (hdr->flags & SDF_PAC_ENCODING_MASK) {
    case 0:
        cb->decoder = sdfPacMemDecoderFunc;
        break;
    case 1: {
        struct sdfDecomp_t *decomp = sdfChipAllocDirect(sizeof(*decomp));

        cb->decomp = decomp;
        sdfDecompInit(decomp, cb->dst);
        cb->decoder = sdfPacMemDecompDecoderFunc;
        break;
    }
    }
}

void sdfPacMemDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacMemDecoderFuncInit(cb, hdr);
    cb->onEnd = sdfPacEndBlock;
}

void sdfPacModelDecoderEndFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacDecodeResrc_t *aWork = cb->resrcTail;
    sdfPacReloc_t *reloc = (sdfPacReloc_t *)aWork->data;
    u8 *payload = reloc->payload;
    s32 bytes = reloc->tableBytes;

    aWork->data = payload;
    if (bytes != 0) {
        sdfAddressResolver((s32 *)payload, EE_ADDR_S(payload), payload + reloc->tableOffset, bytes);
        reloc->tableBytes = 0;
    }
    sdfPacEndBlock(cb);
}

void sdfPacModelDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacMemDecoderFuncInit(cb, hdr);
    cb->onEnd = sdfPacModelDecoderEndFunc;
}

void sdfPacMotionDecoderEndFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacDecodeResrc_t *aWork = cb->resrcTail;
    sdfPacReloc_t *reloc = (sdfPacReloc_t *)aWork->data;
    u8 *payload = reloc->payload;
    s32 bytes = reloc->tableBytes;

    aWork->data = payload;
    if (bytes != 0) {
        sdfAddressResolver((s32 *)payload, EE_ADDR_S(payload), payload + reloc->tableOffset, bytes);
        reloc->tableBytes = 0;
    }
    sdfPacEndBlock(cb);
}

void sdfPacMotionDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacMemDecoderFuncInit(cb, hdr);
    cb->onEnd = sdfPacMotionDecoderEndFunc;
}

void sdfPacTmxDecoderFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacTmxDecodeCb_t *buf = cb->decodeCb;
    s32 len = buf->rest;

    if (cb->srcLength < len) {
        len = cb->srcLength;
    }
    if (len != 0) {
        memcpy(buf->workPtr, cb->srcData, len);
        sdfPacUseSrc(cb, len);
        buf->workPtr += len;
        buf->rest -= len;
        if (buf->rest == 0) {
            buf->texHandle = sdfTexSet(sdfMemLock(buf->memHandle));
            sdfMemFreeHandleDirect(&buf->memHandle);
            cb->onEnd(cb);
        }
    }
}

void sdfPacTmxDecompDecoderFunc(sdfPacDecodeCb_t *cb)
{
    s32 len = cb->rest;
    s32 done = sdfDecomp(cb->decomp, cb->srcData, len);

    sdfPacUseSrc(cb, len - cb->decomp->remain);
    if (done != 0) {
        sdfPacTmxDecodeCb_t *buf = cb->decodeCb;

        buf->texHandle = sdfTexSet(sdfMemLock(buf->memHandle));
        sdfMemFreeHandleDirect(&buf->memHandle);
        sdfChipFreeDirect(cb->decomp);
        cb->onEnd(cb);
    }
}

void sdfPacTmxStayDecoderFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacTmxDecodeCb_t *buf = cb->decodeCb;
    s32 len = buf->rest;

    if (cb->srcLength < len) {
        len = cb->srcLength;
    }
    if (len != 0) {
        sdfPacUseSrc(cb, len);
        buf->rest -= len;
        if (buf->rest == 0) {
            buf->texHandle = sdfTexSetDirect(buf->workPtr);
            cb->onEnd(cb);
        }
    }
}

void sdfPacTmxDecoderInitFunc(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr,
                             sdfPacTmxDecodeCb_t *tmxCb)
{
    cb->decodeCb = tmxCb;
    tmxCb->rest = hdr->size - 0x10;
    if (cb->flag & SDF_PAC_STATE_STAY) {
        cb->decoder = sdfPacTmxStayDecoderFunc;
        tmxCb->workPtr = (u8 *)(cb->srcData - 0x10);
        return;
    }
    if (cb->flag & SDF_PAC_STATE_FROMTAIL) {
        tmxCb->memHandle = sdfMemAllocFromTailDirect(hdr->size);
    } else {
        tmxCb->memHandle = sdfMemAllocDirect(hdr->size);
    }
    tmxCb->workPtr = sdfMemLock(tmxCb->memHandle);
    memcpy(tmxCb->workPtr, hdr, 0x10);
    tmxCb->workPtr += 0x10;
    switch (hdr->flags) {
    case 0:
        cb->decoder = sdfPacTmxDecoderFunc;
        break;
    case 1: {
        struct sdfDecomp_t *decomp = sdfChipAllocDirect(sizeof(*decomp));

        cb->decompBuffer = (u8 *)decomp;
        sdfDecompInit(decomp, tmxCb->workPtr);
        cb->decoder = sdfPacTmxDecompDecoderFunc;
        break;
    }
    }
}

void sdfPacTmxDecoderEndFunc(sdfPacDecodeCb_t *cb)
{
    cb->resrcTail->resrc = cb->slot.decodeCb->texHandle;
    sdfChipFreeDirect(cb->slot.decodeCb);
    sdfPacEndBlock(cb);
}

void sdfPacTmxDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacTmxDecodeCb_t *buf;

    sdfPacNewResrc(cb, hdr);
    buf = sdfChipAllocDirect(sizeof(*buf));
    cb->slot.decodeCb = buf;
    sdfPacTmxDecoderInitFunc(cb, hdr, buf);
    cb->onEnd = sdfPacTmxDecoderEndFunc;
}

void sdfPacTexListDecoderTmxHeaderEndFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacTmxDecoderInitFunc(cb, &cb->slot.list->currHeader, &cb->slot.list->tmxCb);
    cb->onEnd = sdfPacTexListDecoderTmxBodyEndFunc;
}

void sdfPacResetOutputToAllocationEntry(sdfPacDecodeCb_t *aState)
{
    aState->dst = (u8 *)&aState->slot.list->currHeader;
    aState->rest = 0x10;
    aState->decoder = sdfPacMemDecoderFunc;
    aState->onEnd = sdfPacTexListDecoderTmxHeaderEndFunc;
}

void sdfPacTexListDecoderTmxBodyEndFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacTexListDecodeCb_t *list = cb->slot.list;

    sdfTexListAdd(cb->resrcTail->resrc, list->tmxCb.texHandle);
    {
        s32 nextIndex = list->currNum + 1;

        list->currNum = nextIndex;
        if (nextIndex == list->count) {
            sdfChipFreeDirect(list);
            sdfPacEndBlock(cb);
        } else {
            sdfPacResetOutputToAllocationEntry(cb);
        }
    }
}

void sdfPacTexListDecoder1(sdfPacDecodeCb_t *cb)
{
    s32 skipBytes = cb->srcLength;

    if (cb->rest < skipBytes) {
        skipBytes = cb->rest;
    }
    sdfPacUseSrc(cb, skipBytes);
    {
        s32 bytesRemaining = cb->rest - skipBytes;

        cb->rest = bytesRemaining;
        if (bytesRemaining != 0) {
            return;
        }
    }
    sdfPacResetOutputToAllocationEntry(cb);
}

void sdfPacTexListDecoder0(sdfPacDecodeCb_t *cb)
{
    u32 count;
    sdfPacTexListDecodeCb_t *list;
    sdfPacDecodeResrc_t *aWork;

    if (cb->srcLength != 0) {
        count = *(const u32 *)cb->srcData;
        sdfPacUseSrc(cb, 4);
        list = cb->slot.list;
        aWork = cb->resrcTail;
        list->count = count;
        list->currNum = 0;
        aWork->resrc = sdfTexListNew(count);
        cb->rest = ((count * 4 + 0x53) & -0x40) - 0x14;
        cb->decoder = sdfPacTexListDecoder1;
    }
}

void sdfPacTexListDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacNewResrc(cb, hdr);
    cb->slot.list = sdfChipAllocDirect(sizeof(sdfPacTexListDecodeCb_t));
    cb->decoder = sdfPacTexListDecoder0;
}
