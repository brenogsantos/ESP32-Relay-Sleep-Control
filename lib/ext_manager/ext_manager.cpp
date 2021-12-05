/*
 *  EXT_MANAGER.cpp
 *  Created on: sep 2021
 *  Version: 1.0
 *  License: MIT
 *
 */

#ifndef EXT_MANAGER_CPP
#define EXT_MANAGER_CPP

// Includes ================================================================================

#include "ext_manager.h"

WebServer SERVER::server(80);
struct tm timeinfo;
RTC_DATA_ATTR uint8_t flag_sleep = 0, flag_rele_state = 1;
// Functions Implementations ===============================================================

// WIFI Functions =========================================================================
void WIFI::ConnectToWiFi(void){
 
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI::SSID, WIFI::PASSWORD);
	Serial.print("Connectando a "); Serial.println(WIFI::SSID);
	
	uint8_t i = 0;
    uint8_t count = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print('.');
		delay(500);
	
		if ((++i % 16) == 0)
		{
		Serial.println(F(" TENTANDO CONECTAR AINDA"));
        count++;
            if(count > 10){
                ESP.restart();
            } 
		}
	}
	
	Serial.print(F("Connectado. Meu IP é: "));
	Serial.println(WiFi.localIP());
}


// RTOS Functions =========================================================================
void RTOS::RTOS_init(void){
    xTaskCreatePinnedToCore(
        RTOS::keepWiFiAlive,
        "keep WiFi Alive",
        5000,
        NULL,
        1,
        NULL,
        0
    );
    xTaskCreatePinnedToCore(
        RTOS::keepSleep,
        "keep Sleep",
        5000,
        NULL,
        1,
        NULL,
        0
    );
}

void RTOS::keepWiFiAlive(void * parameters){
  for(;;){
    if(WiFi.status() == WL_CONNECTED){
        Serial.print("Wifi ainda conectado, IP: ");
        Serial.println(WiFi.localIP());
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        continue;
    }
    else{
        Serial.println("conectando no Wifi...");
        WIFI::ConnectToWiFi();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        continue;
    }

  }
}

void RTOS::keepSleep(void * parameters){
  for(;;){
    if(NTP::comp_hour() == 1){
        flag_sleep = 0;
        Serial.printf("flag_sleep: %d\n", flag_sleep);
        SERVER::handle_rele_on();
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        continue;
    }
    else if(NTP::comp_hour() == 2){         //pediu pra dormir
        flag_sleep = 1;
        Serial.printf("flag_sleep: %d\n", flag_sleep);
        SERVER::handle_sleep_off(); 
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        continue;
    }
    else if(NTP::comp_hour() == 3){     
        Serial.printf("flag_sleep: %d\n", flag_sleep);
        if(!flag_rele_state)SERVER::handle_rele_off(); 
        else SERVER::handle_rele_on(); 
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        continue;
    }
  }
}
// SERVER Functions =========================================================================
void SERVER::handle_OnConnect() {


    Serial.println("GPIO15 Status: ON");
    SERVER::server.send(200, "text/html", SendHTML(HIGH));
    
}

void SERVER::handle_rele_off() {

    flag_rele_state = 0;
    digitalWrite(15, LOW);
    digitalWrite(LED_BUILTIN,LOW);
    Serial.println("GPIO15 Status: OFF");
    SERVER::server.send(200, "text/html", SERVER::SendHTML(LOW)); 

}

void SERVER::handle_rele_on() {

    flag_rele_state = 1;
    flag_sleep = 0;
    gpio_hold_dis(GPIO_NUM_15);
    digitalWrite(RELAY_OUTPUT_PIN, HIGH);
    digitalWrite(LED_BUILTIN,HIGH);
    gpio_hold_en(GPIO_NUM_15);
    gpio_deep_sleep_hold_en();

    Serial.println("GPIO15 Status: ON");
    SERVER::server.send(200, "text/html", SERVER::SendHTML(HIGH)); 

}


void SERVER::handle_sleep_off() {

    flag_rele_state = 0;
    flag_sleep = 1;
    digitalWrite(RELAY_OUTPUT_PIN, LOW);
    digitalWrite(LED_BUILTIN,LOW);
  //  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    esp_sleep_enable_timer_wakeup(1800000000);
    //Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
	//" Seconds");
    esp_deep_sleep_start();
    

}

void SERVER::handle_sleep_on(void){

    gpio_hold_dis(GPIO_NUM_15);
    digitalWrite(RELAY_OUTPUT_PIN, HIGH);
    digitalWrite(LED_BUILTIN,HIGH);
    gpio_hold_en(GPIO_NUM_15);
    gpio_deep_sleep_hold_en();
    //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_sleep_enable_timer_wakeup(1800000000);
  //  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
//	" Seconds");
    esp_deep_sleep_start();

}


String SERVER::SendHTML(uint8_t rele_status){

    String ptr = "<!DOCTYPE html> <html>\n";
    ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr +="<title>Relay Control</title>\n";
    ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr +=".button-on {background-color: #3498db;}\n";
    ptr +=".button-on:active {background-color: #2980b9;}\n";
    ptr +=".button-off {background-color: #34495e;}\n";
    ptr +=".button-off:active {background-color: #2c3e50;}\n";
    ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr +="</style>\n";
    ptr +="</head>\n";
    ptr +="<body>\n";
    ptr +="<h1>RELE TWIBI</h1>\n";
    
    if(rele_status)
    {ptr +="<p>Twibi Status: ON</p><a class=\"button button-off\" href=\"/releoff\">OFF</a>\n";}
    else
    {ptr +="<p>Twibi Status: OFF</p><a class=\"button button-on\" href=\"/releon\">ON</a>\n";}

    
    ptr += "<p>\n</p>";
    ptr +="</body>\n";
    ptr +="</html>\n";
    return ptr;

}



void SERVER::server_init(void){

    SERVER::server.on("/", SERVER::handle_OnConnect);
    server.on("/releon", SERVER::handle_rele_on);
    server.on("/releoff", SERVER::handle_rele_off);
    server.on("/sleepoff", SERVER::handle_sleep_off);
   // server.on("/sleepon", SERVER::handle_sleep_on);
    // Start server
    SERVER::server.begin();

}


//NTP

void NTP::ntp_init(void){

    configTime(-10800, 0, "pool.ntp.org");

}

uint8_t NTP::comp_hour(void){

    NTP::ntp_init();
    char timeHour[3];
    uint16_t hora = 0;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return 3;
    }
    strftime(timeHour,3, "%H", &timeinfo);
    Serial.println(timeHour);
    hora = (atoi(timeHour));
    if(flag_sleep){          
        if(hora == HORA_WAKEUP_FROM_OFF){      // deu a hora de acordar
            return 1;      //quebra o sleep
        }
        if(hora > HORA_WAKEUP_FROM_OFF || hora < HORA_WAKEUP_FROM_OFF){  
            Serial.printf("FLAAAAAAG: %d\n", flag_sleep);
            return 2;      
        }
    }
    else{       //manter acordado
        if(hora >= HORA_WAKEUP_FROM_OFF){      
            return 3;       
        }
    }
}

#endif