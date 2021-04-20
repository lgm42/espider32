#include <Arduino.h>

#include "contants.h"

#include "EEPROMConfiguration.h"
#include "NetworkManager.h"
#include "MovementManager.h"

void setup() {

	Serial.begin(115200);
	Configuration.setup();
	Network.setup();
	Movement.setup();

	delay(500);

}

void loop() {

	Network.handle();
	Movement.handle();

	Serial.println("top\n");
}