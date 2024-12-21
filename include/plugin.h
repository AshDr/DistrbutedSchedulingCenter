#pragma once
#include <iostream>
#include <string>
class Plugin {
  public:
    std::string GetPluginName();
    void work();

  private:
    std::string plugin_name_;
};