#include "ext_manager.h"



void setup() {
  // put your setup code here, to run once:

  pinMode(15, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(15, HIGH);
  Serial.begin(9600);
	WIFI::ConnectToWiFi();
  RTOS::RTOS_init();
  SERVER::server_init();
  NTP::ntp_init();



}

void loop() {

  SERVER::server.handleClient();

}