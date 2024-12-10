#pragma once
#include <string>
class Plugin {
  public:
    std::string GetPluginName();

  private:
    std::string plugin_name_;
};