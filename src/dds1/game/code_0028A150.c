/* Keeps its own declaration of the functions below (see sdk_libc.h note). */
#define SDK_SKIP_memset
#include "sdk_ps2.h"
#include "common.h"
#include "file_menu_start_mode.h"
#include "file_menu_state_result.h"
#include "file_config_list_node.h"
#include "file_config_option_api.h"
#include "sdf_vu0.h"
#include "file_job_type.h"
#include "file_config_task.h"
#include "sdf_vu_rotation_axes.h"
#include "file_load_menu_state.h"
#include "eff.h"
#include "cam_aim_record.h"
#include "eff_generated_texture_descriptor.h"
#include "dds3_world_selection.h"
#include "mnu_title_stream.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "eff_scale_params.h"
#include "ee_mmi.h"
#include "eff_random.h"
#include "sdf_textured_rect.h"
#include "itf_mes_window.h"
#include "evt_world.h"
#include "fr_font.h"
#include "fr_font_context.h"
#include "mc_poll.h"
#include "mc_read_api.h"
#include "mc_write_api.h"
#include "mc_path_api.h"
#include "bill_object_api.h"
#include "eff_resource_slots.h"
#include "eff_resource_source_lifetime.h"
#include "sdf_resource.h"
#include "sdf_chip.h"
#include "eff_curve.h"
#include "file.h"
#include "file_selection_api.h"
#include "file_request_entry.h"
#include "file_slot_flags.h"
#include "file_slot_flags_change_api.h"
#include "file_scroll_arrow_state.h"
#include "sdf_dev_state.h"
#include "file_slot.h"
#include "file_save_record.h"
#include "dat_state.h"
#include "dat_new_cycle_state.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "fpu.h"
#include "mnu_list.h"
#include "kwln_task_lifecycle.h"
#include "eff_expanded_list.h"
#include "file_request_api.h"
#include "mdl_flag_api.h"

#include "sdf.h"
#include "kwln_fade.h"
#include "file_pac.h"
#include "sdf_pac_work.h"
#include "btl_stage_task_cleanup.h"
#include "file_request_api.h"
#include "sdf_texture_file.h"
#include "mdl_flag_api.h"
#include "btl.h"
#include "fr_font.h"
#include "fr_font_draw_glyph.h"
#include "itf.h"

typedef s32 (*FileMenuContinuationCallback)(void);
/* Declared before first use (were implicit int). */
extern void effLoadFlashTextures(void);
extern void effLoadScalyTexture(void);
extern void effLoadWindTexture(void);
extern void dds3SoftResetOn(void);
extern void frEnableMode(s32 mask);
extern u8 frDisableMode(u8 mask);
extern void frSetFontDrawOT(u32 renderFlags);
extern s32 mcResetSlotMetadata(void);
extern void p2sdStopBGM(u32 trackHandle);
extern void *memset(void *dst, s32 value, s32 size);
typedef struct MenuResourceWork MenuResourceWork;
struct MenuListNode;
extern void mnuClearListFlagsOneAndTwo(u32 *flags);
struct EffectSlotSet;
extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, u32, struct EffectSlotSet *, s32, s32);

enum {
    FILE_CONFIG_SET,
    FILE_CONFIG_FRAME,
    FILE_CONFIG_X,
    FILE_CONFIG_Y
};
extern s16 fileConfigSpritePlacements[27][4];

extern FileScrollArrowState fileScrollArrowState;
extern f32 D_003BC83C;
extern u8 fileNewCycleLabel[];
extern u8 D_00347C68[][32];
extern u8 D_003482A8[][32];
extern char D_003BC898[];
extern char D_003BC8A0[];
extern char D_003BC8A8[];
extern u32 fileClampPackedColorByte(u32, s32, s8);
extern void fileDrawSlotIcon(s32, s32, s32, s32);
extern void fileCursorStepUp(void);
extern void fileFadeStepDown(void);
extern void fileLoadCtxSlideUpdate(void);



extern void dds3PadVibrationSet(s32, u8, s32);
extern struct sdfTexHandle_t *fileSavePreviewPortraitTextures[5];
extern struct sdfTexHandle_t *D_003BC868;
extern struct sdfTexHandle_t *D_003BC86C;
extern struct sdfTexHandle_t *D_003BC870;
extern struct sdfTexHandle_t *D_003BC874;
extern struct sdfTexHandle_t *D_003BC878;
extern struct sdfTexHandle_t *D_003BC87C;
extern struct sdfTexHandle_t *D_003BC880;
extern struct sdfTexHandle_t *D_003BC884;

extern u8 sdfViewTargetVector[];
extern u8 aViewTarget[];
extern u8 aViewDown[];







extern s32 slFormatStart(void);
extern s32 slFormatCancel(void);






/* Ten contiguous 0x30-byte save-preview records (0x1E0 bytes total). */
extern FileSavePreviewRecord fileSavePreviewRecords[10];

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 fileReadSlotPreviewBegin(void);
extern s32 fileAdvanceSlotScan(void);

extern FileMenuStateHandler fileMenuStateHandler;
extern s32 D_003BC814;

extern s32 fileDrawMenuFrame(s32);


#include "eff_emitter_type.h"

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

struct sdfMemHandle_t;




extern u32 effModelUpdateControlFlags;

extern u32 D_003BC888;


extern void *fileSaveReadBuffer;

extern struct _FRQ *fileMenuConvertedTextGlyph;
extern FileMenuContinuationCallback fileWaitContinuation;

extern s32 fileWaitTicksRemaining;

extern s32 slExitWaitProc(void);

extern s32 fileSlotStatusPoll(void);


extern void fileResetSlotStates(FileSlotTable *record);

extern char D_003BC940[];

extern char D_003B2688[]; /* "base.ico"; retail record includes padding */

extern u32 fileSaveIconData;

extern u32 fileSaveIconDataSize;

extern const u32 *fileWriteDataAddressSlot;

extern const u32 *fileWriteDataSizeSlot;

extern FileMenuContinuationCallback fileWriteContinuation;

extern struct FileRequest * const *fileWritePendingRequestSlot;

extern u32 D_003BC824;

extern s32 slSaveWriteFile(const char *fileName, const u32 *dataAddressSlot, const u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest * const *pendingRequestStatus);

extern s32 slSaveProc_openFile(void);

extern s32 slSaveProc_writeFile(void);

extern s32 fileBeginLabeledSlotWrite(void);

extern s32 fileBuildMainBlobAfterDelete(void);

extern McFileDescriptor fileSaveFileDescriptor;

extern struct sdfMemHandle_t *fileSaveReadBufferResource;


extern void sdfMcFuncClose(McFileDescriptor fileDescriptor);

extern s32 fileStoreSlotHeader(void);


extern s32 slSaveData_chdir(void);


extern s32 slEndingExit(void);

extern s32 slSaveData(void);

extern s32 fileSlotSelectPollClear(void);

extern s32 slEndingUpd2(void);

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

extern u32 fileMainSaveBlobAddress;
extern u32 fileMainSaveBlobSize;

extern LoadMirror fileSlotFlagMirror;

extern u32 fileSavedSlotFlags;

extern char fileConfigInputTaskName[];

extern char fileConfigLoadTaskName[]; /* "config_draw" */

extern char fileConfigOwnerTaskName[]; /* "config_update" */


#include "sdf_debug_api.h"

extern char D_003BC900[];

extern char D_003BC908[];

extern char D_003BC910[];

extern char D_003BC918[];

extern char D_003BC920[];

extern u32 D_003BC81C;

extern FileSelectionFlowState fileSelectionPending;

extern u32 D_003BC854;

extern u32 D_003BC80C;

extern u32 D_003BC810;

extern u32 fileMemoryCardRequestIndex;

extern s32 D_003BC850;

extern s32 D_003BC860;

extern u32 D_003BC858;

extern s32 D_003BC864;

extern s8 fileSavePreviewVersionByteView[];

extern FileMenuContinuationCallback fileMenuContinuation;

extern struct sdfMemHandle_t *fileSaveIconResourceHandle;

extern struct FileRequest *fileSaveIconRequest;

extern struct _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct _FRQ *);

extern struct _FRQ *fileMenuStringGlyph;

extern s32 D_003BC7FC;

extern s8 slLoadKillFlag;

extern dds3ProcessID_t *D_003BC7F0;

/* Loader context at fileLoadMenuState. */
extern FileLoadMenuState fileLoadMenuState;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern s32 D_0037D4D0[];

extern SdfPoolNode *effPacketSubmissionSurfaces[6];

extern char D_003B2668[];






extern void fileDrawLoadObjectSlots(s32 object);

extern s32 fileResetSelection(void);
extern s32 D_003BC818;
extern s32 D_003BC820;
extern void fileSetMenuValueAndInitializeFlags(u32 value);
extern void fileLoadSetMode(FileLoadMenuSlideMode mode);

extern s32 slEndingTop(void);

extern s32 fileSlotSelectPoll(void);

extern s32 fileOnAllWritten(void);

extern s32 mcdFinishFileDetection(void);

extern s32 fileSlotSelectionPollCount;


extern s32 fileRequestDefaultMode(void);

extern s32 slBootRetry(void);

extern s32 slBootError(void);

extern s32 slSizeChk(void);

extern s32 fileSelectedSlotIndex;


extern s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback);

extern s32 fileScanSlotStates(void);

extern s32 filePollSlotRequestAndResumeFlow(void);


extern u32 D_003BC804;

extern void dds3SoftResetOff(void);

extern s32 mcHandleDetectionResult(void);

extern void fileQueueInitTransform(void *queue);

extern void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);






extern void mnuRecordSetVector(FileSlotTable *record, const void *src);

extern void fileSetRecordSecondVector(FileSlotTable *record, const void *src);

extern void dds3AdminiCallSequence(s32 kind, const void *data, u32 size, s32 flags);

extern FileMenuStartMode D_003BC848;

extern s32 fileSlotScanIndex;

extern char D_003B2658[];

extern s32 slExitWait(FileMenuContinuationCallback callback);

extern void mcFormatSaveFilename(char *dst, s32 number);

extern s32 slChooseData_getCardInfo2(void);


extern char D_003B2678[];

extern sceMcTblGetDir fileDirectoryEntryScratch[];

extern s32 slChooseData_getCardInfo3(void);

extern s32 fileBeginSlotMetadataRefresh(void);

extern s32 slHeadLoad(void);

extern s32 fileScanSlotStatesAdvance(void);

extern s32 slChooseData_getCardInfo3_B(void);

extern s32 slChooseData_getCardInfo2_B(void);

extern s32 slBootError2(void);

extern u32 D_003BC834;

extern s32 filePollSlotDetectionAndBranch(void);
extern s32 slLoadProc_openFile(void);
extern s32 slLoadProc_readFile(void);

extern s32 filePrepareMainBlobWrite(void);

extern s32 slSaveData_deleteIconSys(void);

extern s32 slErrorWait(void);

extern s32 slSaveProc_closeFile(void);

extern s32 slSaveProc_Write_mkdir(void);


extern s32 fileBuildMainBlobAndWrite(void);







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

extern u32 D_0037D4AC[];


/* Object with loader sub-objects (+0x40...). */
typedef struct LoadObj {
    void *owner;        /* 0x00 */
    u32 color;          /* 0x04 */
    f32 scale;          /* 0x08 */
    u32 selector;       /* 0x0C: secondary job buffer operation */
    u8 pad10[0x24];
    void *deviceHandle; /* 0x34 */
    FileJobPayload **jobs; /* 0x38: child file-job payloads */
    struct sdfMemHandle_t *jobAllocation;          /* 0x3C */
    struct EffExpandedList *referenceHolder; /* 0x40 */
    FileSlotTable *recordWork; /* 0x44: created by fileAllocateGridRecordSlots */
    s16 unk48;          /* 0x48 */
    u16 unk4A;
} LoadObj;

extern s32 mcdHandleSaveSetupDone(void);


extern void fileDrawConfigurationMenuBackground(void *);

extern void mnuCallInitWide(s32, s32, s32, u32, s32);

extern LoadObj *fileLoadObjectCreate(void *owner);

extern void fileCloneEffectSurfaceResources(LoadObj *result, LoadObj *owner);


extern void fileJobFreePrimaryBuffer(FileJobPayload *job);

extern void fileJobFreeSecondaryBuffer(FileJobPayload *job);

extern FileJob *fileJobCreate(void);



extern FileJob *fileQueueFindById(FileQueue *, u32);
extern s32 fileFindQueuedJobIndex(FileQueue *, FileJob *);


extern void fileQueueAppend(FileQueue *queue, FileJob *job);

extern s8 dds3ConfigState;

void slKillLoadProcess(void) {
    if (D_003BC7F0 != NULL) {
        dds3KillProcess(D_003BC7F0, 1);
        D_003BC7F0 = NULL;
        slLoadKillFlag = 0;
    }
}

s8 fileMenuTaskIsAlive(void) {
    return slLoadKillFlag;
}

void mnuFormatSaveSlotHeaderName(void) {
}

void mcFormatSaveFilename(char *dst, s32 number) {
    sprintf(dst, "BASLUS-%05d-new-%d", D_003BC888, number);
}

void fileReqGetSlotCode(void) {
    s32 slot = fileReqGetSelectedSlot(fileMemoryCardRequestIndex);
    D_003BC864 = fileSavePreviewVersionByteView[slot * 0x30];
}

/* Size of the persistent main save block copied during a reload. */
#define FILE_MAIN_BLOB_SIZE 0x33600

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
    return condition != 0 && D_003BC7FC == 1;
}

u8 fileIsLoadedWithActiveFlow(s32 loaded) {
    return loaded != 0 && D_003BC7FC == 1;
}

void mnuDrawAndStoreTextGlyphHandle(s32 x, s32 y, u32 colors, const u8 *text) {
    fileMenuConvertedTextGlyph = itfCreateConvertedTextGlyph(x << 4, y << 3, 0, colors, text, 0);
    frFontDrawGlyphWithSharedFlags(fileMenuConvertedTextGlyph, 1);
    frDestroyFontRequestAfter(fileMenuConvertedTextGlyph);
}

extern _FRQ * frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *existingGlyph);

void mcdCreateFontDrawHandle(s32 x, s32 y, u32 color, const char *text) {
    frEnableMode(1);
    fileMenuStringGlyph = frFontBuildGlyphChain(text, 0, 0, 0, 0);
    frDisableMode(1);
    frSetFontSpace(fileMenuStringGlyph, 1);
    frSetDrawPosition(fileMenuStringGlyph, x << 4, y << 3);
    frSetFontColor(fileMenuStringGlyph, color);
    frFontDrawGlyphChain(fileMenuStringGlyph, 0, 0x56);
    frDestroyFontRequestAfter(fileMenuStringGlyph);
    frSetFontDrawOT(0x54);
}

void fileDrawMenuImageAtPoint(s32 x, s32 y, u32 colors, char *text) {
    struct _FRQ *imageHandle;

    imageHandle = itfCreateFont2TextGlyph(x << 4, y << 3, 0, colors, text, 0);
    frFontDrawGlyphWithSharedFlags(imageHandle, 1);
    frDestroyFontRequestAfter(imageHandle);
}

extern f32 fileSaveHighlightPhase;

extern void eglSetOt(u32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);

/* Animate the save-window highlight's alpha with a sinusoidal phase. */
void fileDrawPulsingSaveHighlight(void) {
    s32 angle;
    f32 wave;

    eglSetOt(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    fileSaveHighlightPhase = fileSaveHighlightPhase + 0.39999998f;
    sdfSin(fileSaveHighlightPhase);
    angle = ((s32)D_0037D4D0[2] + 8) % 360;
    D_0037D4D0[2] = angle;
    wave = sdfSin((f32)((angle + 0x5A) % 360) / 180.0f * 3.1415899f);
    D_0037D4D0[3] = (s32)((wave + 1.0f) * 0.5f * 191.0f + 64.0f);
    evtSubmitTexturedGradientQuad(0x1BE, 0x12C, 0x13, 0x1F, 1, 1, 0x13, 0x1F, (D_0037D4D0[3] << 24) | 0xAEC014,
                  (D_0037D4D0[3] << 24) | 0xAEC014, (D_0037D4D0[3] << 24) | 0xAEC014,
                  (D_0037D4D0[3] << 24) | 0xAEC014, itfMesGetWindowTexture());
}

extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);
extern void fileCursorPulseUpdate(void);
extern void fileDrawCursorPulse(s32, s32, s32);
extern s32 D_003BC85C;
void fileDrawSaveWindow(void) {
    eglSetOt(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitTexturedGradientQuad(0x112, 0x113, 0x98, 0x34, 0x14A, 0x1C5, 0x98, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC884);
    evtSubmitTexturedGradientQuad(0x56, 0x113, 0xBC, 0x34, 0x14A, 0x1C5, 1, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC884);
    fileCursorPulseUpdate();
    fileDrawCursorPulse(0x17E, 0x118, 0x56);
    D_003BC85C++;
}

s32 slChkWindowOpenDone(void) {
    if (D_003BC860 < 7) {
        return 0;
    }
    return 1;
}

s32 fileDrawStatusDialogFrame(void) {
    s32 frame;
    s32 y;
    s32 height;
    s32 value;

    eglSetOt(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 128, 3, 0, 0, 1, 1);
    frame = ++D_003BC860;
    if (frame < 4) {
        height = frame * 72;
        y = 224 - frame * 36;
    } else {
        y = 80;
        height = 288;
    }
    if (frame < 4) {
        value = frame * 20;
    } else {
        value = 80;
    }
    evtSubmitTexturedGradientQuad(0, 0, 512, 448, 0, 0, 512, 448,
                 (((value << 7) / 100) << 24) | 0x808080,
                 (((value << 7) / 100) << 24) | 0x808080,
                 (((value << 7) / 100) << 24) | 0x808080,
                 (((value << 7) / 100) << 24) | 0x808080, D_003BC880);
    if (D_003BC860 < 4) {
        value = D_003BC860 * 70 / 4;
    } else {
        value = 70;
    }
    evtSubmitDefaultDepthGradientRect(0, y, 512, height,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000);
    eglSetOt(0x53);
    if (D_003BC860 < 7) {
        return 0;
    }
    return 1;
}

extern char fileCheckingCardMessageLine1[];
extern char fileCheckingCardMessageLine2[];
extern char fileCheckingCardMessageLine3[];
extern char fileSavingText[];
extern char fileCardBusyCardWarningText[];
extern char fileCardBusyControllerWarningText[];
extern char fileCardBusyConsoleWarningText[];
extern char fileLoadingText[];
extern char fileSaveSuccessfulText[];
extern char fileFormattingText[];
extern char fileFormatSuccessfulText[];
extern char fileLoadSuccessfulText[];
extern s32 D_003BC840;
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

static inline void fileDrawDialogLine(s32 line, const char *glyphSource) {
    mcdCreateFontDrawHandle(86, 114 + line * 24, 0x89FEFF80U, glyphSource);
}

void fileShowStatusDialog(void) {
    if (D_003BC854 != 0) {
        if (D_003BC854 == 1) {
            fileSelectedSlotIndex = 0;
            D_003BC840 = 0;
            fileResetLoadContextSlide();
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileNoCardStatus1MessageLine1);
                fileDrawDialogLine(1, fileNoCardStatus1MessageLine2);
                fileDrawDialogLine(2, fileNoCardStatus1MessageLine3);
                fileDrawDialogLine(3, fileNoCardStatus1MessageLine4);
                fileDrawDialogLine(4, fileNoCardStatus1MessageLine5);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 2) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileNoCardStatus2MessageLine1);
                fileDrawDialogLine(1, fileNoCardStatus2MessageLine2);
                fileDrawDialogLine(2, fileNoCardStatus2MessageLine3);
                fileDrawDialogLine(3, fileNoCardStatus2MessageLine4);
                fileDrawDialogLine(4, fileNoCardStatus2MessageLine5);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 3) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileLoadFailedText);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 4) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileSaveFailedText);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 5) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileInsufficientSpaceMessageLine1);
                fileDrawDialogLine(1, fileInsufficientSpaceMessageLine2);
                fileDrawDialogLine(2, fileInsufficientSpaceMessageLine3);
                fileDrawDialogLine(3, fileInsufficientSpaceMessageLine4);
                fileDrawDialogLine(4, fileInsufficientSpaceMessageLine5);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 6) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileFormatFailedText);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 7) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileNoSaveDataMessageLine1);
                fileDrawDialogLine(1, fileNoSaveDataMessageLine2);
                fileDrawDialogLine(2, fileNoSaveDataMessageLine3);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
    }
}

