#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "mnu_sprite_placement.h"
#include "sdf_chip.h"
#include "mnu_title_stream.h"
#include "mdl_flag_api.h"
#include "dds3_world_lifecycle.h"
#include "btl_stage_task_cleanup.h"
#include "sdf_motion.h"
#include "file.h"
#include "eff_resource_slots.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "mnu_work.h"
#include "sdf.h"
#include "mdl.h"
#include "itf.h"
#include "mnu_shooting.h"
#include "kwln_task_lifecycle.h"
#include "file_pac.h"
#include "file_request_api.h"
#include "pcp_vu0.h"
#include "eff_random.h"
#include "sdf_trig.h"
#include "btl.h"
#include "dds3_world_selection.h"
#include "evt_action_object.h"
struct dds3Basic_t;
/* Declared before first use (were implicit int). */
extern void fldProcNextJump(void);
extern void mnuClearInactiveSoundBufferState(void);
extern void mnuLoadTitleSoundBufferEntry(s32 soundEntryIndex);





/* The record payload follows this complete 0x10-byte descriptor. */
typedef struct MnuSectionObjectList {
    sdfMemHandle_t *allocation;
    s32 count;
    u8 *objects;
    u8 pad0C[4];
} MnuSectionObjectList;




extern MenuWorkEntry D_0040ABF8;
extern void mnuDeactivateWorkEntry(MenuWorkEntry *);
extern void mnuDestroyAllModelNodeContexts(MnuNodeList *);
extern f32 mnuEvaluateTimedValue(MenuWorkEntry *);
extern void itfProjectScreenPointToFixedDepth(f32 *, s32, s32);
extern void mnuUpdateTimedEffectPosition(void);

typedef struct SoundSlot SoundSlot;
extern SoundSlot *dds3ClaimSoundSlot(u32, u32);
extern s32 D_00438934;
void mnuClaimTimedEntryEffectRecord(void);

extern MnuEffectRecord *D_00438930;

extern MnuShootingWork *D_0043891C;

extern s32 mnuRunShootingInitCallback(MnuShootingWork *);
extern u32 mnuRunShootingUpdateCallback(MnuShootingWork *);
extern void mnuReleaseShootingSectionResources(MnuShootingWork *);
extern void mnuDestroyShootingWork(MnuShootingWork *);

extern u32 D_00438918;

extern s32 D_00435BB0;

extern s16 dds3VblanksPerFrame;

extern s32 (*D_0040ABF0[])(u8 *work);
extern s32 (*D_0040ABE0[])(u8 *work);

extern void mdlPackDecode(s32 source, s32 destination, s32 flags, s32 packageId, s32 variant);



extern u32 mnuResumeEffectQueueFrameAdvance(void);

extern void mnuInitializeShootingScene(MnuShootingWork *handle);


extern void dds3ReleaseSoundSlotPool(void);
extern sdfMemHandle_t *D_00438948;
extern void mnuDestroyNodeJobQueues(MnuEffectList *);
extern void mnuUpdateHighScoreFlag(MnuShootingWork *);
/* Retail passes the task work to this otherwise empty legacy callback. */
extern void mnuCleanupShootingPauseNoOp();
extern void itfClearTintAndWorkBuffers(u8 *);
extern u32 D_00435CBC;
#include "dds3_world_secondary.h"
s32 mdlAdvanceViewerPackageTask(dds3ProcessID_t *task);

MnuShootingWork *mdlAllocateViewerPackageWork(void);


extern char D_0042D4D0[];


void mdlDestroyOptionalViewerPackageWork(MnuShootingWork *work);

void itfSetPackedRgbAlpha(SdfFlagListWork *entry, u32 color) {
    entry->params.color.initialColor = color & 0xFFFFFF;
    entry->params.alpha.alpha = color >> 24;
}

/* Return the surface-table index consumed by sdfDrawFlagListVertices. */
u32 sdfGetFlagListAlphaSurfaceIndex(SdfFlagListWork *entry) {
    return entry->params.alpha.surfaceIndex;
}

/* Replace the flag list's draw-surface index without validation. */
void sdfSetFlagListAlphaSurfaceIndex(SdfFlagListWork *entry, u32 surfaceIndex) {
    entry->params.alpha.surfaceIndex = surfaceIndex;
}

void itfCopyColorFields(SdfFlagListWork *dst, const SdfFlagListParams *src) {
    dst->params.color.initialColor = src->color.initialColor;
    dst->params.speed = src->speed;
    dst->params.alpha.alpha = src->alpha.alpha;
    dst->params.alpha.surfaceIndex = src->alpha.surfaceIndex;
}

/* Store generated vertices relative to the view target and add it back at draw time. */
void sdfEnableFlagListViewTargetRelativeMode(void) {
    D_00438918 = 1;
}

/* Keep generated vertices in world coordinates instead of target-relative storage. */
void sdfDisableFlagListViewTargetRelativeMode(void) {
    D_00438918 = 0;
}

void mdlCreateViewerPackageTask(void) {
    MnuShootingWork *handle;

    D_00435BB0 = 0;
    dds3VblanksPerFrame = 1;
    handle = mdlAllocateViewerPackageWork();
    D_0043891C = handle;
    mnuInitializeShootingScene(handle);
    dds3StartProcess(D_0042D4D0, 0x2AF8, 0, 0, mdlAdvanceViewerPackageTask, 0, 0);
}

u8 mdlViewerPackageTaskExists(void) {
    return dds3SearchProcessName(D_0042D4D0) != 0;
}

u32 mdlStopViewerPackageTask(void) {
    dds3VblanksPerFrame = 2;
    D_00435BB0 = 1;
    mdlDestroyOptionalViewerPackageWork(D_0043891C);
    dds3KillProcessName(D_0042D4D0, 1);
    return mnuResumeEffectQueueFrameAdvance();
}

/* Return cleared 0x1E0-byte task work with its allocation handle and defaults. */
MnuShootingWork *mdlAllocateViewerPackageWork(void) {
    sdfMemHandle_t *block = sdfMemAllocDirect(0x1E0);
    MnuShootingWork *work = (MnuShootingWork *)sdfMemGetPtr(block);

    memset(work, 0, 0x1E0);
    work->allocation = block;
    work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_INITIALIZE;
    work->unk1D8 = 0x80;
    work->round = 0;
    work->currentScore = 0;
    return work;
}

void mdlDestroyOptionalViewerPackageWork(MnuShootingWork *work) {
    if (work != NULL) {
        mnuDestroyShootingWork(work);
    }
}

s32 mdlAdvanceViewerPackageTask(dds3ProcessID_t *task) {
    u32 result;
    s32 status;

    status = mnuRunShootingInitCallback(D_0043891C);
    if (status == -1) {
        fldProcNextJump();
        result = 0xffffffff;
    }
    else {
        mnuRunShootingUpdateCallback(D_0043891C);
        result = 0;
    }
    return result;
}

extern char D_0042D4E0[];
extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory);
extern void scrDestroyAllNamedProcesses(void);

s32 evtCallShooting(void) {
    evtPrintDeveloperConsoleMessage(D_0042D4E0, 0);
    dds3AdminiCallSequence(0x1D, 0, 0, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

/* The writable loader table has 13 complete 0x2C-byte descriptors. */
typedef struct MnuPackageDescriptor {
    char name[0x24];
    s8 kind;
    u8 pad25[3];
    s32 value;
} MnuPackageDescriptor;

typedef char MnuPackageDescriptor_size_is_0x2C[
    (sizeof(MnuPackageDescriptor) == 0x2C) ? 1 : -1];

extern MnuPackageDescriptor D_0040A970[13];
extern char D_00438920[];
INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D4D0);

INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D4E0);

const char D_0042D500[] __attribute__((aligned(8))) = "fileload ...OK!!\n";
const char D_0042D518[] __attribute__((aligned(8))) = "SMG already Read[%s]\n";
const char D_0042D530[] __attribute__((aligned(8))) = "pac load check ok ...\n";
const char D_0042D548[] __attribute__((aligned(8))) = "FileSize %d\n";
const char D_0042D558[] __attribute__((aligned(8))) = "ObjectList Create ... %s \n";
const char D_0042D578[] __attribute__((aligned(8))) = "... OK!! \n";
const char D_0042D588[] __attribute__((aligned(8))) = "PlayerObjectList Create ... %s \n";
const char D_0042D5B0[] __attribute__((aligned(8))) = "MapObjectList Create ... %s \n";
const char D_0042D5D0[] __attribute__((aligned(8))) = "DrawbjectList Create ... %s \n";
const char D_0042D5F0[] __attribute__((aligned(8))) = "load SPR File %s \n";
const char D_0042D608[] __attribute__((aligned(8))) = "load SE %s \n";
const char D_0042D618[] __attribute__((aligned(8))) = "DrawbjectList Create ...    [%s:%d] \n";
const char D_0042D640[] __attribute__((aligned(8))) = "PlayerObjectList Create ... [%s:%d] \n";
const char D_0042D668[] __attribute__((aligned(8))) = "ObjectList Create ...       [%s:%d] \n";
const char D_0042D690[] __attribute__((aligned(8))) = "MapObjectList Create ...    [%s:%d] \n";
const char D_0042D6B8[] __attribute__((aligned(8))) = "%s ... OK!! \n";
const char D_0042D6C8[] __attribute__((aligned(8))) = "FileHandle Free... OK!! \n";

