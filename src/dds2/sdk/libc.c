#include "sdk_libc.h"
#include "common.h"
#include "sdk_libgcc.h"
#include "newlib_stat.h"
#include "ps2asm.h"

enum {
    SDF_TMX_MAGIC = 0x30584D54,
    SDF_STREAM_CHUNK_BYTES = 0x4000,
    SDF_STREAM_COMMAND_CREATE_STATE = 32,
    SDF_STREAM_COMMAND_CONTROL = 33,
    SDF_STREAM_COMMAND_READ = 34,
    SDF_STREAM_COMMAND_RELEASE_STATE = 35,
    SDF_IOP_MODULES_PER_GROUP = 2,
    SDF_KEY_TREE_PATH_LIMIT = 32,
    SDF_POOL_REBUILD_CHAIN = 0,
    SDF_POOL_PUSH_FREE = 1
};

struct sdfDrawTag_t;

struct sdfSortOtNode_tag;

struct sdfGrOt_t;

#define SDF_POOL_FREE_KIND 0xFFFF

#include "sdk_libkernl.h"

extern void SleepThread(void);

extern s32 sceSifLoadModule(const char *, s32, const char *);

extern char *strcpy(char *, const char *);

extern char *strcat(char *, const char *);

extern void sceMcInit(void);

extern void sdfGrJoinDrawTag(void *aFore, void *aBack);

/* Pool release logic tests the packet head address and kind at 0xC;
 * bytes at 0x8 remain opaque. The float-key tree insertion helper reads the
 * same +0xC word through sdfDrawTag_t. DDS1/2 game/code_002EEA18/003478C0.c. */
typedef struct sdfDrawTag_t {
    struct sdfDrawTag_t *next; /* 0x00 */
    s32 head;       /* 0x04: first DMA packet address. */
    u8 tail[4];                  /* 0x08 */
    f32 z;                    /* 0x0C */
} sdfDrawTag_t;

typedef struct sdfSortOtNode_tag {
    struct sdfSortOtNode_tag *right; /* 0x00 */
    struct sdfSortOtNode_tag *left;  /* 0x04 */
    struct sdfDrawTag_t *tag;  /* 0x08 */
    s32 balance;                  /* 0x0C */
} sdfSortOtNode_tag;

/* A key-sorted draw-surface pool: a draw tag whose append and prepend
 * callbacks release a node into the pool and update its free list. */
typedef struct sdfGrOt_t {
    void *next;                                                /* 0x00: the pool entry's link */
    sdfDrawTag_t *tagHead;                                     /* 0x04: first node in the emitted chain */
    sdfDrawTag_t *tagTail;                                     /* 0x08: last node in the emitted chain */
    sdfDrawTag_t *noSortTag;                                   /* 0x0C: the free list */
    void (*regFunc)(struct sdfGrOt_t *, sdfDrawTag_t *);       /* 0x10 */
    void (*callbackFunc)(struct sdfGrOt_t *, s32, sdfDrawTag_t *); /* 0x14 */
    sdfSortOtNode_tag *data[1];                                /* 0x18: first root; allocation continues */
} sdfGrOt_t;

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

extern s32 WaitSema(s32);

extern void FlushCache(s32);

extern s32 SignalSema(s32);

extern s32 sceSifDmaStat(s32);

extern s32 GetThreadId(void);

extern void *memcpy(void *, const void *, size_t);

sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *node, sdfSortOtNode_tag *parent);

sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *node, sdfSortOtNode_tag *parent);

void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **stack, s32 stack_depth, sdfSortOtNode_tag *node,
                   sdfSortOtNode_tag **root_p);

void sdfSortOtAvlRegist(sdfSortOtNode_tag **root_p, sdfDrawTag_t *tag);

void sdfGrSortOtReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

void sdfGrSortOtFixup(void *tag, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p);

void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p);

void sdfGrSortOtSetup(sdfGrOt_t *aOt);

void sdfGrSortOtCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData);

typedef struct _reent _reent;

typedef __builtin_va_list va_list;

typedef struct __sbuf {
    u8 *_base;
    s32 _size;
} __sbuf;

typedef struct __sFILE {
    u8 *_p;
    s32 _r;
    s32 _w;
    s16 _flags;
    s16 _file;
    __sbuf _bf;
    s32 _lbfsize;
    void *_cookie;
    s32 (*_read)(void *cookie, char *buf, s32 n);
    s32 (*_write)(void *cookie, const char *buf, s32 n);
    long (*_seek)(void *cookie, s32 offset, s32 whence);
    s32 (*_close)(void *cookie);
    __sbuf _ub;
    u8 *_up;
    s32 _ur;
    u8 _ubuf[3];
    u8 _nbuf[1];
    __sbuf _lb;
    s32 _blksize;
    s32 _offset;
    _reent *_data;
} FILE;

typedef struct __siov {
    void *iov_base;
    u32 iov_len;
} __siov;

typedef struct __suio {
    __siov *uio_iov;
    s32 uio_iovcnt;
    s32 uio_resid;
} __suio;

typedef struct _Bigint {
    struct _Bigint *next;
    s32 k;
    s32 maxwds;
    s32 sign;
    s32 wds;
    u32 x[1];
} _Bigint;

typedef struct _glue {
    struct _glue *next;
    s32 niobs;
    FILE *iobs;
} _glue;

/* libgcc frame-dwarf2.c's object list node. __register_frame_info stores the
 * FDE base and clears the rest; __register_frame allocates one of these. */
typedef struct object {
    void *pc_begin;
    void *pc_end;
    void *fde_begin;
    void *fde_array;
    void *count;
    struct object *next;
} object;

/* libgcc's fde_vector, as add_fdes fills it. */
typedef struct fde_vector {
    u32 **array;
    s32 count;
} fde_vector;

typedef struct _atexit {
    struct _atexit *_next;
    s32 _ind;
    void (*_fns[32])(void);
} _atexit;

struct _reent {
    s32 _errno;
    FILE *_stdin;
    FILE *_stdout;
    FILE *_stderr;
    s32 _inc;
    char _emergency[25];
    s32 _current_category;
    const char *_current_locale;
    s32 __sdidinit;
    void (*__cleanup)(struct _reent *r);
    _Bigint *_result;
    s32 _result_k;
    _Bigint *_p5s;
    _Bigint **_freelist;
    u8 _unused[0xF8];
    _atexit *_atexit;
    _atexit _atexit0;
    void (**_sig_func)(s32 sig);
    struct _glue __sglue;
    FILE __sf[3];
};

extern _reent *_impure_ptr __attribute__((section(".data")));
extern _reent impure_data;

extern s32 _puts_r(_reent *r, const char *s);

extern s32 _raise_r(_reent *r, s32 sig);

extern void *_signal_r(_reent *r, s32 sig, void *handler);

extern char *_setlocale_r(_reent *r, s32 category, const char *locale);

extern void *_localeconv_r(_reent *r);

extern void _cleanup_r(_reent *r);

extern s32 fflush(void *fp);

extern s32 _fwalk(_reent *r, s32 (*func)(void *));

extern s32 libc_2DC670(void);

extern long _strtol_r(_reent *r, const char *s, char **end, s32 base);

extern long strtol(const char *s, char **end, s32 base);

extern s32 raise(s32 sig);

extern void _exit(s32 code) __attribute__((noreturn));

typedef struct {
    char *decimal_point;
    char *thousands_sep;
    char *grouping;
    char *int_curr_symbol;
    char *currency_symbol;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *positive_sign;
    char *negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
} lconv_t;

extern lconv_t localeconvData __attribute__((section(".data")));

extern s32 libcErrno __attribute__((section(".data")));

extern s32 mallocLockCount __attribute__((section(".data")));

extern s32 mallocLockOwner __attribute__((section(".data")));

extern s32 mallocSema __attribute__((section(".data")));

extern s32 _init_signal_r(_reent *r);

extern void std(FILE *fp, s32 flags, s32 file, _reent *data);

extern _glue *__sfmoreglue(_reent *r, s32 n);

extern void __sinit(_reent *r);

extern void __smakebuf(FILE *fp);

extern s32 __sigtramp_r(_reent *r, s32 sig);

extern void *__deregister_frame_info(void *fn);

extern void free(void *ptr);

extern void *malloc(size_t size);

extern void __malloc_lock(_reent *r);

extern void __malloc_unlock(_reent *r);

extern void *_malloc_r(_reent *r, u32 size);

extern void _free_r(_reent *r, void *ptr);

extern s32 SignalSema(s32 sema);

extern void *sbrk(s32 incr);
extern void *_sbrk_r(_reent *r, s32 incr);

extern s32 close(s32 fd);

extern _Bigint *_Balloc(_reent *r, s32 k);

extern void __register_frame_info(void *fn, void *entry);

extern s32 __register_frame_info_table(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, s32 offset, s32 whence);

extern s32 fstat(s32 fd, struct stat *buf);

extern s32 kill(s32 pid, s32 sig);

extern long _read_r(_reent *r, s32 fd, char *buf, s32 n);

extern long _write_r(_reent *r, s32 fd, const char *buf, s32 n);

extern long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence);

extern s32 __sfvwrite(FILE *fp, __suio *uio);

extern s32 __sread(void *cookie, char *buf, s32 n);

extern s32 __swrite(void *cookie, const char *buf, s32 n);

extern long __sseek(void *cookie, s32 offset, s32 whence);

extern s32 __sclose(void *cookie);

extern s32 GetThreadId(void);

extern s32 WaitSema(s32 sema);

extern s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

extern void *memset(void *a, s32 b, size_t c);

extern void *_calloc_r(_reent *r, u32 n, u32 size);

extern const double pow10Table[];

extern void abort(void);

extern void __do_global_ctors(void);

extern s32 atexit(void (*fn)(void));

extern void (**dtorListCursor)(void) __attribute__((section(".data")));

extern s32 dtorFrameDeregistered __attribute__((section(".data")));

extern s32 frameObjectSema __attribute__((section(".data")));

extern u8 __EH_FRAME_BEGIN__[] __attribute__((section(".data")));

extern void (*__CTOR_LIST__[])(void) __attribute__((section(".data")));

extern s32 mainCtorsDone __attribute__((section(".data")));

extern object *frameObjectList __attribute__((section(".data")));

typedef struct fp_number_type {
    s32 class;
    s32 sign;
    s32 normal_exp;
    u32 fraction;
} fp_number_type;

extern void sdfUnpackFloat(const void *x, fp_number_type *u);

extern f32 sdfPackFloat(const fp_number_type *u);

extern f64 __make_dp(s32 kind, s32 sign, s32 exp, s64 mant);

#define va_start(ap, last) __builtin_stdarg_start((ap), last)

extern s32 _vfprintf_r(_reent *r, FILE *fp, const char *fmt, va_list ap);

