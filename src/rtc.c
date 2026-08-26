#include "../include/rtc.h"
#include "../include/save.h"
#include "../include/script.h"

// Altered to account for timescale config and decoupled from RTC.
s32 GF_RTC_TimeToSec(void) {
    struct IGT *playTime = Save_PlayerData_GetIGTAddr(gFieldSysPtr->savedata);
    u32 playTimeSeconds = 60 * playTime->minutes + 3600 * (playTime->hours + STARTING_HOUR) + playTime->seconds;
    playTimeSeconds = (playTimeSeconds * DAY_TIMESCALE) / 100;
    playTimeSeconds %= 86400;
    // debug_printf("Timescaled seconds today: %d\n", playTimeSeconds);
    // debug_printf("Timescaled hour today: %d\n\n", playTimeSeconds / 3600);
    return (((60 * playTime->minutes + 3600 * (playTime->hours + STARTING_HOUR) + playTime->seconds) * DAY_TIMESCALE) / 100) % 86400;
}

// Return a time slot according to the timescaled hour.
TIMEOFDAY GF_RTC_GetTimeOfDayByHour(UNUSED s32 hour) {
    s32 scaledHour = GF_RTC_TimeToSec() / 3600;

    if (scaledHour < 4)
    {
        return (TIMEOFDAY)RTC_TIMEOFDAY_LATE;
    }
    else if (scaledHour < 10)
    {
        return (TIMEOFDAY)RTC_TIMEOFDAY_MORN;
    }
    else if (scaledHour < 17)
    {
        return (TIMEOFDAY)RTC_TIMEOFDAY_DAY;
    }
    else if (scaledHour < 20)
    {
        return (TIMEOFDAY)RTC_TIMEOFDAY_EVE;
    }
    
    return (TIMEOFDAY)RTC_TIMEOFDAY_NITE;
}
