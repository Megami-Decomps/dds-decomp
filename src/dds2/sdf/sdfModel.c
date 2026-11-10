#include "common.h"
#include "libsdf.h"
#include "model.h"
#include "draw.h"
#include "ps2asm.h"

#define func_002BFF98 sdfDevResizeBufferedRequest
#define func_002B2628 sdfChunkFindById
#include "eeregs.h"

/* Audited SDK COP2/MMI operations: ee-gcc cannot express packed
 * vector transfers or the implicit VU matrix-register convention in C. */


/* The serialized node-def block has a 16-byte count/padding prefix. */
typedef struct
{
    s32 count;
    u32 pad[3];
} sdfModelNodeDefList_t;

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);
extern void ToMatrix(void);
extern void MultMatrixRevNoArg(void);
extern void MultMatrixRev(void *aMatrix);
extern void sdf3DPrimSetTmPacket(void *aPacket);
extern void sdfModelSetOtCallbacks(sdfGrOt_t *aOt);


extern void *sdfChipAllocDirect(s32 aSize);
extern void *sdfChipAllocZeroDirect(s32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern sdfItemArray_t *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep);
extern void sdfItemArrayFree(void *aArray);
extern void sdfItemArrayGrow(void *aArray);
extern void func_002BFF98(void *aArray, s32 aCount);

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);
extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);
extern sdfDelayedResrcCB_t sdfModelDelayedResrcCB;

extern sdfMatList_t *sdfMatListMake(void *aTexList, u32 *aData);
extern void sdfMatListFreeResrc(void *aList, s32 aDelayed);
extern void sdfMatListCopyStat(void *aDst, void *aSrc);
extern sdfDmaTag_t *sdfMatMakePacket(sdfMat_t *aMat, sdfDmaTag_t *aBuffer, s32 aId);

extern volatile u8 sdfFrameId;
extern f32 sdfFaceDivSoften;
extern f32 sdfFaceDivBias;
extern const char sdfModelDefaultName[];

extern void *sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagLink2(sdfDrawTag_t *aTag, addr_t aHeadPacket, addr_t aTailPacket);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *root, void *p);

extern void *func_002B2628(void *aData, u32 aTag);
extern void sdfModelEvalTm2(sdfModelRootNode_t *aRoot, s32 aFrameId);
extern void sdfModelLinkDrawPacket(sdfModelNode_t *aNode, void *aShape, s32 aOtIndex, s32 aIndexFrom,
                                   s32 aId);
extern void sdfModelNodeBaseSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef);
extern sdfModelNode_t *sdfModelNodeNew(void);
extern void sdfModelMakeLocalTm(sdfModelNode_t *aNode);
extern sdfModelRootNode_t *sdfModelRootNew(void);
extern void sdfModelAllocNodeList(sdfModelRootNode_t *aRoot, s32 aLeng);

extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);
extern void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *aPkt);
extern void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *aPkt);
extern void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *aPkt);
extern void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *aPkt);

/* The shaped packet's header quadwords: the cnt tag, the two blend weights
 * VU1 reads and the UNPACK that follows them. */
typedef struct QWORD_ALIGNED
{
    sdfDmaTag_t dmatag;
    f32 weight[2];
    u32 code0;
    u32 code1;
} sdfModelShapedHeader_t;

/* One frame's 128-byte VU1 transform packet. The 4-aligned element type is
 * what gives retail's runtime 8-byte-alignment dispatch over the copy. */
/* sdfModelCopyStat below keeps its stride as (frame << 7) rather than a
 * sizeof: the multiply there changes two words of the copy's addressing. */
typedef struct
{
    s32 words[0x80 / 4];
} sdfModelTmImage_t;

/* An extra-data chunk: its four-character tag and its total size in bytes. */
typedef struct
{
    u32 tag;
    u32 size;
} sdfModelChunkHeader_t;

/* One MPOS record: 64 bytes, keyed by id, naming the node it hangs off. */
typedef struct QWORD_ALIGNED
{
    s32 nodeId;
    s32 id;
    u32 pad[2];
    sdfVector4_t pos;
    sdfVector4_t dir;
    sdfVector4_t up;
} sdfModelMarkPos_t;

void sdfModelRootFreeResrc(sdfModelRootNode_t *aRoot);
void sdfModelPacketFree(sdfModelNode_t *aNode);
void sdfModelNodeFree(sdfModelNode_t *aNode);
u8 *sdfModelAllocTmPacket(sdfModelNode_t *aNode);
void sdfModelNodeUnlink(sdfModelNode_t *aNode);
void sdfModelNodeSetParent(sdfModelNode_t *aNode, sdfModelNode_t *aParent);
void sdfModelNodeAdd(sdfModelRootNode_t *aRoot, sdfModelNode_t *aParent, sdfModelNode_t *aNode);
sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, s32 aId);
sdfDmaTag_t *sdfModelSetMatRefPacket(sdfModelRootNode_t *aRoot, s32 aMatIndex, sdfDmaTag_t *aBuffer,
                                     s32 aId);
void sdfModelMakeMorphList(sdfModelRootNode_t *root, s32 morphCount);
sdfModelRootNode_t *sdfModelMakeRoot(void *aTexList, sdfModelDef_t *aModelData);
void sdfModelNodeSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef);
void sdfModelMorphListCopyStat(sdfModelMorphList_t *aDst, sdfModelMorphList_t *aSrc);
void *sdfModelGetExtraChunk(sdfModelRootNode_t *aRoot, u32 aChunkId);
sdfModelMarkPos_t *sdfModelSearchMarkPosData(sdfModelRootNode_t *aRoot, s32 aId);
void sdfModelMakeMarkPosMatrix(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark);
void sdfModelMakeMarkPos(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark);

