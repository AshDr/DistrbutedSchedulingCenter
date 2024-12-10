#pragma once

#include "plugin.h"
#include <vector>
class PluginManager {
  public:
    void LoadPlugin(Plugin *plugin);
    void UpdatePlugin(std::string plugin_name);

  private:
    std::vector<Plugin *> plugins_;
};