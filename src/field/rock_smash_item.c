#include "../../include/constants/ability.h"
#include "../../include/constants/file.h"
#include "../../include/constants/item.h"
#include "../../include/constants/maps.h"
#include "../../include/battle.h"
#include "../../include/debug.h"
#include "../../include/map_events_internal.h"
#include "../../include/rock_smash_item.h"

typedef struct {
    u16 odds;
    u16 table;
} ROCK_SMASH;

typedef struct {
    BOOL followMonKnowsHM;
    int ability;
    u16 *itemFound; // TODO: bool16
    u16 *item;
    ROCK_SMASH rockSmash;
    u32 unk14;
} RockSmashItemCheckWork;

static const u16 sRockSmashItems_Default[] = {
    ITEM_MAX_ETHER,
    ITEM_REVIVE,
    ITEM_HEART_SCALE,
    ITEM_RED_SHARD,
    ITEM_BLUE_SHARD,
    ITEM_GREEN_SHARD,
    ITEM_YELLOW_SHARD,
    ITEM_STAR_PIECE,
};

static const u16 sRockSmashItems_RuinsOfAlph[] = {
    ITEM_RED_SHARD,
    ITEM_YELLOW_SHARD,
    ITEM_HELIX_FOSSIL,
    ITEM_MAX_ETHER,
    ITEM_BLUE_SHARD,
    ITEM_GREEN_SHARD,
    ITEM_OLD_AMBER,
    ITEM_MAX_REVIVE,
};

static const u16 sRockSmashItems_CliffCave[] = {
    ITEM_MAX_ETHER,
    ITEM_PEARL,
    ITEM_BIG_PEARL,
    ITEM_RED_SHARD,
    ITEM_YELLOW_SHARD,
    ITEM_CLAW_FOSSIL,
    ITEM_CLAW_FOSSIL,
    ITEM_RARE_BONE,
};

static BOOL LONG_CALL Task_RockSmashItemCheck(TaskManager *taskman);
static BOOL LONG_CALL CheckRockSmashItemDrop(FieldSystem *fieldSystem, RockSmashItemCheckWork *env);
static BOOL LONG_CALL Task_GetRockSmashItem(TaskManager *taskman);
static int LONG_CALL DrawRockSmashIdx(FieldSystem *fieldSystem);

void LONG_CALL FieldSystem_RockSmashItemCheck(FieldSystem *fieldSystem, int followMonKnowsHm, u16 *itemFound, u16 *item) {
    RockSmashItemCheckWork *env = sys_AllocMemoryLo(HEAPID_WORLD, sizeof(RockSmashItemCheckWork));
    env->followMonKnowsHM = followMonKnowsHm != 0;
    env->itemFound = itemFound;
    env->item = item;
    *env->itemFound = FALSE;
    *env->item = ITEM_NONE;
    TaskManager_Call(fieldSystem->taskman, Task_RockSmashItemCheck, env);
}

static BOOL LONG_CALL Task_RockSmashItemCheck(TaskManager *taskman) {
    struct BattleSetup *setup;
    FieldSystem *fieldSystem = taskman->fieldSystem;
    RockSmashItemCheckWork *env = taskman->env;

    if (ov02_022470A0(fieldSystem, &setup)) {
        *env->itemFound = FALSE;
        *env->item = ITEM_NONE;
        sys_FreeMemoryEz(env);
        FieldSystem_StartForcedWildBattle(fieldSystem, taskman, setup);
        return FALSE;
    }

    if (CheckRockSmashItemDrop(fieldSystem, env)) {
        TaskManager_Jump(taskman, Task_GetRockSmashItem, env);
        return FALSE;
    }

    *env->itemFound = FALSE;
    *env->item = ITEM_NONE;
    sys_FreeMemoryEz(env);
    return TRUE;
}

