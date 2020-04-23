#include "Clock.h"
#include <time.h>
#include <coredecls.h>

#define ABRV_LEN 3
const char WKDAYS[] = "SunMonTueWedThuFriSat";
const char MONS[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

void Clock::begin(const char* tz, const char* poolName) {
  // Set optional callback function to be invoked after querying the NTP server
  settimeofday_cb(std::bind(&Clock::handleSync, this));

  Serial.print(F("NTP pool: ")); Serial.println(poolName);
  Serial.print(F("TZ: ")); Serial.println(tz);
  configTime(tz, poolName);
}

void Clock::begin_P(PGM_P tzP, PGM_P poolP) {
  char tzBuff[64], poolBuff[64];

  strncpy_P(tzBuff, tzP, sizeof(tzBuff));
  tzBuff[sizeof(tzBuff) - 1] = 0;
  strncpy_P(poolBuff, poolP, sizeof(poolBuff));
  poolBuff[sizeof(poolBuff) - 1] = 0;
  begin(tzBuff, poolBuff);
}

void Clock::handleSync() {
  timeSynced = true;
  lastSync = time(nullptr);

#if TS_PRT_SYNCS
  Serial.print(F("Time sync:\t"));
  Serial.print(getLastSync());
#endif
}

bool Clock::isSynced() {
  return timeSynced;
}

const char* Clock::getLastSync() {
  return asctime(localtime(&lastSync));
}

bool Clock::getTime(tm &tms) {
  if (timeSynced) {
    time_t tt = time(nullptr);
    localtime_r(&tt, &tms);
  }
  return timeSynced;
}

secs_t Clock::daySecs() {
  struct tm tms;
 
  getTime(tms);
  return (tms.tm_hour * 60 + tms.tm_min) * 60 + tms.tm_sec;
}

const char* Clock::hmsStr(secs_t secs) {
  static char hmsBuff[12];    // "hh:mm:ss\0"
  char* p = hmsBuff;
  int8_t hr = secs / HR_SECS;
  int8_t min = (secs / MIN_SECS) % HR_MINS;
  int8_t sec = secs % MIN_SECS;

  *p++ = hr / 10 + '0';
  *p++ = hr % 10 + '0';
  *p++ = ':';
  *p++ = min / 10 + '0';
  *p++ = min % 10 + '0';
  *p++ = ':';
  *p++ = sec / 10 + '0';
  *p++ = sec % 10 + '0';
  *p = 0;

  return hmsBuff;
}

void Clock::hm12Str(char* hmBuff, tm &tms) {
  char* p = hmBuff;
 
  if (tms.tm_hour > 12)
    tms.tm_hour -= 12;
  else if (tms.tm_hour == 0)
    tms.tm_hour = 12;
  
  *p++ = tms.tm_hour < 10 ? ' ' : '1';
  *p++ = tms.tm_hour % 10 + '0';
  *p++ = tms.tm_sec & 1 ? ':' : ' ';    // colon on odd secs, off even secs
  *p++ = tms.tm_min / 10 + '0';
  *p++ = tms.tm_min % 10 + '0';
  *p = 0;
}

void Clock::dateStr(char* datBuff, tm &tms) {
  char* p = datBuff;

  memcpy(p, &WKDAYS[tms.tm_wday * ABRV_LEN], ABRV_LEN);
  p += ABRV_LEN;
  *p++ = ' ';
	memcpy(p, &MONS[tms.tm_mon * ABRV_LEN], ABRV_LEN);
	p += ABRV_LEN;
	*p++ = ' ';
	itoa(tms.tm_mday, p, 10);
}
