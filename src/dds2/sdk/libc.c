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
    long (*_seek)(void *cookie, long offset, s32 whence);
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

extern int __srefill(FILE *fp);


/* Declaration-complete historical scanner callees. */
extern int __srefill(FILE *fp);
extern size_t fread(void *ptr, size_t size, size_t count, FILE *fp);
extern unsigned long func_00364F50(const char *s, char **end, int base);
extern double func_00361098(const char *s);
extern long func_003610B0(const char *s);
extern int func_00365068(int c, FILE *fp);
extern char *func_00360D78(char *tab, char *fmt);

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
    u8 _unused[0x58];
    /* srand/rand access the 64-bit state at +0xA8; impure_data starts it at 1. */
    u64 _rand_next;
    u8 _unusedB0[0x98];
    _atexit *_atexit;
    _atexit _atexit0;
    void (**_sig_func)(s32 sig);
    struct _glue __sglue;
    FILE __sf[3];
};

extern _reent *_impure_ptr __attribute__((section(".data")));
extern _reent impure_data;

extern FILE *_fopen_r(_reent *r, const char *filename, const char *mode);

extern s32 func_003625C0(_reent *r, const char *mode, s32 *oflags);

extern s32 _open_r(_reent *r, const char *path, s32 flags, s32 mode);

extern s32 func_0035A0A0(FILE *fp, long offset, s32 whence);

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

extern double func_00363DB8(_reent *r, const char *s, char **end);

extern double func_00364CD0(const char *s, char **end);

extern unsigned long func_00364D20(_reent *r, const char *s, char **end, s32 base);

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

extern void *_realloc_r(_reent *r, void *ptr, size_t size);

extern void _free_r(_reent *r, void *ptr);

extern s32 SignalSema(s32 sema);

extern void *sbrk(s32 incr);
extern void *_sbrk_r(_reent *r, s32 incr);

extern s32 open(const char *path, s32 flags, s32 mode);

extern s32 close(s32 fd);

extern _Bigint *_Balloc(_reent *r, s32 k);

extern void __register_frame_info(void *fn, void *entry);

extern s32 __register_frame_info_table(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, long offset, s32 whence);

extern s32 fstat(s32 fd, struct stat *buf);

extern s32 kill(s32 pid, s32 sig);

extern long _read_r(_reent *r, s32 fd, char *buf, s32 n);

extern long _write_r(_reent *r, s32 fd, const char *buf, s32 n);

extern long _lseek_r(_reent *r, s32 fd, long offset, s32 whence);

extern s32 __sfvwrite(FILE *fp, __suio *uio);
extern s32 vfiprintf(FILE *fp, const char *fmt, va_list ap);
extern s32 fiprintf(FILE *fp, const char *fmt, ...);

extern s32 __sread(void *cookie, char *buf, s32 n);

extern s32 __swrite(void *cookie, const char *buf, s32 n);

extern long __sseek(void *cookie, long offset, s32 whence);

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

extern s32 func_003600E8(FILE *fp, const char *fmt, va_list ap);

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
extern const f64 D_004325D8[];

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

extern const s32 pow5MultTable[3] __attribute__((section(".rodata")));

extern const f64 pow10Table[25];

extern const f64 D_004325D8[5];

extern const f64 D_00432600[5];

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

/* Newlib's three-argument assertion ABI: file, line, failed expression. */
void __assert(const char *file, s32 line, const char *failedexpr) {
    (void)fiprintf(_impure_ptr->_stderr,
        "assertion \"%s\" failed: file \"%s\", line %d\n",
        failedexpr, file, line);
    abort();
}

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

s32 fclose(FILE *fp)
{
    s32 status;

    if (fp == NULL)
        return 0;
    /* Same attachment/initialization phase as matched fflush and __swsetup. */
    do {
        if (fp->_data == NULL)
            fp->_data = _impure_ptr;
        if (fp->_data->__sdidinit == 0)
            __sinit(fp->_data);
    } while (0);
    if (fp->_flags == 0)
        return 0;
    status = (fp->_flags & 8) ? fflush(fp) : 0;
    if (fp->_close != NULL && fp->_close(fp->_cookie) < 0)
        status = -1;
    if (fp->_flags & 0x80)
        _free_r(fp->_data, fp->_bf._base);
    if (fp->_ub._base != NULL) {
        if (fp->_ub._base != fp->_ubuf)
            _free_r(fp->_data, fp->_ub._base);
        fp->_ub._base = NULL;
    }
    if (fp->_lb._base != NULL) {
        _free_r(fp->_data, fp->_lb._base);
        fp->_lb._base = NULL;
    }
    fp->_flags = 0;
    return status;
}

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

s32 fiprintf(FILE *fp, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    return vfiprintf(fp, fmt, ap);
}

/* newlib 1.10 stdio/fopen.c: allocate and initialize a reentrant stream. */
FILE *_fopen_r(_reent *r, const char *filename, const char *mode) {
    FILE *fp;
    s32 fd;
    s32 flags, oflags;

    if ((flags = func_003625C0(r, mode, &oflags)) == 0)
        return NULL;
    if ((fp = __sfp(r)) == NULL)
        return NULL;
    if ((fd = _open_r(fp->_data, filename, oflags, 0666)) < 0) {
        fp->_flags = 0;
        return NULL;
    }
    fp->_file = fd;
    fp->_flags = flags;
    fp->_cookie = fp;
    fp->_read = __sread;
    fp->_write = __swrite;
    fp->_seek = __sseek;
    fp->_close = __sclose;
    if (fp->_flags & 0x100)
        func_0035A0A0(fp, 0, 2);
    return fp;
}

/* newlib stdio/fopen.c: forward through the current reentrancy object. */
FILE *fopen(const char *filename, const char *mode) {
    return _fopen_r(_impure_ptr, filename, mode);
}

s32 fprintf(FILE *fp, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    return vfprintf(fp, fmt, ap);
}

/* Historical newlib buffered read; __srefill resets the consumed count. */
size_t fread(void *buf, size_t size, size_t count, FILE *fp) {
    size_t resid;
    char *p;
    int r;
    size_t total;

    if ((resid = count * size) == 0)
        return 0;
    if (fp->_r < 0)
        fp->_r = 0;
    total = resid;
    p = buf;
    while (resid > (r = fp->_r)) {
        memcpy(p, fp->_p, (size_t)r);
        fp->_p += r;
        p += r;
        resid -= r;
        if (__srefill(fp))
            return (total - resid) / size;
    }
    memcpy(p, fp->_p, resid);
    fp->_r -= resid;
    fp->_p += resid;
    return count;
}

/* Newlib 1.10 mallocr.c free with 32-bit chunk words and target 64-bit
 * unsigned long bin-index/bitmap operations. The bin sentinel's header is
 * eight bytes before its actual forward/backward pair; all chunk accesses
 * retain the named boundary-tag and free-link fields. Payload starts at +8.
 * Historical source: mirror/newlib-cygwin, newlib-1_10_0,
 * newlib/libc/stdlib/mallocr.c, free, frontlink, unlink and link_last_remainder.
 */
void _free_r(_reent *r, void *mem)
{
    mallocChunkPrefix_t * p;
    u32 hd;
    u32 sz;
    int idx;
    mallocChunkPrefix_t * next;
    u32 nextsz;
    u32 prevsz;
    mallocChunkPrefix_t * bck;
    mallocChunkPrefix_t * fwd;
    int islr;
    if (mem == 0)
    return;
    __malloc_lock(r);
    p = ((mallocChunkPrefix_t *)((char*)(mem) - 2*4U));
    hd = p->size;
    ;
    sz = hd & ~1U;
    next = ((mallocChunkPrefix_t *)(((char*)(p)) + (sz)));
    nextsz = ((next)->size & ~(3U));
    if (next == __malloc_av_.top)
    {
        sz += nextsz;
        if (!(hd & 1U))
        {
            prevsz = p->previousSize;
            p = ((mallocChunkPrefix_t *)(((char*)(p)) + (-prevsz)));
            sz += prevsz;
            {
                bck = p->backward;
                fwd = p->forward;
                fwd->backward = bck;
                bck->forward = fwd;
            }
            ;
        }
        ((p)->size = (sz | 1U));
        __malloc_av_.top = p;
        if ((unsigned long)(sz) >= (unsigned long)__malloc_trim_threshold)
        _malloc_trim_r(r, __malloc_top_pad);
        __malloc_unlock(r);
        return;
    }
    ((next)->size = (nextsz));
    islr = 0;
    if (!(hd & 1U))
    {
        prevsz = p->previousSize;
        p = ((mallocChunkPrefix_t *)(((char*)(p)) + (-prevsz)));
        sz += prevsz;
        if (p->forward == ((mallocChunkPrefix_t *)((char *)&__malloc_av_.top + (1) * sizeof(mallocBinLinks_t) - 2*4U)))
        islr = 1;
        else
        {
            bck = p->backward;
            fwd = p->forward;
            fwd->backward = bck;
            bck->forward = fwd;
        }
        ;
    }
    if (!((((mallocChunkPrefix_t *)(((char*)(next)) + (nextsz)))->size & 1U)))
    {
        sz += nextsz;
        if (!islr && next->forward == ((mallocChunkPrefix_t *)((char *)&__malloc_av_.top + (1) * sizeof(mallocBinLinks_t) - 2*4U)))
        {
            islr = 1;
            {
                ((mallocChunkPrefix_t *)((char *)&__malloc_av_.top + (1) * sizeof(mallocBinLinks_t) - 2*4U))->forward = ((mallocChunkPrefix_t *)((char *)&__malloc_av_.top + (1) * sizeof(mallocBinLinks_t) - 2*4U))->backward = p;
                p->forward = p->backward = ((mallocChunkPrefix_t *)((char *)&__malloc_av_.top + (1) * sizeof(mallocBinLinks_t) - 2*4U));
            }
            ;
        }
        else
        {
            bck = next->backward;
            fwd = next->forward;
            fwd->backward = bck;
            bck->forward = fwd;
        }
        ;
    }
    ((p)->size = (sz | 1U));
    (((mallocChunkPrefix_t *)((char*)(p) + (sz)))->previousSize = (sz));
    if (!islr)
    {
        if (sz < 512) {
            idx = (((unsigned long)(sz)) >> 3);
            (__malloc_av_.bitmap |= ((unsigned long)1 << (idx / 4)));
            bck = ((mallocChunkPrefix_t *)((char *)&__malloc_av_.top + (idx) * sizeof(mallocBinLinks_t) - 2*4U));
            fwd = bck->forward;
            p->backward = bck;
            p->forward = fwd;
            fwd->backward = bck->forward = p;
        }
        else {
            idx = (((((unsigned long)(sz)) >> 9) == 0) ? (((unsigned long)(sz)) >> 3): ((((unsigned long)(sz)) >> 9) <= 4) ? 56 + (((unsigned long)(sz)) >> 6): ((((unsigned long)(sz)) >> 9) <= 20) ? 91 + (((unsigned long)(sz)) >> 9): ((((unsigned long)(sz)) >> 9) <= 84) ? 110 + (((unsigned long)(sz)) >> 12): ((((unsigned long)(sz)) >> 9) <= 340) ? 119 + (((unsigned long)(sz)) >> 15): ((((unsigned long)(sz)) >> 9) <= 1364) ? 124 + (((unsigned long)(sz)) >> 18): 126);
            bck = ((mallocChunkPrefix_t *)((char *)&__malloc_av_.top + (idx) * sizeof(mallocBinLinks_t) - 2*4U));
            fwd = bck->forward;
            if (fwd == bck) (__malloc_av_.bitmap |= ((unsigned long)1 << (idx / 4)));
            else {
                while (fwd != bck && sz < ((fwd)->size & ~(3U))) fwd = fwd->forward;
                bck = fwd->backward;
            }
            p->backward = bck;
            p->forward = fwd;
            fwd->backward = bck->forward = p;
        }
    }
    ;
    __malloc_unlock(r);
}


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

