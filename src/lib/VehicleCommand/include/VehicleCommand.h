#ifndef VEHICLE_COMMAND_H
#define VEHICLE_COMMAND_H

#include <Arduino.h>

class VehicleCommand {
    public:
        String vehicleId;
        String agentId;
        String action;
        String cmd;
        uint8_t value;

        VehicleCommand();
};

#endif