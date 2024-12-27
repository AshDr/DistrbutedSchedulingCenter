#include "pluginmanager.h"
#include "plugin.h"
#include <dirent.h>
#include <dlfcn.h>
#include <cstring>
#include "utils.h"

bool PluginManager::LoadPlugin(std::string plugin_path, Plugin& my_plugin) {
    void* handle = dlopen(plugin_path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        return false;
    }
    PluginFunction func = (PluginFunction)dlsym(handle, "func");
    // auto idx = plugin_path.find_last_of('/');
    // std::string plugin_name = std::string(plugin_path.begin()+idx,plugin_path.end());
    std::string plugin_name = "plugin_func";
    my_plugin = Plugin(plugin_name, func);
    return true;
}

bool PluginManager::GetPluginList(std::vector<std::string>& plugin_names) {
    DIR *dir = opendir("../plugins");
    if(dir == NULL) return false;
    while(1)
    {
        struct dirent *pentry = readdir(dir);
        if(pentry == 0)
            break;

        if(strcmp(pentry->d_name, ".") == 0)
            continue;

        if(strcmp(pentry->d_name, "..") == 0)
            continue;

        std::string str = "./plugins/";
        str += pentry->d_name;
        plugin_names.push_back(str);
    }
    closedir(dir);
    return true;
}