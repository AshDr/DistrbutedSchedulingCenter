#include "schedulingcenter.h"
int main() {
    std::shared_ptr<TCPServer<SchedulingCenter>> server =
        std::make_shared<TCPServer<SchedulingCenter>>(8888, 100, "127.0.0.1");
    server->Run();
    auto tmp = std::dynamic_pointer_cast<SchedulingCenter>(server);
    int cnt = 0;
    while (1) {
        for (int i = 0; i < 10; i++) {
            tmp->AddTask(++cnt);
        }
        sleep(10);
    }
    return 0;
}