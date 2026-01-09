#ifndef ROCK_SMASH_ITEM_H
#define ROCK_SMASH_ITEM_H

// #include "field_system.h"

#define ROCK_SMASH_TABLE_DEFAULT        0
#define ROCK_SMASH_TABLE_RUINS_OF_ALPH  1
#define ROCK_SMASH_TABLE_CLIFF_CAVE     2

#define NUM_ROCK_SMASH_TABLES           3

typedef struct {
    u16 odds;
    u16 table;
} ROCK_SMASH;

typedef struct {
    BOOL followMonKnowsHM;
    int ability;
    u16 *itemFound; // TODO: bool16
    u16 *item;
    ROCK_SMASH rockSmash;
    u32 unk14;
} RockSmashItemCheckWork;

static BOOL CheckRockSmashItemDrop(FieldSystem *fieldSystem, RockSmashItemCheckWork *env);

// TODO: Move this somewhere better.
void ReadWholeNarcMemberByIdPair(void *dest, u32 narc_id, s32 file_id);

#endif // ROCK_SMASH_ITEM_H
