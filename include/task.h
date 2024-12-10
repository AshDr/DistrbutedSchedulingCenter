#pragma once

class Task{
public:
    void UpdateStatus(int status);
private:
    int m_task_id;    
    int m_task_status;
};