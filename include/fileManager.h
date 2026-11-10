#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "libsdf.h"

typedef struct fileRequest {
    u8 kind;
    u8 state;
    u8 retry;
    u8 mode;
    struct fileRequest *next;
    char *name;
    void *dev;
    s32 rest;
    s32 size;
    void *callback;
    void *arg;
    sdfMemHandle_t *handle;
    u32 top;
    u32 ptr;
    struct fileRequest *donenext;
    sdfPacDecodeCb_t pac;
    u16 pacrest;
    u16 slot;
    struct fileRequest *cbnext;
} fileRequest_t;

fileRequest_t *fileLoad(const char *filename);

#endif
