#include "statemachine.h"
#include <stdio.h>
#include <stdlib.h>


void stateMachine(Elevator *e) {
    switch (e->state) {

        case IDLE:
        state_idle(e);
        break;

        case MOVING:
        state_moving(e);
        break;

        case DOOR_OPEN:
        state_doorOpen(e);
        break;
    }
}

// Fyll in logikk/ funksjonalitet
void state_doorOpen(Elevator *e) {

    elevio_doorOpenLamp(1);

    if(e->obstruction){
        e->doorTime = time(NULL);
    } 
    
    else if (time(NULL) - e->doorTime >= 3){
        elevio_doorOpenLamp(0);
        elevator_clearCurrentFloorRequests(e);
        e->state = IDLE;
    }

}  

void state_idle(Elevator *e) {
    
}

void state_moving(Elevator *e) {

}