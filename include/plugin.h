#pragma once
#include <iostream>
#include <string>
#include "utils.h"
class Plugin {
public:
    Plugin() {
      plugin_name_ = "NULL";
    }
    Plugin(std::string plugin_name, PluginFunction func);
    std::string GetPluginName();
    void Plugin_Work() {
      (plugin_func_)();
    }
  private:
    PluginFunction plugin_func_;
    std::string plugin_name_;
};