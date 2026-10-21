#ifndef SDK_LIBC_H
#define SDK_LIBC_H

/* SDK C library (libc). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#ifndef SDK_SKIP_memcmp
s32 memcmp(const void *a, const void *b, u32 size);
#endif
#ifndef SDK_SKIP_memcpy
void *memcpy(void *dst, const void *src, u32 size);
#endif
#ifndef SDK_SKIP_memmove
void *memmove(void *dst, const void *src, u32 size);
#endif
#ifndef SDK_SKIP_memset
void *memset(void *dst, s32 value, u32 size);
#endif
#ifndef SDK_SKIP_sdfVFormatString
s32 sdfVFormatString(char *dst, const char *format, void *args);
#endif
#ifndef SDK_SKIP_sprintf
s32 sprintf(char *dst, const char *format, ...);
#endif
#ifndef SDK_SKIP_sscanf
s32 sscanf(const char *text, const char *format, ...);
#endif
#ifndef SDK_SKIP_strcasecmp
s32 strcasecmp(const char *a, const char *b);
#endif
#ifndef SDK_SKIP_strcat
char *strcat(char *dst, const char *src);
#endif
#ifndef SDK_SKIP_strchr
char *strchr(const char *text, s32 c);
#endif
#ifndef SDK_SKIP_strcmp
s32 strcmp(const char *a, const char *b);
#endif
#ifndef SDK_SKIP_strcpy
char *strcpy(char *dst, const char *src);
#endif
#ifndef SDK_SKIP_strlen
u32 strlen(const char *text);
#endif
#ifndef SDK_SKIP_strncmp
s32 strncmp(const char *a, const char *b, u32 size);
#endif
#ifndef SDK_SKIP_strrchr
char *strrchr(const char *text, s32 c);
#endif
#ifndef SDK_SKIP_strstr
char *strstr(const char *text, const char *needle);
#endif

#ifdef VERSION_DDS2
#ifndef SDK_SKIP_puts
s32 puts(const char *text);
#endif
#ifndef SDK_SKIP_qsort
void qsort(void *base, u32 count, u32 size, s32 (*compare)(const void *, const void *));
#endif
#ifndef SDK_SKIP_rand
s32 rand(void);
#endif
#ifndef SDK_SKIP_strncpy
char *strncpy(char *dst, const char *src, u32 size);
#endif
#endif

#endif
