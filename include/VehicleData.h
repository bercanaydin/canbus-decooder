#ifndef VEHICLE_DATA_H
#define VEHICLE_DATA_H

#include <cstdint>

struct VehicleState {
    uint16_t rpm = 0;
    uint8_t speed = 0;
    int temperature = 0;
    bool is_redline = false;
};

#endif