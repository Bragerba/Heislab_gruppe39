#pragma once

#include <time.h>
#include <time.h>
#include "elevio.h"
#include <stdlib.h>
typedef enum {
    IDLE,
    MOVING,
    DOOR_OPEN
} ElevatorState;

typedef struct {
MotorDirection dir;
ElevatorState state;
clock_t doorTime; 
int floor; // etasjen man er på (0-3) (-1 for udefinert)
int stopButton; // 0 eller 1
int obstruction; // 0 eller 1
int requests[N_FLOORS][N_BUTTONS]; // Bestillinger / kømatrise

} Elevator;

void elevator_calibrate(Elevator *e); // kalibrerer heisen (oppstartsekvens) O1, O2, O3
void elevator_requests(Elevator *e); // tar imot bestillinger og legger inn i matrisen H1, H2
void elevator_clearCurrentFloorRequests(Elevator *e); // fjerner alle requests på den etasjen man er på H3
void elevator_clearAllRequests(Elevator *e); //fjerner alle requests
void elevator_handleStopButton(Elevator *e); //Håndterer hvordan stoppknappen skal tolkes basrt på heisposisjon


//bestillingslogikk, skal bare sjekke matrisen ikke endre objektet
int elevator_hasRequestsBelow(const Elevator *e);
int elevator_hasRequestsOver(const Elevator *e);
int elevator_hasRequestsHere(const Elevator *e);
int elevator_shouldStop(const Elevator *e); //logikk for om vi skal stoppe

//lys logikk
void elevator_updateButtonLamps(Elevator *e); //oppdaterer alle knappelysene
void elevator_updateFloorLamps(Elevator *e); //setter lysene som indikerer hvilken etasje vi er i
void elevator_updateStopLamp(Elevator *e); //oppdaterer stopp lyset


