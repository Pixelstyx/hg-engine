#include "../include/rtc.h"
#include "../include/save.h"
#include "../include/script.h"

// Altered to account for timescale config.
s32 GF_RTC_TimeToSec(void) {
    struct RTCTime time;
    GF_RTC_CopyTime(&time);
    return (((60 * time.minute + 3600 * time.hour + time.second) * DAY_TIMESCALE) / 100) % 86400;
}

// Return a time slot according to the timescaled hour.
TIMEOFDAY GF_RTC_GetTimeOfDayByHour(UNUSED s32 hour) {
    s32 scaledHour = (GF_RTC_TimeToSec() / 60) / 24;

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