void slSetInfo(u32 state) {
    s32 previous;

    previous = D_003BC850;
    D_003BC850 = state;
    if (previous == 0) {
        D_003BC860 = 0;
    }
}

void fileShowFlowDialog(void) {
    if (D_003BC850 != 0) {
        if (D_003BC850 == 1) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileCheckingCardMessageLine1);
                fileDrawDialogLine(1, fileCheckingCardMessageLine2);
                fileDrawDialogLine(2, fileCheckingCardMessageLine3);
                fileDrawSaveWindow();
            }
        }
        if (D_003BC850 == 2) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileSavingText);
                fileDrawDialogLine(1, fileCardBusyCardWarningText);
                fileDrawDialogLine(2, fileCardBusyControllerWarningText);
                fileDrawDialogLine(3, fileCardBusyConsoleWarningText);
                fileDrawSaveWindow();
            }
        }
        if (D_003BC850 == 3) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileLoadingText);
                fileDrawDialogLine(1, fileCardBusyCardWarningText);
                fileDrawDialogLine(2, fileCardBusyControllerWarningText);
                fileDrawDialogLine(3, fileCardBusyConsoleWarningText);
                fileDrawSaveWindow();
            }
        }
        if (D_003BC850 == 4) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileSaveSuccessfulText);
                fileDrawPulsingSaveHighlight();
            }
            D_003BC81C = FILE_MENU_OPERATION_SAVE_SUCCEEDED;
        }
        if (D_003BC850 == 5) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileFormattingText);
                fileDrawDialogLine(1, fileCardBusyCardWarningText);
                fileDrawDialogLine(2, fileCardBusyControllerWarningText);
                fileDrawDialogLine(3, fileCardBusyConsoleWarningText);
                fileDrawSaveWindow();
            }
        }
        if (D_003BC850 == 6) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileFormatSuccessfulText);
                fileDrawPulsingSaveHighlight();
            }
        }
        if (D_003BC850 == 13) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, fileLoadSuccessfulText);
                fileDrawPulsingSaveHighlight();
            }
            D_003BC81C = FILE_MENU_OPERATION_LOAD_SUCCEEDED;
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
        return fileAdvanceSlotScan();
    }
    path[0] = 0x2F;
    mcFormatSaveFilename(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, MC_OPEN_MODE_READ_ONLY);
    return (s32)fileReadSlotPreviewBegin;
}

extern s32 slHeadLoadProc_readFile(void);
s32 fileReadSlotPreviewBegin(void) {
    McPollResult status = sdfMcPollOpen(&fileSaveFileDescriptor);

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

s32 slHeadLoadProc_readFile(void) {
    McPollResult status = sdfMcPollRead();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncClose(fileSaveFileDescriptor);
        return (s32)fileStoreSlotHeader;
    }
    sdfMemFreeDirect(fileSaveReadBufferResource);
    return fileBeginSlotMetadataRefresh();
}

s32 fileStoreSlotHeader(void) {
    McPollResult status = sdfMcPollClose();

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        memcpy(&fileSavePreviewRecords[fileSlotScanIndex], fileSaveReadBuffer, 0x30);
        sdfMemFreeDirect(fileSaveReadBufferResource);
        return fileAdvanceSlotScan();
    }
    sdfMemFreeDirect(fileSaveReadBufferResource);
    return fileBeginSlotMetadataRefresh();
}

s32 slExitWait(FileMenuContinuationCallback callback) {
    dds3FadeOut(0, 0, 0, 15);
    fileWaitContinuation = callback;
    fileWaitTicksRemaining = 20;
    return (s32)slExitWaitProc;
}

s32 slEndingTop(void) {
    D_003BC848 = FILE_MENU_START_MODE_LOAD;
    slSetInfo(0);
    D_003BC854 = 0;
    D_003BC834 = 4;
    D_003BC810 = 0;
    return slQueryYesNo(&fileResetSelection, &slEndingExit, 0);
}

s32 slEndingUpd2(void) {
    slSetInfo(0);
    D_003BC854 = 0;
    D_003BC834 = 9;
    return slQueryYesNo(&slSaveData, &fileScanSlotStates, 1);
}

s32 slEndingExit(void) {
    slSetInfo(0);
    D_003BC854 = 0;
    D_003BC834 = 8;
    D_003BC810 = 0;
    return slQueryYesNo(&fileRequestDefaultMode, &slEndingTop, 1);
}

s32 slBootTop(void) {
    slSetInfo(0);
    D_003BC810 = 0;
    return (s32)fileSlotSelectPoll;
}

s32 slBootRetry(void) {
    fileReqBegin(0);
    fileSlotSelectionPollCount = 30;
    D_003BC804 = 1;
    slSetInfo(1);
    D_003BC810 = 1;
    return (s32)fileSlotSelectPollClear;
}

s32 slBootError(void) {
    D_003BC80C = 1;
    slSetInfo(0);
    D_003BC834 = 5;
    return slQueryYesNo(&fileRequestDefaultMode, &slBootRetry, 1);
}

s32 slBootError2(void) {
    D_003BC80C = 1;
    slSetInfo(0);
    D_003BC834 = 6;
    return slQueryYesNo(&fileRequestDefaultMode, &slBootRetry, 1);
}

s32 fileBeginSlotMetadataRefresh(void) {
    fileClearAllSlotFlags();
    slSetInfo(0);
    D_003BC854 = 1;
    D_003BC858 = 0;
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
    D_003BC804 = 1;
    slSetInfo(1);
    D_003BC810 = 1;
    return (s32)fileSlotStatusPoll;
}

s32 slExit(void) {
    slSetInfo(0);
    D_003BC810 = 0;
    D_003BC80C = 0;
    return 0xffffffff;
}

s32 mcdEnterDefaultFileFlow(void) {
    u32 mode[4];

    mode[0] = 0;
    dds3AdminiCallSequence(2, mode, 4, 0);
    return 0;
}

s32 fileRequestDefaultMode(void) {
    return mcdEnterDefaultFileFlow();
}

s32 mcdEnterSelectedFileFlow(void) {
    u32 v = 2;

    fileSelectionPending = FILE_SELECTION_FLOW_PENDING;
    dds3AdminiCallSequence(2, &v, 4, 0);
    return 0;
}

s32 fileRestartSlotScan(void) {
    fileClearAllSlotFlags();
    slSetInfo(1);
    D_003BC854 = 0;
    D_003BC810 = 0;
    return fileResetSelection();
}

extern s32 D_003BC82C;
extern s32 fileSlotBrowserUpdate(void);

s32 fileScanSlotStates(void) {
    s32 i;

    D_003BC82C = 0;
    D_003BC810 = 1;
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
    if (D_003BC848 == FILE_MENU_START_MODE_SLOT_SELECTION &&
        dds3SearchProcessName(D_003B2658) == NULL) {
        return mcdEnterSelectedFileFlow();
    }
    slSetInfo(0);
    return slExitWait(&slExit);
}

s32 fileLoadIconFileAndResetSelection(void) {
    fileSaveIconResourceHandle = 0;
    fileSaveIconRequest = fileLoad(D_003B2668);
    return fileResetSelection();
}

s32 fileResetSlotSelection(void) {
    return fileResetSelection();
}

s32 fileLoadIconFileAndBeginSlotReset(void) {
    fileSaveIconResourceHandle = 0;
    fileSaveIconRequest = fileLoad(D_003B2668);
    return slEndingTop();
}

s32 slBootProc1st(void) {
    fileReqBegin(0);
    D_003BC804 = 1;
    fileSlotSelectionPollCount = 0;
    slSetInfo(0);
    D_003BC810 = 0;
    return slBootTop();
}

s32 fileSlotStatusPoll(void) {
    FileReqStatus status;

    if (fileReqPoll() == FILE_REQ_POLL_PENDING) {
        return 0;
    }
    if (fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestIndex);
        return 0;
    }
    status = fileReqGetStatus(fileMemoryCardRequestIndex);
    switch (status) {
    case FILE_REQ_STATUS_NO_CARD:
        return fileBeginSlotMetadataRefresh();
    case FILE_REQ_STATUS_UNFORMATTED:
        if (D_003BC848 == FILE_MENU_START_MODE_LOAD) {
            fileClearAllSlotFlags();
            slSetInfo(0);
            D_003BC854 = 0;
            D_003BC834 = 2;
            return slQueryYesNo(slFormatStart, slFormatCancel, 1);
        }
        slSetInfo(0);
        D_003BC854 = 7;
        D_003BC858 = 0;
        fileReqBegin(fileMemoryCardRequestIndex);
        return (s32)filePollSlotDetectionAndBranch;
    case FILE_REQ_STATUS_UNSUPPORTED_CARD:
        fileClearAllSlotFlags();
        slSetInfo(0);
        D_003BC854 = 2;
        D_003BC858 = 0;
        fileReqBegin(fileMemoryCardRequestIndex);
        return (s32)filePollSlotDetectionAndBranch;
    default:
        return 0;
    case FILE_REQ_STATUS_READY:
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
    return fileReqGetSize(fileMemoryCardRequestIndex) > 0x4C3FF;
}

s32 fileSlotSelectPoll(void) {
    FileReqPollResult result = fileReqPoll();
    if (result == FILE_REQ_POLL_PENDING) {
        return result;
    }
    if (fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestIndex);
        return 0;
    }
    switch (fileReqGetStatus(fileMemoryCardRequestIndex)) {
    case FILE_REQ_STATUS_NO_CARD:
    case FILE_REQ_STATUS_UNSUPPORTED_CARD:
        return slBootError();
    case FILE_REQ_STATUS_UNFORMATTED:
        return fileRequestDefaultMode();
    case FILE_REQ_STATUS_READY:
        if (slSizeChk() != 0) {
            return fileRequestDefaultMode();
        }
        return (s32)slBootRetry();
    default:
        return 0;
    }
}

extern s32 mcClearSlotMetadata(void);

s32 fileSlotSelectPollClear(void) {
    if (fileReqPoll() == FILE_REQ_POLL_PENDING) {
        return 0;
    }
    if (fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestIndex);
        return 0;
    }
    switch (fileReqGetStatus(fileMemoryCardRequestIndex)) {
    case FILE_REQ_STATUS_NO_CARD:
    case FILE_REQ_STATUS_UNSUPPORTED_CARD:
        return slBootError();
    case FILE_REQ_STATUS_UNFORMATTED:
        return fileRequestDefaultMode();
    case FILE_REQ_STATUS_READY:
        if (slSizeChk() != 0) {
            return fileRequestDefaultMode();
        }
        return mcClearSlotMetadata();
    default:
        return 0;
    }
}

extern s8 dds3PadButtonState[];

s32 filePollSlotDetectionAndBranch(void) {
    FileReqStatus reqStatus;

    D_003BC810 = 0;
    if (D_003BC858 == 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_003BC858 = 1;
        }
    }
    if (fileReqPoll() != FILE_REQ_POLL_PENDING) {
        if (D_003BC858 == 1) {
            D_003BC854 = 0;
            D_003BC810 = 0;
            if (D_003BC824 != 0) {
                return slEndingExit();
            }
            if (D_003BC848 == FILE_MENU_START_MODE_LOAD) {
                return slExitWait(&slExit);
            }
            return mcdEnterSelectedFileFlow();
        }
        reqStatus = fileReqGetStatus(fileMemoryCardRequestIndex);
        switch (reqStatus) {
        case FILE_REQ_STATUS_READY:
            if (D_003BC854 == 1) {
                fileClearAllSlotFlags();
                slSetInfo(1);
                D_003BC854 = 0;
                D_003BC810 = 0;
                return fileResetSelection();
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case FILE_REQ_STATUS_NO_CARD:
            fileClearAllSlotFlags();
            if (D_003BC854 != 1) {
                D_003BC854 = 1;
                D_003BC858 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case FILE_REQ_STATUS_UNFORMATTED:
            if (D_003BC848 == FILE_MENU_START_MODE_LOAD) {
                fileClearAllSlotFlags();
                D_003BC854 = 0;
                D_003BC834 = 2;
                return slQueryYesNo(&slFormatStart, &slFormatCancel, 1);
            }
            if (D_003BC854 != 7) {
                fileClearAllSlotFlags();
                D_003BC854 = 7;
                D_003BC858 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case FILE_REQ_STATUS_UNSUPPORTED_CARD:
            if (D_003BC854 != 2) {
                fileClearAllSlotFlags();
                D_003BC854 = 2;
                D_003BC858 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        }
    }
    return 0;
}

s32 slErrorWaitToFileMc(void) {
    if (D_003BC858 == 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            D_003BC858 = 1;
        }
    }
    if (fileReqPoll() != FILE_REQ_POLL_PENDING) {
        if (D_003BC858 == 1) {
            D_003BC854 = 0;
            return fileScanSlotStates();
        }
        if (fileReqGetStatus(fileMemoryCardRequestIndex) != FILE_REQ_STATUS_READY) {
            D_003BC854 = 0;
            D_003BC810 = 0;
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
        D_003BC854 = 0;
        D_003BC810 = 0;
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

s32 fileSetMenuCallbackAndClearResult(FileMenuContinuationCallback callback) {
    fileMenuContinuation = callback;
    D_003BC858 = 0;
    return (s32)mcdFinishFileDetection;
}

s32 mcdFinishFileDetection(void) {
    if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) ||
        fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0)) {
        slSetInfo(0);
        D_003BC854 = 0;
        D_003BC810 = 0;
        p2sdStopBGM(0x310000);
        if (fileMenuContinuation == (FileMenuContinuationCallback)-1) {
            return slExitWait((FileMenuContinuationCallback)-1);
        }
        return fileMenuContinuation();
    }
    return 0;
}

s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback) {
    fileMenuContinuation = callback;
    D_003BC858 = 0;
    fileReqBegin(fileMemoryCardRequestIndex);
    return (s32)filePollSlotRequestAndResumeFlow;
}

s32 filePollSlotRequestAndResumeFlow(void) {
    FileReqStatus fileStatus;

    if (D_003BC858 == 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            D_003BC858 = 1;
        }
    }
    if (fileReqPoll() != FILE_REQ_POLL_PENDING) {
        if (D_003BC858 == 1) {
            slSetInfo(0);
            D_003BC854 = 0;
            D_003BC810 = 0;
            if (fileMenuContinuation == (FileMenuContinuationCallback)-1) {
                return slExitWait((FileMenuContinuationCallback)-1);
            }
            return fileMenuContinuation();
        }
        fileStatus = fileReqGetStatus(fileMemoryCardRequestIndex);
        switch (fileStatus) {
        case FILE_REQ_STATUS_READY:
            if (D_003BC854 == 1) {
                fileClearAllSlotFlags();
                slSetInfo(1);
                D_003BC854 = 0;
                D_003BC810 = 0;
                return fileResetSelection();
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case FILE_REQ_STATUS_NO_CARD:
            return fileBeginSlotMetadataRefresh();
        case FILE_REQ_STATUS_UNFORMATTED:
            if (D_003BC848 == FILE_MENU_START_MODE_LOAD) {
                fileClearAllSlotFlags();
                slSetInfo(0);
                D_003BC854 = 0;
                D_003BC834 = 2;
                return slQueryYesNo(&slFormatStart, &slFormatCancel, 1);
            }
            if (D_003BC854 != 7) {
                fileClearAllSlotFlags();
                slSetInfo(0);
                D_003BC854 = 7;
                D_003BC858 = 0;
            }
            fileReqBegin(fileMemoryCardRequestIndex);
            break;
        case FILE_REQ_STATUS_UNSUPPORTED_CARD:
            if (D_003BC854 != 2) {
                fileClearAllSlotFlags();
                slSetInfo(0);
                D_003BC854 = 2;
                D_003BC858 = 0;
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
    mcFormatSaveFilename(&buf[1], fileSlotScanIndex);
    sdfMcFuncChdir(fileMemoryCardRequestIndex, buf);
    return (s32)slChooseData_getCardInfo2;
}

s32 slChooseData_getCardInfo2(void) {
    McPollResult t = sdfMcPollChdir();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSlotScanIndex, FILE_REQ_SLOT_DIRECTORY_READY);
        sdfMcFuncGetDir(fileMemoryCardRequestIndex, D_003B2678, fileDirectoryEntryScratch, 1);
        return (s32)slChooseData_getCardInfo3;
    }
    if (t == MC_POLL_ERROR) {
        return fileBeginSlotMetadataRefresh();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfo3(void) {
    McDirectoryEntryCount entryCount;
    McPollResult status = sdfMcPollGetDir(&entryCount);
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (entryCount == 1) {
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

s32 fileAdvanceSlotScan(void) {
    fileSlotScanIndex++;
    if (fileSlotScanIndex == 10) {
        fileReqClearSlotMetadataDirty(fileMemoryCardRequestIndex);
        fileSlotScanIndex = 0;
        D_003BC804 = 0;
        if (fileCountSelectableFiles() == 0) {
            if (D_003BC848 != FILE_MENU_START_MODE_LOAD) {
                D_003BC810 = 0;
                slSetInfo(0);
                D_003BC854 = 7;
                D_003BC858 = 0;
                fileReqBegin(fileMemoryCardRequestIndex);
                return (s32)filePollSlotDetectionAndBranch;
            }
            if (slSizeChk() == 0) {
                slSetInfo(0);
                D_003BC810 = 0;
                D_003BC854 = 5;
                D_003BC858 = 0;
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
        D_003BC804 = 0;
        slSetInfo(0);
        return fileScanSlotStates();
    }
}

s32 fileBeginSaveSlotIconScan(void) {
    char buf[0x50];

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], fileSlotScanIndex);
    sdfMcFuncChdir(fileMemoryCardRequestIndex, buf);
    return (s32)slChooseData_getCardInfo2_B;
}

s32 slChooseData_getCardInfo2_B(void) {
    McPollResult t = sdfMcPollChdir();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSlotScanIndex, FILE_REQ_SLOT_DIRECTORY_READY);
        sdfMcFuncGetDir(fileMemoryCardRequestIndex, D_003B2678, fileDirectoryEntryScratch, 1);
        return (s32)slChooseData_getCardInfo3_B;
    }
    if (t == MC_POLL_ERROR) {
        slSetInfo(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return slBootError();
    }
    return fileScanSlotStatesAdvance();
}

s32 slChooseData_getCardInfo3_B(void) {
    McDirectoryEntryCount entryCount;
    McPollResult status = sdfMcPollGetDir(&entryCount);
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (entryCount == 1) {
            fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSlotScanIndex,
                                FILE_REQ_SLOT_CAN_READ_SAVE | FILE_REQ_SLOT_DELETE_BEFORE_WRITE);
        }
        return fileScanSlotStatesAdvance();
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
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
                D_003BC810 = 0;
                D_003BC854 = 0;
                return fileRequestDefaultMode();
            }
        }
    }
    fileSlotScanIndex = slot;
    if (slot == 10) {
        slSetInfo(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return slBootError2();
    }
    return fileBeginSaveSlotIconScan();
}

s32 mcClearSlotMetadata(void) {
    s32 index = 0;
    FileSlotDisplayState *saved;
    slSetInfo(1);
    D_003BC810 = 0;
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
    mcFormatSaveFilename(name + 1, slot);
    sdfMcFuncMkdir(entry, name);
    return (s32)slSaveProc_Write_mkdir;
}

s32 slSaveData_chdir(void) {
    char buf[0x50];
    u32 entry = fileMemoryCardRequestIndex;
    s32 v = fileReqGetSelectedSlot(entry);

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], v);
    sdfMcFuncChdir(entry, buf);
    return (s32)filePrepareMainBlobWrite;
}

