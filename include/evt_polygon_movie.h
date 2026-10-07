#ifndef EVT_POLYGON_MOVIE_H
#define EVT_POLYGON_MOVIE_H

#include "common.h"
#include "sdf.h"

/* PMD2 records and the complete 0x11C-byte owner cleared by both games'
 * evtPolygonMovieAllocWork. Movie parsing, asynchronous resource loading,
 * viewer dialogs and message-window lifetime all use this single object. */
typedef struct PmdEntry {
    u32 type;     /* 0x00 */
    u32 unk_04;   /* 0x04 */
    u32 value;    /* 0x08 */
    u32 offset;   /* 0x0C */
} PmdEntry;

typedef struct PmdHeader {
    u8 pad[0x10];
    s32 count;    /* 0x10 */
    s32 kind;     /* 0x14 */
    u8 pad2[8];
    PmdEntry entries[1]; /* 0x20 */
} PmdHeader;

/* Three retained resources share request/handle/address slots. */
typedef struct PolyMovieResourceSlot {
    void *request;
    SdfMemBlock *handle;
    PmdHeader *address;
} PolyMovieResourceSlot;

typedef struct PolyMovieWork {
    u32 flags;         /* 0x00 */
    PolyMovieResourceSlot mainResource; /* 0x04 */
    PmdHeader *data;   /* 0x10 */
    PmdEntry *entries; /* 0x14 */
    u8 *mainEntry1Data;   /* 0x18 */
    u32 unk_1C;        /* 0x1C */
    u8 *mainEntry2Data;   /* 0x20 */
    u32 unk_24;        /* 0x24 */
    u8 *mainEntry10Data;  /* 0x28 */
    u8 *mainEntry11Data;  /* 0x2C */
    u8 *mainEntry12Data;  /* 0x30 */
    u8 *mainEntry3Data;   /* 0x34 */
    u32 unk_38;        /* 0x38 */
    u8 *mainEntry9Data;   /* 0x3C */
    u8 *mainEntry7Data;   /* 0x40 */
    u32 unk_44;        /* 0x44 */
    u8 *mainEntry8Data;   /* 0x48 */
    u8 *mainEntry6Data;   /* 0x4C */
    u8 *mainEntry22Data;  /* 0x50 */
    u32 unk_54;        /* 0x54 */
    u8 *mainEntry23Data;  /* 0x58 */
    PolyMovieResourceSlot secondaryResource; /* 0x5C */
    PolyMovieResourceSlot tertiaryResource; /* 0x68 */
    PmdHeader *sub;    /* 0x74 */
    PmdEntry *subEntries; /* 0x78 */
    u8 *subEntry1Data;    /* 0x7C */
    u32 unk_80;        /* 0x80 */
    u8 *subEntry0Data;    /* 0x84 */
    u8 *subEntry4Kind4Data; /* 0x88 */
    u8 *subEntry4OtherData; /* 0x8C */
    PmdHeader *sub2;   /* 0x90 */
    PmdEntry *sub2Entries; /* 0x94 */
    u8 *secondEntry4Data; /* 0x98 */
    u32 unk_9C;        /* 0x9C */
    u32 unk_A0;        /* 0xA0 */
    u8 *subEntry5Data;    /* 0xA4 */
    u32 unk_A8;        /* 0xA8 */
    u8 *subEntry13Data;   /* 0xAC */
    u32 unk_B0;        /* 0xB0 */
    u8 *subEntry14Data;   /* 0xB4 */
    u32 unk_B8;        /* 0xB8 */
    u8 *subEntry15Data;   /* 0xBC */
    u32 unk_C0;        /* 0xC0 */
    u8 *subEntry16Data;   /* 0xC4 */
    u32 unk_C8;        /* 0xC8 */
    u8 *subEntry17Data;   /* 0xCC */
    u32 unk_D0;        /* 0xD0 */
    u8 *subEntry18Data;   /* 0xD4 */
    u32 unk_D8;        /* 0xD8 */
    u8 *subEntry19Data;   /* 0xDC */
    u32 unk_E0;        /* 0xE0 */
    u8 *subEntry20Data;   /* 0xE4 */
    u32 unk_E8;        /* 0xE8 */
    u8 *subEntry24Data;   /* 0xEC */
    u32 unk_F0;        /* 0xF0 */
    u8 *subEntry21Data;   /* 0xF4 */
    u32 unk_F8;        /* 0xF8 */
    u8 *subEntry25Data;   /* 0xFC */
    u32 unk_100;       /* 0x100 */
    s32 handle;        /* 0x104 */
    u32 unk108;        /* 0x108: scheduler result word stored by both constructors */
    s32 eventId;       /* 0x10C */
    s32 sceneId;       /* 0x110 */
    u32 unk_114;       /* 0x114 */
    s32 *buffer;       /* 0x118 */
} PolyMovieWork;

typedef char PolyMovieWork_size_must_be_0x11C[(sizeof(PolyMovieWork) == 0x11C) ? 1 : -1];
typedef char PolyMovieWork_handle_at_0x104[((u32)&((PolyMovieWork *)0)->handle == 0x104) ? 1 : -1];
typedef char PolyMovieWork_eventId_at_0x10C[((u32)&((PolyMovieWork *)0)->eventId == 0x10C) ? 1 : -1];

PolyMovieWork *evtPolygonMovieAllocWork(void);
PolyMovieWork *evtPolygonMovieInitWork(PolyMovieWork *, PmdHeader *, PmdHeader *, PmdHeader *);

#ifdef VERSION_DDS1
PolyMovieWork *func_00234DA8(u16 eventId, u16 sceneId, s32 mode);
void func_0023EF90(PolyMovieWork *work, void *viewer);
#elif VERSION_DDS2
PolyMovieWork *func_0024FB48(u16 eventId, u16 sceneId, s32 mode);
void func_0025A280(PolyMovieWork *work, void *viewer);
#endif

#endif
