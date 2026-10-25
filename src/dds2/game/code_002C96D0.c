/* Keeps its own declaration of the functions below (see sdk_libc.h note). */
#define SDK_SKIP_memset
#include "sdk_ps2.h"
#include "eff_surface_node.h"
#include "common.h"
#include "file_config_list_node.h"
#include "sdf_vu0.h"
#include "file_job_type.h"
#include "file_config_task.h"
#include "eff_emitter_type.h"
#include "file_load_menu_state.h"
#include "eff.h"
#include "cam_aim_record.h"
#include "eff_generated_texture_descriptor.h"
#include "dds3_world_selection.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "eff_scale_params.h"
#include "ee_mmi.h"
#include "eff_random.h"
#include "sdf_quaternion.h"
#include "btl_stage_task_cleanup.h"
#include "sdf_textured_rect.h"
#include "itf_mes_window.h"
#include "evt_world.h"
#include "fr_font.h"
#include "fr_font_context.h"
#include "mc_poll.h"
#include "mc_path_api.h"
#include "bill_object_api.h"
#include "eff_resource_slots.h"
#include "eff_update_flags.h"
#include "sdf_resource.h"
#include "sdf_chip.h"
#include "eff_curve.h"
#include "file.h"
#include "file_selection_api.h"
#include "file_request_entry.h"
#include "file_slot_flags.h"
#include "file_scroll_arrow_state.h"
#include "sdf_dev_state.h"
#include "file_slot.h"
#include "file_save_record.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "kwln_task_lifecycle.h"
#include "eff_expanded_list.h"
#include "file_request_api.h"
#include "sdf_texture_file.h"
#include "sdf.h"
#include "mdl_flag_api.h"
#include "btl.h"
#include "fr_font_draw_glyph.h"
#include "itf.h"
typedef s32 (*FileMenuContinuationCallback)(void);
/* Declared before first use (were implicit int). */
extern void effLoadFlashTextures(void);
extern void effLoadScalyTexture(void);
extern void effLoadWindTexture(void);
extern s32 slSaveWriteFile(const char *fileName, u32 *dataAddressSlot, u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest **pendingRequestStatus);
extern void dds3SoftResetOn(void);
extern s32 slSaveData_chdir(void);
extern void frEnableMode(s32 mask);
extern u8 frDisableMode(u8 mask);
extern void frSetFontDrawOT(u32 renderFlags);
extern void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg);
struct EffectSlotSet;
extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, u32, struct EffectSlotSet *, s32, s32);

enum { FILE_CONFIG_SET, FILE_CONFIG_FRAME, FILE_CONFIG_X, FILE_CONFIG_Y };
extern s16 fileConfigSpritePlacements[27][4];

extern s32 D_00437D4C;
extern s32 fileDrawFileMenuPromptAndCheckDelay(void);
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct MenuWork {
    FileSavePreviewRecord header;
    s8 applyLoadResult;
    s8 hardInitSelected;
    u16 flowConditionMask;
    u16 startBranchFlag; /* 0x34: selects one of two initial menu flows */
    u16 flowRowCursor;
    u16 unk38;
    u16 unk3A;
    struct sdfMemHandle_t *unk3C;
} MenuWork;

extern FileScrollArrowState fileScrollArrowState;
extern f32 D_00437D24;
extern u8 fileNewCycleLabel[];
extern u8 D_003A41A8[][32];
extern u8 D_003A47E8[][32];
extern char D_00437D98[];
extern char D_00437DA0[];
extern char D_00437DA8[];
extern u32 fileClampPackedColorByte(u32, s32, s8);
extern void fileDrawSlotIcon(s32, s32, s32, s32);
extern void fileCursorStepUp(void);
extern void fileFadeStepDown(void);
extern void fileLoadCtxSlideUpdate(void);

extern u8 fileOriginalTitleClearedGameLabel[];
extern u8 fileOriginalTitleNewCycleClearedGameLabel[];
extern u8 fileOriginalTitleUnusableSaveText[];
extern void *fileSaveReadBuffer;
typedef struct MdlFlagPair {
    s32 flag;
    s32 unk4;
} MdlFlagPair;

/* Eight-byte story-flag rows select a unit and a six-byte bonus record. */
typedef struct FileStatBonus {
    s32 flag;
    u16 unitId;
    u8 bonusIndex;
    u8 pad07;
} FileStatBonus;
extern MdlFlagPair fileRareItemNoticeFlagRows[];
extern FileStatBonus ptyUnlockedUnitStatBonusRows[24];
extern s32 mnuAdvanceTitleStateUnderSemaphore(void);
#include "kwln.h"
#include "kwln_task_create.h"
#include "kwln_fade.h"
#include "fpu.h"
struct MenuListNode;
extern void mnuClearListFlagsOneAndTwo(u32 *flags);
extern void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

struct sdfMemHandle_t;





extern MenuWork *fileLoadSelectionWork;
extern void fileApplyMenuFlagsToModel(MenuWork *);
extern void fldPrepareDeferredSceneTransition(void);
extern void fileCopySaveHeaderNumbers(FileSavePreviewRecord *);
extern void filePrintAccessoryLoadNotice(void);
extern void fileSaveAndDisplayCurrentMoney(void);
extern s32 ptyApplyUnlockedUnitStatBonusesOnce(s32);
extern u32 datComputeSkillBoostedMaxHp(datUnitWork_t *);
extern u32 datComputeSkillBoostedMaxMp(datUnitWork_t *);
extern void mtrMantraEventBitReset(void);
extern void ptyResetPartyRecordsAndProfiles(void);
extern u32 dds3DrawFlag;

extern u32 fileWaitTicksRemaining;

extern u32 D_00437CEC;

extern s32 fileSlotSelectionPollCount;

extern u32 D_00437CFC;

extern u32 D_00437D08;

extern u32 D_00437D14;

extern u32 D_00437D18;

extern u32 D_00437D1C;

extern u32 D_00437D20;

extern u32 D_00437D40;

extern u32 D_00437D44;

extern u32 D_00437D3C;

extern u32 itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, s32);
extern struct _FRQ *fileMenuConvertedTextGlyph;
extern struct _FRQ *fileMenuConfiguredTextGlyph;

extern struct _FRQ *fileMenuStringGlyph;

extern dds3ProcessID_t *D_00437CD8;

extern s32 D_00437CE4;
extern s32 fileSavePreviewPortraitTextures[];
extern s32 D_00437D74;
extern s32 D_00437D6C;
extern s32 D_00437D68;
extern s32 D_00437D64;
extern s32 D_00437D60;
extern s32 D_00437D5C;
extern u8 (*D_00437D54)[32];
extern u8 (*D_00437D58)[32];


extern s32 D_00437D38;

extern s32 D_00437D48;

extern u32 fileMemoryCardRequestIndex;

extern u32 D_00437CF4;

extern u32 D_00437CF8;

extern u32 fileSelectionPending;

extern u32 D_00437D04;


extern u32 fileSavedSlotFlags;

extern u32 effModelUpdateControlFlags;

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

extern s8 slLoadKillFlag;
extern s32 mcdOriginalTitleFileMode;
extern s32 D_00437D88;

extern s32 fileSlotSelectPoll(void);

extern s32 fileBeginSlotMetadataRefresh(void);

extern s32 slFormatStart(void);

extern s32 mcResetSlotMetadata(void);

extern s32 filePollSlotDetectionAndBranch(void);

extern s32 slSizeChk(void);

extern FileMenuContinuationCallback fileMenuContinuation;

extern s32 mcdFinishFileDetection(void);

extern s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback);

extern s32 filePollSlotRequestAndResumeFlow(void);

extern s32 fileSlotScanIndex;

extern void mcdFormatSaveSlotName(char *buffer, s32 slot);

extern s32 slChooseData_getCardInfo2(void);

extern s32 slChooseData_getCardInfo2_B(void);

extern s32 filePrepareMainBlobWrite(void);

extern s32 fileOnAllWritten(void);

extern void dds3SoftResetOff(void);
extern s8 dds3PadButtonState[];
extern void p2sdStopBGM(u32 command);
extern s32 slExitWait(FileMenuContinuationCallback callback);

extern s32 mcHandleDetectionResult(void);



/* Loader context at D_0037D4A0. */
extern FileLoadMenuState fileLoadMenuState;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern s32 D_003E8008[];

extern SdfPoolNode *effPacketSubmissionSurfaces[6];


extern void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);

/* Effect parameter-set dispatch tables. Every effect kind owns one 0x28-byte
 * entry per table; the handler lives at +0x0. Slots are declared as separate
 * arrays (D_00353710/14/18/1C/20/24/28/2C/30/34 and D_00353880/84/88/90/94/
 * 98/9C/A0/A4). The family2 create table (D_00353880) additionally carries a
 * fallback selector at +0x0C: nonzero calls the entry handler directly,
 * zero falls back through D_003536A0.
 */
/* 8-byte parameter work (family1): kind id plus one data pointer. */
typedef struct EffDispatchExtra {
    void (*func)(void *, f32 vector[4]); /* 0x00 vector handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchExtra;

typedef struct EffDispatchScale {
    void (*func)(void *, f32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchScale;
typedef struct EffDispatchColor {
    void (*func)(void *, u32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchColor;






extern s8 dds3ConfigState;

extern s32 mcdFinishFileDetectionWithAudio(void);

extern s32 fileResetSlotSelection(void);

extern s32 D_00437D50;

extern s8 fileSavePreviewVersionByteView[];

extern s32 (*fileMenuStateHandler)();

extern void eglSetOt(u32);

extern void evtSubmitPrimaryAlphaBlendMode(s32);

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 D_00437D70;

extern void fileCursorPulseUpdate(void);

extern void fileDrawCursorPulse(s32, s32, s32);

extern s32 D_00437D78;

extern s32 fileReadSlotPreviewBegin(void);
extern void sdfMcFuncRead(s32, void *, s32);
extern s32 slLoadProc_readFile(void);
extern s32 fileSaveFileDescriptor;
extern struct sdfMemHandle_t *fileSaveReadBufferResource;
extern s32 slHeadLoadProc_readFile(void);

extern s32 fileBeginReadRemainingSlotIcons(void);

extern s32 slHeadLoad(void);

extern s32 fileSlotSelectPollClear(void);

extern s32 slBootRetry(void);

extern s32 fileSlotStatusPoll(void);

extern s32 fileResetSelection(void);

/* 0x40-byte backing region; the browser uses the first ten slot states. */
extern u32 fileSlotDisplayStates[16];

extern FileMenuContinuationCallback fileWaitContinuation;

extern s32 slExitWaitProc(void);

extern s32 slChooseData_getCardInfo3(void);


extern s32 slBootError(void);

extern s32 fileScanSlotStatesAdvance(void);

extern s32 slChooseData_getCardInfo3_B(void);

extern s32 slSaveData(void);

extern s32 slSaveProc_Write_mkdir(void);

extern s32 fileScanSlotStates(void);

extern s32 slLoadProc_openFile(void);






extern s32 fileDrawMenuFrame(s32);

extern u32 fileSaveIconData;

extern u32 fileSaveIconDataSize;

extern char fileConfigInputTaskName[];

extern char fileConfigLoadTaskName[]; /* "config_draw" */

extern char fileConfigOwnerTaskName[]; /* "config_update" */


typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

extern LoadMirror fileSlotFlagMirror;

extern s32 fileLoadStateChanged(void);

extern void fileCacheSlotFlagsFromState(void);



extern void fileLoadObjectSetResource(EffectSurfaceNode *node, u32 entryId, void *resource);
extern void fileLoadObjectOpenDevice(EffectSurfaceNode *node, u32 resourceId);
extern void fileLoadObjectOpenAndStartDevice(EffectSurfaceNode *node, u32 resourceId);
extern void fileLoadObjectOpenNamedDevice(EffectSurfaceNode *node, u32 resourceId);
extern void fileReplaceEffectSurfaceJobs(EffectSurfaceNode *node, FileJobPayload *job);
extern void fileReplaceEffectSurfaceQueues(EffectSurfaceNode *node, FileQueue *queue);
extern void fileReplaceReferenceHolder(EffectSurfaceNode *node, u32 resource);

extern void fileJobFreePrimaryBuffer(FileJobPayload *job);
extern void fileJobFreeSecondaryBuffer(FileJobPayload *job);


extern void fileQueueAppend(FileQueue *queue, FileJob *job);

extern void fileQueueInitTransform(void *queue);

extern FileJob *fileJobCreate(void);


extern s32 slEndingExit(void);

extern s32 slEndingTop(void);

extern s32 D_00437D30;

extern s32 slEndingUpd2(void);

extern char D_0042B698[];

extern s32 slErrorWait(void);


extern char D_0042B6B8[];

extern sceMcTblGetDir fileDirectoryEntryScratch[];

extern s32 mcClearSlotMetadata(void);

extern s32 slSaveData_deleteIconSys(void);

extern s32 fileSelectedSlotIndex;

extern s32 fileBuildMainBlobAfterDelete(void);

extern s32 fileBuildMainBlobAndWrite(void);

extern s32 slFormatCancel(void);

extern u32 D_00437D0C;

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F660[];




extern FileJob *fileQueueFindById(FileQueue *queue, u32 id);
extern s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target);




extern f32 fileSaveHighlightPhase;



extern u8 sdfHddValid;

extern char D_00437E10[];

extern char D_00437E18[];

extern char D_00437E20[];

extern char *sdfFileGetHostRoot(void);





void slKillLoadProcess(void) {
    if (D_00437CD8 != NULL) {
        dds3KillProcess(D_00437CD8, 1);
        D_00437CD8 = NULL;
        slLoadKillFlag = 0;
    }
}

s8 fileMenuTaskIsAlive(void) {
    return slLoadKillFlag;
}

void mnuFormatSaveSlotHeaderName(void) {
}

void mcdFormatSaveSlotName(char *buffer, s32 slot) {
    s32 titleId = 0x52a0;
    if (mcdOriginalTitleFileMode != 0) {
        titleId = 0x51ee;
    }
    D_00437D88 = titleId;
    sprintf(buffer, "BASLUS-%05d-new-%d", titleId, slot);
}

void fileReqGetSlotCode(void) {
    s32 slot = fileReqGetSelectedSlot(fileMemoryCardRequestIndex);
    D_00437D50 = fileSavePreviewVersionByteView[slot * 0x30];
}

/* Size of the persistent main save block copied during a reload. */
#define FILE_MAIN_BLOB_SIZE 0x1E840

u32 fileMainBlobSize(void) {
    return FILE_MAIN_BLOB_SIZE;
}

void fileReloadSaveBuffer(void) {
    struct sdfMemHandle_t *saved = DDS3_GBWK->header.backingAllocation;
    s32 size = FILE_MAIN_BLOB_SIZE;
    memcpy(DDS3_GBWK, fileSaveReadBuffer, size);
    DDS3_GBWK->header.backingAllocation = saved;
}

u8 fileIsLoadedAndConditionTrue(s32 condition) {
    return condition != 0 && D_00437CE4 == 1;
}

u8 fileIsLoadedWithActiveFlow(s32 condition) {
    return condition != 0 && D_00437CE4 == 1;
}

void mnuDrawAndStoreTextGlyphHandle(s32 x, s32 y, u32 colors, const u8 *text) {
    struct _FRQ *handle = itfCreateConvertedTextGlyph(x << 4, y << 3, 0, colors, text, 0);
    fileMenuConvertedTextGlyph = handle;
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frDestroyFontRequestAfter(fileMenuConvertedTextGlyph);
}

void mcdCreateConfiguredDrawHandle(s32 x, s32 y, u32 colors, const u8 *text) {
    struct _FRQ *handle = itfCreateConvertedTextGlyph(x << 4, y << 3, 0, colors, text, 0);
    fileMenuConfiguredTextGlyph = handle;
    frSetFontColorNo(handle, 3);
    frFontDrawGlyphWithSharedFlags(fileMenuConfiguredTextGlyph, 1);
    frDestroyFontRequestAfter(fileMenuConfiguredTextGlyph);
}

void mcdCreateFontDrawHandle(s32 x, s32 y, u32 colors, const char *text) {
    frEnableMode(1);
    fileMenuStringGlyph = frFontBuildGlyphChain(text, 0, 0, 0, 0);
    frDisableMode(1);
    frSetFontSpace(fileMenuStringGlyph, 1);
    frSetDrawPosition(fileMenuStringGlyph, x << 4, y << 3);
    frSetFontColor(fileMenuStringGlyph, colors);
    frFontDrawGlyphChain(fileMenuStringGlyph, 0, 0x56);
    frDestroyFontRequestAfter(fileMenuStringGlyph);
    frSetFontDrawOT(0x54);
}

void fileDrawMenuImageAtPoint(s32 x, s32 y, u32 colors, char *text) {
    u32 handle;

    handle = itfCreateFont2TextGlyph(x << 4, y << 3, 0, colors, text, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frDestroyFontRequestAfter((struct _FRQ *)(u32)handle);
}

/* Animate the save-window highlight's alpha with a sinusoidal phase. */
void fileDrawPulsingSaveHighlight(void) {
    s32 angle;
    f32 wave;

    eglSetOt(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    fileSaveHighlightPhase = fileSaveHighlightPhase + 0.39999998f;
    sdfSin(fileSaveHighlightPhase);
    angle = ((s32)D_003E8008[2] + 8) % 360;
    D_003E8008[2] = angle;
    wave = sdfSin((f32)((angle + 0x5A) % 360) / 180.0f * 3.1415899f);
    D_003E8008[3] = (s32)((wave + 1.0f) * 0.5f * 191.0f + 64.0f);
    evtSubmitTexturedGradientQuad(0x1BE, 0x12C, 0x13, 0x1F, 1, 1, 0x13, 0x1F, (D_003E8008[3] << 24) | 0xAEC014,
                  (D_003E8008[3] << 24) | 0xAEC014, (D_003E8008[3] << 24) | 0xAEC014,
                  (D_003E8008[3] << 24) | 0xAEC014, itfMesGetWindowTexture());
}

void fileDrawSaveWindow(void) {
    eglSetOt(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitTexturedGradientQuad(0x112, 0x113, 0x98, 0x34, 0x14A, 0x1C5, 0x98, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, (struct sdfTexHandle_t *)(u32)(D_00437D78));
    evtSubmitTexturedGradientQuad(0x56, 0x113, 0xBC, 0x34, 0x14A, 0x1C5, 1, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, (struct sdfTexHandle_t *)(u32)(D_00437D78));
    fileCursorPulseUpdate();
    fileDrawCursorPulse(0x17E, 0x118, 0x56);
    D_00437D44++;
}

s32 slChkWindowOpenDone(void) {
    if (D_00437D48 < 7) {
        return 0;
    }
    return 1;
}

/* Draw the expanding prompt panel and report when its input delay has elapsed. */
s32 fileDrawFileMenuPromptAndCheckDelay(void) {
    s32 y;
    s32 height;
    s32 value;

    eglSetOt(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 128, 3, 0, 0, 1, 1);
    if (D_00437D48 < 32) {
        D_00437D48++;
        D_00437D4C++;
    }
    if (D_00437D48 < 4) {
        height = D_00437D48 * 72;
        y = 224 - D_00437D48 * 36;
    } else {
        y = 80;
        height = 288;
    }
    if (D_00437D48 < 4) {
        value = D_00437D48 * 70 / 4;
    } else {
        value = 70;
    }
    evtSubmitDefaultDepthGradientRect(0, y, 512, height,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000);
    eglSetOt(0x53);
    if (D_00437D48 < 7) {
        return 0;
    }
    return 1;
}

extern s32 D_00437D28;
extern void fileResetLoadContextSlide(void);

extern char fileNoCardStatus1MessageLine1[];
extern char fileNoCardStatus1MessageLine2[];
extern char fileNoCardStatus1MessageLine3[];
extern char fileNoCardStatus1MessageLine4[];
extern char fileNoCardStatus1MessageLine5[];
extern char fileNoCardStatus2MessageLine1[];
extern char fileNoCardStatus2MessageLine2[];
extern char fileNoCardStatus2MessageLine3[];
extern char fileNoCardStatus2MessageLine4[];
extern char fileNoCardStatus2MessageLine5[];
extern char fileLoadFailedText[];
extern char fileSaveFailedText[];
extern char fileFormatFailedText[];
extern char fileInsufficientSpaceMessageLine1[];
extern char fileInsufficientSpaceMessageLine2[];
extern char fileInsufficientSpaceMessageLine3[];
extern char fileInsufficientSpaceMessageLine4[];
extern char fileInsufficientSpaceMessageLine5[];
extern char fileNoSaveDataMessageLine1[];
extern char fileNoSaveDataMessageLine2[];
extern char fileNoSaveDataMessageLine3[];
extern char D_003E8990[];
extern char D_003E89B8[];
extern char fileOriginalTitleSaveMissingText[];
extern char fileNotClearedGameSaveText[];
extern char fileSelectClearedGameSaveMessageLine1[];
extern char fileSelectClearedGameSaveMessageLine2[];
extern char fileCorruptSaveDataMessageLine1[];
extern char fileCorruptSaveDataMessageLine2[];
extern char fileCorruptSaveDataMessageLine3[];

void fileDrawMemoryCardFlowPrompt(void) {
    if (D_00437D3C == 0) {
        return;
    }
    if (D_00437D3C == 1) {
        fileSelectedSlotIndex = 0;
        D_00437D28 = 0;
        fileResetLoadContextSlide();
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileNoCardStatus1MessageLine1);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileNoCardStatus1MessageLine2);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileNoCardStatus1MessageLine3);
            mcdCreateFontDrawHandle(86, 186, 0x89FEFF80U, fileNoCardStatus1MessageLine4);
            mcdCreateFontDrawHandle(86, 210, 0x89FEFF80U, fileNoCardStatus1MessageLine5);
            fileDrawPulsingSaveHighlight();
            D_00437CF8 = 0;
        }
    }
    if (D_00437D3C == 2) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileNoCardStatus2MessageLine1);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileNoCardStatus2MessageLine2);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileNoCardStatus2MessageLine3);
            mcdCreateFontDrawHandle(86, 186, 0x89FEFF80U, fileNoCardStatus2MessageLine4);
            mcdCreateFontDrawHandle(86, 210, 0x89FEFF80U, fileNoCardStatus2MessageLine5);
            fileDrawPulsingSaveHighlight();
            D_00437CF8 = 0;
        }
    }
    if (D_00437D3C == 3) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileLoadFailedText);
            fileDrawPulsingSaveHighlight();
            D_00437CF8 = 0;
        }
    }
    if (D_00437D3C == 4) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileSaveFailedText);
            fileDrawPulsingSaveHighlight();
            D_00437CF8 = 0;
        }
    }
    if (D_00437D3C == 5) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileInsufficientSpaceMessageLine1);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileInsufficientSpaceMessageLine2);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileInsufficientSpaceMessageLine3);
            mcdCreateFontDrawHandle(86, 186, 0x89FEFF80U, fileInsufficientSpaceMessageLine4);
            mcdCreateFontDrawHandle(86, 210, 0x89FEFF80U, fileInsufficientSpaceMessageLine5);
            fileDrawPulsingSaveHighlight();
            D_00437CF8 = 0;
        }
    }
    if (D_00437D3C == 6) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileFormatFailedText);
            fileDrawPulsingSaveHighlight();
            D_00437CF8 = 0;
        }
    }
    if (D_00437D3C == 7) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            if (mcdOriginalTitleFileMode == 0) {
                mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileNoSaveDataMessageLine1);
                mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileNoSaveDataMessageLine2);
                mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileNoSaveDataMessageLine3);
            } else {
                mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileOriginalTitleSaveMissingText);
            }
            fileDrawPulsingSaveHighlight();
            D_00437CF8 = 0;
        }
    }
    if (D_00437D3C == 8) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileNotClearedGameSaveText);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, D_003E8990);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, D_003E89B8);
            mcdCreateFontDrawHandle(86, 186, 0x89FEFF80U, fileSelectClearedGameSaveMessageLine1);
            mcdCreateFontDrawHandle(86, 210, 0x89FEFF80U, fileSelectClearedGameSaveMessageLine2);
            fileDrawPulsingSaveHighlight();
        }
    }
    if (D_00437D3C == 9) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileCorruptSaveDataMessageLine1);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileCorruptSaveDataMessageLine2);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileCorruptSaveDataMessageLine3);
            fileDrawPulsingSaveHighlight();
        }
    }
}


void slSetInfo(u32 value) {
    s32 previous;

    previous = D_00437D38;
    D_00437D38 = value;
    if (previous == 0) {
        D_00437D48 = 0;
    }
}

extern char fileCheckingCardMessageLine1[];
extern char fileCheckingCardMessageLine2[];
extern char fileCheckingCardMessageLine3[];
extern char fileLoadingText[];
extern char fileSavingText[];
extern char fileFormattingText[];
extern char fileCardBusyCardWarningText[];
extern char fileCardBusyConsoleWarningText[];
extern char fileLoadSuccessfulText[];
extern char fileSaveSuccessfulText[];
extern char fileFormatSuccessfulText[];
extern char fileOriginalTitleSaveBenefitsMessageLine1[];
extern char fileOriginalTitleSaveBenefitsMessageLine2[];
extern char fileOriginalTitleSaveBenefitsMessageLine3[];
extern char fileOriginalTitleSaveBenefitsMessageLine4[];
extern char fileRareItemAvailabilityMessageLine1[];
extern char fileRareItemAvailabilityMessageLine2[];
extern char fileCharacterStatBonusMessageLine1[];
extern char fileCharacterStatBonusMessageLine2[];
extern char fileDifficultySelectionAvailableText[];
extern char fileDataTransferCompleteText[];

void fileDrawModeSpecificMemoryCardPrompt(void) {
    if (D_00437D38 == 0) {
        return;
    }

    if (D_00437D38 == 1) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileCheckingCardMessageLine1);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileCheckingCardMessageLine2);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileCheckingCardMessageLine3);
            fileDrawSaveWindow();
        }
    }
    if (D_00437D38 == 2) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileSavingText);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileCardBusyCardWarningText);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileCardBusyConsoleWarningText);
            fileDrawSaveWindow();
        }
    }
    if (D_00437D38 == 3) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileLoadingText);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileCardBusyCardWarningText);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileCardBusyConsoleWarningText);
            fileDrawSaveWindow();
        }
    }
    if (D_00437D38 == 4) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileSaveSuccessfulText);
            fileDrawPulsingSaveHighlight();
        }
        D_00437D04 = 1;
    }
    if (D_00437D38 == 5) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileFormattingText);
            mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileCardBusyCardWarningText);
            mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileCardBusyConsoleWarningText);
            fileDrawSaveWindow();
        }
    }
    if (D_00437D38 == 6) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileFormatSuccessfulText);
            fileDrawPulsingSaveHighlight();
        }
    }
    if (D_00437D38 == 13) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileLoadSuccessfulText);
            fileDrawPulsingSaveHighlight();
        }
        D_00437D04 = 2;
    }
    if (D_00437D38 >= 20) {
        if (fileDrawFileMenuPromptAndCheckDelay() != 0) {
            switch (D_00437D38) {
            case 20:
                mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileOriginalTitleSaveBenefitsMessageLine1);
                mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileOriginalTitleSaveBenefitsMessageLine2);
                mcdCreateFontDrawHandle(86, 162, 0x89FEFF80U, fileOriginalTitleSaveBenefitsMessageLine3);
                mcdCreateFontDrawHandle(86, 186, 0x89FEFF80U, fileOriginalTitleSaveBenefitsMessageLine4);
                fileDrawPulsingSaveHighlight();
                break;
            case 21:
                mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileRareItemAvailabilityMessageLine1);
                mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileRareItemAvailabilityMessageLine2);
                fileDrawPulsingSaveHighlight();
                break;
            case 22:
                mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileCharacterStatBonusMessageLine1);
                mcdCreateFontDrawHandle(86, 138, 0x89FEFF80U, fileCharacterStatBonusMessageLine2);
                fileDrawPulsingSaveHighlight();
                break;
            case 23:
                mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileDifficultySelectionAvailableText);
                fileDrawPulsingSaveHighlight();
                break;
            case 24:
                mcdCreateFontDrawHandle(86, 114, 0x89FEFF80U, fileDataTransferCompleteText);
                fileDrawPulsingSaveHighlight();
                break;
            }
        }
    }
}


void fileClearAllSlotFlags(void) {
    fileReqMarkSlotMetadataDirty(fileMemoryCardRequestIndex);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 0);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 1);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 2);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 3);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 4);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 5);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 6);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 7);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 8);
    fileReqClearSlotFlags(fileMemoryCardRequestIndex, 9);
}

s32 slHeadLoad(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    fileReqSetSelectedSlot(fileMemoryCardRequestIndex, fileSlotScanIndex);
    ctx = fileMemoryCardRequestIndex;
    slot = fileReqGetSelectedSlot(ctx);
    if ((fileReqGetSlotFlags(ctx, slot) & FILE_REQ_SLOT_CAN_READ_SAVE) == 0) {
        return fileBeginReadRemainingSlotIcons();
    }
    path[0] = 0x2F;
    mcdFormatSaveSlotName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return (s32)fileReadSlotPreviewBegin;
}

s32 fileReadSlotPreviewBegin(void) {
    McPollResult status = sdfMcFuncOpen1(&fileSaveFileDescriptor);

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        fileSaveReadBufferResource = sdfMemAllocDirect(0x30);
        fileSaveReadBuffer = (void *)(u32)sdfMemLock(fileSaveReadBufferResource);
        sdfMcFuncRead(fileSaveFileDescriptor, fileSaveReadBuffer, 0x30);
        return (s32)slHeadLoadProc_readFile;
    }
    return fileBeginSlotMetadataRefresh();
}

extern void sdfMcFuncClose(u32);
extern s32 fileStoreSlotHeader(void);

s32 slHeadLoadProc_readFile(void) {
    McPollResult r = sdfMcFuncRead1();

    if (r == MC_POLL_PENDING) {
        return 0;
    }
    if (r == MC_POLL_SUCCESS) {
        sdfMcFuncClose(fileSaveFileDescriptor);
        return (s32)fileStoreSlotHeader;
    }
    sdfMemFreeDirect(fileSaveReadBufferResource);
    return fileBeginSlotMetadataRefresh();
}

/* Ten contiguous 0x30-byte save-preview records (0x1E0 bytes total). */
extern FileSavePreviewRecord fileSavePreviewRecords[10];

s32 fileStoreSlotHeader(void) {
    McPollResult status = sdfMcFuncClose1();

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        memcpy(&fileSavePreviewRecords[fileSlotScanIndex], fileSaveReadBuffer, 0x30);
        sdfMemFreeDirect(fileSaveReadBufferResource);
        return fileBeginReadRemainingSlotIcons();
    }
    sdfMemFreeDirect(fileSaveReadBufferResource);
    return fileBeginSlotMetadataRefresh();
}

s32 slExitWait(FileMenuContinuationCallback callback) {
    dds3FadeOut(0, 0, 0, 15);
    fileWaitContinuation = callback;
    fileWaitTicksRemaining = 0x14;
    return (s32)slExitWaitProc;
}

s32 slEndingTop(void) {
    D_00437D30 = 0;
    slSetInfo(0);
    D_00437D3C = 0;
    D_00437D1C = 4;
    D_00437CF8 = 0;
    return slQueryYesNo(&fileResetSelection, &slEndingExit, 0);
}

s32 slEndingUpd2(void) {
    slSetInfo(0);
    D_00437D3C = 0;
    D_00437D1C = 9;
    return slQueryYesNo(&slSaveData, &fileScanSlotStates, 1);
}

extern s32 fileRequestDefaultMode(void);

s32 slEndingExit(void) {
    slSetInfo(0);
    D_00437D3C = 0;
    D_00437D1C = 8;
    D_00437CF8 = 0;
    return slQueryYesNo(&fileRequestDefaultMode, &slEndingTop, 1);
}

s32 slBootTop(void) {
    slSetInfo(0);
    D_00437CF8 = 0;
    return (s32)fileSlotSelectPoll;
}

s32 slBootRetry(void) {
    fileReqBegin(0);
    fileSlotSelectionPollCount = 30;
    D_00437CEC = 1;
    slSetInfo(1);
    D_00437CF8 = 1;
    return (s32)fileSlotSelectPollClear;
}

s32 slBootError(void) {
    D_00437CF4 = 1;
    slSetInfo(0);
    D_00437D1C = 5;
    return slQueryYesNo(&fileRequestDefaultMode, &slBootRetry, 1);
}

extern s32 fileRequestDefaultMode(void);

s32 slBootError2(void) {
    D_00437CF4 = 1;
    slSetInfo(0);
    D_00437D1C = 6;
    return slQueryYesNo(&fileRequestDefaultMode, &slBootRetry, 1);
}

