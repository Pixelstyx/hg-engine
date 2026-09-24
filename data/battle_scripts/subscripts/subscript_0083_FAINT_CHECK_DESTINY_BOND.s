#include "constants/battle_constants.h"
.include "battle_commands.inc"

.data

_Start:
    CompareMonDataToValue OPCODE_FLAG_NOT, BATTLER_CATEGORY_FAINTED_MON, BMON_DATA_STATUS2, STATUS2_DESTINY_BOND, _CheckGrudge
    IfSameSide BATTLER_CATEGORY_ATTACKER, BATTLER_CATEGORY_FAINTED_MON, _CheckGrudge
    CompareMonDataToValue OPCODE_EQU, BATTLER_CATEGORY_ATTACKER, BMON_DATA_HP, 0, _CheckGrudge
    CompareVarToValue OPCODE_FLAG_SET, BSCRIPT_VAR_BATTLE_TYPE, BATTLE_TYPE_TOTEM, _CheckTotemThreshold

_HandleDestinyBond:
    // {0} took {1} down with it!
    PrintMessage 391, TAG_NICKNAME_NICKNAME, BATTLER_CATEGORY_FAINTED_MON, BATTLER_CATEGORY_ATTACKER
    UpdateVarFromVar OPCODE_GET, BSCRIPT_VAR_BATTLER_FAINTED, BSCRIPT_VAR_LAST_BATTLER_ID
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_ATTACKER, BMON_DATA_HP, BSCRIPT_VAR_HP_CALC
    UpdateVar OPCODE_MUL, BSCRIPT_VAR_HP_CALC, -1
    UpdateVarFromVar OPCODE_SET, BSCRIPT_VAR_MSG_BATTLER_TEMP, BSCRIPT_VAR_BATTLER_ATTACKER
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_BATTLE_STATUS, BATTLE_STATUS_NO_BLINK
    Call BATTLE_SUBSCRIPT_UPDATE_HP
    PlayFaintAnimation 
    Wait 
    HealthbarSlideOut BATTLER_CATEGORY_FAINTED_MON
    // {0} fainted!
    PrintMessage 30, TAG_NICKNAME, BATTLER_CATEGORY_FAINTED_MON
    Wait 
    WaitButtonABTime 30
    IncrementGameStat BATTLER_CATEGORY_FAINTED_MON, BATTLER_TYPE_SOLO_ENEMY, 42
    IncrementGameStat BATTLER_CATEGORY_FAINTED_MON, BATTLER_TYPE_SOLO_PLAYER, 97
    UpdateVarFromVar OPCODE_GET, BSCRIPT_VAR_LAST_BATTLER_ID, BSCRIPT_VAR_BATTLER_FAINTED

_CheckGrudge:
    TryGrudge _FaintMon
    // {0}’s {1} lost all its PP due to the grudge!
    PrintMessage 568, TAG_NICKNAME_MOVE, BATTLER_CATEGORY_ATTACKER, BATTLER_CATEGORY_MSG_TEMP
    Wait 
    WaitButtonABTime 30

_FaintMon:
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_BATTLE_STATUS_2, BATTLE_STATUS2_NO_EXP_GAINED
    Call BATTLE_SUBSCRIPT_FAINT_MON
    UpdateVar OPCODE_FLAG_OFF, BSCRIPT_VAR_BATTLE_STATUS_2, BATTLE_STATUS2_NO_EXP_GAINED
    End 

_CheckTotemThreshold:
    // TODO: Check if defender slot contains a valid Totem Pokemon. Needs a custom macro.
    // Skipping for now since Totems probably won't ever use Destiny Bond, but SOS/adds that can would be affected.

    // Check if we are at the execution threshold (25% HP or less).
    // This is slightly imprecise due to integer division.
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_DEFENDER, BMON_DATA_MAX_HP, BSCRIPT_VAR_HP_CALC
    DivideVarByValue BSCRIPT_VAR_HP_CALC, 4
    CompareMonDataToVar OPCODE_LTE, BATTLER_CATEGORY_DEFENDER, BMON_DATA_HP, BSCRIPT_VAR_HP_CALC, _HandleDestinyBond
    // {0} was too strong to be taken down by {1}!
    PrintMessage BATTLE_MSG_TOTEM_OHKO_IMMUNITY, TAG_NICKNAME_MOVE, BATTLER_CATEGORY_DEFENDER, BATTLER_CATEGORY_ATTACKER
    Wait 
    WaitButtonABTime 30
    GoTo _FaintMon
