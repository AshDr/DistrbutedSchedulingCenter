#pragma once

#include "plugin.h"
#include <vector>
#include <string>
class PluginManager {
  public:
    bool LoadPlugin(std::string plugin_path, Plugin& my_plugin);
    bool GetPluginList(std::vector<std::string>& plugin_names);
  private:
    std::vector<std::string> PluginNames;
};