void sdfModelInitSystem(void)
{
    sdfDelayedResrcInitCB(&sdfModelDelayedResrcCB, (sdfDelayedResrcFreeFunc)sdfModelRootFreeResrc);
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
sdfModelNode_t *sdfModelNodeNew(void) {
    sdfModelNode_t *node = sdfChipAllocZeroDirect(sizeof(sdfModelNode_t));

    node->color = 0x80808080;
    node->drawAttr = -1;
    node->prev = node;
    node->next = node;
    SDF_VU0_STOREIDENTITYVECTOR(&node->rotate);
    SDF_VU0_STOREIDENTITYVECTOR(&node->pos);
    SDF_VU0_SETSCALE1VECTOR();
    SDF_VU0_STOREVECTOR(&node->scale);
    SDF_VU0_STOREIDENTITY(&node->localNoScaledTm);
    return node;
}

u8 *sdfModelAllocTmPacket(sdfModelNode_t *aNode)
{
    u8 *packet = aNode->tmPacket;

    if (packet == NULL) {
        packet = sdfChipAllocDirect(0x100);
        aNode->tmPacket = packet;
    }
    return packet;
}

void sdfModelPacketFree(sdfModelNode_t *aNode)
{
    sdfModelPacketHeader_tag **chain = aNode->drawPacket;
    s32 i = 0;
    s32 count = 2;

    do {
        sdfModelPacketHeader_tag *packet = *chain;

        while (packet != NULL) {
            sdfModelPacketHeader_tag *next = packet->next;

            if (packet->memHandle != NULL) {
                sdfMemFreeDirect(packet->memHandle);
            } else {
                sdfChipFreeDirect(packet);
            }
            packet = next;
        }
        i++;
        chain++;
    } while (i != count);
}

void sdfModelNodeFree(sdfModelNode_t *aNode)
{
    sdfModelPacketFree(aNode);
    sdfChipFreeDirect(aNode->tmPacket);
    aNode->tmPacket = NULL;
    sdfChipFreeDirect(aNode);
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
sdfModelRootNode_t *sdfModelRootNew(void) {
    sdfModelRootNode_t *slot;

    slot = sdfChipAllocZeroDirect(sizeof(sdfModelRootNode_t));
    slot->nodeList = (sdfModelNodeList_t *)sdfItemArrayNew(0, 4, 0x20);
    SDF_VU0_STOREIDENTITY(&slot->tm);
    SDF_VU0_STOREIDENTITYVECTOR(&slot->rotate);
    SDF_VU0_SETSCALE1VECTOR();
    SDF_VU0_STOREVECTOR(&slot->scale);
    slot->drawAttr = -1;
    slot->color = 0x80808080;
    return slot;
}

void sdfModelRootFreeResrc(sdfModelRootNode_t *aRoot)
{
    s32 i;

    for (i = 0; i < aRoot->nodeList->count; i++) {
        sdfModelNodeFree(aRoot->nodeList->items[i]);
    }
    sdfItemArrayFree(aRoot->nodeList);
    sdfChipFreeDirect(aRoot);
}

void sdfModelRootFree(sdfModelRootNode_t *aRoot, s32 aReleaseNodes, s32 aReleaseMatList)
{
    if (aRoot == NULL) {
        return;
    }
    if (aReleaseMatList != 0) {
        sdfMatListFreeResrc(aRoot->matList, 1);
    }
    if (aRoot->morphList != NULL) {
        sdfItemArrayFree(aRoot->morphList);
    }
    if (aReleaseNodes != 0) {
        sdfDelayedResrcEntry(&sdfModelDelayedResrcCB, aRoot);
    } else {
        sdfItemArrayFree(aRoot->nodeList);
        sdfChipFreeDirect(aRoot);
    }
}

void sdfModelAllocNodeList(sdfModelRootNode_t *aRoot, s32 aLeng)
{
    func_002BFF98(aRoot->nodeList, aLeng);
}

void sdfModelMakeNodeList(sdfModelRootNode_t *aRoot) {
    sdfModelNode_t **nodeArray = aRoot->nodeList->items;
    sdfModelNode_t *currentNode = aRoot->headNode;
    sdfModelNode_t **output;

    if (currentNode != NULL) {
        output = nodeArray;
        do {
            *output++ = currentNode;
            if (currentNode->children != NULL) {
                currentNode = currentNode->children;
            } else {
                do {
                    sdfModelNode_t *nextSibling = currentNode->next;
                    if (nextSibling != NULL) {
                        currentNode = nextSibling;
                        break;
                    }
                    currentNode = currentNode->parent;
                } while (currentNode != NULL);
            }
        } while (currentNode != NULL);
    }
}

void sdfModelNodeAdd(sdfModelRootNode_t *aRoot, sdfModelNode_t *aParent, sdfModelNode_t *aNode)
{
    s16 count;
    sdfModelNode_t **items;
    sdfModelNodeList_t *list;
    s32 newCount;

    list = aRoot->nodeList;
    count = list->count;
    newCount = count + 1;
    if ((s16)list->capacity < newCount) {
        sdfItemArrayGrow(list);
        list = aRoot->nodeList;
    }
    items = list->items;
    aNode->root = aRoot;
    list->count = newCount;
    items[count] = aNode;
    sdfModelNodeSetParent(aNode, aParent);
}

void sdfModelNodeUnlink(sdfModelNode_t *aNode)
{
    sdfModelNode_t *parent = aNode->parent;

    if (parent == NULL) {
        sdfModelRootNode_t *root = aNode->root;

        if (root->headNode == aNode) {
            root->headNode = NULL;
        }
        return;
    }
    {
        sdfModelNode_t *next = aNode->next;
        sdfModelNode_t *prev = aNode->prev;

        if (next != aNode) {
            next->prev = prev;
            prev->next = next;
            if (parent->children == aNode) {
                parent->children = next;
            }
            aNode->prev = aNode;
            aNode->next = aNode;
            return;
        }
        if (parent->children == aNode) {
            parent->children = NULL;
        }
    }
}

void sdfModelNodeSetParent(sdfModelNode_t *aNode, sdfModelNode_t *aParent)
{
    if (aParent == NULL) {
        sdfModelRootNode_t *root = aNode->root;
        sdfModelNode_t *head = root->headNode;

        if (head != aNode) {
            root->headNode = aNode;
            if (head != NULL) {
                aNode->children = head;
                head->parent = aNode;
            }
            aNode->parent = NULL;
        }
    } else if (aNode->parent != aParent) {
        sdfModelNode_t *head;

        sdfModelNodeUnlink(aNode);
        head = aParent->children;
        if (head == NULL) {
            aParent->children = aNode;
        } else {
            sdfModelNode_t *prev = head->prev;

            aNode->prev = prev;
            prev->next = aNode;
            head->prev = aNode;
            aNode->next = head;
        }
        aNode->parent = aParent;
    }
}

sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, s32 aId)
{
    sdfModelNodeList_t *list = aRoot->nodeList;
    s16 count;
    sdfModelNode_t **items;
    s32 i;

    count = list->count;
    items = list->items;
    if (aRoot->flags & SDF_MODEL_FLAG_COMPLEX_NODE_ID) {
        for (i = 0; i < count; i++) {
            sdfModelNode_t *node = items[i];

            if (node->id == aId) {
                return node;
            }
        }
        return 0;
    }
    if ((u32)aId >= (u32)count) {
        return 0;
    }
    return items[aId];
}

sdfDmaTag_t *sdfModelSetTmRefPacket(sdfModelNode_t *aNode, sdfDmaTag_t *aBuffer, s32 aId)
{
    addr_t addr = EE_PHYSICAL(EE_ADDR((u8 *)aNode->tmPacket + aId * sizeof(sdfModelTmImage_t)));

    *(u64 *)aBuffer = ((u64)addr << 32) | 0x30000008;
    aBuffer->dat1 = 0x6C07C000;
    aBuffer->dat0 = 0;
    return aBuffer + 1;
}

sdfDmaTag_t *sdfModelSetTermPacket(sdfDmaTag_t *aBuffer)
{
    *(u64 *)&aBuffer->dat0 = 0;
    *(u64 *)aBuffer = 0x60000000;
    return aBuffer + 1;
}

sdfDmaTag_t *sdfModelSetMatRefPacket(sdfModelRootNode_t *aRoot, s32 aMatIndex, sdfDmaTag_t *aBuffer,
                                     s32 aId)
{
    return sdfMatMakePacket(aRoot->matList->items[aMatIndex], aBuffer, aId);
}

s32 sdfModelGetDrawPacketSize(const u32 *aList, s32 *aShapeCount, s32 *aExtSize)
{
    s32 count;
    const sdfShapePacket_t *packet;
    s32 bytes;
    s32 skinCount;
    s32 tempQwc;

    if (aList == 0) {
        *aShapeCount = 0;
        *aExtSize = 0;
        return 0;
    }
    count = aList[0];
    aList++;
    bytes = 0;
    skinCount = 0;
    count &= 0xFFFF;
    tempQwc = 0;
    if (count != 0) {
        do {
            packet = EE_ADDR_AS(const sdfShapePacket_t *, *aList);
            aList++;
            switch (packet->packetType) {
            case 1:
                bytes += 0x20;
                break;
            case 2:
                bytes += (*(const u16 *)&((const sdfShapeOneSkinPacket_t *)packet)->boneCount
                          << 4) +
                         0x40;
                break;
            case 3:
                skinCount++;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                tempQwc += 0x10;
                break;
            }
            count--;
        } while (count != 0);
    }
    *aShapeCount += skinCount;
    *aExtSize = tempQwc;
    return bytes != 0 ? bytes + 0x20 : 0;
}

sdfDmaTag_t *sdfModelSetNormalDrawPacket(sdfModelNode_t *aNode, sdfShapePacket_t *aDataPtr,
                                         sdfDmaTag_t *aBuffer, s32 aId)
{
    u32 packed = *(u32 *)&aDataPtr->packetSize;
    u16 matIndex = packed >> 16;
    u16 qwc = packed;
    sdfDmaTag_t *body;

    body = sdfModelSetMatRefPacket(aNode->root, matIndex, aBuffer, aId);
    body->id = 0x30;
    body->qwc = qwc;
    body->addr = (void *)EE_PHYSICAL(EE_ADDR(aDataPtr->dataBody));
    body->dat0 = 0;
    body->dat1 = 0;
    return body + 1;
}

/* NON_MATCHING: 82 of 96 words match at the exact size (first differing word
 * at +0x38); aDataPtr and the 0x6C07C000 constant swap $23 and $30. */
/* NON_MATCHING. Behaviour checked against retail: 4 PCSX2 call vectors give the same return and memory,
 * battle_0_1, seq28, seq32, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
sdfDmaTag_t *sdfModelSetOneSkinDrawPacket(sdfModelNode_t *aNode, sdfShapeOneSkinPacket_t *aDataPtr,
                                          sdfDmaTag_t *aBuffer, s32 aId)
{
    u32 packed = *(u32 *)&aDataPtr->packetSize;
    u16 matIndex = packed >> 16;
    s32 ofs;
    u32 vuAddr = 1;
    s32 i = 0;
    sdfDmaTag_t *tag;
    s32 count;
    u16 *boneId;
    u32 *words;

    packed &= 0xFFFF;
    tag = sdfModelSetMatRefPacket(aNode->root, matIndex, aBuffer, aId);
    ofs = aId << 7;
    count = aDataPtr->boneCount;
    boneId = aDataPtr->boneId;
    do {
        sdfModelNode_t *node = sdfModelSearchNode(aNode->root, *boneId++);

        u32 addr;

        i++;
        tag->qwc = 7;
        tag->id = 0x30;
        addr = EE_ADDR(sdfModelAllocTmPacket(node));
        tag->dat0 = 0;
        tag->dat1 = vuAddr | 0x6C07C000;
        tag->addr = EE_ADDR_AS(void *, EE_PHYSICAL(addr + ofs));
        vuAddr += 7;
        tag++;
    } while (i != count);
    tag->qwc = 1;
    tag->id = 0x10;
    tag->dat0 = 0;
    tag->dat1 = 0;
    words = (u32 *)(tag + 1);
    words[0] = 0x6001C000;
    words[1] = count;
    words[2] = 0x0400000E;
    words[3] = 0x14000000;
    tag += 2;
    tag->qwc = packed;
    tag->id = 0x30;
    tag->dat0 = 0;
    tag->dat1 = 0;
    tag->addr = (void *)EE_PHYSICAL(EE_ADDR(aDataPtr->dataBody));
    return tag + 1;
}
#else
INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelSetOneSkinDrawPacket);
#endif

/* NON_MATCHING: 63 of 150 words match (first differing word at +0x0); eight
 * saved registers where retail fits seven. */
/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * capture_009, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void sdfModelLinkShapedDrawPacket(sdfModelNode_t *aNode, sdfShapeShapedPacket_t *aDataPtr,
                                  sdfDrawTag_t *aDrawTag, s32 aId)
{
    sdfModelMorphParm_t out[2];
    s32 used = 0;
    s32 j = 0;
    s32 k;
    s32 i;
    u32 packed;
    u16 matIndex;
    s32 listCount;
    sdfModelMorph_t *morph;
    void *buf;
    sdfDmaTag_t *tag;
    sdfModelShapeList_t **cursor;

    packed = *(u32 *)&aDataPtr->morphIndex;
    matIndex = packed >> 16;
    packed &= 0xFFFF;
    morph = &aNode->root->morphList->items[packed];
    listCount = aDataPtr->listCount;
    do {
        if (1e-6f < morph->target[j].blend) {
            out[used] = morph->target[j];
            used++;
        }
        j++;
    } while (j != 2);
    buf = sdfGrAllocPacketBuff((used * 16 + 0x10) * listCount + 0x40);
    tag = sdfModelSetMatRefPacket(aNode->root, matIndex, buf, aId);
    *(u64 *)tag = 0x10000001;
    *(u64 *)&tag->dat0 = (u64)0x6002C000 << 32;
    for (k = 0; k < used; k++) {
        ((sdfModelShapedHeader_t *)tag)->weight[k] = out[k].blend;
    }
    for (; k < 2; k++) {
        ((sdfModelShapedHeader_t *)tag)->weight[k] = 0.0f;
    }
    ((sdfModelShapedHeader_t *)tag)->code0 = 0x0400000A;
    ((sdfModelShapedHeader_t *)tag)->code1 = 0x14000000;
    tag += 2;
    cursor = aDataPtr->shapeList;
    i = 0;
    do {
        sdfModelShapeList_t *list = *cursor++;
        s32 m;

        for (m = 0; m < used; m++) {
            sdfModelShapeEntry_t *entry =
                (sdfModelShapeEntry_t *)(EE_ADDR_AS(u8 *, (out[m].targetId * 8)) + EE_ADDR(list));

            *(u64 *)&tag->dat0 = 0;
            *(u64 *)tag = SDF_DMA_TAG(EE_ADDR(entry[1].dataPtr),
                                      entry[1].qwc | 0x30000000);
            tag++;
        }
        *(u64 *)&tag->dat0 = 0;
        *(u64 *)tag = SDF_DMA_TAG(EE_ADDR(list->next), list->count | 0x30000000);
        tag++;
        i++;
    } while (i != listCount);
    QUAD_ZERO(*(u128 *)tag);
    sdfDrawTagLink2(aDrawTag, EE_ADDR(buf), EE_ADDR(tag));
}
#else
INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelLinkShapedDrawPacket);
#endif

