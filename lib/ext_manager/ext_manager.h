/*
 *  ext_manager.h
 *  Created on: sep 2021
 *  Version: 1.0
 *  License: MIT
 *
 */


#ifndef EXT_MANAGER_H
#define EXT_MANAGER_H


// Includes ================================================================================

//Já incluso nas bibliotecas padrões
#include <WiFi.h>
#include "FS.h"
#include "WebServer.h"
#include "time.h"

#define uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  5        //Time ESP32 will go to sleep (in seconds)
#define HORA_WAKEUP_FROM_OFF    7
extern uint8_t flag_sleep;

// WIFI FUnctions =========================================================================
namespace WIFI {
	
	const char * const SSID = "MATRIX";
	const char * const PASSWORD = "100lula12"; 

	void ConnectToWiFi(void);
}

//RTOS FUnctions =========================================================================
namespace RTOS {
    void RTOS_init(void);
    void keepWiFiAlive(void * parameters);
    void keepSleep(void * parameters);
}

// Server FUnctions =========================================================================
namespace SERVER {
    extern WebServer server;


    void handle_OnConnect(void);
    void handle_rele_on(void);
    void handle_rele_off(void);
    void handle_sleep_off(void);
    void handle_sleep_on(void);
    String SendHTML(uint8_t);
    void server_init(void);

}


namespace NTP {

    const char * const ntpServer = "pool.ntp.org" ;
    const long gmtOffset_sec = 0;
    const int   daylightOffset_sec = 3600;
    

    void ntp_init(void);
    uint8_t comp_hour(void);

}








#endif