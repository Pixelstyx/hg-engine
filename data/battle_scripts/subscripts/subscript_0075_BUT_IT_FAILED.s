#include "constants/battle_constants.h"
#include "constants/battle_message_constants.h"
.include "battle_commands.inc"

.data

_Start:
    // If the attacker's most recent move was Psych Up or Heart Swap, the only way it can fail is due to Totem shenanigans.
    CompareVarToValue OPCODE_EQU, BSCRIPT_VAR_MOVE_NO_CUR, MOVE_PSYCH_UP, _TotemMessage
    CompareVarToValue OPCODE_EQU, BSCRIPT_VAR_MOVE_NO_CUR, MOVE_HEART_SWAP, _TotemMessage
    // But it failed!
    PrintMessage BATTLE_MSG_BUT_IT_FAILED, TAG_NONE
    GoTo _Cleanup

_TotemMessage:
    PrintMessage BATTLE_MSG_TOTEM_STAT_COPY_IMMUNITY, TAG_NICKNAME_NICKNAME, BATTLER_CATEGORY_ATTACKER, BATTLER_CATEGORY_DEFENDER

_Cleanup:
    Wait 
    WaitButtonABTime 30
    UpdateVar OPCODE_FLAG_ON, BSCRIPT_VAR_MOVE_STATUS_FLAGS, MOVE_STATUS_NO_MORE_WORK
    End 
