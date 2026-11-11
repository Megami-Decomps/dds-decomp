#include "common.h"
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
    struct sdfDrawTag_t *next; /* 0x00: also the replaced item in the key tree */
    s32 head;                  /* 0x04: first DMA packet address. */
    u8 tail[4];                /* 0x08 */
    f32 z;                     /* 0x0C: the sort key; the pool reads its bits */
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
    sdfSortOtNode_tag *data[1];                                /* 0x18: first key-tree root; the allocation continues */
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

extern s32 _puts_r(_reent *r, const char *s);

extern s32 _raise_r(_reent *r, s32 sig);

extern void *_signal_r(_reent *r, s32 sig, void *handler);

extern s32 _setlocale_r(_reent *r, s32 category, const char *locale);

extern void *_localeconv_r(_reent *r);

extern void _cleanup_r(_reent *r);

extern s32 fflush(void *fp);

extern s32 _fwalk(_reent *r, s32 (*func)(void *));

extern s32 libc_2DC670(void);

extern long _strtol_r(_reent *r, const char *s, char **end, s32 base);

extern long strtol(const char *s, char **end, s32 base);

extern s32 raise(s32 sig);

extern void _exit(s32 code) __attribute__((noreturn));

extern u8 localeconvData[] __attribute__((section(".data")));

extern s32 libcErrno __attribute__((section(".data")));

extern s32 mallocLockCount __attribute__((section(".data")));

extern s32 mallocLockOwner __attribute__((section(".data")));

extern s32 mallocSema __attribute__((section(".data")));

extern s32 _init_signal_r(_reent *r);

extern void gcc2_compiled__2(FILE *fp, s32 flags, s32 file, _reent *data);

extern _glue *__sfmoreglue(_reent *r, s32 n);

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

extern s32 close(s32 fd);

extern _Bigint *_Balloc(_reent *r, s32 k);

extern void __register_frame_info(void *fn, void *entry);

extern s32 __register_frame_info_table(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, s32 offset, s32 whence);

extern s32 fstat(s32 fd, void *buf);

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

sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *node, sdfSortOtNode_tag *parent) {
    sdfSortOtNode_tag *childRoot = node;
    sdfSortOtNode_tag *pivot;

    if (childRoot->balance < 0) {
        pivot = childRoot->left;
        parent->right = pivot->left;
        childRoot->left = pivot->right;
        pivot->left = parent;
        pivot->right = childRoot;
        if (pivot->balance > 0) {
            parent->balance = -1;
            childRoot->balance = 0;
        } else {
            parent->balance = 0;
            childRoot->balance = 1;
        }
        pivot->balance = 0;
    } else {
        parent->balance = 0;
        parent->right = childRoot->left;
        childRoot->balance = 0;
        childRoot->left = parent;
        pivot = childRoot;
    }
    return pivot;
}

sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *node, sdfSortOtNode_tag *parent) {
    sdfSortOtNode_tag *childRoot = node;
    sdfSortOtNode_tag *pivot;

    if (childRoot->balance > 0) {
        pivot = childRoot->right;
        parent->left = pivot->right;
        childRoot->right = pivot->left;
        pivot->right = parent;
        pivot->left = childRoot;
        if (pivot->balance > 0) {
            parent->balance = -1;
            childRoot->balance = 0;
        } else {
            parent->balance = 0;
            childRoot->balance = 1;
        }
        pivot->balance = 0;
    } else {
        parent->balance = 0;
        parent->left = childRoot->right;
        childRoot->balance = 0;
        childRoot->right = parent;
        pivot = childRoot;
    }
    return pivot;
}

