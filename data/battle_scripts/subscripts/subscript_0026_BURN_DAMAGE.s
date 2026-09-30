#include "constants/battle_constants.h"
.include "battle_commands.inc"

.data

_Start:
    CheckAbility CHECK_OPCODE_HAVE, BATTLER_CATEGORY_MSG_TEMP, ABILITY_MAGIC_GUARD, _End
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_MAXHP, BSCRIPT_VAR_HP_CALC
#ifdef TOTEM_FIXED_DAMAGE_REDUCTION
    CompareVarToValue OPCODE_FLAG_NOT, BSCRIPT_VAR_BATTLE_TYPE, BATTLE_TYPE_TOTEM, _RegularDivide
    GoToIfTotem BATTLER_CATEGORY_DEFENDER, _TotemDivide
#endif

_RegularDivide:
    DivideVarByValue BSCRIPT_VAR_HP_CALC, 16

_CheckHeatproof:
    CheckAbility CHECK_OPCODE_NOT_HAVE, BATTLER_CATEGORY_MSG_TEMP, ABILITY_HEATPROOF, _DealDamage
    DivideVarByValue BSCRIPT_VAR_HP_CALC, 2

_DealDamage:
    UpdateVar OPCODE_MUL, BSCRIPT_VAR_HP_CALC, -1
    // {0} is hurt by its burn!
    PrintMessage 95, TAG_NICKNAME, BATTLER_CATEGORY_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    PlayBattleAnimation BATTLER_CATEGORY_MSG_TEMP, BATTLE_ANIMATION_BURNED
    Wait 
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_BATTLE_STATUS, BATTLE_STATUS_NO_BLINK
    GoToSubscript BATTLE_SUBSCRIPT_UPDATE_HP
    Call BATTLE_SUBSCRIPT_SWITCH_IN_ABILITY_CHECK

_End:
    End 

_TotemDivide:
    DivideVarByValue BSCRIPT_VAR_HP_CALC, 32
    GoTo _CheckHeatproof