s32 slSaveProc_Write_mkdir(void) {
    McPollResult status = sdfMcPollMkdir();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        fileReqSetSlotFlags(fileMemoryCardRequestIndex, fileSelectedSlotIndex, FILE_REQ_SLOT_DIRECTORY_READY);
        return slSaveData_chdir();
    }
    slSetInfo(0);
    D_003BC854 = 4;
    return (s32)slErrorWait;
}

s32 filePrepareMainBlobWrite(void) {
    McPollResult t = sdfMcPollChdir();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        return slSaveData_deleteIconSys();
    }
    if (t == MC_POLL_ERROR) {
        slSetInfo(0);
        D_003BC854 = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 fileBuildMainBlobAfterDelete(void) {
    McPollResult t = sdfMcPollDelete();

    if (t == MC_POLL_PENDING) {
        return 0;
    }
    if (t == MC_POLL_SUCCESS) {
        return fileBuildMainBlobAndWrite();
    }
    if (t == MC_POLL_ERROR) {
        slSetInfo(0);
        D_003BC854 = 4;
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
extern u32 D_003BC890;
extern u32 D_003BC894;

s32 fileBeginLabeledSlotWrite(void) {
    s32 slot = fileSelectedSlotIndex + 1;
    s8 tens = 0x4F + slot / 10;
    s8 ones = 0x4F + slot % 10;

    fileSaveIconSysData[0xD0] = 0x82;
    fileSaveIconSysData[0xD1] = tens;
    fileSaveIconSysData[0xD2] = 0x82;
    fileSaveIconSysData[0xD3] = ones;
    return slSaveWriteFile(D_003B2678, &D_003BC890, &D_003BC894, fileWriteIconSysComplete, 0);
}

s32 slSaveWriteEnd_saveData(void) {
    return slSaveWriteFile(D_003B2688, &fileSaveIconData, &fileSaveIconDataSize,
                          fileBeginLabeledSlotWrite, &fileSaveIconRequest);
}

s32 fileBuildMainBlobAndWrite(void) {
    char filename[0x50];
    s32 slot = fileReqGetSelectedSlot(fileMemoryCardRequestIndex);

    mcFormatSaveFilename(filename, slot);
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
    DDS3_GBWK->header.savedMoney = DDS3_GBWK->header.currency;
    if (D_003BC824 != 0) {
        if (evtBitCheck(0x74) != 0) {
            DDS3_GBWK->header.saveFlags |= 1;
        } else {
            DDS3_GBWK->header.saveFlags &= ~1;
        }
        if (evtBitCheck(0x71) != 0) {
            DDS3_GBWK->header.saveFlags |= 2;
        } else {
            DDS3_GBWK->header.saveFlags &= ~2;
        }
        if (evtBitCheck(0x70) != 0) {
            DDS3_GBWK->header.saveFlags |= 4;
        } else {
            DDS3_GBWK->header.saveFlags &= ~4;
        }
        if (evtBitCheck(0x72) != 0) {
            DDS3_GBWK->header.saveFlags |= 8;
        } else {
            DDS3_GBWK->header.saveFlags &= ~8;
        }
        if (evtBitCheck(0x79) != 0) {
            DDS3_GBWK->header.saveFlags |= 0x10;
        } else {
            DDS3_GBWK->header.saveFlags &= ~0x10;
        }
        if (evtBitCheck(0x7B) != 0) {
            DDS3_GBWK->header.saveFlags |= 0x20;
        } else {
            DDS3_GBWK->header.saveFlags &= ~0x20;
        }
        if (evtBitCheck(0x7D) != 0) {
            DDS3_GBWK->header.saveFlags |= 0x40;
        } else {
            DDS3_GBWK->header.saveFlags &= ~0x40;
        }
        if (evtBitCheck(0x73) != 0) {
            DDS3_GBWK->header.saveFlags |= 0x80;
        } else {
            DDS3_GBWK->header.saveFlags &= ~0x80;
        }
        if (evtBitCheck(0x380) != 0) {
            DDS3_GBWK->header.saveFlags |= 0x100;
        } else {
            DDS3_GBWK->header.saveFlags &= ~0x100;
        }
        if (evtBitCheck(0x381) != 0) {
            DDS3_GBWK->header.saveFlags |= 0x200;
        } else {
            DDS3_GBWK->header.saveFlags &= ~0x200;
        }
        if (evtBitCheck(0x850) != 0) {
            DDS3_GBWK->header.saveFlags2 |= 0x2000;
        } else {
            DDS3_GBWK->header.saveFlags2 &= ~0x2000;
        }
        if (evtBitCheck(0x851) != 0) {
            DDS3_GBWK->header.saveFlags2 |= 0x4000;
        } else {
            DDS3_GBWK->header.saveFlags2 &= ~0x4000;
        }
    }
    if (evtBitCheck(0x90B) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x400;
    }
    if (evtBitCheck(0x90C) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x800;
    }
    if (evtBitCheck(0x90D) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x1000;
    }
    if (evtBitCheck(0x90E) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x2000;
    }
    if (evtBitCheck(0x90F) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x4000;
    }
    if (evtBitCheck(0x970) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x8000;
    }
    if (evtBitCheck(0x971) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x10000;
    }
    if (evtBitCheck(0x972) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x20000;
    }
    if (evtBitCheck(0x973) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x40000;
    }
    if (evtBitCheck(0x974) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x80000;
    }
    if (evtBitCheck(0x97F) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x100000;
    }
    if (evtBitCheck(0xA00) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x200000;
    }
    if (evtBitCheck(0xA01) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x400000;
    }
    if (evtBitCheck(0xA02) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x800000;
    }
    if (evtBitCheck(0x6C4) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x01000000;
    }
    if (evtBitCheck(0x6C6) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x02000000;
    }
    if (evtBitCheck(0x6C8) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x04000000;
    }
    if (evtBitCheck(0x6C9) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x08000000;
    }
    if (evtBitCheck(0x6CA) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x10000000;
    }
    if (evtBitCheck(0x820) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x20000000;
    }
    if (evtBitCheck(0x821) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x40000000;
    }
    if (evtBitCheck(0x822) != 0) {
        DDS3_GBWK->header.saveFlags |= 0x80000000;
    }
    if (evtBitCheck(0x823) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 1;
    }
    if (evtBitCheck(0x824) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 2;
    }
    if (evtBitCheck(0x825) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 4;
    }
    if (evtBitCheck(0x826) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 8;
    }
    if (evtBitCheck(0x827) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x10;
    }
    if (evtBitCheck(0x828) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x20;
    }
    if (evtBitCheck(0x829) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x40;
    }
    if (evtBitCheck(0x82A) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x80;
    }
    if (evtBitCheck(0x82B) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x100;
    }
    if (evtBitCheck(0x82C) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x200;
    }
    if (evtBitCheck(0x82D) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x400;
    }
    if (evtBitCheck(0x82E) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x800;
    }
    if (evtBitCheck(0x82F) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x1000;
    }
    if (evtBitCheck(0x830) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x2000;
    }
    if (evtBitCheck(0x831) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x4000;
    }
    if (evtBitCheck(0x832) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x8000;
    }
    if (evtBitCheck(0x833) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x10000;
    }
    if (evtBitCheck(0x834) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x20000;
    }
    if (evtBitCheck(0x835) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x40000;
    }
    if (evtBitCheck(0x836) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x80000;
    }
    if (evtBitCheck(0x837) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x100000;
    }
    if (evtBitCheck(0x838) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x200000;
    }
    if (evtBitCheck(0x839) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x400000;
    }
    if (evtBitCheck(0x83A) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x800000;
    }
    if (evtBitCheck(0x83B) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x01000000;
    }
    if (evtBitCheck(0x83C) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x02000000;
    }
    if (evtBitCheck(0x83D) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x04000000;
    }
    if (evtBitCheck(0x83E) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x08000000;
    }
    if (evtBitCheck(0x83F) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x10000000;
    }
    if (evtBitCheck(0x840) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x20000000;
    }
    if (evtBitCheck(0x841) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x40000000;
    }
    if (evtBitCheck(0x842) != 0) {
        DDS3_GBWK->header.saveFlags1 |= 0x80000000;
    }
    if (evtBitCheck(0x843) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 1;
    }
    if (evtBitCheck(0x844) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 2;
    }
    if (evtBitCheck(0x845) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 4;
    }
    if (evtBitCheck(0x846) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 8;
    }
    if (evtBitCheck(0x847) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x10;
    }
    if (evtBitCheck(0x848) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x20;
    }
    if (evtBitCheck(0x849) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x40;
    }
    if (evtBitCheck(0x84A) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x80;
    }
    if (evtBitCheck(0x84B) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x100;
    }
    if (evtBitCheck(0x84C) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x200;
    }
    if (evtBitCheck(0x84D) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x400;
    }
    if (evtBitCheck(0x84E) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x800;
    }
    if (evtBitCheck(0x84F) != 0) {
        DDS3_GBWK->header.saveFlags2 |= 0x1000;
    }
    fileSavePreviewRecords[slot].signature[0] = DDS3_GBWK->header.magic[0];
    fileSavePreviewRecords[slot].signature[1] = DDS3_GBWK->header.magic[1];
    fileSavePreviewRecords[slot].signature[2] = DDS3_GBWK->header.magic[2];
    fileSavePreviewRecords[slot].version = DDS3_GBWK->header.version;
    fileSavePreviewRecords[slot].mapGroup = DDS3_GBWK->header.mapGroup;
    fileSavePreviewRecords[slot].mapIndex = DDS3_GBWK->header.mapIndex;
    fileSavePreviewRecords[slot].playTicks = DDS3_GBWK->header.playTicks;
    fileSavePreviewRecords[slot].status = DDS3_GBWK->header.status;
    fileSavePreviewRecords[slot].newCycle = DDS3_GBWK->header.transition;
    memcpy(fileSavePreviewRecords[slot].party, DDS3_GBWK->header.partyIds, 8);
    memcpy(fileSavePreviewRecords[slot].levels, DDS3_GBWK->header.partyLevels, 8);
    fileSavePreviewRecords[slot].money = DDS3_GBWK->header.savedMoney;
    fileSavePreviewRecords[slot].saveWords.words[0] = DDS3_GBWK->header.saveFlags;
    fileSavePreviewRecords[slot].saveWords.words[1] = DDS3_GBWK->header.saveFlags1;
    fileSavePreviewRecords[slot].saveWords.words[2] = DDS3_GBWK->header.saveFlags2;
    fileMainSaveBlobAddress = (u32)DDS3_GBWK;
    fileMainSaveBlobSize = 0x33600;
    if (D_003BC820 == 0 || D_003BC820 == 2) {
        return slSaveWriteFile(filename, &fileMainSaveBlobAddress, &fileMainSaveBlobSize, slSaveWriteEnd_saveData, NULL);
    }
    return slSaveWriteFile(filename, &fileMainSaveBlobAddress, &fileMainSaveBlobSize, fileBeginLabeledSlotWrite, NULL);
}

s32 slSaveData_deleteIconSys(void) {
    u32 entry = fileMemoryCardRequestIndex;
    s32 slot = fileReqGetSelectedSlot(entry);
    u32 flags = fileReqGetSlotFlags(entry, slot);
    if (!(flags & FILE_REQ_SLOT_DELETE_BEFORE_WRITE)) {
        return fileBuildMainBlobAndWrite();
    }
    sdfMcFuncDelete(entry, D_003B2678);
    return (s32)fileBuildMainBlobAfterDelete;
}

/* Later save callbacks read these borrowed slots without changing them; callers
 * keep the slots alive until the asynchronous save completes. */
s32 slSaveWriteFile(const char *fileName, const u32 *dataAddressSlot, const u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest * const *pendingRequestStatus) {
    fileWriteDataAddressSlot = dataAddressSlot;
    fileWriteDataSizeSlot = dataSizeSlot;
    fileWriteContinuation = continuation;
    fileWritePendingRequestSlot = pendingRequestStatus;
    sdfMcFuncOpen(fileMemoryCardRequestIndex, fileName, MC_OPEN_MODE_READ_WRITE_CREATE);
    return (s32)slSaveProc_openFile;
}

extern s32 slSaveProc_waitDataReady(void);

s32 slSaveProc_openFile(void) {
    McPollResult status = sdfMcPollOpen(&fileSaveFileDescriptor);

    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        if (fileWritePendingRequestSlot != NULL && *fileWritePendingRequestSlot != 0) {
            return (s32)slSaveProc_waitDataReady;
        }
        sdfMcFuncWrite(fileSaveFileDescriptor, (const void *)*fileWriteDataAddressSlot, *fileWriteDataSizeSlot);
        return (s32)slSaveProc_writeFile;
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        D_003BC854 = 4;
        return (s32)slErrorWait;
    }
    return 0;
}


s32 slSaveProc_writeFile(void) {
    McPollResult status = sdfMcPollWrite();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncClose(fileSaveFileDescriptor);
        return (s32)slSaveProc_closeFile;
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        D_003BC854 = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_closeFile(void) {
    McPollResult status = sdfMcPollClose();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        return fileWriteContinuation();
    }
    if (status == MC_POLL_ERROR) {
        slSetInfo(0);
        D_003BC854 = 4;
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
    McPollResult status = sdfMcPollFormat();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    dds3SoftResetOn();
    if (status < 0) {
        slSetInfo(0);
        D_003BC854 = 6;
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
    if (D_003BC824 != 0) {
        D_003BC834 = 7;
        return slQueryYesNo(slEndingExit, fileResetSelection, 1);
    }
    D_003BC834 = 7;
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
    mcFormatSaveFilename(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = '/';
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, MC_OPEN_MODE_READ_ONLY);
    return (s32)slLoadProc_openFile;
}

s32 slLoadProc_openFile(void) {
    McPollResult status = sdfMcPollOpen(&fileSaveFileDescriptor);
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
    D_003BC854 = 3;
    return (s32)slErrorWait;
}

s32 slLoadProc_readFile(void) {
    McPollResult status = sdfMcPollRead();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        sdfMcFuncClose(fileSaveFileDescriptor);
        return (s32)mcdHandleSaveSetupDone;
    }
    sdfMemFreeDirect(fileSaveReadBufferResource);
    slSetInfo(0);
    D_003BC854 = 3;
    return (s32)slErrorWait;
}

extern s8 D_003BC7ED;
extern void fileCacheSlotFlagsFromState(void);
extern void fileRestoreSlotFlagsToState(void);

s32 mcdHandleSaveSetupDone(void) {
    McPollResult status = sdfMcPollClose();
    if (status == MC_POLL_PENDING) {
        return 0;
    }
    if (status == MC_POLL_SUCCESS) {
        fileReloadSaveBuffer();
        sdfMemFreeDirect(fileSaveReadBufferResource);
        D_003BC7ED = 1;
        slKillLoadProcess();
        if (fileLoadStateChanged() == FILE_SLOT_FLAGS_UNCHANGED) {
            fileCacheSlotFlagsFromState();
        } else {
            fileRestoreSlotFlagsToState();
        }
        slSetInfo(13);
        return fileSetMenuCallbackAndClearResult((FileMenuContinuationCallback)-1);
    }
    sdfMemFreeDirect(fileSaveReadBufferResource);
    slSetInfo(0);
    D_003BC854 = 3;
    return (s32)slErrorWait;
}

s32 fileSlotBrowserUpdate(void) {
    s32 oldSelection = fileSelectedSlotIndex;
    s32 offset;
    s32 selection;
    s32 action;
    FileReqStatus requestStatus;
    u32 flags;

    if (D_003BC82C == 0) {
        offset = oldSelection - D_003BC840;
        fileLoadMenuState.inputRepeatTimer++;
        fileLoadMenuState.inputRepeatTimer = fileLoadMenuState.inputRepeatTimer <= 0 ? 0 : fileLoadMenuState.inputRepeatTimer > 3 ? 3 : fileLoadMenuState.inputRepeatTimer;

        if (fileIsLoadedWithActiveFlow(((u8)dds3PadButtonState[0x26] >> 1) & 1) != 0 && fileLoadMenuState.inputRepeatTimer >= 3) {
            selection = D_003BC840;
            fileLoadMenuState.inputRepeatTimer = 0;
            if (selection > 0) {
                if (offset == 2) {
                    offset = 1;
                } else if (offset == 1) {
                    D_003BC840 = selection - 1;
                    fileLoadSetMode(FILE_LOAD_MENU_SLIDE_SELECTION_UP);
                    fileSetMenuValueAndInitializeFlags(0x80);
                }
            } else {
                offset -= offset > 0;
            }
        }

        if (fileIsLoadedWithActiveFlow(((u8)dds3PadButtonState[0x27] >> 1) & 1) != 0 && fileLoadMenuState.inputRepeatTimer >= 3) {
            selection = D_003BC840;
            fileLoadMenuState.inputRepeatTimer = 0;
            if (selection < 7) {
                if (offset == 0) {
                    offset = 1;
                } else if (offset == 1) {
                    D_003BC840 = selection + 1;
                    fileLoadSetMode(FILE_LOAD_MENU_SLIDE_SELECTION_DOWN);
                    fileSetMenuValueAndInitializeFlags(0x80);
                }
            } else {
                offset += offset < 2;
            }
        }

        fileSelectedSlotIndex = D_003BC840 + offset;
        if (oldSelection != fileSelectedSlotIndex) {
            p2sdPlaySE(0, 0x7F, 0x3F);
        }
        if (fileSelectedSlotIndex < oldSelection) {
            D_003BC818 = -8;
        } else if (oldSelection < fileSelectedSlotIndex) {
            D_003BC818 = 8;
        }
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
            D_003BC82C = 1;
        }
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            D_003BC82C = 2;
        }
    }

    if (fileReqPoll() != FILE_REQ_POLL_PENDING) {
        requestStatus = fileReqGetStatus(fileMemoryCardRequestIndex);
        action = D_003BC82C;
        if (requestStatus == FILE_REQ_STATUS_NO_CARD) {
            return fileBeginSlotMetadataRefresh();
        }

        if (action == 0) {
            fileReqBegin(fileMemoryCardRequestIndex);
            return 0;
        }
        if (action == 1) {

            slSetInfo(0);
            if (D_003BC848 == FILE_MENU_START_MODE_LOAD) {
        fileReqSetSelectedSlot(fileMemoryCardRequestIndex, fileSelectedSlotIndex);
        flags = fileReqGetSlotFlags(fileMemoryCardRequestIndex, fileSelectedSlotIndex);
        if ((flags & FILE_REQ_SLOT_SELECTABLE_SAVE_MASK) ==
            FILE_REQ_SLOT_SELECTABLE_SAVE_MASK) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            if (D_003BC824 != 0) {
                D_003BC820 = action;
                D_003BC834 = action;
                return slQueryYesNo(slEndingUpd2, fileScanSlotStates, 1);
            }
            D_003BC820 = action;
            D_003BC834 = action;
            return slQueryYesNo(slSaveData, fileScanSlotStates, 1);
        }

        action = flags & FILE_REQ_SLOT_DIRECTORY_DELETE_MASK;
        if (action == FILE_REQ_SLOT_DIRECTORY_READY) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            D_003BC820 = action;
            return slSaveData();
        }

        p2sdPlaySE(8, 0x7F, 0x3F);
        D_003BC820 = 0;
        if (slSizeChk() != 0) {
            return slSaveData();
        }
        slSetInfo(0);
        D_003BC854 = 5;
        D_003BC858 = 0;
        fileReqBegin(fileMemoryCardRequestIndex);
        return (s32)slErrorWaitToFileMc;
    }

            fileReqSetSelectedSlot(fileMemoryCardRequestIndex, fileSelectedSlotIndex);
            flags = fileReqGetSlotFlags(fileMemoryCardRequestIndex, fileSelectedSlotIndex);
            if ((flags & FILE_REQ_SLOT_CAN_READ_SAVE) != 0) {
                p2sdPlaySE(8, 0x7F, 0x3F);
                D_003BC834 = 3;
                return slQueryYesNo(slLoadData, fileScanSlotStates, 1);
            }
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            return fileScanSlotStates();
        }

        if (action == 2) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            fileResetLoadContextSlide();
            if (D_003BC824 != 0) {
                return slEndingExit();
            }
            return slCancel();
        }
    }

    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2658);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2668);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2678);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2688);

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

    if (D_003BC804 == 1) {
        return 0;
    }
    if (D_003BC818 > 0) {
        D_003BC818--;
    }
    if (D_003BC818 < 0) {
        D_003BC818++;
    }
    if (D_003BC848 == FILE_MENU_START_MODE_LOAD) {
        evtSubmitTexturedGradientQuad(0x56, 0x33, 0x137, 0x17, 5, 0x1C4, 0x137, 0x17, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC880);
    } else {
        evtSubmitTexturedGradientQuad(0x56, 0x33, 0x137, 0x17, 5, 0x1DD, 0x137, 0x17, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC880);
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
    targetY = D_003BC840 * -110 + 104;
    delta = targetY - D_003BC83C;
    delta /= 3.0f;
    if (delta < 0.0f && delta > -16.0f) {
        delta = -16.0f;
    }
    if (delta > 0.0f && delta < 16.0f) {
        delta = 16.0f;
    }
    D_003BC83C = targetY;
    fileCursorStepUp();
    fileFadeStepDown();
    if (D_003BC850 == 0) {
        fileCursorPulseUpdate();
        fileDrawCursorPulse(0x25, 0x1F, 0x53);
    }
    fileLoadCtxSlideUpdate();
    if (fileLoadMenuState.slideMode == FILE_LOAD_MENU_SLIDE_IDLE) {
        rowCount = 3;
        rowBase = D_003BC840;
        slotBase = D_003BC840;
    } else if (fileLoadMenuState.slideMode == FILE_LOAD_MENU_SLIDE_SELECTION_DOWN) {
        rowCount = 4;
        rowBase = D_003BC840;
        slotBase = D_003BC840 - 1;
        if (slotBase < 0) {
            slotBase = 0;
        }
    } else {
        rowCount = 4;
        rowBase = D_003BC840 - 1;
        slotBase = D_003BC840;
    }
    for (row = 0; row < rowCount; row++) {
        slot = slotBase + row;
        y = (s32)D_003BC83C + (rowBase + row) * 110 + fileLoadMenuState.rowVerticalOffset;
        if (fileSelectedSlotIndex == slot) {
            titleColor = 0xA09DC380;
            detailColor = 0x89FEFF80;
        } else {
            titleColor = 0x805D8340;
            detailColor = 0x69BEBF40;
        }
        if (fileLoadMenuState.slideMode != FILE_LOAD_MENU_SLIDE_IDLE) {
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
        evtSubmitTexturedGradientQuad(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, baseColor, baseColor, baseColor, baseColor, D_003BC868);
        if (fileSlotDisplayStates[slot] == FILE_SLOT_DISPLAY_NO_RECOGNIZED_SAVE) {
            evtSubmitTexturedGradientQuad(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, D_003BC870);
        } else if (fileSlotDisplayStates[slot] == FILE_SLOT_DISPLAY_DIRECTORY_WITHOUT_SAVE) {
            evtSubmitTexturedGradientQuad(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, D_003BC878);
        } else if (fileSavePreviewRecords[slot].status > 0) {
            evtSubmitTexturedGradientQuad(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, D_003BC874);
        }
        if (slot != 10) {
            fileDrawSlotIcon(slot + 1, 0x5A, y + 4, alpha);
            if (fileSelectedSlotIndex == slot) {
                trailAlpha = 16;
                if (D_003BC818 > 0) {
                    for (trail = 0; trail < D_003BC818; trail++, trailAlpha -= 2) {
                        evtSubmitTexturedGradientQuad(0x1F, y - trail * 4, 0x1BB, trail * 4 + 16, 3, 3, 0x1BB, trail * 4 + 16, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, D_003BC86C);
                    }
                } else if (D_003BC818 < 0) {
                    trailAlpha = 32;
                    for (trail = 0; trail < -D_003BC818; trail++, trailAlpha -= 4) {
                        evtSubmitTexturedGradientQuad(0x1F, y + 0x58, 0x1BB, trail * 4 + 16, 3, 0x5B - trail * 4, 0x1BB, trail * 4 + 16, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, D_003BC86C);
                    }
                }
            }
            if (fileSlotDisplayStates[slot] != FILE_SLOT_DISPLAY_SAVE_PRESENT) {
                if (fileSelectedSlotIndex == slot) {
                    u32 highlightColor = ((D_0037D4D0[0] + 0x80) << 24) | 0x808080;
                    evtSubmitTexturedGradientQuad(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, highlightColor, highlightColor, highlightColor, highlightColor, D_003BC86C);
                }
            } else {
                u32 bannerColor;

                preview = &fileSavePreviewRecords[slot];
                if (preview->newCycle != DAT_NEW_CYCLE_STATE_INACTIVE) {
                    mnuDrawAndStoreTextGlyphHandle(0x40, y + 0x20, detailColor, fileNewCycleLabel);
                }
                bannerColor = (alpha << 24) | 0x808080;
                evtSubmitTexturedGradientQuad(0x88, y + 4, 0x137, 0x1F, 4, 0x1C5, 0x137, 0x1F, bannerColor, bannerColor, bannerColor, bannerColor, D_003BC884);
                if (preview->newCycle == DAT_NEW_CYCLE_STATE_INACTIVE) {
                    if (preview->mapGroup == 0) {
                        mnuDrawAndStoreTextGlyphHandle(0xF0, y, detailColor, D_00347C68[preview->mapIndex]);
                    } else {
                        mnuDrawAndStoreTextGlyphHandle(0xF0, y, detailColor, D_003482A8[preview->mapIndex]);
                    }
                }
                ticks = fileSavePreviewRecords[slot].playTicks;
                if (ticks > 0x066FEBF8) {
                    ticks = 0x066FEBF8;
                }
                hours = ticks / 108000;
                minutes = (ticks % 108000) / 1800;
                seconds = (ticks % 1800) / 30;
                sprintf(text, D_003BC898, hours);
                fileDrawMenuImageAtPoint(0x187, y + 0x15, titleColor, text);
                sprintf(text, D_003BC8A0, minutes);
                fileDrawMenuImageAtPoint(0x1A7, y + 0x15, titleColor, text);
                sprintf(text, D_003BC8A0, seconds);
                fileDrawMenuImageAtPoint(0x1BD, y + 0x15, titleColor, text);
                if (fileSelectedSlotIndex == slot) {
                    u32 highlightColor = ((D_0037D4D0[0] + 0x80) << 24) | 0x808080;
                    evtSubmitTexturedGradientQuad(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, highlightColor, highlightColor, highlightColor, highlightColor, D_003BC86C);
                }
                if (fileSavePreviewRecords[slot].newCycle == DAT_NEW_CYCLE_STATE_INACTIVE) {
                    if (fileSelectedSlotIndex == slot) {
                        partyColor = (alpha << 24) | 0x808080;
                    } else {
                        partyColor = (((alpha >> 1) + (alpha >> 2)) << 24) | 0x282828;
                    }
                    for (partyIndex = 0; partyIndex < 5; partyIndex++) {
                        if (fileSavePreviewRecords[slot].party[partyIndex] != 0) {
                            portrait = 0;
                            switch (fileSavePreviewRecords[slot].party[partyIndex]) {
                            case 1: portrait = 0; break;
                            case 3: portrait = 1; break;
                            case 4: portrait = 2; break;
                            case 5: portrait = 3; break;
                            case 6: portrait = 4; break;
                            }
                            evtSubmitTexturedGradientQuad(partyIndex * 60 + 0x80, y + 0x23, 0x32, 0x3E, 1, 1, 0x32, 0x3E, partyColor, partyColor, partyColor, partyColor, fileSavePreviewPortraitTextures[portrait]);
                            sprintf(text, D_003BC8A8, fileSavePreviewRecords[slot].levels[partyIndex]);
                            fileDrawMenuImageAtPoint(partyIndex * 60 + 0x98, y + 0x51, titleColor, text);
                        }
                    }
                }
            }
        }
    }
    evtSubmitPrimaryAlphaBlendMode(1);
    fileScrollArrowState.angle = (fileScrollArrowState.angle + 10) % 360;
    if (D_003BC840 > 0) {
        u32 arrowColor;

        evtSubmitTexturedGradientQuad(0xF8, 0x50, 0xF, 0x22, 0x35, 2, 0xF, 0x22, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC87C);
        wave = sdfSin((f32)((fileScrollArrowState.angle + 90) % 360) / 180.0f * 3.1415899f);
        fileScrollArrowState.upAlpha = (s32)((wave + 1.0f) * 0.5f * 176.0f + 16.0f);
        arrowColor = (fileScrollArrowState.upAlpha << 24) | 0x808080;
        evtSubmitTexturedGradientQuad(0xF8, 0x50, 0xF, 0x22, 0x35, 2, 0xF, 0x22, arrowColor, arrowColor, arrowColor, arrowColor, D_003BC87C);
    }
    if (D_003BC840 < 7) {
        u32 arrowColor;

        evtSubmitTexturedGradientQuad(0xF8, 0x18B, 0xF, 0x22, 0x47, 2, 0xF, 0x22, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC87C);
        wave = sdfSin((f32)((fileScrollArrowState.angle + 90) % 360) / 180.0f * 3.1415899f);
        fileScrollArrowState.downAlpha = (s32)((wave + 1.0f) * 0.5f * 176.0f + 16.0f);
        arrowColor = (fileScrollArrowState.downAlpha << 24) | 0x808080;
        evtSubmitTexturedGradientQuad(0xF8, 0x18B, 0xF, 0x22, 0x47, 2, 0xF, 0x22, arrowColor, arrowColor, arrowColor, arrowColor, D_003BC87C);
    }
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 1);
    eglSetOt(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}


s32 slCalc(dds3ProcessID_t *task) {
    FileMenuStateResult next;
    struct FileRequest *job;
    FileMenuStateHandler cur;
    D_003BC814++;
    fileDrawMenuFrame((s32)task);
    next.encodedValue = fileMenuStateHandler(task);
    if (next.encodedValue == FILE_MENU_STATE_EXIT) {
        return next.encodedValue;
    }
    job = fileSaveIconRequest;
    cur = fileMenuStateHandler;
    if (next.encodedValue != FILE_MENU_STATE_RETAIN) {
        cur = next.nextHandler;
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

s32 fileDrawMenuFrame(s32 work) {
    eglSetOt(0x52);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (D_003BC80C != 0) {
        evtSubmitTexturedGradientQuad(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_003BC880);
        evtSubmitTexturedGradientQuad(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_003BC884);
    }
    if (D_003BC810 != 0) {
        fileDrawSlotListAndPreview(work);
    }
    fileShowFlowDialog();
    fileShowPromptDialog();
    fileShowStatusDialog();
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

extern s32 D_003BC830;
extern s32 D_003BC838;
void fileDestroyMemoryCardMenuTask(void);

void fileInitCursorPulse(void);

/* Load the memory-card pack textures, pick the menu entry state for the
 * requested mode (load, slot select, new-cycle save, or boot) and
 * start the file-menu task with every flow counter cleared. */
void fileEnterMcPackScene(FileMenuStartMode mode) {
    FilePacRequest *request;
    PacWork *node;
    u32 index = 0;
    void *world;
    s32 enabled;

    dds3SoftResetOff();
    request = (FilePacRequest *)pacLoad("/mc/mcpack.LB");
    pacSync((struct FileRequest *)request);
    node = request->packet.queueHead;
    while (node != NULL) {
        switch (index) {
        case 0:
            D_003BC868 = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 1:
            D_003BC86C = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 2:
            D_003BC870 = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 3:
            D_003BC874 = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 4:
            D_003BC878 = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 5:
            D_003BC87C = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 6:
            D_003BC880 = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 7:
            D_003BC884 = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 8:
            fileSavePreviewPortraitTextures[0] = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 9:
            fileSavePreviewPortraitTextures[1] = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 10:
            fileSavePreviewPortraitTextures[2] = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 11:
            fileSavePreviewPortraitTextures[3] = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        case 12:
            fileSavePreviewPortraitTextures[4] = sdfTexSet((SdfTextureFileHeader *)(sdfMemGetPtr(PAC_WORK_ALLOCATION(node))));
            break;
        }
        sdfMemFreeDirect(PAC_WORK_ALLOCATION(node));
        index++;
        node = node->next;
    }
    pacFileHandleFree_(request);
    enabled = 1;
    if (mode == FILE_MENU_START_MODE_LOAD) {
        D_003BC80C = enabled;
        D_003BC824 = 0;
        fileMenuStateHandler = (s32 (*)())fileLoadIconFileAndResetSelection;
    } else if (mode == FILE_MENU_START_MODE_SLOT_SELECTION) {
        D_003BC824 = 0;
        D_003BC80C = enabled;
        fileMenuStateHandler = (s32 (*)())fileResetSlotSelection;
    } else if (mode == FILE_MENU_START_MODE_NEW_CYCLE) {
        D_003BC80C = enabled;
        D_003BC824 = enabled;
        evtBitOn(0xB90);
        DDS3_GBWK->header.status = (u16)DDS3_GBWK->header.status + 1;
        DDS3_GBWK->header.transition = DAT_NEW_CYCLE_STATE_ACTIVE;
        fileMenuStateHandler = (s32 (*)())fileLoadIconFileAndBeginSlotReset;
    } else {
        D_003BC824 = 0;
        D_003BC80C = 0;
        fileMenuStateHandler = (s32 (*)())slBootProc1st;
    }
    dds3StartProcess("FileMentCalc", 0x2B0A, 0, 0, slCalc, fileDestroyMemoryCardMenuTask, 0);
    D_003BC7FC = 1;
    fileInitCursorPulse();
    dds3FadeIn(0, 0, 0, 8);
    slSetInfo(0);
    D_003BC81C = FILE_MENU_OPERATION_NO_RESULT;
    D_003BC848 = mode;
    D_003BC830 = -1;
    fileWaitTicksRemaining = 0;
    fileSelectionPending = FILE_SELECTION_FLOW_IDLE;
    D_003BC854 = 0;
    D_003BC858 = 0;
    D_003BC810 = 0;
    D_003BC820 = 0;
    fileSlotSelectionPollCount = 0;
    D_003BC814 = 0;
    D_003BC804 = 0;
    D_003BC82C = 0;
    D_003BC834 = 0;
    D_003BC838 = 0;
    D_003BC85C = 0;
    D_003BC860 = 0;
    fileClearAllSlotFlags();
    world = dds3GetActiveScene();
    if (world != 0) {
        dds3VisibleSceneBasic((dds3Basic_t *)world, 0);
    }
}

void fileReleaseMenuResources(void);

void fileDestroyMemoryCardMenuTask(void) {
    fileReleaseMenuResources();
}

void fileMenuReleaseNoOp(void) {
}

void fileReleaseMenuResources(void) {
    struct sdfTexHandle_t **slot;
    s32 i;
    void *world;
    struct sdfTexHandle_t *texture;

    if (D_003BC7FC != 0) {
        D_003BC7FC = 0;
        slot = fileSavePreviewPortraitTextures;
        i = 4;
        do {
            texture = *slot;
            i--;
            if (texture != 0) {
                sdfTexReleaseReferenceViaHandler(texture);
                *slot = 0;
            }
            slot++;
        } while (i >= 0);
        if (D_003BC884 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC884);
            D_003BC884 = 0;
        }
        if (D_003BC880 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC880);
            D_003BC880 = 0;
        }
        if (D_003BC87C != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC87C);
            D_003BC87C = 0;
        }
        if (D_003BC878 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC878);
            D_003BC878 = 0;
        }
        if (D_003BC874 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC874);
            D_003BC874 = 0;
        }
        if (D_003BC870 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC870);
            D_003BC870 = 0;
        }
        if (D_003BC86C != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC86C);
            D_003BC86C = 0;
        }
        if (D_003BC868 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC868);
            D_003BC868 = 0;
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

