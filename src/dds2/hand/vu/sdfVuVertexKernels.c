#include "common.h"

#ifdef NON_MATCHING
#include "common.h"

/*
 * Retail's hand-written VU0 vertex kernels (hand/vu/sdfVuVertexKernels.s),
 * which fill the model work buffer's 0x60-byte vertex records: position at
 * 0x00, normal at 0x10, colour at 0x20, texture coordinates at 0x30 and clip
 * bytes at 0x50/0x51. Each loop body is retail's own instruction sequence,
 * run on the EE interpreter in retail's order, so VU0 rounding, the
 * accumulator, Q and the clip flags behave as on the PS2; C runs the loop
 * and advances the pointers. Retail software-pipelines the loops (loading
 * the next vertex before storing this one, to other addresses), which does
 * not change the results.
 *
 * Callers set vf1 = (1, 1, 1, -1) and the object basis rows vf24..vf26 (and
 * vf28..vf31 where a kernel does not load them); the lighting kernels load
 * the model's colour transform, sdfModelColorTransform, as vf19..vf21 and
 * scale the rows' w by its first row.
 */

extern u128 sdfModelColorTransform[3];

#define SDF_VU_VERTEX_BYTES 0x60

/* $3 = a packed xyz at p as (x, y, z, z's sign): ldr/ldl the first two
 * words, lw (sign-extending) the third, pcpyld them together. */
#define SDF_VU_LOAD_XYZ(p) \
    "ldr $2, 0(" p ")\n\tldl $2, 7(" p ")\n\tlw $3, 8(" p ")\n\tpcpyld $3, $3, $2\n\t"

/* A float's bits, for qmtc2 (retail moves the weight through mfc1; only
 * lane x of the result is used). */
static u32 sdfVuFloatBits(f32 value) {
    union {
        f32 f;
        u32 u;
    } bits = {value};
    return bits.u;
}

/* vf19..vf21 = the light; the basis rows' w = light.w * row.x. */
static void sdfVuLoadLight(void) {
    __asm__ volatile(
        "lqc2 vf19, 0(%0)\n\tlqc2 vf20, 16(%0)\n\tlqc2 vf21, 32(%0)\n\t"
        "vmulx.w vf24, vf19, vf24x\n\tvmulx.w vf25, vf19, vf25x\n\tvmulx.w vf26, vf19, vf26x"
        : : "r"(sdfModelColorTransform));
}

/* vf16..vf18 = a texture transform's three 64-bit rows. */
static void sdfVuLoadUvTransform(const u8 *rows) {
    __asm__ volatile(
        "ld $2, 0(%0)\n\tqmtc2.ni $2, vf16\n\tld $3, 8(%0)\n\tqmtc2.ni $3, vf17\n\tld $2, 16(%0)\n\tqmtc2.ni $2, vf18"
        : : "r"(rows) : "$2", "$3");
}

/* Lit vertex from vf2 (position) and vf6 (normal): position, normal (w = the
 * light's 2d - 1), colour and clip bytes to out. With uv, vf4's coordinates
 * through vf16..vf18 to out + 0x30. Retail's sequence of corePktXfmLightClipPB and,
 * with uv, corePktXfmLightClipUVPB. */
