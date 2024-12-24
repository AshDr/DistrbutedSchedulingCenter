#include "branchoffice.h"
#include <cstdlib>
int main(int argc, char *argv[]) {
  int id = argc > 1 ? std::atoi(argv[1]) : 1;
  BranchOffice branchoffice(8888, "0.0.0.0", 5, id);
  branchoffice.Run();
  return 0;
}