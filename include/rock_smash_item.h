#ifndef ROCK_SMASH_ITEM_H
#define ROCK_SMASH_ITEM_H

#include "battle.h"

#define ROCK_SMASH_TABLE_DEFAULT        0
#define ROCK_SMASH_TABLE_RUINS_OF_ALPH  1
#define ROCK_SMASH_TABLE_CLIFF_CAVE     2

#define NUM_ROCK_SMASH_TABLES           3

void LONG_CALL FieldSystem_RockSmashItemCheck(FieldSystem *fieldSystem, int followMonKnowsHm, u16 *itemFound, u16 *item);

// TODO: Move this somewhere better (filesystem.h does not exist yet).
void LONG_CALL ReadWholeNarcMemberByIdPair(void *dest, u32 narc_id, s32 file_id);

// TODO: Move this somewhere better (overlay_02.h does not exist).
BOOL LONG_CALL ov02_022470A0(FieldSystem *fieldSystem, struct BattleSetup **setupPtr);

// TODO: Move this somewhere better (encounter.h does not exist).
void LONG_CALL FieldSystem_StartForcedWildBattle(FieldSystem *fieldSystem, TaskManager *taskManager, struct BattleSetup *setup);

// TODO: Move this somewhere better (overlay_01_022001E4.h does not exist).
int LONG_CALL ov01_02200540(LocalMapObject *obj, int a0, int a1);
BOOL LONG_CALL ov01_022003F4(int a0);

// TODO: Move this somewhere better (unk_020689C8.h does not exist).
void LONG_CALL sub_02068B48(int a0);

#endif // ROCK_SMASH_ITEM_H