typedef union f32_bits {
    f32 f;
    u32 u;
} f32_bits;

/* The same record from the double-precision build of fp-bit.c. */
typedef struct fp_number_type_df {
    s32 class;
    s32 sign;
    s32 normal_exp;
    long fraction;
} fp_number_type_df;

extern f64 __pack_df(const fp_number_type_df *u);

extern void sdfUnpackDouble(const double *x, fp_number_type_df *u);

extern fp_number_type_df *libc_2D1590(const fp_number_type_df *a, const fp_number_type_df *b, fp_number_type_df *r);

extern long __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b);

typedef struct mallocChunkPrefix_t {
    u32 previousSize;
    u32 size;
    struct mallocChunkPrefix_t *forward;
    struct mallocChunkPrefix_t *backward;
} mallocChunkPrefix_t;

typedef struct mallocBinLinks_t {
    mallocChunkPrefix_t *forward;
    mallocChunkPrefix_t *backward;
} mallocBinLinks_t;

typedef struct mallocArenaPrefix_t {
    u32 opaque00;
    u32 bitmap;
    mallocChunkPrefix_t *top;
    mallocChunkPrefix_t *topBackward; /* bin 0's backward link; its forward link is top */
    mallocBinLinks_t bins[127];
} mallocArenaPrefix_t;

typedef struct mallocInfo_t {
    s32 arena;
    u8 opaque04[0x24];
} mallocInfo_t;

extern mallocArenaPrefix_t __malloc_av_;
extern mallocInfo_t __malloc_current_mallinfo;
extern void *__malloc_sbrk_base __attribute__((section(".data")));

extern u64 __malloc_trim_threshold __attribute__((section(".data")));
extern u64 __malloc_top_pad __attribute__((section(".data")));
s32 _malloc_trim_r(_reent *r, u32 pad);

typedef struct mallocFenceposts_t {
    u32 previousSize;
    u32 firstSize;
    u32 secondSize;
} mallocFenceposts_t;

extern u64 __malloc_max_sbrked_mem __attribute__((section(".data")));
extern u64 __malloc_max_total_mem __attribute__((section(".data")));

/* NON_MATCHING: 584/600 bytes; second-request width conversions and
 * later register/scheduling choices remain unresolved. */

extern u32 strlen(const char *s);
/* The newline owns a padded 16-byte read-only block in retail. */
extern const char putsNewline[];

typedef s32 (*qsortCompare_t)(const void *, const void *);

static inline char *qsortMedian3(char *a, char *b, char *c, qsortCompare_t compare)
{
    return compare(a, b) < 0 ?
        (compare(b, c) < 0 ? b : (compare(a, c) < 0 ? c : a)) :
        (compare(b, c) > 0 ? b : (compare(a, c) < 0 ? a : c));
}

static inline void qsortSwapRange(char *first, char *second, s32 bytes, s32 swapType)
{
    if (swapType < 2) {
        s64 remaining = (u32)bytes / sizeof(s64);
        s64 *left = (s64 *)first;
        s64 *right = (s64 *)second;
        do {
            s64 temporary = *left;
            *left++ = *right;
            *right++ = temporary;
        } while (--remaining > 0);
    } else {
        s64 remaining = (u32)bytes;
        char *left = first;
        char *right = second;
        do {
            char temporary = *left;
            *left++ = *right;
            *right++ = temporary;
        } while (--remaining > 0);
    }
}

#define QSORT_SWAP_ELEMENT(first, second) { \
    if (swapType == 0) { \
        s64 temporary = *(s64 *)(first); \
        *(s64 *)(first) = *(s64 *)(second); \
        *(s64 *)(second) = temporary; \
    } else { \
        qsortSwapRange(first, second, size, swapType); \
    } \
}

extern s32 _getpid_r(_reent *r);
extern s32 _kill_r(_reent *r, s32 pid, s32 sig);

extern const u8 _ctype_[];

extern const u8 _ctype_[];

/* BUG: the limits are written as 64-bit literals because long is 64 bits on
 * the EE. Where long is 32 bits they truncate to 0 and 0xFFFFFFFF, so every
 * negative number overflows to 0. Same values on the EE, derived from the
 * width of long. */
#ifdef FIX_BUGS
#define STRTOL_MAX (~0UL >> 1)
#define STRTOL_MIN (~(~0UL >> 1))
#else
#define STRTOL_MAX 0x7fffffffffffffffUL
#define STRTOL_MIN 0x8000000000000000UL
#endif

/* Buffer an unbuffered stream using only the fields the native path initializes. */

/* NON_MATCHING: formatter control flow, frame and allocation remain different. */
extern const u8 _ctype_[264];

extern const char D_0054B658[8] __attribute__((section(".rodata")));

extern const char putsNewline[16];

extern const char printfSpaces[16];

extern const char printfZeros[16];

extern char *_dtoa_r(_reent *r, f64 value, s32 mode, s32 digits,
                     s32 *decimalPoint, s32 *sign, char **end);

s32 __mcmp(_Bigint *a, _Bigint *b);

/* NON_MATCHING: native 556-byte extent; 65 differing words in limb-loop allocation. */

/* Little-endian IEEE binary64 word view used by the numeric helpers. */
typedef union libcDoubleBits_t {
    f64 value;
    u32 word[2];
    u64 bits;
} libcDoubleBits_t;

void _Bfree(_reent *r, _Bigint *v);
s32 _hi0bits(u32 x);
_Bigint *_d2b(_reent *r, f64 d, s32 *e, s32 *bitsOut);
_Bigint *_i2b(_reent *r, s32 i);
_Bigint *_multadd(_reent *r, _Bigint *b, s32 m, s32 a);
_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b);
_Bigint *_pow5mult(_reent *r, _Bigint *b, s32 n);
_Bigint *_lshift(_reent *r, _Bigint *b, s32 shift);
_Bigint *__mdiff(_reent *r, _Bigint *a, _Bigint *b);
s32 quorem(_Bigint *dividend, _Bigint *divisor);
extern const char dtoaInfinityText[], dtoaNanText[];
extern const f64 dtoaLog10Slope __attribute__((section(".rodata")));
extern const f64 dtoaLog10Offset __attribute__((section(".rodata")));
extern const f64 dtoaLog10Of2 __attribute__((section(".rodata")));
extern const f64 dtoaPow10Big[];
extern const f64 dtoaPow10Limit __attribute__((section(".rodata")));

extern const char printfNanText[8] __attribute__((section(".rodata")));

extern const char printfHexLower[24];

extern const char printfNullText[8] __attribute__((section(".rodata")));

extern const char printfHexUpper[24];

extern const char printfBadBaseMsg[32];

extern const char printfZeroDigit[8] __attribute__((section(".rodata")));

extern const char printfDecimalPoint[16];

extern const char dtoaInfinityText[16];

extern const char dtoaNanText[8] __attribute__((section(".rodata")));

/* NON_MATCHING: native 756-byte size; chunk-size/successor allocation
 * and coalescing-path scheduling differ in 103 instruction words. */

extern void *memmove(void *destination, const void *source, size_t size);
extern void *memchr(const void *source, s32 character, u32 size);

extern const char localeEmptyStr[];
extern const char localeCName[];
extern s32 strcmp(const char *left, const char *right);

extern s32 _fstat_r(_reent *r, s32 fd, struct stat *buf);
extern s32 isatty(s32 fd);

/* Ordinary source reconstruction of the newlib decimal-string to Bigint helper. */

/* Native indexed constants span the separately labeled word at 0x0054B928. */
extern const s32 pow5MultTable[];
_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b);

/* Qualified allocator controls for the two count lifetimes are recorded in
 * docs/levers/cross-block-locals.md, "Retain bigint limb and bit counts". */

/* The shared pointer and limb intermediates have qualified allocator controls;
 * see docs/levers/cross-block-locals.md, "Retain bigint subtraction values". */

f64 _b2d(_Bigint *b, s32 *exponent);

extern s32 _close_r(_reent *r, s32 fd);

extern f64 dtoaLog10Slope __attribute__((section(".rodata")));

extern f64 dtoaLog10Offset __attribute__((section(".rodata")));

extern const f64 dtoaLog10Of2 __attribute__((section(".rodata")));

extern const char localeEmptyStr[8] __attribute__((section(".rodata")));

extern const char localeDecimalPoint[8] __attribute__((section(".rodata")));

extern const char localeCName[4] __attribute__((section(".rodata")));

extern const s32 pow5MultTable[2] __attribute__((section(".rodata")));

extern const f64 pow10Table[25];

extern const f64 dtoaPow10Big[4];

extern const f64 dtoaPow10Limit __attribute__((section(".rodata")));

extern const f64 dtoaPow10Tiny[5];

extern const char kernlInternalErrorMsg[40];

extern const char ttyMsgPacketTooLarge[40];

extern const char ttyMsgReceiveError[24];

extern const char ttyMsgSendError[24];

extern const char ttyMsgWlenError[24];

void abort(void) {
    while (1) {
        raise(6);
        _exit(1);
    }
}

INCLUDE_ASM(const s32, "sdk/libc", __assert);

s32 atexit(void (*fn)(void)) {
    _atexit *p;

    p = _impure_ptr->_atexit;
    if (p == NULL) {
        p = &_impure_ptr->_atexit0;
        _impure_ptr->_atexit = p;
    }
    if (p->_ind >= 32) {
        if ((p = malloc(sizeof(_atexit))) == NULL) {
            return -1;
        }
        p->_ind = 0;
        p->_next = _impure_ptr->_atexit;
        _impure_ptr->_atexit = p;
    }
    p->_fns[p->_ind++] = fn;
    return 0;
}

s32 atoi(const char *s) {
    return strtol(s, NULL, 10);
}

INCLUDE_ASM(const s32, "sdk/libc", fclose);

s32 fflush(void *stream)
{
    FILE *fp = stream;
    u8 *cursor;
    s32 remaining;
    s32 status;

    if (fp == NULL)
        return _fwalk(_impure_ptr, fflush);
    /* Same initialization phase as __swsetup; see levers/control-flow.md. */
    do {
        if (fp->_data == NULL)
            fp->_data = _impure_ptr;
        if (fp->_data->__sdidinit == 0)
            __sinit(fp->_data);
    } while (0);
    /* Live status scratch; see levers/cross-block-locals.md. */
    status = fp->_flags;
    if ((status & 8) == 0 || (cursor = fp->_bf._base) == NULL)
        return 0;
    remaining = fp->_p - cursor;
    fp->_p = cursor;
    fp->_w = (status & 3) ? 0 : fp->_bf._size;
    for (; remaining > 0; remaining -= status, cursor += status) {
        status = fp->_write(fp->_cookie, (char *)cursor, remaining);
        if (status <= 0) {
            fp->_flags |= 0x40;
            return -1;
        }
    }
    return 0;
}