sdfModelPacketHeader_tag *sdfModelNewPacketHeader(sdfModelNode_t *aNode, s32 aOtIndex, s32 aId)
{
    sdfModelPacketHeader_tag *packet = sdfChipAllocDirect(sizeof(*packet));
    sdfModelPacketHeader_tag **head =
        (sdfModelPacketHeader_tag **)((u32)aId * sizeof(*aNode->drawPacket) + EE_ADDR(aNode) +
                                    (u32)&((sdfModelNode_t *)0)->drawPacket);
    sdfModelPacketHeader_tag *next = *head;

    packet->otIndex = aOtIndex;
    *head = packet;
    packet->size = 1;
    packet->memHandle = NULL;
    packet->next = next;
    return packet;
}

/* NON_MATCHING: 104 of 122 words differ (first at +0x14).
 * Behaviour checked against retail: 34 PCSX2 call vectors give the same return and memory,
 * a battle, field and event scenes, covering only the paths those calls took.
 * (sdfport a0afdc1, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void sdfModelLinkDrawPacket(sdfModelNode_t *aNode, void *aShape, s32 aOtIndex, s32 aIndexFrom, s32 aId)
{
    s32 numOfSkin;
    s32 tempQwc;
    s32 bytes;
    u32 *cursor;
    u32 packed;
    s32 count;
    sdfDmaTag_t *tag = NULL;
    sdfShapePacket_t *packet;
    sdfModelPacketHeader_tag *header;

    /* BUG: the callee adds to numOfSkin and the caller never clears it, so retail reads stack garbage. */
