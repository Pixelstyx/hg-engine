#include "../include/constants/maps.h"
#include "../include/constants/species.h"
#include "../include/encounter.h"

#define SWARM_MAP_COUNT 20

#define SWARM_GRASS   0
#define SWARM_SURFING 1
#define SWARM_FISHING 2

void LONG_CALL WildEncounters_ReadFromNarc(EncounterData *encData, u32 mapno);

const u16 sSwarmMapLUT[SWARM_MAP_COUNT][2] = {
<<<<<<< HEAD
    {MAP_ROUTE_1,                   SWARM_GRASS},
    {MAP_ROUTE_3,                   SWARM_GRASS},
    {MAP_ROUTE_9,                   SWARM_GRASS},
    {MAP_ROUTE_12,                  SWARM_FISHING},
    {MAP_ROUTE_13,                  SWARM_GRASS},
    {MAP_ROUTE_19,                  SWARM_SURFING},
    {MAP_ROUTE_32,                  SWARM_FISHING},
    {MAP_ROUTE_25,                  SWARM_GRASS},
    {MAP_ROUTE_27,                  SWARM_SURFING},
    {MAP_ROUTE_34,                  SWARM_GRASS},
    {MAP_ROUTE_35,                  SWARM_GRASS},
    {MAP_ROUTE_38,                  SWARM_GRASS},
    {MAP_ROUTE_44,                  SWARM_FISHING},
    {MAP_ROUTE_45,                  SWARM_GRASS},
    {MAP_ROUTE_47,                  SWARM_GRASS},
    {MAP_MOUNT_MORTAR_1F_ENTRANCE,  SWARM_GRASS},
    {MAP_DARK_CAVE_ROUTE_31_SIDE,   SWARM_GRASS},
    {MAP_VIRIDIAN_FOREST,           SWARM_GRASS},
    {MAP_VERMILION,                 SWARM_SURFING},
    {MAP_VIOLET,                    SWARM_FISHING},
=======
    { MAP_R01, SWARM_GRASS },
    { MAP_R03, SWARM_GRASS },
    { MAP_R09, SWARM_GRASS },
    { MAP_R12, SWARM_FISHING },
    { MAP_R13, SWARM_GRASS },
    { MAP_W19, SWARM_SURFING },
    { MAP_R32, SWARM_FISHING },
    { MAP_R25, SWARM_GRASS },
    { MAP_R27, SWARM_SURFING },
    { MAP_R34, SWARM_GRASS },
    { MAP_R35, SWARM_GRASS },
    { MAP_R38, SWARM_GRASS },
    { MAP_R44, SWARM_FISHING },
    { MAP_R45, SWARM_GRASS },
    { MAP_R47, SWARM_GRASS },
    { MAP_D38R0101, SWARM_GRASS },
    { MAP_D42R0102, SWARM_GRASS },
    { MAP_D46R0101, SWARM_GRASS },
    { MAP_T06, SWARM_SURFING },
    { MAP_T22, SWARM_FISHING },
>>>>>>> upstream/main
};

BOOL LONG_CALL sub_02097F6C(u32 rand, u16 mapno, u16 swarmValue)
{
    u8 offset = rand % SWARM_MAP_COUNT;

    if (mapno == sSwarmMapLUT[offset][0]) {
        if (swarmValue == sSwarmMapLUT[offset][1]) {
            return TRUE;
        }
    }

    return FALSE;
}

void LONG_CALL GetSwarmInfoFromRand(u32 rand, u16 *mapno, u16 *species)
{
    EncounterData encData;
    const u16 *swarmMap = sSwarmMapLUT[rand % SWARM_MAP_COUNT];

    WildEncounters_ReadFromNarc(&encData, swarmMap[0]);

    switch (swarmMap[1]) {
    case SWARM_SURFING:
        *species = encData.surfSwarm;
        break;
    case SWARM_FISHING:
        *species = encData.fishSwarm;
        break;
    default:
        *species = encData.landSwarm;
        break;
    }

    GF_ASSERT(*species != SPECIES_NONE);
    *mapno = swarmMap[0];
}