void std(FILE *fp, s32 flags, s32 file, _reent *data) {
    fp->_p = NULL;
    fp->_r = 0;
    fp->_w = 0;
    fp->_flags = flags;
    fp->_file = file;
    fp->_bf._base = NULL;
    fp->_bf._size = 0;
    fp->_lbfsize = 0;
    fp->_cookie = fp;
    fp->_read = __sread;
    fp->_write = __swrite;
    fp->_seek = __sseek;
    fp->_close = __sclose;
    fp->_data = data;
}

_glue *__sfmoreglue(_reent *r, s32 n) {
    _glue *g;
    FILE *p;

    g = _malloc_r(r, sizeof(_glue) + n * sizeof(FILE));
    if (g == NULL) {
        return NULL;
    }
    p = (FILE *)(g + 1);
    g->next = NULL;
    g->niobs = n;
    g->iobs = p;
    memset(p, 0, n * sizeof(FILE));
    return g;
}

FILE *__sfp(_reent *r)
{
    _glue *glue;
    FILE *fp;
    s32 i;

    if (r->__sdidinit == 0) {
        __sinit(r);
    }
    glue = &r->__sglue;
    for (;;) {
        i = glue->niobs;
        fp = glue->iobs;
        for (i--; i >= 0; i--, fp++) {
            if (fp->_flags == 0) {
                /* Keep initialization outside the short search loop. */
                goto found;
            }
        }
        if (glue->next == NULL) {
            glue->next = __sfmoreglue(r, 4);
            if (glue->next == NULL) {
                break;
            }
        }
        glue = glue->next;
    }
    r->_errno = 12;
    return NULL;

found:
    fp->_flags = 1;
    fp->_file = -1;
    fp->_data = r;
    fp->_p = NULL;
    fp->_w = 0;
    fp->_r = 0;
    fp->_bf._base = NULL;
    fp->_bf._size = 0;
    fp->_lbfsize = 0;
    fp->_ub._base = NULL;
    fp->_ub._size = 0;
    fp->_lb._base = NULL;
    fp->_lb._size = 0;
    return fp;
}

void _cleanup_r(_reent *r) {
    _fwalk(r, fflush);
}

void _cleanup(void) {
    _cleanup_r(_impure_ptr);
}

void __sinit(_reent *r) {
    r->__cleanup = _cleanup_r;
    r->__sdidinit = 1;
    std(&r->__sf[0], 4, 0, r);
    std(&r->__sf[1], 9, 1, r);
    std(&r->__sf[2], 0xA, 2, r);
    r->__sglue.next = NULL;
    r->__sglue.niobs = 3;
    r->__sglue.iobs = &r->__sf[0];
}

INCLUDE_ASM(const s32, "sdk/libc", fiprintf);

INCLUDE_ASM(const s32, "sdk/libc", _fopen_r);

INCLUDE_ASM(const s32, "sdk/libc", fopen);

INCLUDE_ASM(const s32, "sdk/libc", fprintf);

INCLUDE_ASM(const s32, "sdk/libc", fread);

#ifdef NON_MATCHING
void _free_r(_reent *r, void *memory)
{
    mallocChunkPrefix_t *chunk;
    mallocChunkPrefix_t *next;
    mallocChunkPrefix_t *forward;
    mallocChunkPrefix_t *backward;
    mallocChunkPrefix_t *bin;
    u32 size;
    u32 nextSize;
    u32 previousSize;
    u32 original;
    u32 scaled;
    s32 index;
    s32 isRemainder;

    if (memory == NULL) return;
    __malloc_lock(r);
    chunk = (mallocChunkPrefix_t *)((u8 *)memory - 8);
    original = chunk->size;
    size = original & ~1U;
    next = (mallocChunkPrefix_t *)((u8 *)chunk + size);
    nextSize = next->size & ~3U;
    if (next == __malloc_av_.top) {
        size += nextSize;
        if ((original & 1) == 0) {
            previousSize = chunk->previousSize;
            chunk = (mallocChunkPrefix_t *)((u8 *)chunk - previousSize);
            size += previousSize;
            backward = chunk->backward;
            forward = chunk->forward;
            forward->backward = backward;
            backward->forward = forward;
        }
        __malloc_av_.top = chunk;
        chunk->size = size | 1;
        if ((u64)size >= __malloc_trim_threshold) {
            _malloc_trim_r(r, (u32)__malloc_top_pad);
        }
        __malloc_unlock(r);
        return;
    }
    next->size = nextSize;
    isRemainder = 0;
    if ((original & 1) == 0) {
        previousSize = chunk->previousSize;
        chunk = (mallocChunkPrefix_t *)((u8 *)chunk - previousSize);
        size += previousSize;
        forward = chunk->forward;
        if (forward == (mallocChunkPrefix_t *)&__malloc_av_.top) {
            isRemainder = 1;
        } else {
            backward = chunk->backward;
            forward->backward = backward;
            backward->forward = forward;
        }
    }
    if ((((mallocChunkPrefix_t *)((u8 *)next + nextSize))->size & 1) == 0) {
        size += nextSize;
        if (!isRemainder && next->forward ==
            (mallocChunkPrefix_t *)(EE_ADDR(&__malloc_av_.bins[0]) - 8)) {
            forward = next->forward;
            forward->backward = chunk;
            isRemainder = 1;
            forward->forward = chunk;
            chunk->forward = forward;
            chunk->backward = forward;
        } else {
            forward = next->forward;
            backward = next->backward;
            forward->backward = backward;
            backward->forward = forward;
        }
    }
    chunk->size = size | 1;
    ((mallocChunkPrefix_t *)((u8 *)chunk + size))->previousSize = size;
    if (!isRemainder) {
        if (size < 0x200) {
            u32 smallIndex = size >> 3;
            backward = (mallocChunkPrefix_t *)((u8 *)&__malloc_av_ + smallIndex * 8);
            forward = backward->forward;
            chunk->backward = backward;
            chunk->forward = forward;
            __malloc_av_.bitmap |= (u32)(1L << (smallIndex >> 2));
        } else {
            scaled = size >> 9;
            if (scaled == 0) index = size >> 3;
            else if (scaled < 5) index = (size >> 6) + 0x38;
            else if (scaled < 0x15) index = scaled + 0x5B;
            else if (scaled < 0x55) index = (size >> 12) + 0x6E;
            else if (scaled < 0x155) index = (size >> 15) + 0x77;
            else if (scaled < 0x555) index = (size >> 18) + 0x7C;
            else index = 0x7E;
            bin = EE_ADDR_AS(mallocChunkPrefix_t *, (EE_ADDR(&__malloc_av_.top) + index * 8 - 8));
            forward = bin->forward;
            if (forward == bin) {
                __malloc_av_.bitmap |= (u32)(1L << (index / 4));
                backward = bin;
            } else {
                while (forward != bin && size < (forward->size & ~3U)) {
                    forward = forward->forward;
                }
                backward = forward->backward;
            }
            chunk->backward = backward;
            chunk->forward = forward;
        }
        backward->forward = chunk;
        forward->backward = chunk;
    }
    __malloc_unlock(r);
}
#else
INCLUDE_ASM(const s32, "sdk/libc", _free_r);
#endif

