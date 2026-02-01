#include "../include/types.h"
#include "../include/battle.h"
#include "../include/party_menu.h"
#include "../include/pokemon.h"
#include "../include/constants/ability.h"
#include "../include/constants/game.h"
#include "../include/constants/moves.h"

static const u16 FrontierItemReplacementList[] = { ITEM_BRIGHT_POWDER, ITEM_LUM_BERRY, ITEM_LEFTOVERS, ITEM_QUICK_CLAW };

typedef struct FrontierMon {
    u16 species;
    u16 moves[4];
    u8 evs;
    u8 nature;
    u16 item;
    u16 form;
} FrontierMon;

// Originally in include/scrcmd_9.h.
typedef struct FrontierMonStruct { // UnkStruct_0204B470
    u16 species : 11;
    u16 form : 5;
    u16 item;
    u16 moves[4];
    u32 otID;
    u32 pid;
    union {
        struct {
            u32 hpIv : 5;
            u32 atkIv : 5;
            u32 defIv : 5;
            u32 spdIv : 5;
            u32 spAtkIv : 5;
            u32 spDefIv : 5;
            u32 dummy : 2;
        };
        u32 ivsWord;
    };
    u8 evs[6];
    u8 ppUp;
    u8 language;
    u16 ability; // Expanded from u8
    u8 friendship;
    u16 nickname[11];
} FrontierMonStruct;

typedef struct FrontierTrainer { // UnkStruct_0204B7D0
    u32 frontierTrainerIndex;
    u16 trainerClass;
    u8 filler06[2];
    u16 unk08[8];
    u16 unk18;
    u16 unk1a;
    u8 filler1c[0x14];
    FrontierMonStruct frontierMonStructs[2];
    u8 fillera0[0x70];
} FrontierTrainer;

typedef struct FrontierTagBattleAlly { // UnkStruct_0204A824_7E8
    u32 otID;
    u16 frontierMonIDs[2];
    u32 frontierMonPIDs[2];
} FrontierTagBattleAlly;

// Originally in include/scrcmd_9.h.
typedef struct FrontierFieldSystem { // UnkStruct_Fsys_A0
    u32 unk00; // Seems to always be 0x12345678.
    u32 heapID;
    u32 unk08;
    u8 unk0c; // Something related to currentWinStreak...
    u8 currentWinStreak; // curWinStreak
    u8 numPokemon; // unk0e
    u8 battleMode; // unk0f
    u8 unk10_0 : 1;
    u8 unk10_1 : 2;
    u8 unk10_3 : 1;
    u8 unk10_4 : 1;
    u8 unk10_5 : 3;
    u8 trainerGender; // unk11
    u8 unk12;
    u8 filler13;
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u16 unk1a;
    u16 unk1c;
    u8 filler1e[2];
    u32 unk20; // Something related to battle tower win count
    u16 unk24;
    u16 unk26;
    u16 unk28; 
    u8 partyMonIndexes[4]; // unk2a
    u16 unk2e[4];
    u16 unk36[4];
    u16 unk3e[0xe];
    u8 filler5a[0x13];
    u32 unk70;
    void *frontierData; // FRONTIERDATA?
    FrontierTrainer unk78[2];
    FrontierTrainer frontierStatTrainers[5]; // unk298
    FrontierTagBattleAlly tagAlly[5];
    u8 unk838[5];
    u8 filler83d;
    u16 unk83e[0x23];
    u16 unk884[0x23];
    u8 filler8CA[0xa];
    u8 unk8D4;
    u8 unk8D5;
    u8 filler8D6[2];
    u16 unk8D8;
    u16 unk8DA;
} FrontierFieldSystem;

u32 LONG_CALL GenerateFrontierMon(struct FrontierFieldSystem *frontierFsys, struct FrontierMonStruct *frontierMonStruct, u16 frontierMonIndex, u32 otID, u32 pid, u8 iv, u8 replacementItemIndex, BOOL replaceItem, u32 heapID); // sub_0204B834

void LONG_CALL GetFrontierMon(FrontierMon *mon, u32 frontierMonIndex);
u16 LONG_CALL FrontierRand(FrontierFieldSystem *frontierFsys); // sub_0204B510
void LONG_CALL MI_CpuFill8(void *dest, u8 data, u32 size);

