#include "../../include/constants/ability.h"
#include "../../include/constants/item.h"
#include "../../include/constants/maps.h"
#include "../../include/battle.h"
#include "../../include/debug.h"
#include "../../include/rock_smash_item.h"

static BOOL CheckRockSmashItemDrop(FieldSystem *fieldSystem, RockSmashItemCheckWork *env) {
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