static void sdfVuLightVertex(u8 *out, int uv) {
    if (uv) {
        __asm__ volatile(
            "vmulay.w ACC, vf0, vf19y\n\tvmaddax.w ACC, vf24, vf6x\n\tvmadday.w ACC, vf25, vf6y\n\t"
            "vmaddz.w vf7, vf26, vf6z\n\t"
            "vmulax.xyzw ACC, vf28, vf2x\n\tvmadday.xyzw ACC, vf29, vf2y\n\tvmaddaz.xyzw ACC, vf30, vf2z\n\t"
            "vmaddw.xyzw vf3, vf31, vf0w\n\t"
            "vaddaw.w ACC, vf7, vf7w\n\tvmsubw.w vf6, vf0, vf0w\n\tvmaxx.w vf8, vf7, vf0x\n\t"
            "vclipw.xyzw vf3, vf3w\n\t"
            "vmulax.xy ACC, vf16, vf4x\n\tvaddw.x vf9, vf0, vf6w\n\tvmadday.xy ACC, vf17, vf4y\n\t"
            "vminiw.w vf8, vf8, vf0w\n\tvmaddw.xy vf5, vf18, vf0w\n\tcfc2.ni $2, $vi18\n\t"
            "vclipw.xyzw vf9, vf0w\n\tvmulaw.xyzw ACC, vf20, vf0w\n\tvmaddw.xyzw vf8, vf21, vf8w\n\t"
            "sqc2 vf3, 0(%0)\n\tsqc2 vf6, 16(%0)\n\tsqc2 vf5, 48(%0)\n\tsqc2 vf8, 32(%0)\n\tsb $2, 81(%0)\n\t"
            "cfc2.ni $2, $vi18\n\tandi $2, $2, 3\n\tsb $2, 80(%0)"
            : : "r"(out) : "$2", "memory");
    } else {
        __asm__ volatile(
            "vmulay.w ACC, vf0, vf19y\n\tvmaddax.w ACC, vf24, vf6x\n\tvmadday.w ACC, vf25, vf6y\n\t"
            "vmaddz.w vf7, vf26, vf6z\n\t"
            "vmulax.xyzw ACC, vf28, vf2x\n\tvmadday.xyzw ACC, vf29, vf2y\n\tvmaddaz.xyzw ACC, vf30, vf2z\n\t"
            "vmaddw.xyzw vf3, vf31, vf0w\n\t"
            "vaddaw.w ACC, vf7, vf7w\n\tvmsubw.w vf6, vf0, vf0w\n\tvmaxx.w vf8, vf7, vf0x\n\t"
            "vclipw.xyzw vf3, vf3w\n\tsqc2 vf3, 0(%0)\n\t"
            "vaddw.x vf9, vf0, vf6w\n\tvminiw.w vf8, vf8, vf0w\n\tsqc2 vf6, 16(%0)\n\t"
            "vmulaw.xyzw ACC, vf20, vf0w\n\tcfc2.ni $2, $vi18\n\tvmaddw.xyzw vf8, vf21, vf8w\n\t"
            "vclipw.xyzw vf9, vf0w\n\tvnop\n\tvnop\n\t"
            "sqc2 vf8, 32(%0)\n\tsb $2, 81(%0)\n\tcfc2.ni $2, $vi18\n\tandi $2, $2, 3\n\tsb $2, 80(%0)"
            : : "r"(out) : "$2", "memory");
    }
}

/* Skin by the VU0 microprogram at 0: vf28..vf31 and vf24..vf26 from
 * matrices; each position (16 bytes) and packed normal through it, to the
 * record's position and normal. */
void corePktSkinPointsFirst(u8 *out, s32 count, const u8 *matrices, const u8 *positions, const u8 *normals) {
    __asm__ volatile(
        "lqc2 vf28, 0(%0)\n\tlqc2 vf29, 16(%0)\n\tlqc2 vf30, 32(%0)\n\tlqc2 vf31, 48(%0)\n\t"
        "lqc2 vf24, 64(%0)\n\tlqc2 vf25, 80(%0)\n\tlqc2 vf26, 96(%0)"
        : : "r"(matrices));
    do {
        __asm__ volatile(
            SDF_VU_LOAD_XYZ("%2") "lq $2, 0(%1)\n\t"
            "qmtc2.ni $2, vf2\n\tqmtc2.ni $3, vf3\n\tvcallms 0x0\n\t"
            "qmfc2.i $4, vf7\n\tsqc2 vf4, 0(%0)\n\tsq $4, 16(%0)"
            : : "r"(out), "r"(positions), "r"(normals) : "$2", "$3", "$4", "memory");
        out += SDF_VU_VERTEX_BYTES;
        positions += 16;
        normals += 12;
    } while (--count != 0);
}

/* Skin a further influence by the VU0 microprogram at 0x48, which blends
 * it into the record's position and normal (vf5, vf6 in; vf4, vf3 out).
 * Retail's loop needs at least two vertices. */
