#pragma once

#include <time.h>
#include "elevio.h"
#include <stdlib.h>
typedef enum {
    IDLE,
    MOVING,
    DOOR_OPEN
} ElevatorState;

//snapshots
typedef struct {
MotorDirection dir;
ElevatorState state;
time_t doorTime; 
int floor; // etasjen man er på (0-3) (-1 for udefinert)
int stopButton; // 0 eller 1
int obstruction; // 0 eller 1
int requests[N_FLOORS][N_BUTTONS]; // Bestillinger / kømatrise

} Elevator;

void elevator_calibrate(Elevator *e); // kalibrerer heisen (oppstartsekvens) O1, O2, O3
void elevator_requests(Elevator *e); // tar imot bestillinger og legger inn i matrisen H1, H2
void elevator_clearRequests(Elevator *e); // fjerner alle requests på den etasjen man er på H3


//bestillingslogikk
void elevator_hasRequestsBelow(Elevator *e);
void elevator_hasRequestsOver(Elevator *e);
void elevator_hasRequestsHere(Elevator *e);

//lys logikk
void elevator_updateButtonLamps(Elevator *e); //oppdaterer alle knappelysene
void elevator_clearButtonLamps(Elevator *e); //slukker alle lysene
void elevator_updateFloorLamps(Elevator *e); //setter lysene som indikerer hvilken etasje vi er i
void elevator_updateStopLamp(Elevator *e); //oppdaterer stopp lyset