#ifdef FIX_BUGS
    numOfSkin = 0;
#endif
    bytes = sdfModelGetDrawPacketSize(aShape, &numOfSkin, &tempQwc);
    cursor = aShape;
    if (bytes == 0 && numOfSkin == 0 && tempQwc == 0) {
        return;
    }
    sdfModelAllocTmPacket(aNode);
    packed = *cursor++;
    count = packed & 0xFFFF;
    if (aIndexFrom != 0) {
        aOtIndex = packed >> 16;
    }
    if (bytes != 0) {
        sdfMemHandle_t *handle = sdfMemAllocDirect(bytes);
        sdfModelPacketHeader_tag *chunk = (sdfModelPacketHeader_tag *)sdfMemLock(handle);
        sdfModelPacketHeader_tag **head =
            (sdfModelPacketHeader_tag **)((u32)aId * sizeof(*aNode->drawPacket) + EE_ADDR(aNode) +
                                        (u32)&((sdfModelNode_t *)0)->drawPacket);

        tag = (sdfDmaTag_t *)(chunk + 1);
        chunk->otIndex = aOtIndex;
        chunk->dataPtr = tag;
        chunk->size = (bytes + 0xF) >> 4;
        chunk->next = *head;
        chunk->memHandle = handle;
        *head = chunk;
        chunk->type = 1;
    }
    do {
        packet = EE_ADDR_AS(sdfShapePacket_t *, *cursor++);
        switch (packet->packetType) {
        case 1:
            tag = sdfModelSetNormalDrawPacket(aNode, packet, tag, aId);
            break;
        case 2:
            tag = sdfModelSetOneSkinDrawPacket(aNode, (sdfShapeOneSkinPacket_t *)packet, tag, aId);
            break;
        case 3:
            header = sdfModelNewPacketHeader(aNode, aOtIndex, aId);
            header->type = 2;
            header->dataPtr = packet;
            break;
        case 4:
        case 6:
        case 8:
            header = sdfModelNewPacketHeader(aNode, aOtIndex, aId);
            header->type = 3;
            header->dataPtr = packet;
            break;
        case 5:
        case 7:
            sdfCoreDrawScanOneSkinNodes(aNode->root, packet);
            header = sdfModelNewPacketHeader(aNode, aOtIndex, aId);
            header->type = 3;
            header->dataPtr = packet;
            break;
        }
    } while (--count != 0);
    if (bytes != 0) {
        sdfModelSetTermPacket(tag);
    }
}
#else
INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelLinkDrawPacket);
#endif

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeLocalTm(sdfModelNode_t *aNode) {
    SDF_VU0_LOADVECTOR(&aNode->rotate);
    ToMatrix();
    SDF_VU0_LOAD_ROW3(&aNode->pos);
    SDF_VU0_STOREMATRIX(&aNode->localNoScaledTm);
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelNodeBaseSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef)
{
    aNode->shape = aDef;
    aNode->id = aDef->id;
    FromEuler(aDef->rotate.x, aDef->rotate.y, aDef->rotate.z);
    SDF_VU0_STOREVECTOR(&aNode->rotate);
    SDF_VU0_COPYVECTOR(&aNode->pos, &aDef->pos);
    SDF_VU0_COPYVECTOR(&aNode->scale, &aDef->scale);
    aNode->pos.w = 1.0f;
    sdfModelMakeLocalTm(aNode);
    aNode->bounds = aDef->bounds;
}

void sdfModelNodeSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef)
{
    s32 pass;
    s32 slot;
    struct sdfShapeHeader_t **cursor;
    struct sdfShapeHeader_t *shape;

    sdfModelNodeBaseSet(aNode, aDef);
    switch (aDef->format) {
    case 0:
        for (pass = 0; pass != 2; pass++) {
            for (slot = 0; slot != 3; slot++) {
                sdfModelLinkDrawPacket(aNode, aDef->shape[slot], slot, 0, pass);
            }
        }
        break;
    case 1:
        if (aDef->shape[0] != NULL) {
            for (pass = 0; pass != 2; pass++) {
                cursor = (struct sdfShapeHeader_t **)aDef->shape[0];
                while ((shape = *cursor++) != NULL) {
                    sdfModelLinkDrawPacket(aNode, shape, 0, 1, pass);
                }
            }
        }
        break;
    }
}

void sdfModelNodeSetShape(sdfModelNode_t *aNode, void *aData, s32 aType)
{
    s32 pass;

    aNode->shape = aData;
    sdfModelPacketFree(aNode);
    sdfModelAllocTmPacket(aNode);
    sdfModelMakeLocalTm(aNode);
    for (pass = 0; pass != 2; pass++) {
        sdfModelLinkDrawPacket(aNode, aData, aType, 0, pass);
    }
}

void sdfModelMakeMorphList(sdfModelRootNode_t *root, s32 morphCount)
{
    sdfModelMorphList_t *list;
    sdfModelMorph_t *items;
    s32 i;
    s32 j;

    if (morphCount > 0) {
        list = (sdfModelMorphList_t *)sdfItemArrayNew(morphCount, 0x10, 1);
        root->morphList = list;
        items = list->items;
        for (i = 0; i != morphCount; i++) {
            for (j = 0; j != 2; j++) {
                items[i].target[j].targetId = 0;
                items[i].target[j].blend = 0.0f;
            }
        }
        list->count = morphCount;
    }
}

sdfModelRootNode_t *sdfModelMakeRoot(void *aTexList, sdfModelDef_t *aModelData) {
    s32 itemIndex = 0;
    sdfModelRootNode_t *model = sdfModelRootNew();
    sdfModelNodeDefList_t *list;
    s32 count;
    sdfModelNodeDef_t *item;
    sdfModelNode_t *node;
    sdfModelNode_t *parent;

    model->texList = aTexList;
    model->matList = sdfMatListMake(aTexList, aModelData->matDef_New);
    model->extraData = aModelData->extraData;
    sdfModelMakeMorphList(model, aModelData->morphCount);
    list = aModelData->modelDef;
    count = list->count;
    list++;
    sdfModelAllocNodeList(model, count);
    item = (sdfModelNodeDef_t *)list;
    if (count != itemIndex) {
        do {
            node = sdfModelNodeNew();
            parent = NULL;
            if (item->parentIndex >= 0) {
                parent = ((sdfModelNode_t **)model->nodeList->items)[item->parentIndex];
            }
            sdfModelNodeAdd(model, parent, node);
            itemIndex++;
            node->id = item->id;
            item++;
        } while (itemIndex != count);
    }
    return model;
}

sdfModelRootNode_t *sdfModelMake(void *aTexList, sdfModelDef_t *aModelData)
{
    s32 i = 0;
    sdfModelRootNode_t *root = sdfModelMakeRoot(aTexList, aModelData);
    u32 *def = aModelData->modelDef;
    s32 count = def[0];
    sdfModelNodeDef_t *nodeDef = (sdfModelNodeDef_t *)(def + 4);

    if (count != i) {
        do {
            sdfModelNodeSet(root->nodeList->items[i], nodeDef);
            nodeDef++;
            i++;
        } while (i != count);
    }
    return root;
}

