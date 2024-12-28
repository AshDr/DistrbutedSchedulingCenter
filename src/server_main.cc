#include "schedulingcenter.h"
#include <thread>
int main() {
    SchedulingCenter schedulingcenter(8888);
    std::thread work([&schedulingcenter]() {
        schedulingcenter.Run();
    });
    work.detach();
    int cnt = 0;
    while (1) {
        schedulingcenter.AddTask(++cnt);
        sleep(3);
        if(cnt == 3) {
            schedulingcenter.UpdatePlugin("libfuncv2.so", "../plugins/");
        }
    }
    return 0;
}