s32 _malloc_trim_r(_reent *r, u32 pad)
{
    s64 topSize;
    s64 extra;
    void *newBreak;

    __malloc_lock(r);
    topSize = __malloc_av_.top->size & ~3U;
    /* Page rounding uses 64 bits; break increments and chunk words narrow. */
    extra = (((((u64)topSize - pad + 0xFEF) >> 12) - 1) << 12);
    if (extra < 0x1000) {
        __malloc_unlock(r);
        return 0;
    }
    if (_sbrk_r(r, 0) != (u8 *)__malloc_av_.top + (s32)topSize) {
        __malloc_unlock(r);
        return 0;
    }
    if (_sbrk_r(r, -(s32)extra) == (void *)-1) {
        newBreak = _sbrk_r(r, 0);
        topSize = (s32)(EE_ADDR(newBreak) - EE_ADDR(__malloc_av_.top));
        if (topSize >= 16) {
            __malloc_current_mallinfo.arena =
                EE_ADDR(newBreak) - EE_ADDR(__malloc_sbrk_base);
            __malloc_av_.top->size = topSize | 1;
        }
        __malloc_unlock(r);
        return 0;
    }
    __malloc_av_.top->size = (topSize - extra) | 1;
    __malloc_current_mallinfo.arena -= (s32)extra;
    __malloc_unlock(r);
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libc", func_0035A0A0);

s32 _fstat_r(_reent *r, s32 fd, struct stat *buf) {
    s32 result;

    libcErrno = 0;
    result = fstat(fd, buf);
    if (result == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

s32 _fwalk(_reent *r, s32 (*func)(void *)) {
    _glue *g;
    FILE *fp;
    s32 n;
    s32 ret = 0;

    for (g = &r->__sglue; g != NULL; g = g->next) {
        for (fp = g->iobs, n = g->niobs; --n >= 0; fp++) {
            if (fp->_flags != 0) {
                ret |= func(fp);
            }
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "sdk/libc", fwrite);

void __smakebuf(FILE *fp)
{
    struct stat st;
    u8 *buf;
    s32 size;
    s32 couldBeTty;

    if (fp->_flags & 2) {
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
        return;
    }
    if (fp->_file < 0 || _fstat_r(fp->_data, fp->_file, &st) < 0) {
        couldBeTty = 0;
        size = 1024;
        fp->_flags |= 0x800;
    } else {
        couldBeTty = (st.st_mode & 0xF000) == 0x2000;
        size = 1024;
        if ((st.st_mode & 0xF000) == 0x8000 && fp->_seek == __sseek) {
            fp->_flags |= 0x400;
            fp->_blksize = size;
        } else {
            fp->_flags |= 0x800;
        }
    }
    buf = _malloc_r(fp->_data, size);
    if (buf == NULL) {
        fp->_flags |= 2;
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
    } else {
        fp->_data->__cleanup = _cleanup_r;
        fp->_flags |= 0x80;
        fp->_bf._base = fp->_p = buf;
        fp->_bf._size = size;
        if (couldBeTty && isatty(fp->_file)) fp->_flags |= 1;
    }
}

void *malloc(size_t size) {
    void *result;

    __malloc_lock(_impure_ptr);
    result = _malloc_r(_impure_ptr, size);
    __malloc_unlock(_impure_ptr);
    return result;
}

void free(void *ptr) {
    __malloc_lock(_impure_ptr);
    _free_r(_impure_ptr, ptr);
    __malloc_unlock(_impure_ptr);
}

#ifdef NON_MATCHING
void malloc_extend_top(_reent *r, u32 needed)
{
    mallocChunkPrefix_t *oldTop = __malloc_av_.top;
    u32 oldSize = oldTop->size & ~3U;
    u8 *oldEnd = (u8 *)oldTop + oldSize;
    saddr_t increment = needed + __malloc_top_pad + 16;
    s32 correction;
    u32 misalignment;
    u8 *newTop;
    u8 *secondBreak;
    mallocFenceposts_t *fenceposts;

    if (__malloc_sbrk_base != (void *)-1) {
        increment = ((u64)(u32)increment + 0xFFF) & ~0xFFFULL;
    }
    newTop = _sbrk_r(r, increment);
    if (newTop == (void *)-1 ||
        (EE_ADDR(newTop) < EE_ADDR(oldEnd) && oldTop != (void *)&__malloc_av_)) {
        return;
    }
    __malloc_current_mallinfo.arena += increment;
    if (newTop == oldEnd) {
        __malloc_av_.top->size = (increment + oldSize) | 1;
    } else {
        if (__malloc_sbrk_base == (void *)-1) {
            __malloc_sbrk_base = newTop;
        } else {
            __malloc_current_mallinfo.arena += EE_ADDR(newTop) - EE_ADDR(oldEnd);
        }
        misalignment = (EE_ADDR(newTop) + 8) & 15;
        if (misalignment != 0) {
            correction = 16 - misalignment;
            newTop += correction;
        } else {
            correction = 0;
        }
        correction += (s32)(0x1000UL - ((EE_ADDR(newTop) + increment) & 0xFFF));
        secondBreak = _sbrk_r(r, correction);
        if (secondBreak == (void *)-1) return;
        __malloc_current_mallinfo.arena += correction;
        __malloc_av_.top = (mallocChunkPrefix_t *)newTop;
        __malloc_av_.top->size = (EE_ADDR(secondBreak) - EE_ADDR(newTop) + correction) | 1;
        if (oldTop != (void *)&__malloc_av_) {
            if (oldSize < 16) {
                __malloc_av_.top->size = 1;
                return;
            }
            oldSize = (oldSize - 12) & ~15U;
            oldTop->size = oldSize | (oldTop->size & 1);
            fenceposts = (mallocFenceposts_t *)((u8 *)oldTop + oldSize);
            fenceposts->firstSize = 5;
            fenceposts->secondSize = 5;
            if (oldSize >= 16) {
                _free_r(r, (u8 *)oldTop + 8);
            }
        }
    }
    if (__malloc_max_sbrked_mem < (u64)__malloc_current_mallinfo.arena) {
        __malloc_max_sbrked_mem = __malloc_current_mallinfo.arena;
    }
    if (__malloc_max_total_mem < (u64)__malloc_current_mallinfo.arena) {
        __malloc_max_total_mem = __malloc_current_mallinfo.arena;
    }
}
#else
INCLUDE_ASM(const s32, "sdk/libc", malloc_extend_top);
#endif

INCLUDE_ASM(const s32, "sdk/libc", _malloc_r);

INCLUDE_ASM(const s32, "sdk/libc", memcmp);

INCLUDE_ASM(const s32, "sdk/libc", memcpy);

INCLUDE_ASM(const s32, "sdk/libc", memmove);

INCLUDE_ASM(const s32, "sdk/libc", memset);

void __malloc_lock(_reent *r) {
    s32 owner = GetThreadId();

    if (mallocLockOwner == owner) {
        mallocLockCount++;
    } else {
        WaitSema(mallocSema);
        mallocLockOwner = owner;
        mallocLockCount++;
    }
}

void __malloc_unlock(_reent *r) {
    if (--mallocLockCount != 0) {
        return;
    }
    mallocLockOwner = -1;
    SignalSema(mallocSema);
}

INCLUDE_ASM(const s32, "sdk/libc", _open_r);

s32 _printf_r(_reent *r, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    return _vfprintf_r(r, r->_stdout, fmt, ap);
}

s32 printf(const char *fmt, ...) {
    _reent *r = _impure_ptr;
    va_list ap;

    va_start(ap, fmt);
    r->_stdout->_data = r;
    return vfprintf(r->_stdout, fmt, ap);
}

s32 _puts_r(_reent *r, const char *s)
{
    __suio request;
    __siov vectors[2];
    u32 length = strlen(s);

    vectors[0].iov_base = (void *)s;
    vectors[0].iov_len = length;
    vectors[1].iov_base = (void *)putsNewline;
    vectors[1].iov_len = 1;
    request.uio_resid = length + 1;
    request.uio_iov = vectors;
    request.uio_iovcnt = 2;
    return __sfvwrite(r->_stdout, &request) != 0 ? -1 : 10;
}

s32 puts(const char *s) {
    return _puts_r(_impure_ptr, s);
}

void qsort(void *array, u32 count, u32 size, qsortCompare_t compare)
{
    char *base = array;
    char *pa;
    char *pb;
    char *pc;
    char *pd;
    char *pl;
    char *pm;
    char *pn;
    s32 swapType;
    s32 swapped;
    /* Comparator result, then byte spans; see levers/cross-block-locals.md. */
    s32 scratch;
    s32 sampleStep;

sortLoop:
    swapType = (EE_ADDR(base) % sizeof(s64) || size % sizeof(s64)) ? 2 :
               (size == sizeof(s64) ? 0 : 1);
    swapped = 0;
    if (count < 7) {
        for (pm = base + size; pm < base + count * size; pm += size) {
            for (pl = pm; pl > base && compare(pl - size, pl) > 0; pl -= size) {
                QSORT_SWAP_ELEMENT(pl, pl - size);
            }
        }
        return;
    }
    pm = base + (count / 2) * size;
    if (count > 7) {
        pl = base;
        pn = base + (count - 1) * size;
        if (count > 40) {
            sampleStep = (count / 8) * size;
            pl = qsortMedian3(pl, pl + sampleStep, pl + 2 * sampleStep, compare);
            pm = qsortMedian3(pm - sampleStep, pm, pm + sampleStep, compare);
            pn = qsortMedian3(pn - 2 * sampleStep, pn - sampleStep, pn, compare);
        }
        pm = qsortMedian3(pl, pm, pn, compare);
    }
    QSORT_SWAP_ELEMENT(base, pm);
    pa = pb = base + size;
    pc = pd = base + (count - 1) * size;
    for (;;) {
        while (pb <= pc && (scratch = compare(pb, base)) <= 0) {
            if (scratch == 0) {
                swapped = 1;
                QSORT_SWAP_ELEMENT(pa, pb);
                pa += size;
            }
            pb += size;
        }
        while (pb <= pc && (scratch = compare(pc, base)) >= 0) {
            if (scratch == 0) {
                swapped = 1;
                QSORT_SWAP_ELEMENT(pc, pd);
                pd -= size;
            }
            pc -= size;
        }
        if (pb > pc) {
            break;
        }
        QSORT_SWAP_ELEMENT(pb, pc);
        swapped = 1;
        pb += size;
        pc -= size;
    }
    if (swapped == 0) {
        for (pm = base + size; pm < base + count * size; pm += size) {
            for (pl = pm; pl > base && compare(pl - size, pl) > 0; pl -= size) {
                QSORT_SWAP_ELEMENT(pl, pl - size);
            }
        }
        return;
    }
    pn = base + count * size;
    scratch = pa - base < pb - pa ? pa - base : pb - pa;
    if (scratch > 0) {
        qsortSwapRange(base, pb - scratch, scratch, swapType);
    }
    scratch = pd - pc < pn - pd - size ? pd - pc : pn - pd - size;
    if (scratch > 0) {
        qsortSwapRange(pb, pn - scratch, scratch, swapType);
    }
    scratch = pb - pa;
    if ((u32)scratch > size) {
        qsort(base, (u32)scratch / size, size, compare);
    }
    scratch = pd - pc;
    if ((u32)scratch > size) {
        base = pn - scratch;
        count = (u32)scratch / size;
        goto sortLoop;
    }
}

INCLUDE_ASM(const s32, "sdk/libc", srand);

INCLUDE_ASM(const s32, "sdk/libc", rand);

INCLUDE_ASM(const s32, "sdk/libc", func_0035C268);

INCLUDE_ASM(const s32, "sdk/libc", __srefill);

void *_sbrk_r(_reent *r, s32 incr) {
    void *result;

    libcErrno = 0;
    result = sbrk(incr);
    if (result == (void *)-1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

s32 _init_signal_r(_reent *r) {
    s32 i;

    if (r->_sig_func == NULL) {
        r->_sig_func = _malloc_r(r, 32 * sizeof(void *));
        if (r->_sig_func == NULL) {
            return -1;
        }
        for (i = 0; i < 32; i++) {
            r->_sig_func[i] = NULL;
        }
    }
    return 0;
}

void *_signal_r(_reent *r, s32 sig, void *handler) {
    void *old;

    if ((u32)sig >= 32) {
        r->_errno = 22;
        return (void *)-1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return (void *)-1;
    }
    old = (void *)r->_sig_func[sig];
    r->_sig_func[sig] = (void (*)(s32))handler;
    return old;
}

s32 _raise_r(_reent *r, s32 sig)
{
    void (*handler)(s32);

    if ((u32)sig >= 32) {
        r->_errno = 22;
        return -1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return -1;
    }
    handler = r->_sig_func[sig];
    if (handler == NULL) {
        return _kill_r(r, _getpid_r(r), sig);
    }
    if (handler == (void (*)(s32))1) {
        return 0;
    }
    if (handler == (void (*)(s32))-1) {
        r->_errno = 22;
        return 1;
    }
    r->_sig_func[sig] = NULL;
    handler(sig);
    return 0;
}

s32 __sigtramp_r(_reent *r, s32 sig) {
    void (*handler)(s32 sig);

    if ((u32)sig >= 32) {
        return -1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return -1;
    }
    handler = r->_sig_func[sig];
    if (handler == NULL) {
        return 1;
    }
    if (handler == (void (*)(s32))-1) {
        return 2;
    }
    if (handler == (void (*)(s32))1) {
        return 3;
    }
    r->_sig_func[sig] = NULL;
    handler(sig);
    return 0;
}

s32 raise(s32 sig) {
    return _raise_r(_impure_ptr, sig);
}

void *signal(s32 sig, void *handler) {
    return _signal_r(_impure_ptr, sig, handler);
}

s32 _init_signal(void) {
    return _init_signal_r(_impure_ptr);
}

s32 __sigtramp(s32 sig) {
    return __sigtramp_r(_impure_ptr, sig);
}

s32 _kill_r(_reent *r, s32 pid, s32 sig) {
    s32 result;

    libcErrno = 0;
    result = kill(pid, sig);
    if (result == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

s32 _getpid_r(_reent *r) {
    return libc_2DC670();
}

INCLUDE_ASM(const s32, "sdk/libc", _vsprintf_r);

s32 sprintf(char *str, const char *fmt, ...) {
    s32 result;
    FILE f;
    va_list ap;

    va_start(ap, fmt);
    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = _impure_ptr;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

INCLUDE_ASM(const s32, "sdk/libc", func_0035C8F0);

INCLUDE_ASM(const s32, "sdk/libc", sscanf);

s32 __sread(void *cookie, char *buf, s32 n) {
    FILE *fp = cookie;
    s32 result;

    result = _read_r(fp->_data, fp->_file, buf, n);
    if (result >= 0) {
        fp->_offset += result;
    } else {
        fp->_flags &= ~0x1000;
    }
    return result;
}

s32 __swrite(void *cookie, const char *buf, s32 n) {
    FILE *fp = cookie;

    if (fp->_flags & 0x100) {
        _lseek_r(fp->_data, fp->_file, 0, 2);
    }
    fp->_flags &= ~0x1000;
    return _write_r(fp->_data, fp->_file, buf, n);
}

long __sseek(void *cookie, s32 offset, s32 whence) {
    FILE *fp = cookie;
    long result;

    if ((result = _lseek_r(fp->_data, fp->_file, offset, whence)) == -1) {
        fp->_flags &= ~0x1000;
    } else {
        fp->_flags |= 0x1000;
        fp->_offset = result;
    }
    return result;
}

s32 __sclose(void *cookie) {
    FILE *fp = cookie;

    return _close_r(fp->_data, fp->_file);
}

s32 strcasecmp(const char *left, const char *right)
{
    const s8 *s = (const s8 *)left;
    const s8 *t = (const s8 *)right;

    /* Retail signed-byte classification can reach the adjacent mapped CFI
     * table, and 0xE0 versus NUL can advance beyond the right terminator.
     * Use EE integer addresses for these native mapped-memory operations. */
    while (*s &&
           ({ s32 c = *s;
              (*EE_ADDR_AS(const u8 *, EE_ADDR(_ctype_) + 1 + c) & 1)
                  ? c + 32 : c; }) ==
           ({ s32 c = *t;
              (*EE_ADDR_AS(const u8 *, EE_ADDR(_ctype_) + 1 + c) & 1)
                  ? c + 32 : c; })) {
        s = EE_ADDR_AS(const s8 *, EE_ADDR(s) + 1);
        t = EE_ADDR_AS(const s8 *, EE_ADDR(t) + 1);
    }
    return ({ s32 c = *(const u8 *)s;
              ((_ctype_ + 1)[c] & 1) ? c + 32 : c; }) -
           ({ s32 c = *(const u8 *)t;
              ((_ctype_ + 1)[c] & 1) ? c + 32 : c; });
}

INCLUDE_ASM(const s32, "sdk/libc", strcat);

INCLUDE_ASM(const s32, "sdk/libc", strchr);

INCLUDE_ASM(const s32, "sdk/libc", strcmp);

INCLUDE_ASM(const s32, "sdk/libc", strcpy);

INCLUDE_ASM(const s32, "sdk/libc", strlen);

INCLUDE_ASM(const s32, "sdk/libc", strncmp);

INCLUDE_ASM(const s32, "sdk/libc", strncpy);

char *strrchr(const char *s, s32 c) {
    char ch = c;
    const char *last = NULL;

    while (*s != '\0') {
        if (*s == ch) {
            last = s;
        }
        s++;
    }
    return (char *)(*s != ch ? last : s);
}

char *strstr(const char *haystack, const char *needle)
{
    /* Signed byte views preserve the native loads; comparisons use equality. */
    const s8 *s = (const s8 *)haystack;
    const s8 *n = (const s8 *)needle;
    s32 i;
    if (*s == 0)
        return *n == 0 ? (char *)s : 0;
    do {
        i = 0;
        for (;;) {
            if (n[i] == 0)
                return (char *)s;
            if (n[i] != s[i])
                break;
            ++i;
        }
        ++s;
    } while (*s != 0);
    return 0;
}

long _strtol_r(_reent *r, const char *input, char **end, s32 base)
{
    const char *s = input;
    unsigned long acc;
    s32 c;
    unsigned long cutoff;
    s32 negative = 0;
    s32 any;
    s32 cutlim;

    do {
        c = *s++;
    } while ((_ctype_ + 1)[c] & 8);
    if (c == '-') {
        negative = 1;
        c = *s++;
    } else if (c == '+') {
        c = *s++;
    }
    if ((base == 0 || base == 16) && c == '0' &&
        (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = negative ? STRTOL_MIN : STRTOL_MAX;
    cutlim = cutoff % (unsigned long)base;
    cutoff /= (unsigned long)base;
    for (acc = 0, any = 0;; c = *s++) {
        if ((_ctype_ + 1)[c] & 4)
            c -= '0';
        else if ((_ctype_ + 1)[c] & 3)
            c -= ((_ctype_ + 1)[c] & 1) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            goto done;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
            any = -1;
        } else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
done:
    if (any < 0) {
        acc = negative ? STRTOL_MIN : STRTOL_MAX;
        r->_errno = 34;
    } else if (negative) {
        acc = -acc;
    }
    if (end != 0)
        *end = (char *)(any ? s - 1 : input);
    return acc;
}

long strtol(const char *s, char **end, s32 base) {
    return _strtol_r(_impure_ptr, s, end, base);
}

INCLUDE_ASM(const s32, "sdk/libc", func_0035D8F0);

INCLUDE_ASM(const s32, "sdk/libc", func_0035D938);

INCLUDE_ASM(const s32, "sdk/libc", vfiprintf);

INCLUDE_RODATA(const s32, "sdk/libc", _ctype_);

INCLUDE_RODATA(const s32, "sdk/libc", D_00431BD8);

INCLUDE_RODATA(const s32, "sdk/libc", putsNewline);

INCLUDE_RODATA(const s32, "sdk/libc", D_00431BF0);

INCLUDE_RODATA(const s32, "sdk/libc", D_00431C00);

INCLUDE_ASM(const s32, "sdk/libc", _vfiprintf_r);

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

INCLUDE_RODATA(const s32, "sdk/libc", D_00431DE0);

INCLUDE_RODATA(const s32, "sdk/libc", D_00431DF0);

INCLUDE_ASM(const s32, "sdk/libc", _vfprintf_r);
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

INCLUDE_ASM(const s32, "sdk/libc", exponent);

INCLUDE_ASM(const s32, "sdk/libc", func_00360040);

INCLUDE_ASM(const s32, "sdk/libc", func_003600C0);

INCLUDE_ASM(const s32, "sdk/libc", func_003600E8);

INCLUDE_RODATA(const s32, "sdk/libc", D_00431FF0);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432018);

INCLUDE_ASM(const s32, "sdk/libc", func_003601E0);

INCLUDE_ASM(const s32, "sdk/libc", func_00360304);

INCLUDE_ASM(const s32, "sdk/libc", func_00360370);

INCLUDE_ASM(const s32, "sdk/libc", func_00360410);

INCLUDE_ASM(const s32, "sdk/libc", func_00360D30);

INCLUDE_ASM(const s32, "sdk/libc", func_00360D40);

INCLUDE_ASM(const s32, "sdk/libc", func_00360D44);

INCLUDE_ASM(const s32, "sdk/libc", func_00360D78);

s32 vsprintf(char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = _impure_ptr;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

s32 vsprintf_r(_reent *r, char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = r;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

long _write_r(_reent *r, s32 fd, const char *buf, s32 n) {
    long result;

    libcErrno = 0;
    if ((result = write(fd, buf, n)) == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

s32 __swsetup(FILE *fp)
{
    /* Keep attachment and initialization as one phase; see levers/control-flow.md. */
    do {
        if (fp->_data == NULL) fp->_data = _impure_ptr;
        if (!fp->_data->__sdidinit) __sinit(fp->_data);
    } while (0);
    if (!(fp->_flags & 8)) {
        if (!(fp->_flags & 0x10)) return -1;
        if (fp->_flags & 4) {
            if (fp->_ub._base != NULL) {
                if (fp->_ub._base != fp->_ubuf)
                    _free_r(fp->_data, fp->_ub._base);
                fp->_ub._base = NULL;
            }
            fp->_flags &= ~(4 | 0x20);
            fp->_r = 0;
            fp->_p = fp->_bf._base;
        }
        fp->_flags |= 8;
    }
    if (fp->_bf._base == NULL) __smakebuf(fp);
    if (fp->_flags & 1) {
        fp->_w = 0;
        fp->_lbfsize = -fp->_bf._size;
    } else {
        fp->_w = (fp->_flags & 2) ? 0 : fp->_bf._size;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libc", func_00361098);

INCLUDE_ASM(const s32, "sdk/libc", func_003610B0);

s32 _close_r(_reent *r, s32 fd) {
    s32 result;

    libcErrno = 0;
    result = close(fd);
    if (result == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

#ifdef NON_MATCHING
s32 quorem(_Bigint *dividend, _Bigint *divisor)
{
    s32 n = divisor->wds;
    u32 *divisorCursor;
    u32 *divisorEnd;
    u32 *divisorStart;
    u32 *dividendCursor;
    u32 *dividendEnd;
    u32 *dividendStart;
    u32 quotient;
    u32 product;
    u32 carry;
    u32 borrow;
    s32 low;
    s32 highDifference;
    u32 high;

    if (dividend->wds < n) {
        return 0;
    }
    n--;
    divisorStart = divisorCursor = divisor->x;
    divisorEnd = divisorCursor + n;
    dividendStart = dividendCursor = dividend->x;
    dividendEnd = dividendCursor + n;
    quotient = *dividendEnd / (*divisorEnd + 1);
    if (quotient != 0) {
        borrow = 0;
        carry = 0;
        do {
            product = (*divisorCursor & 0xFFFF) * quotient + carry;
            high = (*divisorCursor++ >> 16) * quotient + (product >> 16);
            carry = high >> 16;
            low = (*dividendCursor & 0xFFFF) - (product & 0xFFFF) + borrow;
            borrow = low >> 16;
            ((u16 *)dividendCursor)[0] = low;
            highDifference = (*dividendCursor >> 16) - (high & 0xFFFF) + borrow;
            ((u16 *)dividendCursor)[1] = highDifference;
            dividendCursor++;
            borrow = highDifference >> 16;
        } while (divisorCursor <= divisorEnd);
        if (*dividendEnd == 0) {
            while (dividendStart < --dividendEnd && *dividendEnd == 0) {
                n--;
            }
            dividend->wds = n;
        }
    }
    if (__mcmp(dividend, divisor) >= 0) {
        quotient++;
        borrow = 0;
        carry = 0;
        divisorCursor = divisorStart;
        dividendCursor = dividendStart;
        do {
            product = (*divisorCursor & 0xFFFF) + carry;
            high = (*divisorCursor++ >> 16) + (product >> 16);
            carry = high >> 16;
            low = (*dividendCursor & 0xFFFF) - (product & 0xFFFF) + borrow;
            borrow = low >> 16;
            ((u16 *)dividendCursor)[0] = low;
            highDifference = (*dividendCursor >> 16) - (high & 0xFFFF) + borrow;
            ((u16 *)dividendCursor)[1] = highDifference;
            dividendCursor++;
            borrow = highDifference >> 16;
        } while (divisorCursor <= divisorEnd);
        dividendEnd = dividendStart + n;
        if (*dividendEnd == 0) {
            while (dividendStart < --dividendEnd && *dividendEnd == 0) {
                n--;
            }
            dividend->wds = n;
        }
    }
    return quotient;
}
#else
INCLUDE_ASM(const s32, "sdk/libc", quorem);
#endif

#ifdef NON_MATCHING
char *_dtoa_r(_reent *r, f64 input, s32 mode, s32 digits,
              s32 *decimalPoint, s32 *sign, char **end)
{
    libcDoubleBits_t value, normalized, epsilon;
    _Bigint *b, *S, *mhi, *mlo, *difference, *product;
    char *start, *out;
    const f64 *power;
    f64 estimate, scale, savedValue, remainder, threshold;
    u32 capacity, significand;
    s32 binaryExponent, bitCount, binaryScale, k, kCheck, denormal;
    s32 b2, b5, s2, s5, m2, m5, leftRight, tryQuick;
    s32 limit, limit1, needed, savedLimit, savedK, errors;
    s32 i, j, digit, lowComparison, highComparison, specialBoundary;

    /* The result buffer overwrites this header; restore it before recycling. */
    if (r->_result != NULL) {
        r->_result->k = r->_result_k;
        r->_result->maxwds = 1U << r->_result_k;
        _Bfree(r, r->_result);
        r->_result = NULL;
    }
    value.value = input;
    if ((s32)value.word[1] < 0) {
        *sign = 1;
        value.word[1] &= 0x7FFFFFFFU;
    } else {
        *sign = 0;
    }
    if ((value.word[1] & 0x7FF00000U) == 0x7FF00000U) {
        *decimalPoint = 9999;
        start = (value.bits & 0x000FFFFFFFFFFFFFULL) == 0 ?
                dtoaInfinityText : dtoaNanText;
        if (end != NULL) {
            *end = start[3] == '\0' ? start + 3 : start + 8;
        }
        return start;
    }
    if (value.value == 0.0) {
        *decimalPoint = 1;
        start = "0";
        if (end != NULL) {
            *end = start + 1;
        }
        return start;
    }

    b = _d2b(r, value.value, &binaryExponent, &bitCount);
    binaryScale = (value.word[1] >> 20) & 0x7FF;
    if (binaryScale != 0) {
        normalized.value = value.value;
        normalized.word[1] &= 0xFFFFFU;
        normalized.word[1] |= 0x3FF00000U;
        binaryScale -= 0x3FF;
        denormal = 0;
    } else {
        i = bitCount + binaryExponent;
        j = i + 0x432;
        if (j > 32) {
            significand = (value.word[1] << (64 - j)) |
                          (value.word[0] >> (j - 32));
        } else {
            significand = value.word[0] << (32 - j);
        }
        normalized.value = (f64)significand;
        normalized.word[1] -= 0x01F00000U;
        binaryScale = j - 0x433;
        denormal = 1;
    }
    estimate = ((normalized.value - 1.5) * dtoaLog10Slope + dtoaLog10Offset) +
               (f64)binaryScale * dtoaLog10Of2;
    k = (s32)estimate;
    if (estimate < 0.0 && estimate != (f64)k) {
        k--;
    }
    kCheck = 1;
    if ((u32)k < 23U) {
        if (value.value < pow10Table[k]) {
            k--;
        }
        kCheck = 0;
    }
    i = (bitCount - binaryScale) - 1;
    if (i < 0) {
        s2 = 0;
        b2 = -i;
    } else {
        s2 = i;
        b2 = 0;
    }
    if (k >= 0) {
        b5 = 0;
        s5 = k;
        s2 += k;
    } else {
        b5 = -k;
        s5 = 0;
        b2 -= k;
    }
    tryQuick = 1;
    if ((u32)mode >= 10U) {
        mode = 0;
    }
    if (mode >= 6) {
        mode -= 4;
        tryQuick = 0;
    }
    leftRight = 1;
    limit = limit1 = -1;
    switch (mode) {
    case 0:
    case 1:
        needed = 18;
        digits = 0;
        break;
    case 2:
        leftRight = 0;
        /* fallthrough */
    case 4:
        if (digits <= 0) {
            digits = 1;
        }
        needed = limit = limit1 = digits;
        break;
    case 3:
        leftRight = 0;
        /* fallthrough */
    case 5:
        limit1 = digits + k;
        limit = limit1 + 1;
        needed = limit > 0 ? limit : 1;
        break;
    }
    r->_result_k = 0;
    capacity = 4;
    if ((u32)needed >= 24U) {
        i = 0;
        do {
            capacity *= 2;
            i++;
        } while ((u32)needed >= capacity + 20U);
        r->_result_k = i;
    }
    r->_result = _Balloc(r, r->_result_k);
    start = out = (char *)r->_result;

    if ((u32)limit < 15U && tryQuick != 0) {
        savedValue = value.value;
        savedK = k;
        savedLimit = limit;
        errors = 2;
        if (k > 0) {
            i = k >> 4;
            scale = pow10Table[k & 15];
            if ((i & 16) != 0) {
                i &= 15;
                errors = 3;
                value.value /= dtoaPow10Limit;
            }
            if (i != 0) {
                power = dtoaPow10Big;
                do {
                    if ((i & 1) != 0) {
                        errors++;
                        scale *= *power;
                    }
                    i >>= 1;
                    power++;
                } while (i != 0);
            }
            value.value /= scale;
        } else {
            j = -k;
            if (j != 0) {
                i = j >> 4;
                value.value = pow10Table[j & 15] * value.value;
                if (i != 0) {
                    power = dtoaPow10Big;
                    do {
                        if ((i & 1) != 0) {
                            errors++;
                            value.value = *power * value.value;
                        }
                        i >>= 1;
                        power++;
                    } while (i != 0);
                }
            }
        }
        if (kCheck != 0 && value.value < 1.0 && limit > 0) {
            if (limit1 <= 0) {
                goto quickFailed;
            }
            k--;
            limit = limit1;
            errors++;
            value.value = 10.0 * value.value;
        }
        epsilon.value = (f64)errors * value.value + 7.0;
        epsilon.word[1] -= 0x03400000U;
        if (limit == 0) {
            mhi = NULL;
            S = NULL;
            remainder = value.value - 5.0;
            if (remainder > epsilon.value) {
                goto oneDigit;
            }
            if (remainder < 0.0 - epsilon.value) {
                goto noDigits;
            }
            goto quickFailed;
        }
        if (leftRight != 0) {
            i = 0;
            threshold = 0.5 / pow10Table[limit - 1] - epsilon.value;
            for (;;) {
                digit = (s32)value.value;
                remainder = value.value - (f64)digit;
                *out++ = digit + '0';
                if (remainder < threshold) {
                    goto finish;
                }
                if (1.0 - remainder < threshold) {
                    goto roundQuick;
                }
                i++;
                if (i >= limit) {
                    goto quickFailed;
                }
                threshold = 10.0 * threshold;
                value.value = 10.0 * remainder;
            }
        } else {
            i = 1;
            threshold = pow10Table[limit - 1] * epsilon.value;
            for (;;) {
                digit = (s32)value.value;
                remainder = value.value - (f64)digit;
                *out++ = digit + '0';
                if (i == limit) {
                    if (remainder > 0.5 + threshold) {
                        goto roundQuick;
                    }
                    if (remainder < 0.5 - threshold) {
                        do {
                            out--;
                        } while (*out == '0');
                        out++;
                        goto finish;
                    }
                    goto quickFailed;
                }
                i++;
                value.value = 10.0 * remainder;
            }
        }
quickFailed:
        value.value = savedValue;
        k = savedK;
        limit = savedLimit;
        out = start;
    }

    if (binaryExponent >= 0 && k < 15) {
        scale = pow10Table[k];
        i = 1;
        if (digits < 0 && limit <= 0) {
            mhi = NULL;
            S = NULL;
            if (limit < 0 || value.value <= scale * 5.0) {
                goto noDigits;
            }
            goto oneDigit;
        }
        for (;;) {
            digit = (s32)(value.value / scale);
            remainder = value.value - (f64)digit * scale;
            *out++ = digit + '0';
            if (i == limit) {
                remainder = remainder + remainder;
                if (remainder > scale ||
                    (remainder == scale && (digit & 1) != 0)) {
                    goto roundQuick;
                }
                goto finish;
            }
            value.value = 10.0 * remainder;
            i++;
            if (value.value == 0.0) {
                goto finish;
            }
        }

roundQuick:
    out--;
    while (*out == '9') {
        if (out == start) {
            k++;
            *start = '0';
            break;
        }
        out--;
    }
    *out = (u8)*out + 1;
    out++;
    goto finish;
    }

    m2 = b2;
    m5 = b5;
    mlo = NULL;
    mhi = NULL;
    if (leftRight != 0) {
        if (mode >= 2) {
            i = limit - 1;
            m5 = b5 - i;
            if (b5 < i) {
                m5 = 0;
                j = i - b5;
                b5 += j;
                s5 += j;
            }
            j = limit;
            if (j < 0) {
                m2 = b2 - j;
                j = 0;
            }
        } else {
            j = denormal != 0 ? binaryExponent + 0x433 : 0x36 - bitCount;
        }
        s2 += j;
        mhi = _i2b(r, 1);
        b2 += j;
    }
    if (m2 > 0 && s2 > 0) {
        i = m2 < s2 ? m2 : s2;
        m2 -= i;
        s2 -= i;
        b2 -= i;
    }
    if (b5 > 0) {
        if (leftRight != 0) {
            if (m5 > 0) {
                mhi = _pow5mult(r, mhi, m5);
                product = _multiply(r, mhi, b);
                _Bfree(r, b);
                b = product;
            }
            i = b5 - m5;
            if (i != 0) {
                b = _pow5mult(r, b, i);
            }
        } else {
            b = _pow5mult(r, b, b5);
        }
    }
    S = _i2b(r, 1);
    if (s5 > 0) {
        S = _pow5mult(r, S, s5);
    }
    specialBoundary = 0;
    if (mode < 2 && (value.bits & 0x000FFFFFFFFFFFFFULL) == 0 &&
        (value.word[1] & 0x7FF00000U) != 0) {
        specialBoundary = 1;
        b2++;
        s2++;
    }
    i = s5 != 0 ? s2 - _hi0bits(S->x[S->wds - 1]) : s2 + 1;
    j = i & 31;
    if (j != 0) {
        j = 32 - j;
    }
    if (j > 4) {
        j -= 4;
        m2 += j;
        s2 += j;
        b2 += j;
    } else if (j < 4) {
        j += 28;
        m2 += j;
        s2 += j;
        b2 += j;
    }
    if (b2 > 0) {
        b = _lshift(r, b, b2);
    }
    if (s2 > 0) {
        S = _lshift(r, S, s2);
    }
    if (kCheck != 0 && __mcmp(b, S) < 0) {
        k--;
        b = _multadd(r, b, 10, 0);
        if (leftRight != 0) {
            mhi = _multadd(r, mhi, 10, 0);
        }
        limit = limit1;
    }
    if (limit <= 0 && mode >= 3) {
        if (limit < 0) {
            goto noDigits;
        }
        S = _multadd(r, S, 5, 0);
        if (__mcmp(b, S) > 0) {
            goto oneDigit;
        }
        goto noDigits;
    }

    if (leftRight != 0) {
        if (m2 > 0) {
            mhi = _lshift(r, mhi, m2);
        }
        mlo = mhi;
        if (specialBoundary != 0) {
            mhi = _Balloc(r, mlo->k);
            memcpy(&mhi->sign, &mlo->sign, mlo->wds * sizeof(u32) + 8);
            mhi = _lshift(r, mhi, 1);
        }
        i = 1;
        j = value.word[0] & 1;
        for (;;) {
            digit = quorem(b, S) + '0';
            lowComparison = __mcmp(b, mlo);
            difference = __mdiff(r, S, mhi);
            highComparison = difference->sign != 0 ? 1 : __mcmp(b, difference);
            _Bfree(r, difference);
            if (highComparison == 0 && mode == 0 && j == 0) {
                if (digit == '9') {
                    *out++ = '9';
                    goto roundBigint;
                }
                *out++ = digit + (highComparison < lowComparison);
                goto bigintDone;
            }
            if (lowComparison < 0 ||
                (lowComparison == 0 && mode == 0 && j == 0)) {
                if (highComparison > 0) {
                    b = _lshift(r, b, 1);
                    lowComparison = __mcmp(b, S);
                    if (lowComparison > 0 ||
                        (lowComparison == 0 && (digit & 1) != 0)) {
                        digit++;
                        if (digit == ':') {
                            *out++ = '9';
                            goto roundBigint;
                        }
                    }
                }
                *out++ = digit;
                goto bigintDone;
            }
            if (highComparison > 0) {
                if (digit == '9') {
                    *out++ = '9';
                    goto roundBigint;
                }
                *out++ = digit + 1;
                goto bigintDone;
            }
            *out++ = digit;
            if (i == limit) {
                break;
            }
            b = _multadd(r, b, 10, 0);
            if (mlo == mhi) {
                mlo = mhi = _multadd(r, mhi, 10, 0);
            } else {
                mlo = _multadd(r, mlo, 10, 0);
                mhi = _multadd(r, mhi, 10, 0);
            }
            i++;
        }
    } else {
        for (i = 1;; i++) {
            digit = quorem(b, S) + '0';
            *out++ = digit;
            if (i >= limit) {
                break;
            }
            b = _multadd(r, b, 10, 0);
        }
    }
    b = _lshift(r, b, 1);
    j = __mcmp(b, S);
    if (j > 0 || (j == 0 && (digit & 1) != 0)) {
roundBigint:
        out--;
        for (;;) {
            if (*out != '9') {
                *out = (u8)*out + 1;
                out++;
                break;
            }
            if (out == start) {
                k++;
                *start = '1';
                out = start + 1;
                break;
            }
            out--;
        }
    } else {
        do {
            out--;
        } while (*out == '0');
        out++;
    }
bigintDone:
    _Bfree(r, S);
    if (mhi != NULL) {
        if (mlo != NULL && mlo != mhi) {
            _Bfree(r, mlo);
        }
        _Bfree(r, mhi);
    }
finish:
    _Bfree(r, b);
    *out = '\0';
    *decimalPoint = k + 1;
    if (end != NULL) {
        *end = out;
    }
    return start;

noDigits:
    k = ~digits;
    goto bigintDone;
oneDigit:
    k++;
    out = start + 1;
    *start = '1';
    goto bigintDone;
}
#else

INCLUDE_RODATA(const s32, "sdk/libc", D_00432460);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432470);

INCLUDE_ASM(const s32, "sdk/libc", _dtoa_r);
#endif

INCLUDE_ASM(const s32, "sdk/libc", func_003625C0);

s32 __sfvwrite(FILE *fp, __suio *uio)
{
    u32 length;
    char *p;
    __siov *iov;
    s32 written;
    s32 available;
    s32 newlineKnown;
    s32 newlineDistance;
    char *newline;

    /* One byte count spans the entry and vector phases; see levers/cross-block-locals.md. */
    length = uio->uio_resid;
    if (length == 0)
        return 0;
    if ((fp->_flags & 8) == 0 || fp->_bf._base == 0)
        if (__swsetup(fp) != 0)
            return -1;
    iov = uio->uio_iov;
    length = 0;
    if (fp->_flags & 2) {
        do {
            while (length == 0) {
                p = iov->iov_base;
                length = iov->iov_len;
                iov++;
            }
            written = fp->_write(fp->_cookie, p, length > 1024 ? 1024 : length);
            if (written <= 0)
                goto error;
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    } else if ((fp->_flags & 1) == 0) {
        do {
            while (length == 0) {
                p = iov->iov_base;
                length = iov->iov_len;
                iov++;
            }
            written = fp->_w;
            if (fp->_flags & 0x200) {
                if (length < written)
                    written = length;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
                written = length;
            } else if (fp->_p > fp->_bf._base && length > written) {
                memmove(fp->_p, p, written);
                fp->_p += written;
                if (fflush(fp) != 0)
                    goto error;
            } else if (length >= (written = fp->_bf._size)) {
                written = fp->_write(fp->_cookie, p, written);
                if (written <= 0)
                    goto error;
            } else {
                written = length;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
            }
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    } else {
        newlineKnown = 0;
        do {
            if (length == 0) {
                newlineKnown = 0;
                while (length == 0) {
                    p = iov->iov_base;
                    length = iov->iov_len;
                    iov++;
                }
            }
            if (!newlineKnown) {
                newline = memchr(p, '\n', length);
                newlineDistance = newline ? newline + 1 - p : length + 1;
                newlineKnown = 1;
            }
            available = length < newlineDistance ? length : newlineDistance;
            written = fp->_w + fp->_bf._size;
            if (fp->_p > fp->_bf._base && written < available) {
                memmove(fp->_p, p, written);
                fp->_p += written;
                if (fflush(fp) != 0)
                    goto error;
            } else if (available >= (written = fp->_bf._size)) {
                written = fp->_write(fp->_cookie, p, written);
                if (written <= 0)
                    goto error;
            } else {
                written = available;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
            }
            if ((newlineDistance -= written) == 0) {
                if (fflush(fp) != 0)
                    goto error;
                newlineKnown = 0;
            }
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    }
    return 0;
error:
    fp->_flags |= 0x40;
    return -1;
}

char *_setlocale_r(_reent *r, s32 category, const char *locale)
{
    if (locale != NULL) {
        if (strcmp(locale, localeCName) != 0 && strcmp(locale, localeEmptyStr) != 0)
            return NULL;
        r->_current_category = category;
        r->_current_locale = locale;
    }
    return localeCName;
}

void *_localeconv_r(_reent *r) {
    return &localeconvData;
}

char *setlocale(s32 category, const char *locale) {
    return _setlocale_r(_impure_ptr, category, locale);
}

void *localeconv(void) {
    return _localeconv_r(_impure_ptr);
}

long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence) {
    long result;

    libcErrno = 0;
    result = lseek(fd, offset, whence);
    if (result == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

s32 _mbtowc_r(_reent *r, s32 *pwc, const char *s, u32 n, s32 *state) {
    s32 dummy;
    s32 *out = pwc;

    if (pwc == NULL) {
        out = &dummy;
    }
    if (s == NULL) {
        return 0;
    }
    if (n == 0) {
        return -1;
    }
    *out = *(u8 *)s;
    if (*(u8 *)s == 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libc", memchr);

_Bigint *_Balloc(_reent *r, s32 k) {
    _Bigint *rv;
    s32 x;

    if (r->_freelist == NULL) {
        r->_freelist = _calloc_r(r, 4, 16);
        if (r->_freelist == NULL) {
            return NULL;
        }
    }
    rv = r->_freelist[k];
    if (rv != NULL) {
        r->_freelist[k] = rv->next;
    } else {
        x = 1 << k;
        rv = _calloc_r(r, 1, 0x14 + x * sizeof(u32));
        if (rv == NULL) {
            return NULL;
        }
        rv->k = k;
        rv->maxwds = x;
    }
    rv->sign = rv->wds = 0;
    return rv;
}

void _Bfree(_reent *r, _Bigint *v) {
    if (v != NULL) {
        v->next = r->_freelist[v->k];
        r->_freelist[v->k] = v;
    }
}

_Bigint *_multadd(_reent *r, _Bigint *b, s32 m, s32 a)
{
    s32 wds = b->wds;
    u32 *x = b->x;
    s32 i = 0;
    u32 lo;
    u32 hi;
    _Bigint *b1;

    do {
        lo = (*x & 0xffff) * m + a;
        hi = (*x >> 16) * m + (lo >> 16);
        a = hi >> 16;
        *x++ = (hi << 16) + (lo & 0xffff);
    } while (++i < wds);
    if (a != 0) {
        if (wds >= b->maxwds) {
            b1 = _Balloc(r, b->k + 1);
            memcpy(&b1->sign, &b->sign, b->wds * sizeof(u32) + 2 * sizeof(s32));
            _Bfree(r, b);
            b = b1;
        }
        b->x[wds++] = a;
        b->wds = wds;
    }
    return b;
}

_Bigint *_s2b(_reent *r, const char *s, s32 nd0, s32 nd, u32 y9)
{
    _Bigint *b;
    s32 n;
    s32 k;
    s32 i;
    s32 capacity;

    n = (nd + 8) / 9;
    k = 0;
    capacity = 1;
    while (capacity < n) {
        capacity <<= 1;
        k++;
    }

    b = _Balloc(r, k);
    b->x[0] = y9;
    b->wds = 1;

    i = 9;
    if (nd0 > 9) {
        s += 9;
        do {
            b = _multadd(r, b, 10, *s++ - '0');
        } while (++i < nd0);
        s++;
    } else {
        s += 10;
    }

    while (i < nd) {
        b = _multadd(r, b, 10, *s++ - '0');
        ++i;
    }
    return b;
}

s32 _hi0bits(u32 x) {
    s32 k = 0;

    if ((x & 0xFFFF0000) == 0) {
        k = 16;
        x <<= 16;
    }
    if ((x & 0xFF000000) == 0) {
        k += 8;
        x <<= 8;
    }
    if ((x & 0xF0000000) == 0) {
        k += 4;
        x <<= 4;
    }
    if ((x & 0xC0000000) == 0) {
        k += 2;
        x <<= 2;
    }
    if ((x & 0x80000000) == 0) {
        k++;
        if ((x & 0x40000000) == 0) {
            return 32;
        }
    }
    return k;
}

s32 _lo0bits(u32 *y) {
    s32 k;
    u32 x = *y;

    if (x & 7) {
        if (x & 1) {
            return 0;
        }
        if (x & 2) {
            *y = x >> 1;
            return 1;
        }
        *y = x >> 2;
        return 2;
    }
    k = 0;
    if ((x & 0xFFFF) == 0) {
        k = 16;
        x >>= 16;
    }
    if ((x & 0xFF) == 0) {
        k += 8;
        x >>= 8;
    }
    if ((x & 0xF) == 0) {
        k += 4;
        x >>= 4;
    }
    if ((x & 3) == 0) {
        k += 2;
        x >>= 2;
    }
    if ((x & 1) == 0) {
        k++;
        x >>= 1;
        if (x == 0) {
            return 32;
        }
    }
    *y = x;
    return k;
}

_Bigint *_i2b(_reent *r, s32 i) {
    _Bigint *b = _Balloc(r, 1);

    b->x[0] = i;
    b->wds = 1;
    return b;
}

INCLUDE_ASM(const s32, "sdk/libc", _multiply);

INCLUDE_ASM(const s32, "sdk/libc", func_00363230);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432480);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432488);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432490);

INCLUDE_RODATA(const s32, "sdk/libc", jtbl_004324A0);

INCLUDE_RODATA(const s32, "sdk/libc", localeconvData);

INCLUDE_RODATA(const s32, "sdk/libc", localeEmptyStr);

INCLUDE_RODATA(const s32, "sdk/libc", D_004324F0);

INCLUDE_RODATA(const s32, "sdk/libc", localeCName);

_Bigint *_pow5mult(_reent *r, _Bigint *b, s32 n)
{
    _Bigint *power;
    _Bigint *nextPower;
    _Bigint *product;
    s32 remainder;

    remainder = n & 3;
    if (remainder != 0) {
        b = _multadd(r, b, pow5MultTable[remainder - 1], 0);
    }
    n >>= 2;
    if (n == 0) {
        return b;
    }
    power = r->_p5s;
    if (power == NULL) {
        power = r->_p5s = _i2b(r, 625);
        power->next = NULL;
    }
    for (;;) {
        if (n & 1) {
            product = _multiply(r, b, power);
            _Bfree(r, b);
            b = product;
        }
        n >>= 1;
        if (n == 0) {
            break;
        }
        if ((nextPower = power->next) == NULL) {
            nextPower = power->next = _multiply(r, power, power);
            nextPower->next = NULL;
        }
        power = nextPower;
    }
    return b;
}

INCLUDE_ASM(const s32, "sdk/libc", _lshift);

INCLUDE_ASM(const s32, "sdk/libc", func_00363530);

s32 __mcmp(_Bigint *a, _Bigint *b) {
    u32 *xa, *xa0, *xb, *xb0;
    s32 i, j;

    i = a->wds;
    j = b->wds;
    if ((i -= j) != 0) {
        return i;
    }
    xa0 = a->x;
    xa = xa0 + j;
    xb0 = b->x;
    xb = xb0 + j;
    for (;;) {
        if (*--xa != *--xb) {
            return *xa < *xb ? -1 : 1;
        }
        if (xa <= xa0) {
            break;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libc", __mdiff);

INCLUDE_ASM(const s32, "sdk/libc", func_00363730);

f64 _ulp(f64 x)
{
    libcDoubleBits_t input;
    libcDoubleBits_t result;
    s32 exponent;

    input.value = x;
    exponent = (input.word[1] & 0x7FF00000) - 0x03400000;
    if (exponent > 0) {
        result.word[1] = exponent;
        result.word[0] = 0;
    } else {
        exponent = -exponent >> 20;
        if (exponent < 20) {
            result.word[1] = 0x80000 >> exponent;
            result.word[0] = 0;
        } else {
            result.word[1] = 0;
            exponent -= 20;
            result.word[0] = exponent >= 31 ? 1 : 1 << (31 - exponent);
        }
    }
    return result.value;
}

f64 _b2d(_Bigint *b, s32 *exponent)
{
    libcDoubleBits_t result;
    u32 *first;
    u32 *last;
    u32 top;
    u32 previous;
    s32 leading;

    first = b->x;
    last = first + b->wds;
    top = *--last;
    leading = _hi0bits(top);
    *exponent = 32 - leading;

    if (leading < 11) {
        u32 below;
        result.word[1] = (top >> (11 - leading)) | 0x3FF00000;
        below = first < last ? *--last : 0;
        result.word[0] = (top << (leading + 21)) | (below >> (11 - leading));
    } else {
        if (first < last) {
            last--;
            previous = *last;
        } else {
            previous = 0;
        }
        leading -= 11;
        if (leading != 0) {
            result.word[1] = (top << leading) | (previous >> (32 - leading)) |
                             0x3FF00000;
            if (first < last) {
                last--;
                top = *last;
            } else {
                top = 0;
            }
            result.word[0] = (previous << leading) | (top >> (32 - leading));
        } else {
            result.word[1] = top | 0x3FF00000;
            result.word[0] = previous;
        }
    }
    return result.value;
}

_Bigint *_d2b(_reent *r, f64 d, s32 *e, s32 *bitsOut) {
    libcDoubleBits_t value;
    _Bigint *b;
    u32 highMantissa;
    u32 *limbs;
    s32 exponent;
    s32 trailingBits;
    s32 wordCount;

    b = _Balloc(r, 1);
    value.value = d;
    limbs = b->x;
    highMantissa = value.word[1] & 0xFFFFF;
    value.word[1] &= 0x7FFFFFFF;
    exponent = value.word[1] >> 20;
    if (exponent != 0) {
        highMantissa |= 0x100000;
    }
    /* Target-specific allocation aid: the real raw low-word snapshot uses
     * $5 only through the test and stack copy, before _lo0bits can clobber it.
     * This constrains reconstruction, not the historical source spelling. */
    {
        FAKEMATCH_ASM_REG(u32, rawLowWord, "$5") = value.word[0];
        FAKEMATCH_ASM_LIVE(rawLowWord);
        if (rawLowWord != 0) {
            u32 lowWord = rawLowWord;
            trailingBits = _lo0bits(&lowWord);
            if (trailingBits != 0) {
                limbs[0] = lowWord | (highMantissa << (32 - trailingBits));
                highMantissa >>= trailingBits;
            } else {
                limbs[0] = lowWord;
            }
            wordCount = b->wds = (limbs[1] = highMantissa) != 0 ? 2 : 1;
        } else {
            trailingBits = _lo0bits(&highMantissa) + 32;
            limbs[0] = highMantissa;
            wordCount = b->wds = 1;
        }
    }
    if (exponent != 0) {
        *e = (exponent + trailingBits) - 0x433;
        *bitsOut = 0x35 - trailingBits;
    } else {
        *e = trailingBits - 0x432;
        *bitsOut = (wordCount << 5) - _hi0bits(limbs[wordCount - 1]);
    }
    return b;
}

f64 _ratio(_Bigint *a, _Bigint *b)
{
    libcDoubleBits_t numerator;
    libcDoubleBits_t denominator;
    s32 aExponent;
    s32 bExponent;
    s32 difference;

    numerator.value = _b2d(a, &aExponent);
    denominator.value = _b2d(b, &bExponent);
    difference = aExponent - bExponent + 32 * (a->wds - b->wds);
    if (difference > 0) {
        numerator.word[1] += difference * 0x100000U;
    } else {
        denominator.word[1] -= difference * 0x100000U;
    }
    return numerator.value / denominator.value;
}

double _mprec_log10(s32 dig) {
    double v = 1.0;

    if (dig < 24) {
        return pow10Table[dig];
    }
    while (dig > 0) {
        v *= 10.0;
        dig--;
    }
    return v;
}

long _read_r(_reent *r, s32 fd, char *buf, s32 n) {
    long result;

    libcErrno = 0;
    if ((result = read(fd, buf, n)) == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

INCLUDE_ASM(const s32, "sdk/libc", isinf);

INCLUDE_ASM(const s32, "sdk/libc", isnan);

INCLUDE_RODATA(const s32, "sdk/libc", pow5MultTable);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432508);

INCLUDE_RODATA(const s32, "sdk/libc", pow10Table);

INCLUDE_RODATA(const s32, "sdk/libc", D_004325D8);

INCLUDE_RODATA(const s32, "sdk/libc", D_004325F8);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432600);

INCLUDE_ASM(const s32, "sdk/libc", func_00363DB8);

INCLUDE_ASM(const s32, "sdk/libc", func_00363E28);

INCLUDE_ASM(const s32, "sdk/libc", func_00363E48);

INCLUDE_ASM(const s32, "sdk/libc", func_00364C70);

INCLUDE_ASM(const s32, "sdk/libc", func_00364CD0);

INCLUDE_ASM(const s32, "sdk/libc", func_00364CF8);

INCLUDE_ASM(const s32, "sdk/libc", func_00364D20);

INCLUDE_ASM(const s32, "sdk/libc", func_00364F50);

INCLUDE_ASM(const s32, "sdk/libc", func_00364F80);

INCLUDE_ASM(const s32, "sdk/libc", func_00365068);

INCLUDE_ASM(const s32, "sdk/libc", _calloc_r);

INCLUDE_ASM(const s32, "sdk/libc", _realloc_r);

INCLUDE_RODATA(const s32, "sdk/libc", D_004326E8);

INCLUDE_RODATA(const s32, "sdk/libc", D_004326F0);

INCLUDE_RODATA(const s32, "sdk/libc", D_004326F8);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432700);

INCLUDE_RODATA(const s32, "sdk/libc", kernlInternalErrorMsg);

INCLUDE_RODATA(const s32, "sdk/libc", ttyMsgPacketTooLarge);

INCLUDE_RODATA(const s32, "sdk/libc", ttyMsgReceiveError);

INCLUDE_RODATA(const s32, "sdk/libc", ttyMsgSendError);

INCLUDE_RODATA(const s32, "sdk/libc", ttyMsgWlenError);

