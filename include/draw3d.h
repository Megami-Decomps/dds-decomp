#ifndef DRAW3D_H
#define DRAW3D_H

/* sdf3D and sdfCoreDraw: the camera, light and fog packets VU1 shades with,
 * the per-node transform packet, and the generated-geometry path that turns a
 * mesh into GS primitives on VU0.
 *
 * sdf_lighting.h owns the parallel-light packet; sdf3DFogPacket_t is opaque
 * in libsdf.h so sdfDrawTag_t can point at it.
 * See include/README.md for the evidence per struct. */

#include "common.h"
#include "libsdf.h"
#include "sdf_lighting.h"

typedef struct sdf3DFogPacket_t sdf3DFogPacket_t;

/* The camera, as sdf3DMakeScreenParam derives it and sdf3DSetCamera copies it
 * into the global sdf3DScreenParam. The first twelve fields are the inputs;
 * TM, scale and trans are what it computes, and cameraPos comes from
 * sdf3DMakeViewportPacket inverting the camera matrix. */
typedef struct
{
    u32 flags; /* sdf_3d_screen_flag */
    f32 aspect;
    f32 projDist; /* unused when FOVY is set */
    f32 fovy;
    f32 ox;
    f32 oy;
    f32 width;
    f32 height;
    f32 zMin;
    f32 zMax;
    f32 nearClip;
    f32 farClip;
    sdfMatrix4_t TM;
    sdfVector4_t scale;
    sdfVector4_t trans;
    sdfVector4_t cameraPos;
} sdf3DScreenParam_t;

/* sdf3DScreenParam_t.flags, as sdf3DMakeScreenParam tests them. */
enum sdf_3d_screen_flag
{
    SDF_3D_SCREEN_FLAG_HALFHEIGHT = 1, /* interlaced: halve the Y scale */
    SDF_3D_SCREEN_FLAG_FOVY = 2        /* derive projDist from fovy */
};

/* What sdf3DMakeViewportPacket hands VU1: the camera matrix, the projection,
 * and the clip-space to GS mapping, uploaded by the UNPACK in the DMA tag's
 * upper words. */
typedef struct QWORD_ALIGNED
{
    sdfDmaTag_t dmatag;
    sdfMatrix4_t cameraTm;
    sdfMatrix4_t TM;
    sdfVector4_t scale;
    sdfVector4_t trans;
    sdfVector4_t cameraPos;
    u32 itop;
    u32 mscal;
    u32 pad[2];
} sdf3DViewportPacket_t;

/* One object's transform for VU1: the world matrix, then the three rows
 * sdf3DPrimSetTmPacket derives for normals, then the VIF codes that run the
 * transform. sdfModelNode_t.tmPacket is two of these, one per frame buffer. */
typedef struct QWORD_ALIGNED
{
    sdfVector4_t tm[7];
    u32 cmd[4];
} sdf3DTmPacketData_t;

/* What sdf3DMakeFogPacket is handed: the two fog densities and the two
 * distances they span, plus the colour. */
typedef struct
{
    f32 fogMax;
    f32 fogMin;
    f32 fogNear;
    f32 fogFar;
    u32 color;
} sdf3DFogParam_t;

/* The fog packet: four terms for VU1, then one GIF A+D write that sets FOGCOL
 * on the GS directly. */
struct sdf3DFogPacket_t
{
    sdfDmaTag_t dmatag;
    /* Minimum, maximum, the midpoint VU1 works from, and the depth scale. */
    f32 fogParm[4];
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t fogColor;
    u32 mscal;
    u32 pad[3];
};

/* sdf3DLinkInitPacket: a REF to vu1MicroPacket, then the tag that continues
 * the chain. */
typedef struct QWORD_ALIGNED
{
    sdfDmaTag_t reftag;
    sdfDmaTag_t linktag;
} sdf3DInitPacket_t;



/* A generated draw packet's header, as sdfModelLinkDrawPacket dispatches on it
 * and sdfCorePacketGen reads it. Same first eight bytes as
 * sdfShapePacket_t (model.h); the prim header follows inline. */
typedef struct
{
    u8 packetType; /* PACKET_TYPE_e 4 through 8 */
    u8 mpgType;    /* unconfirmed */
    s16 priority;  /* unconfirmed */
    u16 size;      /* unconfirmed */
    u16 matIndex;
    /* extData[0] holds the blend-shape count in its low half and the morph
     * entry in its high half; nothing reads extData[1]. */
    u32 extData[2];
} sdfCorePacketHeader_t;

/* The counts and flags corePktInitCb parses out of a packet. attrBits is the
 * GS PRIM word with the engine's own pass flags above it; packetBits says
 * which arrays the body that follows carries. */
typedef struct
{
    u16 faceCount;
    u16 pointCount;
    u16 attrBits;
    u16 packetBits;
} sdfCorePrimHeader_t;

