#include "constants/battle_constants.h"
#include "constants/battle_message_constants.h"
.include "battle_commands.inc"

.data

_Start:
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_FANCY_APPLE, _CrispMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_COMET_SHARD, _GlitteringMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_HEART_SCALE, _GlitteringMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_CASTELIACONE, _MeltingMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_TOXIC_ORB, _CautiousMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_POISON_BARB, _CautiousMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_SNOWBALL, _OutOfSeasonMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_LIGHT_BALL, _BrightMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_LEEK, _LeakMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_UTILITY_UMBRELLA, _WeatherMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_HEAT_ROCK, _WarmMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_DRAGON_FANG, _PointyMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_KINGS_ROCK, _PointyMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_TIN_OF_BEANS, _WellChewedMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_ODD_KEYSTONE, _SurprisingMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_THICK_CLUB, _HeftyMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_RARE_BONE, _HeftyMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_HARD_STONE, _HeftyMessage
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_HELD_ITEM, ITEM_IRON_BALL, _VeryHeavyMessage

_DefaultMessage:
    // An Aipom hands {0} a {1}!
    PrintMessage BATTLE_MSG_PARK_PICKUP_DEFAULT, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_CrispMessage:
    // An Aipom hands {0} a perfectly crisp {1}!
    PrintMessage BATTLE_MSG_PARK_PICKUP_CRISP, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_GlitteringMessage:
    // An Aipom hands {0} a glittering {1}!
    PrintMessage BATTLE_MSG_PARK_PICKUP_GLITTERING, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_MeltingMessage:
    // An Aipom hands {0} a {1}! Hurry, or it’ll melt!
    PrintMessage BATTLE_MSG_PARK_PICKUP_MELTING, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_CautiousMessage:
    // An Aipom cautiously hands {0} a {1}!
    PrintMessage BATTLE_MSG_PARK_PICKUP_CAUTIOUS, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_OutOfSeasonMessage:
    // An Aipom hands {0} a {1}! At this time of year?
    PrintMessage BATTLE_MSG_PARK_PICKUP_OUT_OF_SEASON, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_BrightMessage:
    // An Aipom hands {0} a blindingly bright {1}!
    PrintMessage BATTLE_MSG_PARK_PICKUP_BRIGHT, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_LeakMessage:
    // An Aipom hands {0} a {1}! Watch your step!
    PrintMessage BATTLE_MSG_PARK_PICKUP_CAUTIOUS, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_WeatherMessage:
    // An Aipom hands {0} a {1}! How’s the weather over there?
    PrintMessage BATTLE_MSG_PARK_PICKUP_WEATHER, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_WarmMessage:
    // An Aipom hands {0} a pleasantly warm {1}!
    PrintMessage BATTLE_MSG_PARK_PICKUP_WARM, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_PointyMessage:
    // An Aipom hands {0} a {1}! Pointy!
    PrintMessage BATTLE_MSG_PARK_PICKUP_POINTY, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_WellChewedMessage:
    // An Aipom hands {0} a well-chewed {1}!
    PrintMessage BATTLE_MSG_PARK_PICKUP_WELL_CHEWED, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_SurprisingMessage:
    // An Aipom hands {0} an... {1}? Where did they get that?!
    PrintMessage BATTLE_MSG_PARK_PICKUP_SURPRISING, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_HeftyMessage:
    // An Aipom hands {0} a hefty {1}!
    PrintMessage BATTLE_MSG_PARK_PICKUP_HEFTY, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    GoTo _Cleanup

_VeryHeavyMessage:
    // An Aipom, with considerable strain, hoists an {1} into {0}’s hands!
    PrintMessage BATTLE_MSG_PARK_PICKUP_VERY_HEAVY, TAG_NICKNAME_ITEM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP

_Cleanup: 
    Wait 
    WaitButtonABTime 30
    End