s32 fileBeginSlotMetadataRefresh(void) {
    fileClearAllSlotFlags();
    slSetInfo(0);
    D_00437D3C = 1;
    D_00437D40 = 0;
    fileReqBegin(fileMemoryCardRequestIndex);
    return (s32)filePollSlotDetectionAndBranch;
}

s32 fileResetSelection(void) {
    fileClearAllSlotFlags();
    fileSlotDisplayStates[0] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[1] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[2] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[3] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[4] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[5] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[6] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[7] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[8] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileSlotDisplayStates[9] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
    fileReqBegin(0);
    fileSlotSelectionPollCount = 15;
    D_00437CEC = 1;
    slSetInfo(1);
    D_00437CF8 = 1;
    return (s32)fileSlotStatusPoll;
}
extern void dds3AdminiCallSequence(s32, const void *, u32, s32);
extern s32 fileMenuWorkStart(void);


s32 slExit(void) {
    slSetInfo(0);
    D_00437CF8 = 0;
    D_00437CF4 = 0;
    return 0xffffffff;
}

s32 mcdEnterDefaultFileFlow(void) {
    s32 mode = 0;
    if (mcdOriginalTitleFileMode != 0) {
        return (s32)fileMenuWorkStart;
    }
    dds3AdminiCallSequence(2, &mode, 4, 0);
    return 0;
}

s32 fileRequestDefaultMode(void) {
    return mcdEnterDefaultFileFlow();
}

s32 mcdEnterSelectedFileFlow(void) {
    s32 mode = 2;
    fileSelectionPending = 1;
    if (mcdOriginalTitleFileMode != 0) {
        return (s32)fileMenuWorkStart;
    }
    dds3AdminiCallSequence(2, &mode, 4, 0);
    return 0;
}

s32 fileRestartSlotScan(void) {
    fileClearAllSlotFlags();
    slSetInfo(1);
    D_00437D3C = 0;
    D_00437CF8 = 0;
    return fileResetSelection();
}

/* 0x40-byte backing region; the browser uses the first ten slot states. */
extern u32 fileSlotDisplayStates[16];
extern s32 fileSlotBrowserUpdate(void);

s32 fileScanSlotStates(void) {
    s32 i;

    D_00437D14 = 0;
    D_00437CF8 = 1;
    for (i = 0; i < 10; i++) {
        u32 buttons = fileReqGetSlotFlags(fileMemoryCardRequestIndex, i);

        fileSlotDisplayStates[i] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
        if (buttons & FILE_REQ_SLOT_CAN_READ_SAVE) {
            if (buttons & FILE_REQ_SLOT_DIRECTORY_READY) {
                if (buttons & FILE_REQ_SLOT_DELETE_BEFORE_WRITE) {
                    fileSlotDisplayStates[i] = FILE_SLOT_DISPLAY_SAVE_PRESENT;
                }
            }
        } else if (buttons & FILE_REQ_SLOT_DIRECTORY_READY) {
            if (!(buttons & FILE_REQ_SLOT_DELETE_BEFORE_WRITE)) {
                fileSlotDisplayStates[i] = FILE_SLOT_DISPLAY_DIRECTORY_WITHOUT_SAVE;
            }
        }
    }
    fileReqBegin(fileMemoryCardRequestIndex);
    return (s32)fileSlotBrowserUpdate;
}

s32 slCancel(void) {
    if (D_00437D30 == 1 && dds3SearchProcessName(D_0042B698) == NULL) {
        return mcdEnterSelectedFileFlow();
    }
    slSetInfo(0);
    return slExitWait(&slExit);
}

extern struct sdfMemHandle_t *fileSaveIconResourceHandle;
extern struct FileRequest *fileSaveIconRequest;
extern char D_0042B6A8[];

s32 fileLoadIconFileAndResetSelection(void) {
    fileSaveIconResourceHandle = 0;
    fileSaveIconRequest = fileLoad(D_0042B6A8);
    return fileResetSelection();
}

s32 fileResetSlotSelection(void) {
    return fileResetSelection();
}

s32 fileLoadIconFileAndBeginSlotReset(void) {
    fileSaveIconResourceHandle = 0;
    fileSaveIconRequest = fileLoad(D_0042B6A8);
    return slEndingTop();
}

s32 slBootProc1st(void) {
    fileReqBegin(0);
    D_00437CEC = 1;
    fileSlotSelectionPollCount = 0;
    slSetInfo(0);
    D_00437CF8 = 0;
    return slBootTop();
}

s32 fileSlotStatusPoll(void) {
    s32 status;

    if (fileReqPoll() == 0) {
        return 0;
    }
    if (fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestIndex);
        return 0;
    }
    status = fileReqGetStatus(fileMemoryCardRequestIndex);
    switch (status) {
    case 0:
        return fileBeginSlotMetadataRefresh();
    case 2:
        if (D_00437D30 == 0) {
            fileClearAllSlotFlags();
            slSetInfo(0);
            D_00437D3C = 0;
            D_00437D1C = 2;
            return slQueryYesNo(slFormatStart, slFormatCancel, 1);
        }
        slSetInfo(0);
        D_00437D3C = 7;
        D_00437D40 = 0;
        fileReqBegin(fileMemoryCardRequestIndex);
        return (s32)filePollSlotDetectionAndBranch;
    case 3:
        fileClearAllSlotFlags();
        slSetInfo(0);
        D_00437D3C = 2;
        D_00437D40 = 0;
        fileReqBegin(fileMemoryCardRequestIndex);
        return (s32)filePollSlotDetectionAndBranch;
    default:
        return 0;
    case 1:
        return mcResetSlotMetadata();
    }
}

s32 fileCountSelectableFiles(void) {
    s32 count = 0;
    s32 index;
    for (index = 0; index < 10; index++) {
        u32 flags = fileReqGetSlotFlags(fileMemoryCardRequestIndex, index);
        if (flags & FILE_REQ_SLOT_CAN_READ_SAVE) {
            if (flags & FILE_REQ_SLOT_DIRECTORY_READY) {
                if (flags & FILE_REQ_SLOT_DELETE_BEFORE_WRITE) {
                    count++;
                }
            }
        }
    }
    return count;
}

s32 slSizeChk(void) {
    return fileReqGetSize(fileMemoryCardRequestIndex) > 0x2A7FF;
}

extern s32 slBootError(void);
extern s32 fileRequestDefaultMode(void);
extern s32 slSizeChk(void);
extern s32 slBootRetry(void);

s32 fileSlotSelectPoll(void) {
    s32 result = fileReqPoll();

    if (result == 0) {
        return result;
    }
    if ((s32)fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestIndex);
        return 0;
    }
    switch (fileReqGetStatus(fileMemoryCardRequestIndex)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return (s32)fileRequestDefaultMode();
    case 1:
        if (slSizeChk() != 0) {
            return (s32)fileRequestDefaultMode();
        }
        return (s32)slBootRetry();
    default:
        return 0;
    }
}

extern s32 slBootError(void);
extern s32 fileRequestDefaultMode(void);
extern s32 slSizeChk(void);

s32 fileSlotSelectPollClear(void) {
    if (fileReqPoll() == 0) {
        return 0;
    }
    if ((s32)fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestIndex);
        return 0;
    }
    switch (fileReqGetStatus(fileMemoryCardRequestIndex)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return fileRequestDefaultMode();
    case 1:
        if (slSizeChk() != 0) {
            return fileRequestDefaultMode();
        }
        return mcClearSlotMetadata();
    default:
        return 0;
    }
}

s32 filePollSlotDetectionAndBranch(void) {
    s32 reqStatus;

    D_00437CF8 = 0;
    if (D_00437D40 == 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_00437D40 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_00437D40 == 1) {
            D_00437D3C = 0;
            D_00437CF8 = 0;
            if (D_00437D0C != 0) {
                return slEndingExit();
            }
            if (D_00437D30 == 0) {
                return slExitWait(&slExit);
            }
            return mcdEnterSelectedFileFlow();
        }
        reqStatus = fileReqGetStatus(fileMemoryCardRequestIndex);
        switch (reqStatus) {
        case 1:
            if (D_00437D3C == 1) {
                fileClearAllSlotFlags();
                slSetInfo(1);
                D_00437D3C = 0;
                D_00437CF8 = 0;
                return fileResetSelection();
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case 0:
            fileClearAllSlotFlags();
            if (D_00437D3C != 1) {
                D_00437D3C = 1;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case 2:
            if (D_00437D30 == 0) {
                fileClearAllSlotFlags();
                D_00437D3C = 0;
                D_00437D1C = 2;
                return slQueryYesNo(&slFormatStart, &slFormatCancel, 1);
            }
            if (D_00437D3C != 7) {
                fileClearAllSlotFlags();
                D_00437D3C = 7;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case 3:
            if (D_00437D3C != 2) {
                fileClearAllSlotFlags();
                D_00437D3C = 2;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        }
    }
    return 0;
}

s32 slErrorWaitToFileMc(void) {
    if (D_00437D40 == 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            D_00437D40 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_00437D40 == 1) {
            D_00437D3C = 0;
            return fileScanSlotStates();
        }
        if (fileReqGetStatus(fileMemoryCardRequestIndex) != 1) {
            D_00437D3C = 0;
            D_00437CF8 = 0;
            return fileResetSelection();
        }
        fileReqBegin(fileMemoryCardRequestIndex);
    }
    return 0;
}

s32 slErrorWait(void) {
    if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) ||
        fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0)) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        D_00437D3C = 0;
        D_00437CF8 = 0;
        return fileResetSelection();
    }
    return 0;
}

s32 slExitWaitProc(void) {
    s32 remaining = fileWaitTicksRemaining - 1;
    fileWaitTicksRemaining = remaining;
    if (remaining <= 0) {
        if (fileMenuContinuation == (FileMenuContinuationCallback)-1) {
            return -1;
        }
        return fileWaitContinuation();
    }
    return 0;
}
s32 fileSetMenuCallbackAndClearResult(FileMenuContinuationCallback menuCallback) {
    fileMenuContinuation = menuCallback;
    D_00437D40 = 0;
    return (s32)mcdFinishFileDetection;
}

s32 mcdFinishFileDetection(void) {
    if (fileIsLoadedAndConditionTrue((u32)dds3PadButtonState[0x21] >> 31) ||
        fileIsLoadedAndConditionTrue((u32)dds3PadButtonState[0x23] >> 31)) {
        slSetInfo(0);
        D_00437D3C = 0;
        D_00437CF8 = 0;
        if (mcdOriginalTitleFileMode == 0) {
            p2sdStopBGM(0x310000);
        }
        if (fileMenuContinuation == (FileMenuContinuationCallback)-1) {
            return slExitWait((FileMenuContinuationCallback)-1);
        }
        return fileMenuContinuation();
    }
    return 0;
}

s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback) {
    fileMenuContinuation = callback;
    D_00437D40 = 0;
    fileReqBegin(fileMemoryCardRequestIndex);
    return (s32)filePollSlotRequestAndResumeFlow;
}

s32 filePollSlotRequestAndResumeFlow(void) {
    s32 reqStatus;

    if (D_00437D40 == 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            D_00437D40 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_00437D40 == 1) {
            slSetInfo(0);
            D_00437D3C = 0;
            D_00437CF8 = 0;
            if (fileMenuContinuation == (FileMenuContinuationCallback)-1) {
                return slExitWait((FileMenuContinuationCallback)-1);
            }
            return fileMenuContinuation();
        }
        reqStatus = fileReqGetStatus(fileMemoryCardRequestIndex);
        switch (reqStatus) {
        case 1:
            if (D_00437D3C == 1) {
                fileClearAllSlotFlags();
                slSetInfo(1);
                D_00437D3C = 0;
                D_00437CF8 = 0;
                return fileResetSelection();
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case 0:
            return fileBeginSlotMetadataRefresh();
        case 2:
            if (D_00437D30 == 0) {
                fileClearAllSlotFlags();
                slSetInfo(0);
                D_00437D3C = 0;
                D_00437D1C = 2;
                return slQueryYesNo(&slFormatStart, &slFormatCancel, 1);
            }
            if (D_00437D3C != 7) {
                fileClearAllSlotFlags();
                slSetInfo(0);
                D_00437D3C = 7;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case 3:
            if (D_00437D3C != 2) {
                fileClearAllSlotFlags();
                slSetInfo(0);
                D_00437D3C = 2;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        }
    }
    return 0;
}

s32 fileBeginReadSlotIcon(void) {
    char buf[0x50];

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], fileSlotScanIndex);
    sdfMcFuncChdir(fileMemoryCardRequestIndex, (const char *)buf);
    return (s32)slChooseData_getCardInfo2;
}

s32 slChooseData_getCardInfo2(void) {
    McPollResult t = sdfMcFuncChdir1();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSlotScanIndex, FILE_REQ_SLOT_DIRECTORY_READY);
        sdfMcFuncGetDir(fileMemoryCardRequestIndex, D_0042B6B8, fileDirectoryEntryScratch, 1);
        return (s32)slChooseData_getCardInfo3;
    }
    if (t == MC_POLL_ERROR) {
        return fileBeginSlotMetadataRefresh();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfo3(void) {
    s32 value;
    McPollResult status = sdfMcFuncGetDir1(&value);
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (value == 1) {
            fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSlotScanIndex,
                                FILE_REQ_SLOT_CAN_READ_SAVE | FILE_REQ_SLOT_DELETE_BEFORE_WRITE);
        }
        return slHeadLoad();
    }
    if (status == MC_POLL_ERROR) {
        return fileBeginSlotMetadataRefresh();
    }
    return slHeadLoad();
}

s32 fileBeginReadRemainingSlotIcons(void) {
    s32 found = 0;
    s32 slot;
    u32 flags;

    fileSlotScanIndex++;
    if (fileSlotScanIndex == 10) {
        fileReqClearSlotMetadataDirty(fileMemoryCardRequestIndex);
        fileSlotScanIndex = 0;
        D_00437CEC = slot = 0;
        if (fileCountSelectableFiles() == 0) {
            if (D_00437D30 != 0) {
                D_00437CF8 = 0;
                slSetInfo(0);
                for (slot = 0; slot < 10; slot++) {
                    flags = fileReqGetSlotFlags(fileMemoryCardRequestIndex, slot);
                    if (flags & 2) {
                        if (!(flags & 8)) {
                            found = 1;
                        }
                    }
                }
                if (found) {
                    D_00437D3C = 9;
                } else {
                    D_00437D3C = 7;
                }
                D_00437D40 = 0;
                fileReqBegin(fileMemoryCardRequestIndex);
                return (s32)filePollSlotDetectionAndBranch;
            }
            if (slSizeChk() == 0) {
                slSetInfo(0);
                D_00437CF8 = 0;
                D_00437D3C = 5;
                D_00437D40 = 0;
                fileReqBegin(fileMemoryCardRequestIndex);
                return (s32)filePollSlotDetectionAndBranch;
            }
        }
        slSetInfo(0);
        return fileScanSlotStates();
    }
    return fileBeginReadSlotIcon();
}

s32 mcResetSlotMetadata(void) {
    s32 index;
    if (fileReqIsSlotMetadataDirty(fileMemoryCardRequestIndex) != 0) {
        slSetInfo(1);
        fileSlotScanIndex = 0;
        index = 0;
        do {
            fileSlotDisplayStates[index] = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
            fileReqClearSlotFlags(fileMemoryCardRequestIndex, index);
            index++;
        } while (index < 10);
        return fileBeginReadSlotIcon();
    } else {
        D_00437CEC = 0;
        slSetInfo(0);
        return fileScanSlotStates();
    }
}

s32 fileBeginSaveSlotIconScan(void) {
    char buf[0x50];

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], fileSlotScanIndex);
    sdfMcFuncChdir(fileMemoryCardRequestIndex, (const char *)buf);
    return (s32)slChooseData_getCardInfo2_B;
}

s32 slChooseData_getCardInfo2_B(void) {
    McPollResult t = sdfMcFuncChdir1();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSlotScanIndex, FILE_REQ_SLOT_DIRECTORY_READY);
        sdfMcFuncGetDir(fileMemoryCardRequestIndex, D_0042B6B8, fileDirectoryEntryScratch, 1);
        return (s32)slChooseData_getCardInfo3_B;
    }
    if (t == MC_POLL_ERROR) {
        slSetInfo(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return slBootError();
    }
    return fileScanSlotStatesAdvance();
}

s32 slChooseData_getCardInfo3_B(void) {
    s32 value;
    McPollResult status = sdfMcFuncGetDir1(&value);
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (value == 1) {
            fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSlotScanIndex,
                                FILE_REQ_SLOT_CAN_READ_SAVE | FILE_REQ_SLOT_DELETE_BEFORE_WRITE);
        }
        return fileScanSlotStatesAdvance();
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return slBootError();
    }
    return fileScanSlotStatesAdvance();
}

s32 fileScanSlotStatesAdvance(void) {
    s32 slot = fileSlotScanIndex;
    s32 buttons = fileReqGetSlotFlags(fileMemoryCardRequestIndex, slot);

    slot++;
    if (buttons & FILE_REQ_SLOT_CAN_READ_SAVE) {
        if (buttons & FILE_REQ_SLOT_DIRECTORY_READY) {
            if (buttons & FILE_REQ_SLOT_DELETE_BEFORE_WRITE) {
                slSetInfo(0);
                D_00437CF8 = 0;
                D_00437D3C = 0;
                return fileRequestDefaultMode();
            }
        }
    }
    fileSlotScanIndex = slot;
    if (slot == 10) {
        slSetInfo(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return slBootError2();
    }
    return fileBeginSaveSlotIconScan();
}

s32 mcClearSlotMetadata(void) {
    s32 index = 0;
    u32 *saved;
    slSetInfo(1);
    D_00437CF8 = 0;
    fileSlotScanIndex = 0;
    fileReqMarkSlotMetadataDirty(fileMemoryCardRequestIndex);
    saved = fileSlotDisplayStates;
    do {
        *saved++ = FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE;
        fileReqClearSlotFlags(fileMemoryCardRequestIndex, index);
        index++;
    } while (index < 10);
    return fileBeginSaveSlotIconScan();
}

s32 slSaveData(void) {
    char name[0x50];
    u32 entry = fileMemoryCardRequestIndex;
    s32 slot = fileReqGetSelectedSlot(entry);
    fileReqGetSlotFlags(entry, slot);
    slSetInfo(2);
    fileReqMarkSlotMetadataDirty(entry);
    if (fileReqGetSlotFlags(entry, slot) & FILE_REQ_SLOT_DIRECTORY_READY) {
        return slSaveData_chdir();
    }
    name[0] = '/';
    mcdFormatSaveSlotName(name + 1, slot);
    sdfMcFuncMkdir(entry, (const char *)name);
    return (s32)slSaveProc_Write_mkdir;
}

s32 slSaveData_chdir(void) {
    char buf[0x50];
    u32 entry = fileMemoryCardRequestIndex;
    s32 v = fileReqGetSelectedSlot(entry);

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], v);
    sdfMcFuncChdir(entry, (const char *)buf);
    return (s32)filePrepareMainBlobWrite;
}

s32 slSaveProc_Write_mkdir(void) {
    McPollResult status = sdfMcFuncMkdir1();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSelectedSlotIndex, FILE_REQ_SLOT_DIRECTORY_READY);
        return slSaveData_chdir();
    }
    slSetInfo(0);
    D_00437D3C = 4;
    return (s32)slErrorWait;
}

s32 filePrepareMainBlobWrite(void) {
    McPollResult t = sdfMcFuncChdir1();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        return slSaveData_deleteIconSys();
    }
    if (t == MC_POLL_ERROR) {
        slSetInfo(0);
        D_00437D3C = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 fileBuildMainBlobAfterDelete(void) {
    McPollResult t = sdfMcFuncDelete1();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        return fileBuildMainBlobAndWrite();
    }
    if (t == MC_POLL_ERROR) {
        slSetInfo(0);
        D_00437D3C = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 fileOnAllWritten(void) {
    fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSelectedSlotIndex, FILE_REQ_SLOT_CAN_READ_SAVE);
    fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSelectedSlotIndex, FILE_REQ_SLOT_DELETE_BEFORE_WRITE);
    slSetInfo(4);
    return fileBeginDetectionRequest(fileScanSlotStates);
}

s32 fileWriteIconSysComplete(void) {
    slKillLoadProcess();
    return (s32)fileOnAllWritten;
}

extern u8 fileSaveIconSysData[];
extern u32 D_00437D90;
extern u32 D_00437D94;

s32 fileBeginLabeledSlotWrite(void) {
    s32 v = fileSelectedSlotIndex + 1;
    s32 q = v / 10;
    s32 r = v % 10;

    fileSaveIconSysData[0xF8] = -0x7E;
    fileSaveIconSysData[0xFA] = -0x7E;
    fileSaveIconSysData[0xF9] = q + 0x4F;
    fileSaveIconSysData[0xFB] = r + 0x4F;
    return slSaveWriteFile(D_0042B6B8, &D_00437D90, &D_00437D94, fileWriteIconSysComplete, 0);
}

extern s32 slSaveWriteFile(const char *fileName, u32 *dataAddressSlot, u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest **pendingRequestStatus);
extern s32 fileBeginLabeledSlotWrite(void);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B698);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B6A8);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B6B8);

s32 slSaveWriteEnd_saveData(void) {
    return slSaveWriteFile("base.ico", &fileSaveIconData, &fileSaveIconDataSize, fileBeginLabeledSlotWrite, &fileSaveIconRequest);
}

extern DatGameState *fileMainSaveBlobAddress;
extern u32 fileMainSaveBlobSize;

s32 fileBuildMainBlobAndWrite(void) {
    char filename[0x50];
    FileSavePreviewRecord *header;
    s32 slot = fileReqGetSelectedSlot(fileMemoryCardRequestIndex);

    mcdFormatSaveSlotName(filename, slot);
    DDS3_GBWK->header.magic[0] = 'V';
    DDS3_GBWK->header.magic[1] = 'E';
    DDS3_GBWK->header.magic[2] = 'R';
    DDS3_GBWK->header.version = 3;
    DDS3_GBWK->header.mapGroup = DDS3_GBWK->vr.previewX;
    DDS3_GBWK->header.mapIndex = DDS3_GBWK->vr.previewY;
    DDS3_GBWK->header.playTicks = DDS3_GBWK->header.secondTick;
    DDS3_GBWK->header.partyIds[0] = DDS3_GBWK->party[0].unitId;
    DDS3_GBWK->header.partyIds[1] = DDS3_GBWK->party[1].unitId;
    DDS3_GBWK->header.partyIds[2] = DDS3_GBWK->party[2].unitId;
    DDS3_GBWK->header.partyIds[3] = DDS3_GBWK->party[3].unitId;
    DDS3_GBWK->header.partyIds[4] = DDS3_GBWK->party[4].unitId;
    DDS3_GBWK->header.partyIds[5] = 0;
    DDS3_GBWK->header.partyIds[6] = 0;
    DDS3_GBWK->header.partyIds[7] = 0;
    DDS3_GBWK->header.partyLevels[0] = DDS3_GBWK->party[0].level;
    DDS3_GBWK->header.partyLevels[1] = DDS3_GBWK->party[1].level;
    DDS3_GBWK->header.partyLevels[2] = DDS3_GBWK->party[2].level;
    DDS3_GBWK->header.partyLevels[3] = DDS3_GBWK->party[3].level;
    DDS3_GBWK->header.partyLevels[4] = DDS3_GBWK->party[4].level;
    DDS3_GBWK->header.partyLevels[5] = 0;
    DDS3_GBWK->header.partyLevels[6] = 0;
    DDS3_GBWK->header.partyLevels[7] = 0;

    header = (FileSavePreviewRecord *)DDS3_GBWK;
    fileSavePreviewRecords[slot].signature[0] = header->signature[0];
    fileSavePreviewRecords[slot].signature[1] = header->signature[1];
    fileSavePreviewRecords[slot].signature[2] = header->signature[2];
    fileSavePreviewRecords[slot].version = header->version;
    fileSavePreviewRecords[slot].mapGroup = header->mapGroup;
    fileSavePreviewRecords[slot].mapIndex = header->mapIndex;
    fileSavePreviewRecords[slot].playTicks = header->playTicks;
    fileSavePreviewRecords[slot].status = header->status;
    fileSavePreviewRecords[slot].newCycle = header->newCycle;
    memcpy(fileSavePreviewRecords[slot].party, header->party, 8);
    memcpy(fileSavePreviewRecords[slot].levels, header->levels, 8);
    fileSavePreviewRecords[slot].money = header->money;
    fileSavePreviewRecords[slot].saveWords.words[0] = header->saveWords.words[0];
    fileSavePreviewRecords[slot].saveWords.words[1] = header->saveWords.words[1];
    fileSavePreviewRecords[slot].saveWords.words[2] = header->saveWords.words[2];
    fileMainSaveBlobAddress = DDS3_GBWK;
    fileMainSaveBlobSize = sizeof(DatGameState);
    if (D_00437D08 == 0 || D_00437D08 == 2) {
        return slSaveWriteFile(filename, &fileMainSaveBlobAddress, &fileMainSaveBlobSize,
                                       slSaveWriteEnd_saveData, NULL);
    }
    return slSaveWriteFile(filename, &fileMainSaveBlobAddress, &fileMainSaveBlobSize,
                                   fileBeginLabeledSlotWrite, NULL);
}

s32 slSaveData_deleteIconSys(void) {
    u32 entry = fileMemoryCardRequestIndex;
    s32 slot = fileReqGetSelectedSlot(entry);
    u32 flags = fileReqGetSlotFlags(entry, slot);
    if (!(flags & FILE_REQ_SLOT_DELETE_BEFORE_WRITE)) {
        /* Build the main blob from the current global state. */
        return fileBuildMainBlobAndWrite();
    }
    sdfMcFuncDelete(entry, D_0042B6B8);
    return (s32)fileBuildMainBlobAfterDelete;
}

extern u32 *fileWriteDataAddressSlot;
extern u32 *fileWriteDataSizeSlot;
extern FileMenuContinuationCallback fileWriteContinuation;
extern struct FileRequest **fileWritePendingRequestSlot;
extern s32 slSaveProc_openFile(void);

s32 slSaveWriteFile(const char *fileName, u32 *dataAddressSlot, u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest **pendingRequestStatus) {
    fileWriteDataAddressSlot = dataAddressSlot;
    fileWriteDataSizeSlot = dataSizeSlot;
    fileWriteContinuation = continuation;
    fileWritePendingRequestSlot = pendingRequestStatus;
    sdfMcFuncOpen(fileMemoryCardRequestIndex, fileName, 0x203);
    return (s32)slSaveProc_openFile;
}

extern void sdfMcFuncWrite(s32 request, const void *data, u32 size);
extern s32 slSaveProc_writeFile(void);
extern s32 slSaveProc_waitDataReady(void);

s32 slSaveProc_openFile(void) {
    McPollResult status = sdfMcFuncOpen1(&fileSaveFileDescriptor);

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (fileWritePendingRequestSlot != 0 && *fileWritePendingRequestSlot != 0) {
            return (s32)slSaveProc_waitDataReady;
        }
        sdfMcFuncWrite(fileSaveFileDescriptor, (const void *)*fileWriteDataAddressSlot, *fileWriteDataSizeSlot);
        return (s32)slSaveProc_writeFile;
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        D_00437D3C = 4;
        return (s32)slErrorWait;
    }
    return 0;
}


extern s32 slSaveProc_closeFile(void);

s32 slSaveProc_writeFile(void) {
    McPollResult status = sdfMcFuncWrite1();

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncClose(fileSaveFileDescriptor);
        return (s32)slSaveProc_closeFile;
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        D_00437D3C = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

extern FileMenuContinuationCallback fileWriteContinuation;
s32 slSaveProc_closeFile(void) {
    McPollResult status = sdfMcFuncClose1();

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        return fileWriteContinuation();
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        D_00437D3C = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_waitDataReady(void) {
    if (fileSaveIconRequest != 0) {
        return 0;
    }
    sdfMcFuncWrite(fileSaveFileDescriptor, (const void *)*fileWriteDataAddressSlot, *fileWriteDataSizeSlot);
    return (s32)slSaveProc_writeFile;
}

s32 mcHandleDetectionResult(void) {
    McPollResult status = sdfMcFuncFormat1();

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    dds3SoftResetOn();
    if (status < 0) {
        slSetInfo(0);
        D_00437D3C = 6;
        return (s32)slErrorWait;
    }
    slSetInfo(6);
    return fileBeginDetectionRequest(fileRestartSlotScan);
}

s32 slFormatStart(void) {
    slSetInfo(5);
    dds3SoftResetOff();
    sdfMcFuncFormat(fileMemoryCardRequestIndex);
    return (s32)mcHandleDetectionResult;
}

s32 slFormatCancel(void) {
    if (D_00437D0C != 0) {
        D_00437D1C = 7;
        return slQueryYesNo(slEndingExit, fileResetSelection, 1);
    }
    D_00437D1C = 7;
    return slQueryYesNo(slExit, fileResetSelection, 1);
}

s32 slLoadData(void) {
    char path[0x50];
    u32 ctx = fileMemoryCardRequestIndex;
    s32 slot = fileReqGetSelectedSlot(ctx);
    u32 attr = fileReqGetSlotFlags(ctx, slot);
    u32 len;

    slSetInfo(3);
    if ((attr & FILE_REQ_SLOT_CAN_READ_SAVE) == 0) {
        return fileScanSlotStates();
    }
    fileReqGetSlotCode();
    path[0] = '/';
    mcdFormatSaveSlotName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = '/';
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return (s32)slLoadProc_openFile;
}

s32 slLoadProc_openFile(void) {
    McPollResult status = sdfMcFuncOpen1(&fileSaveFileDescriptor);
    u32 size;

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    size = fileMainBlobSize();
    fileSaveReadBufferResource = sdfMemAllocDirect(size);
    fileSaveReadBuffer = (void *)(u32)sdfMemLock(fileSaveReadBufferResource);
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncRead(fileSaveFileDescriptor, fileSaveReadBuffer, size);
        return (s32)slLoadProc_readFile;
    }
    slSetInfo(0);
    D_00437D3C = 3;
    return (s32)slErrorWait;
}

extern s32 mcdHandleSaveSetupDone(void);
s32 slLoadProc_readFile(void) {
    McPollResult status = sdfMcFuncRead1();

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncClose(fileSaveFileDescriptor);
        return (s32)mcdHandleSaveSetupDone;
    }
    sdfMemFreeDirect(fileSaveReadBufferResource);
    slSetInfo(0);
    D_00437D3C = 3;
    return (s32)slErrorWait;
}

extern s8 D_00437CD5;

void fileRestoreSlotFlagsToState(void);

s32 mcdHandleSaveSetupDone(void) {
    McPollResult status = sdfMcFuncClose1();

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        fileReloadSaveBuffer();
        sdfMemFreeDirect(fileSaveReadBufferResource);
        D_00437CD5 = 1;
        slKillLoadProcess();
        if (fileLoadStateChanged() == 0) {
            fileCacheSlotFlagsFromState();
        } else {
            fileRestoreSlotFlagsToState();
        }
        slSetInfo(13);
        return fileSetMenuCallbackAndClearResult((FileMenuContinuationCallback)-1);
    }
    sdfMemFreeDirect(fileSaveReadBufferResource);
    slSetInfo(0);
    D_00437D3C = 3;
    return (s32)slErrorWait;
}



extern s32 mcdContinueLoadSelection();

s32 mcdAdvanceToLoadSelection(void) {
    slSetInfo(13);
    return fileSetMenuCallbackAndClearResult(mcdContinueLoadSelection);
}

extern s32 D_00437D00;
extern void fileLoadSetMode(s8);
extern void fileSetMenuValueAndInitializeFlags(u32);
extern void fileCopyRecordHeader(FileSavePreviewRecord *, const void *);

