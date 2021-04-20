#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#include "Logger.h"
#include "EEPROMConfiguration.h"
#include "contants.h"
#include "HttpServer.h"

#include "NetworkManager.h"

NetworkManager::NetworkManager()  : _tickNTPUpdate(0) 
{
}

NetworkManager::~NetworkManager() 
{}

void NetworkManager::wiFiEvent(system_event_id_t event)
{
    Log.println("wifi event : " + String(event));
}

void NetworkManager::setup()
{
	WiFi.onEvent(wiFiEvent);

	ESP_WiFiManager wm;
	// wm.setDebugOutput(false);
	// wm.resetSettings();

	// WiFiManagerParameter
	ESP_WMParameter custom_mqtt_hostname("hostname", "hostname", Configuration.hostname().c_str(), 60);

	// add all your parameters here
	wm.addParameter(&custom_mqtt_hostname);

	Log.println("Try to connect to WiFi...");
	// wm.setWiFiChannel(6);
	wm.setConfigPortalTimeout(30); // Set Timeout for portal configuration to 300 seconds
	if (!wm.autoConnect(Configuration.hostname().c_str()))
	{
		Log.println("failed to connect and hit timeout");
		delay(3000);
		//reset and try again, or maybe put it to deep sleep
		ESP.restart();
		delay(5000);
	}

	Log.println(String("Connected to ") + WiFi.SSID());
	Log.println(String("IP address: ") + WiFi.localIP().toString());

	// Disable sleep mode
	WiFi.setSleep(false);

	// Stop AP Mode
	WiFi.enableAP(false);
	WiFi.softAPdisconnect();
	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);

	/* Get configuration from WifiManager */
	Configuration.hostname(custom_mqtt_hostname.getValue());
	Configuration.saveConfig();

	/* Initialize HTTP Server */
    HTTPServer.setup();

    /* Initialize OTA Server */
    Log.println("Arduino OTA activated");

    // Port defaults to 3232
    ArduinoOTA.setPort(3232);
    ArduinoOTA.setHostname(Configuration.hostname().c_str());

    ArduinoOTA.onStart([&]() {
        Log.println("Arduino OTA: Start updating");
    });

    ArduinoOTA.onEnd([]() {
        Log.println("Arduino OTA: End");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Log.print("Arduino OTA Progress: ");
        Log.print(String(progress / (total / 100)));
        Log.println(" %");
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Log.print("Arduino OTA Error : ");
        Log.print(String(error));
        if (error == OTA_AUTH_ERROR)
            Log.println("Arduino OTA: Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Log.println("Arduino OTA: Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Log.println("Arduino OTA: Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Log.println("Arduino OTA: Receive Failed");
        else if (error == OTA_END_ERROR)
            Log.println("Arduino OTA: End Failed");
    });

    ArduinoOTA.begin();

    /* Initialize MDNS */
    //TODO : look at ESPmDNS

   // MDNS.begin(Configuration.hostname().c_str());
    //MDNS.addService("http", "tcp", 80);
}

void NetworkManager::handle()
{
    Log.println("NetworkManager::handle()");
	HTTPServer.handle();
    ArduinoOTA.handle();
    //MDNS.update();

    unsigned long currentMillis = millis();
    if ((currentMillis - _tickNTPUpdate) >= (unsigned long)(NTP_UPDATE_INTERVAL_MSEC))
    {
        updateNTP();
        _tickNTPUpdate = currentMillis;
    }
}

void NetworkManager::updateNTP()
{
  configTime(UTC_OFFSET * 3600, 0, NTP_SERVERS);
  delay(500);
  while (!time(nullptr))
  {
    Log.print("#");
    delay(1000);
  }
  Log.println("Update NTP");
}

#if !defined(NO_GLOBAL_INSTANCES) 
NetworkManager Network;
#endif
