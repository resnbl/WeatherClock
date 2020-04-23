#ifndef _CLOCK_H_
#define _CLOCK_H_

/*  A Clock object initilizes the internal clock mechanism from an NTP service on
    the net and provides query functions. Alarm/timer functions are available
    that provide "second accuracy" (for finer control use something else).
  
    NTP service derived from:
    https://github.com/scanlime/esp8266-Arduino/blob/master/tests/Time/Time.ino

    Timezone names are available in TZ.h in the ESP8266 libraries.
 */

#include <Arduino.h>
#include <sys/time.h>
#include <TZ.h>

// Default to debug output
#ifndef TS_PRT_SYNCS
#define TS_PRT_SYNCS 1
#endif

// Useful storage types and definitions
typedef int16_t mins_t;       // can hold (+|-) # mins since midnight
typedef int32_t secs_t;       // can hold (+|-) # secs since midnight
#define SEC_MSECS 1000
#define MIN_SECS 60
#define MIN_MSECS (MIN_SECS * SEC_MSECS)
#define HR_MINS 60
#define HR_SECS (HR_MINS * MIN_SECS)
#define HM_MINS(h,m) (h*HR_MINS + m)
#define DAY_HRS 24
#define DAY_MINS (DAY_HRS * HR_MINS)
#define DAY_SECS (DAY_HRS * HR_MINS * MIN_SECS)

class Clock {
  public:
    // Initialize timezone and NTP service pool name
    void begin(const char* tz, const char* poolName);
    void begin_P(PGM_P tzP, PGM_P poolP);   // PROGMEM version
    
    // Check for sync with NTP server
    bool isSynced();
    const char* getLastSync();

    // Fetch current date/time
    bool getTime(tm &tms);

    // Return # seconds since midnight
    secs_t daySecs();
    // Return time as string "HH:MM:SS" (24-hr)
    const char* hmsStr(secs_t secs);
    // Fetch time as string "HH:MM", (12-hr, alternating colon)
    void hm12Str(char* buff, tm &tms);
    // Fetch date as string "Wkd Mth dd"
    void dateStr(char* datBuff, tm& tms);
  
  private:
    time_t lastSync;     // when last NTP sync occurred
    volatile bool timeSynced = false;

    void handleSync();

};

#endif
