#include "../include/types.h"
#include "../include/config.h"
#include "../include/pokemon.h"
#include "../include/bag.h"
#include "../include/battle.h"
#include "../include/constants/item.h"
#include "../include/constants/moves.h"
#include "../include/constants/species.h"
#include "../include/constants/file.h"
#include "../include/party_menu.h"
#include "../include/constants/generated/learnsets.h"

u16 GetFieldEffectMoveID(u8 fieldEffect);
BOOL CanAccessFieldMove(struct PartyPokemon *mon, u16 fieldMove, int heapID);

u8 LONG_CALL sub_0207B0B0(struct PLIST_WORK *wk, u8 *buf)
{
    struct PartyPokemon *pp = Party_GetMonByIndex(wk->dat->pp, wk->pos);
    //u32 *learnset = GetCompleteLearnset(pp, HEAP_ID_FIELD3);
    u16 move;
    u8 displayedCount = 0;
    u8 i;
    u8 count = 0;
    u8 fieldEffect;

    u8 isEgg = GetMonData(pp, MON_DATA_IS_EGG, NULL);
    u32 item = GetMonData(pp, MON_DATA_HELD_ITEM, NULL);

    buf[count] = PARTY_MON_CONTEXT_MENU_SUMMARY;
    ++count;
    if (!FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(wk->dat->fsys))
    {
        buf[count] = PARTY_MON_CONTEXT_MENU_SWITCH;
        ++count;
        if (!isEgg)
        {
            if (IS_ITEM_MAIL(item))
            {
                buf[count] = PARTY_MON_CONTEXT_MENU_MAIL;
            }
            else
            {
                buf[count] = PARTY_MON_CONTEXT_MENU_ITEM;
            }
            ++count;
            buf[count] = PARTY_MON_CONTEXT_MENU_QUIT;
            ++count;

            int fieldEffectsToDisplay[4];

            // Priority 1: Known field moves.
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                move = GetMonData(pp, MON_DATA_MOVE1 + i, NULL);
                if (move == MOVE_NONE)
                {
                    break;
                }

                fieldEffect = MoveId_GetFieldEffectId(move);
                if (fieldEffect > PARTY_MON_CONTEXT_MENU_FLASH && fieldEffect != 0xFF)
                {
                    fieldEffectsToDisplay[displayedCount] = fieldEffect;
                    ++displayedCount;
                }
            }

            // Priority 2: Fly.
            i = PARTY_MON_CONTEXT_MENU_FLY;
            if (CanAccessFieldMove(pp, GetFieldEffectMoveID(i), HEAP_ID_PARTY_MENU)
            && displayedCount < 4)
            {
                fieldEffectsToDisplay[displayedCount] = i;
                ++displayedCount;
            }

            // Priority 3: Flash.
            i = PARTY_MON_CONTEXT_MENU_FLASH;
            if (CanAccessFieldMove(pp, GetFieldEffectMoveID(i), HEAP_ID_PARTY_MENU)
            && displayedCount < 4)
            {
                fieldEffectsToDisplay[displayedCount] = i;
                ++displayedCount;
            }

            // Priority 4: Unknown (valid) HM field moves.
            for (i = PARTY_MON_CONTEXT_MENU_FIELD_MOVES_BEGIN; i <= PARTY_MON_CONTEXT_MENU_MAX; i++)
            {
                if (displayedCount == 4) break;
                if (i == PARTY_MON_CONTEXT_MENU_FLY || i == PARTY_MON_CONTEXT_MENU_FLASH) continue;
                if (CanAccessFieldMove(pp, GetFieldEffectMoveID(i), HEAP_ID_PARTY_MENU))
                {
                    fieldEffectsToDisplay[displayedCount] = i;
                    ++displayedCount;
                }
            }

            // Loop through fieldEffectsToDisplay and display all selected field moves.
            for (i = 0; i < displayedCount; i++)
            {
                buf[count] = fieldEffectsToDisplay[i];
                ++count;
                PartyMenu_ContextMenuAddFieldMove(wk, GetFieldEffectMoveID(fieldEffectsToDisplay[i]), i);
            }
        }
        else
        {
            buf[count] = PARTY_MON_CONTEXT_MENU_QUIT;
            ++count;
        }
    }
    else
	{
        buf[count] = PARTY_MON_CONTEXT_MENU_QUIT;
        ++count;
    }
    
    return count;
}

