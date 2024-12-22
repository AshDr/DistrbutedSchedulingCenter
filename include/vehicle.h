#pragma once

class Vehicle {
  public:
    Vehicle(int id, int status);
    void UpdateStatus(int status);
  private:
    int vehicle_id_;
    int vehicle_status_;
};