void corePktSkinPointsAccum(u8 *out, s32 count, const u8 *matrices, const u8 *positions, const u8 *normals) {
    __asm__ volatile(
        "lqc2 vf28, 0(%0)\n\tlqc2 vf29, 16(%0)\n\tlqc2 vf30, 32(%0)\n\tlqc2 vf31, 48(%0)\n\t"
        "lqc2 vf24, 64(%0)\n\tlqc2 vf25, 80(%0)\n\tlqc2 vf26, 96(%0)"
        : : "r"(matrices));
    do {
        __asm__ volatile(
            SDF_VU_LOAD_XYZ("%2") "lq $2, 0(%1)\n\tlq $4, 0(%0)\n\tlq $5, 16(%0)\n\t"
            "qmtc2.ni $2, vf2\n\tqmtc2.ni $3, vf3\n\tqmtc2.ni $4, vf5\n\tqmtc2.ni $5, vf6\n\tvcallms 0x48\n\t"
            "qmfc2.i $6, vf4\n\tsqc2 vf3, 16(%0)\n\tsq $6, 0(%0)"
            : : "r"(out), "r"(positions), "r"(normals) : "$2", "$3", "$4", "$5", "$6", "memory");
        out += SDF_VU_VERTEX_BYTES;
        positions += 16;
        normals += 12;
    } while (--count != 0);
}

/* Retail's loop tests the output pointer instead of the count, and stores
 * through the vertex pointer. Preserve both of these apparent mistakes. */
void corePktSkinVertFirst(u8 *out, s32 count, const u8 *matrix, const u8 *vertices) {
    u32 outputAddress = (u32)out;
    (void)count;
    __asm__ volatile(
        "lqc2 vf28, 0(%0)\n\tlqc2 vf29, 16(%0)\n\t"
        "lqc2 vf30, 32(%0)\n\tlqc2 vf31, 48(%0)"
        : : "r"(matrix));
    do {
        __asm__ volatile(
            "ldr $2, 0(%0)\n\tldl $2, 7(%0)\n\t"
            "ldr $3, 8(%0)\n\tldl $3, 15(%0)\n\tpcpyld $2, $3, $2\n\t"
            "qmtc2.ni $2, vf2\n\tvmulax.xyzw ACC, vf28, vf2x\n\t"
            "vmadday.xyzw ACC, vf29, vf2y\n\tvmaddaz.xyzw ACC, vf30, vf2z\n\t"
            "vmaddw.xyzw vf4, vf31, vf0w\n\tvmulw.xyzw vf4, vf4, vf2w"
            : : "r"(vertices) : "$2", "$3");
        vertices += 16;
        outputAddress += 0x60;
        /* Retail stores through the advanced input, not the output pointer. */
        __asm__ volatile("sqc2 vf4, -0x60(%0)" : : "r"(vertices) : "memory");
    } while (outputAddress != 0);
}

/* Add each vertex (16 bytes, unaligned) through vf28..vf31, weighted by its
 * w, to the record's position. */
void corePktSkinVertAccum(u8 *out, s32 count, const u8 *matrix, const u8 *vertices) {
    __asm__ volatile(
        "lqc2 vf28, 0(%0)\n\tlqc2 vf29, 16(%0)\n\tlqc2 vf30, 32(%0)\n\tlqc2 vf31, 48(%0)"
        : : "r"(matrix));
    do {
        __asm__ volatile(
            "ldr $2, 0(%1)\n\tldl $2, 7(%1)\n\tldr $3, 8(%1)\n\tldl $3, 15(%1)\n\tpcpyld $2, $3, $2\n\t"
            "qmtc2.ni $2, vf2\n\tlqc2 vf5, 0(%0)\n\t"
            "vmulax.xyzw ACC, vf28, vf2x\n\tvmadday.xyzw ACC, vf29, vf2y\n\tvmaddaz.xyzw ACC, vf30, vf2z\n\t"
            "vmaddw.xyzw vf4, vf31, vf0w\n\tvmulaw.xyzw ACC, vf5, vf0w\n\tvmaddw.xyzw vf4, vf4, vf2w\n\t"
            "sqc2 vf4, 0(%0)"
            : : "r"(out), "r"(vertices) : "$2", "$3", "memory");
        out += SDF_VU_VERTEX_BYTES;
        vertices += 16;
    } while (--count != 0);
}

/* The record's position from packed positions. */
void corePktLoadVerts(u8 *out, s32 count, const u8 *positions) {
    do {
        __asm__ volatile(SDF_VU_LOAD_XYZ("%1") "sq $3, 0(%0)"
                         : : "r"(out), "r"(positions) : "$2", "$3", "memory");
        out += SDF_VU_VERTEX_BYTES;
        positions += 12;
    } while (--count != 0);
}

