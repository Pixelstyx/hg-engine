#include "../../include/types.h"
#include "../../include/config.h"
#include "../../include/debug.h"
#include "../../include/sound.h"
#include "../../include/pokemon.h"
#include "../../include/script.h"

#include "../../include/constants/sndseq.h"
#include "../../include/constants/weather_numbers.h"

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
    u16 unk2;
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
BOOL LONG_CALL SoundplateIsActive(FieldSystem *fieldSystem, SoundplateStruct *soundplateStruct, int soundplateID);
void LONG_CALL sub_02006088(u16 sndseq);
SoundplateStruct LONG_CALL *sub_02054874(FieldSystem *fieldSystem, int x, int z);
void LONG_CALL GF_SndHandleMoveVolume(int handle, int volume, int);
void LONG_CALL StopSE(u16 sndseq, int a1);

static BOOL WeatherMakesNoise(u32 weather) {
    return weather == WEATHER_SYS_RAIN 
    || weather == WEATHER_SYS_HEAVY_RAIN
    || weather == WEATHER_SYS_THUNDER
    || weather == WEATHER_SYS_STORM;
};

void LONG_CALL ov01_021E7DFC(FieldSystem *fieldSystem, int x, int z) {
    SoundplateStruct *soundplateStruct = sub_02054874(fieldSystem, x, z);
    
    if (fieldSystem->unkC4 == -2) {
        fieldSystem->unkC4 = -1;
    } else if (fieldSystem->unkC4 == -3) {
        fieldSystem->unkC4 = -1;
    }

    if (WeatherMakesNoise(Fsys_GetWeather_HandleDiamondDust(fieldSystem, fieldSystem->location->mapId))) return;
    
    z = GetLocalSoundplateID(soundplateStruct, x, z);
    if (z != -1) {
        if (SoundplateIsActive(fieldSystem, soundplateStruct, z)) {
            u8 soundplateSoundID = soundplateStruct->soundplates[z].soundplateSoundID;
            if (soundplateSoundID < 16) {
                if (fieldSystem->unkC4 != sSoundplateSounds[soundplateSoundID][SOUNDPLATE_SOUND_SEQ]) {
                    if (sSoundplateSounds[soundplateSoundID][SOUNDPLATE_SOUND_UNK_BOOL] == TRUE) {
                        sub_02006088(sSoundplateSounds[soundplateSoundID][SOUNDPLATE_SOUND_SEQ]);
                    } else {
                        PlaySE(sSoundplateSounds[soundplateSoundID][SOUNDPLATE_SOUND_SEQ]);
                    }
                }
                // fieldSystem->unkC4 = sSoundplateSounds[soundplateStruct->soundplates[z].soundplateSoundID][SOUNDPLATE_SOUND_SEQ];
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
            // fieldSystem->unkC4 = -1;
        }
    }
}