/* The native wrapper tail-forwards the request through its legacy ABI. */
extern s32 sndFindPackedTrackLoadStatus(s32 sound);
extern void p2sdLoadSMGFile(s32 sound);
extern void effRequestResourceByMode(const char *prefix, const char *name,
                                     s32 mode, u32 *outInstance);
#include "sdf_debug_api.h"
extern MnuSectionObjectList *mnuLoadSectionObjectListWithRelocations(const u8 *data);
void mdlLoadViewerPackageFromWork(PacWork *entry);

/* Advance the shooting package loader; only the completed state returns one. */
s32 mnuAdvanceShootingPackageLoad(u8 *workBytes) {
    MnuShootingWork *work = (MnuShootingWork *)workBytes;
    void *data = NULL;

    switch (work->packageLoadState) {
    case MNU_SHOOTING_PACKAGE_LOAD_INITIALIZE:
        work->packageEntryIndex = 0;
        work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_REQUEST_ENTRY;
        break;

    case MNU_SHOOTING_PACKAGE_LOAD_FINISHED:
        work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_INITIALIZE;
        work->state = 1;
        evtPrintDeveloperConsoleMessage(D_0042D500);
        return 1;

    case MNU_SHOOTING_PACKAGE_LOAD_REQUEST_ENTRY:
        switch (D_0040A970[work->packageEntryIndex].kind) {
        case 0:
            if (work->round == D_0040A970[work->packageEntryIndex].value) {
                work->packageRequest = fileLoad(
                    D_0040A970[work->packageEntryIndex].name);
            } else {
                work->packageEntryIndex++;
                if (work->packageEntryIndex < 13) {
                    work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_REQUEST_ENTRY;
                } else {
                    work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_FINISHED;
                }
                return 0;
            }
            break;
        case 1:
            if (work->resourceSlots[D_0040A970[work->packageEntryIndex].value] == 0) {
                effRequestResourceByMode(D_00438920,
                    D_0040A970[work->packageEntryIndex].name, 0,
                    (u32 *)&work->resourceSlots[D_0040A970[work->packageEntryIndex].value]);
            } else {
                work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_PROCESS_ENTRY;
            }
            break;
        case 2: {
            s32 status = sndFindPackedTrackLoadStatus(
                D_0040A970[work->packageEntryIndex].value);
            if (status == 0) {
                p2sdLoadSMGFile(D_0040A970[work->packageEntryIndex].value);
            } else {
                work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_PROCESS_ENTRY;
                evtPrintDeveloperConsoleMessage(D_0042D518,
                    D_0040A970[work->packageEntryIndex].name);
            }
            break;
        }
        case 5:
            if (work->queueCopies[0] == NULL) {
                work->packageDataRequest = pacLoad(
                    D_0040A970[work->packageEntryIndex].name);
            } else {
                work->packageEntryIndex++;
                if (work->packageEntryIndex < 13) {
                    work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_REQUEST_ENTRY;
                } else {
                    work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_FINISHED;
                }
                return 0;
            }
            break;
        case 3:
        case 4:
            if (work->round == D_0040A970[work->packageEntryIndex].value) {
                work->packageDataRequest = pacLoad(
                    D_0040A970[work->packageEntryIndex].name);
            } else {
                work->packageEntryIndex++;
                if (work->packageEntryIndex < 13) {
                    work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_REQUEST_ENTRY;
                } else {
                    work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_FINISHED;
                }
                return 0;
            }
            break;
        }
        work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_WAIT_ENTRY;
        break;

    case MNU_SHOOTING_PACKAGE_LOAD_WAIT_ENTRY:
        switch (D_0040A970[work->packageEntryIndex].kind) {
        case 0:
            if (pacFileCheckEnd(work->packageRequest) != 0) {
                work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_PROCESS_ENTRY;
            }
            break;
        case 1:
            if (work->resourceSlots[D_0040A970[work->packageEntryIndex].value] != 0) {
                work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_PROCESS_ENTRY;
            }
            break;
        case 2:
            if (sndFindPackedTrackLoadStatus(D_0040A970[work->packageEntryIndex].value) == 1) {
                work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_PROCESS_ENTRY;
            }
            break;
        case 3:
        case 4:
        case 5:
            if (pacCheckEnd(work->packageDataRequest) != 0) {
                evtPrintDeveloperConsoleMessage(D_0042D530);
                work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_PROCESS_ENTRY;
            }
            break;
        }
        break;

    case MNU_SHOOTING_PACKAGE_LOAD_PROCESS_ENTRY: {
        struct FileRequest *request = work->packageRequest;

        if (request != NULL) {
            data = (void *)fileGetLoadPtr(request);
            request = work->packageRequest;
            evtPrintDeveloperConsoleMessage(D_0042D548,
                                            fileGetLoadSize(request));
        }

        switch (D_0040A970[work->packageEntryIndex].kind) {
        case 0:
            switch (work->packageEntryIndex) {
            case 6:
            case 10:
            case 14:
                evtPrintDeveloperConsoleMessage(D_0042D558, D_0040A970[work->packageEntryIndex].name);
                work->objects = mnuLoadSectionObjectListWithRelocations(data);
                printf(D_0042D578);
                break;
            case 7:
            case 11:
            case 15:
                printf(D_0042D588, D_0040A970[work->packageEntryIndex].name);
                work->playerObjects = mnuLoadSectionObjectListWithRelocations(data);
                printf(D_0042D578);
                break;
            case 8:
            case 12:
            case 16:
                printf(D_0042D5B0, D_0040A970[work->packageEntryIndex].name);
                work->mapObjects = mnuLoadSectionObjectListWithRelocations(data);
                printf(D_0042D578);
                break;
            case 9:
            case 13:
            case 17:
                printf(D_0042D5D0, D_0040A970[work->packageEntryIndex].name);
                work->drawObjects = mnuLoadSectionObjectListWithRelocations(data);
                printf(D_0042D578);
                break;
            default:
                break;
            }
            break;

        case 1:
            printf(D_0042D5F0, D_0040A970[work->packageEntryIndex].name);
            break;

        case 2:
            printf(D_0042D608, D_0040A970[work->packageEntryIndex].name);
            break;

        case 5: {
            struct FilePacRequest *pacRequest =
                (struct FilePacRequest *)work->packageDataRequest;
            PacWork *entry = pacRequest->packet.queueHead;
            if (entry != NULL) {
                struct FileQueue **queueSlot = work->queueCopies;

                do {
                    if (entry->packet[0] == 1) {
                        struct FileQueue *clone =
                            fileCloneQueueEntries((struct FileQueue *)entry->dataCursor);
                        *queueSlot++ = clone;
                        fileQueueNotifyAllJobsComplete(clone);
                        sdfMemFreeDirect(PAC_WORK_ALLOCATION(entry));
                    }
                    entry = entry->next;
                } while (entry != NULL);
            }
            pacFileHandleFree_(work->packageDataRequest);
            work->packageDataRequest = NULL;
            break;
        }

        case 4: {
            struct FilePacRequest *pacRequest =
                (struct FilePacRequest *)work->packageDataRequest;
            PacWork *entry = pacRequest->packet.queueHead;
            s32 entryIndex = 0;

            while (entry != NULL) {
                if (entry->packet[0] == 1) {
                    switch (entryIndex) {
                    case 0:
                        evtPrintDeveloperConsoleMessage(D_0042D618, D_0040A970[work->packageEntryIndex].name, 0);
                        work->drawObjects = mnuLoadSectionObjectListWithRelocations(entry->dataCursor);
                        sdfMemFreeDirect(PAC_WORK_ALLOCATION(entry));
                        evtPrintDeveloperConsoleMessage(D_0042D578);
                        break;
                    case 1:
                        evtPrintDeveloperConsoleMessage(D_0042D640, D_0040A970[work->packageEntryIndex].name, 1);
                        work->playerObjects = mnuLoadSectionObjectListWithRelocations(entry->dataCursor);
                        sdfMemFreeDirect(PAC_WORK_ALLOCATION(entry));
                        evtPrintDeveloperConsoleMessage(D_0042D578);
                        break;
                    case 2:
                        evtPrintDeveloperConsoleMessage(D_0042D668, D_0040A970[work->packageEntryIndex].name, 2);
                        work->objects = mnuLoadSectionObjectListWithRelocations(entry->dataCursor);
                        sdfMemFreeDirect(PAC_WORK_ALLOCATION(entry));
                        evtPrintDeveloperConsoleMessage(D_0042D578);
                        break;
                    case 3:
                        evtPrintDeveloperConsoleMessage(D_0042D690, D_0040A970[work->packageEntryIndex].name, 3);
                        work->mapObjects = mnuLoadSectionObjectListWithRelocations(entry->dataCursor);
                        sdfMemFreeDirect(PAC_WORK_ALLOCATION(entry));
                        evtPrintDeveloperConsoleMessage(D_0042D578);
                        break;
                    default:
                        break;
                    }
                }
                entry = entry->next;
                entryIndex++;
            }
            pacFileHandleFree_(work->packageDataRequest);
            work->packageDataRequest = NULL;
            break;
        }

        case 3: {
            struct FilePacRequest *pacRequest =
                (struct FilePacRequest *)work->packageDataRequest;
            PacWork *entry = pacRequest->packet.queueHead;

            while (entry != NULL) {
                if (entry->packet[0] == 1) {
                    mdlLoadViewerPackageFromWork(entry);
                    sdfMemFreeDirect(PAC_WORK_ALLOCATION(entry));
                }
                entry = entry->next;
            }
            pacFileHandleFree_(work->packageDataRequest);
            work->packageDataRequest = NULL;
            break;
        }

        default:
            break;
        }

        evtPrintDeveloperConsoleMessage(D_0042D6B8, D_0040A970[work->packageEntryIndex].name);

        if (D_0040A970[work->packageEntryIndex].kind == 0) {
            sdfMemFreeDirect(
                fileGetMemHandle(work->packageRequest));
            fileRequestFreeIfComplete(work->packageRequest);
            evtPrintDeveloperConsoleMessage(D_0042D6C8);
        }

        work->packageEntryIndex++;
        if (work->packageEntryIndex < 13) {
            work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_REQUEST_ENTRY;
        } else {
            work->packageLoadState = MNU_SHOOTING_PACKAGE_LOAD_FINISHED;
        }
        break;
    }
    }
    return 0;
}