sdfModelRootNode_t *sdfModelMakeEarly(void *aTexList, sdfModelDef_t *aModelData)
{
    s32 i = 0;
    sdfModelRootNode_t *root = sdfModelMakeRoot(aTexList, aModelData);
    u32 *def;
    s32 count;
    sdfModelNodeDef_t *nodeDef;

    root->flags |= SDF_MODEL_FLAG_EARLY_SETUP;
    def = aModelData->modelDef;
    count = def[0];
    nodeDef = (sdfModelNodeDef_t *)(def + 4);
    if (count != i) {
        do {
            sdfModelNodeBaseSet(root->nodeList->items[i], nodeDef);
            nodeDef++;
            i++;
        } while (i != count);
    }
    return root;
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelEvalNode(sdfModelNode_t *aNode, const f32 (*aParentTm)[4], s32 aFrameId) {
    f32 *xAxis = &aNode->localNoScaledTm.m[0];
    f32 *yAxis;
    f32 *zAxis;
    f32 *scale;
    f32 *translation;
    f32 (*transformed)[4];
    u8 *workspace;
    sdfModelNode_t *child;

    SDF_VU0_LOAD_ROW0_BARRIER(xAxis);
    yAxis = &aNode->localNoScaledTm.m[4];
    SDF_VU0_LOAD_ROW1_BARRIER(yAxis);
    zAxis = &aNode->localNoScaledTm.m[8];
    SDF_VU0_LOAD_ROW2_BARRIER(zAxis);
    scale = (f32 *)&aNode->scale;
    SDF_VU0_LOADVECTOR_BARRIER(scale);
    SDF_VU0_SCALE_XYZ();
    translation = (f32 *)&aNode->pos;
    SDF_VU0_LOAD_ROW3_BARRIER(translation);
    SDF_VU0_LOADMATRIX_B(aParentTm);
    MultMatrixRevNoArg();
    transformed = (f32 (*)[4])aNode->tm.m;
    SDF_VU0_STOREMATRIX(transformed);
    workspace = aNode->tmPacket;
    if (workspace != NULL) {
        sdf3DPrimSetTmPacket((void *)(workspace + aFrameId * sizeof(sdfModelTmImage_t)));
    }
    child = aNode->children;
    if (child == 0) {
        return;
    }
    do {
        sdfModelEvalNode(child, (const f32 (*)[4])transformed, aFrameId);
        child = child->next;
    } while (child != aNode->children);
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelEvalTm2(sdfModelRootNode_t *aRoot, s32 aFrameId) {
    u128 rootMatrix[4];
    void *transform = &aRoot->tm;
    f32 *scale;
    sdfModelNodeList_t *list;

    SDF_VU0_LOADMATRIX(transform);
    scale = (f32 *)&aRoot->scale;
    SDF_VU0_LOADVECTOR_BARRIER(scale);
    SDF_VU0_SCALE_XYZ_REV();
    SDF_VU0_STOREMATRIX_MEMOPS(rootMatrix[0], rootMatrix[1], rootMatrix[2], rootMatrix[3]);
    list = aRoot->nodeList;
    sdfModelEvalNode(((sdfModelNode_t **)list->items)[0], (const f32 (*)[4])rootMatrix, aFrameId);
}

void sdfModelEvalTm(sdfModelRootNode_t *aRoot)
{
    sdfModelEvalTm2(aRoot, (s8)sdfFrameId);
}

void sdfModelOtInit(sdfModelOt_t *aModelOt, sdfGrOt_t *aOt0, sdfGrOt_t *aOt1,
                    sdfGrOt_t *aOt2, sdfGrOt_t *aOt3)
{
    aModelOt->ot[3] = aOt3;
    aModelOt->ot[0] = aOt0;
    aModelOt->ot[2] = aOt2;
    aModelOt->ot[1] = aOt1;
    sdfModelSetOtCallbacks(aOt1);
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
s32 sdfModelCheckBounds(const f32 *aMinMax) {
    s32 flags;
    s32 result;

    VU0_CLIP_TEST_BOX_CORNERS(flags, aMinMax);
    result = flags & 0x3F;
    result &= (flags >> 12) & (flags >> 6);
    result &= flags >> 18;
    return result;
}

extern void sdfMatListUpdatePacket(sdfMatList_t *aMatList, s32 aId);
extern void sdfResourceListApplyEntryChanges(sdfMatList_t *aMatList, s32 aId);
extern void sdfSetForcedAssetTextureMode(s32 aMode);
extern void *sdf3DLightDefault;
extern void *sdf3DFogDefault;
extern u8 corePktSoftenBias;
extern u8 sdf3DScreenTm[];
extern sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32));
extern void sdfDrawTagLinkRef(sdfDrawTag_t *aTag, addr_t aPacket);
extern void sdfDrawTagLinkCall(sdfDrawTag_t *aTag, addr_t aPacket);
extern void sdfCoreDrawSetZBias(f32 z);
extern void sdfCoreDrawSetLight(void *light);
extern void sdfCoreDrawSetFog(void *fog);
extern void *sdfCorePacketGen(sdfModelNode_t *node, void *packet_p, u32 attr_mask, u32 color,
                              f32 fdSoften, f32 fdBias, s32 frame_id, s32 reverse);

/* The 0xC0-byte packet a temp-linked node builds: the tm REF, the 128 bytes
 * VU1 reads through it, and the draw parameters that close it. */
typedef struct QWORD_ALIGNED
{
    sdfDmaTag_t tmTag;
    u8 tm[0x80];
    sdfModelDrawParmPacket_t parm;
} sdfModelTempDrawPacket_t;

/* vu0 routine: each node's screen transform, and its w lane as the sort key. */
/* NON_MATCHING: 163 of 282 words match at the exact size (first differing word
 * at +0x60). */
#ifdef NON_MATCHING
void sdfModelTempPacketLinkOt(sdfGrOt_t **ot, sdfModelRootNode_t *root)
{
    sdfDrawTag_t *tags[4];
    s32 i;
    s32 frame;
    u32 rootColor;
    u32 color;
    u32 drawAttr;
    u32 attr;
    void *light;
    void *fog;
    s32 drew;
    u32 softenParm;
    u32 softenBias;
    u64 parmTag;
    s32 semiTrans;
    f32 soften;
    f32 bias;

    frame = (s8)sdfFrameId;
    sdfMatListUpdatePacket(root->matList, frame);
    rootColor = root->color;
    light = root->light;
    fog = root->fog;
    if (light == NULL) {
        light = sdf3DLightDefault;
    }
    if (fog == NULL) {
        fog = sdf3DFogDefault;
    }
    drawAttr = root->drawAttr;
    softenBias = corePktSoftenBias;
    if (root->flags & SDF_MODEL_FLAG_SELF_FACEDIV_PARM) {
        soften = root->faceDivSoften;
        bias = root->faceDivBias;
    } else {
        soften = sdfFaceDivSoften;
        bias = sdfFaceDivBias;
    }
    softenParm = softenBias << 16;
    drew = 0;
    for (i = 0; i < root->nodeList->count; i++) {
        sdfModelNode_t *node = root->nodeList->items[i];
        sdfModelNodeDef_t *def = node->shape;
        sdfModelBounds_t *bounds;
        struct sdfShapeHeader_t **list;
        struct sdfShapeHeader_t *header;
        sdfDrawTag_t *tag;
        f32 *matrix;
        f32 zValue;
        s32 j;

        if (def == NULL) {
            continue;
        }
        if (def->format != 1) {
            continue;
        }
        list = (struct sdfShapeHeader_t **)def->shape[0];
        if (list == NULL) {
            continue;
        }
        if (node->flags & (SDF_MODEL_NODE_FLAG_HIDE | SDF_MODEL_NODE_FLAG_MOTION_HIDE)) {
            continue;
        }
        matrix = (f32 *)&node->tm;
        SDF_VU0_LOADMATRIX(matrix);
        MultMatrixRev(sdf3DScreenTm);
        bounds = node->bounds;
        if (bounds != NULL) {
            if (sdfModelCheckBounds((const f32 *)bounds) != 0) {
                continue;
            }
        }
        SDF_VU0_GET_ROW3();
        SDF_VU0_GETVECTOR_W(zValue);
        color = rootColor;
        if (node->flags & SDF_MODEL_NODE_FLAG_LOCALCOLOR) {
            color = node->color;
        }
        attr = drawAttr & node->drawAttr;
        for (j = 0; j != 4; j++) {
            tags[j] = NULL;
        }
        sdfCoreDrawSetZBias(root->zBias);
        semiTrans = (s32)color < 0;
        parmTag = ((u64)(attr | softenParm) << 32) | 0x6102C000;
        while ((header = *list++) != NULL) {
            s32 otIndex;
            sdfShapePacket_t **packets;
            sdfShapePacket_t *pkt;

            otIndex = (*(u32 *)header & 0xFFFF0000) >> 16;
            if (otIndex == 0) {
                otIndex = semiTrans == 0;
            }
            tag = tags[otIndex];
            if (tag == NULL) {
                sdfModelTempDrawPacket_t *buf;

                tag = sdfPrimAllocDrawTag(NULL);
                *(f32 *)&tag->z = zValue;
                tag->lightPacket = EE_ADDR(light);
                tag->fogPacket = EE_ADDR(fog);
                tags[otIndex] = tag;
                buf = sdfGrAllocPacketBuff(sizeof(*buf));
                *(u64 *)&buf->tmTag = 0x10000008;
                *(u64 *)&buf->tmTag.dat0 = (u64)0x6C07C000 << 32;
                SDF_VU0_LOADMATRIX(matrix);
                sdf3DPrimSetTmPacket(buf->tm);
                *(u64 *)&buf->parm.next = 0x20000002;
                buf->parm.unpackColor = 0x6E01C002;
                buf->parm.color = color;
                buf->parm.unpackParm = 0x6003C003;
                *(u32 *)&buf->parm.zBias = *(u32 *)&root->zBias;
                buf->parm.faceDivSoften = soften;
                buf->parm.faceDivBias = bias;
                buf->parm.itop = 0x04000004;
                buf->parm.mscal = 0x14000000;
                *(u64 *)&buf->parm.next.dat0 = parmTag;
                sdfDrawTagLink2(tag, EE_ADDR(buf), EE_ADDR(&buf->parm));
            }
            packets = (sdfShapePacket_t **)header->packet;
            while ((pkt = *packets++) != NULL) {
                if (pkt->packetType == PACKET_TYPE_NORMAL) {
                    u32 packed;
                    u16 matIndex;
                    sdfDmaTag_t *buf;
                    sdfDmaTag_t *body;

                    buf = sdfGrAllocPacketBuff(0x30);
                    packed = *(u32 *)&pkt->packetSize;
                    matIndex = packed >> 16;
                    packed &= 0xFFFF;
                    body = sdfModelSetMatRefPacket(root, matIndex, buf, frame);
                    *(u64 *)&body->dat0 = 0;
                    *(u64 *)body = SDF_DMA_TAG(EE_ADDR(pkt->dataBody),
                                               packed | 0x30000000);
                    body++;
                    *(u64 *)body = 0x20000000;
                    *(u64 *)&body->dat0 = 0;
                    sdfDrawTagLink2(tag, EE_ADDR(buf), EE_ADDR(body));
                }
            }
        }
        for (j = 0; j != 4; j++) {
            tag = tags[j];
            if (tag != NULL) {
                drew = 1;
                ot[j]->regFunc(ot[j], tag);
            }
            tags[j] = NULL;
        }
    }
    if (drew != 0) {
        root->flags |= SDF_MODEL_FLAG_DREW;
    }
}
#else
INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelTempPacketLinkOt);
#endif

