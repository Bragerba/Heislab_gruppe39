#include "lights.h"
#include "elevio.h"

void floorIndicatorLight(){
    int floor = elevio_floorSensor();
    elevio_floorIndicator(floor);
}