#include "constants/battle_constants.h"
#include "constants/battle_message_constants.h"
.include "battle_commands.inc"

.data

_Start:
    CompareVarToValue OPCODE_EQU, BSCRIPT_VAR_TEMP_DATA, 0, _HandleTempestMove
    CompareVarToValue OPCODE_EQU, BSCRIPT_VAR_TEMP_DATA, 2, _HandleTempestBuildup_Strong

_HandleTempestBuildup:
    // A wicked gust starts to stir!
    PrintMessage BATTLE_MSG_TOTEM_TEMPEST_WICKED_GUST, TAG_NONE
    // UpdateVar OPCODE_SET, BSCRIPT_VAR_MSG_MOVE_TEMP, MOVE_TAILWIND
    // PlayMoveAnimation BATTLER_CATEGORY_MSG_TEMP
    Wait
    WaitButtonABTime 30
    End

_HandleTempestBuildup_Strong:
    // A horrible wind is howling!
    PrintMessage BATTLE_MSG_TOTEM_TEMPEST_HOWLING_WIND, TAG_NONE
    // UpdateVar OPCODE_SET, BSCRIPT_VAR_MSG_MOVE_TEMP, MOVE_TAILWIND
    // PlayMoveAnimation BATTLER_CATEGORY_MSG_TEMP
    Wait
    WaitButtonABTime 30
    End

_HandleTempestMove:
    UpdateVar OPCODE_SET, BSCRIPT_VAR_MSG_MOVE_TEMP, MOVE_HURRICANE
    PlayMoveAnimation BATTLER_CATEGORY_MSG_TEMP
    // A terrible storm tears into you!
    PrintMessage BATTLE_MSG_TOTEM_TEMPEST_TERRIBLE_STORM, TAG_NONE
    Wait
    WaitButtonABTime 30
    End
