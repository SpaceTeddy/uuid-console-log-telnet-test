#include <Arduino.h>

#include <memory>
#include <string>
#include <vector>

#include <uuid/common.h>
#include <uuid/console.h>
#include <uuid/telnet.h>
#include <uuid/syslog.h>
#include "commands.h"

using uuid::read_flash_string;
using uuid::flash_string_vector;
using uuid::console::Commands;
using uuid::console::Shell;

// uuid-console + telnet
static std::shared_ptr<uuid::console::Commands> commands = std::make_shared<uuid::console::Commands>();
static uuid::telnet::TelnetService telnet{commands};

//uuid-syslog
static uuid::syslog::SyslogService syslog;

//------------------------------[ Wifi settings ]--------------------------------
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;
const uint32_t connectTimeoutMs = 5000;

String hostname_base = "esp32-S3_";

// setup WiFi connection
void setup_wifi() {
  
    // Set in station mode
    WiFi.mode(WIFI_STA);
    WiFi.setTxPower(WIFI_POWER_11dBm); //+10dBm
     
    // Register multi WiFi networks
    wifiMulti.addAP("your ssid", "your pw");
    
    if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
        Serial.print(F("SSID:"));Serial.print(WiFi.SSID()); Serial.print(F(" IP:")); Serial.print(WiFi.localIP());Serial.print(F(" RSSI: "));Serial.println(WiFi.RSSI()); 
        
        //get local time from ntp Server
        Serial.println("Adjusting system time.from ntp server..");
        //configTime(2 * 60 * 60, 0, "pool.ntp.org", "ntp.nict.jp", "time.google.com");        
        configTime(0,0,"pool.ntp.prg", "ntp.nict.jp", "time.google.com");
        
        //The ESP32 tries to reconnect automatically when the connection is lost
        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);

        // Disable AP mode once connected
        if (WiFi.softAPgetStationNum() == 0) { // Only if no clients are connected
            WiFi.softAPdisconnect(true);
            Serial.println("AP disabled, connected to WiFi");
        }

    } else {
        // no wifi connection, start AP for configuration
        const char* apSSID     = "ESP32_AP";
        const char* apPassword = "12345678";

        WiFi.softAP(apSSID, apPassword);
        Serial.println("Access Point started, AP:");
        Serial.println(apSSID);
        delay(1000);
    }   
}

void setup(){
    
    //setup wifi
    setup_wifi();
    
    //Setup Console
    registerCommands(commands);
    telnet.start();

    //Setup syslog logger
    static uuid::log::Logger logger{F("setup")};

    syslog.start();
    String hostname_syslog = hostname_base;
    syslog.hostname(hostname_syslog.c_str());
    syslog.log_level(uuid::log::ALL);
    syslog.mark_interval(3600);
    syslog.destination(IPAddress(192, 168, 0, 207), 514);

    // Log testmessage
    logger.info(F("LLU Client Application started"));
}

void loop(){

    uuid::loop();
    syslog.loop();
    telnet.loop();
    Shell::loop_all();yield();

    static uuid::log::Logger logger{F("loop")};

    logger.info(F("test message"));
    delay(1000);

}