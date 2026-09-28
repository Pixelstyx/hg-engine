#include "constants/battle_constants.h"
.include "battle_commands.inc"

.data

_Start:
    CheckSubstitute BATTLER_CATEGORY_DEFENDER, _MoveFailed
    Call BATTLE_SUBSCRIPT_ATTACK_MESSAGE_AND_ANIMATION
    // Get current HP values.
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_ATTACKER, BMON_DATA_HP, BSCRIPT_VAR_CALC_TEMP
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_DEFENDER, BMON_DATA_HP, BSCRIPT_VAR_HP_CALC
    // Store the average in BSCRIPT_VAR_CALC_TEMP.
    UpdateVarFromVar OPCODE_ADD, BSCRIPT_VAR_CALC_TEMP, BSCRIPT_VAR_HP_CALC
    UpdateVar OPCODE_DIV, BSCRIPT_VAR_CALC_TEMP, 2
    CompareVarToValue OPCODE_FLAG_NOT, BSCRIPT_VAR_BATTLE_TYPE, BATTLE_TYPE_TOTEM, _AverageHP
    GoToIfTotem BATTLER_CATEGORY_DEFENDER, _CheckTotemDamageCap

_AverageHP:
    // Disable blinking because it's technically not damage.
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_BATTLE_STATUS, BATTLE_STATUS_NO_BLINK
    // Deal with attacker.
    UpdateVarFromVar OPCODE_SET, BSCRIPT_VAR_MSG_BATTLER_TEMP, BSCRIPT_VAR_BATTLER_ATTACKER
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_ATTACKER, BMON_DATA_HP, BSCRIPT_VAR_HP_CALC
    UpdateVarFromVar OPCODE_SUB, BSCRIPT_VAR_HP_CALC, BSCRIPT_VAR_CALC_TEMP
    UpdateVar OPCODE_MUL, BSCRIPT_VAR_HP_CALC, -1
    Call BATTLE_SUBSCRIPT_UPDATE_HP
    // Disable blinking again because it gets reset after each HP update.
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_BATTLE_STATUS, BATTLE_STATUS_NO_BLINK
    // Deal with defender.
    UpdateVarFromVar OPCODE_SET, BSCRIPT_VAR_MSG_BATTLER_TEMP, BSCRIPT_VAR_BATTLER_TARGET
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_DEFENDER, BMON_DATA_HP, BSCRIPT_VAR_HP_CALC
    UpdateVarFromVar OPCODE_SUB, BSCRIPT_VAR_HP_CALC, BSCRIPT_VAR_CALC_TEMP
    UpdateVar OPCODE_MUL, BSCRIPT_VAR_HP_CALC, -1
    Call BATTLE_SUBSCRIPT_UPDATE_HP

_PrintMessage:
    // The battlers shared their pain!
    PrintMessage 820, TAG_NONE
    Wait 
    WaitButtonABTime 30
    End 

_MoveFailed:
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_MOVE_STATUS_FLAGS, MOVE_STATUS_FAILED
    End 

// Make sure Totems only lose 1/4 of their max HP at most; set the attacker's HP gain to the same number.
_CheckTotemDamageCap:
    // Do the regular calculation for modification to the defender's HP.
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_DEFENDER, BMON_DATA_HP, BSCRIPT_VAR_HP_CALC
    UpdateVarFromVar OPCODE_SUB, BSCRIPT_VAR_HP_CALC, BSCRIPT_VAR_CALC_TEMP
    // Get 1/4 of the defender's max HP.
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_DEFENDER, BMON_DATA_MAXHP, BSCRIPT_VAR_CALC_TEMP
    UpdateVar OPCODE_DIV, BSCRIPT_VAR_CALC_TEMP, 4
    // If 1/4 of the defender's max HP is less than our difference, return to the main function.
    CompareVarToVar OPCODE_LTE, BSCRIPT_VAR_CALC_TEMP, BSCRIPT_VAR_HP_CALC, _AverageHP
    // Otherwise, set our difference to be 1/4th of the defender's max HP.
    UpdateVarFromVar OPCODE_SET, BSCRIPT_VAR_HP_CALC, BSCRIPT_VAR_CALC_TEMP
    // Deal with attacker.
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_BATTLE_STATUS, BATTLE_STATUS_NO_BLINK
    UpdateVarFromVar OPCODE_SET, BSCRIPT_VAR_MSG_BATTLER_TEMP, BSCRIPT_VAR_BATTLER_ATTACKER
    Call BATTLE_SUBSCRIPT_UPDATE_HP
    // Deal with defender.
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_BATTLE_STATUS, BATTLE_STATUS_NO_BLINK
    UpdateVarFromVar OPCODE_SET, BSCRIPT_VAR_MSG_BATTLER_TEMP, BSCRIPT_VAR_BATTLER_TARGET
    UpdateVar OPCODE_MUL, BSCRIPT_VAR_HP_CALC, -1
    Call BATTLE_SUBSCRIPT_UPDATE_HP
    GoTo _PrintMessage
