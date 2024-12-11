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

setup(){
    //Setup Console
    registerCommands(commands);
    telnet.start();

    //Setup syslog
    //Setup Logger
    static uuid::log::Logger logger{F("setup")};

    syslog.start();
    String hostname_syslog = hostname_base + get_flashmemory_id();
    syslog.hostname(hostname_syslog.c_str());
    syslog.log_level(uuid::log::ALL);
    syslog.mark_interval(3600);
    syslog.destination(IPAddress(192, 168, 0, 207), 514);

    // Log testmessage
    logger.info(F("LLU Client Application started"));
}

loop(){

    uuid::loop();
    syslog.loop();
    telnet.loop();
    Shell::loop_all();yield();

    logger.info(F("TestMessage"));
    delay(1000);
  
}
