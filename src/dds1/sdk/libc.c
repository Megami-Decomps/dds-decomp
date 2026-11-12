#include "sdk_libc.h"
#include "common.h"
#include "sdk_libgcc.h"
#include "newlib_stat.h"
#include "ps2asm.h"
#include "dds1/sdk/libc_internal.h"

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
extern _reent impure_data;

extern FILE *func_002FE870(_reent *r, const char *filename, const char *mode);

extern s32 _open_r(_reent *r, const char *path, s32 flags, s32 mode);

extern s32 func_002FEE38(FILE *fp, long offset, s32 whence);

extern s32 _puts_r(_reent *r, const char *s);

extern s32 _raise_r(_reent *r, s32 sig);

extern void *_signal_r(_reent *r, s32 sig, void *handler);

extern void _cleanup_r(_reent *r);

extern s32 _fwalk(_reent *r, s32 (*func)(void *));

extern s32 libc_2DC670(void);

extern long _strtol_r(_reent *r, const char *s, char **end, s32 base);

extern s32 raise(s32 sig);

extern void _exit(s32 code) __attribute__((noreturn));

extern s32 mallocLockCount __attribute__((section(".data")));

extern s32 mallocLockOwner __attribute__((section(".data")));

extern s32 mallocSema __attribute__((section(".data")));

extern s32 _init_signal_r(_reent *r);

extern void std(FILE *fp, s32 flags, s32 file, _reent *data);

extern _glue *__sfmoreglue(_reent *r, s32 n);

extern s32 __sigtramp_r(_reent *r, s32 sig);

extern void *__deregister_frame_info(void *fn);

extern void free(void *ptr);

extern void *malloc(size_t size);

extern s32 SignalSema(s32 sema);

extern void *sbrk(s32 incr);
extern void *_sbrk_r(_reent *r, s32 incr);

extern s32 open(const char *path, s32 flags, s32 mode);

extern void __register_frame_info(void *fn, void *entry);

extern s32 __register_frame_info_table(void *fn, void *entry);

extern s32 fstat(s32 fd, struct stat *buf);

extern s32 kill(s32 pid, s32 sig);
extern s32 vfiprintf(FILE *fp, const char *fmt, va_list ap);
extern s32 fiprintf(FILE *fp, const char *fmt, ...);

extern s32 __sread(void *cookie, char *buf, s32 n);

extern s32 __swrite(void *cookie, const char *buf, s32 n);

extern long __sseek(void *cookie, long offset, s32 whence);

extern s32 __sclose(void *cookie);

extern s32 GetThreadId(void);

extern s32 WaitSema(s32 sema);

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

typedef struct mallocInfo_t {
    s32 arena;
    u8 opaque04[0x24];
} mallocInfo_t;
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

extern const char D_0054B658[8] __attribute__((section(".rodata")));

extern const char putsNewline[16];

/* NON_MATCHING: native 556-byte extent; 65 differing words in limb-loop allocation. */
extern const char dtoaInfinityText[], dtoaNanText[];
extern const f64 dtoaLog10Slope __attribute__((section(".rodata")));
extern const f64 dtoaLog10Offset __attribute__((section(".rodata")));
extern const f64 dtoaLog10Of2 __attribute__((section(".rodata")));

extern const char dtoaInfinityText[16];

extern const char dtoaNanText[8] __attribute__((section(".rodata")));

/* NON_MATCHING: native 756-byte size; chunk-size/successor allocation
 * and coalescing-path scheduling differ in 103 instruction words. */
extern int D_0039C438 __attribute__((section(".data")));

extern s32 _fstat_r(_reent *r, s32 fd, struct stat *buf);
extern s32 isatty(s32 fd);

/* Ordinary source reconstruction of the newlib decimal-string to Bigint helper. */

/* Qualified allocator controls for the two count lifetimes are recorded in
 * docs/levers/cross-block-locals.md, "Retain bigint limb and bit counts". */

/* The shared pointer and limb intermediates have qualified allocator controls;
 * see docs/levers/cross-block-locals.md, "Retain bigint subtraction values". */

extern f64 dtoaLog10Slope __attribute__((section(".rodata")));

extern f64 dtoaLog10Offset __attribute__((section(".rodata")));

extern const f64 dtoaLog10Of2 __attribute__((section(".rodata")));

