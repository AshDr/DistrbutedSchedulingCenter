#include "branchoffice.h"
#include <cstdlib>
#include <thread>
#include "plugin.h"
#include "pluginmanager.h"
int main(int argc, char *argv[]) {
  int id = argc > 1 ? std::atoi(argv[1]) : 1;
  BranchOffice branchoffice(8888, "0.0.0.0", 5, id);
  PluginManager manager;
  Plugin start;
  manager.LoadPlugin("../plugins/libfuncv1.so", start);
  branchoffice.SetPlugin(start);
  branchoffice.Run();
  return 0;
}