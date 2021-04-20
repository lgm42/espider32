#pragma once

#define DEFAULT_LED     16

// Version
#define VERSION                             "V1.0.0"

#define NTP_UPDATE_INTERVAL_MSEC     (1 * 3600 * 1000)      // Update time from NTP server every 1 hour
// Timezone
#define UTC_OFFSET      +1

// change for different ntp (time servers)
#define NTP_SERVERS "0.fr.pool.ntp.org", "time.nist.gov", "pool.ntp.org"

#define NUMBER_OF_LEGS      4

#define USE_ESP_IDF_LOG