s32 fileSlotBrowserUpdate(void) {
    s32 oldSelection = fileSelectedSlotIndex;
    s32 offset;
    s32 selection;
    s32 action;
    u32 flags;
    u32 slotState;

    D_00437D4C = 0;
    if (D_00437D14 == 0) {
        offset = oldSelection - D_00437D28;
        fileLoadMenuState.inputRepeatTimer++;
        fileLoadMenuState.inputRepeatTimer =
            fileLoadMenuState.inputRepeatTimer <= 0 ? 0 :
            fileLoadMenuState.inputRepeatTimer > 3 ? 3 : fileLoadMenuState.inputRepeatTimer;
        if (fileIsLoadedWithActiveFlow(((u8)dds3PadButtonState[0x26] >> 1) & 1) != 0 &&
            fileLoadMenuState.inputRepeatTimer >= 3) {
            selection = D_00437D28;
            fileLoadMenuState.inputRepeatTimer = 0;
            if (selection > 0) {
                if (offset == 2) {
                    offset = 1;
                } else if (offset == 1) {
                    D_00437D28 = selection - 1;
                    fileLoadSetMode(2);
                    fileSetMenuValueAndInitializeFlags(0x80);
                }
            } else {
                offset -= offset > 0;
            }
        }
        if (fileIsLoadedWithActiveFlow(((u8)dds3PadButtonState[0x27] >> 1) & 1) != 0 &&
            fileLoadMenuState.inputRepeatTimer >= 3) {
            selection = D_00437D28;
            fileLoadMenuState.inputRepeatTimer = 0;
            if (selection < 7) {
                if (offset == 0) {
                    offset = 1;
                } else if (offset == 1) {
                    D_00437D28 = selection + 1;
                    fileLoadSetMode(1);
                    fileSetMenuValueAndInitializeFlags(0x80);
                }
            } else {
                offset += offset < 2;
            }
        }
        fileSelectedSlotIndex = D_00437D28 + offset;
        if (oldSelection != fileSelectedSlotIndex) {
            p2sdPlaySE(0, 0x7F, 0x3F);
        }
        if (fileSelectedSlotIndex < oldSelection) {
            D_00437D00 = -8;
        } else if (oldSelection < fileSelectedSlotIndex) {
            D_00437D00 = 8;
        }
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
            D_00437D14 = 1;
        }
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            D_00437D14 = 2;
        }
    }
    if (fileReqPoll() != 0) {
        flags = fileReqGetStatus(fileMemoryCardRequestIndex);
        action = D_00437D14;
        if (flags == 0) {
            return fileBeginSlotMetadataRefresh();
        }
        if (action == 0) {
            fileReqBegin(fileMemoryCardRequestIndex);
            return 0;
        }
        if (action == 1) {
            slSetInfo(0);
            if (D_00437D30 == 0) {
                fileReqSetSelectedSlot(fileMemoryCardRequestIndex, fileSelectedSlotIndex);
                flags = fileReqGetSlotFlags(fileMemoryCardRequestIndex, fileSelectedSlotIndex);
                if ((flags & FILE_REQ_SLOT_SELECTABLE_SAVE_MASK) ==
                    FILE_REQ_SLOT_SELECTABLE_SAVE_MASK) {
                    p2sdPlaySE(8, 0x7F, 0x3F);
                    if (D_00437D0C != 0) {
                        D_00437D08 = action;
                        D_00437D1C = action;
                        return slQueryYesNo(slEndingUpd2, fileScanSlotStates, 1);
                    }
                    D_00437D08 = action;
                    D_00437D1C = action;
                    return slQueryYesNo(slSaveData, fileScanSlotStates, 1);
                }
                slotState = flags & FILE_REQ_SLOT_DIRECTORY_DELETE_MASK;
                if (slotState == FILE_REQ_SLOT_DIRECTORY_READY) {
                    p2sdPlaySE(8, 0x7F, 0x3F);
                    D_00437D08 = slotState;
                    return slSaveData();
                }
                p2sdPlaySE(8, 0x7F, 0x3F);
                D_00437D08 = 0;
                if (slSizeChk() != 0) {
                    return slSaveData();
                }
                slSetInfo(0);
                D_00437D3C = 5;
                D_00437D40 = 0;
                fileReqBegin(fileMemoryCardRequestIndex);
                return (s32)slErrorWaitToFileMc;
            }
            fileReqSetSelectedSlot(fileMemoryCardRequestIndex, fileSelectedSlotIndex);
            flags = fileReqGetSlotFlags(fileMemoryCardRequestIndex, fileSelectedSlotIndex);
            if ((flags & FILE_REQ_SLOT_CAN_READ_SAVE) != 0) {
                p2sdPlaySE(8, 0x7F, 0x3F);
                if (mcdOriginalTitleFileMode != 0 &&
                    fileSavePreviewRecords[fileSelectedSlotIndex].status == 0) {
                    slSetInfo(0);
                    D_00437D3C = 8;
                    D_00437D40 = 0;
                    fileReqBegin(fileMemoryCardRequestIndex);
                    return (s32)slErrorWaitToFileMc;
                }
                D_00437D1C = 3;
                if (mcdOriginalTitleFileMode == 0) {
                    return slQueryYesNo(slLoadData, fileScanSlotStates, 1);
                }
                fileCopyRecordHeader(&fileLoadSelectionWork->header,
                    &fileSavePreviewRecords[fileSelectedSlotIndex]);
                return slQueryYesNo(mcdAdvanceToLoadSelection, fileScanSlotStates, 1);
            }
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            return fileScanSlotStates();
        }
        if (action == 2) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            fileResetLoadContextSlide();
            if (D_00437D0C != 0) {
                return slEndingExit();
            }
            return slCancel();
        }
    }
    return 0;
}

/* Draw save slots and the selected preview; native scrolling snaps to its target. */
s32 fileDrawSlotListAndPreview(s32 work) {
    char text[64];
    s32 row;
    s32 rowBase;
    s32 rowCount;
    s32 slotBase;
    s32 slot;
    s32 y;
    u32 titleColor;
    u32 detailColor;
    u32 alpha;
    u32 baseColor;
    u32 partyColor;
    s32 trail;
    s32 trailAlpha;
    s32 partyIndex;
    s32 portrait;
    s32 ticks;
    s32 hours;
    s32 minutes;
    s32 seconds;
    FileSavePreviewRecord *preview;
    f32 targetY;
    f32 delta;
    f32 wave;

    if (D_00437CEC == 1) {
        return 0;
    }
    if (D_00437D00 > 0) {
        D_00437D00--;
    }
    if (D_00437D00 < 0) {
        D_00437D00++;
    }
    if (D_00437D30 == 0) {
        evtSubmitTexturedGradientQuad(0x56, 0x33, 0x137, 0x17, 5, 0x1C4, 0x137, 0x17, 0x80808080, 0x80808080, 0x80808080, 0x80808080, (struct sdfTexHandle_t *)(u32)(D_00437D74));
    } else {
        evtSubmitTexturedGradientQuad(0x56, 0x33, 0x137, 0x17, 5, 0x1DD, 0x137, 0x17, 0x80808080, 0x80808080, 0x80808080, 0x80808080, (struct sdfTexHandle_t *)(u32)(D_00437D74));
    }
    eglSetOt(0x55);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 2, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitDefaultDepthGradientRect(0, 0, 0x200, 0x1C0, 0, 0, 0, 0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglSetOt(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    /* Retail computes the clamped slide delta but snaps to the target. */
    targetY = D_00437D28 * -110 + 104;
    delta = targetY - D_00437D24;
    delta /= 3.0f;
    if (delta < 0.0f && delta > -16.0f) {
        delta = -16.0f;
    }
    if (delta > 0.0f && delta < 16.0f) {
        delta = 16.0f;
    }
    D_00437D24 = targetY;
    fileCursorStepUp();
    fileFadeStepDown();
    if (D_00437D38 == 0) {
        fileCursorPulseUpdate();
        fileDrawCursorPulse(0x25, 0x1F, 0x53);
    }
    fileLoadCtxSlideUpdate();
    if (fileLoadMenuState.slideMode == 0) {
        rowCount = 3;
        rowBase = D_00437D28;
        slotBase = D_00437D28;
    } else if (fileLoadMenuState.slideMode == 1) {
        rowCount = 4;
        rowBase = D_00437D28;
        slotBase = D_00437D28 - 1;
        if (slotBase < 0) {
            slotBase = 0;
        }
    } else {
        rowCount = 4;
        rowBase = D_00437D28 - 1;
        slotBase = D_00437D28;
    }
    for (row = 0; row < rowCount; row++) {
        slot = slotBase + row;
        y = (s32)D_00437D24 + (rowBase + row) * 110 + fileLoadMenuState.rowVerticalOffset;
        if (fileSelectedSlotIndex == slot) {
            titleColor = 0xA09DC380;
            detailColor = 0xA09DC380;
        } else {
            titleColor = 0x605D8340;
            detailColor = 0x605D8340;
        }
        if (mcdOriginalTitleFileMode != 0) {
            if (fileSavePreviewRecords[slot].status == 0) {
                titleColor = 0x605D8340;
                detailColor = 0x605D8340;
            } else {
                titleColor = 0xA09DC380;
                detailColor = 0xA09DC380;
            }
        }
        if (fileLoadMenuState.slideMode != 0) {
            if (row == 0) {
                alpha = fileLoadMenuState.leadingRowAlpha;
                titleColor = fileClampPackedColorByte(titleColor, alpha, 0);
                detailColor = fileClampPackedColorByte(detailColor, fileLoadMenuState.leadingRowAlpha, 0);
            } else if (row == 3) {
                alpha = fileLoadMenuState.trailingRowAlpha;
                titleColor = fileClampPackedColorByte(titleColor, alpha, 0);
                detailColor = fileClampPackedColorByte(detailColor, fileLoadMenuState.trailingRowAlpha, 0);
            } else {
                alpha = fileLoadMenuState.standardRowAlpha;
                titleColor = fileClampPackedColorByte(titleColor, alpha, 0);
                detailColor = fileClampPackedColorByte(detailColor, fileLoadMenuState.standardRowAlpha, 0);
            }
        } else {
            alpha = fileLoadMenuState.standardRowAlpha;
            titleColor = fileClampPackedColorByte(titleColor, alpha, 0);
            detailColor = fileClampPackedColorByte(detailColor, fileLoadMenuState.standardRowAlpha, 0);
        }
        baseColor = (alpha << 24) | 0x808080;
        evtSubmitTexturedGradientQuad(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, baseColor, baseColor, baseColor, baseColor, (struct sdfTexHandle_t *)(u32)(D_00437D5C));
        if (fileSlotDisplayStates[slot] == FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE) {
            evtSubmitTexturedGradientQuad(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, (struct sdfTexHandle_t *)(u32)(D_00437D64));
        } else if (fileSlotDisplayStates[slot] == FILE_SLOT_DISPLAY_DIRECTORY_WITHOUT_SAVE) {
            evtSubmitTexturedGradientQuad(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, (struct sdfTexHandle_t *)(u32)(D_00437D6C));
        } else if (fileSavePreviewRecords[slot].status > 0) {
            evtSubmitTexturedGradientQuad(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, (struct sdfTexHandle_t *)(u32)(D_00437D68));
        } else if (mcdOriginalTitleFileMode != 0 && fileSavePreviewRecords[slot].status == 0) {
            mcdCreateConfiguredDrawHandle(0x40, y + 0x20, detailColor, fileOriginalTitleUnusableSaveText);
        }
        if (slot != 10) {
            fileDrawSlotIcon(slot + 1, 0x5A, y + 4, alpha);
            if (fileSelectedSlotIndex == slot) {
                trailAlpha = 16;
                if (D_00437D00 > 0) {
                    for (trail = 0; trail < D_00437D00; trail++, trailAlpha -= 2) {
                        evtSubmitTexturedGradientQuad(0x1F, y - trail * 4, 0x1BB, trail * 4 + 16, 3, 3, 0x1BB, trail * 4 + 16, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (struct sdfTexHandle_t *)(u32)(D_00437D60));
                    }
                } else if (D_00437D00 < 0) {
                    trailAlpha = 32;
                    for (trail = 0; trail < -D_00437D00; trail++, trailAlpha -= 4) {
                        evtSubmitTexturedGradientQuad(0x1F, y + 0x58, 0x1BB, trail * 4 + 16, 3, 0x5B - trail * 4, 0x1BB, trail * 4 + 16, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (struct sdfTexHandle_t *)(u32)(D_00437D60));
                    }
                }
            }
            if (fileSlotDisplayStates[slot] != FILE_SLOT_DISPLAY_SAVE_PRESENT) {
                if (fileSelectedSlotIndex == slot) {
                    u32 highlightColor = ((D_003E8008[0] + 0x80) << 24) | 0x808080;
                    evtSubmitTexturedGradientQuad(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, highlightColor, highlightColor, highlightColor, highlightColor, (struct sdfTexHandle_t *)(u32)(D_00437D60));
                }
            } else {
                u32 bannerColor;

                if (mcdOriginalTitleFileMode != 0) {
                    if (fileSavePreviewRecords[slot].newCycle != 0) {
                        mcdCreateConfiguredDrawHandle(0x40, y + 0x20, titleColor, fileOriginalTitleNewCycleClearedGameLabel);
                    } else if (fileSavePreviewRecords[slot].status != 0) {
                        mcdCreateConfiguredDrawHandle(0x40, y + 0x20, titleColor, fileOriginalTitleClearedGameLabel);
                    }
                } else if (fileSavePreviewRecords[slot].newCycle != 0) {
                    mcdCreateConfiguredDrawHandle(0x40, y + 0x20, detailColor, fileNewCycleLabel);
                }
                bannerColor = (alpha << 24) | 0x808080;
                evtSubmitTexturedGradientQuad(0x88, y + 4, 0x137, 0x1F, 4, 0x1C5, 0x137, 0x1F, bannerColor, bannerColor, bannerColor, bannerColor, (struct sdfTexHandle_t *)(u32)(D_00437D78));
                preview = &fileSavePreviewRecords[slot];
                if (preview->newCycle == 0) {
                    if (mcdOriginalTitleFileMode == 0) {
                        if (preview->mapGroup == 0) {
                            mcdCreateConfiguredDrawHandle(0xF0, y, detailColor, D_003A41A8[preview->mapIndex]);
                        } else {
                            mcdCreateConfiguredDrawHandle(0xF0, y, detailColor, D_003A47E8[preview->mapIndex]);
                        }
                    } else {
                        if (preview->mapGroup == 0) {
                            mcdCreateConfiguredDrawHandle(0xF0, y, detailColor, D_00437D54[preview->mapIndex]);
                        } else {
                            mcdCreateConfiguredDrawHandle(0xF0, y, detailColor, D_00437D58[preview->mapIndex]);
                        }
                    }
                }
                ticks = fileSavePreviewRecords[slot].playTicks;
                if (ticks > 0x066FEBF8) {
                    ticks = 0x066FEBF8;
                }
                hours = ticks / 108000;
                minutes = (ticks % 108000) / 1800;
                seconds = (ticks % 1800) / 30;
                sprintf(text, D_00437D98, hours);
                fileDrawMenuImageAtPoint(0x187, y + 0x15, titleColor, text);
                sprintf(text, D_00437DA0, minutes);
                fileDrawMenuImageAtPoint(0x1A7, y + 0x15, titleColor, text);
                sprintf(text, D_00437DA0, seconds);
                fileDrawMenuImageAtPoint(0x1BD, y + 0x15, titleColor, text);
                if (fileSelectedSlotIndex == slot) {
                    u32 highlightColor = ((D_003E8008[0] + 0x80) << 24) | 0x808080;
                    evtSubmitTexturedGradientQuad(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, highlightColor, highlightColor, highlightColor, highlightColor, (struct sdfTexHandle_t *)(u32)(D_00437D60));
                }
                if (fileSavePreviewRecords[slot].newCycle == 0) {
                    if (mcdOriginalTitleFileMode != 0) {
                        if (fileSavePreviewRecords[slot].status != 0) {
                            partyColor = (alpha << 24) | 0x808080;
                        } else {
                            partyColor = (((alpha >> 1) + (alpha >> 2)) << 24) | 0x282828;
                        }
                    } else {
                        if (fileSelectedSlotIndex == slot) {
                            partyColor = (alpha << 24) | 0x808080;
                        } else {
                            partyColor = (((alpha >> 1) + (alpha >> 2)) << 24) | 0x282828;
                        }
                    }
                    for (partyIndex = 0; partyIndex < 5; partyIndex++) {
                        if (fileSavePreviewRecords[slot].party[partyIndex] != 0) {
                            portrait = 0;
                            switch (fileSavePreviewRecords[slot].party[partyIndex]) {
                            case 1:
            portrait = 0;
            break;
                            case 3:
            portrait = 1;
            break;
                            case 4:
            portrait = 2;
            break;
                            case 5:
            portrait = 3;
            break;
                            case 6:
            portrait = 4;
            break;
                            case 7:
            portrait = 5;
            break;
                            case 2:
            portrait = 6;
            break;
                            case 8:
            portrait = 7;
            break;
                            }
                            evtSubmitTexturedGradientQuad(partyIndex * 60 + 0x80, y + 0x23, 0x32, 0x3E, 1, 1, 0x32, 0x3E, partyColor, partyColor, partyColor, partyColor, (struct sdfTexHandle_t *)(u32)(fileSavePreviewPortraitTextures[portrait]));
                            sprintf(text, D_00437DA8, fileSavePreviewRecords[slot].levels[partyIndex]);
                            fileDrawMenuImageAtPoint(partyIndex * 60 + 0x98, y + 0x51, titleColor, text);
                        }
                    }
                }
                if (fileSavePreviewRecords[slot].saveWords.words[2] & 0x80000000) {
                    u32 badgeColor = (alpha << 24) | 0x808080;
                    evtSubmitTexturedGradientQuad(0x1B, y + 0x4C, 0x57, 0x23, 2, 0x5D, 0x57, 0x23, badgeColor, badgeColor, badgeColor, badgeColor, (struct sdfTexHandle_t *)(u32)(D_00437D70));
                }
            }
        }
    }
    evtSubmitPrimaryAlphaBlendMode(1);
    fileScrollArrowState.angle = (fileScrollArrowState.angle + 10) % 360;
    if (D_00437D28 > 0) {
        u32 arrowColor;

        evtSubmitTexturedGradientQuad(0xF8, 0x50, 0xF, 0x22, 0x35, 2, 0xF, 0x22, 0x80808080, 0x80808080, 0x80808080, 0x80808080, (struct sdfTexHandle_t *)(u32)(D_00437D70));
        wave = sdfSin((f32)((fileScrollArrowState.angle + 90) % 360) / 180.0f * 3.1415899f);
        fileScrollArrowState.upAlpha = (s32)((wave + 1.0f) * 0.5f * 176.0f + 16.0f);
        arrowColor = (fileScrollArrowState.upAlpha << 24) | 0x808080;
        evtSubmitTexturedGradientQuad(0xF8, 0x50, 0xF, 0x22, 0x35, 2, 0xF, 0x22, arrowColor, arrowColor, arrowColor, arrowColor, (struct sdfTexHandle_t *)(u32)(D_00437D70));
    }
    if (D_00437D28 < 7) {
        u32 arrowColor;

        evtSubmitTexturedGradientQuad(0xF8, 0x18B, 0xF, 0x22, 0x47, 2, 0xF, 0x22, 0x80808080, 0x80808080, 0x80808080, 0x80808080, (struct sdfTexHandle_t *)(u32)(D_00437D70));
        wave = sdfSin((f32)((fileScrollArrowState.angle + 90) % 360) / 180.0f * 3.1415899f);
        fileScrollArrowState.downAlpha = (s32)((wave + 1.0f) * 0.5f * 176.0f + 16.0f);
        arrowColor = (fileScrollArrowState.downAlpha << 24) | 0x808080;
        evtSubmitTexturedGradientQuad(0xF8, 0x18B, 0xF, 0x22, 0x47, 2, 0xF, 0x22, arrowColor, arrowColor, arrowColor, arrowColor, (struct sdfTexHandle_t *)(u32)(D_00437D70));
    }
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 1);
    eglSetOt(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}


s32 slCalc(dds3ProcessID_t *task) {
    s32 next;
    struct FileRequest *job;
    s32 (*cur)();
    D_00437CFC++;
    fileDrawMenuFrame((s32)task);
    next = fileMenuStateHandler(task);
    if (next == -1) {
        return next;
    }
    job = fileSaveIconRequest;
    cur = fileMenuStateHandler;
    if (next != 0) {
        cur = (s32 (*)())next;
    }
    fileMenuStateHandler = cur;
    if (job != 0 && pacFileCheckEnd(job) != 0) {
        fileSaveIconRequest = 0;
        fileSaveIconResourceHandle = fileGetMemHandle(job);
        fileSaveIconData = (u32)fileGetLoadPtr(job);
        fileSaveIconDataSize = fileGetLoadSize(job);
        fileRequestFreeIfComplete(job);
    }
    return 0;
}


s32 fileDrawMenuFrame(s32 task) {
    s32 fade;
    s32 alpha;

    eglSetOt(0x52);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (D_00437CF4 != 0) {
        evtSubmitTexturedGradientQuad(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      (struct sdfTexHandle_t *)(u32)(D_00437D74));
        evtSubmitTexturedGradientQuad(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      (struct sdfTexHandle_t *)(u32)(D_00437D78));
    }
    if (D_00437CF8 != 0) {
        fileDrawSlotListAndPreview(task);
    }
    fade = D_00437D4C;
    if (fade > 0) {
        if (fade < 4) {
            alpha = fade * 20;
        } else {
            alpha = 0x50;
        }
        evtSubmitTexturedGradientQuad(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, (alpha * 0x80 / 100 << 24) | 0x808080,
                      (alpha * 0x80 / 100 << 24) | 0x808080, (alpha * 0x80 / 100 << 24) | 0x808080,
                      (alpha * 0x80 / 100 << 24) | 0x808080, (struct sdfTexHandle_t *)(u32)(D_00437D74));
    }
    fileDrawModeSpecificMemoryCardPrompt();
    fileShowPromptDialog();
    fileDrawMemoryCardFlowPrompt();
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

void fileResetMenuFlowState(void) {
    slSetInfo(0);
    D_00437D04 = 0;
    fileWaitTicksRemaining = 0;
    D_00437D18 = 0xffffffff;
    fileSelectionPending = 0;
    D_00437D3C = 0;
    D_00437D40 = 0;
    D_00437CF8 = 0;
    D_00437D08 = 0;
    fileSlotSelectionPollCount = 0;
    D_00437CFC = 0;
    D_00437CEC = 0;
    D_00437D14 = 0;
    D_00437D1C = 0;
    D_00437D20 = 0;
    D_00437D44 = 0;
    D_00437D48 = 0;
    fileClearAllSlotFlags();
}

#include "file_pac.h"

extern void *memset(void *dst, s32 value, s32 size);

struct sdfMemHandle_t;
extern void dds3SoftResetOff(void);
extern s32 D_00437D80;
extern void fileDestroyMemoryCardMenuTask(dds3ProcessID_t *task);
extern void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

void fileInitCursorPulse(void);

void fileStartMemoryCardMenuForMode(s32 mode) {
    FilePacRequest *request;
    PacWork *node;
    u32 index = 0;
    struct sdfMemHandle_t *titleNamesAllocation;
    u32 titleNamesAddress;
    void *world;

    dds3SoftResetOff();
    request = (FilePacRequest *)pacLoad("/mc/mcpack.LB");
    pacSync((struct FileRequest *)request);
    node = request->packet.queueHead;
    while (node != NULL) {
        switch (index) {
        case 0:
            D_00437D5C = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 1:
            D_00437D60 = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 2:
            D_00437D64 = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 3:
            D_00437D68 = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 4:
            D_00437D6C = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 5:
            D_00437D70 = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 6:
            D_00437D74 = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 7:
            D_00437D78 = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 8:
            fileSavePreviewPortraitTextures[0] = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 9:
            fileSavePreviewPortraitTextures[1] = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 10:
            fileSavePreviewPortraitTextures[2] = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 11:
            fileSavePreviewPortraitTextures[3] = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 12:
            fileSavePreviewPortraitTextures[4] = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 13:
            fileSavePreviewPortraitTextures[5] = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 14:
            fileSavePreviewPortraitTextures[6] = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        case 15:
            fileSavePreviewPortraitTextures[7] = (s32)sdfTexSet((SdfTextureFileHeader *)sdfMemGetPtr(PAC_WORK_ALLOCATION(node)));
            break;
        }
        sdfMemFreeDirect(PAC_WORK_ALLOCATION(node));
        index++;
        node = node->next;
    }
    pacFileHandleFree_(request);
    D_00437D80 = 1;
    if (mode == 0) {
        D_00437CF4 = 1;
        D_00437D0C = 0;
        fileMenuStateHandler = (s32 (*)())fileLoadIconFileAndResetSelection;
        mcdOriginalTitleFileMode = 0;
    } else if (mode == 1) {
        D_00437D0C = 0;
        D_00437CF4 = 1;
        fileMenuStateHandler = (s32 (*)())fileResetSlotSelection;
        mcdOriginalTitleFileMode = 0;
    } else if (mode == 2) {
        D_00437CF4 = 1;
        mcdOriginalTitleFileMode = 0;
        D_00437D0C = 1;
        evtBitOn(0xBA0);
        DDS3_GBWK->header.status =
            (u16)DDS3_GBWK->header.status + 1;
        DDS3_GBWK->header.transition = 1;
        fileMenuStateHandler = (s32 (*)())fileLoadIconFileAndBeginSlotReset;
    } else {
        D_00437D0C = 0;
        if (mode == 4) {
            D_00437CF4 = 1;
            mcdOriginalTitleFileMode = 1;
            fileMenuStateHandler = (s32 (*)())fileMenuWorkStart;
            mode = 1;
        } else {
            mcdOriginalTitleFileMode = 0;
            D_00437CF4 = 0;
            fileMenuStateHandler = (s32 (*)())slBootProc1st;
        }
    }
    if (mcdOriginalTitleFileMode != 0) {
        titleNamesAllocation = sdfDevLoadFileEx("/mc/DDS1TN.DAT", &titleNamesAddress, NULL);
        D_00437D54 = sdfAllocAuto(0x3C0);
        D_00437D58 = sdfAllocAuto(0x780);
        memcpy(D_00437D54, (void *)titleNamesAddress, 0x3C0);
        memcpy(D_00437D58, (void *)(titleNamesAddress + 0x3C0), 0x780);
        sdfMemFreeDirect(titleNamesAllocation);
    }
    dds3StartProcess("FileMentCalc", 0x2B0A, 0, 0, slCalc, fileDestroyMemoryCardMenuTask, 0);
    D_00437CE4 = 1;
    fileInitCursorPulse();
    dds3FadeIn(0, 0, 0, 8);
    D_00437D30 = mode;
    fileResetMenuFlowState();
    world = dds3GetActiveScene();
    if (world != 0) {
        dds3VisibleSceneBasic((dds3Basic_t *)world, 0);
    }
}

void fileReleaseMenuResources(void);

void fileDestroyMemoryCardMenuTask(dds3ProcessID_t *task) {
    fileReleaseMenuResources();
}

void fileMenuReleaseNoOp(void) {
}

void fileReleaseMenuResources(void) {
    s32 *slot;
    s32 i;
    void *world;
    s32 handle;

    if (D_00437CE4 != 0) {
        D_00437CE4 = 0;
        slot = fileSavePreviewPortraitTextures;
        i = 7;
        do {
            handle = *slot;
            i--;
            if (handle != 0) {
                sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)handle);
                *slot = 0;
            }
            slot++;
        } while (i >= 0);
        if (D_00437D78 != 0) {
            sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)D_00437D78);
            D_00437D78 = 0;
        }
        if (D_00437D74 != 0) {
            sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)D_00437D74);
            D_00437D74 = 0;
        }
        if (D_00437D70 != 0) {
            sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)D_00437D70);
            D_00437D70 = 0;
        }
        if (D_00437D6C != 0) {
            sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)D_00437D6C);
            D_00437D6C = 0;
        }
        if (D_00437D68 != 0) {
            sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)D_00437D68);
            D_00437D68 = 0;
        }
        if (D_00437D64 != 0) {
            sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)D_00437D64);
            D_00437D64 = 0;
        }
        if (D_00437D60 != 0) {
            sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)D_00437D60);
            D_00437D60 = 0;
        }
        if (D_00437D5C != 0) {
            sdfTexReleaseReferenceViaHandler((sdfTexHandle_t *)D_00437D5C);
            D_00437D5C = 0;
        }
        if (D_00437D54 != 0) {
            sdfFreeAuto(D_00437D54);
            D_00437D54 = 0;
        }
        if (D_00437D58 != 0) {
            sdfFreeAuto(D_00437D58);
            D_00437D58 = 0;
        }
        world = dds3GetActiveScene();
        if (world != 0) {
            dds3VisibleSceneBasic((dds3Basic_t *)world, 1);
        }
        if (fileSaveIconRequest != 0) {
            fileSync(fileSaveIconRequest);
            fileSaveIconResourceHandle = fileGetMemHandle(fileSaveIconRequest);
            fileRequestFreeIfComplete(fileSaveIconRequest);
            fileSaveIconRequest = 0;
        }
        sdfMemFreeHandleDirect(&fileSaveIconResourceHandle);
        dds3KillProcessName("FileMentCalc", 1);
        dds3SoftResetOn();
    }
}

u32 fileMenuOperationSucceeded(void) {
    return 1;
}

s32 fileMenuTaskExists(void) {
    return dds3SearchProcessName("FileMentCalc") != NULL;
}

u32 fileGetSelectionPendingFlag(void) {
    return fileSelectionPending;
}

extern FileSelectionCallback D_00439024, D_00439028;
/* Yes/no cursor input while the card request is idle. */
static inline void filePollPromptChoice(void) {
    if (D_00437D20 == 0 && slChkWindowOpenDone() != 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x27] < 0) != 0) {
            if (D_00437D18 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 1;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x26] < 0) != 0) {
            if (D_00437D18 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 0;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
            if (D_00437D18 == 0 && (D_00437D1C == 1 || D_00437D1C == 3)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_00437D18 != 0) {
                D_00437D20 = 2;
            } else {
                D_00437D20 = 1;
            }
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_00437D20 = 2;
        }
    }
}