/* Read the inline packet header, then forward its request through the legacy ABI. */
void mdlLoadViewerPackageFromWork(PacWork *entry) {
    u32 commandWord;

    memcpy(&commandWord, entry->packet, sizeof(commandWord));
    mdlPackDecode(5, commandWord >> 16, 0x101, (s32)entry->dataCursor,
                         ((const SdfPacStreamPacketHeader *)entry->packet)->decodedSize);
}

extern void dds3ApplyRelocationOffsets(u8 *base, u32 adjustment, u32 *offsets, u32 size);

/* Copy a section's object payload into a fresh block and relocate it in place.
 * The data starts with a 16-byte header and five information words (payload
 * size, object count, an unused word, relocation size and an unused word). */
MnuSectionObjectList *mnuLoadSectionObjectListWithRelocations(const u8 *data) {
    u8 header[16];
    u32 info[5];
    u32 offset;
    const u8 *payload;
    sdfMemHandle_t *block;
    MnuSectionObjectList *list;
    u8 *objects;
    sdfMemHandle_t *relocBlock;
    u32 *offsets;

    memcpy(header, data, sizeof(header));
    offset = sizeof(header);
    memcpy(info, data + offset, sizeof(info));
    offset += sizeof(info);
    payload = data + offset;
    block = sdfMemAllocDirect(info[0] + 0x10);
    list = (MnuSectionObjectList *)sdfMemGetPtr(block);
    objects = (u8 *)(list + 1);
    list->allocation = block;
    list->count = info[1];
    list->objects = objects;
    memset(objects, 0, info[0]);
    offset = sizeof(*list);
    memcpy((u8 *)list + offset, payload, info[0]);
    payload += info[0];
    relocBlock = sdfMemAllocDirect(info[3]);
    offsets = (u32 *)sdfMemGetPtr(relocBlock);
    memset(offsets, 0, info[3]);
    memcpy(offsets, payload, info[3]);
    dds3ApplyRelocationOffsets(objects, (u32)objects, offsets, info[3]);
    sdfMemFreeDirect(relocBlock);
    return list;
}

/* State-zero update entry in the shooting-scene callback table; no work in retail. */
u32 mnuShootingState0UpdateNoOp(void) {
    return 0;
}

typedef struct ShootingVector {
    f32 v[4];
} ShootingVector;

extern const ShootingVector D_0042D7A0;
extern struct dds3Basic_t *dds3AddScene(void);
extern void AddCamera(void);
extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, f32 *color);
extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, f32 *direction);
extern void dds3SetAmbientColor(s32, f32 *);
extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);
extern void dds3InitSoundSlotPool(void);
extern void mnuRegisterWorkEntryPool(MenuWorkEntry *entries, u32 count);
extern void mnuInitializeRuntimeHitTargetList(MenuRuntimeRecord *records, s32 capacity);
extern void mnuInitializeRuntimeAnimationList(MenuRuntimeRecord *records, s32 capacity);
extern void mnuResetProgressLimitAndStep(u8 cadenceLimit, u8 progressStep);
extern void mnuSetWorkEntryFinishOrDeactivateCallback(MenuWorkCallback callback);
extern void mnuSetRuntimeRecordPairCallback(MenuRuntimePairCallback callback);
extern void mnuInitializeHighScoreState(MnuShootingWork *work);
extern MenuWorkEntry *D_0043894C;
extern MenuProgressParameters D_0040AC40;
extern s32 D_004390B4;

/* Build the shooting scene: world node, camera and area actor, the three
 * lights and background, sprite/hit pools, the work-entry pool and the two
 * runtime record lists, then the progress parameters and callbacks. */
INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D7A0);

void mnuInitializeShootingScene(MnuShootingWork *handle) {
    MnuSectionObjectList *lists[2];
    f32 direction[4] = { 0 };
    ShootingVector color;
    struct sdfMemHandle_t *block;
    struct dds3Basic_t *node;

    direction[3] = 1.0f;
    color = D_0042D7A0;
    D_00435CBC = 0x80000000;
    node = dds3AddScene();
    dds3SetControlScene(node);
    dds3SetActiveScene(node);
    AddCamera();
    dds3AddAreaBasic(0, "shooting_Area");
    color.v[0] = 1.0f;
    color.v[1] = 1.0f;
    color.v[2] = 1.0f;
    dds3SetLightColor(0, 0, color.v);
    color.v[0] = 0.0f;
    color.v[1] = 0.0f;
    color.v[2] = 0.0f;
    dds3SetLightColor(0, 1, color.v);
    color.v[0] = 0.0f;
    color.v[1] = 0.0f;
    color.v[2] = 0.0f;
    dds3SetLightColor(0, 2, color.v);
    direction[0] = 1.0f;
    direction[1] = 1.0f;
    direction[2] = 1.0f;
    dds3SetLightDir(0, 0, direction);
    direction[0] = 0.0f;
    direction[1] = 0.0f;
    direction[2] = 0.0f;
    dds3SetLightDir(0, 1, direction);
    direction[0] = 0.0f;
    direction[1] = 0.0f;
    direction[2] = 0.0f;
    dds3SetLightDir(0, 2, direction);
    color.v[0] = 0.4f;
    color.v[1] = 0.4f;
    color.v[2] = 0.4f;
    dds3SetAmbientColor(0, color.v);
    dds3SetFogPara(0, 255.0f, 255.0f, 3000.0f, 20000.0f);
    handle->spriteNumberPool = itfCreateSpriteWorkPool(100);
    handle->hitEffectPool = itfCreateSpriteHitEffectWorkPool(100);
    dds3InitSoundSlotPool();
    D_00438948 = sdfMemAllocDirect(0x1C20);
    D_0043894C = (MenuWorkEntry *)sdfMemGetPtr(D_00438948);
    memset(D_0043894C, 0, 0x1C20);
    mnuRegisterWorkEntryPool(D_0043894C, 100);
    block = sdfMemAllocDirect(0x1C30);
    lists[0] = (MnuSectionObjectList *)sdfMemGetPtr(block);
    memset(lists[0], 0, 0x1C30);
    lists[0]->allocation = block;
    lists[0]->objects = (u8 *)(lists[0] + 1);
    lists[0]->count = 200;
    block = sdfMemAllocDirect(0xE20);
    lists[1] = (MnuSectionObjectList *)sdfMemGetPtr(block);
    memset(lists[1], 0, 0xE20);
    lists[1]->count = 100;
    lists[1]->objects = (u8 *)(lists[1] + 1);
    lists[1]->allocation = block;
    mnuInitializeRuntimeHitTargetList((MenuRuntimeRecord *)lists[0]->objects, lists[0]->count);
    mnuInitializeRuntimeAnimationList((MenuRuntimeRecord *)lists[1]->objects, lists[1]->count);
    handle->progressWork = lists[0];
    handle->alternateProgressWork = lists[1];
    memset(&D_0040AC40, 0, sizeof(D_0040AC40));
    D_0040AC40.width = 0x140;
    D_0040AC40.height = 0x1C0;
    D_0040AC40.x = 0x62;
    D_0040AC40.y = 0;
    mnuCopyResourceProgressParameters(&D_0040AC40);
    mnuResetProgressLimitAndStep(1, 1);
    mnuSetWorkEntryStartCallback(mnuCreateShootingEntryObject);
    mnuSetWorkEntryFinishOrDeactivateCallback(mnuFinishOrDeactivateShootingEntry);
    mnuSetActiveWorkVisitor(mnuPlaceShootingEntryObject);
    mnuSetRuntimeRecordInitializationCallback(mnuApplyPlayerRuntimeRecordFeedback);
    mnuSetRuntimeWorkHitCallback(mnuApplyShootingRuntimeRecordEvent);
    mnuSetRuntimeRecordPairCallback(mnuAdvanceShootingProgressFromRecord);
    mnuInitializeHighScoreState(handle);
    handle->initialize = D_0040ABE0[0];
    handle->update = D_0040ABF0[0];
    D_004390B4 = 0;
    handle->initialized = 0;
    handle->frameDisplayValue = 2;
}