void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **stack, s32 stack_depth, sdfSortOtNode_tag *node,
                   sdfSortOtNode_tag **root_p) {
    sdfSortOtNode_tag *parent;
    sdfSortOtNode_tag *replacement = NULL;
    s32 balance;

    if (stack_depth > 0) {
        do {
            parent = stack[--stack_depth];
            if (node == parent->right) {
                balance = ++parent->balance;
            } else {
                balance = --parent->balance;
            }
            if (balance == 0) {
                return;
            }
            if (balance > 1) {
                replacement = sdfSortOtAvlRotateLR(node, parent);
                break;
            }
            if (balance < -1) {
                replacement = sdfSortOtAvlRotateRL(node, parent);
                break;
            }
            node = parent;
        } while (stack_depth > 0);
    }
    if (stack_depth > 0) {
        sdfSortOtNode_tag *ancestor = stack[--stack_depth];
        if (ancestor->right == parent) {
            ancestor->right = replacement;
        } else {
            ancestor->left = replacement;
        }
    } else if (replacement != NULL) {
        *root_p = replacement;
    }
}

void sdfSortOtAvlRegist(sdfSortOtNode_tag **root_p, sdfDrawTag_t *tag) {
    sdfSortOtNode_tag *path[SDF_KEY_TREE_PATH_LIMIT];
    s32 depth = 0;
    sdfSortOtNode_tag **link = root_p;
    sdfSortOtNode_tag *cur = *root_p;
    sdfSortOtNode_tag *node;
    f32 key = tag->z;

    while (cur != NULL) {
        if (key == cur->tag->z) {
            tag->next = cur->tag;
            cur->tag = tag;
            return;
        }
        path[depth] = cur;
        depth++;
        if (cur->tag->z < key) {
            link = &cur->right;
        } else {
            link = &cur->left;
        }
        cur = *link;
    }
    tag->next = NULL;
    node = EE_ADDR_PTR(sdfGrAllocPacketBuff(0x10));
    node->right = NULL;
    node->left = NULL;
    node->tag = tag;
    *link = node;
    node->balance = 0;
    if (depth != 0) {
        sdfSortOtAvlCheckBalance(path, depth, node, root_p);
    }
}

void sdfGrSortOtReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag) {
    if (aTag->head != 0) {
        if (*(s32 *)&aTag->z == SDF_POOL_FREE_KIND) {
            aTag->next = aOt->noSortTag;
            aOt->noSortTag = aTag;
        } else {
            /* The key tree views the pool node's +0xC word as its float key. */
            sdfSortOtAvlRegist(aOt->data, aTag);
        }
    }
}

void sdfGrSortOtFixup(void *tag, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p) {
    sdfDrawTag_t *current;
    sdfDrawTag_t *next;

    if (tag != NULL) {
        if (*tail_p == NULL) {
            *head_p = tag;
        } else {
            sdfGrJoinDrawTag(*tail_p, tag);
        }
        current = tag;
        while ((next = current->next) != NULL) {
            sdfGrJoinDrawTag(current, next);
            current = next;
        }
        *tail_p = current;
    }
}

void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p) {
    sdfSortOtNode_tag *next;

    do {
        if (node->right != NULL) {
            sdfGrSortOtTraverse(node->right, head_p, tail_p);
        }
        sdfGrSortOtFixup(node->tag, head_p, tail_p);
        next = node->left;
        node = next;
    } while (next != NULL);
}

void sdfGrSortOtSetup(sdfGrOt_t *aOt) {
    sdfDrawTag_t *head = NULL;
    sdfDrawTag_t *tail = NULL;

    sdfGrSortOtFixup(aOt->noSortTag, &head, &tail);
    if (aOt->data[0] != NULL) {
        sdfGrSortOtTraverse(aOt->data[0], &head, &tail);
    }
    aOt->tagHead = head;
    aOt->tagTail = tail;
}

void sdfGrSortOtCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData) {
    switch (aFunc) {
    case SDF_POOL_REBUILD_CHAIN:
        sdfGrSortOtSetup(aOt);
        break;
    case SDF_POOL_PUSH_FREE:
        if (aData->head != 0) {
            aData->next = aOt->noSortTag;
            aOt->noSortTag = aData;
        }
        break;
    }
}

void sdfModelSetOtCallbacks(sdfGrOt_t *aOt) {
    aOt->regFunc = sdfGrSortOtReg;
    aOt->callbackFunc = sdfGrSortOtCallback;
}

