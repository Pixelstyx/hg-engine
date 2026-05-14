#include "../../include/battle.h"
#include "../../include/config.h"
#include "../../include/pokemon.h"
#include "../../include/constants/ability.h"

// These should probably be in header files but we're doing this to avoid conflicts with upstream.
int LONG_CALL PokeParty_GetPokeCount(struct Party *party); // const?
BOOL LONG_CALL MonNotFaintedOrEgg(struct PartyPokemon *mon);

BOOL ApplyPoisonStep(struct Party *party, u16 location) {
#ifdef UPDATE_OVERWORLD_POISON
    return 0;
#endif
    int n_poisoned = 0;
    int n_survived = 0;
    for (int i = 0; i < PokeParty_GetPokeCount(party); i++) {
        struct PartyPokemon *mon = Party_GetMonByIndex(party, i);
        if (!MonNotFaintedOrEgg(mon)) {
            continue;
        }
        if ((!(GetMonData(mon, MON_DATA_STATUS, NULL) & (STATUS_POISON | STATUS_BAD_POISON)) || GetMonData(mon, MON_DATA_ABILITY, NULL) == ABILITY_MAGIC_GUARD)) {
            continue;
        }
        u32 hp = GetMonData(mon, MON_DATA_HP, NULL);
        if (GetMonData(mon, MON_DATA_ABILITY, NULL) == ABILITY_POISON_HEAL) {
            if (hp < GetMonData(mon, MON_DATA_MAXHP, NULL)) {
                hp++;
            }
        }
        else if (hp > 1) {
            hp--;
        }
        SetMonData(mon, MON_DATA_HP, &hp);
        if (GetMonData(mon, MON_DATA_ABILITY, NULL) != ABILITY_POISON_HEAL) {
            if (hp == 1) {
                n_survived++;
                MonApplyFriendshipMod(mon, 7, location); // FRIENDSHIP_EVENT_HEAL_FIELD_PSN
                ApplyMonMoodModifier(mon, 5); // MON_MOOD_MODIFIER_SURVIVED_PSN
            }
            n_poisoned++;
        }
    }
    if (n_survived != 0) {
        return 2;
    } else if (n_poisoned != 0) {
        return 1;
    } else {
        return 0;
    }
}

BOOL SurvivePoisoning(struct PartyPokemon *mon) {
    if ((GetMonData(mon, MON_DATA_STATUS, NULL) & (STATUS_POISON | STATUS_BAD_POISON)) 
    && GetMonData(mon, MON_DATA_HP, NULL) == 1 
    && !(GetMonData(mon, MON_DATA_ABILITY, NULL) == ABILITY_POISON_HEAL || GetMonData(mon, MON_DATA_ABILITY, NULL) == ABILITY_MAGIC_GUARD)) {
        u32 status = STATUS_NONE;
        SetMonData(mon, MON_DATA_STATUS, &status);
        return TRUE;
    } else {
        return FALSE;
    }
}