/* DDS engine donor: dds-decomp (redthing1), sdfModelBuildNodeResourcePackets. */
/* vu0 routine: each node's screen transform, and its w lane as the sort key. */
void sdfModelLinkOt(sdfGrOt_t **aOt, sdfModelRootNode_t *aRoot)
{
    sdfDrawTag_t *tags[4];
    s32 i;
    s32 frame;
    u32 rootColor;
    s32 color;
    u32 drawAttr;
    u32 attr;
    void *light;
    void *fog;
    s32 drew;
    u32 softenParm;
    u32 softenBias;
    u64 parmTag;
    s32 semiTrans;
    f32 soften;
    f32 bias;
    s32 reverseFace;
    s32 reverse;

    aRoot->flags &= ~SDF_MODEL_FLAG_DREW;
    if (aRoot->flags & SDF_MODEL_FLAG_EARLY_SETUP) {
        sdfModelTempPacketLinkOt(aOt, aRoot);
    }
    frame = (s8)sdfFrameId;
    if (aRoot->flags & SDF_MODEL_FLAG_FORCE_TEX_NO_SPECULAR) {
        sdfSetForcedAssetTextureMode(1);
        sdfMatListUpdatePacket(aRoot->matList, frame);
        sdfSetForcedAssetTextureMode(0);
    } else if (aRoot->flags & SDF_MODEL_FLAG_FORCE_TEX) {
        sdfSetForcedAssetTextureMode(2);
        sdfMatListUpdatePacket(aRoot->matList, frame);
        sdfSetForcedAssetTextureMode(0);
    } else {
        sdfResourceListApplyEntryChanges(aRoot->matList, frame);
    }
    rootColor = aRoot->color;
    light = aRoot->light;
    if (light == NULL) {
        light = sdf3DLightDefault;
    }
    fog = aRoot->fog;
    if (fog == NULL) {
        fog = sdf3DFogDefault;
    }
    drawAttr = aRoot->drawAttr;
    softenBias = corePktSoftenBias;
    if (aRoot->flags & SDF_MODEL_FLAG_SELF_FACEDIV_PARM) {
        soften = aRoot->faceDivSoften;
        bias = aRoot->faceDivBias;
    } else {
        soften = sdfFaceDivSoften;
        bias = sdfFaceDivBias;
    }
    reverseFace = aRoot->flags & SDF_MODEL_FLAG_REVERSE_FACE;
    reverse = reverseFace != 0;
    drew = 0;
    for (i = 0; i < aRoot->nodeList->count; i++) {
        sdfModelNode_t *node = aRoot->nodeList->items[i];
        sdfModelPacketHeader_tag *header = node->drawPacket[frame];
        sdfModelBounds_t *bounds;
        f32 zValue;
        s32 j;
        sdfDrawTag_t *tag;

        softenParm = softenBias;
        softenParm <<= 16;

        if (header == NULL) {
            continue;
        }
        if (node->flags & (SDF_MODEL_NODE_FLAG_HIDE | SDF_MODEL_NODE_FLAG_MOTION_HIDE)) {
            continue;
        }
        SDF_VU0_LOADMATRIX((f32 *)&node->tm);
        MultMatrixRev(sdf3DScreenTm);
        bounds = node->bounds;
        if (bounds != NULL) {
            if (sdfModelCheckBounds((const f32 *)bounds) != 0) {
                continue;
            }
        }
        SDF_VU0_GET_ROW3();
        SDF_VU0_GETVECTOR_W(zValue);
        color = rootColor;
        if (node->flags & SDF_MODEL_NODE_FLAG_LOCALCOLOR) {
            color = node->color;
        }
        attr = drawAttr & node->drawAttr;
        for (j = 0; j != 4; j++) {
            tags[j] = NULL;
        }
        sdfCoreDrawSetZBias(aRoot->zBias);
        semiTrans = 0;
        if (color < 0) {
            semiTrans = 1;
        }
        parmTag = ((u64)(attr | softenParm) << 32) | 0x6102C000;
        do {
            j = header->otIndex;
            if ((j >> 4) != aRoot->lod) {
                continue;
            }
            j &= 0x0F;
            j = j == 0 ? semiTrans == 0 : j;
            tag = tags[j];
            if (tag == NULL) {
                sdfModelNodeDrawPacket_t *buf;

                tag = sdfPrimAllocDrawTag(NULL);
                *(f32 *)&tag->z = zValue;
                tag->lightPacket = EE_ADDR(light);
                tags[j] = tag;
                tag->fogPacket = EE_ADDR(fog);
                buf = sdfGrAllocPacketBuff(sizeof(*buf));
                sdfModelSetTmRefPacket(node, &buf->tmRef, frame);
                *(u64 *)&buf->parm.next = 0x20000002;
                *(u64 *)&buf->parm.next.dat0 = parmTag;
                buf->parm.unpackColor = 0x6E01C002;
                buf->parm.color = color;
                buf->parm.unpackParm = 0x6003C003;
                *(u32 *)&buf->parm.zBias = *(u32 *)&aRoot->zBias;
                buf->parm.faceDivSoften = soften;
                buf->parm.faceDivBias = bias;
                buf->parm.itop = 0x04000004;
                buf->parm.mscal = 0x14000000;
                sdfDrawTagLinkRef(tag, EE_ADDR(buf));
            }
            switch (header->type) {
            case MODEL_PACKET_HEADER_TYPE_NORMAL:
                if (reverse == 0) {
                    sdfDmaTag_t *ref = sdfGrAllocPacketBuff(0x20);

                    QUAD_ZERO(*(u128 *)&ref[1]);
                    ref->qwc = 0;
                    ref->addr = (void *)EE_PHYSICAL(EE_ADDR(header + 1));
                    *(u64 *)&ref->dat0 = 0;
                    sdfDrawTagLinkCall(tag, EE_ADDR(ref));
                }
                break;
            case MODEL_PACKET_HEADER_TYPE_SHAPE:
                if (reverse == 0) {
                    sdfModelLinkShapedDrawPacket(node, header->dataPtr, tag, frame);
                }
                break;
            case MODEL_PACKET_HEADER_TYPE_COREDRAW: {
                void *gen;

                sdfCoreDrawSetLight(light);
                sdfCoreDrawSetFog(fog);
                gen = sdfCorePacketGen(node, header->dataPtr, attr, color, soften, bias, frame,
                                       reverse);
                if (gen != NULL) {
                    sdfDrawTagLink(tag, EE_ADDR(gen));
                }
                break;
            }
            }
        } while ((header = header->next) != NULL);
        j = 0;
        do {
            tag = tags[j];
            if (tag != NULL) {
                sdfGrOt_t **slot = aOt + j;
                (*slot)->regFunc(*slot, tag);
                drew = 1;
            }
            tags[j] = NULL;
            j++;
        } while (j != 4);
    }
    if (drew != 0) {
        aRoot->flags |= SDF_MODEL_FLAG_DREW;
    }
}

