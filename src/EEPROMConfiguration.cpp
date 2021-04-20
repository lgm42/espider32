#include <EEPROM.h>

#include <ArduinoJson.h>

#include "Logger.h"
#include "EEPROMConfiguration.h"

#define START_ADDRESS 10
#define EXPECTED_EEPROM_VALIDITY  0xAB12CD89

/********************************************************/
/******************** Public Method *********************/
/********************************************************/

EEPROMConfiguration::EEPROMConfiguration()
{
}

EEPROMConfiguration::~EEPROMConfiguration()
{
}

void EEPROMConfiguration::setup(void)
{
  EEPROM.begin(sizeof(rawData) + START_ADDRESS);

	if (!readConfig()) {
		Log.println("Invalid configuration values, restoring default values");
		restoreDefault();
	}

	Log.println(String("    hostname: ") + String(_data.hostname));
}

bool EEPROMConfiguration::readConfig()
{
  Log.println("Read Configuration file from EEPROM...");
  EEPROM.get(START_ADDRESS, _data);
  return _data.eepromValidity == EXPECTED_EEPROM_VALIDITY;
}

void EEPROMConfiguration::saveConfig()
{
  EEPROM.put (START_ADDRESS, _data);
  EEPROM.commit();
}

String EEPROMConfiguration::hostname()
{
    return String(_data.hostname);
}

void EEPROMConfiguration::hostname(const String & value)
{
  strcpy(_data.hostname, value.c_str());
}

void EEPROMConfiguration::restoreDefault()
{  
  _data.eepromValidity = EXPECTED_EEPROM_VALIDITY;
  strcpy(_data.hostname, "espsider32");

	saveConfig();
	Log.println("configuration restored.");
}

String EEPROMConfiguration::toJson()
{
  DynamicJsonDocument conf(4096);

  conf["hostname"] = hostname();

  String output;  
  serializeJson(conf, output);
  return output;
}

void EEPROMConfiguration::fromJson(const String & json)
{
  Log.println(json);
  DynamicJsonDocument conf(1024);
  deserializeJson(conf, json);

  Log.println("deserialized");

  const char* tmp;
  tmp = conf["hostname"].as<const char*>();
  memset(_data.hostname, 0, 30);
  strncpy(_data.hostname, tmp, min((int)strlen(tmp), 30));

  saveConfig();
}

/********************************************************/
/******************** Private Method ********************/
/********************************************************/

#if !defined(NO_GLOBAL_INSTANCES) 
EEPROMConfiguration Configuration;
#endif