BOOL CanAccessFieldMove(struct PartyPokemon *mon, u16 fieldMove, int heapID)
{
    BAG_DATA *bag = Sav2_Bag_get(SaveBlock2_get());
    switch (fieldMove)
    {
        case MOVE_CUT:
            if (Bag_HasItem(bag, ITEM_HM01, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_HM01)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_FLY:
            if (Bag_HasItem(bag, ITEM_HM02, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_HM02)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_SURF:
            if (Bag_HasItem(bag, ITEM_HM03, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_HM03)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_STRENGTH:
            if (Bag_HasItem(bag, ITEM_HM04, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_HM04)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_ROCK_SMASH:
            if (Bag_HasItem(bag, ITEM_HM06, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_HM06)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_WATERFALL:
            if (Bag_HasItem(bag, ITEM_HM07, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_HM07)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_ROCK_CLIMB:
            if (Bag_HasItem(bag, ITEM_HM08, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_HM08)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_WHIRLPOOL:
            if (Bag_HasItem(bag, ITEM_HM05, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_HM05)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_FLASH:
            if (Bag_HasItem(bag, ITEM_TM070, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_TM070)))
                {
                    return TRUE;
                }
            }
            break;
        /*case MOVE_TELEPORT:
            break;
        case MOVE_DIG:
            if (Bag_HasItem(bag, ITEM_TM028, 1, heapID))
            {
                if (GetMonMachineMoveCompat(mon, ItemToMachineMoveIndex(ITEM_TM028)))
                {
                    return TRUE;
                }
            }
            break;
        case MOVE_SWEET_SCENT:
            break;
        case MOVE_CHATTER:
            break;
        case MOVE_HEADBUTT:
            break;
        case MOVE_MILK_DRINK:
            break;
        case MOVE_SOFT_BOILED:
            break;*/
        default: break;
    }
    return FALSE;
}

u16 GetFieldEffectMoveID(u8 fieldEffect)
{
    switch (fieldEffect)
    {
        case PARTY_MON_CONTEXT_MENU_CUT:
            return MOVE_CUT;
        case PARTY_MON_CONTEXT_MENU_FLY:
            return MOVE_FLY;
        case PARTY_MON_CONTEXT_MENU_SURF:
            return MOVE_SURF;
        case PARTY_MON_CONTEXT_MENU_STRENGTH:
            return MOVE_STRENGTH;
        case PARTY_MON_CONTEXT_MENU_ROCK_SMASH:
            return MOVE_ROCK_SMASH;
        case PARTY_MON_CONTEXT_MENU_WATERFALL:
            return MOVE_WATERFALL;
        case PARTY_MON_CONTEXT_MENU_ROCK_CLIMB:
            return MOVE_ROCK_CLIMB;
        case PARTY_MON_CONTEXT_MENU_WHIRLPOOL:
           return MOVE_WHIRLPOOL;
        case PARTY_MON_CONTEXT_MENU_FLASH:
            return MOVE_FLASH;
        case PARTY_MON_CONTEXT_MENU_TELEPORT:
            return MOVE_TELEPORT;
        case PARTY_MON_CONTEXT_MENU_DIG:
            return MOVE_DIG;
        case PARTY_MON_CONTEXT_MENU_SWEET_SCENT:
            return MOVE_SWEET_SCENT;
        case PARTY_MON_CONTEXT_MENU_CHATTER:
            return MOVE_CHATTER;
        case PARTY_MON_CONTEXT_MENU_HEADBUTT:
            return MOVE_HEADBUTT;
        case PARTY_MON_CONTEXT_MENU_MILK_DRINK:
            return MOVE_SWEET_SCENT;
        case PARTY_MON_CONTEXT_MENU_SOFTBOILED:
            return MOVE_SOFT_BOILED;
        default: break;
    }
    return MOVE_NONE;
}

u32 *GetCompleteLearnset(struct PartyPokemon *mon, int heapID) {
    u32 species = (u16)GetMonData(mon, MON_DATA_SPECIES, NULL);
    u32 form = GetMonData(mon, MON_DATA_FORM, NULL);

    u32 *returnTable = sys_AllocMemory(heapID, MAX_LEVELUP_MOVES * sizeof(u32));

    // Load level up learnset into levelUpTable.
    u32 *levelUpTable = sys_AllocMemory(heapID, MAX_LEVELUP_MOVES * sizeof(u32));
    LoadLevelUpLearnset_HandleAlternateForm(species, form, levelUpTable);

    // Add levelUpTable to returnTable.

    // Be sure to dispose of levelUpTable since we had to allocate memory.
    sys_FreeMemoryEz(levelUpTable);

    // Load TM learnset.
    u32 machineLearnset[MACHINE_LEARNSETS_BITFIELD_COUNT];
    ArchiveDataLoadOfs(machineLearnset, ARC_CODE_ADDONS, CODE_ADDON_MACHINE_LEARNSETS, PokeOtherFormMonsNoGet(species, form) * MACHINE_LEARNSETS_BITFIELD_COUNT * sizeof(u32), MACHINE_LEARNSETS_BITFIELD_COUNT * sizeof(u32));

    // Add TM learnset to returnTable.

    // Load tutor learnset.
    u32 tutorLearnset[TUTOR_LEARNSETS_BITFIELD_COUNT];
    ArchiveDataLoadOfs(tutorLearnset, ARC_CODE_ADDONS, CODE_ADDON_TUTOR_LEARNSETS, PokeOtherFormMonsNoGet(species, form) * TUTOR_LEARNSETS_BITFIELD_COUNT * sizeof(u32), TUTOR_LEARNSETS_BITFIELD_COUNT * sizeof(u32));

    // Add tutorLearnset to returnTable.

    // Load egg moves?

    return returnTable;
}

void LONG_CALL sub_0207AFC4(struct PLIST_WORK *wk)
{
    ClearFrameAndWindow2(&wk->windows[PARTY_MENU_WINDOW_ID_32], TRUE);

    u8 *buf;
    buf = sys_AllocMemory(HEAP_ID_PARTY_MENU, MAX_BUTTONS_IN_PARTY_MENU);
    u8 numItems;

    switch (wk->dat->mode)//(partyMenu->args->context)
    {
    case PARTY_MENU_CONTEXT_0:
        numItems = sub_0207B0B0(wk, buf);
        break;
    case PARTY_MENU_CONTEXT_UNION_ROOM_BATTLE_SELECT:
    case PARTY_MENU_CONTEXT_17:
        numItems = sub_0207B23C(wk, buf);
        break;
    case PARTY_MENU_CONTEXT_ATTACH_CAPSULE:
        numItems = PartyMenu_SetContextMenuItems_GiveCapsule(wk, buf);
        break;
    case PARTY_MENU_CONTEXT_18:
        numItems = sub_0207B1C8(wk, buf);
        break;
    case PARTY_MENU_CONTEXT_SPIN_TRADE:
        numItems = PartyMenu_SetContextMenuItems_SpinTrade(wk, buf);
        break;
    case PARTY_MENU_CONTEXT_BATTLE_HALL:
        numItems = PartyMenu_SetContextMenuItems_BattleHallEntry(wk, buf);
        break;
    case PARTY_MENU_CONTEXT_23:
        numItems = sub_0207B2DC(wk, buf);
        break;
    default:
        numItems = sub_0207B200(wk, buf);
        break;
    }

    PartyMenu_OpenContextMenu(wk, buf, numItems);
    FreeToHeapExplicit(HEAP_ID_PARTY_MENU, buf);
    sub_0207D1C8(wk);
    PartyMenu_PrintMessageOnWindow33(wk, -1, TRUE);
    thunk_Sprite_SetPalIndex(wk->sprites[PARTY_MENU_SPRITE_ID_CURSOR], 1);
}

/*
 * @brief hooks rare candy usage in the bag to allow for repeated use without returning to the bag between each
 * thanks to yako for the for the format
 */
int PartyMenu_ItemUseFunc_LevelUpLearnMovesLoop_Case6(struct PLIST_WORK *wk) {
    struct PartyPokemon *mon = Party_GetMonByIndex(wk->dat->pp, wk->pos);
    wk->dat->after_mons = GetMonEvolution(wk->dat->pp, mon, EVOCTX_LEVELUP, EVO_NONE, (int *)&wk->dat->shinka_cond);
    if (wk->dat->after_mons != SPECIES_NONE) {
        wk->dat->ret_mode = 0x9;
        return 0x20;
    }
    wk->dat->ret_mode = 0x0;
    if (Bag_HasItem(wk->dat->myitem, wk->dat->item, 1, HEAP_ID_PARTY_MENU)) {
        ClearFrameAndWindow2(&wk->windows[34], TRUE);
        PartyMenu_PrintMessageOnWindow32(wk, 33, TRUE); // message index in 300.txt
        return 0x4;
    }
    return 0x20;
}

/*
 * @brief hooks into the ending of pokeheartgold PartyMenu_ItemUseFunc_WaitTextPrinterThenExit
 * to allow for item reuse if not an evo item and the bag has more of the item
 */
int PartyMenu_ItemUseFunc_ReuseItem(struct PLIST_WORK *wk) {
    wk->dat->ret_mode = 0;
    if (GetItemData(wk->dat->item, ITEM_PARAM_EVOLUTION, HEAP_ID_PARTY_MENU) == 0 && Bag_HasItem(wk->dat->myitem, wk->dat->item, 1, HEAP_ID_PARTY_MENU)) {
        ClearFrameAndWindow2(&wk->windows[34], TRUE);
        PartyMenu_PrintMessageOnWindow32(wk, 33, TRUE); // message index in 300.txt
        return 0x4;
    }
    return 0x20;
}

void PartyMenu_LearnMoveToSlot(struct PLIST_WORK *partyMenu, struct PartyPokemon *mon, int moveIdx) {
    int data = partyMenu->dat->move;
    SetMonData(mon, MON_DATA_MOVE1 + moveIdx, &data);
    data = 0;
    SetMonData(mon, MON_DATA_MOVE1PPUP + moveIdx, &data);
    data = GetMoveMaxPP(partyMenu->dat->move, 0);
    SetMonData(mon, MON_DATA_MOVE1PP + moveIdx, &data);
    if (partyMenu->dat->item != ITEM_NONE) {
#ifdef REUSABLE_TMS
    BOOL consumeItem = IS_ITEM_TR(partyMenu->dat->item);
#else
    BOOL consumeItem = IS_ITEM_TM(partyMenu->dat->item) || IS_ITEM_TR(partyMenu->dat->item);
#endif // REUSABLE_TMS
        if (consumeItem) {
            Bag_TakeItem(partyMenu->dat->myitem, partyMenu->dat->item, 1, HEAP_ID_PARTY_MENU);
        }
        MonApplyFriendshipMod(mon, 4, PartyMenu_GetCurrentMapSec(partyMenu));
        ApplyMonMoodModifier(mon, 3);
    }
}
