#include "task.h"
#include "utils.h"
#include <cstring>

void Task::UpdateStatus(int status) { task_status_ = status; }
bool Task::Serialize(std::vector<char> &buf) {
  buf.resize(sizeof(int) * 2);
  memcpy(buf.data(), &this->task_id_, sizeof(int));
  memcpy(buf.data() + sizeof(int), &this->task_status_, sizeof(int));
  return true;
}
bool Task::Deserialize(std::vector<char> &buf) {
  if (buf.size() < sizeof(int) * 2) {
    return false;
  }
  memcpy(&this->task_id_, buf.data(), sizeof(int));
  memcpy(&this->task_status_, buf.data() + sizeof(int), sizeof(int));
  return true;
}
int Task::GetType() { return static_cast<int>(CLASS_TYPE::TASK); }
