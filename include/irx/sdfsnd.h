#ifndef IRX_SDFSND_H
#define IRX_SDFSND_H

/* Types and data of SDFSDMAN.IRX, the sound driver. The behaviour of each
 * piece is described in docs/sdf-irx-modules.md. */

#include "iop.h"

/* Bank kinds, each with its own volume table: 13 in DDS, 7 in SMT3. */
#define SDF_SND_KINDS 13

/* modhsyn: the hardware synthesiser that plays the .SMG sequences */
typedef struct {
    u8 unk00;
    u8 unk01;
    u8 unk02;
    u8 unk03;
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    u8 rest[1348];
} sceHSynSlot_t;

/* The message queue one slot's worth of MIDI events passes through, which
 * MODMSIN appends to and the synthesiser drains. The driver hands the same
 * array to both, as sceHSynEntry_t.buf and as sceMidiVoice_t.buf, and writes
 * 1032 into size, which is the capacity of data. */
typedef struct {
    u32 size;
    u32 len;
    u8 data[1024];
} sceMSInBuf_t;

typedef struct {
    u32 unk00;
    sceMSInBuf_t *buf;
    u32 unk08;
    sceHSynSlot_t *slot;
} sceHSynEntry_t;

typedef struct {
    u32 nEntry;
    sceHSynEntry_t *entry;
} sceHSynTbl_t;

typedef struct {
    u32 unk00;
    sceHSynTbl_t *tbl;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
} sceHSynWork_t;

/* modmidi: the MIDI front end driving the synthesiser */
typedef struct {
    u32 unk00;
    u32 unk04;
    u32 location;
    u32 flags;
    u16 chanMask[16];
    u16 slotMask;
    u16 pad32;
    void *msgCb;
    s32 msgCbArg;
    void *cbB;
    s32 cbBArg;
    u32 unk44;
    u32 unk48;
    void *cbC;
    s32 cbCArg;
    u8 rest[480];
} sceMidiSlot_t;

typedef struct {
    u32 unk00;
    void *data;
    u32 unk08;
    sceMidiSlot_t *slot;
} sceMidiSeq_t;

/* MODMSIN walks the MIDI work area's table to reach these, so what the
 * sequencer calls a voice entry is that module's buffer entry. */
typedef struct {
    u32 unk00;
    sceMSInBuf_t *buf;
} sceMidiVoice_t;

typedef struct {
    u32 nSeq;
    sceMidiSeq_t *seq;
    u32 nVoice;
    sceMidiVoice_t *voice;
} sceMidiTbl_t;

typedef struct {
    u32 unk00;
    sceMidiTbl_t *tbl;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
} sceMidiWork_t;

/* sifcmd */
typedef struct {
    u32 psize_dsize;
    void *dest;
    s32 cid;
    u32 opt;
} SifCmdHeader_t;

typedef struct {
    void *(*handler)(void *, void *);
    void *harg;
} SifCmdHandlerData_t;

typedef struct {
    void *src;
    void *dest;
    s32 size;
    s32 attr;
} SifDmaTransfer_t;

/* One EE command packet; the ring buffer holds 256 of them. */
typedef struct {
    u32 psize_dsize;
    void *dest;
    s32 cid;
    u32 opt;
    u32 tag;
    u8 data[108];
} sdfSndCmd_t;

/* What the driver sends back to the EE when a command completes. */
typedef struct {
    SifCmdHeader_t hdr;
    u32 code;
    u32 arg;
} sdfSndAck_t;

/* The argument of an RPC the driver makes back to the EE. */
typedef struct {
    u32 unused;
    s32 arg;
    void *buf;
    s32 len;
} sdfSndEeReq_t;

typedef struct {
    u8 unk00;
    u8 unk01;
    u16 size;
    u32 waveSize;
    void *data;
    u32 spuAddr;
} sdfSndBank_t;

typedef struct {
    u32 size;
    void *ptr;
} sdfSndBankData_t;

typedef struct {
    s32 id;
    u16 kind;
    u16 pad;
} sdfSndBankStat_t;

