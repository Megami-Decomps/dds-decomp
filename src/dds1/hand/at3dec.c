#include "common.h"

/* Sony's ATRAC3 decoder (libat3dec.c). The NON_MATCHING C assumes 32-bit
 * wrap-around and arithmetic right shifts (-fwrapv). */

typedef struct at3decWeight_t
{
    u16 a;
    u16 b;
} at3decWeight_t;

/* Mixing weights for one frame: one pair per QMF band, the joint stereo pair
 * and the number of coded bands. */
typedef struct at3decMix_t
{
    const at3decWeight_t *band[4];
    const at3decWeight_t *matrix;
    s32 nbands;
} at3decMix_t;

/* One gain control point: location * 8 and level. Level 4 is unity. */
typedef struct at3decGainPoint_t
{
    s32 loc;
    s32 level;
} at3decGainPoint_t;

#define AT3DEC_HISTORY 46 /* samples of QMF history kept after the 1024 outputs */

typedef struct at3decChannel_t
{
    s32 band[4][0x198];               /* 0x0000: per QMF band: 23 words of history, then samples */
    u8 pad1980[0xC0];
    s32 spectrum[4][0x100];           /* 0x1A40 */
    u8 pad2A40[0x20];
    s16 pcm[1024 + AT3DEC_HISTORY];   /* 0x2A60 */
    u8 pad32BC[0x18];
    at3decGainPoint_t gain[4][8];     /* 0x32D4 */
    u8 pad33D4[0xC];
} at3decChannel_t;

typedef struct at3decWork_t
{
    at3decMix_t cur;               /* 0x0000 */
    at3decMix_t prev;              /* 0x0018 */
    at3decChannel_t chan[2];       /* 0x0030 */
    s32 maxLine;                   /* 0x67F0: highest spectral line coded so far */
    s32 nch;                       /* 0x67F4 */
    s32 format;                    /* 0x67F8: 1 mono, 2 stereo, 0x12 joint stereo */
    s32 unit;                      /* 0x67FC: bytes per channel sound unit */
    u8 *stream;                    /* 0x6800 */
    u8 rev[0xC0];                  /* 0x6804: second unit of format 0x12, byte-reversed */
    s32 atten;                     /* 0x68C4: scale factor attenuation */
    s32 bitPos;                    /* 0x68C8 */
} at3decWork_t;

typedef char at3decChannel_t_size_must_be_0x33E0[(sizeof(at3decChannel_t) == 0x33E0) ? 1 : -1];
typedef char at3decWork_t_maxLine_at_0x67F0[((u32)&((at3decWork_t *)0)->maxLine == 0x67F0) ? 1 : -1];
typedef char at3decWork_t_bitPos_at_0x68C8[((u32)&((at3decWork_t *)0)->bitPos == 0x68C8) ? 1 : -1];

/* What the game passes to at3decReset and at3decDecodeFrame. */
typedef struct at3decState_t
{
    u8 *stream;          /* compressed frame, set per call */
    s16 *pcm;            /* output, set once */
    u32 mode;            /* see at3decReset */
    at3decWork_t *work;  /* 0x68CA bytes */
    s32 atten;           /* raised step by step for fades */
} at3decState_t;

/* The band and joint stereo weight selectors run one entry past these
 * arrays, into at3decDefaultMixWeight and at3decUnityMixWeight. */
extern const s32 at3decImdctTwiddles[128][4];
extern const s16 at3decOutputFirTaps[48];
extern const s16 at3decQmfTaps[24];
extern const s16 at3decQmfTapsReversed[24];
extern const s32 at3decLowHalfMask[4];
extern const s32 at3decScaleFactorShifts[64];
extern const u16 at3decGainSteps[8];
extern const at3decWeight_t at3decBandMixWeights[];
extern const at3decWeight_t at3decDefaultMixWeight;
extern const at3decWeight_t at3decJointStereoWeights[];
extern const at3decWeight_t at3decUnityMixWeight;
extern const s32 at3decSubbandStartLines[33];
extern const s8 at3decLookaheadBits[16];
extern const s8 *const at3decCodeLengthTables[];
extern const s32 *const at3decDequantTables[];
extern const s8 *const at3decSymbolLookupTables[];
extern const u16 at3decOverlapWindow[128];
extern const s32 at3decOverlapOrder[128];
extern const s16 at3decOverlapWindows[256];
extern const s16 at3decFallingWindow[128];

extern void *memset(void *s, s32 c, u32 n);
extern void *memcpy(void *dst, const void *src, u32 n);
extern void __assert(const char *file, s32 line, const char *expr);

