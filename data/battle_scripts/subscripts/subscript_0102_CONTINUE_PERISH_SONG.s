#include "constants/battle_constants.h"
.include "battle_commands.inc"

.data

_Start:
    // {0}’s perish count fell to {1}!
    PrintMessage 439, TAG_NICKNAME_NUM, BATTLER_CATEGORY_MSG_TEMP, BATTLER_CATEGORY_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    CompareMonDataToValue OPCODE_FLAG_SET, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_MOVE_EFFECT, MOVE_EFFECT_FLAG_PERISH_SONG, _End
    CompareVarToValue OPCODE_FLAG_SET, BSCRIPT_VAR_BATTLE_TYPE, BATTLE_TYPE_TOTEM, _CheckTotemThreshold

_FaintMon:
    Call BATTLE_SUBSCRIPT_UPDATE_HP

_End:
    End

_CheckTotemThreshold:
    // Check if we are at the execution threshold (25% HP or less).
    // This is slightly imprecise due to integer division.
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_DEFENDER, BMON_DATA_MAX_HP, BSCRIPT_VAR_HP_CALC
    DivideVarByValue BSCRIPT_VAR_HP_CALC, 4
    CompareMonDataToVar OPCODE_LTE, BATTLER_CATEGORY_DEFENDER, BMON_DATA_HP, BSCRIPT_VAR_HP_CALC, _FaintMon
    // TODO: Get correct move name.
    // {0} was too strong to be taken down by {1}!
    PrintMessage BATTLE_MSG_TOTEM_OHKO_IMMUNITY, TAG_NICKNAME_MOVE, BATTLER_CATEGORY_DEFENDER, BATTLER_CATEGORY_ATTACKER
    Wait 
    WaitButtonABTime 30
    End