typedef struct {
    s32 id;
    s32 location;
} sdfSndResume_t;

/* One playing sequence: a MIDI slot plus where the caller put it in 3d. */
typedef struct {
    s32 id;
    s32 group;
    s32 vol;
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 is3d;
    s32 radius;
    s16 volStep;
    s16 fadeIn;
    u32 serial;
    s32 location;
    u32 unk2C;
    u32 unk30;
    u32 unk34[4]; /* DDS only: the slot is 0x44 bytes, 0x34 in SMT3 */
} sdfSndSlot_t;

typedef struct {
    s32 thid;
    s32 timerId;
    s32 interval;
} sdfSndTimer_t;

/* The work area the EE hands over at boot: bank table, volume tables and the
 * error slot the EE polls. */
typedef struct {
    u32 bankCount;
    u32 unk04;
    void *xferBuf;
    u32 xferBufSize;
    sdfSndBank_t banks[8];
    u8 unk90[128];
    sdfSndBankData_t progData[7];
    u8 unk148[72];
    sdfSndBankStat_t stat[SDF_SND_KINDS];
    void *eeAddr;
    u32 size;
    u32 unk1D0;
    u32 unk1D4;
    u32 errCount;
    s32 errCode;
    u8 volTable[SDF_SND_KINDS][128];
    u8 voiceMap[64];
} sdfSndWorkArea_t;

/* .SMG, the packed bank file: sequences, wave data and program data. */
typedef struct {
    u32 unk00[5];
    u32 seqSize;
    u32 unk18;
    u32 waveSize;
    u32 unk20;
    u32 progSize;
    u32 idPacked;
    u32 kindPacked;
    u8 volTable[128];
} sdfSmgHeader_t;

/* APCM, the streamed ADPCM header. */
typedef struct {
    u8 type;
    u8 pad0[7];
    u32 magic;
    u32 unk0C;
    u32 sampleLen;
    u32 unk14;
    u32 unk18;
    u8 stereo;
    u8 freq;
    u8 reverb;
    u8 pad1;
} sdfAdpcmHeader_t;

/* Command payloads the EE sends in sdfSndCmd_t.data. */
typedef struct {
    u32 id;
    u32 location;
    u16 vol;
    u16 pad;
} sdfSndBgmReq_t;

typedef struct {
    u32 id;
    u32 unk04;
    u16 vol;
    u16 pan;
} sdfSndSeReq_t;

typedef struct {
    u32 id;
    s32 radius;
    s32 x;
    s32 y;
    s32 z;
} sdfSndSe3dReq_t;

typedef struct {
    u32 unk00;
    u32 unk04;
    s32 atX;
    s32 atY;
    s32 atZ;
    s32 posX;
    s32 posY;
    s32 posZ;
} sdfSndListenerReq_t;

/* The module's own data. Everything here lives in asm/irx/SDFSDMAN/. */
extern u32 sdfSndBankSizeTbl[SDF_SND_KINDS];
extern u32 sdfSndBankAddrTbl[SDF_SND_KINDS];
extern u32 sdfSndBankOffTbl[SDF_SND_KINDS + 1];
extern u32 sdfSndBankFlagTbl[SDF_SND_KINDS + 1];
extern s32 sdfSndBank1Loaded;
extern s32 sdfSndBank3Loaded;
extern u8 sdfSndBankAltSlot;
extern u32 sdfSndSlotSerialNext;
extern s32 sdfSndResumeIdx;
extern s32 sdfSndSeqVolume;
extern iop_sema_t sdfSndEeRpcSemaParam;
extern sceSdEffectAttr_t sdfSndEffectAttr[2];
extern s32 sdfSndAtanTbl[32];
extern sdfSndTimer_t sdfSndTimer;
extern char *sdfSndSmgPathPtr;
extern void *sdfSndBankBase;
extern s32 sdfSndLastBgmSlot;
extern s32 sdfSndLastSeSlot;
extern u32 sdfAdpcmPitchTbl[6];
extern char sdfAdpcmFreqNames[6][8];