size_t fwrite(const void *buf, size_t size, size_t count, FILE *fp)
{
    size_t total;
    __suio uio;
    __siov iov;

    iov.iov_base = (void *)buf;
    uio.uio_resid = iov.iov_len = total = count * size;
    uio.uio_iov = &iov;
    uio.uio_iovcnt = 1;
    if (__sfvwrite(fp, &uio) == 0)
        return count;
    return (total - uio.uio_resid) / size;
}

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

s32 _open_r(_reent *r, const char *path, s32 flags, s32 mode) {
    s32 result;

    libcErrno = 0;
    if ((result = open(path, flags, mode)) == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

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

void srand(u32 seed) {
    _impure_ptr->_rand_next = seed;
}

s32 rand(void) {
    _impure_ptr->_rand_next = _impure_ptr->_rand_next * 0x5851F42D4C957F2DULL + 1;
    return (_impure_ptr->_rand_next >> 32) & 0x7FFFFFFF;
}

/* __srefill's _fwalk callback: flush line-buffered output streams. */
s32 func_0035C268(void *stream)
{
    FILE *fp = stream;

    if ((fp->_flags & 9) == 9)
        return fflush(fp);
    return 0;
}

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

s32 _sprintf_r(_reent *r, char *str, const char *fmt, ...) {
    s32 result;
    FILE f;
    va_list ap;

    va_start(ap, fmt);
    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = r;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

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

/* newlib sscanf's EOF read callback has the FILE read-hook signature. */
s32 func_0035C8F0(void *cookie, char *buf, s32 len) {
    return 0;
}

s32 sscanf(const char *str, const char *fmt, ...) {
    s32 result;
    va_list ap;
    FILE f;

    f._flags = 4;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._r = strlen(str);
    f._read = func_0035C8F0;
    f._ub._base = NULL;
    f._lb._base = NULL;
    f._data = _impure_ptr;
    va_start(ap, fmt);
    result = func_003600E8(&f, fmt, ap);
    __builtin_va_end(ap);
    return result;
}

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

long __sseek(void *cookie, long offset, s32 whence) {
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

/* Integer-formatting copy of __sprint; uses the same SMT3-derived
 * __suio contract and matched sibling source below. */
s32 func_0035D8F0(FILE *fp, __suio *uio) {
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

/* Integer-formatting copy of the SMT3-derived __sbprintf below.
 * The temporary stream forwards to vfiprintf rather than vfprintf. */
s32 func_0035D938(FILE *fp, const char *fmt, va_list ap)
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
    ret = vfiprintf(&fake, fmt, ap);
    if (ret >= 0 && fflush(&fake) != 0) {
        ret = -1;
    }
    if (fake._flags & 0x40) {
        fp->_flags |= 0x40;
    }
    return ret;
}

s32 _vfiprintf_r(_reent *r, FILE *fp, const char *fmt, va_list ap);

s32 vfiprintf(FILE *fp, const char *fmt, va_list ap)
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
    return _vfiprintf_r(fp->_data, fp, fmt, ap);
}

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

s32 exponent(char *output, s32 value, s32 marker)
{
    /* BSD vfprintf exponent uses expbuf[MAXEXP], with MAXEXP = 308 in
     * floatio.h. The live reverse-digit buffer ends at stack + 0x134. */
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

/* Historical newlib c87be3e4d6ae90dcc48b5f26dd87645e7cabff01
 * (2000-04-17) __svfscanf evidence trial. The unsigned-long callback cast
 * of strtol is historical but is not a compatible-return-type ISO C call.
 * This target-ABI experiment requires independent semantic review.
 * This software was developed by the University of California, Berkeley.
 * Header expansions: _LONG_DOUBLE=long double, _POINTER_INT=long;
 * BUF=308+39+3, CHAR_BIT=8; compiler __WCHAR_TYPE__=signed int.
 * Floating point enabled, no multibyte mode; original local state retained.
 */
/* No user fns here. Pesch 15apr92. */

/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifdef _HAVE_STDC
#else
#endif

#ifndef	NO_FLOATING_POINT
#define FLOATING_POINT
#endif

#ifdef FLOATING_POINT
#define	BUF	(308+39+3)	/* 3 = sign + decimal point + NUL */
/* An upper bound for how long a long prints in decimal.  4 / 13 approximates
   log (2).  Add one char for roundoff compensation and one for the sign.  */
#define MAX_LONG_LEN ((8 * sizeof (long)  - 1) * 4 / 13 + 2)
#else
#define	BUF	40
#endif

/*
 * Flags used during conversion.
 */

#define	LONG		0x01	/* l: long or double */
#define	LONGDBL		0x02	/* L: long double; unimplemented */
#define	SHORT		0x04	/* h: short */
#define	SUPPRESS	0x08	/* suppress assignment */
#define	POINTER		0x10	/* weird %p pointer (`fake hex') */
#define	NOSKIP		0x20	/* do not skip blanks */

/*
 * The following are used in numeric conversions only:
 * SIGNOK, NDIGITS, DPTOK, and EXPOK are for floating point;
 * SIGNOK, NDIGITS, PFXOK, and NZDIGITS are for integral.
 */

#define	SIGNOK		0x40	/* +/- is (still) legal */
#define	NDIGITS		0x80	/* no digits detected */

#define	DPTOK		0x100	/* (float) decimal point is still legal */
#define	EXPOK		0x200	/* (float) exponent (e+3, etc) still legal */

#define	PFXOK		0x100	/* 0x prefix is (still) legal */
#define	NZDIGITS	0x200	/* no zero digits detected */

/*
 * Conversion types.
 */

#define	CT_CHAR		0	/* %c conversion */
#define	CT_CCL		1	/* %[...] conversion */
#define	CT_STRING	2	/* %s conversion */
#define	CT_INT		3	/* integer, i.e., strtol or func_00364F50 */
#define	CT_FLOAT	4	/* floating, i.e., strtod */

#if 0
#define u_char unsigned char
#endif
#define u_char char
#define u_long unsigned long



/*
 * vfscanf
 */

#define BufferEmpty (fp->_r <= 0 && __srefill(fp))

int func_003600E8 (fp, fmt0, ap)
     register FILE *fp;
     char const *fmt0;
     va_list ap;
{
  register u_char *fmt = (u_char *) fmt0;
  register int c;		/* character from format, or conversion */
  register size_t width;	/* field width, or 0 */
  register char *p;		/* points into all kinds of strings */
  register int n;		/* handy integer */
  register int flags;		/* flags as defined above */
  register char *p0;		/* saves original value of p when necessary */
  int nassigned;		/* number of fields assigned */
  int nread;			/* number of characters consumed from fp */
  int base = 0;			/* base argument to strtol/func_00364F50 */
  int nbytes = 1;               /* number of bytes read from fmt string */
  __WCHAR_TYPE__ wc;                   /* wchar to use to read format string */

  u_long (*ccfn) () = 0;	/* conversion function (strtol/func_00364F50) */
  char ccltab[256];		/* character class table for %[...] */
  char buf[BUF];		/* buffer for numeric conversions */
  char *lptr;                   /* literal pointer */
  int state = 0;                /* value to keep track of multibyte state */

  short *sp;
  int *ip;
  float *flp;
  long double *ldp;
  double *dp;
  long *lp;

  /* `basefix' is used to avoid `if' tests in the integer scanner */
  static const short basefix[17] =
    {10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

  nassigned = 0;
  nread = 0;
  for (;;)
    {
#ifndef MB_CAPABLE
      wc = *fmt;
#else
      nbytes = _mbtowc_r (_REENT, &wc, fmt, MB_CUR_MAX, &state);
#endif
      fmt += nbytes;
      if (wc == 0)
	return nassigned;
      if (nbytes == 1 && ((_ctype_ + 1)[(unsigned)(wc)] & 8))
	{
	  for (;;)
	    {
	      if (BufferEmpty)
		return nassigned;
	      if (!((_ctype_ + 1)[(unsigned)(*fp->_p)] & 8))
		break;
	      nread++, fp->_r--, fp->_p++;
	    }
	  continue;
	}
      if (wc != '%')
	goto literal;
      width = 0;
      flags = 0;

      /*
       * switch on the format.  continue if done; break once format
       * type is derived.
       */

    again:
      c = *fmt++;

      switch (c)
	{
	case '%':
	literal:
          lptr = fmt - nbytes;
          for (n = 0; n < nbytes; ++n)
            {
	      if (BufferEmpty)
	        goto input_failure;
	      if (*fp->_p != *lptr)
	        goto match_failure;
	      fp->_r--, fp->_p++;
	      nread++;
              ++lptr;
            }
	  continue;

	case '*':
	  flags |= SUPPRESS;
	  goto again;
	case 'l':
	  flags |= LONG;
	  goto again;
	case 'L':
	  flags |= LONGDBL;
	  goto again;
	case 'h':
	  flags |= SHORT;
	  goto again;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  width = width * 10 + c - '0';
	  goto again;

	  /*
	   * Conversions. Those marked `compat' are for
	   * 4.[123]BSD compatibility.
	   *
	   * (According to ANSI, E and X formats are supposed to
	   * the same as e and x.  Sorry about that.)
	   */

	case 'D':		/* compat */
	  flags |= LONG;
	  /* FALLTHROUGH */
	case 'd':
	  c = CT_INT;
	  ccfn = (u_long (*)())strtol;
	  base = 10;
	  break;

	case 'i':
	  c = CT_INT;
	  ccfn = (u_long (*)())strtol;
	  base = 0;
	  break;

	case 'O':		/* compat */
	  flags |= LONG;
	  /* FALLTHROUGH */
	case 'o':
	  c = CT_INT;
	  ccfn = func_00364F50;
	  base = 8;
	  break;

	case 'u':
	  c = CT_INT;
	  ccfn = func_00364F50;
	  base = 10;
	  break;

	case 'X':		/* compat   XXX */
	case 'x':
	  flags |= PFXOK;	/* enable 0x prefixing */
	  c = CT_INT;
	  ccfn = func_00364F50;
	  base = 16;
	  break;

#ifdef FLOATING_POINT
	case 'E':		/* compat   XXX */
	case 'G':		/* compat   XXX */
/* ANSI says that E,G and X behave the same way as e,g,x */
	  /* FALLTHROUGH */
	case 'e':
	case 'f':
	case 'g':
	  c = CT_FLOAT;
	  break;
#endif

	case 's':
	  c = CT_STRING;
	  break;

	case '[':
	  fmt = func_00360D78 (ccltab, fmt);
	  flags |= NOSKIP;
	  c = CT_CCL;
	  break;

	case 'c':
	  flags |= NOSKIP;
	  c = CT_CHAR;
	  break;

	case 'p':		/* pointer format is like hex */
	  flags |= POINTER | PFXOK;
	  c = CT_INT;
	  ccfn = func_00364F50;
	  base = 16;
	  break;

	case 'n':
	  if (flags & SUPPRESS)	/* ??? */
	    continue;
	  if (flags & SHORT)
	    {
	      sp = __builtin_va_arg (ap, short *);
	      *sp = nread;
	    }
	  else if (flags & LONG)
	    {
	      lp = __builtin_va_arg (ap, long *);
	      *lp = nread;
	    }
	  else
	    {
	      ip = __builtin_va_arg (ap, int *);
	      *ip = nread;
	    }
	  continue;

	  /*
	   * Disgusting backwards compatibility hacks.	XXX
	   */
	case '\0':		/* compat */
	  return (-1);

	default:		/* compat */
	  if (((_ctype_ + 1)[(unsigned)(c)] & 1))
	    flags |= LONG;
	  c = CT_INT;
	  ccfn = (u_long (*)())strtol;
	  base = 10;
	  break;
	}

      /*
       * We have a conversion that requires input.
       */
      if (BufferEmpty)
	goto input_failure;

      /*
       * Consume leading white space, except for formats that
       * suppress this.
       */
      if ((flags & NOSKIP) == 0)
	{
	  while (((_ctype_ + 1)[(unsigned)(*fp->_p)] & 8))
	    {
	      nread++;
	      if (--fp->_r > 0)
		fp->_p++;
	      else
#ifndef CYGNUS_NEC
	      if (__srefill (fp))
#endif
		goto input_failure;
	    }
	  /*
	   * Note that there is at least one character in the
	   * buffer, so conversions that do not set NOSKIP ca
	   * no longer result in an input failure.
	   */
	}

      /*
       * Do the conversion.
       */
      switch (c)
	{

	case CT_CHAR:
	  /* scan arbitrary characters (sets NOSKIP) */
	  if (width == 0)
	    width = 1;
	  if (flags & SUPPRESS)
	    {
	      size_t sum = 0;

	      for (;;)
		{
		  if ((n = fp->_r) < width)
		    {
		      sum += n;
		      width -= n;
		      fp->_p += n;
#ifndef CYGNUS_NEC
		      if (__srefill (fp))
			{
#endif
			  if (sum == 0)
			    goto input_failure;
			  break;
#ifndef CYGNUS_NEC
			}
#endif
		    }
		  else
		    {
		      sum += width;
		      fp->_r -= width;
		      fp->_p += width;
		      break;
		    }
		}
	      nread += sum;
	    }
	  else
	    {
#ifdef CYGNUS_NEC
	      /* Kludge city for the moment */
	      char *dest = __builtin_va_arg (ap, char *);
	      int n = width;
	      if (fp->_r == 0)
		goto input_failure;

	      while (n && fp->_r)
		{
		  *dest++ = *(fp->_p++);
		  n--;
		  fp->_r--;
		  nread++;
		}
#else
	      size_t r = fread ((void *) __builtin_va_arg (ap, char *), 1, width, fp);

	      if (r == 0)
		goto input_failure;
	      nread += r;
#endif
	      nassigned++;
	    }
	  break;

	case CT_CCL:
	  /* scan a (nonempty) character class (sets NOSKIP) */
	  if (width == 0)
	    width = ~0;		/* `infinity' */
	  /* take only those things in the class */
	  if (flags & SUPPRESS)
	    {
	      n = 0;
	      while (ccltab[*fp->_p])
		{
		  n++, fp->_r--, fp->_p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty)
		    {
		      if (n == 0)
			goto input_failure;
		      break;
		    }
		}
	      if (n == 0)
		goto match_failure;
	    }
	  else
	    {
	      p0 = p = __builtin_va_arg (ap, char *);
	      while (ccltab[*fp->_p])
		{
		  fp->_r--;
		  *p++ = *fp->_p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty)
		    {
		      if (p == p0)
			goto input_failure;
		      break;
		    }
		}
	      n = p - p0;
	      if (n == 0)
		goto match_failure;
	      *p = 0;
	      nassigned++;
	    }
	  nread += n;
	  break;

	case CT_STRING:
	  /* like CCL, but zero-length string OK, & no NOSKIP */
	  if (width == 0)
	    width = ~0;
	  if (flags & SUPPRESS)
	    {
	      n = 0;
	      while (!((_ctype_ + 1)[(unsigned)(*fp->_p)] & 8))
		{
		  n++, fp->_r--, fp->_p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty)
		    break;
		}
	      nread += n;
	    }
	  else
	    {
	      p0 = p = __builtin_va_arg (ap, char *);
	      while (!((_ctype_ + 1)[(unsigned)(*fp->_p)] & 8))
		{
		  fp->_r--;
		  *p++ = *fp->_p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty)
		    break;
		}
	      *p = 0;
	      nread += p - p0;
	      nassigned++;
	    }
	  continue;

	case CT_INT:
	  /* scan an integer as if by strtol/func_00364F50 */
#ifdef hardway
	  if (width == 0 || width > sizeof (buf) - 1)
	    width = sizeof (buf) - 1;
#else
	  /* size_t is unsigned, hence this optimisation */
	  if (--width > sizeof (buf) - 2)
	    width = sizeof (buf) - 2;
	  width++;
#endif
	  flags |= SIGNOK | NDIGITS | NZDIGITS;
	  for (p = buf; width; width--)
	    {
	      c = *fp->_p;
	      /*
	       * Switch on the character; `goto ok' if we
	       * accept it as a part of number.
	       */
	      switch (c)
		{
		  /*
		   * The digit 0 is always legal, but is special.
		   * For %i conversions, if no digits (zero or nonzero)
		   * have been scanned (only signs), we will have base==0.
		   * In that case, we should set it to 8 and enable 0x
		   * prefixing. Also, if we have not scanned zero digits
		   * before this, do not turn off prefixing (someone else
		   * will turn it off if we have scanned any nonzero digits).
		   */
		case '0':
		  if (base == 0)
		    {
		      base = 8;
		      flags |= PFXOK;
		    }
		  if (flags & NZDIGITS)
		    flags &= ~(SIGNOK | NZDIGITS | NDIGITS);
		  else
		    flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* 1 through 7 always legal */
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		  base = basefix[base];
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* digits 8 and 9 ok iff decimal or hex */
		case '8':
		case '9':
		  base = basefix[base];
		  if (base <= 8)
		    break;	/* not legal here */
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* letters ok iff hex */
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		  /* no need to fix base here */
		  if (base <= 10)
		    break;	/* not legal here */
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* sign ok only as first character */
		case '+':
		case '-':
		  if (flags & SIGNOK)
		    {
		      flags &= ~SIGNOK;
		      goto ok;
		    }
		  break;

		  /* x ok iff flag still set & 2nd char */
		case 'x':
		case 'X':
		  if (flags & PFXOK && p == buf + 1)
		    {
		      base = 16;/* if %i */
		      flags &= ~PFXOK;
		      goto ok;
		    }
		  break;
		}

	      /*
	       * If we got here, c is not a legal character
	       * for a number.  Stop accumulating digits.
	       */
	      break;
	    ok:
	      /*
	       * c is legal: store it and look at the next.
	       */
	      *p++ = c;
	      if (--fp->_r > 0)
		fp->_p++;
	      else
#ifndef CYGNUS_NEC
	      if (__srefill (fp))
#endif
		break;		/* (-1) */
	    }
	  /*
	   * If we had only a sign, it is no good; push back the sign.
	   * If the number ends in `x', it was [sign] '0' 'x', so push back
	   * the x and treat it as [sign] '0'.
	   */
	  if (flags & NDIGITS)
	    {
	      if (p > buf)
		(void) func_00365068 (*(u_char *)-- p, fp);
	      goto match_failure;
	    }
	  c = ((u_char *) p)[-1];
	  if (c == 'x' || c == 'X')
	    {
	      --p;
	      /*(void)*/ func_00365068 (c, fp);
	    }
	  if ((flags & SUPPRESS) == 0)
	    {
	      u_long res;

	      *p = 0;
	      res = (*ccfn) (buf, (char **) NULL, base);
	      if (flags & POINTER)
		*(__builtin_va_arg (ap, void * *)) = (void *) (unsigned long) res;
	      else if (flags & SHORT)
		{
		  sp = __builtin_va_arg (ap, short *);
		  *sp = res;
		}
	      else if (flags & LONG)
		{
		  lp = __builtin_va_arg (ap, long *);
		  *lp = res;
		}
	      else
		{
		  ip = __builtin_va_arg (ap, int *);
		  *ip = res;
		}
	      nassigned++;
	    }
	  nread += p - buf;
	  break;

#ifdef FLOATING_POINT
	case CT_FLOAT:
	{
	  /* scan a floating point number as if by strtod */
	  /* This code used to assume that the number of digits is reasonable.
	     However, ANSI / ISO C makes no such stipulation; we have to get
	     exact results even when there is an unreasonable amount of
	     leading zeroes.  */
	  long leading_zeroes, zeroes, exp_adjust;
	  char *exp_start;
#ifdef hardway
	  if (width == 0 || width > sizeof (buf) - 1)
	    width = sizeof (buf) - 1;
#else
	  /* size_t is unsigned, hence this optimisation */
	  if (--width > sizeof (buf) - 2)
	    width = sizeof (buf) - 2;
	  width++;
#endif
	  flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
	  zeroes = 0;
	  exp_adjust = 0;
	  for (p = buf; width; )
	    {
	      c = *fp->_p;
	      /*
	       * This code mimicks the integer conversion
	       * code, but is much simpler.
	       */
	      switch (c)
		{

		case '0':
		  if (flags & NDIGITS)
		    {
		      flags &= ~SIGNOK;
		      zeroes++;
		      goto fskip;
		    }
		  /* Fall through.  */
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		  flags &= ~(SIGNOK | NDIGITS);
		  goto fok;

		case '+':
		case '-':
		  if (flags & SIGNOK)
		    {
		      flags &= ~SIGNOK;
		      goto fok;
		    }
		  break;
		case '.':
		  if (flags & DPTOK)
		    {
		      flags &= ~(SIGNOK | DPTOK);
		      leading_zeroes = zeroes;
		      goto fok;
		    }
		  break;
		case 'e':
		case 'E':
		  /* no exponent without some digits */
		  if ((flags & (NDIGITS | EXPOK)) == EXPOK
		      || ((flags & EXPOK) && zeroes))
		    {
		      if (! (flags & DPTOK))
			{
			  exp_adjust = zeroes - leading_zeroes;
			  exp_start = p;
			}
		      flags =
			(flags & ~(EXPOK | DPTOK)) |
			SIGNOK | NDIGITS;
		      zeroes = 0;
		      goto fok;
		    }
		  break;
		}
	      break;
	    fok:
	      *p++ = c;
	      width--;
	    fskip:
              ++nread;
	      if (--fp->_r > 0)
		fp->_p++;
	      else
#ifndef CYGNUS_NEC
	      if (__srefill (fp))
#endif
		break;		/* (-1) */
	    }
	  if (zeroes)
	    flags &= ~NDIGITS;
	  /*
	   * If no digits, might be missing exponent digits
	   * (just give back the exponent) or might be missing
	   * regular digits, but had sign and/or decimal point.
	   */
	  if (flags & NDIGITS)
	    {
	      if (flags & EXPOK)
		{
		  /* no digits at all */
		  while (p > buf)
                    {
		      func_00365068 (*(u_char *)-- p, fp);
                      --nread;
                    }
		  goto match_failure;
		}
	      /* just a bad exponent (e and maybe sign) */
	      c = *(u_char *)-- p;
              --nread;
	      if (c != 'e' && c != 'E')
		{
		  (void) func_00365068 (c, fp);	/* sign */
		  c = *(u_char *)-- p;
                  --nread;
		}
	      (void) func_00365068 (c, fp);
	    }
	  if ((flags & SUPPRESS) == 0)
	    {
	      double res;
	      long new_exp;

	      *p = 0;
	      if ((flags & (DPTOK | EXPOK)) == EXPOK)
		{
		  exp_adjust = zeroes - leading_zeroes;
		  new_exp = -exp_adjust;
		  exp_start = p;
		}
	      else if (exp_adjust)
		new_exp = func_003610B0 (exp_start + 1) - exp_adjust;
	      if (exp_adjust)
		{
		  static const char exponent_format[] __attribute__((section(".rodata"))) = "e%ld";

		  /* If there might not be enough space for the new exponent,
		     truncate some trailing digits to make room.  */
		  if (exp_start >= buf + sizeof (buf) - MAX_LONG_LEN)
		    exp_start = buf + sizeof (buf) - MAX_LONG_LEN - 1;
                 sprintf (exp_start, exponent_format, new_exp);
		}
	      res = func_00361098 (buf);
	      if (flags & LONG)
		{
		  dp = __builtin_va_arg (ap, double *);
		  *dp = res;
		}
	      else if (flags & LONGDBL)
		{
		  ldp = __builtin_va_arg (ap, long double *);
		  *ldp = res;
		}
	      else
		{
		  flp = __builtin_va_arg (ap, float *);
		  *flp = res;
		}
	      nassigned++;
	    }
	  break;
	}
#endif /* FLOATING_POINT */
	}
    }
input_failure:
  return nassigned ? nassigned : -1;
match_failure:
  return nassigned;
}


#undef FLOATING_POINT
#undef BUF
#undef MAX_LONG_LEN
#undef LONG
#undef LONGDBL
#undef SHORT
#undef SUPPRESS
#undef POINTER
#undef NOSKIP
#undef SIGNOK
#undef NDIGITS
#undef DPTOK
#undef EXPOK
#undef PFXOK
#undef NZDIGITS
#undef CT_CHAR
#undef CT_CCL
#undef CT_STRING
#undef CT_INT
#undef CT_FLOAT
#undef u_char
#undef u_long
#undef BufferEmpty

/* Historical __sccl from newlib 1.9.0 vfscanf.c.
 * This software was developed by the University of California, Berkeley.
 * The donor defines u_char as signed plain char on this target.
 */
/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*static*/
char *
func_00360D78 (tab, fmt)
     register char *tab;
     register char *fmt;
{
  register int c, n, v;

  /* first `clear' the whole table */
  c = *fmt++;			/* first char hat => negated scanset */
  if (c == '^')
    {
      v = 1;			/* default => accept */
      c = *fmt++;		/* get new first char */
    }
  else
    v = 0;			/* default => reject */
  /* should probably use memset here */
  for (n = 0; n < 256; n++)
    tab[n] = v;
  if (c == 0)
    return fmt - 1;		/* format ended before closing ] */

  /*
   * Now set the entries corresponding to the actual scanset to the
   * opposite of the above.
   *
   * The first character may be ']' (or '-') without being special; the
   * last character may be '-'.
   */

  v = 1 - v;
  for (;;)
    {
      tab[c] = v;		/* take character c */
    doswitch:
      n = *fmt++;		/* and examine the next */
      switch (n)
	{

	case 0:		/* format ended too soon */
	  return fmt - 1;

	case '-':
	  /*
	   * A scanset of the form [01+-] is defined as `the digit 0, the
	   * digit 1, the character +, the character -', but the effect of a
	   * scanset such as [a-zA-Z0-9] is implementation defined.  The V7
	   * Unix scanf treats `a-z' as `the letters a through z', but treats
	   * `a-a' as `the letter a, the character -, and the letter a'.
	   *
	   * For compatibility, the `-' is not considerd to define a range if
	   * the character following it is either a close bracket (required by
	   * ANSI) or is not numerically greater than the character we just
	   * stored in the table (c).
	   */
	  n = *fmt;
	  if (n == ']' || n < c)
	    {
	      c = '-';
	      break;		/* resume the for(;;) */
	    }
	  fmt++;
	  do
	    {			/* fill in the range */
	      tab[++c] = v;
	    }
	  while (c < n);
#if 1			/* XXX another disgusting compatibility hack */
	  /*
	   * Alas, the V7 Unix scanf also treats formats such
	   * as [a-c-e] as `the letters a through e'. This too
	   * is permitted by the standard....
	   */
	  goto doswitch;
#else
	  c = *fmt++;
	  if (c == 0)
	    return fmt - 1;
	  if (c == ']')
	    return fmt;
#endif

	  break;


	case ']':		/* end of scanset */
	  return fmt;

	default:		/* just another character */
	  c = n;
	  break;
	}
    }
  /* NOTREACHED */
}


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

double func_00361098(const char *s) {
    return func_00364CD0(s, NULL);
}

long func_003610B0(const char *s) {
    return strtol(s, NULL, 10);
}

s32 _close_r(_reent *r, s32 fd) {
    s32 result;

    libcErrno = 0;
    result = close(fd);
    if (result == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

/****************************************************************
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 ***************************************************************/


/* Historical newlib 1.9 dtoa.c Pack_32 form; target limbs are 32-bit. */
s32 quorem(_Bigint *b, _Bigint *S)
{
  int n;
  s32 borrow, y;
  u32 carry, q, ys;
  u32 *bx, *bxe, *sx, *sxe;
  s32 z;
  u32 si, zs;

  n = S->wds;
  if (b->wds < n)
    return 0;
  sx = S->x;
  sxe = sx + --n;
  bx = b->x;
  bxe = bx + n;
  q = *bxe / (*sxe + 1);	/* ensure q <= true quotient */
  if (q)
    {
      borrow = 0;
      carry = 0;
      do
	{
	  si = *sx++;
	  ys = (si & 0xffff) * q + carry;
	  zs = (si >> 16) * q + (ys >> 16);
	  carry = zs >> 16;
	  y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
	  borrow = y >> 16;
	  z = (*bx >> 16) - (zs & 0xffff) + borrow;
	  borrow = z >> 16;
	  (((unsigned short *)bx)[1] = (unsigned short)z,
           ((unsigned short *)bx)[0] = (unsigned short)y, bx++);
	}
      while (sx <= sxe);
      if (!*bxe)
	{
	  bx = b->x;
	  while (--bxe > bx && !*bxe)
	    --n;
	  b->wds = n;
	}
    }
  if (__mcmp (b, S) >= 0)
    {
      q++;
      borrow = 0;
      carry = 0;
      bx = b->x;
      sx = S->x;
      do
	{
	  si = *sx++;
	  ys = (si & 0xffff) + carry;
	  zs = (si >> 16) + (ys >> 16);
	  carry = zs >> 16;
	  y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
	  borrow = y >> 16;
	  z = (*bx >> 16) - (zs & 0xffff) + borrow;
	  borrow = z >> 16;
	  (((unsigned short *)bx)[1] = (unsigned short)z,
           ((unsigned short *)bx)[0] = (unsigned short)y, bx++);
	}
      while (sx <= sxe);
      bx = b->x;
      bxe = bx + n;
      if (!*bxe)
	{
	  while (--bxe > bx && !*bxe)
	    --n;
	  b->wds = n;
	}
    }
  return q;
}

/****************************************************************
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 ***************************************************************/


/* Historical newlib 1.9 dtoa.c, IEEE binary64 little-endian / Pack_32. */
/* Immutable native strings remain in their original .rodata objects. */
extern const char D_00432460[] __attribute__((section(".rodata")));
extern const char D_00432470[] __attribute__((section(".rodata")));
extern const char D_00432478[] __attribute__((section(".rodata")));

INCLUDE_RODATA(const s32, "sdk/libc", D_00432460);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432470);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432478);

char *_dtoa_r(_reent *ptr, f64 _d, s32 mode, s32 ndigits,
              s32 *decpt, s32 *sign, char **rve)
{
  int bbits, b2, b5, be, dig, i, ieps, ilim, ilim0, ilim1, j, j1, k, k0,
    k_check, leftright, m2, m5, s2, s5, spec_case, try_quick;
  libcDoubleBits_t d, d2, eps;
  s32 L;
  int denorm;
  u32 x;
  _Bigint *b, *b1, *delta, *mlo, *mhi, *S;
  double ds;
  char *s, *s0;
  d.value = _d;
  if (ptr->_result)
    {
      ptr->_result->k = ptr->_result_k;
      ptr->_result->maxwds = 1 << ptr->_result_k;
      _Bfree (ptr, ptr->_result);
      ptr->_result = 0;
    }
  if ((d.word[1]) & ((u32)0x80000000L))
    {
      *sign = 1;
      (d.word[1]) &= ~((u32)0x80000000L);
    }
  else
    *sign = 0;
  if (((d.word[1]) & ((u32)0x7ff00000L)) == ((u32)0x7ff00000L))
    {
      *decpt = 9999;
      s =
 !(d.word[0]) && !((d.word[1]) & 0xfffff) ? D_00432460 :
 D_00432470;
      if (rve)
 *rve =
   s[3] ? s + 8 :
   s + 3;
      return s;
    }
  if (!d.value)
    {
      *decpt = 1;
      s = D_00432478;
      if (rve)
 *rve = s + 1;
      return s;
    }
  b = _d2b (ptr, d.value, &be, &bbits);
  if ((i = (int) ((d.word[1]) >> 20 & (((u32)0x7ff00000L) >> 20))) != 0)
    {
      d2.value = d.value;
      (d2.word[1]) &= ((u32)0xfffffL);
      (d2.word[1]) |= ((u32)0x3ff00000L);
      i -= 1023;
      denorm = 0;
    }
  else
    {
      i = bbits + be + (1023 + (53 - 1) - 1);
      x = (i > 32) ? ((d.word[1]) << (64 - i)) | ((d.word[0]) >> (i - 32))
       : ((d.word[0]) << (32 - i));
      d2.value = x;
      (d2.word[1]) -= 31 * ((u32)0x100000L);
      i -= (1023 + (53 - 1) - 1) + 1;
      denorm = 1;
    }
  ds = (d2.value - 1.5) * 0.289529654602168 + 0.1760912590558 + i * 0.301029995663981;
  k = (int) ds;
  if (ds < 0. && ds != k)
    k--;
  k_check = 1;
  if (k >= 0 && k <= 22)
    {
      if (d.value < pow10Table[k])
 k--;
      k_check = 0;
    }
  j = bbits - i - 1;
  if (j >= 0)
    {
      b2 = 0;
      s2 = j;
    }
  else
    {
      b2 = -j;
      s2 = 0;
    }
  if (k >= 0)
    {
      b5 = 0;
      s5 = k;
      s2 += k;
    }
  else
    {
      b2 -= k;
      b5 = -k;
      s5 = 0;
    }
  if (mode < 0 || mode > 9)
    mode = 0;
  try_quick = 1;
  if (mode > 5)
    {
      mode -= 4;
      try_quick = 0;
    }
  leftright = 1;
  ilim = ilim1 = -1;
  switch (mode)
    {
    case 0:
    case 1:
      i = 18;
      ndigits = 0;
      break;
    case 2:
      leftright = 0;
    case 4:
      if (ndigits <= 0)
 ndigits = 1;
      ilim = ilim1 = i = ndigits;
      break;
    case 3:
      leftright = 0;
    case 5:
      i = ndigits + k + 1;
      ilim = i;
      ilim1 = i - 1;
      if (i <= 0)
 i = 1;
    }
  j = sizeof (u32);
  for (ptr->_result_k = 0; sizeof (_Bigint) - sizeof (u32) + j <= i;
       j <<= 1)
    ptr->_result_k++;
  ptr->_result = _Balloc (ptr, ptr->_result_k);
  s = s0 = (char *) ptr->_result;
  if (ilim >= 0 && ilim <= 14 && try_quick)
    {
      i = 0;
      d2.value = d.value;
      k0 = k;
      ilim0 = ilim;
      ieps = 2;
      if (k > 0)
 {
   ds = pow10Table[k & 0xf];
   j = k >> 4;
   if (j & 0x10)
     {
       j &= 0x10 - 1;
       d.value /= D_004325D8[5 - 1];
       ieps++;
     }
   for (; j; j >>= 1, i++)
     if (j & 1)
       {
  ieps++;
  ds *= D_004325D8[i];
       }
   d.value /= ds;
 }
      else if ((j1 = -k) != 0)
 {
   d.value *= pow10Table[j1 & 0xf];
   for (j = j1 >> 4; j; j >>= 1, i++)
     if (j & 1)
       {
  ieps++;
  d.value *= D_004325D8[i];
       }
 }
      if (k_check && d.value < 1. && ilim > 0)
 {
   if (ilim1 <= 0)
     goto fast_failed;
   ilim = ilim1;
   k--;
   d.value *= 10.;
   ieps++;
 }
      eps.value = ieps * d.value + 7.;
      (eps.word[1]) -= (53 - 1) * ((u32)0x100000L);
      if (ilim == 0)
 {
   S = mhi = 0;
   d.value -= 5.;
   if (d.value > eps.value)
     goto one_digit;
   if (d.value < -eps.value)
     goto no_digits;
   goto fast_failed;
 }
      if (leftright)
 {
   eps.value = 0.5 / pow10Table[ilim - 1] - eps.value;
   for (i = 0;;)
     {
       L = d.value;
       d.value -= L;
       *s++ = '0' + (int) L;
       if (d.value < eps.value)
  goto ret1;
       if (1. - d.value < eps.value)
  goto bump_up;
       if (++i >= ilim)
  break;
       eps.value *= 10.;
       d.value *= 10.;
     }
 }
      else
 {
   eps.value *= pow10Table[ilim - 1];
   for (i = 1;; i++, d.value *= 10.)
     {
       L = d.value;
       d.value -= L;
       *s++ = '0' + (int) L;
       if (i == ilim)
  {
    if (d.value > 0.5 + eps.value)
      goto bump_up;
    else if (d.value < 0.5 - eps.value)
      {
        while (*--s == '0');
        s++;
        goto ret1;
      }
    break;
  }
     }
 }
    fast_failed:
      s = s0;
      d.value = d2.value;
      k = k0;
      ilim = ilim0;
    }
  if (be >= 0 && k <= 14)
    {
      ds = pow10Table[k];
      if (ndigits < 0 && ilim <= 0)
 {
   S = mhi = 0;
   if (ilim < 0 || d.value <= 5 * ds)
     goto no_digits;
   goto one_digit;
 }
      for (i = 1;; i++)
 {
   L = d.value / ds;
   d.value -= L * ds;
   *s++ = '0' + (int) L;
   if (i == ilim)
     {
       d.value += d.value;
             if ((d.value > ds) || ((d.value == ds) && (L & 1)))
  {
  bump_up:
    while (*--s == '9')
      if (s == s0)
        {
   k++;
   *s = '0';
   break;
        }
    ++*s++;
  }
       break;
     }
   if (!(d.value *= 10.))
     break;
 }
      goto ret1;
    }
  m2 = b2;
  m5 = b5;
  mhi = mlo = 0;
  if (leftright)
    {
      if (mode < 2)
 {
   i =
     denorm ? be + (1023 + (53 - 1) - 1 + 1) :
     1 + 53 - bbits;
 }
      else
 {
   j = ilim - 1;
   if (m5 >= j)
     m5 -= j;
   else
     {
       s5 += j -= m5;
       b5 += j;
       m5 = 0;
     }
   if ((i = ilim) < 0)
     {
       m2 -= i;
       i = 0;
     }
 }
      b2 += i;
      s2 += i;
      mhi = _i2b (ptr, 1);
    }
  if (m2 > 0 && s2 > 0)
    {
      i = m2 < s2 ? m2 : s2;
      b2 -= i;
      m2 -= i;
      s2 -= i;
    }
  if (b5 > 0)
    {
      if (leftright)
 {
   if (m5 > 0)
     {
       mhi = _pow5mult (ptr, mhi, m5);
       b1 = _multiply (ptr, mhi, b);
       _Bfree (ptr, b);
       b = b1;
     }
         if ((j = b5 - m5) != 0)
     b = _pow5mult (ptr, b, j);
 }
      else
 b = _pow5mult (ptr, b, b5);
    }
  S = _i2b (ptr, 1);
  if (s5 > 0)
    S = _pow5mult (ptr, S, s5);
  spec_case = 0;
  if (mode < 2)
    {
      if (!(d.word[0]) && !((d.word[1]) & ((u32)0xfffffL))
   && (d.word[1]) & ((u32)0x7ff00000L)
 )
 {
   b2 += 1;
   s2 += 1;
   spec_case = 1;
 }
    }
  if ((i = ((s5 ? 32 - _hi0bits (S->x[S->wds - 1]) : 1) + s2) & 0x1f) != 0)
    i = 32 - i;
  if (i > 4)
    {
      i -= 4;
      b2 += i;
      m2 += i;
      s2 += i;
    }
  else if (i < 4)
    {
      i += 28;
      b2 += i;
      m2 += i;
      s2 += i;
    }
  if (b2 > 0)
    b = _lshift (ptr, b, b2);
  if (s2 > 0)
    S = _lshift (ptr, S, s2);
  if (k_check)
    {
      if (__mcmp (b, S) < 0)
 {
   k--;
   b = _multadd (ptr, b, 10, 0);
   if (leftright)
     mhi = _multadd (ptr, mhi, 10, 0);
   ilim = ilim1;
 }
    }
  if (ilim <= 0 && mode > 2)
    {
      if (ilim < 0 || __mcmp (b, S = _multadd (ptr, S, 5, 0)) <= 0)
 {
 no_digits:
   k = -1 - ndigits;
   goto ret;
 }
    one_digit:
      *s++ = '1';
      k++;
      goto ret;
    }
  if (leftright)
    {
      if (m2 > 0)
 mhi = _lshift (ptr, mhi, m2);
      mlo = mhi;
      if (spec_case)
 {
   mhi = _Balloc (ptr, mhi->k);
   memcpy((char *)&mhi->sign, (char *)&mlo->sign, mlo->wds*sizeof(s32) + 2*sizeof(int));
   mhi = _lshift (ptr, mhi, 1);
 }
      for (i = 1;; i++)
 {
   dig = quorem (b, S) + '0';
   j = __mcmp (b, mlo);
   delta = __mdiff (ptr, S, mhi);
   j1 = delta->sign ? 1 : __mcmp (b, delta);
   _Bfree (ptr, delta);
   if (j1 == 0 && !mode && !((d.word[0]) & 1))
     {
       if (dig == '9')
  goto round_9_up;
       if (j > 0)
  dig++;
       *s++ = dig;
       goto ret;
     }
         if ((j < 0) || ((j == 0) && !mode
       && !((d.word[0]) & 1)
           ))
     {
       if (j1 > 0)
  {
    b = _lshift (ptr, b, 1);
    j1 = __mcmp (b, S);
                 if (((j1 > 0) || ((j1 == 0) && (dig & 1)))
        && dig++ == '9')
      goto round_9_up;
  }
       *s++ = dig;
       goto ret;
     }
   if (j1 > 0)
     {
       if (dig == '9')
  {
  round_9_up:
    *s++ = '9';
    goto roundoff;
  }
       *s++ = dig + 1;
       goto ret;
     }
   *s++ = dig;
   if (i == ilim)
     break;
   b = _multadd (ptr, b, 10, 0);
   if (mlo == mhi)
     mlo = mhi = _multadd (ptr, mhi, 10, 0);
   else
     {
       mlo = _multadd (ptr, mlo, 10, 0);
       mhi = _multadd (ptr, mhi, 10, 0);
     }
 }
    }
  else
    for (i = 1;; i++)
      {
 *s++ = dig = quorem (b, S) + '0';
 if (i >= ilim)
   break;
 b = _multadd (ptr, b, 10, 0);
      }
  b = _lshift (ptr, b, 1);
  j = __mcmp (b, S);
  if ((j > 0) || ((j == 0) && (dig & 1)))
    {
    roundoff:
      while (*--s == '9')
 if (s == s0)
   {
     k++;
     *s++ = '1';
     goto ret;
   }
      ++*s++;
    }
  else
    {
      while (*--s == '0');
      s++;
    }
ret:
  _Bfree (ptr, S);
  if (mhi)
    {
      if (mlo && mlo != mhi)
 _Bfree (ptr, mlo);
      _Bfree (ptr, mhi);
    }
ret1:
  _Bfree (ptr, b);
  *s = 0;
  *decpt = k + 1;
  if (rve)
    *rve = s;
  return s0;
}

/* newlib stdio/flags.c: __sflags; binary/text flags are absent on this target. */
s32 func_003625C0(_reent *r, const char *mode, s32 *openFlags) {
    s32 flags, access, options;

    switch (mode[0]) {
    case 'r':
        flags = 0x4;
        access = 0;
        options = 0;
        break;
    case 'w':
        flags = 0x8;
        access = 1;
        options = 0x600;
        break;
    case 'a':
        flags = 0x108;
        access = 1;
        options = 0x208;
        break;
    default:
        r->_errno = 22;
        return 0;
    }
    if (mode[1] && (mode[1] == '+' || mode[2] == '+')) {
        flags = 0x10;
        access = 2;
    }
    *openFlags = access | options;
    return flags;
}

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

long _lseek_r(_reent *r, s32 fd, long offset, s32 whence) {
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

/* newlib mprec multiplication, packed 32-bit limbs and little-endian Storeinc. */
_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b)
{
    _Bigint *c;
    s32 k, wa, wb, wc;
    u32 carry, y, z;
    u32 *x, *xa, *xae, *xb, *xbe, *xc, *xc0;
    u32 z2;

    if (a->wds < b->wds) {
        c = a;
        a = b;
        b = c;
    }
    k = a->k;
    wa = a->wds;
    wb = b->wds;
    wc = wa + wb;
    if (wc > a->maxwds)
        k++;
    c = _Balloc(r, k);
    for (x = c->x, xa = x + wc; x < xa; x++)
        *x = 0;
    xa = a->x;
    xae = xa + wa;
    xb = b->x;
    xbe = xb + wb;
    xc0 = c->x;
    for (; xb < xbe; xb++, xc0++) {
        if ((y = *xb & 0xffff) != 0) {
            x = xa;
            xc = xc0;
            carry = 0;
            do {
                z = (*x & 0xffff) * y + (*xc & 0xffff) + carry;
                carry = z >> 16;
                z2 = (*x++ >> 16) * y + (*xc >> 16) + carry;
                carry = z2 >> 16;
                ((u16 *)xc)[1] = z2;
                ((u16 *)xc)[0] = z;
                xc++;
            } while (x < xae);
            *xc = carry;
        }
        if ((y = *xb >> 16) != 0) {
            x = xa;
            xc = xc0;
            carry = 0;
            z2 = *xc;
            do {
                z = (*x & 0xffff) * y + (*xc >> 16) + carry;
                carry = z >> 16;
                ((u16 *)xc)[1] = z;
                ((u16 *)xc)[0] = z2;
                xc++;
                z2 = (*x++ >> 16) * y + (*xc & 0xffff) + carry;
                carry = z2 >> 16;
            } while (x < xae);
            *xc = z2;
        }
    }
    for (xc0 = c->x, xc = xc0 + wc; wc > 0 && !*--xc; --wc)
        ;
    c->wds = wc;
    return c;
}

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

_Bigint *_lshift(_reent *r, _Bigint *b, s32 shift) {
    _Bigint *b1;
    u32 *src;
    u32 *dst;
    u32 *end;
    u32 carry;
    s32 n;
    s32 wds;
    s32 bitCount;
    s32 count;

    n = shift >> 5;
    count = b->maxwds;
    wds = n + b->wds + 1;
    bitCount = b->k;
    if (count < wds) {
        do {
            count *= 2;
            bitCount++;
        } while (count < wds);
    }
    b1 = _Balloc(r, bitCount);
    dst = b1->x;
    count = 0;
    while (count < n) {
        *dst++ = 0;
        count++;
    }
    src = b->x;
    shift &= 0x1F;
    end = src + b->wds;
    if (shift != 0) {
        bitCount = 0x20 - shift;
        carry = 0;
        do {
            *dst++ = (*src << shift) | carry;
            carry = *src++ >> bitCount;
        } while (src < end);
        *dst = carry;
        if (carry != 0) {
            wds++;
        }
    } else {
        do {
            *dst++ = *src++;
        } while (src < end);
    }
    b1->wds = wds - 1;
    _Bfree(r, b);
    return b1;
}

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

_Bigint *__mdiff(_reent *r, _Bigint *a, _Bigint *b) {
    s32 sign = __mcmp(a, b);
    _Bigint *result;
    u32 *srcA;
    u32 *srcB;
    u32 *dst;
    u32 *endA;
    u32 *endB;
    s32 countA;
    s32 countB;
    s32 borrow;
    s32 low;
    u32 limb;

    if (sign == 0) {
        result = _Balloc(r, 0);
        result->wds = 1;
        result->x[0] = 0;
        return result;
    }
    if (sign < 0) {
        result = a;
        a = b;
        b = result;
        sign = 1;
    } else {
        sign = 0;
    }

    result = _Balloc(r, a->k);
    result->sign = sign;
    srcA = a->x;
    srcB = b->x;
    dst = result->x;
    countA = a->wds;
    countB = b->wds;
    endA = srcA + countA;
    endB = srcB + countB;
    borrow = 0;
    do {
        u32 wordB;
        limb = *srcA++;
        wordB = *srcB++;
        low = (limb & 0xFFFF) - (wordB & 0xFFFF) + borrow;
        borrow = low >> 16;
        limb = (limb >> 16) - (wordB >> 16);
        limb += borrow;
        ((u16 *)dst)[0] = low;
        ((u16 *)dst)[1] = limb;
        dst++;
        borrow = (s32)limb >> 16;
    } while (srcB < endB);
    while (srcA < endA) {
        u32 word = *srcA++;
        low = (word & 0xFFFF) + borrow;
        borrow = low >> 16;
        limb = (word >> 16) + borrow;
        ((u16 *)dst)[0] = low;
        ((u16 *)dst)[1] = limb;
        dst++;
        borrow = (s32)limb >> 16;
    }
    dst--;
    while (*dst == 0) {
        dst--;
        countA--;
    }
    result->wds = countA;
    return result;
}

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

const s32 pow5MultTable[3] = {5, 25, 125};

const f64 pow10Table[25] = {
    1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
    1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
    1e20, 1e21, 1e22, 1e23, 1e24
};

const f64 D_004325D8[5] = {1e16, 1e32, 1e64, 1e128, 1e256};

const f64 D_00432600[5] = {1e-16, 1e-32, 1e-64, 1e-128, 1e-256};

INCLUDE_ASM(const s32, "sdk/libc", func_00363DB8);

INCLUDE_ASM(const s32, "sdk/libc", func_00363E28);

INCLUDE_ASM(const s32, "sdk/libc", func_00363E48);

INCLUDE_ASM(const s32, "sdk/libc", func_00364C70);

double func_00364CD0(const char *s, char **end) {
    return func_00363DB8(_impure_ptr, s, end);
}

float func_00364CF8(const char *s, char **end) {
    return func_00364CD0(s, end);
}

/* Historical newlib _strtoul_r: unsigned long is 64-bit on the target.
 * Preserve the legacy ctype input domain and caller-supplied base contract.
 * The early base exit uses the matched _strtol_r loop/finalization shape. */
unsigned long func_00364D20(_reent *r, const char *input, char **end, s32 base)
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
    cutoff = ~0UL / (unsigned long)base;
    cutlim = ~0UL % (unsigned long)base;
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
        acc = ~0UL;
        r->_errno = 34;
    } else if (negative) {
        acc = -acc;
    }
    if (end != 0)
        *end = (char *)(any ? s - 1 : input);
    return acc;
}

