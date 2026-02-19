#pragma once

typedef enum {
    IDLE,
    MOVING,
    DOOR_OPEN
} ElevatorState;

typedef struct {
ElevatorState state;
} Elevator;