s32 fileUpdatePromptDialog(void) {
    s32 status;
    s32 decision;

    if (D_00437D1C - 5 < 2) {
        s32 requestStatus;
        filePollPromptChoice();
        if (fileReqPoll() == 0) {
            goto idle;
        }
        requestStatus = fileReqGetStatus(fileMemoryCardRequestIndex);
        switch (requestStatus) {
        case 1:
            decision = (s32)D_00437D20;
            if (decision == 1) {
                /* Retail 0x002CEACC beq: shared yes tail (.L002CEDE0). */
                goto yes;
            }
            if (decision >= 2) {
                /* Retail 0x002CEAD4 beqz: shared checkNo tail (.L002CEDC0). */
                goto checkNo;
            }
            if (decision != 0) {
                goto idle;
            }
            if (D_00437D1C == 5) {
                D_00437D1C = 0;
                D_00437D18 = -1;
                return slBootRetry();
            }
            /* Retail 0x002CEAEC bne: shared begin tail (.L002CEDD0). */
            goto begin;
        case 2:
            /* Retail 0x002CEAB8 beq: shared yes tail (.L002CEDE0). */
            goto yes;
        }
        requestStatus = (s32)D_00437D20;
        D_00437D1C = 5;
        switch (requestStatus) {
        case 0:
            /* Retail 0x002CEB28 beqz: shared begin tail (.L002CEDD0). */
            goto begin;
        case 1:
            /* Retail 0x002CEB14 beq: shared yes tail (.L002CEDE0). */
            goto yes;
        case 2:
            /* Retail 0x002CEB38 beq: shared no tail (.L002CEE00). */
            goto no;
        }
        return 0;
    }
    if (D_00437D1C == 4 || D_00437D1C == 8) {
        if (dds3PadButtonState[0x27] < 0) {
            if (D_00437D18 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 1;
        } else if (dds3PadButtonState[0x26] < 0) {
            if (D_00437D18 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 0;
        } else if (dds3PadButtonState[0x21] < 0) {
            if (D_00437D18 == 0 && (D_00437D1C == 1 || D_00437D1C == 3 || D_00437D1C == 9)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_00437D18 == 0) {
                /* Retail 0x002CEC20 b: enters the shared yes tail mid-block at .L002CEDE8;
                 * its delay slot (sw $0,D_00437D1C) is the tail's first store. */
                goto yes;
            }
            /* Retail 0x002CEC18 bnez: enters the shared no tail mid-block at .L002CEE04;
             * its delay slot (addiu $2,$0,-1) is the tail's first step. */
            goto no;
        } else if (dds3PadButtonState[0x23] < 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_00437D18 = -1;
            D_00437D1C = 0;
            return D_00439028();
        }
        goto idle;
    }
    filePollPromptChoice();
    if (fileReqPoll() == 0) {
        goto idle;
    }
    status = fileReqGetStatus(fileMemoryCardRequestIndex);
    switch (status) {
    case 1:
    case 2:
        decision = (s32)D_00437D20;
        if (decision == 1) {
            /* Retail 0x002CEDA0 beq: shared yes tail (.L002CEDE0). */
            goto yes;
        }
        if (decision >= 2) {
            /* Retail 0x002CEDA8 beqz: shared checkNo tail (.L002CEDC0). */
            goto checkNo;
        }
        if (decision == 0) {
            /* Retail 0x002CEDB0 beqz: shared begin tail (.L002CEDD0). */
            goto begin;
        }
        return 0;
    default:
        /* Retail 0x002CED88 beqz: shared restartRequest tail (.L002CEE20);
         * the blezl at 0x002CED90 enters mid-block at .L002CEE24 (its delay slot does the tail's first store). */
        goto restartRequest;
    }
checkNo:
    switch (decision) {
    case 2:
        /* Retail 0x002CEDC0 beq: first insn of the shared checkNo tail;
         * branches to the shared no tail (.L002CEE00). */
        goto no;
    default:
        return 0;
    }
begin:
    fileReqBegin(fileMemoryCardRequestIndex);
    goto idle;
yes:
    D_00437D1C = 0;
    D_00437D18 = -1;
    return D_00439024();
no:
    D_00437D1C = 0;
    D_00437D18 = -1;
    return D_00439028();
restartRequest:
    D_00437D1C = 0;
    D_00437D18 = -1;
    fileClearAllSlotFlags();
    slSetInfo(0);
    D_00437D3C = 1;
    D_00437D40 = 0;
    fileReqBegin(fileMemoryCardRequestIndex);
    return (s32)filePollSlotDetectionAndBranch;
    /* Retail `b` jumps (0x002CEB8C, 0x002CEBB8) reach one shared `return 0` block at 0x002CEE58 (twin of DDS1 0x0028FB30);
     * the beql 0x002CEA98 / 0x002CED70 and bnez 0x002CEADC enter at 0x002CEE5C with v0 already zeroed by their delay slots.
     * A plain `return 0` per site lets the compiler reuse v0 == 0 from the fileReqPoll test (26 of 322 words differ). */
idle:
    return 0;
}

extern s32 fileUpdatePromptDialog(void);

s32 slQueryYesNo(FileSelectionCallback start, FileSelectionCallback finish, s32 mode) {
    D_00439024 = start;
    D_00439028 = finish;
    D_00437D18 = (u32)mode;
    D_00437D20 = 0;
    if (D_00437D1C != 4 && D_00437D1C != 8) {
        fileReqBegin(fileMemoryCardRequestIndex);
    }
    return (s32)fileUpdatePromptDialog;
}

INCLUDE_ASM(const s32, "game/code_002C96D0", fileShowPromptDialog);

u32 fileGetLoadSelectionState(void) {
    return D_00437D04;
}

void slSetTmnlID(s32 x, s32 y) {
    DatGameState *state = DDS3_GBWK;

    state->vr.previewX = x;
    state->vr.previewY = y;
}

void fileSetMenuValueAndInitializeFlags(u32 value) {
    D_003E8008[0] = value;
    fileLoadMenuState.standardRowAlpha = 0x80;
    fileLoadMenuState.transitionTickCount = 0;
}

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */
/* Keep the input word separate from the per-mode result; see docs/idioms.md,
 * "Trace-justified result selection: packed byte clamp". */
u32 fileClampPackedColorByte(u32 color, s32 limit, s8 alpha) {
    u32 result;
    s32 component = color & 0xFF;

    if (alpha == 0) {
        if (limit >= component) {
            result = color;
        } else {
            result = (color & 0xFFFFFF00) | limit;
        }
    } else {
        component = color >> 24;
        if (limit >= component) {
            result = color;
        } else {
            result = (color & 0xFFFFFF) | ((u32)limit << 24);
        }
    }
    return result;
}

void fileCursorStepUp(void) {
    fileLoadMenuState.transitionTickCount += 1;
    fileLoadMenuState.transitionTickCount = fileLoadMenuState.transitionTickCount <= 0 ? 0 : fileLoadMenuState.transitionTickCount > 12 ? 12 : fileLoadMenuState.transitionTickCount;
    if (fileLoadMenuState.transitionTickCount >= 7) {
        fileLoadMenuState.standardRowAlpha += 0x30;
        fileLoadMenuState.standardRowAlpha = fileLoadMenuState.standardRowAlpha <= 0 ? 0 : fileLoadMenuState.standardRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.standardRowAlpha;
    }
}

void fileFadeStepDown(void) {
    D_003E8008[0] -= 0x10;
    D_003E8008[0] = D_003E8008[0] <= 0 ? 0 : D_003E8008[0] > 0x80 ? 0x80 : D_003E8008[0];
}

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B770);

void fileDrawSlotIcon(s32 index, s32 x, s32 y, s32 alpha) {
    s32 uv[21][2] = {
        {2, 2},   {2, 2},   {2, 20},  {2, 38},  {2, 56},  {2, 74},  {26, 2},
        {26, 20}, {26, 38}, {26, 56}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
        {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
    };

    eglSetOt(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    evtSubmitTexturedGradientQuad(x, y, 0x16, 0x10, uv[index][0], uv[index][1], 0x16, 0x10, (alpha << 24) | 0x808080,
                  (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, (struct sdfTexHandle_t *)(u32)(D_00437D70));
}

void fileCursorOffsetLookup(s32 dir, s32 step, s32 *outX, s32 *outY) {
    s32 offsets[3][5][2] = {
        {{0, 0}, {2, 1}, {4, 2}, {6, 4}, {8, 6}},
        {{0, 0}, {-2, 1}, {-4, 2}, {-6, 4}, {-8, 6}},
        {{0, 0}, {0, -2}, {0, -4}, {0, -6}, {0, -8}},
    };

    *outX = offsets[dir][step][0];
    *outY = offsets[dir][step][1];
}

/* Init record at fileCursorPulseState. */
typedef struct Init374E0 {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} Init374E0;

extern Init374E0 fileCursorPulseState;
extern s32 D_003E7FE4[];

void fileInitCursorPulse(void) {
    fileCursorPulseState.mode[1] = 0;
    D_003E7FE4[0] = 0x80;
    fileCursorPulseState.mode[0] = 1;
    fileCursorPulseState.mode[2] = 2;
    fileCursorPulseState.alpha[0] = 0x80;
    fileCursorPulseState.alpha[1] = 0x80;
    fileCursorPulseState.alpha[2] = 0x80;
}

void fileCursorPulseUpdate(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 mode = fileCursorPulseState.mode[i];

        if (mode < 3) {
            if (mode >= 0) {
                switch (mode) {
                case 0:
                    fileCursorPulseState.alpha[i] -= 12;
                    fileCursorPulseState.alpha[i] = fileCursorPulseState.alpha[i] <= 0x40 ? 0x40 : fileCursorPulseState.alpha[i] > 0x80 ? 0x80 : fileCursorPulseState.alpha[i];
                    break;
                case 1:
                    fileCursorPulseState.alpha[i] -= 28;
                    fileCursorPulseState.alpha[i] = fileCursorPulseState.alpha[i] <= 0 ? 0 : fileCursorPulseState.alpha[i] > 0x80 ? 0x80 : fileCursorPulseState.alpha[i];
                    break;
                }
                fileCursorPulseState.counter[i] += 1;
                fileCursorPulseState.counter[i] = fileCursorPulseState.counter[i] <= 0 ? 0 : fileCursorPulseState.counter[i] > 6 ? 6 : fileCursorPulseState.counter[i];
                if (fileCursorPulseState.counter[i] >= 6) {
                    fileCursorPulseState.mode[i]++;
                    fileCursorPulseState.alpha[i] = 0x80;
                    if (fileCursorPulseState.mode[i] >= 3) {
                        fileCursorPulseState.mode[i] = 0;
                    }
                    fileCursorPulseState.counter[i] = 0;
                }
                fileCursorOffsetLookup(i, 0, &fileCursorPulseState.pos[i][0], &fileCursorPulseState.pos[i][1]);
            }
        }
    }
}

extern const s32 D_0042B8F8[3][2];

void fileDrawCursorPulse(s32 x, s32 y, s32 surface) {
    s32 texturePositions[3][2] = {{54, 39}, {54, 67}, {54, 39}};
    s32 offsets[3][2];
    s32 i;

    memcpy(offsets, D_0042B8F8, sizeof(offsets));
    eglSetOt(surface);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    for (i = 0; i < 3; i++) {
        s32 mode = fileCursorPulseState.mode[i];

        if (mode < 2) {
            if (mode >= 0) {
                evtSubmitTexturedGradientQuad(x + fileCursorPulseState.pos[i][0] + offsets[i][0],
                             y + fileCursorPulseState.pos[i][1] + offsets[i][1],
                             27, 26,
                             texturePositions[mode][0], texturePositions[mode][1],
                             27, 26,
                             ((u32)fileCursorPulseState.alpha[i] << 24) | 0x808080,
                             ((u32)fileCursorPulseState.alpha[i] << 24) | 0x808080,
                             ((u32)fileCursorPulseState.alpha[i] << 24) | 0x808080,
                             ((u32)fileCursorPulseState.alpha[i] << 24) | 0x808080,
                             (struct sdfTexHandle_t *)(u32)(D_00437D70));
            }
        }
    }
}

void fileLoadSetMode(s8 mode) {
    fileLoadMenuState.rowVerticalOffset = 0;
    fileLoadMenuState.slideMode = mode;
    if (mode == 1) {
        fileLoadMenuState.trailingRowAlpha = 0;
        fileLoadMenuState.leadingRowAlpha = 0x74;
    } else {
        fileLoadMenuState.leadingRowAlpha = 0;
        fileLoadMenuState.trailingRowAlpha = 0x74;
    }
}

void fileResetLoadContextSlide(void) {
    fileLoadMenuState.rowVerticalOffset = 0;
    fileLoadMenuState.slideMode = 0;
    fileLoadMenuState.transitionTickCount = 0;
}

void fileLoadCtxSlideUpdate(void) {
    switch (fileLoadMenuState.slideMode) {
    case 1:
        fileLoadMenuState.rowVerticalOffset -= 0x14;
        fileLoadMenuState.rowVerticalOffset = fileLoadMenuState.rowVerticalOffset <= -0x6E ? -0x6E : fileLoadMenuState.rowVerticalOffset > 0 ? 0 : fileLoadMenuState.rowVerticalOffset;
        if (fileLoadMenuState.rowVerticalOffset <= -0x6E) {
            fileLoadMenuState.rowVerticalOffset = 0;
            fileLoadMenuState.slideMode = 0;
        }
        fileLoadMenuState.leadingRowAlpha -= 0x20;
        fileLoadMenuState.leadingRowAlpha = fileLoadMenuState.leadingRowAlpha <= 0 ? 0 : fileLoadMenuState.leadingRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.leadingRowAlpha;
        fileLoadMenuState.trailingRowAlpha += 0x10;
        fileLoadMenuState.trailingRowAlpha = fileLoadMenuState.trailingRowAlpha <= 0 ? 0 : fileLoadMenuState.trailingRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.trailingRowAlpha;
        break;
    case 2:
        fileLoadMenuState.rowVerticalOffset += 0x14;
        fileLoadMenuState.rowVerticalOffset = fileLoadMenuState.rowVerticalOffset <= 0 ? 0 : fileLoadMenuState.rowVerticalOffset > 0x6E ? 0x6E : fileLoadMenuState.rowVerticalOffset;
        if (fileLoadMenuState.rowVerticalOffset >= 0x6E) {
            fileLoadMenuState.rowVerticalOffset = 0;
            fileLoadMenuState.slideMode = 0;
        }
        fileLoadMenuState.leadingRowAlpha += 0x10;
        fileLoadMenuState.leadingRowAlpha = fileLoadMenuState.leadingRowAlpha <= 0 ? 0 : fileLoadMenuState.leadingRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.leadingRowAlpha;
        fileLoadMenuState.trailingRowAlpha -= 0x20;
        fileLoadMenuState.trailingRowAlpha = fileLoadMenuState.trailingRowAlpha <= 0 ? 0 : fileLoadMenuState.trailingRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.trailingRowAlpha;
        break;
    }
}

s32 mcdFinishFileDetectionWithAudio(void) {
    if (fileIsLoadedAndConditionTrue((u32)dds3PadButtonState[0x21] >> 31) ||
        fileIsLoadedAndConditionTrue((u32)dds3PadButtonState[0x23] >> 31)) {
        slSetInfo(0);
        p2sdPlaySE(8, 0x7f, 0x3f);
        return fileMenuContinuation();
    }
    return 0;
}

s32 fileCreateDetectionAudioCallback(FileMenuContinuationCallback callback) {
    fileMenuContinuation = callback;
    D_00437D40 = 0;
    return (s32)mcdFinishFileDetectionWithAudio;
}

extern FileSelectionCallback fileFourWayDialogCancelCallback;

s32 fileFourWayDialogPoll(void) {
    if (slChkWindowOpenDone() == 0) {
        return 0;
    }
    if (dds3PadButtonState[0x27] < 0) {
        if (D_00437D18 != 1) {
            p2sdPlaySE(0, 0x7f, 0x3f);
        }
        D_00437D18 = 1;
    } else if (dds3PadButtonState[0x26] < 0) {
        if (D_00437D18 != 0) {
            p2sdPlaySE(0, 0x7f, 0x3f);
        }
        D_00437D18 = 0;
    } else if (dds3PadButtonState[0x21] < 0) {
        p2sdPlaySE(8, 0x7f, 0x3f);
        if (D_00437D18 == 0) {
            return D_00439024();
        }
        return D_00439028();
    } else if (dds3PadButtonState[0x23] < 0) {
        if (fileFourWayDialogCancelCallback != 0) {
            p2sdPlaySE(0xA, 0x7f, 0x3f);
            return fileFourWayDialogCancelCallback();
        }
    }
    return 0;
}

extern s32 fileFourWayDialogPoll(void);

s32 fileBeginFourWayDialog(FileSelectionCallback ready, FileSelectionCallback completed,
                           FileSelectionCallback cancelled, u32 state) {
    D_00439024 = ready;
    D_00439028 = completed;
    fileFourWayDialogCancelCallback = cancelled;
    D_00437D18 = state;
    D_00437D20 = 0;
    return (s32)fileFourWayDialogPoll;
}

s32 fileWaitAfterLoadResult(void) {
    return 0;
}

#include "sdf_debug_api.h"

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B8F8);

s32 fileApplySelectedLoadResult(void) {
    s32 i;

    if (dds3DrawFlag & 2) {
        return 0;
    }
    slSetInfo(0);
    D_00437D18 = -1;
    D_00437D1C = 0;
    fldPrepareDeferredSceneTransition();
    if (fileLoadSelectionWork->applyLoadResult == 0) {
        return (s32)fileWaitAfterLoadResult;
    }
    if (fileLoadSelectionWork->startBranchFlag == 0) {
        evtBitOn(0xB8F);
        fileCopySaveHeaderNumbers(&fileLoadSelectionWork->header);
    } else {
        fileCopyRecordHeader(&fileLoadSelectionWork->header, &DDS3_GBWK->header);
    }
    fileApplyMenuFlagsToModel(fileLoadSelectionWork);
    filePrintAccessoryLoadNotice();
    fileSaveAndDisplayCurrentMoney();
    for (i = 0; i < 5; i++) {
        u16 activeFlag = DDS3_GBWK->party[i].flags & DAT_PARTY_FLAG_OCCUPIED;
        if (activeFlag && ptyApplyUnlockedUnitStatBonusesOnce(DDS3_GBWK->party[i].unitId)) {
            u32 maximumMp;
            u16 restoredHp;
            DDS3_GBWK->party[i].maxHp =
                datComputeSkillBoostedMaxHp(&DDS3_GBWK->party[i]);
            maximumMp = datComputeSkillBoostedMaxMp(&DDS3_GBWK->party[i]);
            restoredHp = DDS3_GBWK->party[i].maxHp;
            DDS3_GBWK->party[i].maxMp = maximumMp;
            DDS3_GBWK->party[i].hp = restoredHp;
            DDS3_GBWK->party[i].mp = maximumMp;
        }
    }
    if (fileLoadSelectionWork->hardInitSelected != 0) {
        DatGameState *state;
        u16 startBranchFlag;
        evtBitOn(0x80E);
        state = DDS3_GBWK;
        startBranchFlag = fileLoadSelectionWork->startBranchFlag;
        state->header.saveFlags2 |= 0x80000000;
        if (startBranchFlag) {
            mtrMantraEventBitReset();
            ptyResetPartyRecordsAndProfiles();
            printf("scd:hard init\n");
        }
    } else {
        evtBitOff(0x80E);
        DDS3_GBWK->header.saveFlags2 &= 0x7FFFFFFF;
    }
    if (evtBitCheck(0xB8F)) {
        evtBitOff(0x801);
        evtBitOn(0x81D);
        evtBitOn(0x805);
        evtBitOn(0x806);
    }
    return (s32)fileWaitAfterLoadResult;
}


s32 fileBeginFadeAndConfirmSound(void) {
    dds3FadeOut(0, 0, 0, 8);
    p2sdStopBGM(0x310000);
    return (s32)fileApplySelectedLoadResult;
}


s32 fileSelectLoadWithoutHardInit(void) {
    fileLoadSelectionWork->hardInitSelected = 0;
    return fileBeginFadeAndConfirmSound();
}

s32 fileSelectLoadWithHardInit(void) {
    fileLoadSelectionWork->hardInitSelected = 1;
    return fileBeginFadeAndConfirmSound();
}

s32 fileBeginLoadBranchDialog(void) {
    slSetInfo(0);
    D_00437D1C = 12;
    return fileBeginFourWayDialog(fileSelectLoadWithoutHardInit, fileSelectLoadWithHardInit, 0, 0);
}

s32 fileStartLoadDetectionAfterBranchDialog(void) {
    D_00437D18 = -1;
    D_00437D1C = 0;
    slSetInfo(0x18);
    fileLoadSelectionWork->applyLoadResult = 1;
    return fileCreateDetectionAudioCallback(fileBeginLoadBranchDialog);
}

s32 fileRestartSelectionFlow(void) {
    fileResetMenuFlowState();
    return (s32)fileResetSlotSelection;
}

s32 fileBeginLoadConfirmationDialog(void) {
    slSetInfo(0);
    D_00437D1C = 11;
    return fileBeginFourWayDialog(fileStartLoadDetectionAfterBranchDialog, fileRestartSelectionFlow, fileRestartSelectionFlow, 0);
}

extern s32 fileNextMenuFlowState(void);

s32 fileWaitForLoadStepBeforeDetection(void) {
    if (slChkWindowOpenDone() != 0) {
        return fileCreateDetectionAudioCallback(fileNextMenuFlowState);
    }
    return 0;
}

typedef struct FileFlowEntry {
    u16 state;
    u16 mask;
} FileFlowEntry;

extern FileFlowEntry fileMenuFlowStateEntries[];

s32 fileNextMenuFlowState(void) {
    while (fileLoadSelectionWork->flowRowCursor < 4) {
        MenuWork *work = fileLoadSelectionWork;
        u32 index = work->flowRowCursor;

        work->flowRowCursor = index + 1;
        if (fileMenuFlowStateEntries[index].mask & work->flowConditionMask) {
            slSetInfo(fileMenuFlowStateEntries[index].state);
            return (s32)fileWaitForLoadStepBeforeDetection;
        }
    }
    return (s32)fileBeginLoadConfirmationDialog;
}


s32 fileResetPendingRequest(void) {
    s32 mode;
    if (dds3DrawFlag & 2) {
        return 0;
    }
    D_00437D18 = -1;
    mode = 3;
    D_00437D1C = 0;
    dds3AdminiCallSequence(2, &mode, 4, 0);
    return 0;
}

s32 fileFadeBeforeResettingRequest(void) {
    dds3FadeOut(0, 0, 0, 8);
    return (s32)fileResetPendingRequest;
}

s32 fileBeginLoadOrAbortDialog(void) {
    slSetInfo(0);
    D_00437D1C = 10;
    fileLoadSelectionWork->applyLoadResult = 0;
    return fileBeginFourWayDialog(fileRestartSelectionFlow, fileBeginFadeAndConfirmSound, fileFadeBeforeResettingRequest, 0);
}

s32 mcdContinueLoadSelection(void) {
    u32 state = fileGetLoadSelectionState();
    MenuWork *block;
    s32 next = (s32)fileMenuWorkStart;
    if (state != 0) {
        if (state == 2) {
            block = fileLoadSelectionWork;
            block->flowRowCursor = 0;
            fileApplyMenuFlagsToModel(block);
            p2sdPlaySE(8, 0x7f, 0x3f);
            next = (s32)fileNextMenuFlowState;
        } else {
            next = 0;
        }
    }
    return next;
}

extern void dds3FadeIn2(s32);
extern s32 fileBeginLoadOrAbortDialog(void);

s32 fileMenuWorkStart(void) {
    fileSelectedSlotIndex = 0;
    D_00437D28 = 0;
    D_00437CF8 = 0;
    if (dds3FadeCheck()) {
        dds3FadeIn2(0x10);
    }
    if (fileLoadSelectionWork->startBranchFlag == 0) {
        return (s32)fileBeginLoadOrAbortDialog;
    }
    fileLoadSelectionWork->applyLoadResult = 1;
    return fileBeginLoadBranchDialog();
}


void fileMenuWorkCreate(u32 startBranchFlag) {
    struct sdfMemHandle_t *buffer = sdfMemAllocDirect(0x40);
    MenuWork *work;

    fileLoadSelectionWork = (MenuWork *)sdfMemLock(buffer);
    memset(fileLoadSelectionWork, 0, 0x40);
    work = fileLoadSelectionWork;
    work->applyLoadResult = 0;
    work->unk3C = buffer;
    work->startBranchFlag = startBranchFlag;
    work->unk38 = 0;
    fileLoadSelectionWork->hardInitSelected = 0;
}

void fileReleaseMenuFlowResource(void) {
    if (fileLoadSelectionWork != 0) {
        sdfMemFreeDelayedDirect(fileLoadSelectionWork->unk3C);
        fileLoadSelectionWork = NULL;
    }
}

extern char D_0042B920[];

void filePrintAccessoryLoadNotice(void) {
    printf(D_0042B920);
}

extern char D_0042B938[];

void fileSaveAndDisplayCurrentMoney(void) {
    DatGameState *state = DDS3_GBWK;
    u32 money = state->header.savedMoney;
    state->savedCurrency = money;
    printf(D_0042B938, money);
}

typedef struct FileMultiFlagBonus {
    s32 flags[4];
    u8 bonusIndex;
    u8 pad11[3];
} FileMultiFlagBonus;

extern FileMultiFlagBonus ptyUnit7StatBonusFlagGroups[6];
extern s8 (*D_00435E3C)[6];
extern s32 dds3FindEntryIndex(s32 unitId);
extern char D_0042B950[];

/* Apply each unlocked story bonus once to the active party record. */
s32 ptyApplyUnlockedUnitStatBonusesOnce(s32 unitId) {
    s32 partyIndex;
    datUnitWork_t *party;
    s32 changed;
    u32 row;
    s32 index;
    s32 stat;
    s8 *bonus;

    partyIndex = dds3FindEntryIndex(unitId);
    if (partyIndex < 0) {
        return 0;
    }
    party = &DDS3_GBWK->party[partyIndex];
    if (party->flags & 0x8000) {
        return 0;
    }
    printf(D_0042B950, unitId);
    changed = 0;
    party->flags |= 0x8000;
    if (unitId != 7) {
        for (row = 0; row < 24; row++) {
            if (ptyUnlockedUnitStatBonusRows[row].unitId == unitId) {
                if (evtBitCheck(ptyUnlockedUnitStatBonusRows[row].flag)) {
                    bonus = D_00435E3C[58 + ptyUnlockedUnitStatBonusRows[row].bonusIndex];
                    for (index = 0; index < DAT_BASE_STAT_COUNT; index++) {
                        party->baseStats[index] += bonus[index];
                    }
                    changed = 1;
                }
            }
        }
    } else {
        for (row = 0; row < 6; row++) {
            for (index = 0; index < 4; index++) {
                if (evtBitCheck(ptyUnit7StatBonusFlagGroups[row].flags[index])) {
                    bonus = D_00435E3C[58 + ptyUnit7StatBonusFlagGroups[row].bonusIndex];
                    for (stat = 0; stat < DAT_BASE_STAT_COUNT; stat++) {
                        party->baseStats[stat] += bonus[stat];
                    }
                    changed = 1;
                    break;
                }
            }
        }
    }
    return changed;
}


void fileCopySaveHeaderNumbers(FileSavePreviewRecord *source) {
    DatGameState *state = DDS3_GBWK;

    state->header.savedMoney = source->money;
    state->header.saveFlags = source->saveWords.words[0];
    state->header.saveFlags1 = source->saveWords.words[1];
    state->header.saveFlags2 = source->saveWords.words[2];
}

void fileCopyRecordHeader(FileSavePreviewRecord *destination, const void *source) {
    memcpy(destination, source, sizeof(*destination));
}

void fileApplyMenuFlagsToModel(MenuWork *work) {
    u32 i;

    for (i = 0; i < 0x4F; i++) {
        if (((s32)work->header.saveWords.words[i >> 5] >> (i & 0x1F)) & 1) {
            evtBitOn(0xB40 + i);
        } else {
            evtBitOff(0xB40 + i);
        }
    }
    for (i = 0; i < 5; i++) {
        if (evtBitCheck(fileRareItemNoticeFlagRows[i].flag)) {
            work->flowConditionMask |= 2;
            break;
        }
    }
    for (i = 0; i < 0x18; i++) {
        if (evtBitCheck(ptyUnlockedUnitStatBonusRows[i].flag)) {
            work->flowConditionMask |= 4;
            break;
        }
    }
    work->flowConditionMask |= 9;
}

s32 fileLoadStateChanged(void) {
    return fileSlotFlagMirror.current != fileSlotFlagMirror.previous;
}

void fileCacheSlotFlagsFromState(void) {
    fileSlotFlagMirror.current = fileSlotFlagMirror.previous =
        DDS3_GBWK->world.slotFlags;
}

void fileRestoreSlotFlagsToState(void) {
    DDS3_GBWK->world.slotFlags = fileSavedSlotFlags;
}

extern void dds3PadVibrationSet(s32, u8, s32);

extern s32 fileTestSlotFlagsBit(s32 kind, u32 *flags);

u8 fileToggleSlotFlagsBit(u32 kind, u32 *flags) {
    switch (kind) {
    case 0:
        *flags ^= 2;
        if (fileTestSlotFlagsBit(kind, flags) != 0) {
            dds3PadVibrationSet(0, 1, 0xF);
            dds3PadVibrationSet(1, 0x80, 0xF);
        } else {
            dds3PadVibrationSet(0, 0, 0xF);
            dds3PadVibrationSet(1, 0, 0xF);
        }
        return 1;
    case 1:
        *flags ^= 4;
        return 1;
    case 2:
        *flags ^= 8;
        return 1;
    case 3:
        *flags ^= 0x10;
        return 1;
    default:
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B920);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B938);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B950);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B970);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B980);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B998);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B9B0);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B9D0);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B9F0);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA10);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA30);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA50);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA70);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA90);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BAB0);

s32 fileTestSlotFlagsBit(kind, flags)
    s32 kind;
    u32 *flags;
{
    switch (kind) {
    case 0:
        return ((((s32)*flags >> 1) ^ 1) & 1);
    case 1:
        return *flags & 4;
    case 2:
        return *flags & 8;
    case 3:
        return ((((s32)*flags >> 4) ^ 1) & 1);
    case 6:
        return 1;
    default:
        return -1;
    }
}

s32 fileTestSavedSlotFlags(u32 kind) {
    return fileTestSlotFlagsBit(kind, &DDS3_GBWK->world.slotFlags);
}


extern FileConfigTask *GBWK;

extern struct MenuList *mnuCreateListState(s32, s32, s32);
extern void fileDrawConfigurationMenuEntry(s32 x, s32 y, s32 depth, struct MenuList *list, FileConfigListNode *node, s32 drawArg);
extern void mnuResetTitleStreamLocked(void);
extern void mnuResetTitleStreamAfterFileIdle(void);
extern void mnuQueueTitleSoundStreamEntry(s32);
extern void effRequestResourceByMode(const char *, const char *, s32, u32 *);
extern char D_00437DF0[];
extern char *fileConfigSpriteResourceNames[];

/* Allocate the save/config task, its five-row list and its effect resource slots. */
FileConfigTask *fileCreateConfigTask(s32 mode) {
    struct sdfMemHandle_t *block;
    FileConfigTask *task;
    struct MenuList *list;
    FileConfigListNode *node;
    s32 i;

    block = sdfMemAllocDirect(0x40);
    task = (FileConfigTask *)sdfMemLock(block);
    memset(task, 0, 0x40);
    task->backingAllocation = block;
    task->mode = mode;
    task->ticks = 0;
    task->result = 0;
    list = mnuCreateListState(0, 5, 0x23);
    task->frame = list;
    list->drawCallback = fileDrawConfigurationMenuEntry;
    for (i = 0; i < 5; i++) {
        node = (FileConfigListNode *)mnuListAppendNode(task->frame, NULL);
        node->resource = sdfChipAllocDirect(4);
        memset(node->resource, 0, 4);
    }
    switch (mode) {
    case 1:
        for (i = 0; i < 5; i++) {
            effRequestResourceByMode(D_00437DF0, fileConfigSpriteResourceNames[5 + i], 0, (u32 *)&task->slots[i]);
        }
        mnuResetTitleStreamLocked();
        mnuQueueTitleSoundStreamEntry(0x14);
        mnuResetTitleStreamAfterFileIdle();
        dds3FadeIn(0, 0, 0, 0xF);
        break;
    default:
        for (i = 0; i < 4; i++) {
            effRequestResourceByMode(D_00437DF0, fileConfigSpriteResourceNames[i], 0, (u32 *)&task->slots[i]);
        }
        task->slots[i] = 0;
        break;
    }
    return task;
}



void fileConfigTaskDestroy(dds3ProcessID_t *task) {
    s32 request = 1;
    FileConfigListNode *node;
    s32 i;

    if (GBWK != 0) {
        fileSavedSlotFlags = DDS3_GBWK->world.slotFlags;
        if (*(u32 *)((u32)GBWK + 4) == 1) {
            dds3AdminiCallSequence(2, &request, 4, 0);
            mnuAdvanceTitleStateUnderSemaphore();
        }
        node = (FileConfigListNode *)((FileConfigTask *)GBWK)->frame->first;
        for (i = 0; i < ((FileConfigTask *)GBWK)->frame->count; i++) {
            sdfChipFreeDirect(node->resource);
            node = node->next;
        }
        mnuDestroyListState(((FileConfigTask *)GBWK)->frame);
        ((FileConfigTask *)GBWK)->frame = 0;
        for (i = 0; i < 5; i++) {
            if (((FileConfigTask *)GBWK)->slots[i] != 0) {
                effDestroyResourceSlotSet(((FileConfigTask *)GBWK)->slots[i]);
                ((FileConfigTask *)GBWK)->slots[i] = 0;
            }
        }
        sdfMemFreeDirect(((FileConfigTask *)GBWK)->backingAllocation);
        GBWK = 0;
        dds3ConfigState = 0;
    }
}

extern FileConfigTask *fileCreateConfigTask(s32 mode);

extern s32 fileStartQueuedLoad(dds3ProcessID_t *task);
extern void fileConfigTaskDestroy(dds3ProcessID_t *task);
extern s32 fileUpdateConfigurationMenu(dds3ProcessID_t *task);
extern s32 fileUpdateConfigOwnerTask(dds3ProcessID_t *task);

extern s8 dds3ConfigState;

void dds3ConfigProcessStart(s32 mode) {
    if (GBWK == 0) {
        GBWK = fileCreateConfigTask(mode);
        dds3StartProcess(fileConfigInputTaskName, 0x3F2, 1, 1, fileUpdateConfigurationMenu, NULL, (u32)GBWK);
        dds3StartProcess(fileConfigLoadTaskName, 0x2B07, 1, 1, fileStartQueuedLoad, NULL, (u32)GBWK);
        dds3StartProcess(fileConfigOwnerTaskName, 0x520B, 1, 1, fileUpdateConfigOwnerTask, fileConfigTaskDestroy, (u32)GBWK);
        dds3ConfigState = 1;
    }
}

void dds3ConfigProcessEnd(void) {
    dds3KillProcessName(fileConfigInputTaskName, 1);
    dds3KillProcessName(fileConfigLoadTaskName, 1);
    dds3KillProcessName(fileConfigOwnerTaskName, 1);
}

s32 checkConfigState(void) {
    s32 state = dds3ConfigState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        dds3ConfigState = 0;
    }
    return 0;
}

