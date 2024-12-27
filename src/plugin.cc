#include "plugin.h"
Plugin::Plugin(std::string plugin_name, PluginFunction func) {
  plugin_name_ = plugin_name;
  plugin_func_ = func;
}
std::string Plugin::GetPluginName() { return plugin_name_; }