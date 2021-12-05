#include "ext_manager.h"



void setup() {
  // put your setup code here, to run once:

  pinMode(RELAY_OUTPUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(RELAY_OUTPUT_PIN, HIGH);
  Serial.begin(9600);
	WIFI::ConnectToWiFi();
  RTOS::RTOS_init();
  SERVER::server_init();
  NTP::ntp_init();



}

void loop() {

  SERVER::server.handleClient();

}