FileSelectionFlowState fileGetSelectionPendingFlag(void) {
    return fileSelectionPending;
}

extern FileSelectionCallback D_003BD908, D_003BD90C;
/* Yes/no cursor input while the card request is idle. */
static inline void filePollPromptChoice(void) {
    if (D_003BC838 == 0 && slChkWindowOpenDone() != 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x27] < 0) != 0) {
            if (D_003BC830 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 1;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x26] < 0) != 0) {
            if (D_003BC830 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 0;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
            if (D_003BC830 == 0 && (D_003BC834 == 1 || D_003BC834 == 3)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_003BC830 != 0) {
                D_003BC838 = 2;
            } else {
                D_003BC838 = 1;
            }
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_003BC838 = 2;
        }
    }
}

s32 fileUpdatePromptDialog(void) {
    s32 status;
    s32 decision;

    if (D_003BC834 - 5 < 2) {
        s32 requestStatus;
        filePollPromptChoice();
        if (fileReqPoll() == FILE_REQ_POLL_PENDING) {
            goto idle;
        }
        requestStatus = fileReqGetStatus(fileMemoryCardRequestIndex);
        switch (requestStatus) {
        case FILE_REQ_STATUS_READY:
            decision = (s32)D_003BC838;
            if (decision == 1) {
                /* Retail 0x0028F7A4 beq: shared yes tail (.L0028FAB8). */
                goto yes;
            }
            if (decision >= 2) {
                /* Retail 0x0028F7AC beqz: shared checkNo tail (.L0028FA98). */
                goto checkNo;
            }
            if (decision != 0) {
                goto idle;
            }
            if (D_003BC834 == 5) {
                D_003BC834 = 0;
                D_003BC830 = -1;
                return slBootRetry();
            }
            /* Retail 0x0028F7C4 bne: shared begin tail (.L0028FAA8). */
            goto begin;
        case FILE_REQ_STATUS_UNFORMATTED:
            /* Retail 0x0028F790 beq: shared yes tail (.L0028FAB8). */
            goto yes;
        }
        requestStatus = (s32)D_003BC838;
        D_003BC834 = 5;
        switch (requestStatus) {
        case 0:
            /* Retail 0x0028F800 beqz: shared begin tail (.L0028FAA8). */
            goto begin;
        case 1:
            /* Retail 0x0028F7EC beq: shared yes tail (.L0028FAB8). */
            goto yes;
        case 2:
            /* Retail 0x0028F810 beq: shared no tail (.L0028FAD8). */
            goto no;
        }
        return 0;
    }
    if (D_003BC834 == 4 || D_003BC834 == 8) {
        if (dds3PadButtonState[0x27] < 0) {
            if (D_003BC830 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 1;
        } else if (dds3PadButtonState[0x26] < 0) {
            if (D_003BC830 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 0;
        } else if (dds3PadButtonState[0x21] < 0) {
            if (D_003BC830 == 0 && (D_003BC834 == 1 || D_003BC834 == 3 || D_003BC834 == 9)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_003BC830 == 0) {
                /* Retail 0x0028F8F8 b: enters the shared yes tail mid-block at .L0028FAC0;
                 * its delay slot (sw $0,D_003BC834) is the tail's first store. */
                goto yes;
            }
            /* Retail 0x0028F8F0 bnez: enters the shared no tail mid-block at .L0028FADC;
             * its delay slot (addiu $2,$0,-1) is the tail's first step. */
            goto no;
        } else if (dds3PadButtonState[0x23] < 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_003BC830 = -1;
            D_003BC834 = 0;
            return D_003BD90C();
        }
        goto idle;
    }
    filePollPromptChoice();
    if (fileReqPoll() == FILE_REQ_POLL_PENDING) {
        goto idle;
    }
    status = fileReqGetStatus(fileMemoryCardRequestIndex);
    switch (status) {
    case FILE_REQ_STATUS_READY:
    case FILE_REQ_STATUS_UNFORMATTED:
        decision = (s32)D_003BC838;
        if (decision == 1) {
            /* Retail 0x0028FA78 beq: shared yes tail (.L0028FAB8). */
            goto yes;
        }
        if (decision >= 2) {
            /* Retail 0x0028FA80 beqz: shared checkNo tail (.L0028FA98). */
            goto checkNo;
        }
        if (decision == 0) {
            /* Retail 0x0028FA88 beqz: shared begin tail (.L0028FAA8). */
            goto begin;
        }
        return 0;
    default:
        /* Retail 0x0028FA60 beqz: shared restartRequest tail (.L0028FAF8);
         * the blezl at 0x0028FA68 enters mid-block at .L0028FAFC (its delay slot does the tail's first store). */
        goto restartRequest;
    }
checkNo:
    switch (decision) {
    case 2:
        /* Retail 0x0028FA98 beq: first insn of the shared checkNo tail;
         * branches to the shared no tail (.L0028FAD8). */
        goto no;
    default:
        return 0;
    }
begin:
    fileReqBegin(fileMemoryCardRequestIndex);
    goto idle;
yes:
    D_003BC834 = 0;
    D_003BC830 = -1;
    return D_003BD908();
no:
    D_003BC834 = 0;
    D_003BC830 = -1;
    return D_003BD90C();
restartRequest:
    D_003BC834 = 0;
    D_003BC830 = -1;
    fileClearAllSlotFlags();
    slSetInfo(0);
    D_003BC854 = 1;
    D_003BC858 = 0;
    fileReqBegin(fileMemoryCardRequestIndex);
    return (s32)filePollSlotDetectionAndBranch;
    /* Retail jumps (b 0x0028F864, b 0x0028F890) reach one shared `return 0` block at 0x0028FB30;
     * the beql 0x0028F770 and bnez 0x0028F7B4 enter at 0x0028FB34 with v0 already zeroed by their delay slots.
     * A plain `return 0` per site lets the compiler reuse v0 == 0 from the fileReqPoll test (26 of 322 words differ). */
idle:
    return 0;
}

extern s32 fileUpdatePromptDialog(void);
s32 slQueryYesNo(FileSelectionCallback start, FileSelectionCallback finish, s32 mode) {
    D_003BD908 = start;
    D_003BD90C = finish;
    D_003BC830 = mode;
    D_003BC838 = 0;
    if (D_003BC834 != 4 && D_003BC834 != 8) {
        fileReqBegin(fileMemoryCardRequestIndex);
    }
    return (s32)fileUpdatePromptDialog;
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf_textured_rect.h"
#include "itf_mes_window.h"
#include "sdf_trig.h"

struct sdfTexHandle_t;

extern s32 D_003BC830;        /* the highlighted answer: 0 yes, otherwise no; -1 while no prompt runs */
extern u32 D_003BC834;        /* the prompt shown; 0 for none. 1 overwrite the save file, 2 card
                              * unformatted, 3 a slot's action, 4 save a new cycle, 5 no memory
                              * card, 6 too little free space, 7 cancel the save, 8 return to the
                              * title screen, 9 overwrite data */
extern s32 fileSelectedSlotIndex;
extern s32 D_0037D4D0[];      /* highlight pulse: [2] its angle in degrees, [3] its alpha */
extern char *D_003B2718[20];  /* each slot's prompt title */
extern char D_0037D8B8[], D_0037D8D8[], D_0037D900[];
extern char D_0037D5C8[], D_0037D5F8[], D_0037D718[];
extern char D_0037DB10[];
extern char D_0037DBA0[], D_0037DBB0[], D_0037DBD0[];
extern char D_0037DC60[], D_0037DC78[], D_0037DC90[], D_0037DCB0[], D_0037DCC8[], D_0037DCE8[], D_0037DD00[],
    D_0037DD28[];
extern char D_0037DD40[], D_0037DD58[], D_0037DD70[], D_0037DD88[], D_0037DDA0[], D_0037DDB8[], D_0037DDD8[],
    D_0037DDF0[], D_0037DE10[];
extern char D_0037D748[];
extern char D_0037DBF8[];
extern char D_0037DC18[], D_0037DC38[], D_0037DC50[];
extern char D_003BC8B0[]; /* "Yes" */
extern char D_003BC8B8[]; /* "No" */
extern char D_003BC8C0[]; /* " " */

extern void eglSetOt(u32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 fileDrawStatusDialogFrame(void);
extern s32 slChkWindowOpenDone(void);

#define FILE_PROMPT_TEXT_COLOR 0x89FEFF80
#define FILE_PROMPT_ANSWER_COLOR 0xA09DC380
#define FILE_PROMPT_HIGHLIGHT_RGB 0xAEC014
#define FILE_PROMPT_HIGHLIGHT_EDGE 0x40AEC014
#define FILE_PROMPT_LINE_HEIGHT 24
#define FILE_PROMPT_ANSWER_X 86

/* Text line `line` of a prompt whose first line is at (x, top). */
static void filePromptLine(s32 x, s32 top, s32 line, const char *text) {
    mcdCreateFontDrawHandle(x, top + line * FILE_PROMPT_LINE_HEIGHT, FILE_PROMPT_TEXT_COLOR, (u32)text);
}

static void filePromptLines(s32 x, s32 top, const char *const *lines, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        filePromptLine(x, top, i, lines[i]);
    }
}

static void filePromptSurface(void) {
    eglSetOt(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
}

/* Draw the yes/no answers under a prompt, the highlighted one under a pulsing
 * bar and cursor. */
static void filePromptAnswers(void) {
    s32 yesRow = (D_003BC834 == 5 || D_003BC834 == 6) ? 0x132 : 0x102;
    s32 noRow = yesRow + FILE_PROMPT_LINE_HEIGHT;
    s32 highlightRow;
    struct sdfTexHandle_t *texture;
    s32 angle;
    f32 wave;
    u32 cursorColor;

    eglSetOt(0x56);
    evtSubmitPrimaryAlphaBlendMode(1);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    texture = itfMesGetWindowTexture();
    angle = (D_0037D4D0[2] + 8) % 360;
    D_0037D4D0[2] = angle;
    wave = sdfSin((f32)((angle + 90) % 360) / 180.0f * 3.1415899f);
    D_0037D4D0[3] = (s32)((wave + 1.0f) * 0.5f * 80.0f + 112.0f);

    highlightRow = D_003BC830 == 0 ? yesRow : noRow;
    /* The bar's halves fade in from its ends. */
    evtSubmitDefaultDepthGradientRect(0x3D, highlightRow, 0x30, 0x18, FILE_PROMPT_HIGHLIGHT_RGB,
                                      FILE_PROMPT_HIGHLIGHT_EDGE, FILE_PROMPT_HIGHLIGHT_EDGE,
                                      FILE_PROMPT_HIGHLIGHT_RGB);
    evtSubmitDefaultDepthGradientRect(0x6D, highlightRow, 0x30, 0x18, FILE_PROMPT_HIGHLIGHT_EDGE,
                                      FILE_PROMPT_HIGHLIGHT_RGB, FILE_PROMPT_HIGHLIGHT_RGB,
                                      FILE_PROMPT_HIGHLIGHT_EDGE);
    cursorColor = ((u32)D_0037D4D0[3] << 24) | FILE_PROMPT_HIGHLIGHT_RGB;
    evtSubmitTexturedGradientQuad(0x45, highlightRow + 8, 0xC, 0xB, 0x29, 1, 0xC, 0xB, cursorColor, cursorColor,
                              cursorColor, cursorColor, texture);

    mcdCreateFontDrawHandle(FILE_PROMPT_ANSWER_X, yesRow, FILE_PROMPT_ANSWER_COLOR, (u32)D_003BC8B0);
    mcdCreateFontDrawHandle(FILE_PROMPT_ANSWER_X, noRow, FILE_PROMPT_ANSWER_COLOR, (u32)D_003BC8B8);
}

/* Draw the memory-card menu's current prompt and, once its load step is
 * complete, its answers. Upstream: INCLUDE_ASM in game/code_0028A150; same
 * calls and arguments as the retail routine, with its repeated text blocks
 * as tables. */
void fileShowPromptDialog(void) {
    static const char *const overwriteFileLines[] = {D_0037D8B8, D_0037D8D8, D_0037D900};
    static const char *const unformattedCardLines[] = {D_0037D5C8, D_0037D5F8, D_0037D718};
    static const char *const newCycleLines[] = {D_0037DBA0, D_0037DBB0, D_0037DBD0, D_003BC8C0};
    static const char *const noCardLines[] = {D_0037DC60, D_0037DC78, D_0037DC90, D_0037DCB0,
                                              D_0037DCC8, D_0037DCE8, D_0037DD00, D_0037DD28};
    static const char *const noSpaceLines[] = {D_0037DD40, D_0037DD58, D_0037DD70, D_0037DD88, D_0037DDA0,
                                            D_0037DDB8, D_0037DDD8, D_0037DDF0, D_0037DE10};
    static const char *const overwriteDataLines[] = {D_0037DC18, D_0037DC38, D_0037DC50};

    if (D_003BC830 == -1 || D_003BC834 == 0) {
        return;
    }
    filePromptSurface();
    if (D_003BC834 == 1 && fileDrawStatusDialogFrame()) {
        filePromptLines(86, 114, overwriteFileLines, 3);
    }
    if (D_003BC834 == 2 && fileDrawStatusDialogFrame()) {
        filePromptLines(62, 114, unformattedCardLines, 3);
    }
    if (D_003BC834 == 3 && fileDrawStatusDialogFrame()) {
        filePromptLine(86, 114, 0, D_003B2718[fileSelectedSlotIndex]);
        filePromptLine(86, 114, 1, D_0037DB10);
    }
    if (D_003BC834 == 4 && fileDrawStatusDialogFrame()) {
        filePromptLines(86, 114, newCycleLines, 4);
    }
    if (D_003BC834 == 5 && fileDrawStatusDialogFrame()) {
        filePromptLines(62, 90, noCardLines, 8);
    }
    if (D_003BC834 == 6 && fileDrawStatusDialogFrame()) {
        filePromptLines(62, 90, noSpaceLines, 9);
    }
    if (D_003BC834 == 7 && fileDrawStatusDialogFrame()) {
        filePromptLine(86, 114, 0, D_0037D748);
    }
    if (D_003BC834 == 8 && fileDrawStatusDialogFrame()) {
        filePromptLine(86, 114, 0, D_0037DBF8);
    }
    if (D_003BC834 == 9 && fileDrawStatusDialogFrame()) {
        filePromptLines(86, 114, overwriteDataLines, 3);
    }
    if (slChkWindowOpenDone()) {
        filePromptAnswers();
    }
    filePromptSurface();
}
#else
INCLUDE_ASM(const s32, "game/code_0028A150", fileShowPromptDialog);
#endif

FileMenuOperationResult fileGetSaveLoadResult(void) {
    return D_003BC81C;
}



void slSetTmnlID(s32 x, s32 y) {
    DatGameState *state = DDS3_GBWK;

    state->vr.previewX = x;
    state->vr.previewY = y;
}

void fileSetMenuValueAndInitializeFlags(u32 value) {
    D_0037D4D0[0] = value;
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
    fileLoadMenuState.transitionTickCount++;
    fileLoadMenuState.transitionTickCount = fileLoadMenuState.transitionTickCount <= 0 ? 0 : fileLoadMenuState.transitionTickCount > 12 ? 12 : fileLoadMenuState.transitionTickCount;
    if (fileLoadMenuState.transitionTickCount >= 7) {
        fileLoadMenuState.standardRowAlpha += 0x30;
        fileLoadMenuState.standardRowAlpha = fileLoadMenuState.standardRowAlpha <= 0 ? 0 : fileLoadMenuState.standardRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.standardRowAlpha;
    }
}

void fileFadeStepDown(void) {
    D_0037D4D0[0] -= 0x10;
    D_0037D4D0[0] = D_0037D4D0[0] <= 0 ? 0 : D_0037D4D0[0] > 0x80 ? 0x80 : D_0037D4D0[0];
}

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2718);

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
                  (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, D_003BC87C);
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

void fileInitCursorPulse(void) {
    fileCursorPulseState.mode[1] = 0;
    D_0037D4AC[0] = 0;
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

extern const s32 D_003B28A0[3][2];

void fileDrawCursorPulse(s32 x, s32 y, s32 surface) {
    s32 texturePositions[3][2] = {{54, 39}, {54, 67}, {54, 39}};
    s32 offsets[3][2];
    s32 i;

    memcpy(offsets, D_003B28A0, sizeof(offsets));
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
                             D_003BC87C);
            }
        }
    }
}

