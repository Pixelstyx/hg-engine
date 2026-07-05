#include "../include/types.h"
#include "../include/config.h"
#include "../include/debug.h"
#include "../include/sound.h"
#include "../include/pokemon.h"

#include "../include/constants/sndseq.h"

BOOL LONG_CALL GF_Snd_LoadSeq(int seqNo) {
    BOOL ret;
    struct SND_WORK *work;

    work = GetSoundDataPointer();
    ret = NNS_SndArcLoadSeq(seqNo, work->heap);
    GF_SndHeapGetFreeSize();

#ifdef DEBUG_SOUND_SSEQ_LOADS
    if (!ret)
    {
        u8 buf[200];
        sprintf(buf, "[GF_Snd_LoadSeq] Failed to load song %d.  There are 0x%x bytes left in the sound heap.\n", seqNo, SoundHeapFreeSize);
        debugsyscall(buf);
    }
    else
    {
        u8 buf[200];
        sprintf(buf, "[GF_Snd_LoadSeq] Loaded song %d.  There are 0x%x bytes left in the sound heap.\n", seqNo, SoundHeapFreeSize);
        debugsyscall(buf);
    }
#endif // DEBUG_SOUND_SSEQ_LOADS

    return ret;
}


BOOL GF_Snd_LoadSeqEx(int seqNo, u32 loadFlag) {
    BOOL ret;
    struct SND_WORK *work;

    work = GetSoundDataPointer();
    ret = NNS_SndArcLoadSeqEx(seqNo, loadFlag, work->heap);
    GF_SndHeapGetFreeSize();

#ifdef DEBUG_SOUND_SSEQ_LOADS
    if (!ret)
    {
        u8 buf[200];
        sprintf(buf, "[GF_Snd_LoadSeqEx] Failed to load song %d.  There are 0x%x bytes left in the sound heap.\n", seqNo, SoundHeapFreeSize);
        debugsyscall(buf);
    }
    else
    {
        u8 buf[200];
        sprintf(buf, "[GF_Snd_LoadSeqEx] Loaded song %d.  There are 0x%x bytes left in the sound heap (EX).\n", seqNo, SoundHeapFreeSize);
        debugsyscall(buf);
    }
#endif // DEBUG_SOUND_SSEQ_LOADS

    return ret;
}


#ifdef DEBUG_SOUND_SBNK_LOADS

const u8 *NNS_SND_ARC_LOAD_ERROR_STRINGS[] =
{
    "NNS_SND_ARC_LOAD_SUCCESS",
    "NNS_SND_ARC_LOAD_ERROR_INVALID_GROUP_NO",
    "NNS_SND_ARC_LOAD_ERROR_INVALID_SEQ_NO",
    "NNS_SND_ARC_LOAD_ERROR_INVALID_SEQARC_NO",
    "NNS_SND_ARC_LOAD_ERROR_INVALID_BANK_NO",
    "NNS_SND_ARC_LOAD_ERROR_INVALID_WAVEARC_NO",
    "NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_SEQ",
    "NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_SEQARC",
    "NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_BANK",
    "NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_WAVE"
};

#endif // DEBUG_SOUND_SBNK_LOADS


