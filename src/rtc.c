#include "../include/rtc.h"
#include "../include/save.h"
#include "../include/script.h"

s32 GF_RTC_TimeToSec(void) {
    struct RTCTime time;
    GF_RTC_CopyTime(&time);
    return ((60 * time.minute + 3600 * time.hour + time.second) * DAY_TIMESCALE) % 86400;
}

TIMEOFDAY GF_RTC_GetTimeOfDayByHour(s32 hour) {
    // Return a time slot according to the current RTC hour, including timescale.
    if (hour < (4 * DAY_TIMESCALE) % 24)
    {
        return (TIMEOFDAY)RTC_TIMEOFDAY_LATE;
    }
    else if (hour < (10 * DAY_TIMESCALE) % 24)
    {
        return (TIMEOFDAY)RTC_TIMEOFDAY_MORN;
    }
    else if (hour < (17 * DAY_TIMESCALE) % 24)
    {
        return (TIMEOFDAY)RTC_TIMEOFDAY_DAY;
    }
    else if (hour < (20 * DAY_TIMESCALE) % 24)
    {
        return (TIMEOFDAY)RTC_TIMEOFDAY_EVE;
    }
    
    return (TIMEOFDAY)RTC_TIMEOFDAY_NITE;
}