void fileLoadSetMode(FileLoadMenuSlideMode mode) {
    fileLoadMenuState.rowVerticalOffset = 0;
    fileLoadMenuState.slideMode = mode;
    if (mode == FILE_LOAD_MENU_SLIDE_SELECTION_DOWN) {
        fileLoadMenuState.trailingRowAlpha = 0;
        fileLoadMenuState.leadingRowAlpha = 0x74;
    } else {
        fileLoadMenuState.leadingRowAlpha = 0;
        fileLoadMenuState.trailingRowAlpha = 0x74;
    }
}

void fileResetLoadContextSlide(void) {
    fileLoadMenuState.rowVerticalOffset = 0;
    fileLoadMenuState.slideMode = FILE_LOAD_MENU_SLIDE_IDLE;
    fileLoadMenuState.transitionTickCount = 0;
}

void fileLoadCtxSlideUpdate(void) {
    switch (fileLoadMenuState.slideMode) {
    case FILE_LOAD_MENU_SLIDE_SELECTION_DOWN:
        fileLoadMenuState.rowVerticalOffset -= 0x14;
        fileLoadMenuState.rowVerticalOffset = fileLoadMenuState.rowVerticalOffset <= -0x6E ? -0x6E : fileLoadMenuState.rowVerticalOffset > 0 ? 0 : fileLoadMenuState.rowVerticalOffset;
        if (fileLoadMenuState.rowVerticalOffset <= -0x6E) {
            fileLoadMenuState.rowVerticalOffset = 0;
            fileLoadMenuState.slideMode = FILE_LOAD_MENU_SLIDE_IDLE;
        }
        fileLoadMenuState.leadingRowAlpha -= 0x20;
        fileLoadMenuState.leadingRowAlpha = fileLoadMenuState.leadingRowAlpha <= 0 ? 0 : fileLoadMenuState.leadingRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.leadingRowAlpha;
        fileLoadMenuState.trailingRowAlpha += 0x10;
        fileLoadMenuState.trailingRowAlpha = fileLoadMenuState.trailingRowAlpha <= 0 ? 0 : fileLoadMenuState.trailingRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.trailingRowAlpha;
        break;
    case FILE_LOAD_MENU_SLIDE_SELECTION_UP:
        fileLoadMenuState.rowVerticalOffset += 0x14;
        fileLoadMenuState.rowVerticalOffset = fileLoadMenuState.rowVerticalOffset <= 0 ? 0 : fileLoadMenuState.rowVerticalOffset > 0x6E ? 0x6E : fileLoadMenuState.rowVerticalOffset;
        if (fileLoadMenuState.rowVerticalOffset >= 0x6E) {
            fileLoadMenuState.rowVerticalOffset = 0;
            fileLoadMenuState.slideMode = FILE_LOAD_MENU_SLIDE_IDLE;
        }
        fileLoadMenuState.leadingRowAlpha += 0x10;
        fileLoadMenuState.leadingRowAlpha = fileLoadMenuState.leadingRowAlpha <= 0 ? 0 : fileLoadMenuState.leadingRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.leadingRowAlpha;
        fileLoadMenuState.trailingRowAlpha -= 0x20;
        fileLoadMenuState.trailingRowAlpha = fileLoadMenuState.trailingRowAlpha <= 0 ? 0 : fileLoadMenuState.trailingRowAlpha > 0x80 ? 0x80 : fileLoadMenuState.trailingRowAlpha;
        break;
    }
}

