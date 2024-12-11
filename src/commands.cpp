#include "commands.h"

#include <cstdarg> // Für va_list, va_start, va_end
#include <cstdio>  // Für vsnprintf


int parseArgument(const std::vector<std::string> &arguments, size_t index, int defaultValue = 0) {
    if (index < arguments.size()) {
        return std::stoi(arguments[index]);
    }
    return defaultValue;
}

void helpCommand(uuid::console::Shell &shell, const std::vector<std::string> &) {
    shell.printfln(F("Available commands:"));
    shell.print_all_available_commands(); 
}

void espResetCommand(uuid::console::Shell &shell, const std::vector<std::string> &) {
    shell.printfln(F("ESP Reset!"));
    ESP.restart();
}

void registerCommands(std::shared_ptr<uuid::console::Commands> commands) {
    commands->add_command(uuid::flash_string_vector{F("help")}, helpCommand);
    commands->add_command(uuid::flash_string_vector{F("esp_reset")}, espResetCommand);
}