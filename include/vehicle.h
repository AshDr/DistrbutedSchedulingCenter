#pragma once

class Vehicle {
  public:
    void UpdateStatus(int status);

  private:
    int vehicle_id_;
    int vehicle_status_;
};