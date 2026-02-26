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
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevator_clearButtonLamps(&e);

    elevator_calibrate(&e);


    while(1){

        int sensor = elevio_floorSensor();
        if(sensor != -1) {
            e.floor = sensor;
        }

        e.stopButton = elevio_stopButton();
        e.obstruction = elevio_obstruction();
        elevator_requests(&e);

        if (e.state == IDLE) {
                if (elevator_hasRequestsHere(&e)) {
                    // Åpne døren hvis det er en bestilling akkurat her
                    e.doorTime = time(NULL);
                    e.state = DOOR_OPEN;
                } 
                else if (elevator_hasRequestsOver(&e)) {
                    elevio_motorDirection(DIRN_UP);
                    e.state = MOVING;
                } 
                else if (elevator_hasRequestsBelow(&e)) {
                    elevio_motorDirection(DIRN_DOWN);
                    e.state = MOVING;
                }
            } 
            
            else if (e.state == MOVING) {
                // Vi bruker hasRequestsHere for å sjekke om vi skal stoppe
                if (sensor != -1 && elevator_hasRequestsHere(&e)) {
                    elevio_motorDirection(DIRN_STOP);
                    e.doorTime = time(NULL);
                    e.state = DOOR_OPEN;
                }
            } 
            
            else if (e.state == DOOR_OPEN) {
                state_doorOpen(&e); // Denne må inneholde elevator_clearRequests!
            }

        stateMachine(&e);

        elevator_updateButtonLamps(&e);
        elevator_updateFloorLamps(&e);
        elevator_updateStopLamp(&e);
            
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            elevator_clearButtonLamps(&e);
            break;
        }

        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    /*while(1){
        int floor = elevio_floorSensor();

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }
        e.stopButton = elevio_stopButton();
        e.obstruction = elevio_obstruction();

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }


        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }


        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }*/

    return 0;
}