void at3decReset(at3decState_t *state);
s32 at3decDecodeFrame(at3decState_t *state);
void at3decMixJointStereo(at3decWork_t *w);
s32 at3decInverseTransformBand(at3decChannel_t *chan, s32 band, s32 level0);
s32 at3decApplyGainControl(at3decChannel_t *chan, s32 band);
void at3decSynthesizeQmf(at3decChannel_t *chan);
s32 at3decPeekBits(s32 nbits, at3decWork_t *w);
s32 at3decReadBits(s32 nbits, at3decWork_t *w);
s32 at3decDecodeSpectrumLines(at3decChannel_t *chan, u32 mode, s32 qstep, s32 first, s32 last, s32 sfindex,
                              at3decWork_t *w);
s32 at3decDecodeTonalComponents(at3decChannel_t *chan, s32 groups, s32 nbands, at3decWork_t *w);
s32 at3decDecodeSpectrum(at3decChannel_t *chan, s32 nsub, at3decWork_t *w);
void at3decCarrySilentBand(at3decChannel_t *chan, s32 band);
s32 at3decDecodeSoundUnit(at3decChannel_t *chan, s32 second, at3decWork_t *w);
s32 at3decDecodeChannels(at3decWork_t *w);
void at3decFilterOutput(s16 *p);
void at3decSynthesizeHalfBand(s16 *out, const s16 *in, s32 sel);
void at3decSplitSumDifference(const s32 *a, const s32 *b, s16 *diff, s16 *sum);
void at3decButterflyStage(s32 *a, const s32 *b, s32 n);
void at3decRotateButterfly(s32 *a, s32 *b, const s32 *tw, const s32 *mask);
void at3decRotateButterflyPair(s32 *a, s32 *b, const s32 *tw, const s32 *mask);
void at3decCopyBlock(const void *src, void *dst);
void at3decInitDefaults(s32 nch, s32 format, s32 unit, at3decWork_t *w);

/* mode: 0, 1 and 5 up stereo (2 x 0xC0 bytes), 2 joint stereo (2 x 0x60),
 * 3 mono 0xC0, 4 mono 0x60. */
#ifdef NON_MATCHING
void at3decReset(at3decState_t *state)
{
    s32 nch;
    s32 format;
    s32 unit;

    switch (state->mode) {
    case 2:
        nch = 2;
        format = 0x12;
        unit = 0x60;
        break;
    case 3:
        nch = 1;
        format = 1;
        unit = 0xC0;
        break;
    case 4:
        nch = 1;
        format = 1;
        unit = 0x60;
        break;
    default:
        nch = 2;
        format = 2;
        unit = 0xC0;
        break;
    }
    memset(state->work, 0, 0x68CA);
    at3decInitDefaults(nch, format, unit, state->work);
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decReset);
#endif

/* Modes 1 and 2 write 4 x (256 left, 256 right) samples; the others write
 * the first channel's 1024. Returns 0, or -1 for a bad frame. */
