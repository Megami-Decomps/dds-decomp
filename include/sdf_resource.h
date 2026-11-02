#ifndef SDF_RESOURCE_H
#define SDF_RESOURCE_H

/* Engine resource records the packet builders need but the engine owners in
 * libsdf.h do not have to drag in with them. Same intent as sdf_types.h one
 * layer down: a unit that builds a VIF packet from a material or a TMX header
 * includes this and gets the canonical layout, without the queue, file and
 * motion definitions it has no use for.
 *
 * libsdf.h includes this, so there is exactly one definition of each record.
 * The pointed-to types stay forward declarations on purpose: a unit that also
 * carries its own copy of one of them keeps compiling. */

#include "common.h"

void *sdfDevLoadFileEx(const char *name, u32 *outData, u32 *outSize);

struct sdfMatPacket_t;
struct sdfTexPacket_t;

/* sdfMem: a doubly linked list over one arena. A block's size is the next
 * handle's ptr minus its own, so the list always ends with a sentinel. */
typedef struct sdfMemHandle_t
{
    struct sdfMemHandle_t *prev;
    struct sdfMemHandle_t *next;
    void *ptr;
    u16 stat; /* sdf_mem_stat; sdfMemGetType returns it */
    u16 mlock;
} sdfMemHandle_t;

/* GS local memory, allocated in the same list-of-slices shape as sdfMem. */
typedef struct sdfGsMemSlice_t
{
    struct sdfGsMemSlice_t *prev;
    struct sdfGsMemSlice_t *next;
    s32 type; /* 0 free */
    u32 addr;
    s32 size;
    s16 w;
    s16 h;
    /* The IDB says u32; retail loads it with lw in sdfSetLoadImage, so s32. */
    s32 pixelFormat;
} sdfGsMemSlice_t;

typedef struct sdfTexHandle_t
{
    struct sdfTexHandle_t *prev;
    struct sdfTexHandle_t *next;
    struct sdfTexHandle_t *origin; /* unconfirmed */
    s16 width;
    s16 height;
    sdfGsMemSlice_t *pixelGsHandle;
    sdfGsMemSlice_t *clutGsHandle;
    u8 numOfCLUT;
    u8 CLUTFormat;
    u8 pixelFormat;
    u8 numOfMipmap;
    u16 mipmapParam;  /* unconfirmed */
    u8 flags;         /* unconfirmed */
    u8 texWrapMode;   /* unconfirmed */
    u32 texId;        /* unconfirmed */
    u32 clutId;       /* unconfirmed */
    struct sdfTexPacket_t *packet;
    struct sdfTexPacket_t *packetCtx2; /* unconfirmed */
    void *clutData;
    u32 clutDataSize;
    u32 clutColor;
    /* The intensity table sdfTexSetMonoClut derives from the CLUT, allocated
     * on first use and written a byte per entry. */
    u8 *monoClutData;
} sdfTexHandle_t;

typedef u8 sdfTextureClampMode_t;

/* One material plus its two prebuilt VIF packets, one per texture context.
 * sdfhook gives this struct a trailing reserved[88] and 160 bytes; sdfMatNew
 * allocates 0x48, which is where the struct ends here. */
typedef struct sdfMat_t
{
    u32 id;
    u16 flags;
    u8 duty;
    u8 pad;
    struct sdfMatPacket_t *packet[2];
    u32 ambient;
    u32 diffuse;
    u32 multiTexDiffuse;
    f32 constant;
    u32 specular;
    u32 specularParm;
    u32 refmap;
    sdfTexHandle_t *texHandle;
    sdfTexHandle_t *multiTexHandle;
    u32 multiTexMethod;
    /* Pointee left untyped on purpose: libsdf.h and the IDB spell it the
     * struct sdfMatUvwParm_t, while sdf3DPrim.c and effPCPMisc.c spell the
     * same 24 bytes a union of three views. Nothing dereferences these two
     * through a material; they are stored, passed on and freed. Unifying the
     * two readings is type recovery, not a header move. */
    void *uvwParm;
    void *multiTexUvwParm;
    f32 fdBias;
    f32 fdSoften;
} sdfMat_t;

/* Native 0x40-byte TMX header; sdfTexSetEx and resource loaders consume it. */
typedef struct TMX_HEADER_t
{
    u8 fileType;
    u8 compression;
    s16 userId;
    u32 fileSize;
    u32 formatId;
    s32 realSize;
    u8 numOfCLUT;
    u8 CLUTFormat;
    s16 width;
    s16 height;
    u8 pixelFormat;
    u8 numOfMipmap;
    u16 mipmapParam;
    u8 flags;
    sdfTextureClampMode_t texWrapMode;
    /* The IDB calls 0x1C onward userArea and comment. These two are what this
     * game puts in its user area; sdfTmx reads both as words. */
    s32 resourceKey;
    s32 battleTextureSlot;
    u8 pad24[0x1C];
} TMX_HEADER_t;

/* One draw list: the CPU link plus the DMA addresses of its packet chain.
 *
 * head, tail and the three reference slots are addresses rather than typed
 * pointers, and z is an integer, because that is what the nine packet-building
 * units are compiled from: sdfDrawTagInit writes `z = 0xFFFF` as an integer,
 * which an f32 member would not reproduce. The IDB types them as pointers and
 * an f32; nothing in the tree exercises that reading, so the proven one is
 * kept here. */
typedef struct sdfDrawTag_t
{
    struct sdfDrawTag_t *next;
    u32 head;
    u32 tail;
    u32 z;
    u32 lightPacket; /* Optional first DMA reference prefix. */
    u32 fogPacket;   /* Optional second DMA reference prefix. */
    u32 texPacket[2];
} sdfDrawTag_t;

#endif /* SDF_RESOURCE_H */
