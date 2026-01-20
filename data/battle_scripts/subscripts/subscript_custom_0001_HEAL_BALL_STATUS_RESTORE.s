.include "asm/include/battle_commands.inc"

.data

_Start:
    CompareMonDataToValue OPCODE_FLAG_NOT, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_STATUS2, STATUS2_CONFUSION, _CheckPrimaryStatus
    UpdateMonData OPCODE_FLAG_OFF, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_STATUS2, STATUS2_CONFUSION
    // {0} snapped out of confusion!
    PrintMessage 153, TAG_NICKNAME, BATTLER_CATEGORY_MSG_TEMP
    Wait 
    WaitButtonABTime 30

_CheckPrimaryStatus:
    CompareMonDataToValue OPCODE_FLAG_SET, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_STATUS, STATUS_NONE, _End
    UpdateMonData OPCODE_SET, BATTLER_CATEGORY_MSG_TEMP, BMON_DATA_STATUS, STATUS_NONE
    // {0}’s status returned to normal!
    PrintMessage 491, TAG_NICKNAME, BATTLER_CATEGORY_MSG_TEMP
    Wait 
    SetHealthbarStatus BATTLER_CATEGORY_MSG_TEMP, BATTLE_ANIMATION_NONE
    WaitButtonABTime 30

_End:
    End
    