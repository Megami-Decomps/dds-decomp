#ifndef DDS1_SDK_LIBC_INTERNAL_H
#define DDS1_SDK_LIBC_INTERNAL_H

/* Private declarations for the three DDS1 libc source owners. */
#include "sdk_libc.h"


extern void *memcpy(void *, const void *, size_t);

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

extern s32 func_00307250(_reent *r, const char *mode, s32 *oflags);

extern s32 fflush(void *fp);

extern long strtol(const char *s, char **end, s32 base);

extern s32 libcErrno __attribute__((section(".data")));

extern void __sinit(_reent *r);

extern void __smakebuf(FILE *fp);

extern void __malloc_lock(_reent *r);

extern void __malloc_unlock(_reent *r);

extern void *_malloc_r(_reent *r, u32 size);

extern void _free_r(_reent *r, void *ptr);

extern long _read_r(_reent *r, s32 fd, char *buf, s32 n);

extern long _write_r(_reent *r, s32 fd, const char *buf, s32 n);

extern long _lseek_r(_reent *r, s32 fd, long offset, s32 whence);

extern s32 __sfvwrite(FILE *fp, __suio *uio);

extern s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

extern void *memset(void *a, s32 b, size_t c);
extern s32 _vfprintf_r(_reent *r, FILE *fp, const char *fmt, va_list ap);

extern s32 func_00304D78(FILE *fp, const char *fmt, va_list ap);

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

extern mallocArenaPrefix_t __malloc_av_;



extern u32 strlen(const char *s);

extern const char _ctype_[];

extern char *_dtoa_r(_reent *r, f64 value, s32 mode, s32 digits,
                     s32 *decimalPoint, s32 *sign, char **end);



extern void *memmove(void *destination, const void *source, size_t size);
extern void *memchr(const void *source, s32 character, u32 size);
extern s32 __swsetup(FILE *fp);
extern s32 _mbtowc_r(_reent *r, s32 *out, const char *s, u32 limit, s32 *state);
extern s32 strcmp(const char *left, const char *right);

extern s32 _close_r(_reent *r, s32 fd);

#endif