#ifdef NON_MATCHING
s32 at3decDecodeFrame(at3decState_t *state)
{
    at3decWork_t *w = state->work;
    s16 *out = state->pcm;
    s16 *left;
    s16 *right;
    s32 i;

    w->atten = state->atten;
    w->stream = state->stream;
    if (at3decDecodeChannels(w) == -1) {
        return -1;
    }
    if (state->mode - 1 < 2) {
        left = w->chan[0].pcm;
        right = w->chan[1].pcm;
        for (i = 0; i < 4; i++) {
            at3decCopyBlock(left, out);
            at3decCopyBlock(right, out + 0x100);
            left += 0x100;
            right += 0x100;
            out += 0x200;
        }
    } else {
        left = w->chan[0].pcm;
        for (i = 0; i < 4; i++) {
            at3decCopyBlock(left, out);
            left += 0x100;
            out += 0x100;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decDecodeFrame);
#endif

/* Per band, the 2x2 weights are interpolated over the first 8 samples from
 * the previous frame's to this frame's. */
#ifdef NON_MATCHING
void at3decMixJointStereo(at3decWork_t *w)
{
    s32 band;
    s32 i;

    for (band = 0; band < 4; band++) {
        s32 r30 = 0x8000;
        s32 r23 = 0x8000;
        s32 r11 = 0x8000;
        s32 r9 = 0x8000;
        s32 r5, r6, r7, r8, r15, r24, d1, d2, d3, d4;
        const at3decWeight_t *prev;
        const at3decWeight_t *cur;
        s32 *q0;
        s32 *q1;

        if (band >= w->prev.nbands) {
            r11 = w->prev.matrix->b;
            r9 = w->prev.matrix->a;
        }
        if (band >= w->cur.nbands) {
            r30 = w->cur.matrix->b;
            r23 = w->cur.matrix->a;
        }
        prev = w->prev.band[band];
        cur = w->cur.band[band];
        r24 = cur->b;
        r15 = cur->a;
        d1 = (((r9 - r23) << 16) >> 19) & 0xFFFF;
        d2 = (((r11 - r30) << 16) >> 19) & 0xFFFF;
        d3 = ((prev->a - r15) >> 3) & 0xFFFF;
        d4 = ((prev->b - r24) >> 3) & 0xFFFF;
        r5 = (d2 << 3) + r30;
        r6 = (d4 << 3) + r24;
        r7 = (d1 << 3) + r23;
        r8 = (d3 << 3) + r15;
        q0 = &w->chan[0].band[band][23];
        q1 = &w->chan[1].band[band][23];
        for (i = 0; i < 0x100; i++) {
            s32 a = r15;
            s32 b = r24;
            s32 c = r23;
            s32 d = r30;
            s32 x0, x1, s0;

            if (i < 8) {
                a = r8 & 0xFFFF;
                c = r7 & 0xFFFF;
                b = r6 & 0xFFFF;
                d = r5 & 0xFFFF;
            }
            x0 = q0[i];
            x1 = q1[i];
            r5 -= d2;
            r6 -= d4;
            r7 -= d1;
            r8 -= d3;
            s0 = a * (x0 >> 4) + b * (x1 >> 4);
            q0[i] = ((s0 >> 8) * c) >> 6;
            q1[i] = (((x0 - s0) >> 8) * d) >> 6;
        }
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decMixJointStereo);
#endif

/* Inverse transform of one QMF band: fold the 256 lines (odd bands are
 * frequency-reversed), a lifting rotation, a normalising shift, six butterfly
 * stages, then the windowed overlap-add into the band buffer scaled by the
 * first gain level. Returns 1 for a silent band, which is left untouched. */
#ifdef NON_MATCHING
s32 at3decInverseTransformBand(at3decChannel_t *chan, s32 band, s32 level0)
{
    s32 *spec = chan->spectrum[band];
    s32 *work = spec;
    s32 *bandbuf = chan->band[band];
    s32 i, maxabs, shift, g, k;
    s32 *p;

    maxabs = 0;
    if (band & 1) {
        work = chan->spectrum[0];
        for (i = 0; i < 0x80; i++) {
            work[i] = spec[0xFF - i] - spec[i];
            work[0x80 + i] = spec[0x7F - i];
        }
    } else {
        for (i = 0; i < 0x80; i++) {
            work[i] -= work[0xFF - i];
        }
    }

    /* x[i + 128] *= sqrt(2), then sum and difference */
    for (i = 0; i < 0x80; i++) {
        s32 hi = work[0x80 + i];
        s32 lo = work[i];
        s32 d, s, a;

        hi += (((hi >> 15) * 0x6A0A) >> 1) + (((hi & 0x7FFF) * 0x6A0A) >> 16);
        d = lo - hi;
        s = lo + hi;
        work[0x80 + i] = d;
        work[i] = s;
        a = d < 0 ? -d : d;
        if (maxabs < a) {
            maxabs = a;
        }
        a = s < 0 ? -s : s;
        if (maxabs < a) {
            maxabs = a;
        }
    }
    if (maxabs == 0) {
        return 1;
    }

    shift = 12;
    if (maxabs >= 0x20000) {
        shift = 11;
        while (shift >= 2 && maxabs >= (1 << (29 - shift))) {
            shift--;
        }
    }
    for (i = 0; i < 0x100; i++) {
        work[i] <<= shift;
    }

    p = work;
    g = 0x40;
    do {
        s32 half = g << 1;

        k = g >> 1;
        do {
            s32 *q;

            if (half >= 5) {
                at3decButterflyStage(p, p + g * 2 - 4, half);
                q = p + g;
            } else {
                s32 c = half;

                q = p;
                do {
                    s32 *e = q + c;

                    c -= 4;
                    q[0] = (q[0] - e[-1]) >> 1;
                    q[1] = (q[1] - e[-2]) >> 1;
                    q += 2;
                } while (c > 0);
            }
            if (p + 2 == q) {
                at3decRotateButterflyPair(p + g, p, at3decImdctTwiddles[k], at3decLowHalfMask);
                p = q;
            } else {
                do {
                    at3decRotateButterfly(p + g, p, at3decImdctTwiddles[k], at3decLowHalfMask);
                    p += 4;
                } while (p < q);
            }
            k += g;
            p += g;
        } while (k < 0x80);
        g >>= 1;
        p -= 0x100;
    } while (g != 1);

    memcpy(bandbuf, bandbuf + 0x100, 0xB8);
    {
        s32 s = level0 + shift;
        s32 s1 = s - 1;
        s32 *out = bandbuf + 23;
        s32 *wp = work + 0xFE;

        for (i = 0x7F; i >= 0; i--) {
            s32 w1 = wp[1];
            s32 w0 = wp[0];
            s32 c = at3decOverlapWindow[i];
            s32 idx = at3decOverlapOrder[i];
            s32 e, half, diff, sum, c2, lo, hi, p1, p2, acc;
            s32 *oIdx;
            s16 winC;

            wp -= 2;
            e = (w1 >> 16) * c + ((c * ((w1 >> 1) & 0x7FFF)) >> 15);
            half = (w0 - w1) >> 1;
            c2 = at3decOverlapWindows[idx + 0x80];
            winC = at3decFallingWindow[idx];
            diff = half - e;
            sum = half + e;
            oIdx = out + idx;
            lo = diff & 0x7FFF;
            hi = diff >> 15;
            p1 = lo * c2;
            p2 = hi * c2;
            acc = 0;
            if (s1 < 0x14) {
                /* the hardware shift takes the count mod 32, and s - 4 can be negative */
                acc = (p2 >> ((s - 4) & 31)) + (p1 >> (s + 0xB));
            }
            out[0x7F - idx] = ((oIdx[0x100] >> 1) - winC * acc) >> 3;
            if (s1 < 0xF) {
                acc = (p2 << (0xF - s1)) + (p1 >> s1);
            } else {
                acc = (p2 >> (s - 0x10)) + (p1 >> s1);
            }
            oIdx[0x80] = acc + ((((s16 *)&oIdx[0x100])[1] * winC) >> 3);
            oIdx[0x100] = (sum << (0xF - shift)) + (sum >> shift) * at3decOverlapWindows[idx];
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decInverseTransformBand);
#endif

/* Walk the band's gain points, shifting the samples right by each point's
 * level; the 7 samples after a point step to the next level in 1/8 steps.
 * Returns 0, or -1 for malformed points. */
#ifdef NON_MATCHING
s32 at3decApplyGainControl(at3decChannel_t *chan, s32 band)
{
    s32 *p = &chan->band[band][23];
    s32 *end = &chan->band[band][23 + 0x100];
    at3decGainPoint_t *gp = chan->gain[band];
    s32 *base = &chan->band[band][24];
    s32 level, cur, d, acc, j;

    if (band >= 4) {
        return -1;
    }
    while (1) {
        if (end < base + gp->loc) {
            return -1;
        }
        cur = gp->level;
        if (p >= base + gp->loc) {
            return -1;
        }
        if ((u32)cur >= 0x10) {
            return -1;
        }
        do {
            *p >>= cur;
            p++;
        } while (p < base + gp->loc);
        if (p >= end) {
            return 0;
        }
        gp++;
        level = gp->level;
        d = level - cur;
        acc = (cur << 3) + d;
        for (j = 0; j < 7; j++) {
            *p = ((*p >> 11) * at3decGainSteps[acc & 7]) >> (((u32)acc >> 3) + 4);
            p++;
            acc += d;
        }
        if (p >= end) {
            return 0;
        }
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decApplyGainControl);
#endif

/* QMF synthesis of the four band buffers into the 1024 output samples:
 * sum and difference of bands 0/1 and 3/2, the two half-band filters, then
 * the 8-output FIR in place. */
#ifdef NON_MATCHING
void at3decSynthesizeQmf(at3decChannel_t *chan)
{
    s16 buf[0x460] __attribute__((aligned(16)));
    s16 *diff = buf;
    s16 *sum = buf + 0x230;
    s16 *out;
    s16 *p;
    s32 i;

    for (i = 0; i < 35; i++) {
        at3decSplitSumDifference(&chan->band[0][i * 8], &chan->band[1][i * 8], diff + i * 8, sum + i * 8);
        at3decSplitSumDifference(&chan->band[3][i * 8], &chan->band[2][i * 8], diff + 0x118 + i * 8,
                                 sum + 0x118 + i * 8);
    }
    out = &chan->pcm[AT3DEC_HISTORY];
    for (i = 0; i < 32; i++) {
        at3decSynthesizeHalfBand(out, diff + i * 8, 0);
        at3decSynthesizeHalfBand(out + 2, sum + i * 8, 1);
        out += 0x20;
    }
    for (p = chan->pcm; p < &chan->pcm[1024]; p += 8) {
        at3decFilterOutput(p);
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decSynthesizeQmf);
#endif

/* nbits plus the bit offset within the byte must not exceed 16. */
#ifdef NON_MATCHING
s32 at3decPeekBits(s32 nbits, at3decWork_t *w)
{
    s32 pos = w->bitPos;
    u8 *p = w->stream + (pos >> 3);

    return ((p[0] << 8 | p[1]) >> (16 - (nbits + (pos & 7)))) & ((1 << nbits) - 1);
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decPeekBits);
#endif

s32 at3decReadBits(s32 nbits, at3decWork_t *w)
{
    s32 v = at3decPeekBits(nbits, w);

    w->bitPos += nbits;
    return v;
}

/* Decode lines first..last-1 with quantiser step qstep (1..7) and coding
 * mode 0 or 1, adding each dequantised value onto the line. Step 1 codes two
 * lines per symbol. Returns 0, or -1 for bad arguments. */
#ifdef NON_MATCHING
s32 at3decDecodeSpectrumLines(at3decChannel_t *chan, u32 mode, s32 qstep, s32 first, s32 last, s32 sfindex,
                              at3decWork_t *w)
{
    s32 idx, nbits, shift, count;
    const s8 *lookup;
    const s8 *lens;
    const s32 *deq;
    s32 *out;

    if (mode >= 2 || (u32)(qstep - 1) >= 7 || last == first) {
        return -1;
    }
    idx = qstep * 2 + mode;
    nbits = at3decLookaheadBits[idx];
    lookup = at3decSymbolLookupTables[idx - 2];
    lens = at3decCodeLengthTables[idx - 2];
    shift = at3decScaleFactorShifts[sfindex];
    deq = at3decDequantTables[7 * (sfindex - 3 * (21 - shift)) + qstep];
    out = (s32 *)chan->spectrum + first;
    count = last - first;

    if (qstep == 1) {
        do {
            s32 sym = lookup[at3decPeekBits(nbits, w)];

            w->bitPos += lens[sym];
            *out++ += deq[sym * 2] >> shift;
            if (count != 1) {
                *out++ += deq[sym * 2 + 1] >> shift;
            }
            count -= 2;
        } while (count > 0);
    } else {
        do {
            s32 sym = lookup[at3decPeekBits(nbits, w)];

            w->bitPos += lens[sym];
            *out++ += deq[sym] >> shift;
        } while (--count > 0);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decDecodeSpectrumLines);
#endif

/* groups groups of tonal components, each: band flags, value count, step,
 * an optional coding mode bit, then per flagged band and 64-line block a
 * component count and per component a scale factor index and start line. */
#ifdef NON_MATCHING
s32 at3decDecodeTonalComponents(at3decChannel_t *chan, s32 groups, s32 nbands, at3decWork_t *w)
{
    s32 selector = at3decReadBits(2, w);

    if (selector == 2) {
        return -1;
    }
    do {
        s32 flags = at3decPeekBits(4, w);
        s32 nvalues, qstep, mode, band, sub, comp;

        w->bitPos += nbands;
        nvalues = at3decReadBits(3, w) + 1;
        qstep = at3decReadBits(3, w);
        if (qstep < 2) {
            return -1;
        }
        mode = selector;
        if (selector == 3) {
            mode = at3decReadBits(1, w);
        }
        for (band = 0; band < nbands; band++) {
            if ((flags & (8 >> band)) == 0) {
                continue;
            }
            for (sub = 0; sub < 0x100; sub += 0x40) {
                for (comp = at3decReadBits(3, w) - 1; comp >= 0; comp--) {
                    s32 sf = at3decReadBits(6, w);
                    s32 first, last;

                    if (w->atten > 0) {
                        sf = w->atten < sf ? sf - w->atten : 0;
                    }
                    first = at3decReadBits(6, w) + (band << 8) + sub;
                    last = nvalues + first;
                    if (last > 0x400) {
                        last = 0x400;
                    }
                    if (w->maxLine < last) {
                        w->maxLine = last;
                    }
                    if (at3decDecodeSpectrumLines(chan, mode, qstep, first, last, sf, w) == -1) {
                        return -1;
                    }
                }
            }
        }
    } while (--groups > 0);
    return 0;
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decDecodeTonalComponents);
#endif

/* nsub (1..32) subbands: coding mode bit, a step per subband (0 = not
 * coded), a scale factor index per coded subband, then the lines. */
#ifdef NON_MATCHING
s32 at3decDecodeSpectrum(at3decChannel_t *chan, s32 nsub, at3decWork_t *w)
{
    s32 step[32];
    s32 sf[32];
    s32 mode, i;

    if ((u32)(nsub - 1) >= 0x20) {
        return -1;
    }
    mode = at3decReadBits(1, w);
    for (i = 0; i < nsub; i++) {
        step[i] = at3decReadBits(3, w);
    }
    for (i = 0; i < nsub; i++) {
        if (step[i] != 0) {
            s32 v = at3decReadBits(6, w);

            sf[i] = v;
            if (w->atten > 0) {
                sf[i] = w->atten < v ? v - w->atten : 0;
            }
        }
    }
    for (i = 0; i < nsub; i++) {
        if (step[i] != 0) {
            if ((u32)i >= 0x21) {
                __assert("libat3dec.c", 0x421, "(unsigned int)iqt < AT2_NQUS+1");
            }
            if ((u32)(i + 1) >= 0x21) {
                __assert("libat3dec.c", 0x421, "(unsigned int)iqt+1 < AT2_NQUS+1");
            }
            if (at3decDecodeSpectrumLines(chan, mode, step[i], at3decSubbandStartLines[i],
                                          at3decSubbandStartLines[i + 1], sf[i], w) == -1) {
                return -1;
            }
        }
    }
    if ((u32)nsub >= 0x21) {
        __assert("libat3dec.c", 0x427, "(unsigned int)iqt < AT2_NQUS+1");
    }
    if (w->maxLine < at3decSubbandStartLines[nsub]) {
        if ((u32)nsub >= 0x21) {
            __assert("libat3dec.c", 0x428, "(unsigned int)iqt < AT2_NQUS+1");
        }
        w->maxLine = at3decSubbandStartLines[nsub];
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decDecodeSpectrum);
#endif

/* A silent band: carry the history down and window out the pending overlap
 * half of the band buffer, clearing it. */
#ifdef NON_MATCHING
void at3decCarrySilentBand(at3decChannel_t *chan, s32 band)
{
    s32 *b = chan->band[band];
    s32 *src = b + 0x117;
    s32 *dst = b + 0x96;
    s32 i;

    memcpy(b, b + 0x100, 0x5C);
    for (i = 0; i < 0x80; i++) {
        s32 v = *src;

        *dst = v >> 4;
        *src = 0;
        src[-0x80] = ((v >> 14) * at3decFallingWindow[i]) >> 5;
        src++;
        dst--;
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decCarrySilentBand);
#endif

/* Parse and reconstruct one channel's sound unit. second is set for the
 * reversed second unit of format 0x12, which starts with a joint stereo
 * header in place of the unit id. Returns 0, or -1 for a bad unit. */
#ifdef NON_MATCHING
s32 at3decDecodeSoundUnit(at3decChannel_t *chan, s32 second, at3decWork_t *w)
{
    at3decGainPoint_t gain[4][8];
    s32 sel[4];
    s32 angle = 0;
    s32 n, g, i, k, cnt, groups, nbands, band;

    if (second) {
        angle = at3decReadBits(4, w);
        for (i = 0; i < 4; i++) {
            sel[i] = at3decReadBits(2, w);
        }
        w->bitPos += 2;
    } else {
        w->bitPos += 6;
    }
    n = at3decReadBits(2, w) + 1;
    memset(gain, 0, sizeof(gain));
    for (g = 0; g < n; g++) {
        cnt = at3decReadBits(3, w);
        for (k = 0; k < cnt; k++) {
            gain[g][k].level = at3decReadBits(4, w);
            gain[g][k].loc = at3decReadBits(5, w) << 3;
        }
        gain[g][cnt].loc = 0xFF;
        gain[g][cnt].level = 4;
    }
    for (; g < 4; g++) {
        gain[g][0].loc = 0xFF;
        gain[g][0].level = 4;
    }

    memcpy(chan->pcm, &chan->pcm[1024], AT3DEC_HISTORY * sizeof(s16));
    memset(chan->spectrum, 0, sizeof(chan->spectrum));
    w->maxLine = n << 8;

    groups = at3decReadBits(5, w);
    if (groups != 0 && at3decDecodeTonalComponents(chan, groups, n, w) == -1) {
        return -1;
    }
    if (at3decDecodeSpectrum(chan, at3decReadBits(5, w) + 1, w) == -1) {
        return -1;
    }

    nbands = n;
    if ((n << 8) < w->maxLine) {
        s32 t = w->maxLine + 0xF7;

        if (t < 0) {
            t = w->maxLine + 0x1F6;
        }
        nbands = t >> 8;
    }

    band = 0;
    do {
        if (at3decInverseTransformBand(chan, band, gain[band][0].level) != 0) {
            at3decCarrySilentBand(chan, band);
        }
        if (at3decApplyGainControl(chan, band++) == -1) {
            return -1;
        }
    } while (band < nbands);
    for (; band < 4; band++) {
        at3decCarrySilentBand(chan, band);
        if (at3decApplyGainControl(chan, band) == -1) {
            return -1;
        }
    }

    if (second) {
        at3decMixJointStereo(w);
        w->prev = w->cur;
        w->cur.nbands = n;
        w->cur.matrix = &at3decJointStereoWeights[angle];
        for (i = 0; i < 4; i++) {
            w->cur.band[i] = &at3decBandMixWeights[sel[i]];
        }
    }
    memcpy(chan->gain, gain, sizeof(gain));
    return 0;
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decDecodeSoundUnit);
#endif

/* For each channel parse its sound unit, then run the QMF synthesis. Format
 * 0x12 stores the second unit byte-reversed at the end of the frame; it is
 * flipped into the work buffer first, without the 0xF8 padding. */
#ifdef NON_MATCHING
s32 at3decDecodeChannels(at3decWork_t *w)
{
    s32 ch;
    s32 pos = 0;

    for (ch = 0; ch < w->nch; ch++) {
        s32 reversed = w->format == 0x12 && ch == 1;

        w->bitPos = pos << 3;
        if (reversed) {
            u8 *in = w->stream;
            u8 *dst = w->rev;
            u8 *p = in + w->unit + pos - 1;
            u8 byte;

            while ((byte = *p) == 0xF8 && in < p) {
                p--;
            }
            if (in < p) {
                *dst++ = byte;
                for (p--; in < p; p--) {
                    *dst++ = *p;
                }
            }
            w->stream = w->rev;
            w->bitPos = 0;
        }
        if (at3decDecodeSoundUnit(&w->chan[ch], reversed, w) == -1) {
            return -1;
        }
        pos += w->unit;
        if (w->format != 0x12 && (pos << 3) < w->bitPos) {
            return -1;
        }
    }
    for (ch = 0; ch < w->nch; ch++) {
        at3decSynthesizeQmf(&w->chan[ch]);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decDecodeChannels);
#endif

#ifdef NON_MATCHING
static s32 at3decAddSat(s32 a, s32 b)
{
    s32 t = a + b;

    if (((a ^ t) & (b ^ t)) < 0) {
        return a < 0 ? (s32)0x80000000 : 0x7FFFFFFF;
    }
    return t;
}

static s32 at3decSubSat(s32 a, s32 b)
{
    s32 t = a - b;

    if (((a ^ b) & (a ^ t)) < 0) {
        return a < 0 ? (s32)0x80000000 : 0x7FFFFFFF;
    }
    return t;
}

static s16 at3decSat16(s32 v)
{
    if (v > 0x7FFF) {
        return 0x7FFF;
    }
    if (v < -0x8000) {
        return -0x8000;
    }
    return v;
}

/* pmaddh accumulates lanes 0, 1, 4, 5 of each 8-halfword product in LO and
 * 2, 3, 6, 7 in HI; the four sums of HI and LO then add with saturation. */
static s32 at3decDot(const s16 *x, const s16 *c, s32 n, s32 *odd)
{
    s32 lo[4] = { 0, 0, 0, 0 };
    s32 hi[4] = { 0, 0, 0, 0 };
    s32 m, t;
    s32 s[4];

    for (m = 0; m < n; m++) {
        for (t = 0; t < 8; t++) {
            s32 prod = x[8 * m + t] * c[8 * m + t];
            s32 lane = (t & 1) | ((t >> 1) & 2);

            if (t & 2) {
                hi[lane] += prod;
            } else {
                lo[lane] += prod;
            }
        }
    }
    for (t = 0; t < 4; t++) {
        s[t] = at3decAddSat(hi[t], lo[t]);
    }
    *odd = at3decAddSat(s[1], s[3]);
    return at3decAddSat(s[0], s[2]);
}
#endif

/* 8 output samples over p[0..7] from the 54-sample window at p and the
 * 48-tap FIR; the window slides 2 samples per output pair. */
#ifdef NON_MATCHING
void at3decFilterOutput(s16 *p)
{
    s16 win[56];
    s16 outw[8];
    s32 j, even, odd;

    memcpy(win, p, sizeof(win));
    for (j = 0; j < 4; j++) {
        even = at3decDot(&win[2 * j], at3decOutputFirTaps, 6, &odd);
        outw[2 * j] = at3decSat16(even >> 13);
        outw[2 * j + 1] = at3decSat16(odd >> 13);
    }
    memcpy(p, outw, sizeof(outw));
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decFilterOutput);
#endif

/* Half-band synthesis: 8 output pairs (every other pair of out) of the low
 * band minus and plus the high band, which is 0x118 samples after the low
 * band in the input. sel picks the forward or reversed taps. */
#ifdef NON_MATCHING
void at3decSynthesizeHalfBand(s16 *out, const s16 *in, s32 sel)
{
    const s16 *c = sel ? at3decQmfTaps : at3decQmfTapsReversed;
    s32 j, va, vb, odd;

    for (j = 0; j < 8; j++) {
        va = at3decDot(in + j, c, 3, &odd);
        va = at3decAddSat(odd, va);
        vb = at3decDot(in + 0x118 + j, c, 3, &odd);
        vb = at3decAddSat(odd, vb);
        out[4 * j] = at3decSat16(at3decSubSat(va, vb) >> 15);
        out[4 * j + 1] = at3decSat16(at3decAddSat(va, vb) >> 15);
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decSynthesizeHalfBand);
#endif

/* Eight samples: diff = sat16(sat(a - b) >> 9), sum = sat16(sat(a + b) >> 9). */
#ifdef NON_MATCHING
void at3decSplitSumDifference(const s32 *a, const s32 *b, s16 *diff, s16 *sum)
{
    s32 k;

    for (k = 0; k < 8; k++) {
        diff[k] = at3decSat16(at3decSubSat(a[k], b[k]) >> 9);
        sum[k] = at3decSat16(at3decAddSat(a[k], b[k]) >> 9);
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decSplitSumDifference);
#endif

/* For n / 8 quadwords: a[k] = sat(a[k] - b reversed[k]) >> 1, with b running
 * back from the end of the group. */
#ifdef NON_MATCHING
void at3decButterflyStage(s32 *a, const s32 *b, s32 n)
{
    s32 count = (u32)n >> 3;
    s32 k;

    do {
        for (k = 0; k < 4; k++) {
            a[k] = at3decSubSat(a[k], b[3 - k]) >> 1;
        }
        a += 4;
        b -= 4;
    } while (--count > 0);
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decButterflyStage);
#endif

/* S[k] = sat(hi(x[k]) * t + ((lo15(x[k]) * t) >> 15)) with t = tw[0] for
 * k = 0, 1 and tw[2] for k = 2, 3 and x the quadword at a; then
 * a = sat(b - S) >> 1 and b = sat(b + S) >> 1. */
#ifdef NON_MATCHING
void at3decRotateButterfly(s32 *a, s32 *b, const s32 *tw, const s32 *mask)
{
    s32 k;

    for (k = 0; k < 4; k++) {
        s32 t = tw[k & 2];
        s32 s = at3decAddSat((a[k] >> 16) * t, (((a[k] >> 1) & mask[k]) * t) >> 15);
        s32 y = b[k];

        a[k] = at3decSubSat(y, s) >> 1;
        b[k] = at3decAddSat(y, s) >> 1;
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decRotateButterfly);
#endif

/* The two-word form for the last stage: a is b + 2, and the rotated values
 * are b[2] and b[3]. */
#ifdef NON_MATCHING
void at3decRotateButterflyPair(s32 *a, s32 *b, const s32 *tw, const s32 *mask)
{
    s32 y[4];
    s32 k;

    for (k = 0; k < 4; k++) {
        y[k] = b[k];
    }
    for (k = 0; k < 2; k++) {
        s32 x = y[2 + k];
        s32 t = tw[2 * k];
        s32 s = at3decAddSat((x >> 16) * t, (((x >> 1) & mask[k]) * t) >> 15);

        a[k] = at3decSubSat(y[k], s) >> 1;
        b[k] = at3decAddSat(y[k], s) >> 1;
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decRotateButterflyPair);
#endif

/* Copies 0x200 bytes; both pointers 16-byte aligned. */
#ifdef NON_MATCHING
void at3decCopyBlock(const void *src, void *dst)
{
    const u32 *s = src;
    u32 *d = dst;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        d[i] = s[i];
    }
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decCopyBlock);
#endif

/* Unity mixing and a terminating gain point (location 0xFF, level 4) at the
 * head of every band's gain table. */
#ifdef NON_MATCHING
void at3decInitDefaults(s32 nch, s32 format, s32 unit, at3decWork_t *w)
{
    s32 band;
    s32 ch;

    w->format = format;
    w->unit = unit;
    w->cur.matrix = &at3decUnityMixWeight;
    w->cur.nbands = 4;
    w->nch = nch;
    for (band = 0; band < 4; band++) {
        w->cur.band[band] = &at3decDefaultMixWeight;
        for (ch = 0; ch < nch; ch++) {
            w->chan[ch].gain[band][0].level = 4;
            w->chan[ch].gain[band][0].loc = 0xFF;
        }
    }
    w->prev = w->cur;
}
#else
INCLUDE_ASM(const s32, "hand/at3dec", at3decInitDefaults);
#endif

INCLUDE_ASM(const s32, "hand/at3dec", func_002F9B88);

INCLUDE_ASM(const s32, "hand/at3dec", func_002F9B90);

INCLUDE_ASM(const s32, "hand/at3dec", func_002F9C48);