/* The record's position and normal from packed positions and normals. */
void corePktLoadVertsNorms(u8 *out, s32 count, const u8 *positions, const u8 *normals) {
    do {
        __asm__ volatile(SDF_VU_LOAD_XYZ("%1") "sq $3, 0(%0)\n\t" SDF_VU_LOAD_XYZ("%2") "sq $3, 16(%0)"
                         : : "r"(out), "r"(positions), "r"(normals) : "$2", "$3", "memory");
        out += SDF_VU_VERTEX_BYTES;
        positions += 12;
        normals += 12;
    } while (--count != 0);
}

/* Add weighted packed positions and normals to the record's (xyz). */
void corePktMorphVertsNorms(u8 *out, s32 count, const u8 *positions, const u8 *normals, f32 weight) {
    __asm__ volatile("qmtc2.ni %0, vf6" : : "r"(sdfVuFloatBits(weight)));
    do {
        __asm__ volatile(
            SDF_VU_LOAD_XYZ("%1") "qmtc2.ni $3, vf2\n\tlqc2 vf4, 0(%0)\n\t"
            SDF_VU_LOAD_XYZ("%2") "qmtc2.ni $3, vf3\n\tlqc2 vf5, 16(%0)\n\t"
            "vmulaw.xyz ACC, vf4, vf0w\n\tvmaddx.xyz vf2, vf2, vf6x\n\t"
            "vmulaw.xyz ACC, vf5, vf0w\n\tvmaddx.xyz vf3, vf3, vf6x\n\t"
            "sqc2 vf2, 0(%0)\n\tsqc2 vf3, 16(%0)"
            : : "r"(out), "r"(positions), "r"(normals) : "$2", "$3", "memory");
        out += SDF_VU_VERTEX_BYTES;
        positions += 12;
        normals += 12;
    } while (--count != 0);
}

/* Add weighted packed positions to the record's (xyz). */
void corePktMorphVerts(u8 *out, s32 count, const u8 *positions, f32 weight) {
    __asm__ volatile("qmtc2.ni %0, vf6" : : "r"(sdfVuFloatBits(weight)));
    do {
        __asm__ volatile(
            "lqc2 vf4, 0(%0)\n\t" SDF_VU_LOAD_XYZ("%1") "qmtc2.ni $3, vf2\n\t"
            "vmulaw.xyz ACC, vf4, vf0w\n\tvmaddx.xyz vf2, vf2, vf6x\n\tsqc2 vf2, 0(%0)"
            : : "r"(out), "r"(positions) : "$2", "$3", "memory");
        out += SDF_VU_VERTEX_BYTES;
        positions += 12;
    } while (--count != 0);
}

/* Normalize the record's normal (xyz). */
void corePktNormalizeNorms(u8 *out, s32 count) {
    do {
        __asm__ volatile(
            "lqc2 vf2, 16(%0)\n\tvmul.xyz vf3, vf2, vf2\n\tvadday.x ACC, vf3, vf3y\n\t"
            "vmaddz.x vf4, vf1, vf3z\n\tvrsqrt Q, vf0w, vf4x\n\tvwaitq\n\tvmulq.xyz vf2, vf2, Q\n\t"
            "sqc2 vf2, 16(%0)"
            : : "r"(out) : "memory");
        out += SDF_VU_VERTEX_BYTES;
    } while (--count != 0);
}

/* Light packed positions and normals into the records. */
void corePktXfmLightClip(u8 *out, s32 count, const u8 *positions, const u8 *normals) {
    sdfVuLoadLight();
    do {
        __asm__ volatile(SDF_VU_LOAD_XYZ("%1") "qmtc2.ni $3, vf6\n\t" SDF_VU_LOAD_XYZ("%0") "qmtc2.ni $3, vf2"
                         : : "r"(positions), "r"(normals) : "$2", "$3");
        sdfVuLightVertex(out, 0);
        out += SDF_VU_VERTEX_BYTES;
        positions += 12;
        normals += 12;
    } while (--count != 0);
}

/* Light the records' own positions and normals. */
void corePktXfmLightClipPB(u8 *out, s32 count) {
    sdfVuLoadLight();
    do {
        __asm__ volatile("lqc2 vf6, 16(%0)\n\tlqc2 vf2, 0(%0)" : : "r"(out));
        sdfVuLightVertex(out, 0);
        out += SDF_VU_VERTEX_BYTES;
    } while (--count != 0);
}

