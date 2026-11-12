#include "sdk_libgcc.h"
#include "dds1/sdk/libc_internal.h"


extern const char printfSpaces[16];

extern const char printfZeros[16];

extern const char printfNanText[8] __attribute__((section(".rodata")));

extern const char printfHexLower[24];

extern const char printfNullText[8] __attribute__((section(".rodata")));

extern const char printfHexUpper[24];

extern const char printfBadBaseMsg[32];

extern const char printfZeroDigit[8] __attribute__((section(".rodata")));

extern const char printfDecimalPoint[16];

s32 __sprint(FILE *fp, __suio *uio) {
    s32 err;

    if (uio->uio_resid == 0) {
        uio->uio_iovcnt = 0;
        return 0;
    }
    err = __sfvwrite(fp, uio);
    uio->uio_resid = 0;
    uio->uio_iovcnt = 0;
    return err;
}

s32 __sbprintf(FILE *fp, const char *fmt, va_list ap)
{
    FILE fake;
    u8 buf[1024];
    s32 ret;

    fake._data = fp->_data;
    fake._flags = fp->_flags & ~2;
    fake._file = fp->_file;
    fake._cookie = fp->_cookie;
    fake._write = fp->_write;
    fake._bf._base = fake._p = buf;
    fake._bf._size = fake._w = sizeof(buf);
    fake._lbfsize = 0;
    ret = vfprintf(&fake, fmt, ap);
    if (ret >= 0 && fflush(&fake) != 0) {
        ret = -1;
    }
    if (fake._flags & 0x40) {
        fp->_flags |= 0x40;
    }
    return ret;
}

s32 vfprintf(FILE *fp, const char *fmt, va_list ap)
{
    /* Keep attachment and initialization as one phase, as in __swsetup. */
    do {
        if (fp->_data == NULL) {
            fp->_data = _impure_ptr;
        }
        if (fp->_data->__sdidinit == 0) {
            __sinit(fp->_data);
        }
    } while (0);
    return _vfprintf_r(fp->_data, fp, fmt, ap);
}

#ifdef NON_MATCHING
/* Formatted-output core. Arguments occupy the EE ABI's eight-byte slots. */

extern s32 __swsetup(FILE *fp);
extern s32 __sbprintf(FILE *fp, const char *format, va_list ap);
extern s32 __sprint(FILE *fp, __suio *uio);
extern s32 _mbtowc_r(_reent *r, s32 *out, const char *s, u32 limit, s32 *state);
extern void *memchr(const void *s, s32 c, u32 n);
extern void *localeconv(void);
extern char *cvt(_reent *r, f64 value, s32 precision, s32 flags, char *sign,
                 s32 *decimalPoint, s32 format, s32 *length);
extern s32 isinf(f64 value);
extern s32 isnan(f64 value);
extern s32 exponent(char *out, s32 value, s32 marker);
extern _reent *_impure_ptr;
extern s32 mbCurMax __attribute__((section(".data")));
extern const char printfInfText[];
extern const char printfNanText[];
extern const char printfHexLower[];
extern const char printfNullText[];
extern const char printfHexUpper[];
extern const char printfBadBaseMsg[];
extern const char printfZeroDigit[];
extern const char printfDecimalPoint[];
extern const char printfSpaces[16];
extern const char printfZeros[16];

/* Native output uses eight vectors and a 348-byte digit workspace. */
s32 _vfprintf_r(_reent *r, FILE *fp, const char *format, va_list ap)
{
    char exponentBuffer[16];
    __suio uio;
    __siov iov[8];
    __siov *current;
    s32 cumulative;
    char numberBuffer[0x15C];
    char prefix[2];
    char floatSign;
    char signByte;
    char *text;
    char *decimalPoint;
    const char *scanStart;
    const char *cursor;
    s32 mbstate;
    s32 wideCharacter;
    s32 decodedBytes;
    s32 literalBytes;
    s32 flags;
    s32 width;
    s32 precision;
    s32 integerPrecision;
    s32 conversion;
    s32 textBytes;
    s32 fieldBytes;
    s32 precisionZeros;
    s32 decimalPosition;
    s32 digitLength;
    s32 exponentLength;
    s32 renderedLength;
    s32 padBytes;
    s32 base;
    s32 nonzero;
    u16 unsignedShort;
    saddr_t signedWord;
    u32 unsignedWord;
    s64 signedLong;
    u64 unsignedLong;
    f64 floatValue;
    s32 *countWord;
    s16 *countShort;
    s64 *countLong;

#define PRINT(source_, length_) { \
        current->iov_base = (void *)(source_); \
        current->iov_len = (length_); \
        current++; \
        uio.uio_iovcnt++; \
        uio.uio_resid += (length_); \
        if (uio.uio_iovcnt == 8) { \
            if (__sprint(fp, &uio) != 0) goto error_exit; \
            else current = iov; \
        } \
    }
