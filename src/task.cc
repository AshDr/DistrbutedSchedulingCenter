#include "task.h"
#include <cstring>

void Task::UpdateStatus(int status) {
    task_status_ = status;
}
bool Task::Serialize(std::vector<char> &buf) {
    buf.resize(sizeof(int) * 2);
    memcpy(buf.data(), &task_id_, sizeof(int));
    memcpy(buf.data() + sizeof(int), &task_status_, sizeof(int));
    return true;
}
Serializable *Task::Deserialize(std::vector<char> &buf) {
    Task *p = new Task();
    memcpy(&(p->task_id_), buf.data(), sizeof(int));
    memcpy(&(p->task_status_), buf.data() + sizeof(int), sizeof(int));
    return p;
}
int Task::GetType() {
    return TASK;
}