u32 fileGetConfigTaskSlot(s32 slot) {
    if (slot < 4) {
        return (u32)GBWK->slots[slot];
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C96D0", fileConfigLoadTaskName);

INCLUDE_RODATA(const s32, "game/code_002C96D0", fileConfigOwnerTaskName);

s32 fileUpdateConfigurationMenu(dds3ProcessID_t *task) {
    s32 oldIndex;
    s32 index;
    s32 i;
    s32 ticks;
    f32 fade;
    FileConfigListNode *cursor;
    FileConfigTask *loadingWork;

    if (GBWK->transitionTicks != 0) {
        GBWK->transitionTicks--;
    }
    switch (GBWK->result) {
    case 0:
        if (GBWK->pending == 0) {
            GBWK->pending = 1;
            for (i = 0; i < 4; i++) {
                loadingWork = GBWK;
                if (loadingWork->slots[i] == 0) {
                    loadingWork->pending = 0;
                    return 0;
                }
            }
        }
        GBWK->result = 1;
        /* fallthrough */
    case 1:
        ticks = GBWK->ticks;
        GBWK->ticks = ticks + 1;
        fade = (f32)ticks / 15.0f;
        GBWK->choiceFade = GBWK->labelFade = fade;
        if (GBWK->ticks >= 16) {
            GBWK->result = 2;
            GBWK->ticks = 0;
            GBWK->choiceFade = GBWK->labelFade = 1.0f;
        }
        break;
    case 2:
        GBWK->ticks++;
        if (GBWK->ticks >= 121) {
            GBWK->ticks = 0;
        }
        break;
    case 3:
        if (GBWK->mode == 1) {
            dds3FadeOut(0, 0, 0, 15);
        }
        GBWK->ticks = 0;
        GBWK->result = 4;
        /* fallthrough */
    case 4:
        if (GBWK->ticks >= 6) {
            GBWK->labelFade = 0.0f;
        } else {
            GBWK->labelFade = 1.0f - (f32)GBWK->ticks / 5.0f;
        }
        ticks = GBWK->ticks;
        GBWK->ticks = ticks + 1;
        GBWK->choiceFade = 1.0f - (f32)ticks / 15.0f;
        if (GBWK->ticks >= 16) {
            GBWK->result = 5;
            GBWK->ticks = 0;
            GBWK->labelFade = 0.0f;
            GBWK->choiceFade = 0.0f;
        }
        return 0;
    case 5:
        GBWK->result |= 0x80000000;
        return -1;
    }

    oldIndex = GBWK->frame->cursor->index;
    if ((u8)dds3PadButtonState[0x26] & 2) {
        if (mnuRetreatListCursorDefault(GBWK->frame) != NULL) {
            p2sdPlaySE(0, 0x7F, 0x3F);
            GBWK->transitionTicks = 8;
            GBWK->previousIndex = oldIndex;
        }
    }
    if ((u8)dds3PadButtonState[0x27] & 2) {
        if (mnuAdvanceListCursorDefault(GBWK->frame) != NULL) {
            p2sdPlaySE(0, 0x7F, 0x3F);
            GBWK->previousIndex = oldIndex;
            GBWK->transitionTicks = 8;
        }
    }
    index = GBWK->frame->cursor->index;
    if (index < 4) {
        if (dds3PadButtonState[0x24] < 0 && fileTestSlotFlagsBit(index, &DDS3_GBWK->world.slotFlags) == 0) {
            fileToggleSlotFlagsBit(GBWK->frame->cursor->index, &DDS3_GBWK->world.slotFlags);
            p2sdPlaySE(8, 0x7F, 0x3F);
            cursor = (FileConfigListNode *)GBWK->frame->cursor;
            cursor->resource->ticks = 8;
        }
        if (dds3PadButtonState[0x25] < 0) {
            if (fileTestSlotFlagsBit(GBWK->frame->cursor->index, &DDS3_GBWK->world.slotFlags) != 0) {
                fileToggleSlotFlagsBit(GBWK->frame->cursor->index, &DDS3_GBWK->world.slotFlags);
                p2sdPlaySE(8, 0x7F, 0x3F);
                cursor = (FileConfigListNode *)GBWK->frame->cursor;
                cursor->resource->ticks = 8;
            }
        }
    }
    if (dds3PadButtonState[0x26] == 0 && dds3PadButtonState[0x27] == 0) {
        mnuClearListFlagsOneAndTwo((u32 *)GBWK->frame);
    }
    if (dds3PadButtonState[0x21] < 0) {
        if (GBWK->frame->cursor->index == 4) {
            GBWK->result = 3;
            p2sdPlaySE(8, 0x7F, 0x3F);
        }
    } else if (dds3PadButtonState[0x23] < 0) {
        GBWK->result = 3;
        fileRestoreSlotFlagsToState();
        p2sdPlaySE(10, 0x7F, 0x3F);
    }
    return 0;
}

extern void fileDrawConfigurationMenuBackground(void *);

s32 fileStartQueuedLoad(dds3ProcessID_t *task) {
    if (GBWK->pending == 0) {
        return 0;
    }
    if (GBWK->result < 0) {
        return -1;
    }
    fileDrawConfigurationMenuBackground((void *)GBWK);
    mnuCallInitWide(0x400, 0x400, 0, (u32)GBWK->frame, 0x53);
    return 0;
}

s32 fileUpdateConfigOwnerTask(dds3ProcessID_t *task) {
    s32 result;

    result = -1;
    if ((GBWK->result & 0x80000000) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileMemoryCardRequestIndex);

INCLUDE_SDATA(const s32, "game/code_002C96D0", slLoadKillFlag);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CD5);

INCLUDE_SDATA(dds3ProcessID_t * const, "game/code_002C96D0", D_00437CD8);

INCLUDE_SDATA(struct FileRequest * const, "game/code_002C96D0", fileSaveIconRequest);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CE4);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileWaitTicksRemaining);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CEC);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSlotSelectionPollCount);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CF4);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CF8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CFC);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D00);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D04);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D08);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D0C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSlotScanIndex);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D14);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D18);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D1C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D20);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D24);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D28);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSelectedSlotIndex);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D30);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSelectionPending);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D38);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D3C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D40);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D44);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D48);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D4C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D50);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D54);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D58);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D5C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D60);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D64);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D68);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D6C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D70);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D74);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D78);

INCLUDE_SDATA(const s32, "game/code_002C96D0", mcdOriginalTitleFileMode);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D80);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileLoadSelectionWork);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D88);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSaveHighlightPhase);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D90);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D94);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D98);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DA0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DA8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DB0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DB8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DC0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DD0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DD8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DE0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", dds3ConfigState);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSlotFlagMirror);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSavedSlotFlags);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DF0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileConfigInputTaskName);

