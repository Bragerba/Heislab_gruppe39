#pragma once
#include "elevator.h"

//Denne skal være ferdig :)

void stateMachine(Elevator *e); //har tilstandene til heisen

void state_doorOpen(Elevator *e); //Åpner døren

void state_idle(Elevator *e); //tilstand når heisen står stille

void state_moving(Elevator *e); //styrer bevegelse