/* Release the shooting task's owned lists, queues, slots and world object. */
void mnuDestroyShootingWork(MnuShootingWork *work) {
    s32 i;
    struct EffectSlotSet **slot;

    itfReleaseSpriteWorkPool(work->spriteNumberPool);
    itfReleaseSpriteHitEffectWorkPool(work->hitEffectPool);
    dds3ReleaseSoundSlotPool();
    if (D_00438948 != NULL) {
        sdfMemFreeDirect(D_00438948);
        D_00438948 = NULL;
    }
    if (work->progressWork != NULL) {
        sdfMemFreeDirect(work->progressWork->allocation);
        work->progressWork = NULL;
    }
    if (work->alternateProgressWork != NULL) {
        sdfMemFreeDirect(work->alternateProgressWork->allocation);
        work->alternateProgressWork = NULL;
    }
    if (work->effectWork != NULL) {
        for (i = 0; i < work->effectWork->count; i++) {
            mnuDestroyNodeJobQueues(&work->effectWork->lists[i]);
        }
        sdfMemFreeDirect(work->effectWork->allocation);
        work->effectWork = NULL;
    }
    if (work->work24 != NULL) {
        sdfMemFreeDirect(work->work24->allocation);
        work->work24 = NULL;
    }
    slot = work->resourceSlots;
    mnuUpdateHighScoreFlag(work);
    mnuReleaseShootingSectionResources(work);
    mnuCleanupShootingPauseNoOp(work);
    itfClearTintAndWorkBuffers((u8 *)work);
    for (i = 6; i >= 0; i--, slot++) {
        if (*slot != NULL) {
            effDestroyResourceSlotSet(*slot);
            *slot = NULL;
        }
    }
    D_00435CBC = 0x80000000;
    dds3DeleteScene(dds3GetControlScene());
    if (work->round >= 3) {
        evtBitOn(0x849);
    } else {
        evtBitOff(0x849);
    }
    sdfMemFreeDirect(work->allocation);
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
}

/* Advance initialization; completing state zero releases the section resources. */
s32 mnuRunShootingInitCallback(MnuShootingWork *work) {
    s32 state;

    if (work->initialize((u8 *)work) != 0) {
        state = work->state;
        if (state == 0) {
            mnuReleaseShootingSectionResources(work);
            state = work->state;
            work->initialized = 1;
            work->update = D_0040ABF0[0];
        }
        if (state == -1) {
            return -1;
        }
        work->initialize = D_0040ABE0[state];
    }
    return 1;
}

/* Run the current update; on a nonzero result, refresh it from the state table.
   The dispatch table keeps its original opaque byte-work callback signature. */
u32 mnuRunShootingUpdateCallback(MnuShootingWork *work) {
    s32 (*update)(u8 *work) = work->update;

    if (update((u8 *)work) != 0) {
        work->update = D_0040ABF0[work->state];
    }
    return 1;
}

extern void mnuResetResourceProgressCounters(void);
extern void mnuResetWorkEntryPool(void);
extern void mnuInitializeActiveEffectWorkEntry(MenuWorkEntry *entry);
extern s32 mnuRebuildMenuWorkEntryCallbackRecords(MenuWorkEntry *entry, u32 mode);
extern MenuRuntimeList *mnuGetRuntimeHitTargetList(void);
extern MenuRuntimeList *mnuGetRuntimeAnimationList(void);
extern void mnuClearRuntimeRecordStorage(MenuRuntimeList *);
extern void mnuBindMenuRecordRegistry(MenuRegistry *, u32);
extern void mnuBindMovementRecordTable(MenuMovementRecord18 *records, u32 count);
extern void mnuBindResourceRecordTable(MenuResourceRecord *, s32);
extern void mnuBindMenuRegistryParameters(MenuRegistryParameters *records, u32 count);
extern MnuSectionModelWork *mnuCreateSectionModelWork(s32 listCount, s32 *nodeCounts);
extern void mnuInitializeNodeTransforms(MnuNodeList *group, f32 x, f32 y, f32 z, f32 w);
extern void mnuPopulateModelNodeList(MnuNodeList *list, s32 resourceGroup, s32 resourceId, s32 entryFlags, f32 x, f32 y, f32 z);
extern void mnuDeactivateAllModelNodes(MnuNodeList *list);
extern MnuEffectWork *mnuCreateEffectWork(s32 listCount, s32 *recordCounts);
extern void mnuClearNodeRecords(MnuEffectList *list);
extern void mnuAssignEffectRecordQueues(MnuEffectList *list, s32 resourceIndex, FileQueue **sources);
extern void mnuClearAllNodeBroadcastFlags(MnuEffectList *list);
extern void itfInstallIndexedImageResourceBanks(u32 *source, s32 count);
extern ModelInstanceWork *itfCreateModelInstanceWork(s32 listCount, s32 *instanceCounts);
extern void itfClearModelInstances(ModelInstanceList *list);
extern void itfSetModelInstanceParameterBits(ModelInstanceList *list, u32 parameter);
extern void itfDeactivateModelInstances(ModelInstanceList *list);
extern void itfApplyWorkTintAndClearBuffers(u8 *work);
extern void mnuMarkTitleStreamResetPending(void);
extern void mnuResetTitleStreamLocked(void);
extern void mnuQueueTitleSoundStreamEntry(s32 soundEntryIndex);
extern void mnuResetTitleStreamAfterFileIdle(void);
extern MnuModelNode *D_00438928;

/* Initialize a shooting section: reset the entry pools, bind the loaded
 * object tables, then build the section models, effect lists and model
 * instances (or reuse the instances), and reset the round state. */