unsigned long func_00364F50(const char *s, char **end, s32 base) {
    return func_00364D20(_impure_ptr, s, end, base);
}

/* Newlib-family ungetc storage growth, using the stream's real reserve buffer. */
s32 func_00364F80(FILE *fp)
{
    s32 i;
    u8 *p;

    if (fp->_ub._base == fp->_ubuf) {
        if ((p = _malloc_r(fp->_data, 1024)) == NULL)
            return -1;
        fp->_ub._base = p;
        fp->_ub._size = 1024;
        p += 1024 - sizeof(fp->_ubuf);
        for (i = sizeof(fp->_ubuf); --i >= 0;)
            p[i] = fp->_ubuf[i];
        fp->_p = p;
        return 0;
    }
    i = fp->_ub._size;
    p = _realloc_r(fp->_data, fp->_ub._base, i << 1);
    if (p == NULL)
        return -1;
    memcpy(p + i, p, i);
    fp->_p = p + i;
    fp->_ub._base = p;
    fp->_ub._size = i << 1;
    return 0;
}

s32 func_00365068(s32 c, FILE *fp)
{
    if (c == -1)
        return -1;
    /* Historical CHECK_INIT, also used by the matched stream functions. */
    do {
        if (fp->_data == NULL)
            fp->_data = _impure_ptr;
        if (!fp->_data->__sdidinit)
            __sinit(fp->_data);
    } while (0);
    fp->_flags &= ~0x20;
    if ((fp->_flags & 4) == 0) {
        if ((fp->_flags & 0x10) == 0)
            return -1;
        if (fp->_flags & 8) {
            if (fflush(fp))
                return -1;
            fp->_flags &= ~8;
            fp->_w = 0;
            fp->_lbfsize = 0;
        }
        fp->_flags |= 4;
    }
    c = (u8)c;
    if (fp->_ub._base != NULL) {
        if (fp->_r >= fp->_ub._size && func_00364F80(fp))
            return -1;
        *--fp->_p = c;
        fp->_r++;
        return c;
    }
    if (fp->_bf._base != NULL && fp->_p > fp->_bf._base && fp->_p[-1] == c) {
        fp->_p--;
        fp->_r++;
        return c;
    }
    fp->_ur = fp->_r;
    fp->_up = fp->_p;
    fp->_ub._base = fp->_ubuf;
    fp->_ub._size = sizeof(fp->_ubuf);
    fp->_ubuf[sizeof(fp->_ubuf) - 1] = c;
    fp->_p = &fp->_ubuf[sizeof(fp->_ubuf) - 1];
    fp->_r = 1;
    return c;
}

