.nds
.thumb

.include "armips/include/battlescriptcmd.s"
.include "armips/include/abilities.s"
.include "armips/include/itemnums.s"
.include "armips/include/monnums.s"
.include "armips/include/movenums.s"

.create "build/move/battle_sub_seq/1_346", 0

a001_346:
    //literally just curse subscript but ignores magic guard
    setstatus2effect BATTLER_xFF, 0x1E
    waitmessage
    changemondatabyvar VAR_OP_GET_RESULT, BATTLER_xFF, 0x30, VAR_HP_TEMP
    damagediv 32, 4
    changevar VAR_OP_MUL, VAR_HP_TEMP, 0xFFFFFFFF
    printmessage 0x1A8, 0x2, 0xFF, "NaN", "NaN", "NaN", "NaN", "NaN"
    waitmessage
    wait 0x1E
    changevar VAR_OP_SETMASK, VAR_SERVER_STATUS1, 0x40
    jumptosubseq 2

.close