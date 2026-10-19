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

int sceMcGetDir(int, int, const char *, unsigned, int, sceMcTblGetDir *);

void mcChangeCurrentDirectory(u32 context, const char *path);
void mcMakeDirectory(u32 context, const char *path);
void mcReadDirectoryEntries(u32 context, const char *path, void *directoryEntries, s32 entryLimit);
void mcDeleteFilePath(u32 context, const char *path);
void mcOpenFilePath(u32 context, const char *path, s32 openFlags);

#endif
