
#include "elevator.h"
#include "elevio.h"

ElevatorState state;

void elevator_calibrate(Elevator *e) {
    elevio_motorDirection(DIRN_DOWN);

    while (elevio_floorSensor() == -1) {
        //fortsett å kjør til vi finner gyldig etasje
    }

    while (elevio_floorSensor() != 0) {
        //fortsett å kjør ned til første etasje
    }
    elevio_motorDirection(DIRN_STOP);

    //endre tilstandene til heisen
    e->floor = 0;
    e->dir = DIRN_STOP;
    e->state = IDLE;

}

void elevator_requests(Elevator *e) {
    for (int f = 0; f < N_FLOORS; f++) {
        for (int b = 0; f < N_BUTTONS; b++)

        if (elevio_callButton(f, b)) {
            e->requests[f][b] = 1; //setter verdien i matrisen til 1 i riktig etasje
        }
    }
}