#ifdef NON_MATCHING
extern void *memcpy(void *a, const void *b, size_t c);
#endif

void sdfModelMorphListCopyStat(sdfModelMorphList_t *aDst, sdfModelMorphList_t *aSrc)
{
    s16 count;

    if (aDst == NULL) {
        return;
    }
    count = aDst->count;
    if (count > 0) {
        memcpy(aDst->items, aSrc->items, count * 16);
    }
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelCopyStat(sdfModelRootNode_t *aDstRoot, sdfModelRootNode_t *aSrcRoot)
{
    s32 frame;
    s32 count;
    sdfModelNode_t **dstItems;
    sdfModelNode_t **srcItems;
    s32 i;

    sdfMatListCopyStat(aDstRoot->matList, aSrcRoot->matList);
    sdfModelMorphListCopyStat(aDstRoot->morphList, aSrcRoot->morphList);
    aDstRoot->flags = aSrcRoot->flags;
    aDstRoot->drawAttr = aSrcRoot->drawAttr;
    aDstRoot->color = aSrcRoot->color;
    SDF_VU0_LOADMATRIX(&aSrcRoot->tm);
    SDF_VU0_STOREMATRIX(&aDstRoot->tm);
    SDF_VU0_COPYVECTOR(&aDstRoot->rotate, &aSrcRoot->rotate);
    SDF_VU0_COPYVECTOR(&aDstRoot->scale, &aSrcRoot->scale);
    frame = (s8)sdfFrameId;
    *(u32 *)&aDstRoot->light = *(u32 *)&aSrcRoot->light;
    aDstRoot->faceDivSoften = aSrcRoot->faceDivSoften;
    *(u32 *)&aDstRoot->fog = *(u32 *)&aSrcRoot->fog;
    aDstRoot->faceDivBias = aSrcRoot->faceDivBias;
    count = aDstRoot->nodeList->count;
    dstItems = aDstRoot->nodeList->items;
    srcItems = aSrcRoot->nodeList->items;
    for (i = 0; i < count; i++) {
        sdfModelNode_t *src = srcItems[i];
        sdfModelNode_t *dst = dstItems[i];

        dst->flags = src->flags;
        dst->color = src->color;
        dst->drawAttr = src->drawAttr;
        SDF_VU0_COPYVECTOR(&dst->rotate, &src->rotate);
        SDF_VU0_COPYVECTOR(&dst->pos, &src->pos);
        SDF_VU0_COPYVECTOR(&dst->scale, &src->scale);
        SDF_VU0_LOADMATRIX(&src->localNoScaledTm);
        SDF_VU0_STOREMATRIX(&dst->localNoScaledTm);
        SDF_VU0_LOADMATRIX(&src->tm);
        SDF_VU0_STOREMATRIX(&dst->tm);
        if (dst->tmPacket != NULL) {
            *(sdfModelTmImage_t *)((u8 *)dst->tmPacket + (frame << 7)) =
                *(sdfModelTmImage_t *)((u8 *)src->tmPacket + (frame << 7));
        }
    }
}

void sdfModelSetDrawEnv(sdfModelDrawEnv_t *aEnv)
{
    sdfModelDrawEnvSet_t *set = aEnv->set;
    s32 i;

    sdfPrimSetOpaquePacket(&aEnv->set[0].transPacket);
    sdfPrimSetSemiTransPacket(&aEnv->set[1].transPacket);
    sdfPrimSetAddTransPacket(&aEnv->set[2].transPacket);
    sdfPrimSetSubTransPacket(&aEnv->set[3].transPacket);
    for (i = 0; i != 4; i++) {
        set->transPacket.header.dmatag.dat0 = 0x11000000;
        sdfDrawTagInit(&set->drawTag);
        sdfDrawTagLink(&set->drawTag, EE_ADDR(&set->transPacket));
        set++;
    }
    sdfDrawTagInit(&aEnv->term.drawTag);
    *(u64 *)&aEnv->term.footer = 0;
    *(u64 *)&aEnv->term.footer.dat0 = 0x13000000;
    sdfDrawTagLink(&aEnv->term.drawTag, EE_ADDR(&aEnv->term.footer));
}

void sdfModelDrawEnvLinkOt(sdfGrOt_t **aOt, sdfModelDrawEnv_t *aEnv)
{
    s32 i;

    for (i = 0; i != 4; i++) {
        ((void (*)(sdfGrOt_t *, s32, sdfDrawTag_t *))aOt[i]->callbackFunc)(aOt[i], 1,
                                                                          &aEnv->set[i].drawTag);
    }
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))aOt[3]->regFunc)(aOt[3], &aEnv->term.drawTag);
}

