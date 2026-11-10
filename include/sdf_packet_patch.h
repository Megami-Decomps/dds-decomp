#ifndef SDF_PACKET_PATCH_H
#define SDF_PACKET_PATCH_H

#include "common.h"

typedef struct sdfPreDrawHook_t sdfPreDrawHook_t;

/* A callback sdfGrDrawFrame runs before it sends the chain. */
struct sdfPreDrawHook_t
{
    sdfPreDrawHook_t *next;
    void *predrawFunc;
    u32 option0;
    u32 option1;
};

#endif /* SDF_PACKET_PATCH_H */