void fileDrawConfigurationMenuEntry(s32 x, s32 y, s32 depth, struct MenuList *list,
                   FileConfigListNode *node, s32 drawArg) {
    s8 labelFrames[5] = {0, 1, 2, 3, 4};
    s32 index = node->index;
    FileConfigCountdown *countdown;
    s32 entryIndex;
    f32 pulse;
    f32 choiceFade = 1.0f;
    f32 labelFade;

    pulse = 0.0f;
    if (GBWK->result != 1) {
        pulse = (f32)(GBWK->ticks % 120) / 60.0f;
        if (pulse > choiceFade) {
            pulse = 2.0f - pulse;
        }
    }
    countdown = node->resource;
    choiceFade = GBWK->choiceFade;
    labelFade = GBWK->labelFade;
    if (countdown->ticks != 0) {
        --countdown->ticks;
    }
    if (list->cursor->index == node->index) {
        effDrawTextureSlotWithBlendedCornerColors(0x80, (index * 35 + 99) << 3, 0,
                     (u32)((pulse * 204.79998779296875f + 102.399993896484375f) * labelFade),
                     0, GBWK->slots[1], 18, 0x53);
        effDrawTextureSlotWithBlendedCornerColors(0x1830, (index * 35 + 99) << 3, 0,
                     (u32)((pulse * 204.79998779296875f + 102.399993896484375f) * labelFade),
                     0, GBWK->slots[1], 19, 0x53);
        effDrawTextureSlotWithBlendedCornerColors(0x8B0, 0x8C0, 0, (u32)(labelFade * 256.0f),
                     0, GBWK->slots[3], labelFrames[index], 0x53);
        entryIndex = index * 2;
        effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[entryIndex][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[entryIndex][FILE_CONFIG_Y] - 30) << 3, 0,
                     (u32)(choiceFade * 256.0f), 0,
                     GBWK->slots[fileConfigSpritePlacements[entryIndex][FILE_CONFIG_SET]],
                     fileConfigSpritePlacements[entryIndex][FILE_CONFIG_FRAME], 0x53);
        if (GBWK->transitionTicks != 0) {
            ++entryIndex;
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[entryIndex][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[entryIndex][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)((f32)GBWK->transitionTicks * 0.125f * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[entryIndex][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[entryIndex][FILE_CONFIG_FRAME], 0x53);
        }
    } else {
        entryIndex = index + 18;
        effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[entryIndex][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[entryIndex][FILE_CONFIG_Y] - 30) << 3, 0,
                     (u32)(labelFade * 256.0f), 0,
                     GBWK->slots[fileConfigSpritePlacements[entryIndex][FILE_CONFIG_SET]],
                     fileConfigSpritePlacements[entryIndex][FILE_CONFIG_FRAME], 0x53);
        entryIndex = index * 2;
        if (GBWK->previousIndex == node->index) {
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[entryIndex][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[entryIndex][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)((f32)GBWK->transitionTicks * 0.125f * 128.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[entryIndex][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[entryIndex][FILE_CONFIG_FRAME], 0x53);
        }
    }
    if (index < 3) {
        if (fileTestSlotFlagsBit(index, &DDS3_GBWK->world.slotFlags) != 0) {
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[10][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[10][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[10][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[10][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[11][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[11][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[11][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[11][FILE_CONFIG_FRAME], 0x53);
            }
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[25][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[25][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[25][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[25][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[14][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[14][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[14][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[14][FILE_CONFIG_FRAME], 0x53);
            }
        } else {
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[23][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[23][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[23][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[23][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[10][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[10][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[10][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[10][FILE_CONFIG_FRAME], 0x53);
            }
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[14][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[14][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[14][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[14][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[15][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[15][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[15][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[15][FILE_CONFIG_FRAME], 0x53);
            }
        }
    } else if (index == 3) {
        if (fileTestSlotFlagsBit(3, &DDS3_GBWK->world.slotFlags) != 0) {
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[12][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[12][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[12][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[12][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[13][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[13][FILE_CONFIG_Y] - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[13][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[13][FILE_CONFIG_FRAME], 0x53);
            }
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[26][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[26][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[26][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[26][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[16][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[16][FILE_CONFIG_Y] - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[16][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[16][FILE_CONFIG_FRAME], 0x53);
            }
        } else {
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[24][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[24][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[24][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[24][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[12][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[12][FILE_CONFIG_Y] - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[12][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[12][FILE_CONFIG_FRAME], 0x53);
            }
            effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[16][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[16][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[16][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[16][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawTextureSlotWithBlendedCornerColors(fileConfigSpritePlacements[17][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[17][FILE_CONFIG_Y] - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[17][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[17][FILE_CONFIG_FRAME], 0x53);
            }
        }
    }
}


void fileDrawConfigurationMenuBackground(void *unusedTask) {
    s16 sprites[5][4] = {
        {0, 2, 99, -57},
        {0, 3, -31, 365},
        {0, 5, 0, 120},
        {0, 4, 0, 120},
        {0, 6, -20, -20}
    };
    f32 labelFade = GBWK->labelFade;
    f32 choiceFade = GBWK->choiceFade;
    s32 spriteIndex;
    s32 row;

    spriteIndex = 0;
    effDrawTextureSlotWithBlendedCornerColors(sprites[spriteIndex][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[spriteIndex][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    spriteIndex++;
    effDrawTextureSlotWithBlendedCornerColors(sprites[spriteIndex][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[spriteIndex][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    if (GBWK->mode == 1) {
        effDrawTextureSlotWithBlendedCornerColors(0, 0, 0, 256, 0,
                     GBWK->slots[4], 0, 0x53);
    }
    spriteIndex++;
    effDrawTextureSlotWithBlendedCornerColors(sprites[spriteIndex][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[spriteIndex][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    spriteIndex++;
    effDrawTextureSlotWithBlendedCornerColors(sprites[spriteIndex][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[spriteIndex][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(choiceFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    spriteIndex++;
    effDrawTextureSlotWithBlendedCornerColors(sprites[spriteIndex][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[spriteIndex][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    for (row = 0; row < 5; row++) {
        effDrawTextureSlotWithBlendedCornerColors(0, (131 + row * 35 - 30) * 8, 0, (u32)(labelFade * 256.0f), 0,
                     GBWK->slots[2], 9, 0x53);
        effDrawTextureSlotWithBlendedCornerColors(461 * 16, (131 + row * 35 - 30) * 8, 0, (u32)(labelFade * 256.0f), 0,
                     GBWK->slots[2], 10, 0x53);
    }
    effDrawTextureSlotWithBlendedCornerColors(73 * 16, (326 - 30) * 8, 0, (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[2], 11, 0x53);
}


void fileResetRenderFlags(void);

void effLoadCommonTexturesAndResetRenderFlags(void) {
    effLoadFlashTextures();
    effLoadWindTexture();
    effLoadScalyTexture();
    fileResetRenderFlags();
}

void fileSetRenderFlag(u32 mask) {
    effModelUpdateControlFlags = effModelUpdateControlFlags | mask;
}

u32 fileClearRenderFlag(u32 mask) {
    u32 flags = effModelUpdateControlFlags & ~mask;

    effModelUpdateControlFlags = flags;
    return flags;
}

void fileResetRenderFlags(void) {
    effModelUpdateControlFlags = 0;
}

SdfPoolNode *effGetPacketSubmissionSurface(s32 index) {
    return effPacketSubmissionSurfaces[index];
}

void mnuProjectViewPoint(void) {
    u8 *matrix;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    matrix = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)matrix);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    matrix += 0x40;
    SDF_VU0_LOADVECTOR_B(matrix);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_0037F660);
    SDF_VU0_ADDVECTOR();
}

extern u8 sdfViewTargetVector[];

/* vu0 routine: tests whether vf10 projects within the camera's forward cone. */
s32 sdfProjectPointInForwardConeVU(void) {
    f32 distance, facing;
    f32 *projection;

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    if (!(distance > 16.0))
        return 0;

    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    VU0_LOAD_MATRIX(sdfViewMatrix);
    SDF_VU0_COPYMATRIX_A_TO_B();
    ReverseMatrix();
    SDF_VU0_GET_ROW2();
    SDF_VU0_NORMALIZE();
    SDF_VU0_INNERPRODUCT(facing);
    if (facing <= 0.25f)
        return 0;

    SDF_VU0_COPYVECTOR_C_TO_A();
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
    projection = (f32 *)sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)projection);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    projection += 16;
    SDF_VU0_LOADVECTOR_B(projection);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_0037F660);
    SDF_VU0_ADDVECTOR();
    return 1;
}

extern u8 aViewDown[];

/* vu0 routine: distance between the projected view point and a second point offset perpendicular to
   the camera axis by rate; 0 when either projection (sdfProjectPointInForwardConeVU) fails. Point comes in vf10. */
f32 mnuMeasureProjectedPerpendicularDistance(f32 rate) {
    f32 scale[4];
    f32 second[4];
    f32 first[4];
    f32 origin[4];
    f32 dx;
    f32 dy;

    SDF_VU0_STOREVECTOR(origin);
    if (sdfProjectPointInForwardConeVU() == 0) {
        return 0.0f;
    }
    SDF_VU0_STOREVECTOR(first);
    scale[0] = scale[1] = scale[2] = rate;
    SDF_VU0_LOADVECTOR(origin);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
    if (sdfProjectPointInForwardConeVU() == 0) {
        return 0.0f;
    }
    SDF_VU0_STOREVECTOR(second);
    dx = second[0] - first[0];
    dy = second[1] - first[1];
    SDF_VU0_LOADVECTOR(first);
    return fsqrtf(dx * dx + dy * dy);
}


/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void vuCatmullRomPoint(f32 (*p)[4], f32 t)
{
    f32 tan[2][4];
    f32 half[4];
    f32 h[4][4];
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    VEC3_SPLAT(half, 0.5f);
    SDF_VU0_LOADVECTOR(p[1]);
    SDF_VU0_LOADVECTOR_B(p[0]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(p[2]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(half);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(tan[0]);
    SDF_VU0_LOADVECTOR(p[2]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(p[3]);
    SDF_VU0_LOADVECTOR_B(p[2]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(half);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(tan[1]);
    VEC3_SPLAT(h[0], 2.0f * t3 - 3.0f * t2 + 1.0f);
    VEC3_SPLAT(h[1], t3 - 2.0f * t2 + t);
    VEC3_SPLAT(h[2], t3 - t2);
    VEC3_SPLAT(h[3], -2.0f * t3 + 3.0f * t2);
    SDF_VU0_LOADVECTOR(h[0]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(h[1]);
    SDF_VU0_LOADVECTOR_B(tan[0]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_C_A();
    SDF_VU0_LOADVECTOR(h[2]);
    SDF_VU0_LOADVECTOR_B(tan[1]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_C_A();
    SDF_VU0_LOADVECTOR(h[3]);
    SDF_VU0_LOADVECTOR_B(p[2]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
}

extern u8 sdfViewTargetVector[];
extern u8 aViewTarget[];
extern u8 aViewDown[];

/* vu0 routine: camera basis rows vf28-vf31 from eye sdfViewTargetVector, target aViewTarget and up aViewDown */
void vuBuildLookAtBasis(void)
{
    SDF_VU0_LOADVECTOR(sdfViewTargetVector);
    SDF_VU0_SET_ROW3();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW2();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW0();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_GET_ROW2();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_SET_ROW1();
}

/* The quaternion result is returned in vf10 for the VU transform routines. */
void sdfVuMatrixToQuaternion(const f32 matrix[4][4]) {
    f32 quaternion[4];
    f32 trace = matrix[0][0] + matrix[1][1] + matrix[2][2] + matrix[3][3];
    f32 scale;
    s32 i;
    s32 j;
    s32 k;

    if (trace >= 1.0f) {
        scale = 2.0f * fsqrtf(trace);
        quaternion[3] = -scale * 0.25f;
        quaternion[0] = (matrix[1][2] - matrix[2][1]) / scale;
        quaternion[1] = (matrix[2][0] - matrix[0][2]) / scale;
        quaternion[2] = (matrix[0][1] - matrix[1][0]) / scale;
    } else {
        i = matrix[0][0] > matrix[1][1] ? 0 : 1;
        if (matrix[i][i] < matrix[2][2]) {
            i = 2;
        }
        j = (i + 1) % 3;
        k = (j + 1) % 3;
        trace = matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f;
        if (trace != 0.0f) {
            scale = 2.0f * fsqrtf(trace);
            quaternion[i] = scale * 0.25f;
            quaternion[j] = (matrix[j][i] + matrix[i][j]) / scale;
            quaternion[k] = (matrix[k][i] + matrix[i][k]) / scale;
            quaternion[3] = -(matrix[j][k] - matrix[k][j]) / scale;
        } else {
            quaternion[i] = 1.0f;
            quaternion[j] = 0.0f;
            quaternion[k] = 0.0f;
            quaternion[3] = 0.0f;
        }
    }
    SDF_VU0_LOADVECTOR(quaternion);
}

FileJobPayload *fileCreateJob(FileJobType type) {
    FileJobType kind = type;
    FileJobPayload *job = sdfChipAllocDirect(0x2C);
    memset(job, 0, 0x2C);
    job->unk0 = 200;
    job->type = kind;
    return job;
}

void *fileResolvePrimaryBuffer(FileJobPayload *job) {
    if (job->primary.allocation != NULL) {
        return (void *)job->primary.offset;
    }
    if (job->primary.offset != 0) {
        return (u8 *)job + job->primary.offset;
    }
    return NULL;
}

void *fileResolveSecondaryBuffer(FileJobPayload *job) {
    if (job->secondary.allocation != NULL) {
        return (void *)job->secondary.offset;
    }
    if (job->secondary.offset != 0) {
        return (u8 *)job + job->secondary.offset;
    }
    return NULL;
}

FileJobPayload *fileJobCreateFromJob(FileJobPayload *request) {
    FileJobPayload *job = fileCreateJob(request->type);
    job->option = request->option;
    job->primary.selector = request->primary.selector;
    job->data = fileJobTypeOperations[job->type].create(request);
    return job;
}

void fileJobDestroy(FileJobPayload *job) {
    void *data = job->data;
    if (data != NULL) {
        fileJobTypeOperations[job->type].destroy(data);
    }
    fileJobFreePrimaryBuffer(job);
    fileJobFreeSecondaryBuffer(job);
    sdfChipFreeDirect(job);
}

void fileJobFreePrimaryBuffer(FileJobPayload *job) {
    struct sdfMemHandle_t *allocation = job->primary.allocation;
    if (allocation != NULL) {
        sdfMemFreeDirect(allocation);
        job->primary.offset = 0;
        job->primary.size = 0;
        job->primary.allocation = NULL;
    }
}

void fileJobFreeSecondaryBuffer(FileJobPayload *job) {
    struct sdfMemHandle_t *allocation = job->secondary.allocation;
    if (allocation != NULL) {
        sdfMemFreeDirect(allocation);
        job->secondary.offset = 0;
        job->secondary.size = 0;
        job->secondary.allocation = NULL;
    }
}

FileJobPayload *fileJobCreateChild(FileJobPayload *request) {
    FileJobPayload *job = fileCreateJob(request->type);
    FileTypeCallbacks *cb = &fileJobTypeOperations[job->type];

    job->option = request->option;
    job->primary.selector = request->primary.selector;
    job->data = cb->createChild(request->data, job->type);
    return job;
}

void fileJobNotifyPair(FileJobPayload *left, FileJobPayload *right) {
    void (*cb)(void *, void *) = fileJobTypeOperations[right->type].notifyPair;

    if (cb != NULL) {
        cb(left->data, right->data);
    }
}

void fileJobNotifyComplete(FileJobPayload *job) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].reset != NULL) {
        fileJobTypeOperations[id].reset(job->data);
    }
}

void fileJobInvokeTypeCallback(FileJobPayload *job) {
    u16 idx = job->type;
    void *data = job->data;

    fileJobTypeOperations[idx].invoke(data);
}

void fileJobInvokePositionCallback(FileJobPayload *job, f32 position[4]) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setPosition != NULL) {
        fileJobTypeOperations[id].setPosition(job->data, position);
    }
}

void fileJobInvokeRotationCallback(FileJobPayload *job, f32 rotation[4]) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setRotation != NULL) {
        fileJobTypeOperations[id].setRotation(job->data, rotation);
    }
}

void fileJobInvokeScaleCallback(FileJobPayload *job, f32 scale) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setScale != NULL) {
        fileJobTypeOperations[id].setScale(job->data, scale);
    }
}

void fileDispatchJobTypeCallback(FileJobPayload *job, u32 color) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setColor != NULL) {
        fileJobTypeOperations[id].setColor(job->data, color);
    }
}

void fileJobSetPrimaryData(FileJobPayload *job, const void *src, s32 size, u16 option) {
    fileJobFreePrimaryBuffer(job);
    if (src != NULL && size > 0) {
        job->primary.allocation = sdfMemAllocDirect(size);
        job->primary.offset = sdfMemLock(job->primary.allocation);
        job->primary.size = size;
        job->option = option;
        memcpy((void *)job->primary.offset, src, size);
    }
}

void fileJobCopyCommandIntoPrimaryData(FileJobPayload *job, const char *commandPath, u16 option) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;

    command = sdfFileOpen(commandPath);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        fileJobSetPrimaryData(job, buffer, size, option);
        sdfMemFreeDirect(allocation);
        return;
    }
}

void fileJobSetSecondaryData(FileJobPayload *job, const void *src, s32 size, u16 selector) {
    fileJobFreeSecondaryBuffer(job);
    if (src != NULL && size > 0) {
        job->secondary.allocation = sdfMemAllocDirect(size);
        job->secondary.offset = sdfMemLock(job->secondary.allocation);
        job->secondary.size = size;
        job->primary.selector = selector;
        memcpy((void *)job->secondary.offset, src, size);
    }
}

void fileJobCopyCommandIntoSecondaryData(FileJobPayload *job, const char *commandPath, u16 selector) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;

    command = sdfFileOpen(commandPath);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        fileJobSetSecondaryData(job, buffer, size, selector);
        sdfMemFreeDirect(allocation);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C96D0", fileJobWriteSerializedPayload);

void fileWriteToPfs(FileJobPayload *job, const char *filePath) {
    char path[0xD0];
    s32 fd;

    if (sdfHddValid != 0) {
        sprintf(path, D_00437E10, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, D_00437E18, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    fileJobWriteSerializedPayload(fd, job);
    sceClose(fd);
    sceSync(D_00437E20, 0);
}

FileJobPayload *fileDuplicateJob(FileJobPayload *request) {
    FileJobPayload *job = fileCreateJob(request->type);

    if (request->primary.size != 0) {
        fileJobSetPrimaryData(job, fileResolvePrimaryBuffer(request), request->primary.size, request->option);
    }
    if (request->secondary.size != 0) {
        fileJobSetSecondaryData(job, fileResolveSecondaryBuffer(request), request->secondary.size, request->primary.selector);
    }
    return job;
}

/* No return on the path where no command state exists: retail hands back
 * whatever v0 held. */
FileJobPayload *fileJobCreateFromCommandState(const char *entry) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;
    FileJobPayload *job;

    command = sdfFileOpen(entry);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        job = fileDuplicateJob((FileJobPayload *)buffer);
        sdfMemFreeDirect(allocation);
        return job;
    }
}

u32 fileJobSerializedSize(FileJobPayload *job) {
    u32 size = 0x2C;

    if (fileResolvePrimaryBuffer(job) != NULL) {
        size = job->primary.size + 0x2C;
    }
    if (fileResolveSecondaryBuffer(job) != NULL) {
        u32 aligned = size >> 4;
        if ((size & 0xF) != 0) {
            aligned = (aligned + 1) << 4;
        } else {
            aligned = aligned << 4;
        }
        size = aligned + job->secondary.size;
    }
    return size;
}

/* Initialize the shared transform prefix, leaving queue/job links untouched. */
void fileQueueInitTransform(void *queue)
{
    FileQueue *view = queue;

    memset(view, 0, 0x80);
    VU0_STORE_VF_UNCLOBBERED(vf0, view);
    VU0_STORE_VF_UNCLOBBERED(vf0, view->axis);
    VU0_STORE_VF_UNCLOBBERED(vf0, view->position);
    view->position[1] = -5.0f;
    VU0_STORE_VF_UNCLOBBERED(vf0, view->quat);
    view->scale = 1.0f;
    view->scaleMultiplier = 1.0f;
    view->color = 0x80808080;
    view->transformFlags = 0x80;
}

void fileJobResetAndInitTransform(FileJob *job) {
    memset(job, 0, 0x90);
    job->updateFlags = FILE_JOB_UPDATE_FLAG_APPLY_QUEUE_SCALE;
    job->unk88[0] = 8;
    job->unk88[1] = 0;
    job->unk88[2] = 0;
    fileQueueInitTransform(job);
}

void fileQueueAppend(FileQueue *queue, FileJob *job) {
    job->next = NULL;
    if (queue->last != NULL) {
        queue->last->next = job;
        job->prev = queue->last;
    } else {
        queue->first = job;
        job->prev = NULL;
    }
    queue->last = job;
    queue->count++;
}

void fileQueueInsertAfter(FileQueue *queue, FileJob *after, FileJob *job) {
    if (after->next != NULL) {
        after->next->prev = job;
        job->next = after->next;
    } else {
        job->next = NULL;
        queue->last = job;
    }
    after->next = job;
    job->prev = after;
    queue->count++;
}

void fileQueueRemove(FileQueue *queue, FileJob *job) {
    if (job->prev != NULL) {
        job->prev->next = job->next;
    } else {
        queue->first = job->next;
    }
    if (job->next != NULL) {
        job->next->prev = job->prev;
    } else {
        queue->last = job->prev;
    }
    queue->count--;
}

FileQueue *fileQueueCreate(void) {
    FileQueue *queue = sdfChipAllocDirect(0x90);
    memset(queue, 0, 0x90);
    queue->count = 0;
    queue->updateFrame = 0;
    fileQueueInitTransform(queue);
    return queue;
}

FileJob *fileJobCreate(void) {
    FileJob *job = sdfChipAllocDirect(0xC0);
    memset(job, 0, 0xC0);
    fileJobResetAndInitTransform(job);
    return job;
}

void fileDestroyJob(FileJob *job) {
    sdfChipFreeDirect(job);
}

FileQueue *fileCloneQueueEntries(FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    FileJob *entry;
    FileJob *job;
    f32 vec[4] __attribute__((aligned(16)));

    SDF_VU0_COPYVECTOR(queue->offset, source->offset);
    SDF_VU0_COPYVECTOR(queue->axis, source->offset);
    queue->scaleMultiplier = source->scaleMultiplier;
    queue->transformFlags = source->transformFlags;
    if (source->first != NULL) {
        for (entry = source->first; entry != NULL; entry = entry->next) {
            job = fileJobCreate();

            if ((entry->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
                job->id = (u32)fileJobCreateFromJob((FileJobPayload *)entry->id);
            } else {
                FileJob *parent = fileQueueFindById(source, entry->id);
                s32 index = fileFindQueuedJobIndex(source, parent);

                parent = fileQueueGetAt(queue, index);
                job->id = (u32)fileJobCreateChild((FileJobPayload *)parent->id);
            }
            fileJobCopyHeader(job, entry);
            strcpy(job->name, entry->name);
            fileQueueAppend(queue, job);
        }
    } else {
        FileJob *records;

        entry = (FileJob *)((u8 *)source + (u32)source->last);
        records = entry;
        if (source->count > 0) {
            s32 count = source->count;

            do {
                job = fileJobCreate();

                if ((entry->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
                    FileJobPayload *request = (FileJobPayload *)((u8 *)source + entry->id);

                    if (entry->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
                        FileJobPayload *secondary = (FileJobPayload *)((u8 *)source + records[entry->sector].id);

                        request->secondary.size = secondary->secondary.size;
                        request->secondary.offset = (u32)fileResolveSecondaryBuffer(secondary) - (u32)request;
                        job->id = (u32)fileJobCreateFromJob(request);
                        request->secondary.offset = request->secondary.size = 0;
                    } else {
                        job->id = (u32)fileJobCreateFromJob(request);
                    }
                } else {
                    FileJob *parent = fileQueueGetAt(queue, entry->id);

                    job->id = (u32)fileJobCreateChild((FileJobPayload *)parent->id);
                }
                fileJobCopyHeader(job, entry);
                strcpy(job->name, entry->name);
                fileQueueAppend(queue, job);
                entry++;
            } while (--count != 0);
        }
    }
    VU0_STORE_VF(vf0, vec);
    fileQueueSetPosition(queue, vec);
    fileQueueSetRotation(queue, vec);
    fileQueueSetScale(queue, 1.0f);
    fileQueueSetColor(queue, 0x80808080);
    return queue;
}

/* Effect-node creation callback: ignore its option and clone the supplied queue. */
FileQueue *CallShopTest(u32 unused, FileQueue *source) {
    return fileCloneQueueEntries(source);
}

void camFollowOffsetVec(FileJob *job, void *dst);
struct CamAim;
extern void camAimRotation(struct CamAim *obj, void *dst);

/* Per-frame update: refreshes the queue rotation when an aim flag (0x60) is set, then repositions and re-notifies every job whose start frame (job+0x80) has been reached; the frame counter only advances when the effect-frame pause flag is clear. */
void fileQueueUpdate(FileQueue *queue)
{
    f32 pos[4];
    f32 aimQuat[4];
    f32 savedQuat[4];
    FileJob *job;
    s32 limit;
    f32 total;

    if (queue->transformFlags & 0x60) {
        SDF_VU0_COPYVECTOR(savedQuat, queue->quat);
        camAimRotation((struct CamAim *)queue, aimQuat);
        fileQueueSetRotation(queue, aimQuat);
        SDF_VU0_COPYVECTOR(queue->quat, savedQuat);
    }
    total = queue->scale * queue->scaleMultiplier;
    limit = queue->updateFrame;
    for (job = queue->first; job != NULL; job = job->next) {
        if (limit < job->startFrame) {
            continue;
        }
        if (job->updateFlags & FILE_JOB_UPDATE_FLAG_SKIP_FRAME_UPDATE) {
            continue;
        }
        if (job->xformFlags & 0x18) {
            camFollowOffsetVec(job, pos);
            SDF_VU0_LOADVECTOR(queue->quat);
            effMiscQuaternionToMatrixVU();
            SDF_VU0_LOADVECTOR(queue->position);
            SDF_VU0_LOADVECTOR_B(queue->offset);
            SDF_VU0_ADDVECTOR();
            if (job->xformFlags & 4) {
                VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
            }
            SDF_VU0_LOADVECTOR_B(pos);
            if (job->xformFlags & 0x80) {
                VU0_SCALE_VF(vf11, total);
            }
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(pos);
            fileJobInvokePositionCallback((FileJobPayload *)job->id, pos);
        }
        if (job->xformFlags & 0x60) {
            camAimRotation((struct CamAim *)job, aimQuat);
            fileJobInvokeRotationCallback((FileJobPayload *)job->id, aimQuat);
        }
        fileJobInvokeTypeCallback((FileJobPayload *)job->id);
    }
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        queue->updateFrame++;
    }
}

/* A queued job's +0x90 word is a child pointer when flag 1 is clear; it is
 * an ID for other job kinds. Destroy children before their parent jobs. */
void fileQueueDestroy(FileQueue *queue) {
    FileJob *job = queue->first;
    while (job != NULL) {
        FileJob *next = job->next;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            fileJobDestroy((FileJobPayload *)job->id);
        }
        fileDestroyJob(job);
        job = next;
    }
    sdfChipFreeDirect(queue);
}

FileQueue *fileQueueClone(const FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    const FileJob *src;
    f32 vec[4] __attribute__((aligned(16)));

    SDF_VU0_COPYVECTOR(queue, source);
    SDF_VU0_COPYVECTOR(queue->axis, source->axis);
    queue->scaleMultiplier = source->scaleMultiplier;
    queue->transformFlags = source->transformFlags;
    for (src = source->first; src != NULL; src = src->next) {
        FileJob *job = fileJobCreate();
        job->id = (u32)fileJobCreateChild((FileJobPayload *)src->id);
        fileJobCopyHeader(job, src);
        fileQueueAppend(queue, job);
    }
    VU0_STORE_VF(vf0, vec);
    fileQueueSetPosition(queue, vec);
    fileQueueSetRotation(queue, vec);
    fileQueueSetScale(queue, 1.0f);
    fileQueueSetColor(queue, 0x80808080);
    return queue;
}

void fileQueueNotifyAllJobsComplete(FileQueue *queue) {
    FileJob *job = queue->first;
    while (job != NULL) {
        fileJobNotifyComplete((FileJobPayload *)job->id);
        job = job->next;
    }
    queue->updateFrame = 0;
}

void fileQueueSetPosition(FileQueue *queue, const f32 vec[4])
{
    f32 rot[16];
    f32 base[4];
    f32 pos[4];
    FileJob *job;
    f32 scale;

    SDF_VU0_LOADVECTOR(vec);
    SDF_VU0_STOREVECTOR(queue->position);
    SDF_VU0_LOADVECTOR_B(queue->offset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(base);
    SDF_VU0_LOADVECTOR(queue->quat);
    effMiscQuaternionToMatrixVU();
    VU0_STORE_MATRIX(rot);
    scale = queue->scale * queue->scaleMultiplier;
    for (job = queue->first; job != NULL; job = job->next) {
        SDF_VU0_LOADVECTOR(base);
        if (job->xformFlags & 4) {
            VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
        }
        SDF_VU0_LOADVECTOR_B(job->offset);
        if (job->xformFlags & 0x80) {
            VU0_SCALE_VF(vf11, scale);
        }
        VU0_LOAD_MATRIX(rot);
        VU0_ROTATE_VEC(vf11, vf11);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        fileJobInvokePositionCallback((FileJobPayload *)job->id, pos);
    }
}

void fileQueueSetRotation(FileQueue *queue, const f32 rot[4])
{
    f32 quat[4];
    f32 pos[4];
    FileJob *job;

    SDF_VU0_LOADVECTOR(rot);
    SDF_VU0_STOREVECTOR(queue->quat);
    SDF_VU0_STOREVECTOR(quat);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_LOADVECTOR(queue->axis);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(queue->offset);
    for (job = queue->first; job != NULL; job = job->next) {
        SDF_VU0_LOADVECTOR(job->quat);
        SDF_VU0_LOADVECTOR_B(quat);
        effMiscQuatMultiplyVU();
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        fileJobInvokeRotationCallback((FileJobPayload *)job->id, pos);
    }
    fileQueueSetPosition(queue, queue->position);
}

void fileQueueSetScale(FileQueue *queue, f32 scale)
{
    s128 pos;
    FileJob *job;
    f32 total;
    f32 jobScale;

    queue->scale = scale;
    total = scale * queue->scaleMultiplier;
    for (job = queue->first; job != NULL; job = job->next) {
        jobScale = job->scale;
        if (job->updateFlags & FILE_JOB_UPDATE_FLAG_APPLY_QUEUE_SCALE) {
            jobScale = jobScale * total;
        }
        fileJobInvokeScaleCallback((FileJobPayload *)job->id, jobScale);
        if (job->xformFlags & 0x80) {
            SDF_VU0_LOADVECTOR(queue->position);
            SDF_VU0_LOADVECTOR_B(queue->offset);
            SDF_VU0_ADDVECTOR();
            if (job->xformFlags & 4) {
                VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
            }
            SDF_VU0_LOADVECTOR_B(job->offset);
            VU0_SCALAR_OP(total, "vmulx.xyzw vf11, vf11, vf2x");
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(&pos);
            fileJobInvokePositionCallback((FileJobPayload *)job->id, (f32 *)&pos);
        }
    }
}

/* Store the queue's modulation colour, then send every job its own colour
 * modulated by it ((job / 128) * (queue / 128) * 128 per channel). The tint
 * store is the unclobbered SDK spelling: as a bare volatile asm it flushes the
 * CSE table, so the loop's 1/128 is rematerialized rather than copied from the
 * first unpack's FPR. Reading the payload before the packed store keeps the
 * store from feeding the argument load. */
void fileQueueSetColor(FileQueue *queue, u32 color) {
    f32 tint[4] __attribute__((aligned(16)));
    u32 queueColor[4] __attribute__((aligned(16)));
    u32 jobColor[4] __attribute__((aligned(16)));
    u32 packed[4] __attribute__((aligned(16)));
    FileJob *job;

    queue->color = color;
    queueColor[0] = color;
    EE_MMI_RGBA_UNPACK(queueColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(tint);
    for (job = queue->first; job != NULL; job = job->next) {
        u32 result;
        FileJobPayload *payload;

        jobColor[0] = job->color;
        EE_MMI_RGBA_UNPACK(jobColor, 1.0f / 128.0f);
        SDF_VU0_LOADVECTOR_B(tint);
        SDF_VU0_MULVECTOR();
        EE_MMI_RGBA_PACK_UNIT(result, 0x43000000);
        payload = (FileJobPayload *)job->id;
        packed[0] = result;
        fileDispatchJobTypeCallback(payload, result);
    }
}

void fileQueueReadPosition(const FileQueue *queue, f32 position[4]) {
    SDF_VU0_COPYVECTOR(position, queue->position);
}

void fileReadStoredQuaternion(const FileQueue *queue, f32 quaternion[4]) {
    SDF_VU0_COPYVECTOR(quaternion, queue->quat);
}

f32 fileGetQueueScale(FileQueue *queue) {
    return queue->scale;
}

u32 fileGetQueueColor(FileQueue *queue) {
    return queue->color;
}

void fileQueueCopyRotationFromSource(FileQueue *queue, f32 matrix[4][4]) {
    f32 vec[4] __attribute__((aligned(16)));
    sdfVuMatrixToQuaternion(matrix);
    SDF_VU0_STOREVECTOR(vec);
    fileQueueSetRotation(queue, vec);
}

extern void fileQueueAppend(FileQueue *queue, FileJob *job);

FileJob *fileAppendJob(FileQueue *queue, u32 id) {
    FileJob *job = fileJobCreate();

    job->id = id;
    fileQueueAppend(queue, job);
    return job;
}

FileJob *fileDuplicateAndAppendJob(FileQueue *queue, FileJobPayload *source) {
    FileJobPayload *job = fileDuplicateJob(source);
    return fileAppendJob(queue, (u32)job);
}

extern u8 D_00437E28[];

FileJob *fileAppendJobFromCommandPath(FileQueue *queue, const char *commandPath) {
    printf(D_00437E28);
    return fileAppendJob(queue, (u32)fileJobCreateFromCommandState(commandPath));
}

FileJob *fileJobDuplicateAfter(FileQueue *queue, FileJob *src) {
    FileJob *job = fileJobCreate();

    memcpy(job, src, 0xC0);
    fileJobResetAndInitTransform(job);
    job->flags |= FILE_JOB_FLAG_SHARED_PAYLOAD;
    job->id = src->id;
    fileQueueInsertAfter(queue, src, job);
    return job;
}

extern FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);
extern FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id);

/* Makes the first job chained to owner's sector the leader and rechains the rest to it. */
static inline void fileQueueRechainSectorFollowers(FileQueue *queue, FileJob *owner, FileJob *leader) {
    FileJob *next;

    leader->sector = 0;
    leader->flags &= ~FILE_JOB_FLAG_SECTOR_FOLLOWER;
    fileQueueLinkJobToSectorLeader(queue, leader, leader);
    next = fileQueueFindBySector(queue, owner->id);
    while (next != NULL) {
        fileQueueLinkJobToSectorLeader(queue, next, leader);
        next = fileQueueFindBySector(queue, owner->id);
    }
}

void fileQueueLinkJobToSectorLeader(FileQueue *queue, FileJob *job, FileJob *ref) {
    FileJob *node;
    FileJob *found;
    FileJobPayload *source;
    u32 refIndex;
    u32 flags;

    if (!(job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD)) {
        for (node = queue->first; node != NULL; node = node->next) {
            if ((node->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) && node->id == job->id) {
                fileQueueLinkJobToSectorLeader(queue, node, ref);
            }
        }
    }
    if (job == ref) {
        return;
    }
    flags = job->flags;
    if (!(flags & FILE_JOB_FLAG_SHARED_PAYLOAD)) {
        fileJobFreeSecondaryBuffer((FileJobPayload *)job->id);
        source = (FileJobPayload *)ref->id;
        fileJobSetSecondaryData((FileJobPayload *)job->id, (void *)source->secondary.offset, source->secondary.size,
                                source->primary.selector);
        flags = job->flags;
    }
    job->flags = flags | FILE_JOB_FLAG_SECTOR_FOLLOWER;
    job->sector = ref->id;
    refIndex = fileFindQueuedJobIndex(queue, ref);
    if (fileFindQueuedJobIndex(queue, job) < refIndex) {
        found = fileQueueFindBySector(queue, ref->id);
        if (found != NULL) {
            fileQueueRechainSectorFollowers(queue, ref, found);
            fileQueueLinkJobToSectorLeader(queue, ref, found);
        }
    }
}

void fileQueueDetachSectorFollower(FileQueue *queue, FileJob *job) {
    u32 flags = job->flags;
    FileJob *first;

    job->flags = flags & ~FILE_JOB_FLAG_SECTOR_FOLLOWER;
    if (!(flags & FILE_JOB_FLAG_SHARED_PAYLOAD)) {
        first = fileQueueFindBySector(queue, job->id);
        if (first != NULL) {
            fileQueueRechainSectorFollowers(queue, job, first);
        }
    }
    job->sector = 0;
}

void fileQueueRemoveAndDestroyJob(FileQueue *queue, FileJob *job) {
    FileJob *first;

    fileQueueRemove(queue, job);
    if (!(job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD)) {
        first = fileQueueFindFlaggedById(queue, job->id);
        while (first != NULL) {
            fileQueueRemoveAndDestroyJob(queue, first);
            first = fileQueueFindFlaggedById(queue, job->id);
        }
        if (!(job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER)) {
            first = fileQueueFindBySector(queue, job->id);
            if (first != NULL) {
                fileQueueRechainSectorFollowers(queue, job, first);
            }
        }
        if (!(job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD)) {
            fileJobDestroy((FileJobPayload *)job->id);
        }
    }
    fileDestroyJob(job);
}

void fileJobCopyHeader(FileJob *dst, const FileJob *src) {
    memcpy(dst, src, 0x90);
}

extern char fileQueueImagePaddingBytes[];
extern s32 fileQueueCountLinkedJobs(FileQueue *queue);

/* Export a queue: the queue header, one relocated record per job, then each
 * owning job's 16-byte-aligned payload. Child jobs store their parent's index. */
void fileQueueSaveImage(FileQueue *queue, const char *filePath) {
    char path[0xD0];
    FileJobPayload payload;
    FileQueue header;
    FileJob record;
    s32 fd;
    s32 count;
    s32 offset;
    s32 size;
    FileJob *job;

    if (sdfHddValid != 0) {
        sprintf(path, D_00437E10, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, D_00437E18, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    header = *queue;
    header.updateFrame = 0;
    size = sizeof(FileQueue);
    header.entryOffset = size;
    header.first = NULL;
    count = fileQueueCountLinkedJobs(queue);
    sceWrite(fd, &header, sizeof(FileQueue));
    offset = count * sizeof(FileJob) + sizeof(FileQueue);

    for (job = queue->first; job != NULL; job = job->next) {
        record = *job;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
                size = fileJobSerializedSize((FileJobPayload *)job->id);
            } else {
                payload = *(FileJobPayload *)job->id;
                payload.secondary.offset = 0;
                payload.secondary.size = 0;
                payload.secondary.allocation = NULL;
                size = fileJobSerializedSize(&payload);
            }
            record.id = offset;
        } else {
            size = 0;
            record.id = fileFindQueuedJobIndex(queue, fileQueueFindById(queue, job->id));
        }
        if (job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
            record.sector = fileFindQueuedJobIndex(queue, fileQueueFindById(queue, job->sector));
        }
        record.next = NULL;
        record.prev = NULL;
        sceWrite(fd, &record, sizeof(FileJob));
        offset += (size & 0xF) != 0 ? ((size >> 4) + 1) << 4 : (size >> 4) << 4;
    }

    for (job = queue->first; job != NULL; job = job->next) {
        s32 blocks;
        s32 aligned;
        s32 padding;

        if (job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) {
            continue;
        }
        if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
            size = fileJobSerializedSize((FileJobPayload *)job->id);
            fileJobWriteSerializedPayload(fd, (FileJobPayload *)job->id);
        } else {
            payload = *(FileJobPayload *)job->id;
            payload.secondary.offset = 0;
            payload.secondary.size = 0;
            payload.secondary.allocation = NULL;
            size = fileJobSerializedSize(&payload);
            fileJobWriteSerializedPayload(fd, &payload);
        }
        blocks = size >> 4;
        if ((size & 0xF) != 0) {
            aligned = (blocks + 1) << 4;
        } else {
            aligned = blocks << 4;
        }
        padding = aligned - size;
        if (padding > 0) {
            sceWrite(fd, fileQueueImagePaddingBytes, padding);
        }
    }
    sceClose(fd);
    sceSync(D_00437E20, 0);
}

/* Sixteen-byte preamble of a saved file queue. */
typedef struct FileQueueImageHeader {
    s32 version;
    s32 unk4;
    s32 unk8;
    f32 unkC;
} FileQueueImageHeader;

extern char fileQueueVersionedImagePaddingBytes[];

/* Save a queue image: the queue header, one relocated record per job, then each
 * owning job's 16-byte-aligned payload. Child jobs store their parent's index. */
void fileQueueSaveVersionedImage(FileQueue *queue, const char *filePath) {
    char path[0xD0];
    FileJobPayload payload;
    FileQueue header;
    FileJob record;
    FileQueueImageHeader image;
    s32 fd;
    s32 count;
    s32 offset;
    s32 size;
    FileJob *job;

    if (sdfHddValid != 0) {
        sprintf(path, D_00437E10, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, D_00437E18, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    image.version = 5;
    image.unk4 = 0;
    image.unkC = 1.03f;
    sceWrite(fd, &image, sizeof(image));

    header = *queue;
    header.updateFrame = 0;
    size = sizeof(FileQueue);
    header.entryOffset = size;
    header.first = NULL;
    count = fileQueueCountLinkedJobs(queue);
    sceWrite(fd, &header, sizeof(FileQueue));
    offset = count * sizeof(FileJob) + sizeof(FileQueue);

    for (job = queue->first; job != NULL; job = job->next) {
        record = *job;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
                size = fileJobSerializedSize((FileJobPayload *)job->id);
            } else {
                payload = *(FileJobPayload *)job->id;
                payload.secondary.offset = 0;
                payload.secondary.size = 0;
                payload.secondary.allocation = NULL;
                size = fileJobSerializedSize(&payload);
            }
            record.id = offset;
        } else {
            size = 0;
            record.id = fileFindQueuedJobIndex(queue, fileQueueFindById(queue, job->id));
        }
        if (job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
            record.sector = fileFindQueuedJobIndex(queue, fileQueueFindById(queue, job->sector));
        }
        record.next = NULL;
        record.prev = NULL;
        sceWrite(fd, &record, sizeof(FileJob));
        offset += (size & 0xF) != 0 ? ((size >> 4) + 1) << 4 : (size >> 4) << 4;
    }

    for (job = queue->first; job != NULL; job = job->next) {
        s32 blocks;
        s32 aligned;
        s32 padding;

        if (job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) {
            continue;
        }
        if ((job->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) == 0) {
            size = fileJobSerializedSize((FileJobPayload *)job->id);
            fileJobWriteSerializedPayload(fd, (FileJobPayload *)job->id);
        } else {
            payload = *(FileJobPayload *)job->id;
            payload.secondary.offset = 0;
            payload.secondary.size = 0;
            payload.secondary.allocation = NULL;
            size = fileJobSerializedSize(&payload);
            fileJobWriteSerializedPayload(fd, &payload);
        }
        blocks = size >> 4;
        if ((size & 0xF) != 0) {
            aligned = (blocks + 1) << 4;
        } else {
            aligned = blocks << 4;
        }
        padding = aligned - size;
        if (padding > 0) {
            sceWrite(fd, fileQueueVersionedImagePaddingBytes, padding);
        }
    }
    sceClose(fd);
    sceSync(D_00437E20, 0);
}

FileQueue *fileQueueCreateFromCommandState(const char *entry) {
    sdfFileCmdHandle_t *command = sdfFileOpen(entry);
    s32 size;
    struct sdfMemHandle_t *allocation;
    FileQueue *image;
    FileQueue *queue;
    FileJob *copy;
    FileJob *src;
    u32 i;

    if (command == NULL) {
        return NULL;
    }
    size = sdfFileLength(command);
    allocation = sdfMemAllocDirect(size);
    image = (FileQueue *)sdfMemLock(allocation);
    sdfFileRead(command, image, size);
    sdfFileClose(command);
    queue = fileQueueCreate();
    SDF_VU0_COPYVECTOR(queue->offset, image->offset);
    SDF_VU0_COPYVECTOR(queue->axis, image->offset);
    queue->scaleMultiplier = image->scaleMultiplier;
    queue->transformFlags = image->transformFlags;
    for (i = 0, src = (FileJob *)((u8 *)image + image->entryOffset);
         i < image->count; i++, src++) {
        if ((src->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            copy = fileDuplicateAndAppendJob(queue, (FileJobPayload *)((u8 *)image + src->id));
        } else {
            copy = fileJobDuplicateAfter(queue, fileQueueGetAt(queue, src->id));
        }
        if (src->flags & FILE_JOB_FLAG_SECTOR_FOLLOWER) {
            fileQueueLinkJobToSectorLeader(queue, copy, fileQueueGetAt(queue, src->sector));
        }
        fileJobCopyHeader(copy, src);
        strcpy(copy->name, src->name);
        copy->flags = src->flags;
    }
    sdfMemFreeDirect(allocation);
    return queue;
}

FileJob *fileQueueFindById(FileQueue *queue, u32 id) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if (job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) != 0 && job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if ((job->flags & (FILE_JOB_FLAG_SHARED_PAYLOAD | FILE_JOB_FLAG_SECTOR_FOLLOWER)) ==
            FILE_JOB_FLAG_SECTOR_FOLLOWER && job->sector == sector) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueGetAt(FileQueue *queue, s32 index) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if (index-- == 0) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target) {
    FileJob *job = queue->first;
    s32 index = 0;
    while (job != NULL) {
        if (job == target) {
            return index;
        }
        job = job->next;
        index++;
    }
    return 0;
}

s32 fileQueueCountLinkedJobs(FileQueue *queue) {
    FileJob *job;
    s32 count;

    count = 0;
    for (job = queue->first; job != 0; job = job->next) {
        count = count + 1;
    }
    return count;
}


/* vu0 routine: dst = |pos| * unit(sdfViewTargetVector - aViewTarget); flag 0x10 flattens y, then puts pos.y back */
void camFollowOffsetVec(FileJob *obj, void *dst)
{
    f32 len[4];
    f32 length;
    u32 flags = obj->xformFlags;

    if ((flags & 0x18) == 0) {
        SDF_VU0_COPYVECTOR(dst, obj->offset);
        return;
    }
    SDF_VU0_LOADVECTOR(obj->offset);
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_GETLENGTH(length);
    VEC3_SPLAT(len, length);
    SDF_VU0_LOADVECTOR(sdfViewTargetVector);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(len);
    SDF_VU0_MULVECTOR();
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(obj->offset[1], "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_STOREVECTOR(dst);
}

/* dst = rotation facing unit(sdfViewTargetVector - aViewTarget) (pitch and yaw); flag 0x40 composes it onto obj->quat, no flag 0x60 copies obj->quat */
void camAimRotation(CamAim *obj, void *dst)
{
    f32 v[4];
    f32 pitch;
    u32 flags = obj->flags;

    if ((flags & 0x60) == 0) {
        SDF_VU0_COPYVECTOR(dst, obj->quat);
        return;
    }
    SDF_VU0_LOADVECTOR(sdfViewTargetVector);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(v);
    pitch = 0.0f;
    if ((flags & 0x40) == 0) {
        pitch = -acosf(-v[1]);
    }
    sdfConvertEulerAnglesToQuaternionVU(pitch, atan2f(v[0], v[2]), 0.0f);
    if (obj->flags & 0x40) {
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(obj->quat);
        effMiscQuatMultiplyVU();
    }
    SDF_VU0_STOREVECTOR(dst);
}



EffectSurfaceNode *fileCreateSurfaceLoaderState(s32 capacity) {
    EffectSurfaceNode *rec = (EffectSurfaceNode *)sdfAllocAndClearQuadwords(sizeof(EffectSurfaceNode));
    u32 color = 0x80808080;

    rec->handleCount = capacity;
    rec->active = 1;
    rec->color = color;
    rec->opacity = 1.0f;
    rec->resource = NULL;
    rec->jobs = NULL;
    rec->jobAllocation = 0;
    rec->queues = NULL;
    rec->queueAllocation = 0;
    rec->record = 0;
    return rec;
}


EffectSurfaceNode *fileCreateGridLoaderRecord(FileKeyBlock *hdr) {
    u32 rows = hdr->emissionDuration;
    u32 count = (rows != 0 ? rows : hdr->spawnRate) * (rows != 0 ? hdr->spawnRate : hdr->length);

    return fileCreateSurfaceLoaderState(count <= 0x12C ? count : 0x12C);
}



EffectSurfaceNode *fileCreateEffectSurfaceFromJob(FileJobPayload *job) {
    void *primary = fileResolvePrimaryBuffer(job);
    EffectSurfaceNode *node = fileCreateGridLoaderRecord(primary);
    void *secondary;

    fileLoadObjectSetResource(node, job->option, primary);
    secondary = fileResolveSecondaryBuffer(job);
    if (secondary != NULL) {
        switch (job->primary.selector) {
        case 1:
            fileLoadObjectOpenDevice(node, (u32)secondary);
            break;
        case 2:
            fileLoadObjectOpenAndStartDevice(node, (u32)secondary);
            break;
        case 4:
            fileLoadObjectOpenNamedDevice(node, *(u32 *)secondary);
            break;
        case 5:
            fileReplaceEffectSurfaceJobs(node, secondary);
            break;
        case 6:
            fileReplaceEffectSurfaceQueues(node, secondary);
            break;
        case 7:
            fileReplaceReferenceHolder(node, (u32)secondary);
            break;
        }
        node->kind = job->primary.selector;
    }
    return node;
}

void fileDestroyEffectSurfaceAndChildren(EffectSurfaceNode *node) {
    u32 count;
    u32 i;

    if (node->resource != NULL) {
        dds3BillboardDelete(node->resource);
    }
    if (node->jobAllocation != 0) {
        count = node->record->count;
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfMemFreeDirect(node->jobAllocation);
    }
    if (node->queueAllocation != 0) {
        count = node->record->count;
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        sdfMemFreeDirect(node->queueAllocation);
    }
    if (node->resourceHolder != NULL) {
        effReleaseReferenceHolder(node->resourceHolder);
    }
    if (node->record != 0) {
        effPCPHelperBEDGetModel(node->record);
    }
    sdfChipFreeDirect(node);
}

void fileCloneEffectSurfaceResources(EffectSurfaceNode *dst, EffectSurfaceNode *src);

EffectSurfaceNode *fileLoadObjectCreateChild(EffectSurfaceNode *owner) {
    FileKeyBlock *source = (FileKeyBlock *)owner->record->data1;
    EffectSurfaceNode *result = fileCreateGridLoaderRecord(source);

    fileLoadObjectSetResource(result, owner->record->type, source);
    fileCloneEffectSurfaceResources(result, owner);
    return result;
}

void fileCloneEffectSurfaceResources(EffectSurfaceNode *dst, EffectSurfaceNode *src) {
    u32 count;
    s32 size;
    u32 i;

    switch (src->kind) {
    case 1:
    case 2:
    case 4:
        if (dst->resource != NULL) {
            dds3BillboardDelete(dst->resource);
        }
        dst->resource = dds3BillboardCopy(src->resource);
        dds3BillboardSetAnimColorAdvMode(dst->resource);
        if (dst->record != 0) {
            dds3BillboardSetMode(dst->resource, (s16)((FileKeyBlock *)dst->record->data0)->alphaTrack.surfaceIndex);
        }
        break;
    case 5:
        count = src->record->count;
        if (count == 0) {
            return;
        }
        if (dst->jobAllocation != 0) {
            for (i = 0; i < count; i++) {
                fileJobDestroy(dst->jobs[i]);
            }
            sdfMemFreeDirect(dst->jobAllocation);
            dst->jobs = 0;
            dst->jobAllocation = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->jobAllocation = sdfMemAllocDirect(size);
        dst->jobs = (FileJobPayload **)sdfMemLock(dst->jobAllocation);
        for (i = 0; i < count; i++) {
            dst->jobs[i] = fileJobCreateChild(src->jobs[0]);
        }
        break;
    case 6:
        count = src->record->count;
        if (count == 0) {
            return;
        }
        if (dst->queueAllocation != 0) {
            for (i = 0; i < count; i++) {
                fileQueueDestroy(dst->queues[i]);
            }
            sdfMemFreeDirect(dst->queueAllocation);
            dst->queues = 0;
            dst->queueAllocation = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->queueAllocation = sdfMemAllocDirect(size);
        dst->queues = sdfMemLock(dst->queueAllocation);
        for (i = 0; i < count; i++) {
            dst->queues[i] = fileQueueClone(src->queues[0]);
        }
        break;
    case 7:
        if (dst->resourceHolder != NULL) {
            effReleaseReferenceHolder(dst->resourceHolder);
        }
        dst->resourceHolder = effReferenceObjectRetain(src->resourceHolder);
        break;
    }
    dst->kind = src->kind;
}

void fileLoadObjectSetResource(EffectSurfaceNode *node, u32 entryId, void *resource) {
    if (node->record != 0) {
        effPCPHelperBEDGetModel(node->record);
    }
    node->record = fileAllocateGridRecordSlots((u16)entryId, node->handleCount, resource);
}

void fileLoadObjectOpenNamedDevice(EffectSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = effCreateBillboardSharingIndexedResource(resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void fileLoadObjectOpenDevice(EffectSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(0, resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void fileLoadObjectOpenAndStartDevice(EffectSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(1, resourceId);
    node->resource = resource;
    dds3BillboardSetAnimColorAdvMode(resource);
    if (node->record != 0) {
        dds3BillboardSetMode(node->resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void fileReplaceEffectSurfaceJobs(EffectSurfaceNode *node, FileJobPayload *job) {
    u32 count = node->record->count;
    u32 i;
    s32 size;

    if (node->jobAllocation != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfMemFreeDirect(node->jobAllocation);
        node->jobs = 0;
        node->jobAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobAllocation = sdfMemAllocDirect(size);
        node->jobs = (FileJobPayload **)sdfMemLock(node->jobAllocation);
        node->jobs[0] = fileJobCreateFromJob(job);
        for (i = 1; i < count; i++) {
            node->jobs[i] = fileJobCreateChild(node->jobs[0]);
        }
    }
}

void fileReplaceEffectSurfaceQueues(EffectSurfaceNode *node, FileQueue *queue) {
    u32 count = node->record->count;
    u32 i;
    s32 size;

    if (node->queueAllocation != 0) {
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        sdfMemFreeDirect(node->queueAllocation);
        node->queues = 0;
        node->queueAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->queueAllocation = sdfMemAllocDirect(size);
        node->queues = sdfMemLock(node->queueAllocation);
        node->queues[0] = fileCloneQueueEntries(queue);
        for (i = 1; i < count; i++) {
            node->queues[i] = fileQueueClone(node->queues[0]);
        }
    }
}

void fileReplaceReferenceHolder(EffectSurfaceNode *obj, u32 resource) {
    if (obj->resourceHolder != NULL) {
        effReleaseReferenceHolder(obj->resourceHolder);
    }
    obj->resourceHolder = effCreateTextureAnimationSet(resource);
}

void fileClearLoadObjectReferences(EffectSurfaceNode *obj) {
    if (obj->record != 0) {
        fileClearRecordReferences(obj->record);
        return;
    }
}

extern void mnuRecordSetVector(void *record, const void *src);
extern void fileSetRecordSecondVector(void *record, const void *src);

/* Original-style implicit int; callers ignore its result. */
fileAcquireLoadObjectRecord(EffectSurfaceNode *obj) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0 && obj->record != 0) {
        fileAcquireRecord(obj->record);
    }
}

#include "eff_anim.h"

extern void dds3BillboardDraw(struct billHandle_t *);
extern void fileReadRecordSecondVector(u8 *, void *);
extern void fileReadVectorPtr20(u8 *, void *);
extern void effSubmitGeneratedTexturePacket(SdfPoolNode *, EffGeneratedTextureDescriptor *);

void fileDrawLoadObjectSlots(s32 object) {
    EffGeneratedTextureDescriptor params;
    EffAnimSample sample;
    f32 transformed[4];
    f32 origin[4];
    f32 orientation[4];
    f32 matrix[16];
    f32 modulation[4];
    EffectSurfaceNode *obj = (EffectSurfaceNode *)object;
    FileSlotTable *record;
    FileSlot *slot;
    struct billHandle_t *billboard;
    FileJobPayload **jobs;
    FileQueue **queues;
    EffExpandedList *animation;
    SdfPoolNode *textureSink;
    s32 previousSegment;
    s32 count;
    s32 i;
    f32 scale;
    u32 billDirectPacked;
    u32 billTransformedPacked;
    u32 jobDirectPacked;
    u32 jobTransformedPacked;
    u32 queueDirectPacked;
    u32 queueTransformedPacked;
    u32 textureDirectPacked;
    s32 textureDirectFrame;
    u32 textureTransformedPacked;
    s32 textureTransformedFrame;

    eeUnpackRGBAWordFloat(obj->color, 0.0078125f);
    SDF_VU0_STOREVECTOR(modulation);
    record = (FileSlotTable *)obj->record;
    count = record->count;
    if (count != 0) {
        slot = record->slots;
        switch (obj->kind) {
        case 1:
        case 2:
        case 4:
            billboard = obj->resource;
            if (!(record->flags & 1)) {
                for (i = 0; i < count; i++, slot++) {
                    if (slot->state >= 0) {

                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        billDirectPacked = eePackRGBAWord(128.0f);
                        scale = slot->scale;
                        dds3BillboardSetRGBA(billboard, billDirectPacked);
                        dds3BillboardSetScale(billboard, scale, scale);
                        dds3BillboardSetRotate(billboard, slot->angle);
                        effCopyVector(billboard->position, slot->pos);
                        dds3BillboardSetAnimKey(billboard, slot->state);
                        dds3BillboardDraw(billboard);
                    }
                }
            } else {
                fileReadRecordSecondVector((u8 *)obj->record, orientation);
                fileReadVectorPtr20((u8 *)obj->record, origin);
                SDF_VU0_LOADVECTOR(orientation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_LOAD_ROW3(origin);
                VU0_STORE_MATRIX(matrix);
                for (i = 0; i < count; i++, slot++) {
                    if (slot->state >= 0) {

                        VU0_LOAD_MATRIX(matrix);
                        SDF_VU0_LOADVECTOR(slot->pos);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_STOREVECTOR(transformed);
                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        billTransformedPacked = eePackRGBAWord(128.0f);
                        scale = slot->scale;
                        dds3BillboardSetRGBA(billboard, billTransformedPacked);
                        dds3BillboardSetScale(billboard, scale, scale);
                        dds3BillboardSetRotate(billboard, slot->angle);
                        effCopyVector(billboard->position, transformed);
                        dds3BillboardSetAnimKey(billboard, slot->state);
                        dds3BillboardDraw(billboard);
                    }
                }
            }
            break;
        case 5:
            jobs = obj->jobs;
            if (!(record->flags & 1)) {
                for (i = 0; i < count; i++, slot++, jobs++) {
                    if (slot->state == 0) {
                        fileJobNotifyComplete(*jobs);
                    }
                    if (slot->state >= 0) {

                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        jobDirectPacked = eePackRGBAWord(128.0f);
                        fileDispatchJobTypeCallback(*jobs, jobDirectPacked);
                        fileJobInvokeScaleCallback(*jobs, slot->scale);
                        fileJobInvokePositionCallback(*jobs, slot->pos);
                        fileJobInvokeTypeCallback(*jobs);
                    }
                }
            } else {
                fileReadRecordSecondVector((u8 *)obj->record, orientation);
                fileReadVectorPtr20((u8 *)obj->record, origin);
                SDF_VU0_LOADVECTOR(orientation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_LOAD_ROW3(origin);
                VU0_STORE_MATRIX(matrix);
                for (i = 0; i < count; i++, slot++, jobs++) {
                    if (slot->state == 0) {
                        fileJobNotifyComplete(*jobs);
                    }
                    if (slot->state >= 0) {

                        VU0_LOAD_MATRIX(matrix);
                        SDF_VU0_LOADVECTOR(slot->pos);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_STOREVECTOR(transformed);
                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        jobTransformedPacked = eePackRGBAWord(128.0f);
                        fileDispatchJobTypeCallback(*jobs, jobTransformedPacked);
                        fileJobInvokeScaleCallback(*jobs, slot->scale);
                        fileJobInvokePositionCallback(*jobs, transformed);
                        fileJobInvokeTypeCallback(*jobs);
                    }
                }
            }
            break;
        case 6:
            queues = (FileQueue **)obj->queues;
            if (!(record->flags & 1)) {
                for (i = 0; i < count; i++, slot++, queues++) {
                    if (slot->state == 0) {
                        fileQueueNotifyAllJobsComplete(*queues);
                    }
                    if (slot->state >= 0) {

                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        queueDirectPacked = eePackRGBAWord(128.0f);
                        fileQueueSetColor(*queues, queueDirectPacked);
                        fileQueueSetScale(*queues, slot->scale);
                        fileQueueSetPosition(*queues, slot->pos);
                        fileQueueUpdate(*queues);
                    }
                }
            } else {
                fileReadRecordSecondVector((u8 *)obj->record, orientation);
                fileReadVectorPtr20((u8 *)obj->record, origin);
                SDF_VU0_LOADVECTOR(orientation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_LOAD_ROW3(origin);
                VU0_STORE_MATRIX(matrix);
                for (i = 0; i < count; i++, slot++, queues++) {
                    if (slot->state == 0) {
                        fileQueueNotifyAllJobsComplete(*queues);
                    }
                    if (slot->state >= 0) {

                        VU0_LOAD_MATRIX(matrix);
                        SDF_VU0_LOADVECTOR(slot->pos);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_STOREVECTOR(transformed);
                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        queueTransformedPacked = eePackRGBAWord(128.0f);
                        fileQueueSetColor(*queues, queueTransformedPacked);
                        fileQueueSetScale(*queues, slot->scale);
                        fileQueueSetPosition(*queues, transformed);
                        fileQueueUpdate(*queues);
                    }
                }
            }
            break;
        case 7: {
            u32 directInput[4];
            u32 directOutput[4];
            u32 transformedInput[4];
            u32 transformedOutput[4];
            animation = obj->resourceHolder;
            previousSegment = -1;
            textureSink = effGetPacketSubmissionSurface(((FileKeyBlock *)record->data0)->alphaTrack.surfaceIndex);
            if (!(((FileSlotTable *)obj->record)->flags & 1)) {
                for (i = 0; i < count; i++, slot++) {
                    if (slot->state >= 0) {

                        directInput[0] = slot->color;
                        EE_MMI_RGBA_UNPACK(directInput, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        {
                            u32 *__restrict packedOutput = directOutput;
                            const s32 *__restrict frameInput;

                            EE_MMI_RGBA_PACK_UNIT(packedOutput[0], 128.0f);
                            textureDirectPacked = packedOutput[0];
                            params.color = textureDirectPacked;
                            frameInput = &slot->state;
                            textureDirectFrame = *frameInput;
                        }
                        effSampleAnimSet(animation, textureDirectFrame, &sample);
                        if (previousSegment != sample.segment) {
                            params.texture = effAssignSampledSegmentReference(animation, textureSink, &sample);
                            previousSegment = sample.segment;
                        }
                        scale = slot->scale;
                        params.scaleX = scale * sample.scaleX;
                        params.scaleY = scale * sample.scaleY;
                        params.angle = slot->angle + sample.angle;
                        SDF_VU0_COPYVECTOR(params.position, slot->pos);
                        effSubmitGeneratedTexturePacket(textureSink, &params);
                    }
                }
            } else {
                fileReadRecordSecondVector((u8 *)obj->record, orientation);
                fileReadVectorPtr20((u8 *)obj->record, origin);
                SDF_VU0_LOADVECTOR(orientation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_LOAD_ROW3(origin);
                VU0_STORE_MATRIX(matrix);
                for (i = 0; i < count; i++, slot++) {
                    if (slot->state >= 0) {

                        VU0_LOAD_MATRIX(matrix);
                        SDF_VU0_LOADVECTOR(slot->pos);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_STOREVECTOR(params.position);
                        transformedInput[0] = slot->color;
                        EE_MMI_RGBA_UNPACK(transformedInput, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        {
                            u32 *__restrict packedOutput = transformedOutput;
                            const s32 *__restrict frameInput;

                            EE_MMI_RGBA_PACK_UNIT(packedOutput[0], 128.0f);
                            textureTransformedPacked = packedOutput[0];
                            params.color = textureTransformedPacked;
                            frameInput = &slot->state;
                            textureTransformedFrame = *frameInput;
                        }
                        effSampleAnimSet(animation, textureTransformedFrame, &sample);
                        if (previousSegment != sample.segment) {
                            params.texture = effAssignSampledSegmentReference(animation, textureSink, &sample);
                            previousSegment = sample.segment;
                        }
                        scale = slot->scale;
                        params.scaleX = scale * sample.scaleX;
                        params.scaleY = scale * sample.scaleY;
                        params.angle = slot->angle + sample.angle;
                        effSubmitGeneratedTexturePacket(textureSink, &params);
                    }
                }
            }
            break;
        }
        }
    }
}


void fileUpdateLoadObjectSlots(u32 objectAddr) {
    fileAcquireLoadObjectRecord((EffectSurfaceNode *)objectAddr);
    fileDrawLoadObjectSlots(objectAddr);
}

void fileSendLoadObjectRecordVector(EffectSurfaceNode *obj, const u128 *value) {
    mnuRecordSetVector(obj->record, value);
}

void fileCopyLoadObjectRecordVector(EffectSurfaceNode *obj, const u128 *value) {
    fileSetRecordSecondVector(obj->record, value);
}

void fileSetRecordWordFour(s32 record, u32 value) {
    *(u32 *)(record + 4) = value;
}

void fileSetLoadObjectScale(EffectSurfaceNode *obj, f32 scale) {
    obj->opacity = scale;
    dds3DispatchIndexedCallback(obj->record, scale);
}





void fileResetSlotStates(FileSlotTable *table) {
    u32 count;
    FileSlot *slot;
    u32 index;

    count = table->count;
    index = 0;
    slot = table->slots;
    if (count != 0) {
        do {
            index = index + 1;
            slot->state = 0xffffffff;
            slot++;
        } while (index < count);
    }
}

/* vu0 routine: preserve the packed RGB snapshot while applying the alpha track. */
u32 effSampleColorAlphaTracks(const SdfColorTrack *colorTrack,
                              const SdfAlphaTrack *alphaTrack,
                              s32 frame, s32 duration) {
    u32 color0;
    u32 color1;
    u32 alpha;
    u32 packedColor;
    u32 unitBits;
    f32 durationFloat;
    f32 factor;
    f32 inverseFactor;
    f32 alphaFactor;
    f32 alphaValue;
    s32 firstFrame;
    s32 secondFrame;
    s32 fadeInFrame;
    s32 fadeOutFrame;

    if (duration == 0) {
        return (colorTrack->initialColor & 0x00FFFFFF) |
               (alphaTrack->alpha << 24);
    }

    durationFloat = (f32)duration;
    switch (colorTrack->segmentMode) {
    case SDF_COLOR_TRACK_MODE_ENDPOINTS:
        factor = (f32)frame / durationFloat;
        color1 = colorTrack->initialColor;
        color0 = colorTrack->finalColor;
        break;
    case SDF_COLOR_TRACK_MODE_ONE_INTERMEDIATE_KEY:
        firstFrame = (s32)(colorTrack->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            color1 = colorTrack->initialColor;
            color0 = colorTrack->keys[0].color;
        } else {
            f32 span = duration - firstFrame;
            factor = (f32)(frame - firstFrame) / span;
            color1 = colorTrack->keys[0].color;
            color0 = colorTrack->finalColor;
        }
        break;
    case SDF_COLOR_TRACK_MODE_TWO_INTERMEDIATE_KEYS:
        firstFrame = (s32)(colorTrack->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            color1 = colorTrack->initialColor;
            color0 = colorTrack->keys[0].color;
        } else {
            secondFrame = (s32)(colorTrack->keys[1].fraction * durationFloat);
            if (frame < secondFrame) {
                f32 span = secondFrame - firstFrame;
                factor = (f32)(frame - firstFrame) / span;
                color1 = colorTrack->keys[0].color;
                color0 = colorTrack->keys[1].color;
            } else {
                f32 span = duration - secondFrame;
                factor = (f32)(frame - secondFrame) / span;
                color1 = colorTrack->keys[1].color;
                color0 = colorTrack->finalColor;
            }
        }
        break;
    default:
        color1 = colorTrack->initialColor;
        color0 = colorTrack->finalColor;
        factor = 0.0f;
        break;
    }

    unitBits = 0x3C000000;
    eeUnpackRGBAWord(color0, unitBits);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(color1, unitBits);
    inverseFactor = 1.0f - factor;
    VU0_SCALE_VF(vf10, inverseFactor);
    VU0_SCALE_VF(vf11, factor);
    SDF_VU0_ADDVECTOR();
    packedColor = eePackRGBAWord(128.0f);

    fadeInFrame = (s32)(alphaTrack->fadeInFraction * durationFloat);
    fadeOutFrame = (s32)(alphaTrack->fadeOutFraction * durationFloat);
    if (frame < fadeInFrame) {
        alphaFactor = (f32)frame / (f32)fadeInFrame;
    } else if (frame > fadeOutFrame) {
        alphaFactor = (f32)(duration - frame) / (f32)(duration - fadeOutFrame);
    } else {
        alphaFactor = 1.0f;
    }

    alphaValue = (f32)alphaTrack->alpha * alphaFactor;
    alpha = (u32)alphaValue;
    return (packedColor & 0x00FFFFFF) | (alpha << 24);
}

f32 effSampleScalarCurve(const EffScalarCurve *curve, s32 frame, s32 duration) {
    f32 from, to, factor, durationFloat;
    s32 firstFrame, secondFrame;
    if (duration == 0) {
        return curve->initialValue;
    }
    durationFloat = duration;
    switch (curve->mode) {
    case EFF_SCALAR_CURVE_MODE_ENDPOINTS:
        factor = (f32)frame / durationFloat;
        from = curve->initialValue;
        to = curve->finalValue;
        break;
    case EFF_SCALAR_CURVE_MODE_ONE_INTERMEDIATE_KEY:
        firstFrame = (s32)(curve->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            from = curve->initialValue;
            to = curve->keys[0].value;
        } else {
            f32 span = duration - firstFrame;
            factor = (f32)(frame - firstFrame) / span;
            from = curve->keys[0].value;
            to = curve->finalValue;
        }
        break;
    case EFF_SCALAR_CURVE_MODE_TWO_INTERMEDIATE_KEYS:
        firstFrame = (s32)(curve->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            from = curve->initialValue;
            to = curve->keys[0].value;
        } else {
            secondFrame = (s32)(curve->keys[1].fraction * durationFloat);
            if (frame < secondFrame) {
                f32 span = secondFrame - firstFrame;
                factor = (f32)(frame - firstFrame) / span;
                from = curve->keys[0].value;
                to = curve->keys[1].value;
            } else {
                f32 span = duration - secondFrame;
                factor = (f32)(frame - secondFrame) / span;
                from = curve->keys[1].value;
                to = curve->finalValue;
            }
        }
        break;
    default:
        from = curve->initialValue;
        to = curve->finalValue;
        factor = 0.0f;
        break;
    }
    return from + (to - from) * factor;
}



/* vu0 routine: samples the colour, scale and heading tracks at frame; in mode 2 the heading is the screen-space direction from out->pos to target (0 when they coincide) */
void fileSampleKeyTracks(FileSlot *out, FileKeyBlock *block, s32 frame, f32 *target)
{
    f32 delta[4];

    out->color = effSampleColorAlphaTracks(&block->colorTrack, &block->alphaTrack, frame, block->length);
    out->scale = effSampleScalarCurve(&block->scale.track.curve, frame, block->length);
    if (block->heading.track.curve.headingMode != 2) {
        out->angle = effSampleScalarCurve(&block->heading.track.curve, frame, block->length);
        return;
    }
    VU0_MOVE_VF(vf20, vf28);
    VU0_MOVE_VF(vf21, vf29);
    VU0_MOVE_VF(vf22, vf30);
    VU0_MOVE_VF(vf23, vf31);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)sdfProjectionMatrix);
    SDF_VU0_LOADVECTOR(out->pos);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(target);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(delta);
    if (delta[0] != 0.0f || delta[1] != 0.0f) {
        out->angle = atan2f(delta[1], delta[0]);
    } else {
        out->angle = 0.0f;
    }
    VU0_MOVE_VF(vf28, vf20);
    VU0_MOVE_VF(vf29, vf21);
    VU0_MOVE_VF(vf30, vf22);
    VU0_MOVE_VF(vf31, vf23);
}

void fileInvalidateSlotGroup(FileSlotTable *table, u32 slotAddr) {
    FileKeyBlock *grid = (FileKeyBlock *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    FileSlot *slot;
    s32 i;

    if (n != 0) {
        index = (slotAddr - (u32)table->slots) >> 5;
        slot = table->slots + (table->instances + index * n);
        for (i = 0; i < n; i++) {
            slot->state = 0xFFFFFFFF;
            slot++;
        }
    }
}

void fileCopyAndInvalidateSlotGroup(FileSlotTable *table, FileSlot *source) {
    FileKeyBlock *grid = (FileKeyBlock *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    FileSlot *slot;

    if (n != 0) {
        index = ((u32)source - (u32)table->slots) >> 5;
        slot = table->slots + (table->instances + index * n);
        *slot = *source;
        slot->state = 0xFFFFFFFF;
    }
}

/* vu0 routine: shifts one slot's history and interpolates its trailing samples. */
void effUpdateGridRecordSlotHistory(FileSlotTable *record, FileSlot *slot) {
    f32 points[4][4];
    f32 color[4];
    f32 colorStep[4];
    u32 curveStartColor[4];
    u32 curveEndColor[4];
    u32 curveResultColor[4];
    u32 linearStartColor[4];
    u32 linearEndColor[4];
    u32 linearResultColor[4];
    FileKeyBlock *dims = (FileKeyBlock *)record->data0;
    u32 rows = (u32)dims->rows;
    u32 columns = (u32)dims->columns;
    u32 count = columns * rows;
    u32 remaining;
    u32 i;
    u32 historyIndex;
    s32 frame;
    FileSlot *group;
    FileSlot *shiftTarget;
    FileSlot *cursor;
    f32 fraction;
    f32 step;
    f32 alpha;
    f32 scale;
    f32 angle;
    f32 scaleDelta;
    f32 angleDelta;

    if (count != 0) {
        u8 *base = (u8 *)record->slots;
        frame = slot->state;
        group = (FileSlot *)(base +
            ((record->instances + ((u32)((u8 *)slot - base) >> 5) * count) << 5));
        if (frame == 0) {
            *group = *slot;
            cursor = group;
            for (historyIndex = 0; historyIndex < count; historyIndex++) {
                cursor->state = -1;
                cursor++;
            }
        } else {
            group->state = (s32)((u32)frame - 1u);
            step = 1.0f / (f32)count;
            fraction = 0.0f;
            alpha = (f32)(slot->color >> 24);
            remaining = count - rows;
            shiftTarget = group + count - 1;
            cursor = shiftTarget - rows;
            for (historyIndex = 0; historyIndex < remaining; historyIndex++) {
                u32 fadedAlpha;
                *shiftTarget = *cursor;
                fadedAlpha = (u32)(alpha * fraction);
                shiftTarget->color = (cursor->color & 0xFFFFFF) |
                    (fadedAlpha << 24);
                fraction += step;
                cursor--;
                shiftTarget--;
            }
            *group = *slot;
            group->state = -1;
            if (rows != 1 && columns >= 2) {
                if (frame >= 2 && columns >= 3) {
                    if (group[rows * 2].state >= 0) {
                        FileSlot *previous;
                        SDF_VU0_COPYVECTOR(points[0], group[rows * 2].pos);
                        previous = group + rows;
                        SDF_VU0_COPYVECTOR(points[1], previous->pos);
                        SDF_VU0_COPYVECTOR(points[2], group->pos);
                        SDF_VU0_COPYVECTOR(points[3], group->pos);
                        fraction = 0.0f;
                        cursor = previous - 1;
                        step = 1.0f / (f32)(rows + 1);
                        scale = previous->scale;
                        scaleDelta = group->scale - scale;
                        angle = previous->angle;
                        angleDelta = group->angle - angle;
                        curveStartColor[0] = group->color;
                        EE_MMI_RGBA_UNPACK(curveStartColor, 0.0078125f);
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        curveEndColor[0] = previous->color;
                        EE_MMI_RGBA_UNPACK(curveEndColor, 0.0078125f);
                        SDF_VU0_STOREVECTOR_NOMEM(color);
                        SDF_VU0_SUBVECTOR_B_A();
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, step);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_STOREVECTOR_NOMEM(colorStep);
                        for (i = 0; i < rows - 1; i++) {
                            fraction += step;
                            vuCatmullRomPoint(points, fraction);
                            SDF_VU0_STOREVECTOR_NOMEM(cursor->pos);
                            cursor->scale = scale + scaleDelta * fraction;
                            cursor->angle = angle + angleDelta * fraction;
                            SDF_VU0_LOADVECTOR(color);
                            SDF_VU0_LOADVECTOR_B(colorStep);
                            VU0_ADD_EXTENDED(vf10, vf10, vf11);
                            SDF_VU0_STOREVECTOR_NOMEM(color);
                            EE_MMI_RGBA_PACK_UNIT(curveResultColor[0], 128.0f);
                            cursor->color = curveResultColor[0];
                            cursor->state = frame;
                            cursor--;
                        }
                    }
                } else if (frame > 0) {
                    if (group[rows].state >= 0) {
                        FileSlot *previous = group + rows;
                        shiftTarget = previous;
                        fraction = 0.0f;
                        cursor = previous - 1;
                        step = 1.0f / (f32)(rows + 1);
                        scale = previous->scale;
                        scaleDelta = group->scale - scale;
                        angle = previous->angle;
                        angleDelta = group->angle - angle;
                        linearStartColor[0] = group->color;
                        EE_MMI_RGBA_UNPACK(linearStartColor, 0.0078125f);
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        linearEndColor[0] = previous->color;
                        EE_MMI_RGBA_UNPACK(linearEndColor, 0.0078125f);
                        SDF_VU0_STOREVECTOR_NOMEM(color);
                        SDF_VU0_SUBVECTOR_B_A();
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, step);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_STOREVECTOR_NOMEM(colorStep);
                        {
                            u32 *__restrict packedOutput = linearResultColor;

                            for (i = 0; i < rows - 1; i++) {
                                fraction += step;
                                SDF_VU0_LOADVECTOR(shiftTarget->pos);
                                SDF_VU0_LOADVECTOR_B(group->pos);
                                VU0_LERP_VF10(fraction);
                                SDF_VU0_STOREVECTOR_NOMEM(cursor->pos);
                                cursor->scale = scale + scaleDelta * fraction;
                                cursor->angle = angle + angleDelta * fraction;
                                SDF_VU0_LOADVECTOR(color);
                                SDF_VU0_LOADVECTOR_B(colorStep);
                                VU0_ADD_EXTENDED(vf10, vf10, vf11);
                                SDF_VU0_STOREVECTOR_NOMEM(color);
                                {
                                    u8 *__restrict slotBytes;

                                    EE_MMI_RGBA_PACK_UNIT(packedOutput[0], 128.0f);
                                    slotBytes = (u8 *)cursor;
                                    *(u32 *)(slotBytes + (u32)&((FileSlot *)0)->color) = packedOutput[0];
                                    *(s32 *)(slotBytes + (u32)&((FileSlot *)0)->state) = frame;
                                }
                                cursor--;
                            }
                        }
                    }
                }
            }
        }
    }
}


extern struct EffRandState effSharedRandomState;
extern f32 fabsf(f32);
extern void effUpdateGridRecordSlotHistory(FileSlotTable *, FileSlot *);

typedef struct FileWaveMotion {
    f32 axis[4];             /* 0x00: xyz only */
    f32 swayDirection[4];    /* 0x10: xyz only */
    f32 phase;
    f32 previousSine;
    f32 amplitude;
    f32 amplitudeStep;
    f32 phaseStep;
    f32 speed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileWaveMotion;            /* 0x44, ordinary word alignment */

extern f32 effWaveEmitterNegativeYAxis[4]; /* (0, -1, 0, 0), existing shared axis */

/* vu0 routine: advances axial motion with a quaternion-oriented sine sway. */
void effUpdateWaveEmitterSlots(FileSlotTable *record) {
    f32 direction[4];
    f32 radiusVector[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileWaveMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 speedDecrease;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 distance;
    f32 sine;
    f32 sway;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileWaveMotion *)record->instanceWork;
    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        speedDecrease = keys->emitter.wave.axialDeceleration;
        radiusVector[3] = 0.0f;
        SDF_VU0_LOADVECTOR(keys->orientation);
        effMiscQuaternionToMatrixVU();
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                if (!(keys->spawnVariance > 0.0f)) {
                    toSpawn = record->instances;
                } else {
                    toSpawn = (s32)((f32)record->instances *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            s32 localSpace = flags & 1;
                            f32 initialAmplitude;
                            f32 finalAmplitude;
                            f32 radius;

                            if (localSpace != 0) {
                                motion->axis[0] = 0.0f;
                                motion->axis[1] = -1.0f;
                                motion->axis[2] = 0.0f;
                            } else {
                                SDF_VU0_LOADVECTOR(effWaveEmitterNegativeYAxis);
                                SDF_VU0_APPLYMATRIX_XYZ();
                                SDF_VU0_STOREVECTOR(direction);
                                motion->axis[0] = direction[0];
                                motion->axis[1] = direction[1];
                                motion->axis[2] = direction[2];
                            }
                            direction[0] = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f;
                            direction[1] = 0.0f;
                            direction[2] = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f;
                            direction[3] = 0.0f;
                            SDF_VU0_LOADVECTOR(direction);
                            SDF_VU0_NORMALIZE();
                            if (localSpace == 0) {
                                SDF_VU0_APPLYMATRIX_XYZ();
                            }
                            SDF_VU0_STOREVECTOR(direction);
                            motion->swayDirection[0] = direction[0];
                            motion->swayDirection[1] = direction[1];
                            motion->swayDirection[2] = direction[2];
                            motion->speed = keys->emitter.wave.axialSpeed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.wave.axialSpeedRandomness +
                                 (1.0f - keys->emitter.wave.axialSpeedRandomness));
                            initialAmplitude = keys->emitter.wave.initialAmplitude *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.wave.initialAmplitudeRandomness +
                                 (1.0f - keys->emitter.wave.initialAmplitudeRandomness));
                            finalAmplitude = keys->emitter.wave.finalAmplitude *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.wave.finalAmplitudeRandomness +
                                 (1.0f - keys->emitter.wave.finalAmplitudeRandomness));
                            motion->amplitude = initialAmplitude;
                            motion->amplitudeStep = (finalAmplitude - initialAmplitude) / (f32)length;
                            motion->phaseStep = keys->emitter.wave.phaseStep *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.wave.phaseStepRandomness +
                                 (1.0f - keys->emitter.wave.phaseStepRandomness));
                            motion->phase = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                            motion->previousSine = sdfSin(motion->phase);
                            radius = keys->emitter.wave.initialRadius *
                                ((dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f);
                            VEC3_SPLAT(radiusVector, radius);
                            if (localSpace != 0) {
                                SDF_VU0_LOADVECTOR(direction);
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                            } else {
                                SDF_VU0_LOADVECTOR(direction);
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                                SDF_VU0_LOADVECTOR_B(keys->pos);
                                SDF_VU0_ADDVECTOR();
                            }
                            SDF_VU0_STOREVECTOR(slot->pos);
                            motion->scaleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                                    if (dds3GetRandA(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                f32 age = (f32)(dds3GetRandA(&effSharedRandomState) % prewarmLength);
                                distance = motion->speed * age;
                                distance -= speedDecrease * age * age * 0.5f;
                                slot->pos[0] += motion->axis[0] * distance;
                                slot->pos[1] += motion->axis[1] * distance;
                                slot->pos[2] += motion->axis[2] * distance;
                                motion->phase += motion->phaseStep * age;
                                motion->amplitude += motion->amplitudeStep * age;
                                sine = sdfSin(motion->phase);
                                sway = motion->amplitude * sine;
                                slot->state = (s32)age;
                                slot->pos[0] += motion->swayDirection[0] * sway;
                                slot->pos[1] += motion->swayDirection[1] * sway;
                                slot->pos[2] += motion->swayDirection[2] * sway;
                                motion->previousSine = sine;
                                motion->phase += motion->phaseStep;
                                motion->amplitude += motion->amplitudeStep;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                        sine = sdfSin(motion->phase);
                        sway = motion->amplitude * (sine - motion->previousSine);
                        slot->pos[0] += motion->swayDirection[0] * sway;
                        slot->pos[1] += motion->swayDirection[1] * sway;
                        slot->pos[2] += motion->swayDirection[2] * sway;
                        distance = motion->speed - speedDecrease * (f32)frame;
                        slot->pos[0] += motion->axis[0] * distance;
                        slot->pos[1] += motion->axis[1] * distance;
                        slot->pos[2] += motion->axis[2] * distance;
                        motion->previousSine = sine;
                        motion->phase += motion->phaseStep;
                        motion->amplitude += motion->amplitudeStep;
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        effUpdateGridRecordSlotHistory(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}


/* Scale selected wave-emitter parameters from source to destination. */
void effScaleWaveEmitterParameters(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkCC = src->unkCC * scale;
    dst->unkD4 = src->unkD4 * scale;
    dst->unkD8 = src->unkD8 * scale;
    dst->unkE0 = src->unkE0 * scale;
}

typedef struct FileSlotMotion {
    f32 direction[4];     /* 0x00; this callback initializes xyz only */
    f32 speed;            /* 0x10 */
    f32 scaleMultiplier;  /* 0x14 */
    f32 angle;            /* 0x18 */
    f32 angleMultiplier;  /* 0x1C */
} FileSlotMotion;

/* vu0 routine: emits and advances randomized radial slots, then samples their key tracks. */
void effUpdateRadialEmitterSlots(FileSlotTable *record) {
    f32 delta[4];
    f32 radiusVector[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileSlotMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 gravity;
    f32 acceleration;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 distance;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileSlotMotion *)record->instanceWork;

    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        gravity = keys->emitter.radial.gravity;
        acceleration = keys->emitter.radial.acceleration;
        delta[3] = 0.0f;
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                toSpawn = count;
                if (keys->spawnVariance > 0.0f) {
                    toSpawn = (s32)((f32)record->instances *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            delta[0] = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f;
                            delta[1] = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f;
                            delta[2] = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f;
                            SDF_VU0_LOADVECTOR(delta);
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR(delta);
                            motion->direction[0] = delta[0];
                            motion->direction[1] = delta[1];
                            motion->direction[2] = delta[2];
                            motion->speed = fabsf(keys->emitter.radial.speed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.radial.speedRandomness + (1.0f - keys->emitter.radial.speedRandomness)));
                            {
                                f32 radius = keys->emitter.radial.radius *
                                    (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.radial.radiusRandomness + (1.0f - keys->emitter.radial.radiusRandomness));
                                VEC3_SPLAT(radiusVector, radius);
                            }
                            /* Retail initializes only XYZ of this radius vector; W is left untouched. */
                            SDF_VU0_LOADVECTOR(delta);
                            if (keys->emitter.radial.speed < 0.0f) {
                                SDF_VU0_NEGVECTOR_XYZ();
                            }
                            if (flags & 1) {
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                            } else {
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                                SDF_VU0_LOADVECTOR_B(keys->pos);
                                SDF_VU0_ADDVECTOR();
                            }
                            SDF_VU0_STOREVECTOR(slot->pos);
                            motion->scaleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                                    if (dds3GetRandA(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                f32 age = (f32)(dds3GetRandA(&effSharedRandomState) % prewarmLength);
                                SDF_VU0_LOADVECTOR_B(slot->pos);
                                distance = motion->speed * age;
                                distance += acceleration * age * age * 0.5f;
                                if (distance < 0.0f) {
                                    distance = 0.0f;
                                }
                                delta[0] = distance * motion->direction[0];
                                delta[1] = distance * motion->direction[1];
                                delta[2] = distance * motion->direction[2];
                                delta[1] += gravity * age * age * 0.5f;
                                SDF_VU0_LOADVECTOR(delta);
                                SDF_VU0_ADDVECTOR();
                                SDF_VU0_STOREVECTOR(slot->pos);
                                slot->state = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        f32 age;
                        SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                        SDF_VU0_LOADVECTOR_B(slot->pos);
                        age = (f32)frame;
                        distance = motion->speed + acceleration * age;
                        if (distance < 0.0f) {
                            distance = 0.0f;
                        }
                        delta[0] = distance * motion->direction[0];
                        delta[1] = distance * motion->direction[1];
                        delta[2] = distance * motion->direction[2];
                        delta[1] += gravity * age;
                        SDF_VU0_LOADVECTOR(delta);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(slot->pos);
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        effUpdateGridRecordSlotHistory(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}

/* Scale selected radial-emitter parameters from source to destination. */
void effScaleRadialEmitterParameters(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkD0 = src->unkD0 * scale;
    dst->unkD8 = src->unkD8 * scale;
    dst->unkDC = src->unkDC * scale;
}

typedef struct FileCircularMotion {
    f32 origin[4];          /* 0x00: xyz only */
    f32 orientation[4];     /* 0x10: saved emitter quaternion */
    f32 height;             /* 0x20 */
    f32 radius;
    f32 radiusStep;
    f32 initialPhase;
    f32 angularSpeed;
    f32 axialSpeed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileCircularMotion;       /* 0x44, ordinary word alignment */


/* vu0 routine: advances changing-radius circular motion in the saved emitter frame. */
void effUpdateCircularEmitterSlots(FileSlotTable *record) {
    f32 direction[4];
    f32 radiusVector[4];
    f32 orientation[4];
    f32 initialOrientation[4];
    f32 position[4];
    f32 origin[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileCircularMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 gravity;
    f32 angularAcceleration;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 age;
    f32 phase;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileCircularMotion *)record->instanceWork;
    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        gravity = keys->emitter.circular.gravity;
        angularAcceleration = keys->emitter.circular.angularAcceleration;
        direction[3] = 0.0f;
        position[3] = 0.0f;
        radiusVector[3] = 0.0f;
        SDF_VU0_COPYVECTOR(origin, keys->pos);
        SDF_VU0_COPYVECTOR(initialOrientation, keys->orientation);
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                toSpawn = count;
                if (keys->spawnVariance > 0.0f) {
                    toSpawn = (s32)((f32)record->instances *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            f32 initialRadius;
                            f32 finalRadius;
                            s32 localSpace;

                            initialRadius = keys->emitter.circular.initialRadius *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.circular.initialRadiusRandomness +
                                 (1.0f - keys->emitter.circular.initialRadiusRandomness));
                            finalRadius = keys->emitter.circular.finalRadius *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.circular.finalRadiusRandomness +
                                 (1.0f - keys->emitter.circular.finalRadiusRandomness));
                            motion->radius = initialRadius;
                            motion->radiusStep = (finalRadius - initialRadius) / (f32)length;
                            motion->initialPhase = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                            motion->angularSpeed = keys->emitter.circular.angularSpeed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.circular.angularSpeedRandomness +
                                 (1.0f - keys->emitter.circular.angularSpeedRandomness));
                            motion->axialSpeed = keys->emitter.circular.axialSpeed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.circular.axialSpeedRandomness +
                                 (1.0f - keys->emitter.circular.axialSpeedRandomness));
                            direction[0] = sdfCos(motion->initialPhase);
                            direction[1] = 0.0f;
                            direction[2] = sdfSin(motion->initialPhase);
                            motion->height = -keys->emitter.circular.initialAxialExtent * dds3GetRandFloatA(&effSharedRandomState);
                            VEC3_SPLAT(radiusVector, initialRadius);
                            SDF_VU0_LOADVECTOR(direction);
                            SDF_VU0_LOADVECTOR_B(radiusVector);
                            SDF_VU0_MULVECTOR();
                            VU0_SET_VF10_COMPONENT(y, motion->height);
                            localSpace = flags & 1;
                            if (localSpace == 0) {
                                motion->origin[0] = origin[0];
                                motion->origin[1] = origin[1];
                                motion->origin[2] = origin[2];
                                motion->orientation[0] = initialOrientation[0];
                                motion->orientation[1] = initialOrientation[1];
                                motion->orientation[2] = initialOrientation[2];
                                motion->orientation[3] = initialOrientation[3];
                                SDF_VU0_LOADVECTOR_B(origin);
                                SDF_VU0_ADDVECTOR();
                            }
                            SDF_VU0_STOREVECTOR(slot->pos);
                            motion->scaleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                                    if (dds3GetRandA(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                age = (f32)(dds3GetRandA(&effSharedRandomState) % prewarmLength);
                                motion->height = motion->height - motion->axialSpeed * age + gravity * age * age * 0.5f;
                                motion->radius += motion->radiusStep * age;
                                position[0] = sdfCos(motion->initialPhase) * motion->radius;
                                position[1] = motion->height;
                                position[2] = sdfSin(motion->initialPhase) * motion->radius;
                                if (localSpace == 0) {
                                    SDF_VU0_LOADVECTOR(initialOrientation);
                                    effMiscQuaternionToMatrixVU();
                                    SDF_VU0_LOADVECTOR(position);
                                    SDF_VU0_APPLYMATRIX_XYZ();
                                    SDF_VU0_LOADVECTOR_B(origin);
                                    SDF_VU0_ADDVECTOR();
                                } else {
                                    SDF_VU0_LOADVECTOR(position);
                                }
                                SDF_VU0_STOREVECTOR(slot->pos);
                                slot->state = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                        age = (f32)frame;
                        motion->height = motion->height - motion->axialSpeed + gravity * age;
                        motion->radius += motion->radiusStep;
                        phase = (motion->angularSpeed + angularAcceleration * age * 0.5f) * age + motion->initialPhase;
                        position[0] = sdfCos(phase) * motion->radius;
                        position[1] = motion->height;
                        position[2] = sdfSin(phase) * motion->radius;
                        if (!(flags & 1)) {
                            orientation[0] = motion->orientation[0];
                            orientation[1] = motion->orientation[1];
                            orientation[2] = motion->orientation[2];
                            orientation[3] = motion->orientation[3];
                            SDF_VU0_LOADVECTOR(orientation);
                            effMiscQuaternionToMatrixVU();
                            SDF_VU0_LOADVECTOR(position);
                            SDF_VU0_APPLYMATRIX_XYZ();
                            SDF_VU0_STOREVECTOR(position);
                            position[0] += motion->origin[0];
                            position[1] += motion->origin[1];
                            position[2] += motion->origin[2];
                        }
                        slot->pos[0] = position[0];
                        slot->pos[1] = position[1];
                        slot->pos[2] = position[2];
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        effUpdateGridRecordSlotHistory(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}

/* Scale selected circular-emitter parameters from source to destination. */
void effScaleCircularEmitterParameters(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkCC = src->unkCC * scale;
    dst->unkD4 = src->unkD4 * scale;
    dst->unkE4 = src->unkE4 * scale;
    dst->unkF0 = src->unkF0 * scale;
}

typedef struct FileOrientedRingMotion {
    f32 origin[4];          /* 0x00: xyz only */
    f32 orientationAngle;   /* 0x10 */
    f32 tiltAngle;
    f32 radius;
    f32 radiusStep;
    f32 angularSpeed;
    f32 initialPhase;
    f32 height;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileOrientedRingMotion;   /* 0x38, ordinary word alignment */

extern void SetRotateMatrixX(f32 angle);
extern void sdfVuLoadSecondaryRotationMatrixY(f32 angle);

/* vu0 routine: advances a ring in its sampled orientation with vertical acceleration. */
void effUpdateOrientedRingEmitterSlots(FileSlotTable *record) {
    f32 direction[4];
    f32 radiusVector[4];
    f32 position[4];
    f32 origin[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileOrientedRingMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 gravity;
    f32 angularAcceleration;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 age;
    f32 phase;
    f32 radius;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileOrientedRingMotion *)record->instanceWork;
    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        gravity = keys->emitter.orientedRing.gravity;
        angularAcceleration = keys->emitter.orientedRing.angularAcceleration;
        direction[3] = 0.0f;
        position[3] = 0.0f;
        /* Retail also initializes this scratch vector's W; its XYZ are unused here. */
        radiusVector[3] = 0.0f;
        SDF_VU0_COPYVECTOR(origin, keys->pos);
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                toSpawn = count;
                if (keys->spawnVariance > 0.0f) {
                    toSpawn = (s32)((f32)record->instances *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            f32 initialRadius;
                            f32 finalRadius;
                            s32 localSpace;

                            initialRadius = keys->emitter.orientedRing.initialRadius *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.orientedRing.initialRadiusRandomness +
                                 (1.0f - keys->emitter.orientedRing.initialRadiusRandomness));
                            finalRadius = keys->emitter.orientedRing.finalRadius *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.orientedRing.finalRadiusRandomness +
                                 (1.0f - keys->emitter.orientedRing.finalRadiusRandomness));
                            motion->radius = initialRadius;
                            motion->radiusStep = (finalRadius - initialRadius) / (f32)length;
                            motion->angularSpeed = keys->emitter.orientedRing.angularSpeed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.orientedRing.angularSpeedRandomness +
                                 (1.0f - keys->emitter.orientedRing.angularSpeedRandomness));
                            motion->initialPhase = dds3GetRandFloatA(&effSharedRandomState) * 1.5707962512969971f;
                            motion->height = 0.0f;
                            motion->orientationAngle = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                            motion->tiltAngle = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                            SetRotateMatrixX(motion->orientationAngle);
                            sdfVuLoadSecondaryRotationMatrixY(motion->tiltAngle);
                            MultMatrixRevNoArg();
                            direction[0] = sdfCos(motion->initialPhase) * initialRadius;
                            direction[1] = 0.0f;
                            direction[2] = sdfSin(motion->initialPhase) * initialRadius;
                            SDF_VU0_LOADVECTOR(direction);
                            SDF_VU0_APPLYMATRIX_XYZ();
                            localSpace = flags & 1;
                            if (localSpace == 0) {
                                motion->origin[0] = origin[0];
                                motion->origin[1] = origin[1];
                                motion->origin[2] = origin[2];
                                SDF_VU0_LOADVECTOR_B(origin);
                                SDF_VU0_ADDVECTOR();
                            }
                            SDF_VU0_STOREVECTOR(slot->pos);
                            motion->scaleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                                    if (dds3GetRandA(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                age = (f32)(dds3GetRandA(&effSharedRandomState) % prewarmLength);
                                motion->height += gravity * age * age * 0.5f;
                                motion->radius += motion->radiusStep * age;
                                radius = motion->radius;
                                direction[0] = sdfCos(motion->initialPhase) * radius;
                                direction[1] = 0.0f;
                                direction[2] = sdfSin(motion->initialPhase) * radius;
                                SDF_VU0_LOADVECTOR(direction);
                                SDF_VU0_APPLYMATRIX_XYZ();
                                if (localSpace == 0) {
                                    SDF_VU0_COPYVECTOR_A_TO_B();
                                    SDF_VU0_LOADVECTOR(origin);
                                    SDF_VU0_ADDVECTOR();
                                }
                                SDF_VU0_STOREVECTOR(slot->pos);
                                slot->state = (s32)age;
                                slot->pos[1] += motion->height;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                        age = (f32)frame;
                        motion->height += gravity * age;
                        motion->radius += motion->radiusStep;
                        phase = (motion->angularSpeed + angularAcceleration * age * 0.5f) * age + motion->initialPhase;
                        SetRotateMatrixX(motion->orientationAngle);
                        sdfVuLoadSecondaryRotationMatrixY(motion->tiltAngle);
                        MultMatrixRevNoArg();
                        radius = motion->radius;
                        direction[0] = sdfCos(phase) * radius;
                        direction[1] = 0.0f;
                        direction[2] = sdfSin(phase) * radius;
                        SDF_VU0_LOADVECTOR(direction);
                        SDF_VU0_APPLYMATRIX_XYZ();
                        SDF_VU0_STOREVECTOR(position);
                        if (!(flags & 1)) {
                            position[0] += motion->origin[0];
                            position[1] += motion->origin[1];
                            position[2] += motion->origin[2];
                        }
                        slot->pos[0] = position[0];
                        slot->pos[1] = position[1] + motion->height;
                        slot->pos[2] = position[2];
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        effUpdateGridRecordSlotHistory(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}

/* Scale selected oriented-ring parameters; keep the destination-first declarations. */
void effScaleOrientedRingEmitterParameters(ScaleOwner *owner, f32 scale) {
    ScaleSet *dst = owner->dst;
    ScaleSet *src = owner->src;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkD0 = src->unkD0 * scale;
    dst->unkE4 = src->unkE4 * scale;
}

/* vu0 routine: advances directed slots through the emitter quaternion frame. */
void effUpdateDirectedEmitterSlots(FileSlotTable *record) {
    f32 delta[4];
    f32 radiusVector[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileSlotMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 gravity;
    f32 acceleration;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 distance;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileSlotMotion *)record->instanceWork;

    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        gravity = keys->emitter.directed.gravity;
        acceleration = keys->emitter.directed.acceleration;
        delta[3] = 0.0f;
        SDF_VU0_LOADVECTOR(keys->orientation);
        effMiscQuaternionToMatrixVU();
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                if (!(keys->spawnVariance > 0.0f)) {
                    toSpawn = record->instances;
                } else {
                    toSpawn = (s32)((f32)record->instances *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            f32 spread;
                            s32 localSpace;

                            spread = keys->emitter.directed.spread *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.directed.spreadRandomness +
                                 (1.0f - keys->emitter.directed.spreadRandomness));
                            delta[0] = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f * spread;
                            delta[1] = -(1.0f - spread);
                            delta[2] = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f * spread;
                            SDF_VU0_LOADVECTOR(delta);
                            SDF_VU0_NORMALIZE();
                            localSpace = flags & 1;
                            if (localSpace == 0) {
                                SDF_VU0_APPLYMATRIX_XYZ();
                            }
                            SDF_VU0_STOREVECTOR(delta);
                            motion->direction[0] = delta[0];
                            motion->direction[1] = delta[1];
                            motion->direction[2] = delta[2];
                            motion->speed = fabsf(keys->emitter.directed.speed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.directed.speedRandomness + (1.0f - keys->emitter.directed.speedRandomness)));
                            {
                                f32 radius = keys->emitter.directed.radius *
                                    (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.directed.radiusRandomness + (1.0f - keys->emitter.directed.radiusRandomness));
                                VEC3_SPLAT(radiusVector, radius);
                            }
                            /* Retail initializes only XYZ of this radius vector; W is left untouched. */
                            SDF_VU0_LOADVECTOR(delta);
                            if (keys->emitter.directed.speed < 0.0f) {
                                SDF_VU0_NEGVECTOR_XYZ();
                            }
                            if (localSpace != 0) {
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                            } else {
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                                SDF_VU0_LOADVECTOR_B(keys->pos);
                                SDF_VU0_ADDVECTOR();
                            }
                            SDF_VU0_STOREVECTOR(slot->pos);
                            motion->scaleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                                    if (dds3GetRandA(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                f32 age = (f32)(dds3GetRandA(&effSharedRandomState) % prewarmLength);
                                SDF_VU0_LOADVECTOR_B(slot->pos);
                                distance = motion->speed * age;
                                distance += acceleration * age * age * 0.5f;
                                if (distance < 0.0f) {
                                    distance = 0.0f;
                                }
                                delta[0] = distance * motion->direction[0];
                                delta[1] = distance * motion->direction[1];
                                delta[2] = distance * motion->direction[2];
                                delta[1] += gravity * age * age * 0.5f;
                                SDF_VU0_LOADVECTOR(delta);
                                SDF_VU0_ADDVECTOR();
                                SDF_VU0_STOREVECTOR(slot->pos);
                                slot->state = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        f32 age;
                        SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                        SDF_VU0_LOADVECTOR_B(slot->pos);
                        age = (f32)frame;
                        distance = motion->speed + acceleration * age;
                        if (distance < 0.0f) {
                            distance = 0.0f;
                        }
                        delta[0] = distance * motion->direction[0];
                        delta[1] = distance * motion->direction[1];
                        delta[2] = distance * motion->direction[2];
                        delta[1] += gravity * age;
                        SDF_VU0_LOADVECTOR(delta);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(slot->pos);
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        effUpdateGridRecordSlotHistory(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}

/* Scale selected directed-emitter parameters; this is the DDS1 callback's twin. */
void effScaleDirectedEmitterParameters(ScaleOwner *owner, f32 factor) {
    f32 *source = (f32 *)owner->src;
    f32 *destination = (f32 *)owner->dst;
    u8 *sourceEntries = (u8 *)source + 4;
    u8 *destinationEntries = (u8 *)destination + 4;
    u32 index = 0;
    u32 offset = 0x70;

    destination[0x64 / 4] = source[0x64 / 4] * factor;
    destination[0x68 / 4] = source[0x68 / 4] * factor;
    do {
        *(f32 *)(destinationEntries + offset) = *(f32 *)(sourceEntries + offset) * factor;
        index++;
        offset += 8;
    } while (index < 3);
    destination[0xC8 / 4] = source[0xC8 / 4] * factor;
    destination[0xD8 / 4] = source[0xD8 / 4] * factor;
    destination[0xE0 / 4] = source[0xE0 / 4] * factor;
    destination[0xE4 / 4] = source[0xE4 / 4] * factor;
}

/* vu0 routine: advances slots within a bounded azimuth of the emitter frame. */
void effUpdateSectorEmitterSlots(FileSlotTable *record) {
    f32 delta[4];
    f32 radiusVector[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileSlotMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 gravity;
    f32 acceleration;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 distance;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileSlotMotion *)record->instanceWork;

    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        gravity = keys->emitter.sector.gravity;
        acceleration = keys->emitter.sector.acceleration;
        delta[3] = 0.0f;
        SDF_VU0_LOADVECTOR(keys->orientation);
        effMiscQuaternionToMatrixVU();
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                if (!(keys->spawnVariance > 0.0f)) {
                    toSpawn = record->instances;
                } else {
                    toSpawn = (s32)((f32)record->instances *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            f32 angle;
                            f32 spread;
                            s32 localSpace;

                            angle = (f32)keys->emitter.sector.azimuthDegrees *
                                ((dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f) *
                                0.5f * 0.0174532925f - 1.5707962512969971f;
                            spread = keys->emitter.sector.spread *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.sector.spreadRandomness +
                                 (1.0f - keys->emitter.sector.spreadRandomness));
                            delta[0] = sdfCos(angle) * spread;
                            delta[1] = -(1.0f - spread);
                            delta[2] = sdfSin(angle) * spread;
                            SDF_VU0_LOADVECTOR(delta);
                            SDF_VU0_NORMALIZE();
                            localSpace = flags & 1;
                            if (localSpace == 0) {
                                SDF_VU0_APPLYMATRIX_XYZ();
                            }
                            SDF_VU0_STOREVECTOR(delta);
                            motion->direction[0] = delta[0];
                            motion->direction[1] = delta[1];
                            motion->direction[2] = delta[2];
                            motion->speed = fabsf(keys->emitter.sector.speed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.sector.speedRandomness + (1.0f - keys->emitter.sector.speedRandomness)));
                            {
                                f32 radius = keys->emitter.sector.radius *
                                    (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.sector.radiusRandomness + (1.0f - keys->emitter.sector.radiusRandomness));
                                VEC3_SPLAT(radiusVector, radius);
                            }
                            /* Retail initializes only XYZ of this radius vector; W is left untouched. */
                            SDF_VU0_LOADVECTOR(delta);
                            if (keys->emitter.sector.speed < 0.0f) {
                                SDF_VU0_NEGVECTOR_XYZ();
                            }
                            if (localSpace != 0) {
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                            } else {
                                SDF_VU0_LOADVECTOR_B(radiusVector);
                                SDF_VU0_MULVECTOR();
                                SDF_VU0_LOADVECTOR_B(keys->pos);
                                SDF_VU0_ADDVECTOR();
                            }
                            SDF_VU0_STOREVECTOR(slot->pos);
                            motion->scaleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                                    if (dds3GetRandA(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                f32 age = (f32)(dds3GetRandA(&effSharedRandomState) % prewarmLength);
                                SDF_VU0_LOADVECTOR_B(slot->pos);
                                distance = motion->speed * age;
                                distance += acceleration * age * age * 0.5f;
                                if (distance < 0.0f) {
                                    distance = 0.0f;
                                }
                                delta[0] = distance * motion->direction[0];
                                delta[1] = distance * motion->direction[1];
                                delta[2] = distance * motion->direction[2];
                                delta[1] += gravity * age * age * 0.5f;
                                SDF_VU0_LOADVECTOR(delta);
                                SDF_VU0_ADDVECTOR();
                                SDF_VU0_STOREVECTOR(slot->pos);
                                slot->state = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        f32 age;
                        SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                        SDF_VU0_LOADVECTOR_B(slot->pos);
                        age = (f32)frame;
                        distance = motion->speed + acceleration * age;
                        if (distance < 0.0f) {
                            distance = 0.0f;
                        }
                        delta[0] = distance * motion->direction[0];
                        delta[1] = distance * motion->direction[1];
                        delta[2] = distance * motion->direction[2];
                        delta[1] += gravity * age;
                        SDF_VU0_LOADVECTOR(delta);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(slot->pos);
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        effUpdateGridRecordSlotHistory(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}


/* Scale selected parameters for the bounded-azimuth sector emitter. */
void effScaleSectorEmitterParameters(ScaleOwner *owner, f32 factor) {
    f32 *source = (f32 *)owner->src;
    f32 *destination = (f32 *)owner->dst;
    u8 *sourceEntries = (u8 *)source + 4;
    u8 *destinationEntries = (u8 *)destination + 4;
    u32 index = 0;
    u32 offset = 0x70;

    destination[0x64 / 4] = source[0x64 / 4] * factor;
    destination[0x68 / 4] = source[0x68 / 4] * factor;
    do {
        *(f32 *)(destinationEntries + offset) = *(f32 *)(sourceEntries + offset) * factor;
        index++;
        offset += 8;
    } while (index < 3);
    destination[0xC8 / 4] = source[0xC8 / 4] * factor;
    destination[0xD8 / 4] = source[0xD8 / 4] * factor;
    destination[0xE0 / 4] = source[0xE0 / 4] * factor;
    destination[0xE4 / 4] = source[0xE4 / 4] * factor;
}

/* Replace type-seven curve-emitter start xyz; other record types are untouched. */
void effSetCurveEmitterStartPosition(FileSlotTable *record, const f32 *value) {
    if (record->type == 7) {
        f32 *sprite = (f32 *)record->data0;
        sprite[0xFC / 4] = value[0];
        sprite[0x100 / 4] = value[1];
        sprite[0x104 / 4] = value[2];
    }
}

/* Replace type-seven curve-emitter end xyz; other record types are untouched. */
void effSetCurveEmitterEndPosition(FileSlotTable *record, const f32 *value) {
    if (record->type == 7) {
        f32 *sprite = (f32 *)record->data0;
        sprite[0x108 / 4] = value[0];
        sprite[0x10C / 4] = value[1];
        sprite[0x110 / 4] = value[2];
    }
}

/* cubic Bezier point at t through four control points; result is left in vf10 */
void vu0CubicBezierPoint(f32 *controlPoint0, f32 *controlPoint1,
                         f32 *controlPoint2, f32 *controlPoint3, f32 t) {
    f32 weight[4];
    f32 point[4];
    f32 t2 = t * t;
    f32 t3 = t2 * t;
    f32 u = 1.0f - t;
    f32 u2 = u * u;

    weight[0] = u2 * u;
    weight[1] = t * u2 * 3.0f;
    weight[2] = t2 * u * 3.0f;
    weight[3] = t3;
    point[0] = controlPoint0[0] * weight[0] + controlPoint1[0] * weight[1] + controlPoint2[0] * weight[2] + controlPoint3[0] * weight[3];
    point[1] = controlPoint0[1] * weight[0] + controlPoint1[1] * weight[1] + controlPoint2[1] * weight[2] + controlPoint3[1] * weight[3];
    point[2] = controlPoint0[2] * weight[0] + controlPoint1[2] * weight[1] + controlPoint2[2] * weight[2] + controlPoint3[2] * weight[3];
    SDF_VU0_LOADVECTOR(point);
}

typedef struct FileCurveMotion {
    f32 firstControl[3];
    f32 secondControl[3];
    f32 rotationReference[3]; /* native live-path input; writer unresolved */
    f32 endpointOffset[3];
    f32 angularSpeed;
    f32 travelSpeed;
    f32 pathLength;
    f32 progress;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileCurveMotion; /* 0x4C, naturally four-byte aligned */

extern f32 effWorldUpVector[4];

/* vu0 routine: advances randomized cubic-curve slots and their axial rotation. */
void effUpdateCurveEmitterSlots(FileSlotTable *record) {
    f32 axis[4];
    f32 previousPosition[4];
    f32 start[4];
    f32 endpoint[4];
    f32 lateral[4];
    f32 point[4];
    f32 firstPoint[4];
    f32 secondPoint[4];
    f32 sampledEndpoint[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    FileSlot *slot;
    FileCurveMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 travelAcceleration;
    f32 angularAcceleration;
    s32 prewarmLength;
    s32 toSpawn;
    s32 frame;
    f32 age;
    f32 travel;
    f32 rotation;
    f32 progress;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    slot = record->slots;
    motion = (FileCurveMotion *)record->instanceWork;

    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        angularAcceleration = keys->emitter.curve.angularAcceleration;
        travelAcceleration = keys->emitter.curve.travelAcceleration;
        start[0] = keys->emitter.curve.start[0];
        start[1] = keys->emitter.curve.start[1];
        start[2] = keys->emitter.curve.start[2];
        SDF_VU0_LOADVECTOR(keys->orientation);
        effMiscQuaternionToMatrixVU();
        if (duration != 0 && (s32)record->references >= duration) {
            prewarm = 0;
            toSpawn = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                if (!(keys->spawnVariance > 0.0f)) {
                    toSpawn = record->instances;
                } else {
                    toSpawn = (s32)((f32)record->instances *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (dds3GetRandFloatA(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            f32 angle;
                            f32 offset;

                            angle = (dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                            motion->endpointOffset[0] = sdfCos(angle) * keys->emitter.curve.endpointRadius;
                            motion->endpointOffset[1] = -keys->emitter.curve.endpointDrop;
                            motion->endpointOffset[2] = sdfSin(angle) * keys->emitter.curve.endpointRadius;
                            endpoint[0] = keys->emitter.curve.end[0] + motion->endpointOffset[0];
                            endpoint[1] = keys->emitter.curve.end[1] + motion->endpointOffset[1];
                            endpoint[2] = keys->emitter.curve.end[2] + motion->endpointOffset[2];
                            SDF_VU0_LOADVECTOR_C(start);
                            SDF_VU0_COPYVECTOR_C_TO_A();
                            SDF_VU0_LOADVECTOR_B(endpoint);
                            VU0_LERP_VF10((f32)keys->emitter.curve.firstPercent / 100.0f);
                            SDF_VU0_STOREVECTOR(firstPoint);
                            SDF_VU0_COPYVECTOR_C_TO_A();
                            VU0_LERP_VF10((f32)keys->emitter.curve.secondPercent / 100.0f);
                            SDF_VU0_STOREVECTOR(secondPoint);
                            SDF_VU0_COPYVECTOR_C_TO_A();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_GETLENGTH(motion->pathLength);
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR(axis);
                            SDF_VU0_COPYVECTOR_A_TO_C();
                            SDF_VU0_LOADVECTOR_B(effWorldUpVector);
                            SDF_VU0_OUTERPRODUCT();
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            SDF_VU0_OUTERPRODUCT();
                            SDF_VU0_STOREVECTOR(lateral);
                            angle = (f32)keys->emitter.curve.spreadDegrees *
                                ((dds3GetRandFloatA(&effSharedRandomState) - 0.5f) * 2.0f) *
                                0.5f * 0.0174532925f;
                            SetRotateMatrixAxis(angle, (const struct RwV3d *)axis);
                            offset = keys->emitter.curve.firstOffset *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.curve.firstRandomness +
                                 (1.0f - keys->emitter.curve.firstRandomness));
                            SDF_VU0_LOADVECTOR(firstPoint);
                            SDF_VU0_LOADVECTOR_C(lateral);
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            VU0_SCALE_VF(vf11, offset);
                            VU0_ROTATE_VEC(vf11, vf11);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(point);
                            motion->firstControl[0] = point[0];
                            motion->firstControl[1] = point[1];
                            motion->firstControl[2] = point[2];
                            offset = keys->emitter.curve.secondOffset *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.curve.secondRandomness +
                                 (1.0f - keys->emitter.curve.secondRandomness));
                            SDF_VU0_LOADVECTOR(secondPoint);
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            VU0_SCALE_VF(vf11, offset);
                            VU0_ROTATE_VEC(vf11, vf11);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(point);
                            motion->secondControl[0] = point[0];
                            motion->secondControl[1] = point[1];
                            motion->secondControl[2] = point[2];
                            motion->travelSpeed = keys->emitter.curve.travelSpeed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.curve.travelRandomness +
                                 (1.0f - keys->emitter.curve.travelRandomness));
                            motion->angularSpeed = keys->emitter.curve.angularSpeed *
                                (dds3GetRandFloatA(&effSharedRandomState) * keys->emitter.curve.angularRandomness +
                                 (1.0f - keys->emitter.curve.angularRandomness));
                            motion->progress = 0.0f;
                            SDF_VU0_COPYVECTOR(slot->pos, start);
                            motion->scaleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = dds3GetRandFloatA(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = dds3GetRandFloatA(&effSharedRandomState) * 6.2831850051879883f;
                                    if (dds3GetRandA(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                age = (f32)(dds3GetRandA(&effSharedRandomState) % prewarmLength);
                                travel = motion->travelSpeed * age;
                                travel += travelAcceleration * age * age * 0.5f;
                                if (travel < 0.0f) {
                                    travel = 0.0f;
                                }
                                rotation = motion->angularSpeed * age;
                                rotation += angularAcceleration * age * age * 0.5f;
                                if (rotation < 0.0f) {
                                    rotation = 0.0f;
                                }
                                progress = travel / 1000.0f;
                                if (progress > 1.0f) {
                                    progress = 1.0f;
                                }
                                if (progress < motion->progress) {
                                    progress = motion->progress;
                                } else {
                                    motion->progress = progress;
                                }
                                sampledEndpoint[0] = endpoint[0];
                                sampledEndpoint[1] = endpoint[1];
                                sampledEndpoint[2] = endpoint[2];
                                vu0CubicBezierPoint(keys->emitter.curve.start, motion->firstControl,
                                    motion->secondControl, sampledEndpoint, progress);
                                if (rotation != 0.0f) {
                                    SDF_VU0_STOREVECTOR(point);
                                    SetRotateMatrixAxis(rotation, (const struct RwV3d *)axis);
                                    SDF_VU0_LOADVECTOR_B(axis);
                                    VU0_SCALE_VF(vf11, motion->pathLength * progress);
                                    SDF_VU0_LOADVECTOR(point);
                                    SDF_VU0_SUBVECTOR();
                                    SDF_VU0_APPLYMATRIX_XYZ();
                                    SDF_VU0_ADDVECTOR();
                                }
                                SDF_VU0_STOREVECTOR(slot->pos);
                                slot->state = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        SDF_VU0_COPYVECTOR(previousPosition, slot->pos);
                        age = (f32)frame;
                        travel = motion->travelSpeed * age;
                        travel += travelAcceleration * age * age * 0.5f;
                        if (travel < 0.0f) {
                            travel = 0.0f;
                        }
                        rotation = motion->angularSpeed * age;
                        rotation += angularAcceleration * age * age * 0.5f;
                        if (rotation < 0.0f) {
                            rotation = 0.0f;
                        }
                        progress = travel / 1000.0f;
                        if (progress > 1.0f) {
                            progress = 1.0f;
                        }
                        if (progress < motion->progress) {
                            progress = motion->progress;
                        } else {
                            motion->progress = progress;
                        }
                        sampledEndpoint[0] = keys->emitter.curve.end[0] + motion->endpointOffset[0];
                        sampledEndpoint[1] = keys->emitter.curve.end[1] + motion->endpointOffset[1];
                        sampledEndpoint[2] = keys->emitter.curve.end[2] + motion->endpointOffset[2];
                        vu0CubicBezierPoint(keys->emitter.curve.start, motion->firstControl,
                            motion->secondControl, sampledEndpoint, progress);
                        if (rotation != 0.0f) {
                            SDF_VU0_STOREVECTOR(point);
                            /* These native fields are read here without being written at spawn. */
                            endpoint[0] = motion->rotationReference[0];
                            endpoint[1] = motion->rotationReference[1];
                            endpoint[2] = motion->rotationReference[2];
                            SDF_VU0_LOADVECTOR(start);
                            SDF_VU0_LOADVECTOR_B(endpoint);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR(axis);
                            SetRotateMatrixAxis(rotation, (const struct RwV3d *)axis);
                            SDF_VU0_LOADVECTOR_B(axis);
                            VU0_SCALE_VF(vf11, motion->pathLength * progress);
                            SDF_VU0_LOADVECTOR(point);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_APPLYMATRIX_XYZ();
                            SDF_VU0_ADDVECTOR();
                        }
                        SDF_VU0_STOREVECTOR(slot->pos);
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        effUpdateGridRecordSlotHistory(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}



/* Curve-emitter scaling changes only the common scale-track values. */
void effScaleCurveEmitterParameters(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
}

FileSlotTable *fileAllocateGridRecordSlots(u16 type, u32 count, const void *data) {
    const FileKeyBlock *src = data;
    u32 slotCount = count * src->columns * src->rows + count;
    u32 slotBytes = slotCount << 5;
    s32 dataBytes = effEmitterTypeDescriptors[type].dataBytes;
    u32 headerSize = 0x30;
    u32 size;
    struct sdfMemHandle_t *allocation;
    FileSlotTable *rec;
    u8 *body;
    u8 *vec;

    size = slotBytes + headerSize;
    size += effEmitterTypeDescriptors[type].slotBytes * count;
    size += dataBytes * 2;
    allocation = sdfMemAllocDirect(size);
    rec = (void *)sdfMemLock(allocation);
    body = (u8 *)rec + headerSize;
    rec->type = type;
    rec->slots = (FileSlot *)body;
    body += slotBytes;
    rec->data0 = body;
    body += dataBytes;
    rec->data1 = body;
    body += dataBytes;
    rec->instanceWork = body;
    rec->instances = count;
    rec->count = slotCount;
    rec->allocation = allocation;
    rec->flags = 0;
    rec->references = 0;
    rec->spawnRemainder = 0.0f;
    memcpy(rec->data0, src, dataBytes);
    memcpy(rec->data1, src, dataBytes);
    vec = rec->data0;
    VU0_STORE_VF(vf0, vec);
    VU0_STORE_VF(vf0, vec + 0x10);
    if (vec[0xBC] != 0) {
        rec->flags |= 1;
    }
    return rec;
}

void effPCPHelperBEDGetModel(FileSlotTable *record) {
    sdfMemFreeDirect(record->allocation);
}

void fileClearRecordReferences(FileSlotTable *record) {
    record->references = 0;
}

void fileAcquireRecord(FileSlotTable *record) {
    if (record->references == 0) {
        fileResetSlotStates(record);
    }
    effEmitterTypeDescriptors[record->type].updateSlots(record);
    record->references++;
}

void fileReadVectorPtr20(u8 *obj, void *dst) {
    SDF_VU0_COPYVECTOR(dst, *(u8 **)(obj + 0x20));
}

void mnuRecordSetVector(void *record, const void *src) {
    SDF_VU0_COPYVECTOR(*(u8 **)((u8 *)record + 0x20), src);
}

void fileReadRecordSecondVector(u8 *obj, void *dst) {
    SDF_VU0_COPYVECTOR(dst, *(u8 **)(obj + 0x20) + 0x10);
}

void fileSetRecordSecondVector(void *record, const void *src) {
    SDF_VU0_COPYVECTOR(*(u8 **)((u8 *)record + 0x20) + 0x10, src);
}

INCLUDE_SDATA(const s32, "game/code_002C96D0", effModelUpdateControlFlags);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437E10);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437E18);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437E20);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437E28);