s32 mnuInitializeShootingWorkResources(MnuShootingWork *work) {
    MenuRegistryParameters *records;
    s32 *counts;
    MnuNodeList *groups;
    MnuEffectList *effects;
    ModelInstanceList *instances;
    s32 i;

    mnuResetResourceProgressCounters();
    mnuResetWorkEntryPool();
    mnuInitializeActiveEffectWorkEntry(&D_0040ABF8);
    if (work->savedPlayerMode != 0) {
        mnuRebuildMenuWorkEntryCallbackRecords(&D_0040ABF8, work->savedPlayerMode);
    }
    D_0040ABF8.currentX = 256.0f;
    D_0040ABF8.currentY = 320.0f;
    mnuClearRuntimeRecordStorage(mnuGetRuntimeHitTargetList());
    mnuClearRuntimeRecordStorage(mnuGetRuntimeAnimationList());
    mnuBindMenuRecordRegistry((MenuRegistry *)work->objects->objects, work->objects->count);
    mnuBindMovementRecordTable((MenuMovementRecord18 *)work->playerObjects->objects, work->playerObjects->count);
    mnuBindResourceRecordTable((MenuResourceRecord *)work->mapObjects->objects, work->mapObjects->count);
    mnuBindMenuRegistryParameters((MenuRegistryParameters *)work->drawObjects->objects, work->drawObjects->count);
    counts = sdfChipAllocDirect(work->drawObjects->count * 4);
    records = (MenuRegistryParameters *)work->drawObjects->objects;
    for (i = 0; i < work->drawObjects->count; i++) {
        if (records[i].kind == 3) {
            counts[i] = records[i].instanceCount;
        } else {
            counts[i] = 0;
        }
    }
    work->modelWork = mnuCreateSectionModelWork(work->drawObjects->count, counts);
    sdfChipFreeDirect(counts);
    D_00438928 = NULL;
    groups = work->modelWork->groups;
    records = (MenuRegistryParameters *)work->drawObjects->objects;
    for (i = 0; i < work->modelWork->count; i++) {
        mnuInitializeNodeTransforms(&groups[i], 1.0f, 1.0f, 1.0f, 1.0f);
        if (i == 8) {
            mnuPopulateModelNodeList(&groups[i], records[i].major, records[i].minor, 0, 1.0f, 1.0f, 3.0f);
        } else {
            mnuPopulateModelNodeList(&groups[i], 5, records[i].minor, 0, 1.0f, 1.0f, 1.0f);
        }
        mnuDeactivateAllModelNodes(&groups[i]);
        evtPrintDeveloperConsoleMessage("Major:0x%x Minor:0x%x Num:%d\n", records[i].major, records[i].minor, records[i].instanceCount);
    }
    if (work->effectWork == NULL) {
        counts = sdfChipAllocDirect(12);
        counts[1] = 2;
        counts[2] = 2;
        counts[0] = 30;
        work->effectWork = mnuCreateEffectWork(3, counts);
        sdfChipFreeDirect(counts);
        effects = work->effectWork->lists;
        for (i = 0; i < work->effectWork->count; i++) {
            MnuEffectList *list = &effects[i];

            mnuClearNodeRecords(list);
            mnuAssignEffectRecordQueues(list, i, work->queueCopies);
            mnuClearAllNodeBroadcastFlags(list);
            evtPrintDeveloperConsoleMessage("Effect Load!!\n");
        }
    }
    if (work->work24 == NULL) {
        itfInstallIndexedImageResourceBanks((u32 *)work->resourceSlots, 7);
        counts = sdfChipAllocDirect(work->drawObjects->count * 4);
        records = (MenuRegistryParameters *)work->drawObjects->objects;
        for (i = 0; i < work->drawObjects->count; i++) {
            if (records[i].kind == 1) {
                counts[i] = records[i].instanceCount;
            } else {
                counts[i] = 0;
            }
        }
        work->work24 = itfCreateModelInstanceWork(work->drawObjects->count, counts);
        sdfChipFreeDirect(counts);
        instances = work->work24->lists;
        for (i = 0; i < work->work24->count; i++) {
            itfClearModelInstances(&instances[i]);
            itfSetModelInstanceParameterBits(&instances[i], i);
        }
    } else {
        instances = work->work24->lists;
        for (i = 0; i < work->work24->count; i++) {
            itfDeactivateModelInstances(&instances[i]);
        }
    }
    itfApplyWorkTintAndClearBuffers((u8 *)work);
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    mnuQueueTitleSoundStreamEntry(0x16);
    mnuResetTitleStreamAfterFileIdle();
    work->state = 2;
    work->update = D_0040ABF0[1];
    work->horizontalTilt = 0;
    work->targets = 0;
    work->unkA4 = 0;
    D_00438934 = 0;
    work->flags = ((work->flags & ~4) | 0x10) & ~0x20;
    work->phase = 0;
    work->result = 0;
    work->phaseTicks = 0;
    work->playerResetState = 0;
    evtPrintDeveloperConsoleMessage("stgInit_Section\n");
    return 1;
}


void mnuReleaseShootingSectionResources(MnuShootingWork *work) {
    s32 i;

    mnuDeactivateWorkEntry(&D_0040ABF8);
    if (work->mapObjects != NULL) {
        sdfMemFreeDirect(work->mapObjects->allocation);
        work->mapObjects = NULL;
    }
    if (work->drawObjects != NULL) {
        sdfMemFreeDirect(work->drawObjects->allocation);
        work->drawObjects = NULL;
    }
    if (work->objects != NULL) {
        sdfMemFreeDirect(work->objects->allocation);
        work->objects = NULL;
    }
    if (work->playerObjects != NULL) {
        sdfMemFreeDirect(work->playerObjects->allocation);
        work->playerObjects = NULL;
    }
    if (work->modelWork != NULL) {
        for (i = 0; i < work->modelWork->count; i++) {
            mnuDestroyAllModelNodeContexts(&work->modelWork->groups[i]);
        }
        sdfMemFreeDirect(work->modelWork->allocation);
        work->modelWork = NULL;
    }
    evtPrintDeveloperConsoleMessage("stgRelease_Section !!\n");
}

extern u8 dds3PadButtonState[2][2][16];
extern void mnuToggleShootingPause(MnuShootingWork *);
extern s32 mnuPollPauseChoiceInput(MnuShootingWork *);
extern s32 mnuAdvanceCursorStepUntilThreshold(void);
extern s32 mnuTriggerProgressResourceRecords(MenuProgressState *);
extern void mnuVisitActiveRecords(MnuShootingWork *);
extern void mnuProcessMenuWorkEntryLifecycle(MenuWorkEntry *, MnuShootingWork *);
extern s32 mnuUpdateActiveMenuWorkEntries(s32);
extern void mnuAdvanceRuntimeHitTargets(void);
extern s32 mnuUpdateShootingPlayerInput(void);
extern s32 mnuAdvanceShootingPlayerEntryReset(MnuShootingWork *);
extern MenuRuntimeRecord *mnuFindOverlappingRuntimeRecord(MenuWorkEntry *, MnuShootingWork *);
extern s32 mnuDispatchRuntimeListPairOverlaps(MnuShootingWork *);
extern s32 mnuAdvanceWorkEntriesOverlappingInput(MenuWorkEntry *);
extern MenuRuntimeList *mnuGetRuntimeAnimationList(void);
extern s32 mnuDispatchRuntimeWorkOverlaps(MenuRuntimeList *, MnuShootingWork *);
extern void CalcCamera(void);
extern void mnuTickScoreProgressState(MnuShootingWork *);
extern s32 mnuAdvanceShootingRoundPhase(MnuShootingWork *);
extern void mnuResetCurrentAndPeakScoresToPending(MnuShootingWork *);
extern void mnuClearRuntimeRecordStorage(MenuRuntimeList *);
void mnuRequestTimedEntryEffectStop(void);
extern void mnuClampPendingScoreToPeak(MnuShootingWork *);
extern void dds3UpdateSoundSlots(void);