int LONG_CALL NNSi_SndArcLoadBank(int bankNo, u32 loadFlag, void *heap, BOOL bSetAddr, struct SNDBankData** pData)
{
    const NNSSndArcBankInfo* bankInfo;
    const NNSSndArcWaveArcInfo* waveArcInfo;
    SNDBankData* bank = NULL;
    SNDWaveArc* waveArc = NULL;
    int result;
    int i;
    BOOL loadingNewCry = 0, hasLoadedCry = 0;

    // Get bank information
    if (bankNo >= CRY_PSEUDOBANK_START || (bankNo < 495 && bankNo > 1)) // assume all cry banks are loading cries
    {
        bankInfo = NNS_SndArcGetBankInfo(1);
        loadingNewCry = 1;
#ifdef DEBUG_SOUND_SBNK_LOADS
        u8 buf[200];
        sprintf(buf, "[NNSi_SndArcLoadBank] Cry load detected for bank %d (Index %d).\n", bankNo, (bankNo >= CRY_PSEUDOBANK_START) ? (bankNo - (CRY_PSEUDOBANK_START - 544)) : bankNo);
        debugsyscall(buf);
#endif // DEBUG_SOUND_SBNK_LOADS
    }
    else
    {
        bankInfo = NNS_SndArcGetBankInfo( bankNo );
    }

#ifdef DEBUG_SOUND_SBNK_LOADS
    if (bankInfo == NULL)
    {
        u8 buf[200];
        GF_SndHeapGetFreeSize();
        sprintf(buf, "[NNSi_SndArcLoadBank] Failed to load bank %d.  There are 0x%x bytes left in the sound heap.\n", bankNo, SoundHeapFreeSize);
        debugsyscall(buf);
    }
#endif // DEBUG_SOUND_SBNK_LOADS

    if ( bankInfo == NULL ) return NNS_SND_ARC_LOAD_ERROR_INVALID_BANK_NO;

    // If necessary to load
    if ( loadFlag & NNS_SND_ARC_LOAD_BANK )
    {
        bank = LoadBank( bankInfo->fileId, heap, bSetAddr );
        if ( bank == NULL ) {
            return NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_BANK;
        }
    }
    else
    {
        bank = (SNDBankData*)NNS_SndArcGetFileAddress( bankInfo->fileId );
    }

    // Load waveform data
    for( i = 0; i < NNS_SND_ARC_BANK_TO_WAVEARC_NUM ; i++ )
    {
        u32 waveArcIndex = bankInfo->waveArcNo[i];
        if (loadingNewCry && !hasLoadedCry)
        {
            waveArcIndex = bankNo;
            hasLoadedCry = 1;
        }

        if ( waveArcIndex == NNS_SND_ARC_INVALID_WAVEARC_NO ) continue;

        // Get waveform archive information
        waveArcInfo = NNS_SndArcGetWaveArcInfo( waveArcIndex );

        if (waveArcInfo == NULL)
        {
#ifdef DEBUG_SOUND_SBNK_LOADS
            u8 buf[200];
            GF_SndHeapGetFreeSize();
            sprintf(buf, "[NNSi_SndArcLoadBank] Failed to load waveArc %d using NNS_SndArcGetWaveArcInfo.  There are 0x%x bytes left in the sound heap.\n", waveArcIndex, SoundHeapFreeSize);
            debugsyscall(buf);
#endif // DEBUG_SOUND_SBNK_LOADS
            
            return NNS_SND_ARC_LOAD_ERROR_INVALID_WAVEARC_NO;
        }

        // Loading waveform archives
        result = NNSi_SndArcLoadWaveArc( waveArcIndex, loadFlag, heap, bSetAddr, &waveArc );

#ifdef DEBUG_SOUND_SBNK_LOADS

        if ( result != NNS_SND_ARC_LOAD_SUCCESS )
        {
            u8 buf[200];
            GF_SndHeapGetFreeSize();
            if (loadingNewCry)
            {
                sprintf(buf, "[NNSi_SndArcLoadBank] Failure to load waveArc %d using NNSi_SndArcLoadWaveArc (%s) ignored because cry detected and debugging is on.  There are 0x%x bytes left in the sound heap.\n", waveArcIndex,  NNS_SND_ARC_LOAD_ERROR_STRINGS[result], SoundHeapFreeSize);
                debugsyscall(buf);
            }
            else
            {
                sprintf(buf, "[NNSi_SndArcLoadBank] Failed to load waveArc %d using NNSi_SndArcLoadWaveArc (%s).  There are 0x%x bytes left in the sound heap.\n", waveArcIndex,  NNS_SND_ARC_LOAD_ERROR_STRINGS[result], SoundHeapFreeSize);
                debugsyscall(buf);
                return result;
            }
        }

#else

        if ( result != NNS_SND_ARC_LOAD_SUCCESS ) return result;

#endif // DEBUG_SOUND_SBNK_LOADS

        if ( waveArcInfo->flags & NNS_SND_ARC_WAVEARC_SINGLE_LOAD )
        {
            // Individual waveform loading
            if ( loadFlag & NNS_SND_ARC_LOAD_WAVE )
            {
                if ( ! LoadSingleWaves( waveArc, bank, i, waveArcInfo->fileId, heap ) )
                {
#ifdef DEBUG_SOUND_SBNK_LOADS
                    {
                        u8 buf[200];
                        GF_SndHeapGetFreeSize();
                        sprintf(buf, "[NNSi_SndArcLoadBank] Failed to load waves for waveArc id %d using LoadSingleWaves.  There are 0x%x bytes left in the sound heap.\n", waveArcIndex, SoundHeapFreeSize);
                        debugsyscall(buf);
                    }
#endif // DEBUG_SOUND_SBNK_LOADS

                    return NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_WAVE;
                }
            }
        }

        // Associate waveforms with banks
        if ( bank != NULL && waveArc != NULL ) {
            SND_AssignWaveArc( bank, i, waveArc );

#ifdef DEBUG_SOUND_SBNK_LOADS
            {
                u8 buf[200];
                GF_SndHeapGetFreeSize();
                sprintf(buf, "[NNSi_SndArcLoadBank] Loaded waveArc id %d fully and assigned it to in-progress loaded bank %d.  There are 0x%x bytes left in the sound heap.\n", waveArcIndex, bankNo, SoundHeapFreeSize);
                debugsyscall(buf);
            }
#endif // DEBUG_SOUND_SBNK_LOADS

        }

    }

    if ( pData != NULL ) *pData = bank;

#ifdef DEBUG_SOUND_SBNK_LOADS
    {
        u8 buf[200];
        GF_SndHeapGetFreeSize();
        sprintf(buf, "[NNSi_SndArcLoadBank] Loaded bank %d.  There are 0x%x bytes left in the sound heap.\n", bankNo, SoundHeapFreeSize);
        debugsyscall(buf);
    }
#endif // DEBUG_SOUND_SBNK_LOADS

    return NNS_SND_ARC_LOAD_SUCCESS;
}