/* newlib mallocr.c: allocated chunks borrow the next chunk's previousSize
 * word while its PREV_INUSE bit is set. The usable extent is size - 4,
 * not size - 8. The historical MALLOC_ZERO macro handles 3/5/7/9 words;
 * this allocator rounds chunks to 16 bytes (minimum 16), so fresh small
 * chunks use 3 or 7 words. Preserve the historical one-pass macro block.
 * This is fresh allocator payload, not traversal of a typed record. */
void *_calloc_r(_reent *r, u32 n, u32 size) {
    mallocChunkPrefix_t *chunk;
    u32 chunkSize;
    u32 bytes = n * size;
    void *memory = _malloc_r(r, bytes);

    if (memory == NULL) {
        return NULL;
    } else {
        chunk = (mallocChunkPrefix_t *)((u8 *)memory - 8);
        chunkSize = chunk->size & ~3U;
        do {
            u32 clearSize = chunkSize - sizeof(u32);
            if (clearSize <= 9 * sizeof(clearSize)) {
                u32 *words = (u32 *)memory;
                if (clearSize >= 5 * sizeof(clearSize)) {
                    *words++ = 0;
                    *words++ = 0;
                    if (clearSize >= 7 * sizeof(clearSize)) {
                        *words++ = 0;
                        *words++ = 0;
                        if (clearSize >= 9 * sizeof(clearSize)) {
                            *words++ = 0;
                            *words++ = 0;
                        }
                    }
                }
                *words++ = 0;
                *words++ = 0;
                *words = 0;
            } else {
                memset(memory, 0, clearSize);
            }
        } while (0);
        return memory;
    }
}