extern s32 sdfSndEeRpcSema;
extern sdfSndWorkArea_t *sdfSndWorkArea;
extern s32 sdfSndSeqThreadId;
extern iop_sema_t sdfSndRpcSemaParam;
extern s32 sdfSndRpcSema;
extern u8 sdfSndVolTblSeq[32];
extern u8 sdfSndVolTblBgm[128];
extern u8 sdfSndVolTblSe[128];
extern u8 sdfSndVolTblVoice[32]; /* 128 in SMT3 */
extern u8 sdfSndVolTblKind4[32]; /* DDS only */
extern u8 sdfSndVolTblCore0[2];
extern u8 sdfSndVolTblCore1[2];
extern u8 sdfSndVolTblKind7[6][2]; /* DDS only: kinds 7 to 12 */
extern char sdfSndSmgPath[256];
extern sdfSndSlot_t sdfSndSlots[16];
extern sdfSndResume_t sdfSndResume[32];
extern s32 sdfSndBgmSwitchReq;
extern s32 sdfSndBgmSwitchSlot;
extern s32 sdfSndBgmSwitchMidi;
extern s32 sdfSndListenerX;
extern s32 sdfSndListenerY;
extern s32 sdfSndListenerZ;
extern s32 sdfSndListenerAngle;
extern s32 sdfSndListenerDirty;
extern sceHSynWork_t sdfSndHsynWork;
extern sceHSynTbl_t sdfSndHsynTbl;
extern sceHSynEntry_t sdfSndHsynEntry[16];
extern sceHSynSlot_t sdfSndHsynSlotWork[16];
extern sceMSInBuf_t sdfSndMsInBuf[16];
extern sceMidiWork_t sdfSndMidiWork;
extern sceMidiTbl_t sdfSndMidiTbl;
extern sceMidiSeq_t sdfSndMidiSeq[16];
extern sceMidiVoice_t sdfSndMidiVoice[16];
extern sceMidiSlot_t sdfSndMidiSlotWork[16];
extern u8 sdfSndHsynLoadParam;
extern u8 sdfSndHsynVoiceMap[48];
extern iop_sema_t sdfSndSmgSemaParam;
extern s32 sdfSndSmgSema;
extern iop_sema_t sdfSndCmdSemaParam;
extern s32 sdfSndCmdSema;
extern sdfSndAck_t sdfSndAckPacket;
extern SifCmdHandlerData_t sdfSndSifCmdBuffer[10];
extern sdfSndCmd_t sdfSndCmdRing[256];
extern u16 sdfSndCmdCount;
extern u16 sdfSndCmdHead;
extern u32 sdfSndCmdOverflow;

/* One object: the stream player's whole state, which is how the retail code
 * reaches fadeStep off the same base register as playing. */
typedef struct {
    s8 core;
    u8 coreId;
    s8 playing;
    u8 fillIdx;
    u8 unk04[2];
    s8 fillBlk;
    u8 xferIdx;
    s8 xferBlk;
    u8 intrState;
    s8 voiceL;
    s8 voiceR;
    s8 onMemory;
    s8 stereo;
    s8 fadeState;
    u8 unk0F;
    s32 fadeStep;
    s32 fadeFrac;
    s32 volume;
    s32 unk1C;
    u32 iopBuf;
    u32 spuAddr;
    u32 totalBytes;
    u32 bytesLeft;
    u32 pitch;
    u32 unk34;
} sdfAdpcm_t;

