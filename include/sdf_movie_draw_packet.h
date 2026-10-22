#ifndef SDF_MOVIE_DRAW_PACKET_H
#define SDF_MOVIE_DRAW_PACKET_H

#include "sdf_gs_packet.h"

/* DMA/VIF/GIF header followed by the nine A+D writes for one movie sprite. */
typedef struct SdfMovieDrawPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite sampling;
    SdfGsRegisterWrite texture;
    SdfGsRegisterWrite clamp;
    SdfGsRegisterWrite textureAlpha;
    SdfGsRegisterWrite color;
    SdfGsRegisterWrite sourceOrigin;
    SdfGsRegisterWrite vertexOrigin;
    SdfGsRegisterWrite sourceExtent;
    SdfGsRegisterWrite vertexExtent;
} SdfMovieDrawPacket;

#endif