s32 mnuUpdateShootingWork(MnuShootingWork *work) {
    MenuInputActionSnapshot input;
    MenuRuntimeRecord *record;
    MnuModelNode *node;
    dds3ModelHandle_t *model;
    s32 result;
    s32 actionFlags;
    s32 alternateAction;
    s32 mode;

    if ((s8)dds3PadButtonState[1][0][12] < 0 && work->phase == 2 &&
        !D_0040ABF8.flagsBits.updated && D_0040ABF8.inputCountdown == 0 &&
        !D_0040ABF8.flagsBits.inputDisabled) {
        mnuToggleShootingPause(work);
    }
    if (work->paused) {
        result = mnuPollPauseChoiceInput(work);
        switch (result) {
        case -1:
            break;
        case 0:
            mnuToggleShootingPause(work);
            break;
        case 1:
            work->flags |= 4;
            mnuToggleShootingPause(work);
            work->flags |= 0x20;
            work->phase = 11;
            work->result = 0;
            D_0040ABF8.remaining = 0;
            return 1;
        case 2:
            work->flags |= 4;
            mnuToggleShootingPause(work);
            work->flags |= 0x20;
            work->result = 1;
            work->phase = 11;
            D_0040ABF8.remaining = 0;
            return 1;
        }
        return 0;
    }
    if (work->phase == 2) mnuAdvanceCursorStepUntilThreshold();
    mnuTriggerProgressResourceRecords(mnuGetResourceProgressStepState());
    mnuVisitActiveRecords(work);
    mnuProcessMenuWorkEntryLifecycle(&D_0040ABF8, work);
    if (work->phase == 2) mnuUpdateActiveMenuWorkEntries(0);
    else mnuUpdateActiveMenuWorkEntries(1);
    mnuAdvanceRuntimeHitTargets();
    memset(&input, 0, sizeof(input));
    if (!((work->flags >> 2) & 1)) {
        if ((s8)dds3PadButtonState[1][0][6] != 0) input.verticalNegative = 1;
        else if ((s8)dds3PadButtonState[1][0][7] != 0) input.verticalPositive = 1;
        if ((s8)dds3PadButtonState[1][0][4] != 0) {
            if ((f32)work->horizontalTilt < 20.0f) work->horizontalTilt = (f32)work->horizontalTilt + 1.0f;
            input.horizontalNegative = 1;
        } else if ((s8)dds3PadButtonState[1][0][5] != 0) {
            if ((f32)work->horizontalTilt > -20.0f) work->horizontalTilt = (f32)work->horizontalTilt - 1.0f;
            input.horizontalPositive = 1;
        }
        if (!input.horizontalNegative && (f32)work->horizontalTilt > 0.0f)
            work->horizontalTilt = (f32)work->horizontalTilt - 1.0f;
        if (!input.horizontalPositive && (f32)work->horizontalTilt < 0.0f)
            work->horizontalTilt = (f32)work->horizontalTilt + 1.0f;
        if (((s8)dds3PadButtonState[1][0][0] < 0) | ((s8)dds3PadButtonState[1][0][1] != 0))
            input.actionSlot0 = 1;
        alternateAction = (s8)dds3PadButtonState[1][0][3];
        actionFlags = (s8)dds3PadButtonState[1][0][2];
        actionFlags |= alternateAction;
        if (actionFlags < 0)
            input.actionSlot1 = 1;
    }
    mnuSetInputActionSnapshot(&input);
    if (mnuUpdateShootingPlayerInput() == 0) mnuAdvanceShootingPlayerEntryReset(work);
    if (!((work->flags >> 5) & 1)) {
        record = mnuFindOverlappingRuntimeRecord(&D_0040ABF8, work);
        if (record != NULL) {
            if ((record->state.kind & 15) == 3) dds3ClaimSoundSlot(0x1E00004, 0);
            record->state.word &= ~1;
        }
    }
    if (work->phase < 3) {
        mnuDispatchRuntimeListPairOverlaps(work);
        if (work->phase < 3) mnuAdvanceWorkEntriesOverlappingInput(&D_0040ABF8);
    }
    mnuDispatchRuntimeWorkOverlaps(mnuGetRuntimeAnimationList(), work);
    CalcCamera();
    mnuTickScoreProgressState(work);
    switch (mnuAdvanceShootingRoundPhase(work)) {
    case 0:
        break;
    case 3:
        work->frameDisplayValue = 2;
        work->savedPlayerMode = 0;
        work->flags |= 4;
        mnuResetCurrentAndPeakScoresToPending(work);
        work->progress = 0;
        work->completed = 0;
        work->currentScore = 0;
        itfSetFadeGaugeDisplayValue(&work->gauge, 0);
        work->round = 0;
        return 1;
    case 2:
        work->state = -1;
        work->round = 0;
        work->flags |= 4;
        return 1;
    case 1:
        mode = D_0040ABF8.flagsBits.mode;
        work->progress = 0;
        work->completed = 0;
        work->savedPlayerMode = mode;
        itfSetFadeGaugeDisplayValue(&work->gauge, 0);
        mnuClearRuntimeRecordStorage(mnuGetRuntimeAnimationList());
        mnuRequestTimedEntryEffectStop();
        if (work->round >= 3) {
            work->state = -1;
            mnuClampPendingScoreToPeak(work);
        }
        return 1;
    }
    dds3UpdateSoundSlots();
    if ((s8)dds3PadButtonState[1][1][13] < 0) work->phase = 3;
    if (D_00438934 > 0) {
        D_00438934--;
        if (D_00438934 == 0) {
            node = D_0040ABF8.object.modelNode;
            model = node->model;
            model->first->frameStep = 1.0f;
            node->savedModelValue = 1.0f;
            mdlSmoothLoopMotion(model, 0, 0, 20.0f, 20.0f);
        }
    }
    return 0;
}



extern MenuWorkEntry *mnuGetWorkEntryPool(void);
extern void mnuVisitActiveWorkAndEffectEntry(MnuShootingWork *context);
extern MenuRuntimeList *mnuGetRuntimeAnimationList(void);
extern MenuRuntimeList *mnuGetRuntimeHitTargetList(void);
extern void itfDrawAndAdvanceModelInstances(ModelInstanceWork *work, u32 flags);
extern void itfDrawCenteredIndexedImage(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 index, s32 context);
extern void itfDrawScaledIndexedImage(s32 x, s32 y, s32 z, s32 alpha, f32 xScale, f32 yScale,
                          s32 flags, s32 index, s32 context);
extern void itfDrawRotatedIndexedImage(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 index, s32 context, f32 angle);
extern void mnuUpdateEffectQueues(MnuEffectWork *work, s32 paused);
extern void mnuUpdateSectionModelWork(MnuSectionModelWork *work, s32 paused);
extern void mnuSetNodePrimaryVector(MnuModelNode *node, f32 x, f32 y, f32 z);
extern void mnuSetNodeScaleVector(MnuModelNode *node, f32 value);
extern void mnuBroadcastNodeModelState(MnuModelNode *node, u32 state);
struct SdfPoolNode;
extern void itfDrawFullExtentWorkPanels(MnuShootingWork *work);
extern void itfDispatchObjectFadeSequenceMode(MnuShootingWork *work);
extern void mnuUpdateAndDrawPauseChoiceFade(MnuShootingWork *work);
extern MnuModelNode *D_00438928;
extern struct SdfPoolNode *kwlnSceneSurfaceSets[13][4];

s32 mnuDrawAndUpdateShootingScene(MnuShootingWork *work) {
    MenuProgressParameters *origin = mnuGetResourceProgressParameters();
    MenuRuntimeList *list;
    MenuRuntimeRecord *record;
    s32 originX;
    s32 originY;
    s32 index;

    (void)mnuGetResourceProgressStepState();
    (void)mnuGetWorkEntryPool();
    originX = origin->x;
    originY = origin->y;
    mnuVisitActiveWorkAndEffectEntry(work);

    if (work->paused == 0) {
        itfDrawAndAdvanceModelInstances(work->work24, 0);
    } else {
        itfDrawAndAdvanceModelInstances(work->work24, 1);
    }

    list = mnuGetRuntimeAnimationList();
    record = list->records;
    index = 0;
    while (index < list->capacity) {
        u32 stateWord = record->state.word;
        if ((stateWord & 1) != 0) {
            s32 x = (s32)((record->rotatedOffsetX + record->displacementX) + record->baseX);
            s32 y = (s32)((record->rotatedOffsetY + record->displacementY) + record->baseY);
            s32 kind = record->state.kind & 0xF;

            switch (kind) {
            case 0:
                switch ((s32)(stateWord >> 1) & 0xF) {
                case 2: {
                    s32 drawX = x + originX;
                    s32 drawY = y + originY;
                    mnuDrawIndexedSpriteWith(itfDrawCenteredIndexedImage, drawX, drawY, 0, 0x80, 0, 0x42, 0x53);
                    mnuDrawIndexedSpriteWith(itfDrawCenteredIndexedImage, drawX, drawY, 0, 0x80, 0, 0x43, 0x53);
                    break;
                }
                case 3:
                    mnuDrawIndexedSpriteWith(itfDrawCenteredIndexedImage, x + originX, y + originY, 0, 0x80, 0, 0x44, 0x53);
                    break;
                case 4:
                    mnuDrawIndexedSpriteWith(itfDrawCenteredIndexedImage, x + originX, y + originY, 0, 0x80, 0, 0x45, 0x53);
                    break;
                }
                break;
            case 5:
                switch ((s32)(stateWord >> 1) & 0xF) {
                case 3:
                    mnuDrawIndexedSpriteWith(itfDrawCenteredIndexedImage, x + originX, y + originY, 0, 0x80, 0, 0x48, 0x53);
                    break;
                case 4:
                    mnuDrawIndexedSpriteWith(itfDrawCenteredIndexedImage, x + originX, y + originY, 0, 0x80, 0, 0x49, 0x53);
                    break;
                }
                break;
            case 4:
                mnuUpdateTimedEffectPosition();
                break;
            default:
                mnuDrawIndexedSpriteWith(itfDrawCenteredIndexedImage, x + originX, y + originY, 0, 0x80, 0, 6, 0x53);
                break;
            }
        }
        index++;
        record++;
    }

    list = mnuGetRuntimeHitTargetList();
    record = list->records;
    index = 0;
    while (index < list->capacity) {
        u32 stateWord = record->state.word;
        if ((stateWord & 1) != 0) {
            s32 x = (s32)((record->rotatedOffsetX + record->displacementX) + record->baseX);
            s32 y = (s32)((record->rotatedOffsetY + record->displacementY) + record->baseY);
            s32 kind = record->state.kind & 0xF;

            switch (kind) {
            case 1:
                itfDrawRotatedIndexedImage(x + originX - 8, y + originY - 8, 0, 0x80, 0, 5, 0x53,
                    -(((f32)record->state.directionDegrees * 3.141592503f) / 180.0f + record->angle));
                break;
            case 2:
                itfDrawScaledIndexedImage(x + originX, y + originY, 0, 0x80, 2.0f, 2.0f, 0, 1, 0x53);
                break;
            case 3:
                mnuDrawIndexedSpriteWith(itfDrawCenteredIndexedImage, x + originX, y + originY, 0, 0x80, 0, 0x4A, 0x53);
                break;
            default:
                itfDrawScaledIndexedImage(x + originX, y + originY, 0, 0x80, 4.0f, 4.0f, 0, 0, 0x53);
                break;
            }
        }
        index++;
        record++;
    }

    if ((work->paused & 1) == 0) {
        mnuUpdateEffectQueues(work->effectWork, 0);
        mnuUpdateSectionModelWork(work->modelWork, 0);
        itfUpdateAndDrawSpriteNumbers(work->spriteNumberPool, 0);
        itfUpdateAndDrawSpriteHitEffects(work->hitEffectPool, 0);
    } else {
        mnuUpdateEffectQueues(work->effectWork, 1);
        mnuUpdateSectionModelWork(work->modelWork, 1);
        itfUpdateAndDrawSpriteNumbers(work->spriteNumberPool, 1);
        itfUpdateAndDrawSpriteHitEffects(work->hitEffectPool, 1);
    }

    if (D_00438928 != NULL) {
        mnuSetNodePrimaryVector(D_00438928, 0.0f, 0.0f, 150.0f);
        mnuSetNodeScaleVector(D_00438928, 1.0f);
        switch (work->round) {
        case 0:
            mnuBroadcastNodeModelState(D_00438928, 0x8059606C);
            break;
        case 1:
            mnuBroadcastNodeModelState(D_00438928, 0x80404C4C);
            break;
        case 2:
            mnuBroadcastNodeModelState(D_00438928, 0x80664C4C);
            break;
        }
        mdlLink(D_00438928->model, kwlnSceneSurfaceSets[0]);
    }

    itfDrawFullExtentWorkPanels(work);
    itfDispatchObjectFadeSequenceMode(work);
    mnuUpdateAndDrawPauseChoiceFade(work);
    return 0;
}

