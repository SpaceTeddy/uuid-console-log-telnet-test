#pragma once
#include <memory>
#include <string>
#include <vector>

#include <uuid/common.h>
#include <uuid/console.h>
#include <uuid/telnet.h>
#include <uuid/console.h>

// Funktionen zur Befehlsregistrierung
void registerCommands(std::shared_ptr<uuid::console::Commands> commands);

// Hilfsfunktionen (falls benötigt)
int parseArgument(const std::vector<std::string> &arguments, size_t index, int defaultValue);
