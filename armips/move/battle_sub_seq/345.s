.nds
.thumb

.include "armips/include/battlescriptcmd.s"
.include "armips/include/abilities.s"
.include "armips/include/itemnums.s"
.include "armips/include/monnums.s"
.include "armips/include/movenums.s"

.create "build/move/battle_sub_seq/1_345", 0

a001_345:
    printmessage 1388, TAG_NONE, "NaN", "NaN", "NaN", "NaN", "NaN", "NaN" //The Curse begins to take its toll...
    waitmessage
    wait 0x1E
    endscript

.close