extern sdfAdpcm_t sdfAdpcm;
extern s8 sdfAdpcmCore;
extern u8 sdfAdpcmCoreId;
extern s8 sdfAdpcmPlaying;
extern u8 sdfAdpcmFillIdx;
extern s8 sdfAdpcmFillBlk;
extern u8 sdfAdpcmXferIdx;
extern s8 sdfAdpcmXferBlk;
extern u8 sdfAdpcmIntrState;
extern s8 sdfAdpcmVoiceL;
extern s8 sdfAdpcmVoiceR;
extern s8 sdfAdpcmOnMemory;
extern u8 sdfAdpcmStereo;
extern s8 sdfAdpcmFadeState;
extern s32 sdfAdpcmFadeStep;
extern s32 sdfAdpcmFadeFrac;
extern s32 sdfAdpcmVolume;
extern u32 sdfAdpcmIopBuf;
extern u32 sdfAdpcmSpuAddr;
extern u32 sdfAdpcmTotalBytes;
extern u32 sdfAdpcmBytesLeft;
extern u32 sdfAdpcmPitch;
extern sdfAdpcmHeader_t sdfAdpcmHeader;
extern iop_sema_t sdfAdpcmPlaySemaParam;
extern iop_sema_t sdfAdpcmIntrSemaParam;
extern s32 sdfAdpcmPlaySema;
extern s32 sdfAdpcmIntrSema;
extern SifRpcClientData_t sdfSndEeRpcClient;


/* modhsyn */
s32 modhsyn_sceHSyn_Init(sceHSynWork_t *work, s32 usec);
s32 modhsyn_e5(sceHSynWork_t *work);
s32 modhsyn_sceHSyn_Load(sceHSynWork_t *work, s32 slot, u32 spuAddr, void *data, s32 which);
s32 modhsyn_sceHSyn_VoiceTrans(s32 mode, void *iopaddr, u32 spuaddr, s32 size);
s32 modhsyn_e8(void *param);
s32 modhsyn_e13(sceHSynWork_t *work, s32 slot);
s32 modhsyn_e15(void *param);
s32 modhsyn_e18(s32 enable);

/* modmidi, modmsin */
s32 modmidi_sceMidi_Init(sceMidiWork_t *work, s32 usec);
s32 modmidi_e5(sceMidiWork_t *work);
s32 modmidi_sceMidi_Load(sceMidiWork_t *work, s32 slot);
s32 modmidi_sceMidi_SelectMidi(sceMidiWork_t *work, s32 slot, s32 seq);
s32 modmidi_sceMidi_MidiPlaySwitch(sceMidiWork_t *work, s32 slot, s32 on);
s32 modmidi_sceMidi_MidiSetLocation(sceMidiWork_t *work, s32 slot, s32 loc);
s32 modmidi_sceMidi_MidiSetVolume(sceMidiWork_t *work, s32 slot, s32 chanMask, s32 vol);
s32 modmsin_sceMSIn_PutMsg(sceMidiWork_t *work, s32 slot, u32 msg);

/* sdfiopfl, the file library SDFIOPFL.IRX exports */
s32 sdfiopfl_sdfIopFileReadOpen(const char *path);
s32 sdfiopfl_sdfIopFileGetFileSize(s32 fd);
s32 sdfiopfl_sdfIopFileRead(s32 fd, void *buf, s32 len);
s32 sdfiopfl_sdfIopFileClose(s32 fd);

/* sifcmd, sifman, timrman */
void sifcmd_SifInitCmd(void);
void sifcmd_SifSetCmdBuffer(SifCmdHandlerData_t *db, s32 size);
void sifcmd_SifAddCmdHandler(s32 cid, void *(*handler)(void *, void *), void *harg);
u32 sifcmd_SifSendCmd(u32 cid, void *packet, s32 psize, void *src, void *dest, s32 size);
s32 sifcmd_SifCheckStatRpc(SifRpcClientData_t *cd);
u32 sifman_sceSifSetDma(SifDmaTransfer_t *sdd, s32 count);
s32 sifman_sceSifDmaStat(u32 id);
s32 timrman_AllocHardTimer(s32 source, s32 size, s32 prescale);
s32 timrman_FreeHardTimer(s32 timerId);
s32 timrman_SetTimerHandler(s32 timerId, u32 compare, s32 (*handler)(void *), void *arg);
s32 timrman_SetupHardTimer(s32 timerId, s32 source, s32 mode, s32 prescale);
s32 timrman_StartHardTimer(s32 timerId);
s32 timrman_StopHardTimer(s32 timerId);

#endif /* IRX_SDFSND_H */
