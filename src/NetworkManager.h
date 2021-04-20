#pragma once

#include <ESP_WiFiManager.h>
#include <Arduino.h>

class NetworkManager
{
	public:
        NetworkManager();
        virtual ~NetworkManager();
		void setup();
		void handle();
    private:
        void updateNTP();
        static void wiFiEvent(system_event_id_t event);
        unsigned long _tickNTPUpdate;
};

#if !defined(NO_GLOBAL_INSTANCES)
extern NetworkManager Network;
#endif
