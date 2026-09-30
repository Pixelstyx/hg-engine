#include "config.h"
#include "constants/battle_constants.h"
.include "battle_commands.inc"

.data

_Start:
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_BATTLE_STATUS, BATTLE_STATUS_IGNORE_TYPE_EFFECTIVENESS
    UpdateMonDataFromVar OPCODE_GET, BATTLER_CATEGORY_DEFENDER, BMON_DATA_HP, BSCRIPT_VAR_DAMAGE
    UpdateVar OPCODE_MUL, BSCRIPT_VAR_DAMAGE, -1
#ifdef TOTEM_FIXED_DAMAGE_REDUCTION
    CompareVarToValue OPCODE_FLAG_NOT, BSCRIPT_VAR_BATTLE_TYPE, BATTLE_TYPE_TOTEM, _RegularDivide
    GoToIfTotem BATTLER_CATEGORY_DEFENDER, _TotemDivide
#endif

_RegularDivide:
    DivideVarByValue BSCRIPT_VAR_DAMAGE, 4
    UpdateVar OPCODE_MUL, BSCRIPT_VAR_DAMAGE, 3
    End

_TotemDivide:
    DivideVarByValue BSCRIPT_VAR_DAMAGE, 8
    UpdateVar OPCODE_MUL, BSCRIPT_VAR_DAMAGE, 3
    End