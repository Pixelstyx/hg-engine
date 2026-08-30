#include "constants/battle_constants.h"
.include "battle_commands.inc"

.data

_Start:
    CompareVarToValue OPCODE_EQU, BSCRIPT_VAR_TEMP_DATA, 0, _HandleTempestMove
    CompareVarToValue OPCODE_EQU, BSCRIPT_VAR_TEMP_DATA, 2, _HandleTempestBuildup_Strong

_HandleTempestBuildup:
    // A wicked gust starts to stir!
    PrintMessage 1782, TAG_NONE
    // UpdateVar OPCODE_SET, BSCRIPT_VAR_MSG_MOVE_TEMP, MOVE_TAILWIND
    // PlayMoveAnimation BATTLER_CATEGORY_MSG_TEMP
    Wait
    WaitButtonABTime 30
    End

_HandleTempestBuildup_Strong:
    // A horrible wind is howling!
    PrintMessage 1783, TAG_NONE
    // UpdateVar OPCODE_SET, BSCRIPT_VAR_MSG_MOVE_TEMP, MOVE_TAILWIND
    // PlayMoveAnimation BATTLER_CATEGORY_MSG_TEMP
    Wait
    WaitButtonABTime 30
    End

_HandleTempestMove:
    UpdateVar OPCODE_SET, BSCRIPT_VAR_MSG_MOVE_TEMP, MOVE_HURRICANE
    PlayMoveAnimation BATTLER_CATEGORY_MSG_TEMP
    // A terrible storm tears into you!
    PrintMessage 1784, TAG_NONE
    Wait
    WaitButtonABTime 30
/*
    // The roaring winds extinguished the burning Pokémon!
    PrintMessage 1785, TAG_NONE*/
    End
