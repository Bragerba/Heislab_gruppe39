#include "statemachine.h"
#include "elevator.h"
#include "elevio.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

void state_doorOpen(Elevator *e) {

    if(e->obstruction){
        e->doorTime = time(NULL);
        return;
    } 
    
    else if (time(NULL) - e->doorTime >= 3){
        elevio_doorOpenLamp(0);
        elevator_clearRequests(e);
        e->state = IDLE;
    }

}  

void state_idle(Elevator *e) {
    elevio_motorDirection(DIRN_STOP);

    if (elevator_hasRequestsHere(e)) {
        elevio_doorOpenLamp(1);
        e->doorTime = time(NULL);
        elevator_clearRequests(e);
        e->state = DOOR_OPEN;
    }

    else if (elevator_hasRequestsOver(e)) {
        e->dir = DIRN_UP;
        elevio_motorDirection(e->dir);
        e->state = MOVING;
    }

    else if (elevator_hasRequestsBelow(e)) {
        e->dir = DIRN_DOWN;
        elevio_motorDirection(e->dir);
        e->state = MOVING;
    }
}

void state_moving(Elevator *e) {
    int floor = elevio_floorSensor();
    if (floor != -1) {
        e->floor = floor;
    }

    if (elevator_shouldStop(e)) {
        elevio_motorDirection(DIRN_STOP);

        elevio_doorOpenLamp(1);
        e->doorTime = time(NULL);
        elevator_clearRequests(e);

        e->state = DOOR_OPEN;
        return;
    }
}