static BOOL LONG_CALL CheckRockSmashItemDrop(FieldSystem *fieldSystem, RockSmashItemCheckWork *env) {
    int odds;
    int ability;
    ROCK_SMASH data;

    int mapID = fieldSystem->location->mapId;
    if (mapID < MAP_ID_MAX)
    {
        // Fills data with base odds and table ID.
        ReadWholeNarcMemberByIdPair(&data, 255, mapID); // NARC_a_2_5_3
    }
    else
    {   
        // It's definitely easier to store that here for now with custom maps.
        switch (mapID)
        {
            case MAP_UNION_TRAIL:
                data.odds = 30;
                data.table = ROCK_SMASH_TABLE_DEFAULT;
                break;
            default:
                break;
        }
    }
    
    odds = data.odds;
    if (odds == 0) {
        return FALSE;
    }
    if (data.table >= NUM_ROCK_SMASH_TABLES) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    struct PartyPokemon *mon = Party_GetMonByIndex(SaveData_GetPlayerPartyPtr(fieldSystem->savedata), 0);
    if (GetMonData(mon, MON_DATA_IS_EGG, NULL) == 0) {
        ability = GetMonData(mon, MON_DATA_ABILITY, NULL);
    } else {
        ability = NUM_ABILITIES;
    }
    env->ability = ability;

    switch (ability) {
    case ABILITY_SUCTION_CUPS:
        odds += 5;
        break;
    case ABILITY_MAGNET_PULL:
        odds += 5;
        break;
    case ABILITY_KEEN_EYE:
        odds += 5;
        break;
    }
    if (env->followMonKnowsHM) {
        odds += 5;
    }
    if (odds > 100) {
        odds = 100;
    }

    if (gf_rand() % 100 < odds) {
        env->rockSmash = data;
        return TRUE;
    }
    return FALSE;
}

static BOOL LONG_CALL Task_GetRockSmashItem(TaskManager *taskman) {
    LocalMapObject *obj;

    u32 *state_p = &taskman->state;
    RockSmashItemCheckWork *env = taskman->env;
    FieldSystem *fieldSystem = taskman->fieldSystem;

    switch (*state_p) {
    case 0:
        if (env->followMonKnowsHM) {
            obj = fieldSystem->followMon.mapObject;
        } else {
            obj = fieldSystem->playerAvatar->mapObject;
        }
        env->unk14 = ov01_02200540(obj, 0, 1);
        ++(*state_p);
        break;
    case 1:
        if (ov01_022003F4(env->unk14)) {
            sub_02068B48(env->unk14);
            ++(*state_p);
        }
        break;
    case 2: {
        u32 idx = DrawRockSmashIdx(fieldSystem);
        if ((env->ability == ABILITY_SERENE_GRACE || env->ability == ABILITY_SUPER_LUCK) && idx < 7) {
            idx = (u8)(idx + 1);
        }
        GF_ASSERT(idx < 8);

        u16 item;
        switch (env->rockSmash.table) {
        case ROCK_SMASH_TABLE_DEFAULT:
            item = sRockSmashItems_Default[idx];
            break;
        case ROCK_SMASH_TABLE_RUINS_OF_ALPH:
            item = sRockSmashItems_RuinsOfAlph[idx];
            break;
        case ROCK_SMASH_TABLE_CLIFF_CAVE:
            item = sRockSmashItems_CliffCave[idx];
            break;
        default:
            GF_ASSERT(FALSE);
            *env->itemFound = FALSE;
            *env->item = ITEM_NONE;
            sys_FreeMemoryEz(env);
            return TRUE;
        }
        *env->itemFound = TRUE;
        *env->item = item;
        sys_FreeMemoryEz(env);
        return TRUE;
    }
    }
    return FALSE;
}

static int LONG_CALL DrawRockSmashIdx(UNUSED FieldSystem *fieldSystem) {
    u8 rand = gf_rand() % 100;
    if (rand < 25) {
        return 0;
    }
    if (rand < 45) {
        return 1;
    }
    if (rand < 55) {
        return 2;
    }
    if (rand < 65) {
        return 3;
    }
    if (rand < 75) {
        return 4;
    }
    if (rand < 85) {
        return 5;
    }
    if (rand < 95) {
        return 6;
    }
    return 7;
}