u32 LONG_CALL GenerateFrontierMon(struct FrontierFieldSystem *frontierFsys, struct FrontierMonStruct *frontierMonStruct, u16 frontierMonIndex, u32 otID, u32 pid, u8 iv, u8 replacementItemIndex, BOOL replaceItem, u32 heapID)
{
    s32 i;
    FrontierMon frontierMon;
    MI_CpuFill8(frontierMonStruct, 0, sizeof(FrontierMonStruct)); // MI_CpuClear8
    GetFrontierMon(&frontierMon, frontierMonIndex);
    frontierMonStruct->species = frontierMon.species;
    frontierMonStruct->form = frontierMon.form;
    frontierMonStruct->item = replaceItem ? FrontierItemReplacementList[replacementItemIndex] : frontierMon.item;

    // Use max friendship unless the FrontierMon knows Frustration.
    u32 friendship = FRIENDSHIP_MAX;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        frontierMonStruct->moves[i] = frontierMon.moves[i];
        if (frontierMon.moves[i] == MOVE_FRUSTRATION)
        {
            friendship = 0;
        }
    }

    frontierMonStruct->otID = otID;
    u32 pidGen;
    if (!pid) 
    {
        do 
        {
            pidGen = FrontierRand(frontierFsys) | FrontierRand(frontierFsys) << 16;
        } 
        while (frontierMon.nature != GetNatureFromPersonality(pidGen)
            || CalcShininessByOtIdAndPersonality(otID, pidGen));

        frontierMonStruct->pid = pidGen;
    } 
    else
    {
        frontierMonStruct->pid = pid;
        pidGen = pid;
    }
    frontierMonStruct->hpIv = iv;
    frontierMonStruct->atkIv = iv;
    frontierMonStruct->defIv = iv;
    frontierMonStruct->spdIv = iv;
    frontierMonStruct->spAtkIv = iv;
    frontierMonStruct->spDefIv = iv;
    s32 ev = 0;
    for (i = 0; i < 6; i++) // 6 == Number of stats with IVs.
    {
        if (frontierMon.evs & MaskOfFlagNo(i))
        {
            ev++;
        }
    }
    ev = 510 / ev; // TODO: Should probably be turned into a MAX_EV_SUM const.
    if (ev > 255)
    {
        ev = 255;
    }
    for (i = 0; i < 6; i++)
    {
        if (frontierMon.evs & MaskOfFlagNo(i))
        {
            frontierMonStruct->evs[i] = ev;
        }
    }
    frontierMonStruct->ppUp = 0;
    frontierMonStruct->language = LANG_ENGLISH; // gGameLanguage, but HG-E only works with English for now.

    // Check if there is only 1 ability, and if not, randomize between them.
    u32 ability = PokePersonalParaGet(frontierMonStruct->species, PERSONAL_ABILITY_2);
    if (ability != ABILITY_NONE) 
    {
        if (frontierMonStruct->pid % 2) 
        {
            frontierMonStruct->ability = ability;
        }
        else
        {
            frontierMonStruct->ability = PokePersonalParaGet(frontierMonStruct->species, PERSONAL_ABILITY_1);
        }
    }
    else 
    {
        frontierMonStruct->ability = PokePersonalParaGet(frontierMonStruct->species, PERSONAL_ABILITY_1);
    }

    #ifdef FRONTIER_POKEMON_HA_CHANCE
    if (FrontierRand(frontierFsys) % FRONTIER_POKEMON_HA_CHANCE)
    {
        u32 hiddenAbility = GetMonHiddenAbility(frontierMonStruct->species, frontierMonStruct->species);
        if (hiddenAbility != ABILITY_NONE)
        {
            frontierMonStruct->ability = hiddenAbility;
        }
    }
    #endif

    debug_printf("Ability: %d\n", frontierMonStruct->ability);
    frontierMonStruct->friendship = friendship;
    GetSpeciesNameIntoArray(frontierMonStruct->species, heapID, frontierMonStruct->nickname);
    return pidGen;
}

