#include "../include/pokemon.h"
#include "../include/constants/item.h"

void LONG_CALL Daycare_GetBothBoxMonsPtr(struct Daycare *dayCare, struct BoxPokemon **boxmons);
BOOL LONG_CALL Daycare_TryGetForcedInheritedIV(Daycare *dayCare, u8 *powerItemStat, u8 *dayCareSlot);
void LONG_CALL _IVList_Remove(u8 *ptr, int idx);

int LONG_CALL Daycare_EverstoneCheck(struct Daycare *dayCare) {
    struct BoxPokemon *boxmons[2];
    Daycare_GetBothBoxMonsPtr(dayCare, boxmons);

    u8 num_everstones = 0;
    u8 everstone_idx = 0;
    for (int i = 0; i < 2; i++) {
        if (GetBoxMonData(boxmons[i], MON_DATA_HELD_ITEM, NULL) == ITEM_EVERSTONE) {
            everstone_idx = i;
            num_everstones++;
        }
    }

    if (num_everstones == 2) {
        everstone_idx = (gf_rand() % 2) ? 0 : 1; // This would obviously be more efficient without the inline if, but we should keep RNG consistent with vanilla.
    }

    if (num_everstones == 0 /*|| gf_rand() >= 0x7FFF*/) { // Vanilla bug: there is actually a ~50.0015% chance to fail nature inheritance.
        return -1;
    } else {
        return everstone_idx;
    }
}

void LONG_CALL InheritIVs(struct PartyPokemon *egg, struct Daycare *dayCare) {
    u8 statsToInherit[5];
    u8 statList[6];
    u8 monToInheritFrom[5];
    u8 i;
    u8 j;

    u8 totalStatsToInherit = 3;
    for (i = 0; i < 2; i++) {
        if (GetBoxMonData(Daycare_GetBoxMonI(dayCare, i), MON_DATA_HELD_ITEM, NULL) == ITEM_DESTINY_KNOT) {
            totalStatsToInherit = 5;
            break;
        }
    }

    for (i = 0; i < 6; i++) {
        statList[i] = i;
    }

    u8 powerItemStat;
    u8 startNum; // Try to randomly inherit less stats if a power item applies.
    u8 daycareSlot;
    if (Daycare_TryGetForcedInheritedIV(dayCare, &powerItemStat, &daycareSlot)) { // Power Items.
        statsToInherit[0] = powerItemStat;
        startNum = 1;
        _IVList_Remove(statList, powerItemStat);
        monToInheritFrom[0] = daycareSlot;
    } else {
        startNum = 0;
    }
    
    for (i = startNum; i < totalStatsToInherit; i++) {
        j = (gf_rand() % (6 - i));
        statsToInherit[i] = statList[j];
        _IVList_Remove(statList, j);
    }

    for (i = startNum; i < totalStatsToInherit; i++) {
        monToInheritFrom[i] = gf_rand() % 2;
    }
    for (i = 0; i < totalStatsToInherit; i++) {
        u8 parentIV = GetBoxMonData(Daycare_GetBoxMonI(dayCare, monToInheritFrom[i]), MON_DATA_HP_IV + statsToInherit[i], NULL);
        SetMonData(egg, MON_DATA_HP_IV, &parentIV);
    }
}