/* The whole 24-byte header. The mesh body starts straight after it, which is
 * where corePktInitCb points the draw state's payload. */
typedef struct
{
    sdfCorePacketHeader_t header;
    sdfCorePrimHeader_t prim;
} sdfCorePacket_t;

/* sdfCorePrimHeader_t.attrBits. The low bits are PRIM's own; the pass bits are
 * what corePktEmitAllPasses and corePktOpenAndEmit branch on. */
enum SDF_ATTRBIT_e
{
    SDF_ATTRBIT_SPECULAR = 0x0001,
    SDF_ATTRBIT_REFMAP = 0x0002,
    SDF_ATTRBIT_LUSTERMAP = 0x0004,
    SDF_ATTRBIT_SMOOTHSHADING = 0x0008,
    SDF_ATTRBIT_TEXMAP = 0x0010,
    SDF_ATTRBIT_FOG = 0x0020,
    SDF_ATTRBIT_ALPHABLEND = 0x0040,
    SDF_ATTRBIT_ANTIALIAS = 0x0080,
    SDF_ATTRBIT_DIRECT_UV = 0x0100,
    SDF_ATTRBIT_CONTEXT = 0x0200,
    SDF_ATTRBIT_FIX = 0x0400,
    SDF_ATTRBIT_VERTCOL = 0x0800,
    SDF_ATTRBIT_MULTITEX = 0x1000,
    SDF_ATTRBIT_SINGLESIDE = 0x2000,
    SDF_ATTRBIT_DOUBLESIDE = 0x4000
};

/* sdfCorePrimHeader_t.packetBits, as sdfCorePacketGen walks the body. */
enum SDF_PACKETBIT_e
{
    SDF_PACKETBIT_FACE = 0x0020,
    SDF_PACKETBIT_VERTEX = 0x0040,
    SDF_PACKETBIT_NORMAL = 0x0080,
    SDF_PACKETBIT_ST = 0x0100,
    SDF_PACKETBIT_VERTCOL = 0x0200,
    SDF_PACKETBIT_CLRBUFF = 0x0400,
    SDF_PACKETBIT_BLENDVERT = 0x0800,
    SDF_PACKETBIT_BLENDGRP = 0x1000,
    SDF_PACKETBIT_NODRAW = 0x2000,
    SDF_PACKETBIT_STRIP = 0x4000
};

/* The draw state sdfCorePacketGen keeps on its stack and every corePkt*
 * routine works through. Every offset from tmSave to backfaceSign is proven from
 * code. The size is 0xB0: sdfCorePacketGen's first spill slot sits at 0xB0, so
 * the block ends there once the 16-byte round-up is applied. The two words at
 * 0xB0 and 0xB4 that earlier sat here are locals of that function.
 * See include/README.md. */
typedef struct QWORD_ALIGNED
{
    /* The composed world-to-screen matrix, kept here so corePktBuildFaceList
     * and corePktEmitTexTris can lq it straight from offset 0. */
    sdfMatrix4_t tmSave;
    s16 faceCount;
    s16 pointCount;
    s32 attrBits; /* masked by the draw's own attr mask */
    s32 packetBits;
    s32 orgAttrBits; /* as the packet carries it, before the mask */
    s16 drawFaceCount;
    s16 backfaceCount;
    sdfMatPacket_t *matPacketPtr;
    u32 color;
    f32 fdSoften;
    f32 fdBias;
    /* The vertex working set corePktSetupPointBuff lays out from the vertex
     * count: the VU0 scratchpad up to 128 vertices and a spill in
     * corePktPointBuff past that. Which of the three pointers and two counts
     * is which is from the HD names, not from the code. */
    void *pointBuff;
    void *freePointBuff;
    void *extPointBuff;
    s32 freePointCount;
    s32 extPointCount;
    void *extPointTop;
    void *extPointLink; /* set to &extPointTop */
    /* The output packet, from sdfGrOpenPacketBuff: where it starts, where the
     * closing tag goes, and the VU1 address before and after the emit, which
     * is how corePktClosePacket knows whether anything was emitted. */
    void *packetOpenPtr;
    void *packetTopPtr;
    void *packetStartPtr;
    void *packetPtr;
    void *dataPtr; /* the mesh body, straight after sdfCorePacket_t */
    void *facePtr;
    void *vertexPtr; /* unconfirmed; the non-skinned path keeps it in a register */
    void *normalPtr;
    void *uvPtr;
    void *uv2Ptr;
    void *vertColPtr;
    f32 backfaceSign; /* -1 when the draw is reversed, else 1 */
} corePktCb_t;

/* sdfCorePacketGen's own threshold for a blend shape being worth applying. */
#define SDF_CORE_MORPH_EPSILON 0.000001f

#endif /* DRAW3D_H */