/* Newlib 1.10 mallocr.c realloc with target-selected 16-byte alignment,
 * 32-bit INTERNAL_SIZE_T, and no mmap/zero-size-free paths. The size_t-to-long
 * comparisons intentionally retain the historical target's 64-bit long.
 * MALLOC_COPY's real do/while blocks copy allocator payload, not chunk fields:
 * the payload starts eight bytes after the typed header and may borrow the
 * next chunk's previousSize word while PREV_INUSE is set. All header accesses
 * use mallocChunkPrefix_t. The ascending copy supports backward coalescing.
 * Historical source: mirror/newlib-cygwin, newlib-1_10_0,
 * newlib/libc/stdlib/mallocr.c, realloc and MALLOC_COPY.
 */
void *_realloc_r(_reent *r, void *oldmem, size_t bytes)
{
    u32 nb;
    mallocChunkPrefix_t * oldp;
    u32 oldsize;
    mallocChunkPrefix_t * newp;
    u32 newsize;
    void* newmem;
    mallocChunkPrefix_t * next;
    u32 nextsize;
    mallocChunkPrefix_t * prev;
    u32 prevsize;
    mallocChunkPrefix_t * remainder;
    u32 remainder_size;
    mallocChunkPrefix_t * bck;
    mallocChunkPrefix_t * fwd;
    if (oldmem == 0) return _malloc_r(r, bytes);
    __malloc_lock(r);
    newp = oldp = ((mallocChunkPrefix_t *)((char*)(oldmem) - 2*4U));
    newsize = oldsize = ((oldp)->size & ~(3U));
    nb = (((long)((bytes) + (4U + 15U)) < (long)(16U + 15U)) ? ((16U + 15U) & ~(15U)) : (((bytes) + (4U + 15U)) & ~(15U)));
    ;
    if ((long)(oldsize) < (long)(nb))
    {
        next = ((mallocChunkPrefix_t *)(((char*)(oldp)) + (oldsize)));
        if (next == __malloc_av_.top || !((((mallocChunkPrefix_t *)(((char*)(next))+((next)->size & ~1U)))->size) & 1U))
        {
            nextsize = ((next)->size & ~(3U));
            if (next == __malloc_av_.top)
            {
                if ((long)(nextsize + newsize) >= (long)(nb + 16U))
                {
                    newsize += nextsize;
                    __malloc_av_.top = ((mallocChunkPrefix_t *)(((char*)(oldp)) + (nb)));
                    ((__malloc_av_.top)->size = ((newsize - nb) | 1U));
                    ((oldp)->size = (((oldp)->size & 1U) | (nb)));
                    __malloc_unlock(r);
                    return ((void*)((char*)(oldp) + 2*4U));
                }
            }
            else if (((long)(nextsize + newsize) >= (long)(nb)))
            {
                {
                    bck = next->backward;
                    fwd = next->forward;
                    fwd->backward = bck;
                    bck->forward = fwd;
                }
                ;
                newsize += nextsize;
                goto split;
            }
        }
        else
        {
            next = 0;
            nextsize = 0;
        }
        if (!((oldp)->size & 1U))
        {
            prev = ((mallocChunkPrefix_t *)( ((char*)(oldp)) - ((oldp)->previousSize) ));
            prevsize = ((prev)->size & ~(3U));
            if (next != 0)
            {
                if (next == __malloc_av_.top)
                {
                    if ((long)(nextsize + prevsize + newsize) >= (long)(nb + 16U))
                    {
                        {
                            bck = prev->backward;
                            fwd = prev->forward;
                            fwd->backward = bck;
                            bck->forward = fwd;
                        }
                        ;
                        newp = prev;
                        newsize += prevsize + nextsize;
                        newmem = ((void*)((char*)(newp) + 2*4U));
                        do {
                            u32 mcsz = (oldsize - 4U);
                            if(mcsz <= 9*sizeof(mcsz)) {
                                u32* mcsrc = (u32*) (oldmem);
                                u32* mcdst = (u32*) (newmem);
                                if(mcsz >= 5*sizeof(mcsz)) {
                                    *mcdst++ = *mcsrc++;
                                    *mcdst++ = *mcsrc++;
                                    if(mcsz >= 7*sizeof(mcsz)) {
                                        *mcdst++ = *mcsrc++;
                                        *mcdst++ = *mcsrc++;
                                        if(mcsz >= 9*sizeof(mcsz)) {
                                            *mcdst++ = *mcsrc++;
                                            *mcdst++ = *mcsrc++;
                                        }
                                    }
                                }
                                *mcdst++ = *mcsrc++;
                                *mcdst++ = *mcsrc++;
                                *mcdst = *mcsrc ;
                            }
                            else memcpy(newmem, oldmem, mcsz);
                        }
                        while(0);
                        __malloc_av_.top = ((mallocChunkPrefix_t *)(((char*)(newp)) + (nb)));
                        ((__malloc_av_.top)->size = ((newsize - nb) | 1U));
                        ((newp)->size = (((newp)->size & 1U) | (nb)));
                        __malloc_unlock(r);
                        return newmem;
                    }
                }
                else if (((long)(nextsize + prevsize + newsize) >= (long)(nb)))
                {
                    {
                        bck = next->backward;
                        fwd = next->forward;
                        fwd->backward = bck;
                        bck->forward = fwd;
                    }
                    ;
                    {
                        bck = prev->backward;
                        fwd = prev->forward;
                        fwd->backward = bck;
                        bck->forward = fwd;
                    }
                    ;
                    newp = prev;
                    newsize += nextsize + prevsize;
                    newmem = ((void*)((char*)(newp) + 2*4U));
                    do {
                        u32 mcsz = (oldsize - 4U);
                        if(mcsz <= 9*sizeof(mcsz)) {
                            u32* mcsrc = (u32*) (oldmem);
                            u32* mcdst = (u32*) (newmem);
                            if(mcsz >= 5*sizeof(mcsz)) {
                                *mcdst++ = *mcsrc++;
                                *mcdst++ = *mcsrc++;
                                if(mcsz >= 7*sizeof(mcsz)) {
                                    *mcdst++ = *mcsrc++;
                                    *mcdst++ = *mcsrc++;
                                    if(mcsz >= 9*sizeof(mcsz)) {
                                        *mcdst++ = *mcsrc++;
                                        *mcdst++ = *mcsrc++;
                                    }
                                }
                            }
                            *mcdst++ = *mcsrc++;
                            *mcdst++ = *mcsrc++;
                            *mcdst = *mcsrc ;
                        }
                        else memcpy(newmem, oldmem, mcsz);
                    }
                    while(0);
                    goto split;
                }
            }
            if (prev != 0 && (long)(prevsize + newsize) >= (long)nb)
            {
                {
                    bck = prev->backward;
                    fwd = prev->forward;
                    fwd->backward = bck;
                    bck->forward = fwd;
                }
                ;
                newp = prev;
                newsize += prevsize;
                newmem = ((void*)((char*)(newp) + 2*4U));
                do {
                    u32 mcsz = (oldsize - 4U);
                    if(mcsz <= 9*sizeof(mcsz)) {
                        u32* mcsrc = (u32*) (oldmem);
                        u32* mcdst = (u32*) (newmem);
                        if(mcsz >= 5*sizeof(mcsz)) {
                            *mcdst++ = *mcsrc++;
                            *mcdst++ = *mcsrc++;
                            if(mcsz >= 7*sizeof(mcsz)) {
                                *mcdst++ = *mcsrc++;
                                *mcdst++ = *mcsrc++;
                                if(mcsz >= 9*sizeof(mcsz)) {
                                    *mcdst++ = *mcsrc++;
                                    *mcdst++ = *mcsrc++;
                                }
                            }
                        }
                        *mcdst++ = *mcsrc++;
                        *mcdst++ = *mcsrc++;
                        *mcdst = *mcsrc ;
                    }
                    else memcpy(newmem, oldmem, mcsz);
                }
                while(0);
                goto split;
            }
        }
        newmem = _malloc_r (r, bytes);
        if (newmem == 0)
        {
            __malloc_unlock(r);
            return 0;
        }
        if ( (newp = ((mallocChunkPrefix_t *)((char*)(newmem) - 2*4U))) == ((mallocChunkPrefix_t *)( ((char*)(oldp)) + ((oldp)->size & ~1U) )))
        {
            newsize += ((newp)->size & ~(3U));
            newp = oldp;
            goto split;
        }
        do {
            u32 mcsz = (oldsize - 4U);
            if(mcsz <= 9*sizeof(mcsz)) {
                u32* mcsrc = (u32*) (oldmem);
                u32* mcdst = (u32*) (newmem);
                if(mcsz >= 5*sizeof(mcsz)) {
                    *mcdst++ = *mcsrc++;
                    *mcdst++ = *mcsrc++;
                    if(mcsz >= 7*sizeof(mcsz)) {
                        *mcdst++ = *mcsrc++;
                        *mcdst++ = *mcsrc++;
                        if(mcsz >= 9*sizeof(mcsz)) {
                            *mcdst++ = *mcsrc++;
                            *mcdst++ = *mcsrc++;
                        }
                    }
                }
                *mcdst++ = *mcsrc++;
                *mcdst++ = *mcsrc++;
                *mcdst = *mcsrc ;
            }
            else memcpy(newmem, oldmem, mcsz);
        }
        while(0);
        _free_r(r, oldmem);
        __malloc_unlock(r);
        return newmem;
    }
    split:
    remainder_size = ((long)((newsize)-(nb)));
    if (remainder_size >= (long)16U)
    {
        remainder = ((mallocChunkPrefix_t *)(((char*)(newp)) + (nb)));
        ((newp)->size = (((newp)->size & 1U) | (nb)));
        ((remainder)->size = (remainder_size | 1U));
        (((mallocChunkPrefix_t *)(((char*)(remainder)) + (remainder_size)))->size |= 1U);
        _free_r(r, ((void*)((char*)(remainder) + 2*4U)));
    }
    else
    {
        ((newp)->size = (((newp)->size & 1U) | (newsize)));
        (((mallocChunkPrefix_t *)(((char*)(newp)) + (newsize)))->size |= 1U);
    }
    ;
    __malloc_unlock(r);
    return ((void*)((char*)(newp) + 2*4U));
}

INCLUDE_RODATA(const s32, "sdk/libc", D_004326E8);

INCLUDE_RODATA(const s32, "sdk/libc", D_004326F0);

INCLUDE_RODATA(const s32, "sdk/libc", D_004326F8);

INCLUDE_RODATA(const s32, "sdk/libc", D_00432700);

INCLUDE_RODATA(const s32, "sdk/libc", kernlInternalErrorMsg);

INCLUDE_RODATA(const s32, "sdk/libc", ttyMsgPacketTooLarge);

INCLUDE_RODATA(const s32, "sdk/libc", ttyMsgReceiveError);

INCLUDE_RODATA(const s32, "sdk/libc", ttyMsgSendError);

INCLUDE_RODATA(const s32, "sdk/libc", ttyMsgWlenError);

