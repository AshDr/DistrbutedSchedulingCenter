#include "vehicle.h"
Vehicle::Vehicle(int id, int status) {
  vehicle_id_ = id;
  vehicle_status_ = status;
}
void Vehicle::UpdateStatus(int status) { vehicle_status_ = status; }
int Vehicle::GetVehicleID() { return vehicle_id_; }