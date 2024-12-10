#pragma once

#include "branchoffice.h"
#include "task.h"
#include <vector>
class ScedulingCenter {
    void AssignTask();
    void monitorTask();
    void GenerateReport();

  private:
    std::vector<Task *> tasks_;
    std::vector<BranchOffice *> branchOffices_;
};