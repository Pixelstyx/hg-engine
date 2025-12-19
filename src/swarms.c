#include "../include/types.h"
#include "../include/constants/maps.h"

#define SWARM_MAP_COUNT 20

#define SWARM_GRASS     0
#define SWARM_SURFING   1
#define SWARM_FISHING   2

const u16 sSwarmMapLUT[SWARM_MAP_COUNT][2] = {
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
};
