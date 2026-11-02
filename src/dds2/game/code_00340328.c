#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "sdf_vu0.h"
#include "ee_mmi.h"
#include "sdf_trig.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf_draw.h"
#include "sdf_sif_command.h"
#include "sdf_dev_event.h"
#include "sdf_dev_protocol.h"
#include "sdf_dev_state.h"
#include "sdf_memory_api.h"

#define SDF_BCD_DIGIT_BITS 4

#define SDF_DECIMAL_RADIX 10

/* Clear `count` quadwords at dst: four per pass, then a falling 3/2/1
 * remainder. The pass is a goto loop (no loop notes, so no loop alignment),
 * and its exit test is taken before the four stores. A do/while here adds
 * the loop's alignment nops (128 bytes vs 120 retail, 28 of 30 words differ). */
u128 *sdfZeroMemQuad(u128 *dst, s32 count) {
    u128 *p = dst;
    s32 done;

    if (count >= 4) {
loop:
        count -= 4;
        done = count < 4;
        EE_STORE_ZERO_QUAD(p, 0x0);
        PS2_STORE_ZERO_QUAD_AT16(p);
        PS2_STORE_ZERO_QUAD_AT32(p);
        PS2_STORE_ZERO_QUAD_AT48(p);
        p += 4;
        if (!done) {
            goto loop;
        }
    }
    switch (count) {
    case 3:
        PS2_STORE_ZERO_QUAD_AT32(p);
    case 2:
        PS2_STORE_ZERO_QUAD_AT16(p);
    case 1:
        EE_STORE_ZERO_QUAD(p, 0x0);
    }
    return dst;
}

char *sdfStrdupChip(const char *text) {
    u32 length;
    char *copy;

    if (text == NULL) {
        return NULL;
    }
    length = strlen(text);
    copy = sdfChipAllocDirect(length + 1);
    memcpy(copy, text, length);
    copy[length] = 0;
    return copy;
}

s32 sdfBcdToDecimal(s32 packedDigits) {
    s32 place = 1;
    s32 acc = 0;

    while (packedDigits > 0) {
        acc += (packedDigits & 0xf) * place;
        packedDigits >>= 4;
        place *= 10;
    }
    return acc;
}

/* Pack positive decimal digits into nibbles; non-positive input returns zero.
 * The original signed shifts and lack of an overflow check are retained.
 */
s32 sdfDecimalToPackedDigits(s32 number) {
    s32 digitShift = 0;
    s32 packedDigits = 0;

    while (number > 0) {
        s32 quotient = number / SDF_DECIMAL_RADIX;
        packedDigits |= (number - quotient * SDF_DECIMAL_RADIX) << digitShift;
        number = quotient;
        digitShift += SDF_BCD_DIGIT_BITS;
    }
    return packedDigits;
}
