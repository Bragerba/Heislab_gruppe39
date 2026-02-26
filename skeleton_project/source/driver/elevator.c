
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
        for (int b = 0; b < N_BUTTONS; b++)

        if (elevio_callButton(f, b)) {
            e->requests[f][b] = 1; //setter verdien i matrisen til 1 i riktig etasje
        }
    }
}

void elevator_clearRequests(Elevator *e){
   if(e->floor == -1) return; // Ikke slett noe hvis vi ikke er i en etasje

    for (int b = 0; b < N_BUTTONS; b++){
        e->requests[e->floor][b] = 0;
    }
}

//lyslogikk

void elevator_updateButtonLamps(Elevator *e){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int value = e->requests[f][b];

            if ((f == 0 && b == BUTTON_HALL_DOWN) || (f == N_FLOORS - 1 && b == BUTTON_HALL_UP)) {
                continue;
            }
            
            elevio_buttonLamp(f, b, value);
        }
    }
}

void elevator_clearButtonLamps(Elevator *e){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            elevio_buttonLamp(f, b, 0);
        }
    }
}

void elevator_updateFloorLamps(Elevator *e){
    if (e->floor != -1){
        elevio_floorIndicator(e->floor);
    }
}

void elevator_updateStopLamp(Elevator *e){
    elevio_stopLamp(e->stopButton);
}
