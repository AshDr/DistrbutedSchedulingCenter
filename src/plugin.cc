#include "plugin.h"
std::string Plugin::GetPluginName() { return plugin_name_; }
void Plugin::work() {
  std::cout << "Plugin " << plugin_name_ << " is working" << std::endl;
}