// ALL THAT SOUNDPLATE NONSENSE GOES BELOW HERE.

const u8 sBGMVolume[3] = {
    0x60,
    0x40,
    0x20
};

const u8 sSoundplateVolume[16][3] = {
    {0x40, 0x60, 0x7F},
    {0x2E, 0x60, 0x7F},
    {0x2E, 0x60, 0x7F},
    {0x40, 0x60, 0x7F},
    {0x2E, 0x40, 0x60},
    {0x40, 0x60, 0x6C},
    {0x2E, 0x60, 0x6C},
    {0x2E, 0x60, 0x7F},
    {0x2E, 0x60, 0x7F},
    {0x2E, 0x60, 0x7F},
    {0x2E, 0x60, 0x7F},
    {0x2E, 0x48, 0x6C},
    {0x2E, 0x60, 0x7F},
    {0x2E, 0x60, 0x7F},
    {0x40, 0x60, 0x7F},
    {0x2E, 0x60, 0x7F}
};

enum SoundplateSoundParams {
    SOUNDPLATE_SOUND_SEQ = 0,
    SOUNDPLATE_SOUND_UNK_BOOL,
    SOUNDPLATE_SOUND_PARAMS
};

typedef struct Soundplate {
    u8 soundplateSoundID;
    u8 volumeIndex;
    u8 unk2;
    u8 unk3;
    u8 x;
    u8 z;
    u8 xBounds;
    u8 zBounds;
} Soundplate;

typedef struct SoundplateStruct {
    u8 unk0[2];
    vu16 unk2; // fakematch?
    Soundplate soundplates[];
} SoundplateStruct;