FileSlotFlagsChangeResult fileLoadStateChanged(void) {
    return fileSlotFlagMirror.current != fileSlotFlagMirror.previous;
}

void fileCacheSlotFlagsFromState(void) {
    fileSlotFlagMirror.current = fileSlotFlagMirror.previous =
        DDS3_GBWK->world.slotFlags;
}

void fileRestoreSlotFlagsToState(void) {
    DDS3_GBWK->world.slotFlags = fileSavedSlotFlags;
}

extern s32 fileTestSlotFlagsBit(FileConfigOption option, u32 *flags);

u8 fileToggleSlotFlagsBit(FileConfigOption option, u32 *flags) {
    switch (option) {
    case FILE_CONFIG_OPTION_RUMBLE:
        *flags ^= 2;
        if (fileTestSlotFlagsBit(option, flags) != 0) {
            dds3PadVibrationSet(0, 1, 0xF);
            dds3PadVibrationSet(1, 0x80, 0xF);
        } else {
            dds3PadVibrationSet(0, 0, 0xF);
            dds3PadVibrationSet(1, 0, 0xF);
        }
        return 1;
    case FILE_CONFIG_OPTION_COMMAND_SELECTION_MEMORY:
        *flags ^= 4;
        return 1;
    case FILE_CONFIG_OPTION_RESTORE_BATTLE_COMMAND:
        *flags ^= 8;
        return 1;
    case FILE_CONFIG_OPTION_REVERSE_FIELD_CAMERA:
        *flags ^= 0x10;
        return 1;
    default:
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B28A0);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B28C0);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B28D0);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B28E8);

s32 fileTestSlotFlagsBit(FileConfigOption option, u32 *flags) {
    switch (option) {
    case FILE_CONFIG_OPTION_RUMBLE:
        return ((((s32)*flags >> 1) ^ 1) & 1);
    case FILE_CONFIG_OPTION_COMMAND_SELECTION_MEMORY:
        return *flags & 4;
    case FILE_CONFIG_OPTION_RESTORE_BATTLE_COMMAND:
        return *flags & 8;
    case FILE_CONFIG_OPTION_REVERSE_FIELD_CAMERA:
        return ((((s32)*flags >> 4) ^ 1) & 1);
    case 6:
        return 1;
    default:
        return -1;
    }
}

s32 fileTestSavedSlotFlags(FileConfigOption option) {
    return fileTestSlotFlagsBit(option, &DDS3_GBWK->world.slotFlags);
}


extern FileConfigTask *GBWK;

extern struct MenuList *mnuCreateListState(s32, s32, s32);
extern MenuResourceWork *mnuRequestEffectResource(const char *, const char *);
extern u8 mnuHasEffectResourceHandle(MenuResourceWork *);
extern void mnuTitleStreamUpdateAndLogBgm(void);
extern s32 effRequestResourceByMode(const char *, const char *, s32, u32 *);
extern void fileDrawConfigurationMenuEntry(s32 x, s32 y, s32 depth,
                                           struct MenuList *list,
                                           FileConfigListNode *node,
                                           s32 drawArg);
extern char D_003BC8E0[];

/* Allocate the save/config task, its five-row list and its effect resource slots. */
FileConfigTask *fileCreateConfigTask(s32 mode) {
    char *names[4] = {
        "/camp/spr/n_con/n_con01.spr",
        "/camp/spr/n_con/n_con02.spr",
        "/camp/spr/n_con/n_con03.spr",
        "/camp/spr/n_con/n_con04.spr",
    };
    struct sdfMemHandle_t *block;
    FileConfigTask *task;
    struct MenuList *list;
    FileConfigListNode *node;
    s32 i;

    block = sdfMemAllocDirect(0x3C);
    task = (FileConfigTask *)sdfMemLock(block);
    memset(task, 0, 0x3C);
    task->backingAllocation = block;
    task->mode = mode;
    task->ticks = 0;
    task->state = FILE_CONFIG_TASK_STATE_WAITING_FOR_RESOURCES;
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
        task->effect = mnuRequestEffectResource(D_003BC8E0, "/facility/spr/mantra/mantr_bg.spr");
        while (mnuHasEffectResourceHandle(task->effect)) {
        }
        for (i = 0; i < 4; i++) {
            effRequestResourceByMode(D_003BC8E0, names[i], EFF_RESOURCE_SOURCE_TRANSIENT, (u32 *)&task->slots[i]);
        }
        mnuResetTitleStreamLocked();
        mnuQueueTitleSoundStreamEntry(0x14);
        mnuTitleStreamUpdateAndLogBgm();
        dds3FadeIn(0, 0, 0, 0xF);
        break;
    default:
        for (i = 0; i < 4; i++) {
            effRequestResourceByMode(D_003BC8E0, names[i], EFF_RESOURCE_SOURCE_TRANSIENT, (u32 *)&task->slots[i]);
        }
        break;
    }
    return task;
}

extern void mnuReleaseEffectResource(MenuResourceWork *);
extern s32 mnuAdvanceTitleStateUnderSemaphore(void);

void fileConfigTaskDestroy(dds3ProcessID_t *task) {
    s32 request = 1;
    FileConfigListNode *node;
    s32 i;

    if (GBWK != 0) {
        fileSavedSlotFlags = DDS3_GBWK->world.slotFlags;
        if (*(u32 *)((u32)GBWK + 4) == 1) {
            dds3AdminiCallSequence(2, &request, 4, 0);
            mnuReleaseEffectResource(((FileConfigTask *)GBWK)->effect);
            mnuAdvanceTitleStateUnderSemaphore();
        }
        node = (FileConfigListNode *)((FileConfigTask *)GBWK)->frame->first;
        for (i = 0; i < ((FileConfigTask *)GBWK)->frame->count; i++) {
            sdfChipFreeDirect(node->resource);
            node = node->next;
        }
        mnuDestroyListState(((FileConfigTask *)GBWK)->frame);
        ((FileConfigTask *)GBWK)->frame = 0;
        for (i = 0; i < 4; i++) {
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
extern s32 fileUpdateConfigurationMenu(dds3ProcessID_t *task);

extern s32 fileStartQueuedLoad(dds3ProcessID_t *task);
extern s32 fileUpdateConfigOwnerTask(dds3ProcessID_t *task);
extern void fileConfigTaskDestroy(dds3ProcessID_t *task);

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

INCLUDE_RODATA(const s32, "game/code_0028A150", fileConfigLoadTaskName);

INCLUDE_RODATA(const s32, "game/code_0028A150", fileConfigOwnerTaskName);

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
    switch (GBWK->state) {
    case FILE_CONFIG_TASK_STATE_WAITING_FOR_RESOURCES:
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
        GBWK->state = FILE_CONFIG_TASK_STATE_FADING_IN;
        /* fallthrough */
    case FILE_CONFIG_TASK_STATE_FADING_IN:
        ticks = GBWK->ticks;
        GBWK->ticks = ticks + 1;
        fade = (f32)ticks / 15.0f;
        GBWK->choiceFade = GBWK->labelFade = fade;
        if (GBWK->ticks >= 16) {
            GBWK->state = FILE_CONFIG_TASK_STATE_ACTIVE;
            GBWK->ticks = 0;
            GBWK->choiceFade = GBWK->labelFade = 1.0f;
        }
        break;
    case FILE_CONFIG_TASK_STATE_ACTIVE:
        GBWK->ticks++;
        if (GBWK->ticks >= 121) {
            GBWK->ticks = 0;
        }
        break;
    case FILE_CONFIG_TASK_STATE_BEGIN_CLOSE:
        if (GBWK->mode == 1) {
            dds3FadeOut(0, 0, 0, 15);
        }
        GBWK->ticks = 0;
        GBWK->state = FILE_CONFIG_TASK_STATE_FADING_OUT;
        /* fallthrough */
    case FILE_CONFIG_TASK_STATE_FADING_OUT:
        if (GBWK->ticks >= 6) {
            GBWK->labelFade = 0.0f;
        } else {
            GBWK->labelFade = 1.0f - (f32)GBWK->ticks / 5.0f;
        }
        ticks = GBWK->ticks;
        GBWK->ticks = ticks + 1;
        GBWK->choiceFade = 1.0f - (f32)ticks / 15.0f;
        if (GBWK->ticks >= 16) {
            GBWK->state = FILE_CONFIG_TASK_STATE_CLOSE_COMPLETE;
            GBWK->ticks = 0;
            GBWK->labelFade = 0.0f;
            GBWK->choiceFade = 0.0f;
        }
        return 0;
    case FILE_CONFIG_TASK_STATE_CLOSE_COMPLETE:
        GBWK->state |= FILE_CONFIG_TASK_EXIT_FLAG;
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
        if (dds3PadButtonState[0x24] < 0 &&
            fileTestSlotFlagsBit((FileConfigOption)index, &DDS3_GBWK->world.slotFlags) == 0) {
            fileToggleSlotFlagsBit((FileConfigOption)GBWK->frame->cursor->index,
                                   &DDS3_GBWK->world.slotFlags);
            p2sdPlaySE(8, 0x7F, 0x3F);
            cursor = (FileConfigListNode *)GBWK->frame->cursor;
            cursor->resource->ticks = 8;
        }
        if (dds3PadButtonState[0x25] < 0) {
            if (fileTestSlotFlagsBit((FileConfigOption)GBWK->frame->cursor->index,
                                     &DDS3_GBWK->world.slotFlags) != 0) {
                fileToggleSlotFlagsBit((FileConfigOption)GBWK->frame->cursor->index,
                                       &DDS3_GBWK->world.slotFlags);
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
            GBWK->state = FILE_CONFIG_TASK_STATE_BEGIN_CLOSE;
            p2sdPlaySE(8, 0x7F, 0x3F);
        }
    } else if (dds3PadButtonState[0x23] < 0) {
        GBWK->state = FILE_CONFIG_TASK_STATE_BEGIN_CLOSE;
        fileRestoreSlotFlagsToState();
        p2sdPlaySE(10, 0x7F, 0x3F);
    }
    return 0;
}


s32 fileStartQueuedLoad(dds3ProcessID_t *task) {
    if (GBWK->pending == 0) {
        return 0;
    }
    if (GBWK->state < 0) {
        return -1;
    }
    fileDrawConfigurationMenuBackground((void *)GBWK);
    mnuCallInitWide(0x400, 0x400, 0, (u32)GBWK->frame, 0x53);
    return 0;
}

s32 fileUpdateConfigOwnerTask(dds3ProcessID_t *task) {
    s32 result;

    result = -1;
    if ((GBWK->state & FILE_CONFIG_TASK_EXIT_FLAG) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_SDATA(const s32, "game/code_0028A150", fileMemoryCardRequestIndex);

INCLUDE_SDATA(const s32, "game/code_0028A150", slLoadKillFlag);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC7ED);

INCLUDE_SDATA(dds3ProcessID_t * const, "game/code_0028A150", D_003BC7F0);

INCLUDE_SDATA(struct FileRequest * const, "game/code_0028A150", fileSaveIconRequest);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC7FC);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileWaitTicksRemaining);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC804);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSlotSelectionPollCount);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC80C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC810);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC814);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC818);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC81C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC820);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC824);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSlotScanIndex);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC82C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC830);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC834);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC838);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC83C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC840);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSelectedSlotIndex);

INCLUDE_SDATA(const FileMenuStartMode, "game/code_0028A150", D_003BC848);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSelectionPending);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC850);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC854);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC858);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC85C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC860);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC864);

INCLUDE_SDATA(struct sdfTexHandle_t *, "game/code_0028A150", D_003BC868);

INCLUDE_SDATA(struct sdfTexHandle_t *, "game/code_0028A150", D_003BC86C);

INCLUDE_SDATA(struct sdfTexHandle_t *, "game/code_0028A150", D_003BC870);

INCLUDE_SDATA(struct sdfTexHandle_t *, "game/code_0028A150", D_003BC874);

INCLUDE_SDATA(struct sdfTexHandle_t *, "game/code_0028A150", D_003BC878);

INCLUDE_SDATA(struct sdfTexHandle_t *, "game/code_0028A150", D_003BC87C);

INCLUDE_SDATA(struct sdfTexHandle_t *, "game/code_0028A150", D_003BC880);

INCLUDE_SDATA(struct sdfTexHandle_t *, "game/code_0028A150", D_003BC884);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC888);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSaveHighlightPhase);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC890);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC894);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC898);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8A0);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8A8);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8B0);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8B8);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8C0);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8D0);

