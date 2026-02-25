#include "statemachine.h"


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

}

void state_idle(Elevator *e) {

}

void state_moving(Elevator *e) {

}