const u16 sSoundplateSounds[16][SOUNDPLATE_SOUND_PARAMS] = {
    { SEQ_SE_GS_N_SESERAGI,     TRUE  }, // Water Flow
	{ SEQ_SE_GS_N_HUUSHA,       FALSE }, // Windmill
	{ SEQ_SE_GS_N_UMIBE,        FALSE }, // Seashore
	{ SEQ_SE_GS_N_HASHIRA,      TRUE  }, // Pillar
	{ SEQ_SE_GS_N_UZUSIO,       FALSE }, // Whirlpool
	{ SEQ_SE_GS_N_TAKI,         FALSE }, // Waterfall
	{ SEQ_SE_GS_N_YOUGAN,       TRUE  }, // Lava
	{ SEQ_SE_GS_N_KANSEI,       FALSE }, // Cheers
	{ SEQ_SE_GS_N_KITEKI,       FALSE }, // Steam Whistle
	{ SEQ_SE_GS_KABIGON_IBIKI,  TRUE  }, // Snorlax's Snoring
	{ SEQ_SE_GS_N_MOTER,        TRUE  }, // Motor
	{ SEQ_SE_GS_N_KANE,         TRUE  }, // Bells
	{ SEQ_SE_GS_KYOUHUU,        TRUE  }, // Strong Wind
	{ SEQ_SE_GS_N_ENGINE,       TRUE  }, // Engine
	{ SEQ_SE_GS_N_HUNSUI,       FALSE }, // Fountain
	{ SEQ_SE_GS_DENGEKIBARIA,   FALSE }  // Electric Barrier
};

int LONG_CALL GetLocalSoundplateID(SoundplateStruct *soundplateStruct, int globalX, int globalZ);
BOOL LONG_CALL ov01_021E7D58(FieldSystem *fieldSystem, SoundplateStruct *soundplateStruct, int soundplateID);
void LONG_CALL sub_02006088(u16 sndseq);
SoundplateStruct LONG_CALL *sub_02054874(FieldSystem *fieldSystem, int x, int z);
void LONG_CALL GF_SndHandleMoveVolume(int handle, int volume, int);
void LONG_CALL StopSE(u16 sndseq, int a1);

/*void LONG_CALL ov01_021E7DFC(FieldSystem *fieldSystem, int x, int z) {
    SoundplateStruct *soundplateStruct = sub_02054874(fieldSystem, x, z);
    
    if (fieldSystem->unkC4 == -2) {
        fieldSystem->unkC4 = -1;
    } else if (fieldSystem->unkC4 == -3) {
        fieldSystem->unkC4 = -1;
    }
    
    z = GetLocalSoundplateID(soundplateStruct, x, z);
    if (z != -1) {
        if (ov01_021E7D58(fieldSystem, soundplateStruct, z)) {
            u8 soundplateSoundID = soundplateStruct->soundplates[z].soundplateSoundID;
            if (soundplateSoundID < 16) {
                if (fieldSystem->unkC4 != sSoundplateSounds[soundplateSoundID][SOUNDPLATE_SOUND_SEQ]) {
                    if (sSoundplateSounds[soundplateSoundID][SOUNDPLATE_SOUND_UNK_BOOL] == TRUE) {
                        sub_02006088(sSoundplateSounds[soundplateSoundID][SOUNDPLATE_SOUND_SEQ]);
                    } else {
                        PlaySE(sSoundplateSounds[soundplateSoundID][SOUNDPLATE_SOUND_SEQ]);
                    }
                }
                fieldSystem->unkC4 = sSoundplateSounds[soundplateStruct->soundplates[z].soundplateSoundID][SOUNDPLATE_SOUND_SEQ];
                u8 volumeIndex = soundplateStruct->soundplates[z].volumeIndex;
                if (volumeIndex < 3) {
                    GF_SndHandleMoveVolume(0, sBGMVolume[volumeIndex], 15);
                    GF_SndHandleMoveVolume(5, sSoundplateVolume[soundplateStruct->soundplates[z].soundplateSoundID][soundplateStruct->soundplates[z].volumeIndex], 5);
                }
            }
        }
    } else {
        if (fieldSystem->unkC4 != -1) {
            StopSE(fieldSystem->unkC4, 10);
            GF_SndHandleMoveVolume(0, 128, 15);
            fieldSystem->unkC4 = -1;
        }
    }
}*/
