#ifndef MC_PATH_API_H
#define MC_PATH_API_H

#include "common.h"

/* SDK 2.4 libmc.h directory record and API. */
typedef struct
{
	unsigned char Resv2,Sec,Min,Hour;
	unsigned char Day,Month;
	unsigned short Year;
} sceMcStDateTime;


typedef struct
{
	sceMcStDateTime _Create;
	sceMcStDateTime _Modify;
	unsigned FileSizeByte;
	unsigned short AttrFile;
	unsigned short Reserve1;
	unsigned Reserve2;
	unsigned PdaAplNo;
	unsigned char EntryName[32];
} sceMcTblGetDir __attribute__((aligned (64)));

/* The save flow reads existing files; new save files use O_RDWR | O_CREAT. */
typedef enum McOpenMode {
    MC_OPEN_MODE_READ_ONLY = 0x001,
    MC_OPEN_MODE_READ_WRITE_CREATE = 0x203
} McOpenMode;

int sceMcGetDir(int, int, const char *, unsigned, int, sceMcTblGetDir *);

void sdfMcFuncChdir(u32 context, const char *path);
void sdfMcFuncMkdir(u32 context, const char *path);
void sdfMcFuncGetDir(u32 context, const char *path, sceMcTblGetDir *directoryEntries, s32 entryLimit);
void sdfMcFuncDelete(u32 context, const char *path);
void sdfMcFuncOpen(u32 context, const char *path, McOpenMode openMode);

#endif