INCLUDE_SDATA(const s32, "game/code_0028A150", dds3ConfigState);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSlotFlagMirror);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSavedSlotFlags);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8E0);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileConfigInputTaskName);

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
    if (GBWK->state != FILE_CONFIG_TASK_STATE_FADING_IN) {
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
        effDrawSpriteWithCornerFade(0x80, (index * 35 + 129) << 3, 0,
                     (u32)((pulse * 204.79998779296875f + 102.399993896484375f) * labelFade),
                     0, GBWK->slots[1], 18, 0x53);
        effDrawSpriteWithCornerFade(0x1830, (index * 35 + 129) << 3, 0,
                     (u32)((pulse * 204.79998779296875f + 102.399993896484375f) * labelFade),
                     0, GBWK->slots[1], 19, 0x53);
        effDrawSpriteWithCornerFade(0x8B0, 0x9B0, 0, (u32)(labelFade * 256.0f),
                     0, GBWK->slots[3], labelFrames[index], 0x53);
        entryIndex = index * 2;
        effDrawSpriteWithCornerFade(fileConfigSpritePlacements[entryIndex][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[entryIndex][FILE_CONFIG_Y] << 3, 0,
                     (u32)(choiceFade * 256.0f), 0,
                     GBWK->slots[fileConfigSpritePlacements[entryIndex][FILE_CONFIG_SET]],
                     fileConfigSpritePlacements[entryIndex][FILE_CONFIG_FRAME], 0x53);
        if (GBWK->transitionTicks != 0) {
            ++entryIndex;
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[entryIndex][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[entryIndex][FILE_CONFIG_Y] << 3, 0,
                         (u32)((f32)GBWK->transitionTicks * 0.125f * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[entryIndex][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[entryIndex][FILE_CONFIG_FRAME], 0x53);
        }
    } else {
        entryIndex = index + 18;
        effDrawSpriteWithCornerFade(fileConfigSpritePlacements[entryIndex][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[entryIndex][FILE_CONFIG_Y] << 3, 0,
                     (u32)(labelFade * 256.0f), 0,
                     GBWK->slots[fileConfigSpritePlacements[entryIndex][FILE_CONFIG_SET]],
                     fileConfigSpritePlacements[entryIndex][FILE_CONFIG_FRAME], 0x53);
        entryIndex = index * 2;
        if (GBWK->previousIndex == node->index) {
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[entryIndex][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[entryIndex][FILE_CONFIG_Y] << 3, 0,
                         (u32)((f32)GBWK->transitionTicks * 0.125f * 128.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[entryIndex][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[entryIndex][FILE_CONFIG_FRAME], 0x53);
        }
    }
    if (index < 3) {
        if (fileTestSlotFlagsBit((FileConfigOption)index, &DDS3_GBWK->world.slotFlags) != 0) {
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[10][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[10][FILE_CONFIG_Y] + index * 35) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[10][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[10][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawSpriteWithCornerFade(fileConfigSpritePlacements[11][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[11][FILE_CONFIG_Y] + index * 35) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[11][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[11][FILE_CONFIG_FRAME], 0x53);
            }
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[25][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[25][FILE_CONFIG_Y] + index * 35) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[25][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[25][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawSpriteWithCornerFade(fileConfigSpritePlacements[14][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[14][FILE_CONFIG_Y] + index * 35) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[14][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[14][FILE_CONFIG_FRAME], 0x53);
            }
        } else {
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[23][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[23][FILE_CONFIG_Y] + index * 35) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[23][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[23][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawSpriteWithCornerFade(fileConfigSpritePlacements[10][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[10][FILE_CONFIG_Y] + index * 35) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[10][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[10][FILE_CONFIG_FRAME], 0x53);
            }
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[14][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[14][FILE_CONFIG_Y] + index * 35) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[14][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[14][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawSpriteWithCornerFade(fileConfigSpritePlacements[15][FILE_CONFIG_X] << 4, (fileConfigSpritePlacements[15][FILE_CONFIG_Y] + index * 35) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[15][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[15][FILE_CONFIG_FRAME], 0x53);
            }
        }
    } else if (index == 3) {
        if (fileTestSlotFlagsBit(FILE_CONFIG_OPTION_REVERSE_FIELD_CAMERA,
                                 &DDS3_GBWK->world.slotFlags) != 0) {
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[12][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[12][FILE_CONFIG_Y] << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[12][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[12][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawSpriteWithCornerFade(fileConfigSpritePlacements[13][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[13][FILE_CONFIG_Y] << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[13][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[13][FILE_CONFIG_FRAME], 0x53);
            }
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[26][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[26][FILE_CONFIG_Y] << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[26][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[26][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawSpriteWithCornerFade(fileConfigSpritePlacements[16][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[16][FILE_CONFIG_Y] << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[16][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[16][FILE_CONFIG_FRAME], 0x53);
            }
        } else {
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[24][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[24][FILE_CONFIG_Y] << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[24][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[24][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawSpriteWithCornerFade(fileConfigSpritePlacements[12][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[12][FILE_CONFIG_Y] << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[12][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[12][FILE_CONFIG_FRAME], 0x53);
            }
            effDrawSpriteWithCornerFade(fileConfigSpritePlacements[16][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[16][FILE_CONFIG_Y] << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         GBWK->slots[fileConfigSpritePlacements[16][FILE_CONFIG_SET]],
                         fileConfigSpritePlacements[16][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                effDrawSpriteWithCornerFade(fileConfigSpritePlacements[17][FILE_CONFIG_X] << 4, fileConfigSpritePlacements[17][FILE_CONFIG_Y] << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             GBWK->slots[fileConfigSpritePlacements[17][FILE_CONFIG_SET]],
                             fileConfigSpritePlacements[17][FILE_CONFIG_FRAME], 0x53);
            }
        }
    }
}


extern void mnuDrawMantraPulseAndSparks(void *resource, s32 alpha, s32 drawArg);

void fileDrawConfigurationMenuBackground(void *unused) {
    s16 sprites[5][4] = {
        {0, 2, 99, -57},
        {0, 3, -31, 365},
        {0, 5, 0, 120},
        {0, 4, 0, 120},
        {0, 6, -20, -20}
    };
    f32 labelFade = GBWK->labelFade;
    f32 choiceFade = GBWK->choiceFade;
    s32 spriteIndex = 0;
    s32 row;

    effDrawSpriteWithCornerFade(sprites[spriteIndex][FILE_CONFIG_X] * 16,
                 sprites[spriteIndex][FILE_CONFIG_Y] * 8,
                 0, (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    spriteIndex++;
    effDrawSpriteWithCornerFade(sprites[spriteIndex][FILE_CONFIG_X] * 16,
                 sprites[spriteIndex][FILE_CONFIG_Y] * 8,
                 0, (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    if (GBWK->mode == 1) {
        mnuDrawMantraPulseAndSparks((void *)GBWK->effect, 0x80, 0x52);
    }
    spriteIndex++;
    effDrawSpriteWithCornerFade(sprites[spriteIndex][FILE_CONFIG_X] * 16,
                 sprites[spriteIndex][FILE_CONFIG_Y] * 8,
                 0, (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    spriteIndex++;
    effDrawSpriteWithCornerFade(sprites[spriteIndex][FILE_CONFIG_X] * 16,
                 sprites[spriteIndex][FILE_CONFIG_Y] * 8,
                 0, (u32)(choiceFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    spriteIndex++;
    effDrawSpriteWithCornerFade(sprites[spriteIndex][FILE_CONFIG_X] * 16,
                 sprites[spriteIndex][FILE_CONFIG_Y] * 8,
                 0, (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[sprites[spriteIndex][FILE_CONFIG_SET]],
                 sprites[spriteIndex][FILE_CONFIG_FRAME], 0x53);
    for (row = 0; row < 5; row++) {
        effDrawSpriteWithCornerFade(0, (131 + row * 35) * 8, 0, (u32)(labelFade * 256.0f), 0,
                     GBWK->slots[2], 9, 0x53);
        effDrawSpriteWithCornerFade(461 * 16, (131 + row * 35) * 8, 0, (u32)(labelFade * 256.0f), 0,
                     GBWK->slots[2], 10, 0x53);
    }
    effDrawSpriteWithCornerFade(73 * 16, 326 * 8, 0, (u32)(labelFade * 256.0f), 0,
                 GBWK->slots[2], 11, 0x53);
}


void fileResetRenderFlags(void);

void fileManagerResetSubsystems(void) {
    printf(D_003BC900);
    effLoadFlashTextures();
    printf(D_003BC908);
    effLoadWindTexture();
    printf(D_003BC910);
    effLoadScalyTexture();
    printf(D_003BC918);
    fileResetRenderFlags();
    printf(D_003BC920);
}

void fileSetRenderFlag(u32 bits) {
    effModelUpdateControlFlags |= bits;
}

void fileClearRenderFlag(u32 bits) {
    effModelUpdateControlFlags &= ~bits;
}

void fileResetRenderFlags(void) {
    effModelUpdateControlFlags = 0;
}

SdfPoolNode *effGetPacketSubmissionSurface(s32 index) {
    return effPacketSubmissionSurfaces[index];
}

extern u8 sdfViewMatrix[];
extern u8 sdfProjectionMatrix[];
extern u8 D_00324660[];

/* vu0 routine: project the point in vf10 through view/projection and screen scale/bias. */
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
    SDF_VU0_LOADVECTOR_B(D_00324660);
    SDF_VU0_ADDVECTOR();
}

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
    projection = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)projection);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    projection += 16;
    SDF_VU0_LOADVECTOR_B(projection);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_00324660);
    SDF_VU0_ADDVECTOR();
    return 1;
}

extern s32 sdfProjectPointInForwardConeVU();

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
    FileJobPayload *job = sdfChipAllocDirect(sizeof(FileJobPayload));
    memset(job, 0, 0x2C);
    job->unk0 = 200;
    job->type = kind;
    return job;
}

/* allocation selects the representation of the payload location word. */
#define FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(payload) ((payload).offset)
#define FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(payload) ((payload).offset)
#define FILE_JOB_LOCATION_CLEAR(payload) ((payload).offset = 0)

void *fileResolvePrimaryBuffer(FileJobPayload *job) {
    if (job->primary.allocation != NULL) {
        return (void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary);
    }
    if (FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(job->primary) != 0) {
        return (u8 *)job + FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(job->primary);
    }
    return NULL;
}

void *fileResolveSecondaryBuffer(FileJobPayload *job) {
    if (job->secondary.allocation != NULL) {
        return (void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary);
    }
    if (FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(job->secondary) != 0) {
        return (u8 *)job + FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(job->secondary);
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
        u16 index = job->type;
        fileJobTypeOperations[index].destroy(data);
    }
    fileJobFreePrimaryBuffer(job);
    fileJobFreeSecondaryBuffer(job);
    sdfChipFreeDirect(job);
}

void fileJobFreePrimaryBuffer(FileJobPayload *job) {
    struct sdfMemHandle_t *allocation = job->primary.allocation;
    if (allocation != NULL) {
        sdfMemFreeDirect(allocation);
        FILE_JOB_LOCATION_CLEAR(job->primary);
        job->primary.size = 0;
        job->primary.allocation = NULL;
    }
}

void fileJobFreeSecondaryBuffer(FileJobPayload *job) {
    struct sdfMemHandle_t *allocation = job->secondary.allocation;
    if (allocation != NULL) {
        sdfMemFreeDirect(allocation);
        FILE_JOB_LOCATION_CLEAR(job->secondary);
        job->secondary.size = 0;
        job->secondary.allocation = NULL;
    }
}

FileJobPayload *fileJobCreateChild(FileJobPayload *request) {
    FileJobPayload *job = fileCreateJob(request->type);
    job->option = request->option;
    job->primary.selector = request->primary.selector;
    job->data = fileJobTypeOperations[job->type].createChild(request->data, job->type);
    return job;
}

void fileJobNotifyPair(FileJobPayload *left, FileJobPayload *right) {
    FileTypeCallbacks *entry = &fileJobTypeOperations[right->type];
    if (entry->notifyPair != NULL) {
        entry->notifyPair(left->data, right->data);
    }
}

void fileJobNotifyComplete(FileJobPayload *job) {
    u16 idx = job->type;
    void (*cb)(void *) = fileJobTypeOperations[idx].reset;
    if (cb != NULL) {
        cb(job->data);
    }
}

void fileJobInvokeTypeCallback(FileJobPayload *job) {
    u16 idx = job->type;
    void *data = job->data;

    fileJobTypeOperations[idx].invoke(data);
}

void fileJobInvokePositionCallback(FileJobPayload *job, const f32 position[4]) {
    u16 idx = job->type;
    void (*cb)(void *, const f32 position[4]) = fileJobTypeOperations[idx].setPosition;
    if (cb != NULL) {
        cb(job->data, position);
    }
}

void fileJobInvokeRotationCallback(FileJobPayload *job, const f32 rotation[4]) {
    u16 idx = job->type;
    void (*cb)(void *, const f32 rotation[4]) = fileJobTypeOperations[idx].setRotation;
    if (cb != NULL) {
        cb(job->data, rotation);
    }
}

void fileJobInvokeScaleCallback(FileJobPayload *job, f32 scale) {
    u16 idx = job->type;
    void (*cb)(void *, f32) = fileJobTypeOperations[idx].setScale;
    if (cb != NULL) {
        cb(job->data, scale);
    }
}

void fileDispatchJobTypeCallback(FileJobPayload *job, u32 color) {
    void (*cb)(void *, u32) = fileJobTypeOperations[job->type].setColor;
    if (cb != NULL) {
        cb(job->data, color);
    }
}

void fileJobSetPrimaryData(FileJobPayload *job, const void *src, s32 size, u16 option) {
    fileJobFreePrimaryBuffer(job);
    if (src != NULL && size > 0) {
        job->primary.allocation = sdfMemAllocDirect(size);
        FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary) = sdfMemLock(job->primary.allocation);
        job->primary.size = size;
        job->option = option;
        memcpy((void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary), src, size);
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
        FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary) = sdfMemLock(job->secondary.allocation);
        job->secondary.size = size;
        job->primary.selector = selector;
        memcpy((void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary), src, size);
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

#ifdef NON_MATCHING
#include "common.h"
#include "file.h"
#include "sdk_libkernl.h"

extern u8 fileJobZeroPadding[16];

/* Write a file job: its header with the buffer pointers cleared and the
 * payload offsets resolved, the primary buffer, zero padding to sixteen
 * bytes, then the secondary buffer. Upstream: INCLUDE_ASM in DDS1's
 * game/code_0028A150 and DDS2's game/code_002C96D0. */
void fileJobWriteSerializedPayload(s32 fd, FileJobPayload *job) {
    FileJobPayload header;
    void *primary;
    void *secondary;
    s32 payloadStart;
    s32 alignedStart;
    s32 primaryOffset;
    s32 padding = 0;

    header = *job;
    header.data = NULL;
    primary = fileResolvePrimaryBuffer(job);
    primaryOffset = header.primary.offset;
    if (primary != NULL) {
        primaryOffset = sizeof(FileJobPayload);
    }
    header.primary.offset = primaryOffset;
    header.primary.allocation = NULL;
    secondary = fileResolveSecondaryBuffer(job);
    if (secondary != NULL) {
        if (primary != NULL) {
            payloadStart = header.primary.offset + header.primary.size;
        } else {
            payloadStart = sizeof(FileJobPayload);
        }
        padding = payloadStart >> 4;
        if ((payloadStart & 0xF) != 0) {
            alignedStart = (padding + 1) << 4;
        } else {
            alignedStart = padding << 4;
        }
        header.secondary.offset = alignedStart;
        padding = alignedStart - payloadStart;
    }
    header.secondary.allocation = NULL;

    sceWrite(fd, &header, sizeof(FileJobPayload));
    if (primary != NULL) {
        sceWrite(fd, primary, job->primary.size);
    }
    if (padding > 0) {
        sceWrite(fd, fileJobZeroPadding, padding);
    }
    if (secondary != NULL) {
        sceWrite(fd, secondary, job->secondary.size);
    }
}
#else
INCLUDE_ASM(const s32, "game/code_0028A150", fileJobWriteSerializedPayload);
#endif

extern u8 sdfHddValid;
extern char D_003BC928[];
extern char D_003BC930[];
extern char D_003BC938[];
extern char *sdfFileGetHostRoot(void);

void fileWriteToPfs(FileJobPayload *job, const char *filePath) {
    char path[0xD0];
    s32 fd;

    if (sdfHddValid != 0) {
        sprintf(path, D_003BC928, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, D_003BC930, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    fileJobWriteSerializedPayload(fd, job);
    sceClose(fd);
    sceSync(D_003BC938, 0);
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
    FileQueue *queue = sdfChipAllocDirect(sizeof(FileQueue));
    memset(queue, 0, 0x90);
    queue->count = 0;
    queue->updateFrame = 0;
    fileQueueInitTransform(queue);
    return queue;
}

FileJob *fileJobCreate(void) {
    FileJob *job = sdfChipAllocDirect(sizeof(FileJob));
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
                        FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(request->secondary) =
                            (u32)fileResolveSecondaryBuffer(secondary) - (u32)request;
                        job->id = (u32)fileJobCreateFromJob(request);
                        FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(request->secondary) = request->secondary.size = 0;
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

void camAimRotation(CamAim *obj, void *dst);

/* Per-frame update: refreshes the queue rotation when an aim flag (0x60) is set, then repositions and re-notifies every job whose start time (job+0x80) has been reached. */
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
        camAimRotation(queue, aimQuat);
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
            camAimRotation(job, aimQuat);
            fileJobInvokeRotationCallback((FileJobPayload *)job->id, aimQuat);
        }
        fileJobInvokeTypeCallback((FileJobPayload *)job->id);
    }
    queue->updateFrame++;
}

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
    FileJob *job;

    for (job = queue->first; job != NULL; job = job->next) {
        fileJobNotifyComplete((FileJobPayload *)job->id);
    }
}

/* Moves the queue to *vec and repositions every job: position = *vec + offset, plus each job's offset rotated by the queue quaternion (scaled when flag 0x80, y lowered by 5 for flag 0x04). */
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

/* Sets the queue rotation to *rot: stores it as queue->quat, rotates queue->axis into queue->offset, notifies each job with its quat multiplied by *rot, then repositions the jobs at queue->position. */
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

void fileQueueCopyRotationFromSource(FileQueue *queue, f32 matrix[4][4]) {
    f32 vec[4] __attribute__((aligned(16)));
    sdfVuMatrixToQuaternion(matrix);
    SDF_VU0_STOREVECTOR(vec);
    fileQueueSetRotation(queue, vec);
}

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

FileJob *fileAppendJobFromCommandPath(FileQueue *queue, const char *commandPath) {
    printf(D_003BC940);
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
        fileJobSetSecondaryData((FileJobPayload *)job->id,
                                (void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(source->secondary),
                                source->secondary.size,
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
        sprintf(path, D_003BC928, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, D_003BC930, sdfFileGetHostRoot(), filePath);
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
                FILE_JOB_LOCATION_CLEAR(payload.secondary);
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
            FILE_JOB_LOCATION_CLEAR(payload.secondary);
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
    sceSync(D_003BC938, 0);
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
        sprintf(path, D_003BC928, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, D_003BC930, sdfFileGetHostRoot(), filePath);
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
                FILE_JOB_LOCATION_CLEAR(payload.secondary);
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
            FILE_JOB_LOCATION_CLEAR(payload.secondary);
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
    sceSync(D_003BC938, 0);
}

#undef FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET
#undef FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD
#undef FILE_JOB_LOCATION_CLEAR

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
    s32 count = 0;
    for (job = queue->first; job != NULL; job = job->next) {
        count++;
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

LoadObj *fileLoadObjectCreate(void *owner) {
    LoadObj *obj = sdfAllocAndClearQuadwords(sizeof(LoadObj));
    obj->owner = owner;
    obj->color = 0x80808080;
    obj->scale = 1.0f;
    obj->recordWork = NULL;
    obj->deviceHandle = NULL;
    obj->jobs = 0;
    obj->jobAllocation = 0;
    obj->unk48 = 1;
    return obj;
}

/* Creates a load object sized by the record's cell count, capped at 0x12C. */
typedef struct FileCellGrid {
    u8 pad00[0x20];
    u32 cols;
    u32 rows;
    u8 pad28[0x90];
    u32 layers;
} FileCellGrid;

LoadObj *fileCreateGridLoaderRecord(FileCellGrid *grid) {
    u32 cols = grid->cols;
    u32 count = (cols != 0 ? cols : grid->rows) * (cols != 0 ? grid->rows : grid->layers);

    return fileLoadObjectCreate((void *)(count <= 0x12C ? count : 0x12C));
}



void fileLoadObjectSetResource(LoadObj *obj, u32 type, void *data);
void fileLoadObjectOpenDevice(LoadObj *obj, void *name);
void fileLoadObjectOpenAndStartDevice(LoadObj *obj, void *name);
void fileLoadObjectOpenNamedDevice(LoadObj *obj, s32 resourceIndex);
void fileReplaceEffectSurfaceJobs(LoadObj *obj, FileJobPayload *job);
void fileReplaceReferenceHolder(LoadObj *obj, u32 resource);

LoadObj *effLoadObjectCreateFromJob(FileJobPayload *job) {
    void *primary = fileResolvePrimaryBuffer(job);
    LoadObj *obj = fileCreateGridLoaderRecord(primary);
    void *secondary;

    fileLoadObjectSetResource(obj, job->option, primary);
    secondary = fileResolveSecondaryBuffer(job);
    if (secondary != NULL) {
        switch (job->primary.selector) {
        case 1:
            fileLoadObjectOpenDevice(obj, secondary);
            break;
        case 2:
            fileLoadObjectOpenAndStartDevice(obj, secondary);
            break;
        case 4:
            fileLoadObjectOpenNamedDevice(obj, *(s32 *)secondary);
            break;
        case 5:
            fileReplaceEffectSurfaceJobs(obj, secondary);
            break;
        case 7:
            fileReplaceReferenceHolder((s32)obj, (u32)secondary);
            break;
        }
        obj->selector = job->primary.selector;
    }
    return obj;
}

void effLoadObjectDestroy(LoadObj *obj) {
    if (obj->deviceHandle != NULL) {
        dds3BillboardDelete(obj->deviceHandle);
    }
    if (obj->jobAllocation != 0) {
        u32 count = obj->recordWork->count;
        u32 i;
        for (i = 0; i < count; i++) {
            fileJobDestroy(obj->jobs[i]);
        }
        sdfMemFreeDirect(obj->jobAllocation);
    }
    if (obj->referenceHolder != NULL) {
        effReleaseReferenceHolder(obj->referenceHolder);
    }
    if (obj->recordWork != NULL) {
        effPCPHelperBEDGetModel(obj->recordWork);
    }
    sdfChipFreeDirect(obj);
}

LoadObj *fileLoadObjectCreateChild(LoadObj *owner) {
    LoadObj *source = (LoadObj *)owner->recordWork->data1;
    LoadObj *result = fileCreateGridLoaderRecord(source);
    fileLoadObjectSetResource(result, owner->recordWork->type, source);
    fileCloneEffectSurfaceResources(result, owner);
    return result;
}

void fileCloneEffectSurfaceResources(LoadObj *dst, LoadObj *src) {
    u32 selector = src->selector;

    switch (selector) {
    case 1:
    case 2:
    case 4:
        if (dst->deviceHandle != NULL) {
            dds3BillboardDelete(dst->deviceHandle);
        }
        dst->deviceHandle = dds3BillboardCopy((struct billHandle_t *)src->deviceHandle);
        dds3BillboardSetAnimColorAdvMode((struct billHandle_t *)(dst->deviceHandle));
        if (dst->recordWork != NULL) {
            FileKeyBlock *record = (FileKeyBlock *)dst->recordWork->data0;
            dds3BillboardSetMode(dst->deviceHandle, (s16)record->alphaTrack.surfaceIndex);
        }
        break;
    case 5: {
        u32 count = src->recordWork->count;
        s32 size;
        u32 i;

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
    }
    case 7:
        if (dst->referenceHolder != NULL) {
            effReleaseReferenceHolder(dst->referenceHolder);
        }
        dst->referenceHolder = effReferenceObjectRetain(src->referenceHolder);
        break;
    }
    dst->selector = src->selector;
}

void fileLoadObjectSetResource(LoadObj *obj, u32 type, void *data) {
    if (obj->recordWork != NULL) {
        effPCPHelperBEDGetModel(obj->recordWork);
    }
    obj->recordWork = fileAllocateGridRecordSlots(type, (u32)obj->owner, data);
}

void fileLoadObjectOpenNamedDevice(LoadObj *obj, s32 resourceIndex) {
    struct billHandle_t *handle;
    if (obj->deviceHandle != NULL) {
        dds3BillboardDelete(obj->deviceHandle);
    }
    handle = effCreateBillboardSharingIndexedResource(resourceIndex);
    obj->deviceHandle = handle;
    if (obj->recordWork != NULL) {
        FileKeyBlock *record = (FileKeyBlock *)obj->recordWork->data0;
        dds3BillboardSetMode(handle, (s16)record->alphaTrack.surfaceIndex);
    }
}

void fileLoadObjectOpenDevice(LoadObj *obj, void *name) {
    void *handle;
    if (obj->deviceHandle != NULL) {
        dds3BillboardDelete(obj->deviceHandle);
    }
    handle = dds3BillboardCreate(0, (u32)name);
    obj->deviceHandle = handle;
    if (obj->recordWork != NULL) {
        FileKeyBlock *record = (FileKeyBlock *)obj->recordWork->data0;
        dds3BillboardSetMode(handle, (s16)record->alphaTrack.surfaceIndex);
    }
}

void fileLoadObjectOpenAndStartDevice(LoadObj *obj, void *name) {
    if (obj->deviceHandle != NULL) {
        dds3BillboardDelete(obj->deviceHandle);
    }
    obj->deviceHandle = dds3BillboardCreate(1, (u32)name);
    dds3BillboardSetAnimColorAdvMode((struct billHandle_t *)(obj->deviceHandle));
    if (obj->recordWork != NULL) {
        FileKeyBlock *record = (FileKeyBlock *)obj->recordWork->data0;
        dds3BillboardSetMode(obj->deviceHandle, (s16)record->alphaTrack.surfaceIndex);
    }
}

void fileReplaceEffectSurfaceJobs(LoadObj *obj, FileJobPayload *job) {
    u32 count = obj->recordWork->count;
    u32 i;
    s32 size;

    if (obj->jobAllocation != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(obj->jobs[i]);
        }
        sdfMemFreeDirect(obj->jobAllocation);
        obj->jobs = 0;
        obj->jobAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        obj->jobAllocation = sdfMemAllocDirect(size);
        obj->jobs = (FileJobPayload **)sdfMemLock(obj->jobAllocation);
        obj->jobs[0] = fileJobCreateFromJob(job);
        for (i = 1; i < count; i++) {
            obj->jobs[i] = fileJobCreateChild(obj->jobs[0]);
        }
    }
}

void fileReplaceReferenceHolder(LoadObj *obj, u32 resource) {
    struct EffExpandedList *holder;

    if (obj->referenceHolder != NULL) {
        effReleaseReferenceHolder(obj->referenceHolder);
    }
    holder = effCreateTextureAnimationSet(resource);
    obj->referenceHolder = holder;
}

void fileClearLoadObjectReferences(LoadObj *obj) {
    if (obj->recordWork != NULL) {
        fileClearRecordReferences(obj->recordWork);
        return;
    }
}

void fileAcquireLoadObjectRecord(LoadObj *obj) {
    if (obj->recordWork != NULL) {
        fileAcquireRecord(obj->recordWork);
        return;
    }
}

#include "eff_anim.h"

extern void dds3BillboardDraw(struct billHandle_t *);
extern void fileReadRecordSecondVector(FileSlotTable *, u128 *);
extern void fileReadVectorPtr20(FileSlotTable *, u128 *);
extern void effSubmitGeneratedTexturePacket(SdfPoolNode *, EffGeneratedTextureDescriptor *);

void fileDrawLoadObjectSlots(s32 object) {
    LoadObj *obj = (LoadObj *)object;
    EffGeneratedTextureDescriptor params;
    EffAnimSample sample;
    f32 transformed[4];
    f32 origin[4];
    f32 orientation[4];
    f32 matrix[16];
    f32 modulation[4];
    FileSlotTable *record;
    FileSlot *slot;
    struct billHandle_t *billboard;
    FileJobPayload **jobs;
    EffExpandedList *animation;
    SdfPoolNode *textureSink;
    s32 previousSegment;
    s32 count;
    s32 i;
    f32 scale;
    u32 packedColor;

    eeUnpackRGBAWordFloat(obj->color, 0.0078125f);
    SDF_VU0_STOREVECTOR(modulation);
    record = obj->recordWork;
    count = record->count;
    if (count != 0) {
        slot = record->slots;
        switch (obj->selector) {
        case 1:
        case 2:
        case 4:
            billboard = obj->deviceHandle;
            if (!(record->flags & 1)) {
                for (i = 0; i < count; i++, slot++) {
                    if (FILE_SLOT_FRAME(slot) >= 0) {

                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        packedColor = eePackRGBAWord(128.0f);
                        scale = slot->scale;
                        dds3BillboardSetRGBA(billboard, packedColor);
                        dds3BillboardSetScale(billboard, scale, scale);
                        dds3BillboardSetRotate(billboard, slot->angle);
                        effCopyVector(billboard->position, slot->pos);
                        dds3BillboardSetAnimKey(billboard, FILE_SLOT_FRAME(slot));
                        dds3BillboardDraw(billboard);
                    }
                }
            } else {
                fileReadRecordSecondVector(obj->recordWork, (u128 *)orientation);
                fileReadVectorPtr20(obj->recordWork, (u128 *)origin);
                SDF_VU0_LOADVECTOR(orientation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_LOAD_ROW3(origin);
                VU0_STORE_MATRIX(matrix);
                for (i = 0; i < count; i++, slot++) {
                    if (FILE_SLOT_FRAME(slot) >= 0) {

                        VU0_LOAD_MATRIX(matrix);
                        SDF_VU0_LOADVECTOR(slot->pos);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_STOREVECTOR(transformed);
                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        packedColor = eePackRGBAWord(128.0f);
                        scale = slot->scale;
                        dds3BillboardSetRGBA(billboard, packedColor);
                        dds3BillboardSetScale(billboard, scale, scale);
                        dds3BillboardSetRotate(billboard, slot->angle);
                        effCopyVector(billboard->position, transformed);
                        dds3BillboardSetAnimKey(billboard, FILE_SLOT_FRAME(slot));
                        dds3BillboardDraw(billboard);
                    }
                }
            }
            break;
        case 5:
            jobs = obj->jobs;
            if (!(record->flags & 1)) {
                for (i = 0; i < count; i++, slot++, jobs++) {
                    if (FILE_SLOT_FRAME(slot) == 0) {
                        fileJobNotifyComplete(*jobs);
                    }
                    if (FILE_SLOT_FRAME(slot) >= 0) {

                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        packedColor = eePackRGBAWord(128.0f);
                        fileDispatchJobTypeCallback(*jobs, packedColor);
                        fileJobInvokeScaleCallback(*jobs, slot->scale);
                        fileJobInvokePositionCallback(*jobs, slot->pos);
                        fileJobInvokeTypeCallback(*jobs);
                    }
                }
            } else {
                fileReadRecordSecondVector(obj->recordWork, (u128 *)orientation);
                fileReadVectorPtr20(obj->recordWork, (u128 *)origin);
                SDF_VU0_LOADVECTOR(orientation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_LOAD_ROW3(origin);
                VU0_STORE_MATRIX(matrix);
                for (i = 0; i < count; i++, slot++, jobs++) {
                    if (FILE_SLOT_FRAME(slot) == 0) {
                        fileJobNotifyComplete(*jobs);
                    }
                    if (FILE_SLOT_FRAME(slot) >= 0) {

                        VU0_LOAD_MATRIX(matrix);
                        SDF_VU0_LOADVECTOR(slot->pos);
                        SDF_VU0_APPLYMATRIX_XYZ1();
                        SDF_VU0_STOREVECTOR(transformed);
                        eeUnpackRGBAWordFloat(slot->color, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        packedColor = eePackRGBAWord(128.0f);
                        fileDispatchJobTypeCallback(*jobs, packedColor);
                        fileJobInvokeScaleCallback(*jobs, slot->scale);
                        fileJobInvokePositionCallback(*jobs, transformed);
                        fileJobInvokeTypeCallback(*jobs);
                    }
                }
            }
            break;
        case 7: {
            u32 directInput[4];
            u32 directOutput[4];
            u32 transformedInput[4];
            u32 transformedOutput[4];
            s32 directFrame;
            s32 transformedFrame;
            animation = obj->referenceHolder;
            previousSegment = -1;
            textureSink = effGetPacketSubmissionSurface(((FileKeyBlock *)record->data0)->alphaTrack.surfaceIndex);
            if (!(((FileSlotTable *)obj->recordWork)->flags & 1)) {
                for (i = 0; i < count; i++, slot++) {
                    if (FILE_SLOT_FRAME(slot) >= 0) {

                        directInput[0] = slot->color;
                        EE_MMI_RGBA_UNPACK(directInput, 0.0078125f);
                        SDF_VU0_LOADVECTOR_B(modulation);
                        SDF_VU0_MULVECTOR();
                        {
                            u32 *__restrict packedOutput = directOutput;
                            const s32 *__restrict frameInput;

                            EE_MMI_RGBA_PACK_UNIT(packedOutput[0], 128.0f);
                            packedColor = packedOutput[0];
                            params.color = packedColor;
                            frameInput = &FILE_SLOT_FRAME(slot);
                            directFrame = *frameInput;
                        }
                        effSampleAnimSet(animation, directFrame, &sample);
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
                fileReadRecordSecondVector(obj->recordWork, (u128 *)orientation);
                fileReadVectorPtr20(obj->recordWork, (u128 *)origin);
                SDF_VU0_LOADVECTOR(orientation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_LOAD_ROW3(origin);
                VU0_STORE_MATRIX(matrix);
                for (i = 0; i < count; i++, slot++) {
                    if (FILE_SLOT_FRAME(slot) >= 0) {

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
                            packedColor = packedOutput[0];
                            params.color = packedColor;
                            frameInput = &FILE_SLOT_FRAME(slot);
                            transformedFrame = *frameInput;
                        }
                        effSampleAnimSet(animation, transformedFrame, &sample);
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


void fileUpdateLoadObjectSlots(s32 object) {
    fileAcquireLoadObjectRecord(object);
    fileDrawLoadObjectSlots(object);
}

void fileSendLoadObjectRecordVector(LoadObj *obj, u128 *vector) {
    mnuRecordSetVector(obj->recordWork, vector);
}

void fileCopyLoadObjectRecordVector(LoadObj *obj, u128 *vector) {
    fileSetRecordSecondVector(obj->recordWork, vector);
}

void fileSetRecordWordFour(s32 record, u32 value) {
    *(u32 *)(record + 4) = value;
}

void fileSetLoadObjectScale(LoadObj *obj, f32 scale) {
    obj->scale = scale;
    dds3DispatchIndexedCallback(obj->recordWork, scale);
}

void fileResetSlotStates(FileSlotTable *record) {
    u32 count = record->count;
    u32 index = 0;
    FileSlot *slot = record->slots;
    if (count != 0) {
        do {
            index++;
            FILE_SLOT_FRAME(slot) = FILE_SLOT_FRAME_NONE;
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



/* vu0 routine: samples the colour, scale and heading tracks at frame; in mode 2 the heading is the screen-space direction from out->pos to target */
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
    }
    VU0_MOVE_VF(vf28, vf20);
    VU0_MOVE_VF(vf29, vf21);
    VU0_MOVE_VF(vf30, vf22);
    VU0_MOVE_VF(vf31, vf23);
}



/* Invalidates the cells x rows slots of the group that starts at `slot`. */
void fileInvalidateSlotGroup(FileSlotTable *group, FileSlot *slot) {
    FileKeyBlock *dims = (FileKeyBlock *)group->data0;
    s32 rows = dims->rows;
    s32 columns = dims->columns;
    s32 count = columns * rows;
    FileSlot *p;
    s32 i;

    if (count != 0) {
        u8 *base = (u8 *)group->slots;
        p = (FileSlot *)(base + ((group->instances + ((u32)((u8 *)slot - base) >> 5) * count) << 5));
        for (i = 0; i < count; i++) {
            FILE_SLOT_FRAME(p) = FILE_SLOT_FRAME_NONE;
            p++;
        }
    }
}

/* Same, but first copies the slot at `slot` over the group's first slot. */
void fileCopyAndInvalidateSlotGroup(FileSlotTable *group, FileSlot *slot) {
    FileKeyBlock *dims = (FileKeyBlock *)group->data0;
    s32 rows = dims->rows;
    s32 columns = dims->columns;
    s32 count = columns * rows;
    FileSlot *p;

    if (count != 0) {
        u8 *base = (u8 *)group->slots;
        p = (FileSlot *)(base + ((group->instances + ((u32)((u8 *)slot - base) >> 5) * count) << 5));
        *p = *slot;
        FILE_SLOT_FRAME(p) = FILE_SLOT_FRAME_NONE;
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
        frame = FILE_SLOT_FRAME(slot);
        group = (FileSlot *)(base +
            ((record->instances + ((u32)((u8 *)slot - base) >> 5) * count) << 5));
        if (frame == 0) {
            *group = *slot;
            cursor = group;
            for (historyIndex = 0; historyIndex < count; historyIndex++) {
                FILE_SLOT_FRAME(cursor) = FILE_SLOT_FRAME_NONE;
                cursor++;
            }
        } else {
            FILE_SLOT_FRAME(group) = (s32)((u32)frame - 1u);
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
            FILE_SLOT_FRAME(group) = FILE_SLOT_FRAME_NONE;
            if (rows != 1 && columns >= 2) {
                if (frame >= 2 && columns >= 3) {
                    if (FILE_SLOT_FRAME(&group[rows * 2]) >= 0) {
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
                            FILE_SLOT_FRAME(cursor) = frame;
                            cursor--;
                        }
                    }
                } else if (frame > 0) {
                    if (FILE_SLOT_FRAME(&group[rows]) >= 0) {
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
                                    *(s32 *)(slotBytes + (u32)&FILE_SLOT_FRAME((FileSlot *)0)) = frame;
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
                if (FILE_SLOT_FRAME(slot) >= length) {
                    FILE_SLOT_FRAME(slot) = duration != 0 ? FILE_SLOT_FRAME_DISABLED : FILE_SLOT_FRAME_NONE;
                    fileInvalidateSlotGroup((FileSlotTable *)record, slot);
                }
                frame = FILE_SLOT_FRAME(slot);
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
                            FILE_SLOT_FRAME(slot) = 0;
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
                                FILE_SLOT_FRAME(slot) = (s32)age;
                                slot->pos[0] += motion->swayDirection[0] * sway;
                                slot->pos[1] += motion->swayDirection[1] * sway;
                                slot->pos[2] += motion->swayDirection[2] * sway;
                                motion->previousSine = sine;
                                motion->phase += motion->phaseStep;
                                motion->amplitude += motion->amplitudeStep;
                            }
                            fileSampleKeyTracks(slot, keys, FILE_SLOT_FRAME(slot), previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup((FileSlotTable *)record, slot);
                                FILE_SLOT_FRAME(slot)++;
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
                        FILE_SLOT_FRAME(slot) = frame + 1;
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
                if (FILE_SLOT_FRAME(slot) >= length) {
                    FILE_SLOT_FRAME(slot) = duration != 0 ? FILE_SLOT_FRAME_DISABLED : FILE_SLOT_FRAME_NONE;
                    fileInvalidateSlotGroup((FileSlotTable *)record, slot);
                }
                frame = FILE_SLOT_FRAME(slot);
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
                            FILE_SLOT_FRAME(slot) = 0;
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
                                FILE_SLOT_FRAME(slot) = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, FILE_SLOT_FRAME(slot), previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup((FileSlotTable *)record, slot);
                                FILE_SLOT_FRAME(slot)++;
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
                        FILE_SLOT_FRAME(slot) = frame + 1;
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
                if (FILE_SLOT_FRAME(slot) >= length) {
                    FILE_SLOT_FRAME(slot) = duration != 0 ? FILE_SLOT_FRAME_DISABLED : FILE_SLOT_FRAME_NONE;
                    fileInvalidateSlotGroup((FileSlotTable *)record, slot);
                }
                frame = FILE_SLOT_FRAME(slot);
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
                            FILE_SLOT_FRAME(slot) = 0;
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
                                FILE_SLOT_FRAME(slot) = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, FILE_SLOT_FRAME(slot), previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup((FileSlotTable *)record, slot);
                                FILE_SLOT_FRAME(slot)++;
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
                        FILE_SLOT_FRAME(slot) = frame + 1;
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
                if (FILE_SLOT_FRAME(slot) >= length) {
                    FILE_SLOT_FRAME(slot) = duration != 0 ? FILE_SLOT_FRAME_DISABLED : FILE_SLOT_FRAME_NONE;
                    fileInvalidateSlotGroup((FileSlotTable *)record, slot);
                }
                frame = FILE_SLOT_FRAME(slot);
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
                            FILE_SLOT_FRAME(slot) = 0;
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
                                FILE_SLOT_FRAME(slot) = (s32)age;
                                slot->pos[1] += motion->height;
                            }
                            fileSampleKeyTracks(slot, keys, FILE_SLOT_FRAME(slot), previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup((FileSlotTable *)record, slot);
                                FILE_SLOT_FRAME(slot)++;
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
                        FILE_SLOT_FRAME(slot) = frame + 1;
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
                if (FILE_SLOT_FRAME(slot) >= length) {
                    FILE_SLOT_FRAME(slot) = duration != 0 ? FILE_SLOT_FRAME_DISABLED : FILE_SLOT_FRAME_NONE;
                    fileInvalidateSlotGroup((FileSlotTable *)record, slot);
                }
                frame = FILE_SLOT_FRAME(slot);
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
                            FILE_SLOT_FRAME(slot) = 0;
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
                                FILE_SLOT_FRAME(slot) = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, FILE_SLOT_FRAME(slot), previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup((FileSlotTable *)record, slot);
                                FILE_SLOT_FRAME(slot)++;
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
                        FILE_SLOT_FRAME(slot) = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}



/* Scale selected directed-emitter parameters from source to destination. */
void effScaleDirectedEmitterParameters(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkD8 = src->unkD8 * scale;
    dst->unkE0 = src->unkE0 * scale;
    dst->unkE4 = src->unkE4 * scale;
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
    VU0_STORE_VF($vf0, vec);
    VU0_STORE_VF($vf0, vec + 0x10);
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

void fileReadVectorPtr20(FileSlotTable *record, u128 *out) {
    SDF_VU0_COPYVECTOR(out, record->data0);
}

void mnuRecordSetVector(FileSlotTable *record, const void *src) {
    SDF_VU0_COPYVECTOR(record->data0, src);
}

void fileReadRecordSecondVector(FileSlotTable *record, u128 *out) {
    SDF_VU0_COPYVECTOR(out, (u128 *)record->data0 + 1);
}

void fileSetRecordSecondVector(FileSlotTable *record, const void *src) {
    SDF_VU0_COPYVECTOR((u128 *)record->data0 + 1, src);
}

INCLUDE_SDATA(const s32, "game/code_0028A150", effModelUpdateControlFlags);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC900);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC908);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC910);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC918);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC920);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC928);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC930);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC938);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC940);