/*
static u32 sub_0204BABC(FrontierFieldSystem *a0, u16 *frontierTrainerData, u16 frontierTrainerIndex, FrontierMonStruct *frontierMonStruct, u8 numPokemon, u16 *partySpecies, u16 *partyHeldItems, FrontierTagBattleAlly *tagAlly, u32 heapID);
static u16 *GetFrontierTrainerData(u32 frontierTrainerIndex, u32 heapID);

// Frontier Trainer IDs grouped by when they show up normally. IDs 0-99 show up normally in battles 1-7, IDs 80-119 show up normally in battles 8-14, etc.
static const u16 FrontierTrainerDifficultyBracket[][2] = {
    { 0,   99  }, // Bracket 0: 1-7.
    { 80,  119 }, // Bracket 1: 8-14.
    { 100, 139 }, // Bracket 2: 15-21.
    { 120, 159 }, // Bracket 3: 22-28.
    { 140, 179 }, // Bracket 4: 29-35.
    { 160, 199 }, // Bracket 5: 36-42.
    { 180, 219 }, // Bracket 6: 43-49.
    { 200, 299 }, // Bracket 7: 50+. This appears to be unused.
};

// Unused.
static const u16 FrontierTrainerOverworlds[][2] = { // _020FBFEC
    { TRAINERCLASS_PKMN_TRAINER_CHERYL, SPRITE_SEVEN1       },
    { TRAINERCLASS_PKMN_TRAINER_RILEY,  SPRITE_SEVEN2       },
    { TRAINERCLASS_PKMN_TRAINER_MARLEY, SPRITE_SEVEN3       },
    { TRAINERCLASS_PKMN_TRAINER_BUCK,   SPRITE_SEVEN4       },
    { TRAINERCLASS_PKMN_TRAINER_MIRA,   SPRITE_SEVEN5       },
    { TRAINERCLASS_YOUNGSTER,           SPRITE_BOY2         },
    { TRAINERCLASS_LASS,                SPRITE_GIRL1        },
    { TRAINERCLASS_SCHOOL_KID_M,        SPRITE_BOY1         },
    { TRAINERCLASS_SCHOOL_KID_F,        SPRITE_GIRL3        },
    { TRAINERCLASS_RICH_BOY,            SPRITE_GORGGEOUSM   },
    { TRAINERCLASS_LADY,                SPRITE_GORGGEOUSW   },
    { TRAINERCLASS_CAMPER,              SPRITE_CAMPBOY      },
    { TRAINERCLASS_PICNICKER,           SPRITE_PICNICGIRL   },
    { TRAINERCLASS_TUBER_M,             SPRITE_BABYBOY1     }, // Seems to be mislabelled.
    { TRAINERCLASS_TUBER_F,             SPRITE_BABYGIRL1    }, // Same here.
    { TRAINERCLASS_POKEFAN_M,           SPRITE_MIDDLEMAN1   },
    { TRAINERCLASS_POKEFAN,             SPRITE_MIDDLEWOMAN1 },
    { TRAINERCLASS_WAITER,              SPRITE_WAITER       },
    { TRAINERCLASS_WAITRESS,            SPRITE_WAITRESS     },
    { TRAINERCLASS_PKMN_BREEDER_M,      SPRITE_MAN1         },
    { TRAINERCLASS_PKMN_BREEDER_F,      SPRITE_WOMAN1       },
    { TRAINERCLASS_CAMERAMAN,           SPRITE_CAMERAMAN    },
    { TRAINERCLASS_REPORTER,            SPRITE_REPORTER     },
    { TRAINERCLASS_RANCHER,             SPRITE_FARMER       },
    { TRAINERCLASS_COWGIRL,             SPRITE_COWGIRL      },
    { TRAINERCLASS_CYCLIST_M,           SPRITE_CYCLEM       },
    { TRAINERCLASS_CYCLIST_F,           SPRITE_CYCLEW       },
    { TRAINERCLASS_BLACK_BELT,          SPRITE_FIGHTER      },
    { TRAINERCLASS_BATTLE_GIRL,         SPRITE_GIRL2        },
    { TRAINERCLASS_VETERAN,             SPRITE_OLDMAN1      },
    { TRAINERCLASS_SOCIALITE,           SPRITE_LADY         },
    { TRAINERCLASS_PSYCHIC_M,           SPRITE_MYSTERY      },
    { TRAINERCLASS_PSYCHIC_F,           SPRITE_MYSTERY      },
    { TRAINERCLASS_PKMN_RANGER_M,       SPRITE_MAN3         },
    { TRAINERCLASS_PKMN_RANGER_F,       SPRITE_WOMAN3       },
    { TRAINERCLASS_ACE_TRAINER_M,       SPRITE_MAN3         },
    { TRAINERCLASS_ACE_TRAINER_F,       SPRITE_WOMAN3       },
    { TRAINERCLASS_ACE_TRAINER_M_GS,    SPRITE_MAN5         },
    { TRAINERCLASS_ACE_TRAINER_F_GS,    SPRITE_WOMAN5       },
    { TRAINERCLASS_DRAGON_TAMER,        SPRITE_MAN3         },
    { TRAINERCLASS_BUG_CATCHER,         SPRITE_BOY3         },
    { TRAINERCLASS_NINJA_BOY,           SPRITE_BABYBOY1     },
    { TRAINERCLASS_JOGGER,              SPRITE_SPORTSMAN    },
    { TRAINERCLASS_FISHERMAN,           SPRITE_FISHING      },
    { TRAINERCLASS_SAILOR,              SPRITE_SEAMAN       },
    { TRAINERCLASS_HIKER,               SPRITE_MOUNT        },
    { TRAINERCLASS_RUIN_MANIAC,         SPRITE_EXPLORE      },
    { TRAINERCLASS_GUITARIST,           SPRITE_MAN2         },
    { TRAINERCLASS_COLLECTOR,           SPRITE_BIGMAN       },
    { TRAINERCLASS_ROUGHNECK,           SPRITE_BADMAN       },
    { TRAINERCLASS_SUPER_NERD,          SPRITE_ASSISTANTM   },
    { TRAINERCLASS_GENTLEMAN,           SPRITE_GENTLEMAN    },
    { TRAINERCLASS_WORKER,              SPRITE_WORKMAN      },
    { TRAINERCLASS_CLOWN,               SPRITE_CLOWN        },
    { TRAINERCLASS_POLICEMAN,           SPRITE_POLICEMAN    },
    { TRAINERCLASS_PI,                  SPRITE_GORGGEOUSM   },
    { TRAINERCLASS_BIRD_KEEPER_GS,      SPRITE_WOMAN3       },
    { TRAINERCLASS_PARASOL_LADY,        SPRITE_AMBRELLA     },
    { TRAINERCLASS_BEAUTY,              SPRITE_WOMAN2       },
    { TRAINERCLASS_AROMA_LADY,          SPRITE_WOMAN1       },
    { TRAINERCLASS_IDOL,                SPRITE_IDOL         },
    { TRAINERCLASS_ARTIST,              SPRITE_ARTIST       },
    { TRAINERCLASS_POKE_KID,            SPRITE_PIKACHU      },
};

// Trainer IDs grouped by when they show up as bracket bosses.
static const u16 FrontierTrainerBracketBosses[][2] = {
    { 100, 119 }, // Bracket 0: Battle 7
    { 120, 139 }, // Bracket 1: Battle 14
    { 140, 159 }, // Bracket 2: Battle 21
    { 160, 179 }, // Bracket 3: Battle 28
    { 180, 199 }, // Bracket 4: Battle 35
    { 200, 219 }, // Bracket 5: Battle 42
    { 220, 239 }, // Bracket 6: Battle 49
    { 200, 299 }, // Bracket 7: All trainers are no longer special. This appears to be unused.
};

MessageFormat *sub_0204B538(SaveData *saveData, u16 numEligiblePokemon, u16 a2, u8 a3, u8 *numLegendaryPokemonSeen) {
    String *speciesName, *unused;
    Pokedex *pokedex;
    MessageFormat *messageFormat;
    MsgData *messageData;
    speciesName = String_New(14, HEAP_ID_FIELD1);
    unused = String_New(2, HEAP_ID_FIELD1);
    pokedex = Save_Pokedex_Get(saveData);
    messageData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_FIELD1);
    messageFormat = MessageFormat_New_Custom(19, 14, HEAP_ID_FIELD1);
    BufferIntegerAsString(messageFormat, 0, numEligiblePokemon, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    for (u8 i = 0; i < NUM_BANNED_BATTLE_FRONTIER; i++)
    {
        u16 species = GetBannedBattleFrontierPokemon(i);
        if (!Pokedex_CheckMonSeenFlag(pokedex, species))
        {
            continue;
        }
        ReadMsgDataIntoString(messageData, species, speciesName);
        // a2 and a3 are always unused, and in this case will invariably be 0.
        BufferString(messageFormat, *numLegendaryPokemonSeen + 1, speciesName, a2, a3, 2);
        (*numLegendaryPokemonSeen)++;
    }
    DestroyMsgData(messageData);
    String_Delete(unused);
    String_Delete(speciesName);
    return messageFormat;
}

u32 GetFrontierTrainerOverworld(u8 trainerClass) //sub_0204B5E8
{
    for (u32 i = 0; i < NELEMS(FrontierTrainerOverworlds); i++)
    {
        if (trainerClass == FrontierTrainerOverworlds[i][0])
        {
            return FrontierTrainerOverworlds[i][1];
        }
    }
    return SPRITE_BOY1;
}

u16 sub_0204B610(FieldSystem *fieldSystem, u16 *a1) {
    u16 unk = 0;
    FrontierFieldSystem *frontierFsys = fieldSystem->unkA0;
    frontierFsys->unk12 = a1[0];
    frontierFsys->unk16 = a1[1];
    frontierFsys->unk18 = a1[2];
    frontierFsys->unk14 = a1[3];
    frontierFsys->unk10_5 = frontierFsys->unk12 + 5;
    if (frontierFsys->unk2e[0] == frontierFsys->unk16 || frontierFsys->unk2e[0] == frontierFsys->unk18)
    {
        unk++;
    }
    if (frontierFsys->unk2e[1] == frontierFsys->unk16 || frontierFsys->unk2e[1] == frontierFsys->unk18)
    {
        unk += 2;
    }
    return unk;
}

BOOL sub_0204B66C(FieldSystem *fieldSystem, u16 *a1)
{
    FrontierFieldSystem *frontierFsys = fieldSystem->unkA0;
    if (sub_0203769C() == 0) 
    {
        return FALSE;
    }
    memcpy(frontierFsys->unk3e, a1, sizeof(frontierFsys->unk3e));
    return TRUE;
}

BOOL sub_0204B690(FieldSystem *fieldSystem, u16 *a1)
{
    return fieldSystem->unkA0->unk10_3 || a1[0] != 0;
}

void sub_0204B6AC(FrontierFieldSystem *frontierFsys, SaveData *saveData)
{
    frontierFsys->unk83e[0] = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(saveData));
    Party *party = SaveArray_Party_Get(saveData);
    for (s32 i = 0; i < 2; i++)
    {
        PartyPokemon *mon = Party_GetMonByIndex(party, frontierFsys->partyMonIndexes[i]);
        frontierFsys->unk83e[1 + i] = GetMonData(mon, MON_DATA_SPECIES, NULL);
    }
    frontierFsys->unk83e[3] = sub_0202D57C(frontierFsys->frontierData, 3, 0);
}

void sub_0204B708(FrontierFieldSystem *frontierFsys)
{
    memcpy(frontierFsys->unk83e, frontierFsys->unk3e, sizeof(frontierFsys->unk3e));
}

void sub_0204B720(FrontierFieldSystem *frontierFsys, u32 a1)
{
    // It seems like a1 will always be 0, whatever it means.
    frontierFsys->unk10_3 = a1;
    frontierFsys->unk83e[0] = a1;
}

u16 GetRandomBattleTowerTrainer(FrontierFieldSystem *frontierFsys, u8 difficultyBracket, u8 winsInBracket, u32 a3) // sub_0204B73C
{
    // a3 is probably whether we can qualify to fight Palmer. I'm unsure as to what this is, maybe Singles?
    if (a3 == 0)
    {
        // If we are between battles 15-21 and have completed 6 battles in our current set, meaning this is battle 21:
        if (difficultyBracket == 2 && winsInBracket == 6)
        {
            return 305; // Palmer (Silver Print)
        }
        // If we are between battles 43-49 and have completed 6 battles in our current set, meaning this is battle 49:
        if (difficultyBracket == 6 && winsInBracket == 6)
        {
            return 306; // Palmer (Gold Print)
        }
    }
    
    if (difficultyBracket < 7) 
    {
        if (winsInBracket == 6)
        {
            // Mod is the amount of trainers in the current bracket.
            u16 mod = FrontierTrainerBracketBosses[difficultyBracket][1] - FrontierTrainerBracketBosses[difficultyBracket][0] + 1;
            // Get a random trainer from the current bracket based on conditions I don't really understand (yet).
            return FrontierRand(frontierFsys) % mod + FrontierTrainerBracketBosses[difficultyBracket][0]; // FrontierRand == sub_0204B510
        } 
        else 
        {
            u16 mod = FrontierTrainerDifficultyBracket[difficultyBracket][1] - FrontierTrainerDifficultyBracket[difficultyBracket][0] + 1;
            return FrontierRand(frontierFsys) % mod + FrontierTrainerDifficultyBracket[difficultyBracket][0];
        }
    }
    else // difficultyBracket == 7, meaning we are on battle 50+.
    {
        // Get a random trainer from the final bracket, which will always be from 200-299.
        return FrontierRand(frontierFsys) % 100 + 200;
    }
}

static u16 *sub_0204B7D0(FrontierTrainer *frontierTrainer, u32 frontierTrainerIndex, u32 heapID)
{
    MsgData *messageData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0027_bin, heapID);
    MI_CpuFill8(frontierTrainer, 0, sizeof(FrontierTrainer)); // MI_CpuClear8.
    u16 *frontierTrainerData = GetFrontierTrainerData(frontierTrainerIndex, heapID);
    frontierTrainer->frontierTrainerIndex = frontierTrainerIndex;
    frontierTrainer->unk18 = 0xffff;
    frontierTrainer->unk1a = frontierTrainerIndex * 3;
    frontierTrainer->trainerClass = frontierTrainerData[0];
    String *trainerName = NewString_ReadMsgData(messageData, frontierTrainerIndex);
    CopyStringToU16Array(trainerName, frontierTrainer->unk08, NELEMS(frontierTrainer->unk08));
    String_Delete(trainerName);
    DestroyMsgData(messageData);
    return frontierTrainerData;
}

u32 sub_0204BA04(FrontierFieldSystem *frontierFsys, FrontierTrainer *frontierTrainer, u16 frontierTrainerIndex, u32 numPokemon, u16 *partySpecies, u16 *partyHeldItems, FrontierTagBattleAlly *tagAlly, u32 heapID)
{
    u16 *frontierTrainerData = sub_0204B7D0(frontierTrainer, frontierTrainerIndex, heapID);
    u32 replaceItem = sub_0204BABC(frontierFsys, frontierTrainer, frontierTrainerIndex, frontierTrainer->frontierMonStructs, numPokemon, partySpecies, partyHeldItems, tagAlly, heapID);
    Heap_Free(frontierTrainerData);
    return replaceItem;
}

void GenerateAllyFrontierMons(FrontierFieldSystem *frontierFsys, FrontierTrainer *frontierTrainer, u16 frontierTrainerIndex, u32 replaceItem, FrontierTagBattleAlly *tagAlly, u32 heapID) // sub_0204BA50
{
    u8 iv;
    u16 *frontierTrainerData = sub_0204B7D0(frontierTrainer, frontierTrainerIndex, heapID);
    // Return IVs according to trainer index. The higher the index number, the higher the IVs.
    iv = GetFrontierTrainerIVs(frontierTrainerIndex); // sub_0204B4D4
    for (s32 i = 0; i < 2; i++) 
    {
        GenerateFrontierMon(frontierFsys, &frontierTrainer->frontierMonStructs[i], tagAlly->frontierMonIDs[i], tagAlly->otID, tagAlly->frontierMonPIDs[i], iv, i, replaceItem, heapID);
    }
    Heap_Free(frontierTrainerData);
}

static u32 sub_0204BABC(FrontierFieldSystem *frontierFsys, u16 *frontierTrainerData, u16 frontierTrainerIndex, FrontierMonStruct *a3, u8 numPokemon, u16 *partySpecies, u16 *partyHeldItems, FrontierTagBattleAlly *tagAlly, u32 heapID)
{
    s32 i;
    u8 iv;
    u32 otID;
    u32 frontierMonIndex;
    u32 frontierMonIDs[4];
    u32 frontierMonPIDs[4];
    BOOL replaceItem = FALSE;
    FrontierMon frontierMon_1;
    FrontierMon frontierMon_2;
    GF_ASSERT(numPokemon <= 4);

    s32 failedItemGenAttempts = 0;
    s32 count = 0;
    while (count != numPokemon)
    {
        // Get a random frontierMonIndex according to the amount of mons defined for that trainer.
        u8 index = FrontierRand(frontierFsys) % frontierTrainerData[1];
        frontierMonIndex = frontierTrainerData[2 + index];
        GetFrontierMon(&frontierMon_2, frontierMonIndex);
        for (i = 0; i < count; i++)
        {
            GetFrontierMon(&frontierMon_1, frontierMonIDs[i]);
            // Do not return the same Pokemon twice.
            if (frontierMon_1.species == frontierMon_2.species)
            {
                break;
            }
        }
        if (i != count)
        {
            continue;
        }
        if (partySpecies)
        {
            for (i = 0; i < numPokemon; i++)
            {
                if (partySpecies[i] == frontierMon_2.species)
                {
                    break;
                }
            }
            if (i != numPokemon)
            {
                continue;
            }
        }
        if (failedItemGenAttempts < 50)
        {
            // For the amount of Pokemon we have already finished generating:
            for (i = 0; i < count; i++)
            {
                // Get each one and make sure their item does not match the one we are currently working with.
                GetFrontierMon(&frontierMon_1, frontierMonIDs[i]);
                if (frontierMon_1.item && frontierMon_1.item == frontierMon_2.item)
                {
                    break;
                }
            }
            // If we have a duplicate held item, regenerate the current FrontierMon.
            if (i != count)
            {
                failedItemGenAttempts++;
                continue;
            }
            if (partyHeldItems)
            {
                for (i = 0; i < numPokemon; i++)
                {
                    if (partyHeldItems[i] == frontierMon_2.item && partyHeldItems[i] != ITEM_NONE)
                    {
                        break;
                    }
                }
                if (i != numPokemon) 
                {
                    failedItemGenAttempts++;
                    continue;
                }
            }
        }
        frontierMonIDs[count] = frontierMonIndex;
        count++;
    }
    iv = GetFrontierTrainerIVs(frontierTrainerIndex); // sub_0204B4D4
    otID = FrontierRand(frontierFsys) | FrontierRand(frontierFsys) << 16;
    // If we had 50 or more failed generation attempts:
    if (failedItemGenAttempts >= 50)
    {
        replaceItem = TRUE;
    }
    for (i = 0; i < count; i++)
    {
        frontierMonPIDs[i] = GenerateFrontierMon(frontierFsys, &a3[i], frontierMonIDs[i], otID, 0, iv, i, replaceItem, heapID);
    }
    if (tagAlly == NULL)
    {
        return replaceItem;
    }
    tagAlly->otID = otID;
    for (i = 0; i < 2; i++)
    {
        tagAlly->frontierMonIDs[i] = frontierMonIDs[i];
        tagAlly->frontierMonPIDs[i] = frontierMonPIDs[i];
    }
    return replaceItem;
}

static u16 *GetFrontierTrainerData(u32 frontierTrainerIndex, u32 heapID)
{
    return AllocAndReadWholeNarcMemberByIdPair(NARC_a_2_0_2, frontierTrainerIndex, heapID);
}

void GetFrontierMon(FrontierMon *mon, u32 frontierMonIndex)
{
    ReadWholeNarcMemberByIdPair(mon, NARC_a_2_0_3, frontierMonIndex);
}

*/