extern const char localeDecimalPoint[8] __attribute__((section(".rodata")));



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
FILE *func_002FE870(_reent *r, const char *filename, const char *mode) {
    FILE *fp;
    s32 fd;
    s32 flags, oflags;

    if ((flags = func_00307250(r, mode, &oflags)) == 0)
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
        func_002FEE38(fp, 0, 2);
    return fp;
}

/* newlib stdio/fopen.c: forward through the current reentrancy object. */
FILE *fopen(const char *filename, const char *mode) {
    return func_002FE870(_impure_ptr, filename, mode);
}

s32 fprintf(FILE *fp, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    return vfprintf(fp, fmt, ap);
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

INCLUDE_ASM(const s32, "sdk/libc", func_002FEE38);

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

INCLUDE_ASM(const s32, "sdk/libc", func_00300000);

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

/* __srefill's _fwalk callback: flush line-buffered output streams. */
s32 func_00300EF8(void *stream)
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
s32 func_00301580(void *cookie, char *buf, s32 len) {
    return 0;
}

s32 sscanf(const char *str, const char *fmt, ...) {
    s32 result;
    va_list ap;
    FILE f;

    f._flags = 4;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._r = strlen(str);
    f._read = func_00301580;
    f._ub._base = NULL;
    f._lb._base = NULL;
    f._data = _impure_ptr;
    va_start(ap, fmt);
    result = func_00304D78(&f, fmt, ap);
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
s32 func_00302580(FILE *fp, __suio *uio) {
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
s32 func_003025C8(FILE *fp, const char *fmt, va_list ap)
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

/* newlib 1.9 ctype/ctype_.c (039e8a3c16964d02838b204b72bfdd4b7529107c).
 * The donor has an EOF slot, 128 explicit ASCII classifications, and 128
 * implicit zero entries. Alignment before the following object is not part
 * of this array. LIBC_CTYPE_* mechanically prefixes the donor flag names.
 */
/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

enum {
    LIBC_CTYPE_U = 01,
    LIBC_CTYPE_L = 02,
    LIBC_CTYPE_N = 04,
    LIBC_CTYPE_S = 010,
    LIBC_CTYPE_P = 020,
    LIBC_CTYPE_C = 040,
    LIBC_CTYPE_X = 0100,
    LIBC_CTYPE_B = 0200
};

const char _ctype_[1 + 256] = {
	0,
	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,
	LIBC_CTYPE_C,	LIBC_CTYPE_C|LIBC_CTYPE_S,	LIBC_CTYPE_C|LIBC_CTYPE_S,	LIBC_CTYPE_C|LIBC_CTYPE_S,	LIBC_CTYPE_C|LIBC_CTYPE_S,	LIBC_CTYPE_C|LIBC_CTYPE_S,	LIBC_CTYPE_C,	LIBC_CTYPE_C,
	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,
	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,	LIBC_CTYPE_C,
	LIBC_CTYPE_S|LIBC_CTYPE_B,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,
	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,
	LIBC_CTYPE_N,	LIBC_CTYPE_N,	LIBC_CTYPE_N,	LIBC_CTYPE_N,	LIBC_CTYPE_N,	LIBC_CTYPE_N,	LIBC_CTYPE_N,	LIBC_CTYPE_N,
	LIBC_CTYPE_N,	LIBC_CTYPE_N,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,
	LIBC_CTYPE_P,	LIBC_CTYPE_U|LIBC_CTYPE_X,	LIBC_CTYPE_U|LIBC_CTYPE_X,	LIBC_CTYPE_U|LIBC_CTYPE_X,	LIBC_CTYPE_U|LIBC_CTYPE_X,	LIBC_CTYPE_U|LIBC_CTYPE_X,	LIBC_CTYPE_U|LIBC_CTYPE_X,	LIBC_CTYPE_U,
	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,
	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,
	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_U,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,
	LIBC_CTYPE_P,	LIBC_CTYPE_L|LIBC_CTYPE_X,	LIBC_CTYPE_L|LIBC_CTYPE_X,	LIBC_CTYPE_L|LIBC_CTYPE_X,	LIBC_CTYPE_L|LIBC_CTYPE_X,	LIBC_CTYPE_L|LIBC_CTYPE_X,	LIBC_CTYPE_L|LIBC_CTYPE_X,	LIBC_CTYPE_L,
	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,
	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,
	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_L,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_P,	LIBC_CTYPE_C
};

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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
 *
 *	%W% (UofMD/Berkeley) %G%
 */

/* Adapted from newlib c87be3e4d6ae90dcc48b5f26dd87645e7cabff01
 * (2000-04-17), INTEGER_ONLY with default _NO_LONGLONG.
 * Preserve native long64/int32/pointer32 and compiler wchar types.
 * MB_CUR_MAX maps to the existing asm-owned int scalar; the adjoining
 * string bytes in its assembly label interval are not part of that int.
 */
#define BUF 40
#define __SNBF 0x0002
#define __SWR 0x0008
#define __SRW 0x0010
#define __SERR 0x0040
#define EOF (-1)
#define	cantwrite(fp) \
  ((((fp)->_flags & __SWR) == 0 || (fp)->_bf._base == NULL) && \
   __swsetup(fp))
#define __sferror(p) (((p)->_flags & __SERR) != 0)

/*
 * Macros for converting digits to letters and vice versa
 */
#define	to_digit(c)	((c) - '0')
#define is_digit(c)	((unsigned)to_digit(c) <= 9)
#define	to_char(n)	((n) + '0')

/*
 * Flags used during conversion.
 */
#define	ALT		0x001		/* alternate form */
#define	HEXPREFIX	0x002		/* add 0x or 0X prefix */
#define	LADJUST		0x004		/* left adjustment */
#define	LONGDBL		0x008		/* long double; unimplemented */
#define	LONGINT		0x010		/* long integer */
#define	QUADINT		0x020		/* quad integer */
#define	SHORTINT	0x040		/* short integer */
#define	ZEROPAD		0x080		/* zero (as opposed to blank) pad */
#define FPT		0x100		/* Floating point number */

INCLUDE_RODATA(const s32, "sdk/libc", D_003B7A98);

INCLUDE_RODATA(const s32, "sdk/libc", putsNewline);

int _vfiprintf_r(struct _reent *data, FILE *fp, const char *fmt0, va_list ap)
{
	register char *fmt;	/* format string */
	register int ch;	/* character from fmt */
	register int n, m;	/* handy integers (short term usage) */
	register char *cp;	/* handy char pointer (short term usage) */
	register struct __siov *iovp;/* for PRINT macro */
	register int flags;	/* flags as above */
	int ret;		/* return value accumulator */
	int width;		/* width from format (%8d), or 0 */
	int prec;		/* precision from format (%.3d), or -1 */
	char sign;		/* sign prefix (' ', '+', '-', or \0) */
	__WCHAR_TYPE__ wc;


	unsigned long _uquad;
	enum { OCT, DEC, HEX } base;/* base for [diouxX] conversion */
	int dprec;		/* a copy of prec if [diouxX], 0 otherwise */
	int realsz;		/* field size expanded by dprec */
	int size;		/* size of converted field or string */
	char *xdigs;		/* digits for [xX] conversion */
#define NIOV 8
	struct __suio uio;	/* output information: summary */
	struct __siov iov[NIOV];/* ... and individual io vectors */
	char buf[BUF];		/* space for %c, %[diouxX], %[eEfgG] */
	char ox[2];		/* space for 0x hex-prefix */
        int state = 0;          /* mbtowc calls from library must not change state */

	/*
	 * Choose PADSIZE to trade efficiency vs. size.  If larger printf
	 * fields occur frequently, increase PADSIZE and make the initialisers
	 * below longer.
	 */
#define	PADSIZE	16		/* pad chunk size */
	static const char blanks[PADSIZE] =
	 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	static const char zeroes[PADSIZE] =
	 {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

	/*
	 * BEWARE, these `goto error' on error, and PAD uses `n'.
	 */
#define	PRINT(ptr, len) { \
	iovp->iov_base = (ptr); \
	iovp->iov_len = (len); \
	uio.uio_resid += (len); \
	iovp++; \
	if (++uio.uio_iovcnt >= NIOV) { \
		if (func_00302580(fp, &uio)) \
			goto error; \
		iovp = iov; \
	} \
}
#define	PAD(howmany, with) { \
	if ((n = (howmany)) > 0) { \
		while (n > PADSIZE) { \
			PRINT(with, PADSIZE); \
			n -= PADSIZE; \
		} \
		PRINT(with, n); \
	} \
}
#define	FLUSH() { \
	if (uio.uio_resid && func_00302580(fp, &uio)) \
		goto error; \
	uio.uio_iovcnt = 0; \
	iovp = iov; \
}

	/*
	 * To extend shorts properly, we need both signed and unsigned
	 * argument extraction methods.
	 */
#define	SARG() \
	(flags&LONGINT ? __builtin_va_arg(ap, long) : \
	    flags&SHORTINT ? (long)(short)__builtin_va_arg(ap, int) : \
	    (long)__builtin_va_arg(ap, int))
#define	UARG() \
	(flags&LONGINT ? __builtin_va_arg(ap, unsigned long) : \
	    flags&SHORTINT ? (unsigned long)(unsigned short)__builtin_va_arg(ap, int) : \
	    (unsigned long)__builtin_va_arg(ap, unsigned int))

	/* sorry, fprintf(read_only_file, "") returns EOF, not 0 */
	if (cantwrite(fp))
		return (EOF);

	/* optimise fprintf(stderr) (and other unbuffered Unix files) */
	if ((fp->_flags & (__SNBF|__SWR|__SRW)) == (__SNBF|__SWR) &&
	    fp->_file >= 0)
		return (func_003025C8(fp, fmt0, ap));

	fmt = (char *)fmt0;
	uio.uio_iov = iovp = iov;
	uio.uio_resid = 0;
	uio.uio_iovcnt = 0;
	ret = 0;

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
	        cp = fmt;
	        while ((n = _mbtowc_r(_impure_ptr, &wc, fmt, D_0039C438, &state)) > 0) {
			fmt += n;
			if (wc == '%') {
				fmt--;
				break;
			}
		}
		if ((m = fmt - cp) != 0) {
			PRINT(cp, m);
			ret += m;
		}
		if (n <= 0)
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		dprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:		ch = *fmt++;
reswitch:	switch (ch) {
		case ' ':
			/*
			 * ``If the space and + flags both appear, the space
			 * flag will be ignored.''
			 *	-- ANSI X3J11
			 */
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
			/*
			 * ``A negative field width argument is taken as a
			 * - flag followed by a positive field width.''
			 *	-- ANSI X3J11
			 * They don't exclude field widths read from args.
			 */
			if ((width = __builtin_va_arg(ap, int)) >= 0)
				goto rflag;
			width = -width;
			/* FALLTHROUGH */
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				n = __builtin_va_arg(ap, int);
				prec = n < 0 ? -1 : n;
				goto rflag;
			}
			n = 0;
			while (is_digit(ch)) {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			}
			prec = n < 0 ? -1 : n;
			goto reswitch;
		case '0':
			/*
			 * ``Note that 0 is taken as a flag, not as the
			 * beginning of a field width.''
			 *	-- ANSI X3J11
			 */
			flags |= ZEROPAD;
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			width = n;
			goto reswitch;
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			if (*fmt == 'l') {
				fmt++;
				flags |= QUADINT;
			} else {
				flags |= LONGINT;
			}
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			*(cp = buf) = __builtin_va_arg(ap, int);
			size = 1;
			sign = '\0';
			break;
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
			_uquad = SARG();
			if ((long) _uquad < 0)
			{

				_uquad = -_uquad;
				sign = '-';
			}
			base = DEC;
			goto number;
		case 'n':
			if (flags & LONGINT)
				*__builtin_va_arg(ap, long *) = ret;
			else if (flags & SHORTINT)
				*__builtin_va_arg(ap, short *) = ret;
			else
				*__builtin_va_arg(ap, int *) = ret;
			continue;	/* no output */
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
			_uquad = UARG();
			base = OCT;
			goto nosign;
		case 'p':
			/*
			 * ``The argument shall be a pointer to void.  The
			 * value of the pointer is converted to a sequence
			 * of printable characters, in an implementation-
			 * defined manner.''
			 *	-- ANSI X3J11
			 */
			/* NOSTRICT */
			_uquad = (unsigned long)(unsigned long)__builtin_va_arg(ap, void *);
			base = HEX;
			xdigs = "0123456789abcdef";
			flags |= HEXPREFIX;
			ch = 'x';
			goto nosign;
		case 's':
			if ((cp = __builtin_va_arg(ap, char *)) == NULL) {
				static const char null_text[] __attribute__((section(".rodata"))) = "(null)";
				cp = null_text;
			}
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p = memchr(cp, 0, prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
			_uquad = UARG();
			base = DEC;
			goto nosign;
		case 'X':
			xdigs = "0123456789ABCDEF";
			goto hex;
		case 'x':
			xdigs = "0123456789abcdef";
hex:			_uquad = UARG();
			base = HEX;
			/* leading 0x/X only if non-zero */
			if (flags & ALT && _uquad != 0)
				flags |= HEXPREFIX;

			/* unsigned conversions */
nosign:			sign = '\0';
			/*
			 * ``... diouXx conversions ... if a precision is
			 * specified, the 0 flag will be ignored.''
			 *	-- ANSI X3J11
			 */
number:			if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;

			/*
			 * ``The result of converting a zero value with an
			 * explicit precision of zero is no characters.''
			 *	-- ANSI X3J11
			 */
			cp = buf + BUF;
			if (_uquad != 0 || prec != 0) {
				/*
				 * Unsigned mod is hard, and unsigned mod
				 * by a constant is easier than that by
				 * a variable; hence this switch.
				 */
				switch (base) {
				case OCT:
					do {
						*--cp = to_char(_uquad & 7);
						_uquad >>= 3;
					} while (_uquad);
					/* handle octal leading 0 */
					if (flags & ALT && *cp != '0')
						*--cp = '0';
					break;

				case DEC:
					/* many numbers are 1 digit */
					while (_uquad >= 10) {
						*--cp = to_char(_uquad % 10);
						_uquad /= 10;
					}
					*--cp = to_char(_uquad);
					break;

				case HEX:
					do {
						*--cp = xdigs[_uquad & 15];
						_uquad >>= 4;
					} while (_uquad);
					break;

				default:
					cp = "bug in vfprintf: bad base";
					size = strlen(cp);
					goto skipsize;
				}
			}
			size = buf + BUF - cp;
		skipsize:
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			/* pretend it was %c with argument ch */
			cp = buf;
			*cp = ch;
			size = 1;
			sign = '\0';
			break;
		}

		/*
		 * All reasonable formats wind up here.  At this point, `cp'
		 * points to a string which (if not flags&LADJUST) should be
		 * padded out to `width' places.  If flags&ZEROPAD, it should
		 * first be prefixed by any sign or other prefix; otherwise,
		 * it should be blank padded before the prefix is emitted.
		 * After any left-hand padding and prefixing, emit zeroes
		 * required by a decimal [diouxX] precision, then print the
		 * string proper, then emit zeroes required by any leftover
		 * floating precision; finally, if LADJUST, pad with blanks.
		 *
		 * Compute actual size, so we know how much to pad.
		 * size excludes decimal prec; realsz includes it.
		 */
		realsz = dprec > size ? dprec : size;
		if (sign)
			realsz++;
		else if (flags & HEXPREFIX)
			realsz+= 2;

		/* right-adjusting blank padding */
		if ((flags & (LADJUST|ZEROPAD)) == 0)
			PAD(width - realsz, blanks);

		/* prefix */
		if (sign) {
			PRINT(&sign, 1);
		} else if (flags & HEXPREFIX) {
			ox[0] = '0';
			ox[1] = ch;
			PRINT(ox, 2);
		}

		/* right-adjusting zero padding */
		if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
			PAD(width - realsz, zeroes);

		/* leading zeroes from decimal precision */
		PAD(dprec - size, zeroes);

		/* the string or number proper */
		PRINT(cp, size);
		/* left-adjusting padding (always blank) */
		if (flags & LADJUST)
			PAD(width - realsz, blanks);

		/* finally, adjust ret */
		ret += width > realsz ? width : realsz;

		FLUSH();	/* copy out the I/O vectors */
	}
done:
	FLUSH();
error:
	return (__sferror(fp) ? EOF : ret);
	/* NOTREACHED */
}

#undef BUF
#undef __SNBF
#undef __SWR
#undef __SRW
#undef __SERR
#undef EOF
#undef cantwrite
#undef __sferror
#undef to_digit
#undef is_digit
#undef to_char
#undef ALT
#undef HEXPREFIX
#undef LADJUST
#undef LONGDBL
#undef LONGINT
#undef QUADINT
#undef SHORTINT
#undef ZEROPAD
#undef FPT
#undef NIOV
#undef PADSIZE
#undef PRINT
#undef PAD
#undef FLUSH
#undef SARG
#undef UARG