extern MnuModelNode *mnuAcquireUnusedModelNode(MnuNodeList *group);
extern ModelInstance *itfClaimFreeModelInstance(ModelInstanceList *list);
extern MnuModelNode *D_00438928;

void mnuCreateShootingEntryObject(MenuWorkEntry *entry, MnuShootingWork *work) {
    u32 tagClass = entry->tag & MNU_WORK_TAG_CLASS_MASK;
    MnuNodeList *groups = work->modelWork->groups;
    ModelInstanceList *instances = work->work24->lists;
    u32 parameterIndex;
    MenuRegistryParameters *parameters = NULL;
    s32 kind;

    switch (tagClass) {
    default:
        parameterIndex = 0;
        kind = 0;
        break;
    case MNU_WORK_TAG_MOVEMENT_TABLE:
        kind = 3;
        mnuLoadTitleSoundBufferEntry(0x12);
        mnuClearInactiveSoundBufferState();
        parameterIndex = mnuGetMovementRecordByIndex(entry->tag)->parameterTag;
        D_00438928 = mnuAcquireUnusedModelNode(&groups[8]);
        evtPrintDeveloperConsoleMessage("---------------------BG create!!\n");
        break;
    case MNU_WORK_TAG_REGISTRY_TABLE: {
        MenuRegistry *record = mnuGetMenuRecordRegistryEntry(entry->tag);
        parameterIndex = record->parameterIndex;
        parameters = mnuGetMenuRegistryParametersByIndex(parameterIndex);
        kind = parameters->kind;
        work->targets++;
        break;
    }
    }

    if (kind != 1) {
        if (kind == 3) {
            MnuModelNode *node = mnuAcquireUnusedModelNode(&groups[parameterIndex]);
            entry->object.modelNode = node;
            if (node != NULL) {
                if (tagClass == MNU_WORK_TAG_MOVEMENT_TABLE) node->flags |= 2;
                mnuPlaceShootingEntryObject(entry, work);
            } else {
                evtPrintDeveloperConsoleMessage("model none\n");
            }
        }
    } else {
        ModelInstance *instance = itfClaimFreeModelInstance(
            &instances[parameterIndex]);
        entry->object.modelInstance = instance;
        if (instance != NULL) {
            instance->scale = (f32)parameters->scale / 100.0f;
            mnuPlaceShootingEntryObject(entry, work);
        } else {
            printf("sprite none\n");
        }
    }
    evtPrintDeveloperConsoleMessage("---------------------create!!\n");
}



extern MenuRuntimeRecord *mnuCreateRuntimeRecord(MenuRuntimeList *, s32, s32, u8,
                                                s32, s32, s32, s16, s16, f32);
extern void itfProjectScreenPointToViewPlane(f32 *, s32, s32);
extern void mnuSetNodePairValue(MnuModelNode *, s32);
extern void mnuSetNodePosition(MnuModelNode *, f32, f32, f32);
extern void itfSetModelInstanceValuePair(ModelInstance *, s32);
extern void itfSetModelInstanceSecondaryVector(ModelInstance *, f32, f32, f32);

/* Finish/deactivate a shooting entry. Keep the timed coordinate distinct from
 * its two screen-space consumers, and retain fresh reads after callbacks. */