void *func_002B2628(void *data, u32 chunkId) {
    sdfModelChunkHeader_t *chunk = data;
    u32 currentId;

    if (chunk == NULL) {
        return NULL;
    }
    currentId = chunk->tag;
    while (currentId != 0) {
        if (currentId == chunkId) {
            return chunk;
        }
        chunk = (sdfModelChunkHeader_t *)((u8 *)chunk + chunk->size);
        currentId = chunk->tag;
    }
    return NULL;
}

void *sdfModelGetExtraChunk(sdfModelRootNode_t *aRoot, u32 aChunkId)
{
    return func_002B2628(aRoot->extraData, aChunkId);
}

#ifdef NON_MATCHING
extern s32 memcmp(const void *first, const void *second, size_t size);
extern u32 strlen(const char *text);
#endif

s32 sdfModelGetNodeIndexByName(sdfModelRootNode_t *aRoot, const char *aName)
{
    sdfModelChunkHeader_t *chunk = sdfModelGetExtraChunk(aRoot, 0x4D4E444E);
    u8 *entry;
    u8 *end;
    u32 len;

    if (chunk == NULL) {
        return -1;
    }
    entry = (u8 *)chunk + 8;
    end = (u8 *)chunk + chunk->size;
    len = strlen(aName);
    do {
        u32 entryLen = strlen((char *)entry);
        u8 *id = EE_ADDR_AS(u8 *, ((EE_ADDR((entry + entryLen + 4))) & ~3U));

        if (entryLen == len && memcmp(entry, aName, len) == 0) {
            return *(s32 *)id;
        }
        entry = id + 4;
    } while (entry < end);
    return -1;
}

const char *sdfModelGetNodeName(sdfModelRootNode_t *aRoot, s32 aIndex) {
    sdfModelChunkHeader_t *chunk = sdfModelGetExtraChunk(aRoot, 0x4D4E444E);
    const u8 *name;
    const u8 *end;
    if (chunk == NULL) {
        return sdfModelDefaultName;
    }
    name = (const u8 *)(chunk + 1);
    end = (const u8 *)chunk + chunk->size;
    do {
        const u8 *cursor = name;
        const s32 *id;
        while (*cursor++ != 0) {
        }
        id = EE_ADDR_AS(const s32 *, ((EE_ADDR(cursor) + 3) & ~3U));
        if (*id == aIndex) {
            return (const char *)name;
        }
        name = (const u8 *)(id + 1);
        if (name >= end) {
            return sdfModelDefaultName;
        }
    } while (1);
}

u32 sdfModelGetMarkPosCount(sdfModelRootNode_t *aRoot)
{
    const sdfModelChunkHeader_t *chunk = sdfModelGetExtraChunk(aRoot, 0x534F504D);

    if (chunk != NULL) {
        return (chunk->size - 0x10) >> 6;
    }
    return 0;
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeMarkPosMatrix(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark) {
    sdfModelNode_t *drawNode = sdfModelSearchNode(aRoot, aMark->nodeId);
    u8 *vector;

    vector = (u8 *)&aMark->dir;
    SDF_VU0_LOADVECTOR(vector);
    SDF_VU0_SET_ROW2();
    SDF_VU0_COPYVECTOR_A_TO_B();
    vector = (u8 *)&aMark->up;
    SDF_VU0_LOADVECTOR(vector);
    SDF_VU0_NEGVECTOR_XYZ();
    SDF_VU0_SET_ROW1();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW0();
    SDF_VU0_LOAD_ROW3(&aMark->pos);
    SDF_VU0_MATRIX_ROW3_W_SET_ONE();
    MultMatrixRev(&drawNode->tm);
}

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeMarkPos(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark) {
    sdfModelNode_t *drawNode = sdfModelSearchNode(aRoot, aMark->nodeId);

    SDF_VU0_LOADMATRIX(&drawNode->tm);
    SDF_VU0_LOADVECTOR(&aMark->pos);
    SDF_VU0_APPLYMATRIX_XYZ1();
}

sdfModelMarkPos_t *sdfModelSearchMarkPosData(sdfModelRootNode_t *aRoot, s32 aId)
{
    sdfModelChunkHeader_t *chunk = sdfModelGetExtraChunk(aRoot, 0x534F504D);
    sdfModelMarkPos_t *data;
    u8 *end;

    if (chunk == NULL) {
        return NULL;
    }
    data = (sdfModelMarkPos_t *)((u8 *)chunk + 0x10);
    end = (u8 *)chunk + chunk->size;
    while ((u8 *)data < end) {
        if (data->id == aId) {
            return data;
        }
        data++;
    }
    return NULL;
}

s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *aRoot, s32 aId)
{
    sdfModelMarkPos_t *data = sdfModelSearchMarkPosData(aRoot, aId);

    if (data != NULL) {
        sdfModelMakeMarkPosMatrix(aRoot, data);
        return 1;
    }
    return 0;
}

s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, s32 aId)
{
    sdfModelMarkPos_t *data = sdfModelSearchMarkPosData(aRoot, aId);

    if (data != NULL) {
        sdfModelMakeMarkPos(aRoot, data);
        return 1;
    }
    return 0;
}

u32 sdfGetUniqueChunkValue(sdfModelRootNode_t *aRoot)
{
    u32 *chunk = sdfModelGetExtraChunk(aRoot, 0x51494E55); /* "UNIQ" */

    if (chunk != NULL) {
        return chunk[2];
    }
    return 0;
}

u32 sdfGetLodChunkValue(sdfModelRootNode_t *aRoot)
{
    u32 *chunk = sdfModelGetExtraChunk(aRoot, 0x43444F4C); /* "LODC" */

    if (chunk != NULL) {
        return chunk[2];
    }
    return 0;
}

void sdfModelSetFaceDivParm(sdfModelRootNode_t *aRoot, f32 aSoften, f32 aBias)
{
    aRoot->faceDivSoften = aSoften;
    aRoot->faceDivBias = aBias;
    aRoot->flags = aRoot->flags | SDF_MODEL_FLAG_SELF_FACEDIV_PARM;
}

void sdfModelResetFaceDivParm(sdfModelRootNode_t *aRoot)
{
    aRoot->flags = aRoot->flags & ~SDF_MODEL_FLAG_SELF_FACEDIV_PARM;
}

f32 sdfModelGetFaceDivSoften(sdfModelRootNode_t *aRoot)
{
    if (aRoot->flags & SDF_MODEL_FLAG_SELF_FACEDIV_PARM) {
        return aRoot->faceDivSoften;
    }
    return sdfFaceDivSoften;
}

f32 sdfModelGetFaceDivBias(sdfModelRootNode_t *aRoot)
{
    if (aRoot->flags & SDF_MODEL_FLAG_SELF_FACEDIV_PARM) {
        return aRoot->faceDivBias;
    }
    return sdfFaceDivBias;
}

INCLUDE_SDATA(const s32, "sdf/sdfModel", corePktSoftenBias);