#define PAD(source_, length_) { \
        s32 padRemaining_ = (length_); \
        while (padRemaining_ > 0) { \
            s32 padBlock_ = padRemaining_ > 16 ? 16 : padRemaining_; \
            PRINT((source_), padBlock_); \
            padRemaining_ -= padBlock_; \
        } \
    }

    decimalPoint = *(char **)localeconv();
    mbstate = 0;
    if ((fp->_flags & 8) == 0 || fp->_bf._base == NULL) {
        if (__swsetup(fp) != 0) return -1;
    }
    if ((fp->_flags & 0x1A) == 0x0A && fp->_file >= 0) {
        return __sbprintf(fp, format, ap);
    }

    uio.uio_iov = iov;
    uio.uio_iovcnt = 0;
    uio.uio_resid = 0;
    current = iov;
    cumulative = 0;
    cursor = format;

    for (;;) {
        scanStart = cursor;
        for (;;) {
            decodedBytes = _mbtowc_r(_impure_ptr, &wideCharacter, cursor,
                                      mbCurMax, &mbstate);
            if (decodedBytes <= 0) break;
            cursor += decodedBytes;
            if (wideCharacter == '%') {
                cursor--;
                break;
            }
        }
        literalBytes = cursor - scanStart;
        if (literalBytes != 0) {
            PRINT(scanStart, literalBytes);
            cumulative += literalBytes;
        }
        if (decodedBytes <= 0) break;

        cursor++;
        signByte = 0;
        flags = 0;
        width = 0;
        precision = -1;
        integerPrecision = 0;
        textBytes = 0;
read_conversion:
        conversion = (s8)*cursor++;
dispatch_conversion:
        switch (conversion) {
        case ' ':
            if (signByte == 0) signByte = ' ';
            goto read_conversion;
        case '#':
            flags |= 1;
            goto read_conversion;
        case '*':
            width = __builtin_va_arg(ap, s32);
            if (width < 0) {
                width = (s32)(0u - (u32)width);
                flags |= 4;
            }
            goto read_conversion;
        case '-':
            flags |= 4;
            goto read_conversion;
        case '+':
            signByte = '+';
            goto read_conversion;
        case '.':
            conversion = (s8)*cursor++;
            if (conversion == '*') {
                precision = __builtin_va_arg(ap, s32);
                if (precision < -1) precision = -1;
                goto read_conversion;
            }
            precision = 0;
            while ((u32)(conversion - '0') < 10) {
                precision = (s32)((u32)precision * 10u + (u32)(conversion - '0'));
                conversion = (s8)*cursor++;
            }
            if (precision < -1) precision = -1;
            goto dispatch_conversion;
        case '0':
            flags |= 0x80;
            goto read_conversion;
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            width = 0;
            do {
                width = (s32)((u32)width * 10u + (u32)(conversion - '0'));
                conversion = (s8)*cursor++;
            } while ((u32)(conversion - '0') < 10);
            goto dispatch_conversion;
        case 'L':
            flags |= 8;
            goto read_conversion;
        case 'h':
            flags |= 0x40;
            goto read_conversion;
        case 'l':
            if (*cursor == 'l') {
                cursor++;
                flags |= 0x20;
            } else flags |= 0x10;
            goto read_conversion;
        case 'q':
            flags |= 0x20;
            goto read_conversion;
        case 'D':
            flags |= 0x10;
            /* fall through */
        case 'd':
        case 'i':
            if (flags & 0x10) signedLong = __builtin_va_arg(ap, s64);
            else if (flags & 0x40) signedLong = (s16)__builtin_va_arg(ap, s32);
            else signedLong = __builtin_va_arg(ap, s32);
            unsignedLong = (u64)signedLong;
            if (signedLong < 0) {
                signByte = '-';
                unsignedLong = 0 - unsignedLong;
            }
            base = 1;
            goto format_integer;
        case 'O':
            flags |= 0x10;
            /* fall through */
        case 'o':
            signByte = 0;
            if (flags & 0x10) unsignedLong = __builtin_va_arg(ap, u64);
            else if (flags & 0x40) {
                unsignedShort = (u16)__builtin_va_arg(ap, u32);
                unsignedLong = unsignedShort;
            } else {
                unsignedWord = __builtin_va_arg(ap, u32);
                unsignedLong = unsignedWord;
            }
            base = 0;
            goto format_integer;
        case 'U':
            flags |= 0x10;
            /* fall through */
        case 'u':
            signByte = 0;
            if (flags & 0x10) unsignedLong = __builtin_va_arg(ap, u64);
            else if (flags & 0x40) {
                unsignedShort = (u16)__builtin_va_arg(ap, u32);
                unsignedLong = unsignedShort;
            } else {
                unsignedWord = __builtin_va_arg(ap, u32);
                unsignedLong = unsignedWord;
            }
            base = 1;
            goto format_integer;
        case 'X':
        case 'x':
            signByte = 0;
            if (flags & 0x10) unsignedLong = __builtin_va_arg(ap, u64);
            else if (flags & 0x40) {
                unsignedShort = (u16)__builtin_va_arg(ap, u32);
                unsignedLong = unsignedShort;
            } else {
                unsignedWord = __builtin_va_arg(ap, u32);
                unsignedLong = unsignedWord;
            }
            base = 2;
            goto format_integer;
        case 'p':
            signByte = 0;
            flags |= 2;
            signedWord = EE_ADDR_S(__builtin_va_arg(ap, void *));
            unsignedLong = (u64)(s64)signedWord;
            base = 2;
            conversion = 'x';
            goto format_integer;
        case 's':
            signByte = 0;
            text = __builtin_va_arg(ap, char *);
            if (text == NULL) text = (char *)printfNullText;
            if (precision >= 0) {
                char *end = (char *)memchr(text, 0, (u32)precision);
                textBytes = end != NULL ? end - text : precision;
                if (textBytes > precision) textBytes = precision;
            } else textBytes = strlen(text);
            goto emit_field;
        case 'c':
            signByte = 0;
            numberBuffer[0] = (u8)__builtin_va_arg(ap, u32);
            text = numberBuffer;
            textBytes = 1;
            goto emit_field;
        case 'n':
            if (flags & 0x10) {
                countLong = __builtin_va_arg(ap, s64 *);
                *countLong = (s64)cumulative;
            } else if (flags & 0x40) {
                countShort = __builtin_va_arg(ap, s16 *);
                *countShort = (s16)cumulative;
            } else {
                countWord = __builtin_va_arg(ap, s32 *);
                *countWord = cumulative;
            }
            continue;
        case 'e': case 'E': case 'f': case 'g': case 'G':
            if (precision == -1) precision = 6;
            if ((conversion == 'g' || conversion == 'G') && precision == 0)
                precision = 1;
            floatValue = __builtin_va_arg(ap, f64);
            if (isinf(floatValue) != 0) {
                if (dpcmp(floatValue, 0.0) < 0) signByte = '-';
                text = (char *)printfInfText;
                textBytes = 3;
                renderedLength = 3;
            } else if (isnan(floatValue) != 0) {
                text = (char *)printfNanText;
                textBytes = 3;
                renderedLength = 3;
            } else {
                flags |= 0x100;
                text = cvt(r, floatValue, precision, flags, &floatSign,
                             &decimalPosition, conversion, &digitLength);
                if (conversion == 'g' || conversion == 'G') {
                    if (decimalPosition < -3 || precision < decimalPosition)
                        conversion = conversion == 'G' ? 'E' : 'e';
                    else
                        conversion = 'g';
                }
                if (conversion == 'e' || conversion == 'E') {
                    decimalPosition--;
                    exponentLength = exponent(exponentBuffer, decimalPosition, conversion);
                    renderedLength = exponentLength + digitLength;
                    if (digitLength >= 2 || (flags & 1)) renderedLength++;
                } else {
                    if (conversion == 'f') {
                        renderedLength = precision + 2;
                        if (decimalPosition > 0) {
                            renderedLength = decimalPosition;
                            if (precision != 0 || (flags & 1))
                                renderedLength += precision + 1;
                        }
                    } else if (decimalPosition >= digitLength) {
                        renderedLength = decimalPosition;
                        if (flags & 1) renderedLength++;
                    } else if (decimalPosition > 0) {
                        renderedLength = digitLength + 1;
                    } else {
                        renderedLength = digitLength - decimalPosition + 2;
                    }
                }
                if (floatSign != 0) signByte = '-';
                textBytes = renderedLength;
            }
            goto emit_field;
        default:
            signByte = 0;
            if (conversion == '%') numberBuffer[0] = '%';
            else numberBuffer[0] = (u8)conversion;
            if (conversion == 0) goto flush_output;
            text = numberBuffer;
            textBytes = 1;
            goto emit_field;
        }
format_integer:
        {
            const char *alphabet = conversion == 'X' ? printfHexUpper : printfHexLower;
            char *end = numberBuffer + sizeof(numberBuffer);
            text = end;
            nonzero = unsignedLong != 0;
            integerPrecision = precision;
            if (precision >= 0) flags &= ~0x80;
            if ((conversion == 'x' || conversion == 'X') && (flags & 1) && nonzero)
                flags |= 2;
            if (precision != 0 || unsignedLong != 0) {
                if (base == 1) {
                    while (unsignedLong >= 10) {
                        *--text = (char)('0' + unsignedLong % 10);
                        unsignedLong /= 10;
                    }
                    *--text = (char)('0' + unsignedLong);
                } else if (base == 0) {
                    do {
                        *--text = (char)('0' + (unsignedLong & 7));
                        unsignedLong >>= 3;
                    } while (unsignedLong != 0);
                    if ((flags & 1) && *text != '0') *--text = '0';
                } else if (base == 2) {
                    do {
                        *--text = alphabet[unsignedLong & 15];
                        unsignedLong >>= 4;
                    } while (unsignedLong != 0);
                } else {
                    text = (char *)printfBadBaseMsg;
                    textBytes = strlen(text);
                    goto emit_field;
                }
            }
            textBytes = end - text;
        }

emit_field:
        precisionZeros = integerPrecision > textBytes ? integerPrecision - textBytes : 0;
        fieldBytes = textBytes;
        if (integerPrecision > fieldBytes) fieldBytes = integerPrecision;
        if (signByte != 0) fieldBytes++;
        else fieldBytes += flags & 2;
        if (width > fieldBytes) padBytes = width - fieldBytes;
        else padBytes = 0;
        if ((flags & 0x84) == 0 && (flags & 4) == 0) {
            PAD(printfSpaces, padBytes);
        }
        if (signByte != 0) {
            PRINT(&signByte, 1);
        } else if (flags & 2) {
            prefix[0] = '0';
            prefix[1] = conversion;
            PRINT(prefix, 2);
        }
        if ((flags & 0x80) && !(flags & 4) && width > fieldBytes) {
            PAD(printfZeros, width - fieldBytes);
        }
        if (precisionZeros > 0) PAD(printfZeros, precisionZeros);
        if ((flags & 0x100) == 0) {
            PRINT(text, textBytes);
        } else if (conversion == 'e' || conversion == 'E') {
            if (digitLength < 2 && !(flags & 1)) {
                PRINT(text, 1);
            } else {
                prefix[0] = text[0];
                prefix[1] = '.';
                PRINT(prefix, 2);
                if (dpcmp(floatValue, 0.0) != 0) {
                    PRINT(text + 1, digitLength - 1);
                } else if (digitLength - 1 > 0) {
                    PAD(printfZeros, digitLength - 1);
                }
            }
            PRINT(exponentBuffer, exponentLength);
        } else if (dpcmp(floatValue, 0.0) == 0) {
            PRINT(printfZeroDigit, 1);
            if (decimalPosition < digitLength || (flags & 1)) PRINT(decimalPoint, 1);
            if (digitLength - 1 > 0) PAD(printfZeros, digitLength - 1);
        } else if (decimalPosition <= 0) {
            PRINT(printfZeroDigit, 1);
            if (decimalPosition != 0 || digitLength != 0) {
                PRINT(decimalPoint, 1);
                if (decimalPosition < 0) PAD(printfZeros, -decimalPosition);
                PRINT(text, digitLength);
            }
        } else if (decimalPosition < digitLength) {
            PRINT(text, decimalPosition);
            PRINT(printfDecimalPoint, 1);
            PRINT(text + decimalPosition, digitLength - decimalPosition);
        } else {
            PRINT(text, digitLength);
            if (decimalPosition > digitLength)
                PAD(printfZeros, decimalPosition - digitLength);
            if (flags & 1) PRINT(printfDecimalPoint, 1);
        }
        if (flags & 4) {
            padBytes = width - fieldBytes;
            if (padBytes > 0) {
                PAD(printfSpaces, padBytes);
            }
        }
        if (width > fieldBytes) cumulative += width;
        else cumulative += fieldBytes;
        if (uio.uio_resid != 0) {
            if (__sprint(fp, &uio) != 0) goto error_exit;
        }
        uio.uio_iovcnt = 0;
        current = iov;
    }