void mnuFinishOrDeactivateShootingEntry(MenuWorkEntry *entry, MnuShootingWork *work) {
    f32 position[4] __attribute__((aligned(16)));
    MnuEffectPositionStep step __attribute__((aligned(16)));
    MenuRegistry *registry;
    MenuRegistryParameters *parameters;
    MenuProgressParameters *origin;
    MenuRuntimeList *runtime;
    MnuEffectList *effects;
    void *object;
    s32 originX;
    s32 originY;
    s32 timedY;
    f32 angle;

    memset(&step, 0, sizeof(step));
    registry = mnuGetMenuRecordRegistryEntry(entry->tag);
    parameters = mnuGetMenuRegistryParametersByIndex(registry->parameterIndex);
    origin = mnuGetResourceProgressParameters();
    originX = origin->x;
    originY = origin->y;

    if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_MOVEMENT_TABLE) {
        work->completed = 0;
        work->progress = 0;
        entry->remaining = 0;
        mnuClearRuntimeRecordStorage(mnuGetRuntimeAnimationList());
        mnuRequestTimedEntryEffectStop();
        entry->object.modelNode->model->flags |= 1;
        if (--work->frameDisplayValue < 0) {
            mnuClampPendingScoreToPeak(work);
            work->playerResetState = 4;
            work->phase = 8;
        }
        if (entry->flagsBits.mode >= 2) {
            runtime = mnuGetRuntimeHitTargetList();
            angle = ((dds3GetRandFloatA(NULL) * 90.0f + 135.0f) *
                     3.141592503f) / 180.0f;
            mnuCreateRuntimeRecord(runtime, (s32)entry->currentX,
                                   (s32)entry->currentY, 0x33, 0, 0, 0, 1, 0,
                                   angle);
        }
        dds3ClaimSoundSlot(0x01E00006, 0);
    }

    if (entry->flagsBits.finished != 0) {
        timedY = (s32)mnuEvaluateTimedValue(entry);
        if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_REGISTRY_TABLE &&
            (entry->flagsBits.unk20 & 1) == 0) {
            itfClaimWideSlotWithTaggedPayload((s32)(entry->currentX + (f32)originX),
                                              timedY + originY, registry->score,
                                              (s8)work->step, work->spriteNumberPool);
        }
        itfProjectScreenPointToViewPlane(position,
                                         (s32)entry->currentX + originX, timedY + originY);
        effects = work->effectWork->lists;
        if (entry->movementEffectScale != 0) {
            step.x = sdfCos(
                         entry->currentAngleRadians + 1.5707963f) * 0.0f +
                     sdfSin(entry->currentAngleRadians + 1.5707963f);
            step.y = sdfCos(
                         entry->currentAngleRadians + 1.5707963f) -
                     sdfSin(entry->currentAngleRadians + 1.5707963f) * 0.0f;
            SDF_VU0_LOADVECTOR(&step);
            VU0_SCALE_VF(vf10, (f32)entry->movementEffectScale / 3.5f);
            SDF_VU0_STOREVECTOR(&step);
        } else {
            step.x = 0.0f;
            step.y = 1.0f;
            SDF_VU0_LOADVECTOR(&step);
            VU0_SCALE_VF(vf10, 4.0f);
            SDF_VU0_STOREVECTOR(&step);
        }
        if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_REGISTRY_TABLE) {
            mnuStartPositionedEffectBurst(effects, &step, 0,
                                          position[0], position[1], position[2], parameters->minor);
        } else {
            mnuStartPositionedEffectRecord(&effects[1], 0,
                                           position[0], position[1], position[2]);
        }
        if (entry->tag == 0x0201000F || entry->tag == 0x02010010) {
            evtPrintDeveloperConsoleMessage("======================================= ADD ITEM !!!!!!!!!!\n");
            runtime = mnuGetRuntimeHitTargetList();
            angle = ((dds3GetRandFloatA(NULL) * 90.0f + 135.0f) *
                     3.141592503f) / 180.0f;
            mnuCreateRuntimeRecord(runtime, (s32)entry->currentX,
                                   (s32)entry->currentY, 0x33, 0, 0, 0, 1, 0,
                                   angle);
        }
        if (registry->parameterIndex == 0x22 && work->playerResetState != 4) {
            D_0040ABF8.remaining = 0;
            work->phase = 3;
        }
    }

    object = entry->object.modelNode;
    if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_REGISTRY_TABLE) {
        if (object != NULL) {
            switch (parameters->kind) {
            case 3: {
                MnuModelNode *node = object;
                if (registry->parameterIndex == 0x22) {
                    switch (work->round) {
                    case 0:
                        mnuBroadcastNodeModelState(node, 0x80000080);
                        node->model->first->frameStep = 0.5f;
                        node->savedModelValue = 0.5f;
                        mdlSmoothLoopMotion(node->model, 0, 1, 0.0f, 30.0f);
                        mnuSetNodePairValue(node, 160);
                        mnuSetNodePosition(node, 0.0f, -0.5f, 0.0f);
                        break;
                    case 1:
                        mnuBroadcastNodeModelState(node, 0x80000080);
                        node->model->first->frameStep = 0.5f;
                        node->savedModelValue = 0.5f;
                        mdlSmoothLoopMotion(node->model, 0, 11, 0.0f, 30.0f);
                        mnuSetNodePairValue(node, 180);
                        mnuSetNodePosition(node, 0.0f, -0.5f, 0.0f);
                        break;
                    case 2:
                        mnuBroadcastNodeModelState(node, 0x80000080);
                        node->model->first->frameStep = 0.3f;
                        node->savedModelValue = 0.3f;
                        mdlSmoothLoopMotion(node->model, 0, 5, 0.0f, 30.0f);
                        mnuSetNodePairValue(node, 140);
                        mnuSetNodePosition(node, 0.0f, -0.5f, 0.0f);
                        break;
                    }
                } else {
                    mnuSetNodePairValue(node, 10);
                    mnuSetNodePosition(node,
                                       step.x / 3.0f, step.y / 3.0f, step.z / 3.0f);
                }
                entry->object.modelNode = NULL;
                object = NULL;
                break;
            }
            case 1:
                itfSetModelInstanceValuePair((ModelInstance *)object, 30);
                itfSetModelInstanceSecondaryVector(entry->object.modelInstance,
                                                   step.x / 3.0f, step.y / 3.0f,
                                                   step.z / 3.0f);
                entry->object.modelInstance = NULL;
                object = NULL;
                break;
            }
        }
        work->targets--;
    }
    if (object == NULL) {
        evtPrintDeveloperConsoleMessage("---------------------delete!! OBJECT INDEX %d\n",
                                         (u16)entry->tag);
    }
}


INCLUDE_ASM(const s32, "game/code_00316E08", mnuPlaceShootingEntryObject);

void mnuApplyPlayerRuntimeRecordFeedback(MenuRuntimeRecord *record) {
    MnuModelNode *node;

    if ((record->state.kind & 0xF0) == 0x40) {
        if (record->state.kind == 0x44) {
            dds3ClaimSoundSlot(0x1E00001, 0);
        } else if ((record->state.word & 0x1E) == 4 ||
                   (record->state.word & 0x1E) == 6) {
            dds3ClaimSoundSlot(0x1E00000, 0);
        }
    }
    node = D_0040ABF8.object.modelNode;
    if (record->state.kind == 0x44) {
        node->model->first->frameStep = 1.0f;
        node->savedModelValue = 1.0f;
        mdlSmoothLoopMotion(node->model, 0, 2, 0.0f, 0.0f);
        sdfMotionSeek(node->model->first, 20.0f);
        D_00438934 = node->model->first->frameCount - 20;
        evtPrintDeveloperConsoleMessage("ModelFrame [%d]\n", D_00438934);
        mnuClaimTimedEntryEffectRecord();
    }
}

/* Acquire a record from the third effect list at the timed UI position. */
void mnuClaimTimedEntryEffectRecord(void) {
    MenuProgressParameters *origin;
    s32 y, x;
    f32 value;
    f32 position[4];
    MnuEffectList *lists;

    origin = mnuGetResourceProgressParameters();
    x = origin->x;
    y = origin->y;
    value = mnuEvaluateTimedValue(&D_0040ABF8);
    itfProjectScreenPointToFixedDepth(position, (s32)D_0040ABF8.currentX + x, (s32)value + y - 32);
    lists = D_0043891C->effectWork->lists;
    D_00438930 = mnuClaimPositionedEffectRecord(lists + 2, position[0], position[1], position[2],
                                               NULL, 0.5f, 0);
    mnuUpdateTimedEffectPosition();
}

void mnuUpdateTimedEffectPosition(void) {
    MenuProgressParameters *origin;
    s32 y, x;
    f32 value;
    f32 position[4];

    if (D_00438930 != NULL && (D_00438930->flags & 0x801) == 1) {
        origin = mnuGetResourceProgressParameters();
        x = origin->x;
        y = origin->y;
        value = mnuEvaluateTimedValue(&D_0040ABF8);
        itfProjectScreenPointToFixedDepth(position, (s32)D_0040ABF8.currentX + x, (s32)value + y - 32);
        fileQueueSetPosition(D_00438930->queue, position);
    }
}

/* The queue updater consumes 0x800 as a deferred stop request. */
void mnuRequestTimedEntryEffectStop(void) {
    if ((D_00438930 != 0) && ((D_00438930->flags & 1) != 0)) {
        D_00438930->flags = D_00438930->flags | 0x800;
    }
}

s32 mnuAdvanceShootingPlayerEntryReset(MnuShootingWork *work) {
    MenuWorkEntry *entry = &D_0040ABF8;
    MnuModelNode *node = entry->object.modelNode;
    s32 finished = 0;
    s32 mode;

    switch (work->playerResetState) {
    case 0:
        work->playerResetState = 2;
        entry->currentX = 120.0f;
        entry->currentY = 500.0f;
        mode = entry->flagsBits.mode;
        if (entry->flagsBits.finished && mode >= 2) {
            mode = mode >= 3 ? mode - 2 : mode - 1;
            mnuRebuildMenuWorkEntryCallbackRecords(entry, mode);
        }
        entry->flags = ((mode & 0xF) << 15) | 0x4001;
        work->playerResetTicks = 0;
        break;
    case 2:
        work->playerResetTicks++;
        if (work->playerResetTicks >= 0x1F) {
            dds3ModelHandle_t *model = node->model;
            u32 modelFlags;

            work->playerResetState = 3;
            work->playerResetTicks = 0;
            modelFlags = model->flags & ~1;
            model->flags = modelFlags;
            entry->currentX = 120.0f;
            entry->currentY = 400.0f;
            entry->inputCountdown = 0x82;
        }
        break;
    case 3:
        work->playerResetTicks++;
        if (work->playerResetTicks >= 0x1F) {
            work->playerResetState = 1;
        }
        entry->currentY -= 2.0f;
        break;
    case 1:
        work->playerResetState = 0;
        finished = 1;
        entry->flags &= ~0x4000;
        break;
    case 4:
        entry->currentX = 120.0f;
        entry->currentY = 500.0f;
        entry->flags = (entry->flags | 0x4000) & ~4;
        break;
    }
    return finished;
}

INCLUDE_SDATA(const s32, "game/code_00316E08", D_0043891C);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438920);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438928);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438930);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438934);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438938);

INCLUDE_SDATA(const s32, "game/code_00316E08", dds3SoundSlotPool);

INCLUDE_SDATA(struct dds3Basic_t * const, "game/code_00316E08", D_00438944);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438948);

INCLUDE_SDATA(MenuWorkEntry * const, "game/code_00316E08", D_0043894C);
