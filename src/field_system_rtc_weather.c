#include "../include/bag.h"
#include "../include/rtc.h"
#include "../include/save.h"
#include "../include/script.h"
#include "../include/constants/badge.h"
#include "../include/constants/maps.h"
#include "../include/constants/weather_numbers.h"

const MonthDay DiamondDustDates[] = {
    { JANUARY,  1  },
    { JANUARY,  31 },
    { FEBRUARY, 1  },
    { FEBRUARY, 29 },
    { MARCH,    15 },
    { OCTOBER,  10 },
    { DECEMBER, 3  },
    { DECEMBER, 31 },
}; //_020FA09C

u32 Fsys_GetWeather_HandleDiamondDust(FieldSystem *fieldSystem, u32 mapId) {
    u8 c;
    u32 weatherType = MapHeader_GetWeatherType(mapId);
    SysInfo_RTC *sysinfo_rtc = Save_SysInfo_RTC_Get(fieldSystem->savedata);
    // Set per-header using the switch cases.
    // Lock to after a flag is set using: if(CheckScriptFlag(flag))
    // Lock to after a badge is obtained using: if(PlayerProfile_TestBadgeFlag(Sav2_PlayerData_GetProfileAddr(fieldSystem->savedata), badge))
    // Lock to when an item is held using: if (Bag_HasItem(Sav2_Bag_get(SaveBlock2_get()), item, 1, HEAPID_WORLD))
    switch (fieldSystem->location->mapId)
    {
        case MAP_MOUNT_SILVER_CAVE_SUMMIT: // MAP_D41R0108
            for (c = 0; c < NELEMS(DiamondDustDates); c++) 
            {
                u8 month = DiamondDustDates[c].month;
                u8 day = DiamondDustDates[c].day;
                if ((sysinfo_rtc->date.month == month) && (sysinfo_rtc->date.day == day)) 
                {
                    if (!FieldSystem_HasPenalty(fieldSystem)) 
                    {
                        weatherType = WEATHER_SYS_DIAMOND_DUST;
                    }
                    return weatherType;
                }
            }
            break;
        default: break;
    }
    
    return weatherType;
}