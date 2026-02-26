#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/elevator.h"
#include "driver/statemachine.h"



int main(){

    Elevator e;

    elevio_init();
    elevator_calibrate(&e);

    while(1) {

        int sensor = elevio_floorSensor();
        if(sensor != -1) {
            e.floor = sensor;
        }
        e.stopButton = elevio_stopButton();
        e.obstruction = elevio_obstruction();

        elevator_requests(&e);

        if (e.stopButton) {
            elevio_motorDirection(DIRN_STOP);
            elevator_clearAllRequests(&e);
            // e.state = IDLE; eller en egen STOP-state
        } else {
            stateMachine(&e);
        }

        elevator_updateButtonLamps(&e);
        elevator_updateFloorLamps(&e);
        elevator_updateStopLamp(&e);

        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