flush_output:
    if (uio.uio_resid != 0 && __sprint(fp, &uio) != 0) goto error_exit;
error_exit:
#undef PAD
#undef PRINT
    return (fp->_flags & 0x40) != 0 ? -1 : cumulative;
}

#else

INCLUDE_RODATA(const s32, "sdk/libc_vfprintf", D_003B7CA0);

INCLUDE_RODATA(const s32, "sdk/libc_vfprintf", D_003B7CB0);

INCLUDE_ASM(const s32, "sdk/libc_vfprintf", _vfprintf_r);
#endif

char *cvt(_reent *r, f64 value, s32 precision, s32 flags, char *sign,
          s32 *decimalPoint, s32 format, s32 *length)
{
    union {
        f64 value;
        u64 bits;
    } input;
    s32 mode = 3;
    s32 dtoaSign;
    char *end;
    char *digits;
    char *limit;

    if (format != 'f') {
        if (format == 'e' || format == 'E') {
            precision++;
        }
        mode = 2;
    }
    input.value = value;
    if ((s32)(input.bits >> 32) < 0) {
        value = 0.0 - value;
        *sign = '-';
    } else {
        *sign = 0;
    }
    digits = _dtoa_r(r, value, mode, precision, decimalPoint, &dtoaSign, &end);
    if ((format != 'g' && format != 'G') || (flags & 1)) {
        limit = digits + precision;
        if (format == 'f') {
            if (*digits == '0' && value != 0.0) {
                *decimalPoint = 1 - precision;
            }
            limit += *decimalPoint;
        }
        if (value == 0.0) {
            end = limit;
        }
        while (end < limit) {
            *end++ = '0';
        }
    }
    *length = end - digits;
    return digits;
}

s32 exponent(char *output, s32 value, s32 marker)
{
    /* The reverse-digit cursor ends at stack + 0x134 in the 0x140-byte frame. */
    char digits[0x134];
    char *cursor = output;
    char *digit;

    *cursor++ = marker;
    if (value < 0) {
        value = -value;
        *cursor++ = '-';
    } else {
        *cursor++ = '+';
    }
    digit = digits + sizeof(digits);
    if (value > 9) {
        while (value > 9) {
            *--digit = value % 10 + '0';
            value /= 10;
        }
        *--digit = value + '0';
        while (digit < digits + sizeof(digits)) {
            *cursor++ = *digit++;
        }
    } else {
        *cursor++ = '0';
        *cursor++ = value + '0';
    }
    return cursor - output;
}

