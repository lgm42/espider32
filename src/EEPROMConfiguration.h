#pragma once

class EEPROMConfiguration 
{
  public:
  	EEPROMConfiguration();
  	virtual ~EEPROMConfiguration();
  
  	void setup();

    bool readConfig();
  	void saveConfig();
  
  	void restoreDefault();
    
    String toJson();
	void fromJson(const String & json);
  
	String hostname();
	void hostname(const String & value);

  private:
	
  typedef struct 
	{
		uint32_t eepromValidity;
		char hostname[30];
	} rawData;

	rawData _data;
};

#if !defined(NO_GLOBAL_INSTANCES)
extern EEPROMConfiguration Configuration;
#endif