/* Light packed positions and normals into the records, with their texture
 * coordinates through the transform. */
void corePktXfmLightClipUV(u8 *out, s32 count, const u8 *positions, const u8 *normals, const u8 *coordinates,
                   const u8 *transform) {
    sdfVuLoadLight();
    sdfVuLoadUvTransform(transform);
    do {
        __asm__ volatile(SDF_VU_LOAD_XYZ("%1") "qmtc2.ni $3, vf6\n\t" SDF_VU_LOAD_XYZ("%0") "qmtc2.ni $3, vf2\n\t"
                         "ld $2, 0(%2)\n\tqmtc2.ni $2, vf4"
                         : : "r"(positions), "r"(normals), "r"(coordinates) : "$2", "$3");
        sdfVuLightVertex(out, 1);
        out += SDF_VU_VERTEX_BYTES;
        positions += 12;
        normals += 12;
        coordinates += 8;
    } while (--count != 0);
}

/* Light the records' own positions and normals, with texture coordinates
 * through the transform. */
void corePktXfmLightClipUVPB(u8 *out, s32 count, const u8 *coordinates, const u8 *transform) {
    sdfVuLoadLight();
    sdfVuLoadUvTransform(transform);
    do {
        __asm__ volatile("lqc2 vf6, 16(%0)\n\tlqc2 vf2, 0(%0)\n\tld $2, 0(%1)\n\tqmtc2.ni $2, vf4"
                         : : "r"(out), "r"(coordinates) : "$2");
        sdfVuLightVertex(out, 1);
        out += SDF_VU_VERTEX_BYTES;
        coordinates += 8;
    } while (--count != 0);
}

/* Light the records' negated normals only: normal, colour and the second
 * clip byte. */
void corePktRelightBackFaces(u8 *out, s32 count) {
    sdfVuLoadLight();
    do {
        __asm__ volatile(
            "lqc2 vf6, 16(%0)\n\tvsub.xyz vf6, vf0, vf6\n\t"
            "vmulay.w ACC, vf0, vf19y\n\tvmaddax.w ACC, vf24, vf6x\n\tvmadday.w ACC, vf25, vf6y\n\t"
            "vmaddz.w vf7, vf26, vf6z\n\tvaddaw.w ACC, vf7, vf7w\n\tvmsubw.w vf6, vf0, vf0w\n\t"
            "vmaxx.w vf8, vf7, vf0x\n\tvaddw.x vf9, vf0, vf6w\n\tvminiw.w vf8, vf8, vf0w\n\t"
            "vclipw.xyzw vf9, vf0w\n\tsqc2 vf6, 16(%0)\n\t"
            "vmulaw.xyzw ACC, vf20, vf0w\n\tvmaddw.xyzw vf8, vf21, vf8w\n\tvnop\n\tvnop\n\tvnop\n\t"
            "cfc2.ni $2, $vi18\n\tsqc2 vf8, 32(%0)\n\tandi $2, $2, 3\n\tsb $2, 80(%0)"
            : : "r"(out) : "$2", "memory");
        out += SDF_VU_VERTEX_BYTES;
    } while (--count != 0);
}

/* The records' texture coordinates (64-bit pairs) through the transform. */
void corePktXfmUV(u8 *out, s32 count, const u8 *coordinates, const u8 *transform) {
    sdfVuLoadUvTransform(transform);
    do {
        __asm__ volatile(
            "ld $2, 0(%1)\n\tqmtc2.ni $2, vf2\n\t"
            "vmulaw.xy ACC, vf18, vf0w\n\tvmaddax.xy ACC, vf16, vf2x\n\tvmaddy.xy vf3, vf17, vf2y\n\t"
            "qmfc2.ni $2, vf3\n\tsd $2, 48(%0)"
            : : "r"(out), "r"(coordinates) : "$2", "memory");
        out += SDF_VU_VERTEX_BYTES;
        coordinates += 8;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktSkinPointsFirst);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktSkinPointsAccum);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktSkinVertFirst);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktSkinVertAccum);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktLoadVerts);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktLoadVertsNorms);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktMorphVertsNorms);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktMorphVerts);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktNormalizeNorms);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktXfmLightClip);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktXfmLightClipPB);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktXfmLightClipUV);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktXfmLightClipUVPB);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktRelightBackFaces);
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexKernels", corePktXfmUV);
#endif
