#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>

#include "elevio.h"
#include "con_load.h"

static int sockfd;
static pthread_mutex_t sockmtx;

void elevio_init(void){
    char ip[16] = "localhost";
    char port[8] = "15657";
    con_load("source/driver/elevio.con",
        con_val("com_ip",   ip,   "%s")
        con_val("com_port", port, "%s")
    )
    
    pthread_mutex_init(&sockmtx, NULL);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1 && "Unable to set up socket");
    
    struct addrinfo hints = {
        .ai_family      = AF_INET, 
        .ai_socktype    = SOCK_STREAM, 
        .ai_protocol    = IPPROTO_TCP,
    };
    struct addrinfo* res;
    getaddrinfo(ip, port, &hints, &res);
    
    int fail = connect(sockfd, res->ai_addr, res->ai_addrlen);
    assert(fail == 0 && "Unable to connect to elevator server");
    
    freeaddrinfo(res);
    
    send(sockfd, (char[4]){0}, 4, 0);
}




void elevio_motorDirection(MotorDirection dirn){
    pthread_mutex_lock(&sockmtx); //Styrer motoren til heisen - enten opp (1), ned (-1) eller stå stille (0)
    send(sockfd, (char[4]){1, dirn}, 4, 0); 
    pthread_mutex_unlock(&sockmtx);
}


void elevio_buttonLamp(int floor, ButtonType button, int value){ //value enten 1 eller 0 for på og av
    assert(floor >= 0); //krasjer programmet hvis du prøver å aksessere en etasje som ikke finnes
    assert(floor < N_FLOORS);
    assert(button >= 0); 
    assert(button < N_BUTTONS);

    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){2, button, floor, value}, 4, 0);
    pthread_mutex_unlock(&sockmtx); //bør skru den på når noen trykker på knappen, og av når heisen ankommer etasjen
}


void elevio_floorIndicator(int floor){ 
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){3, floor}, 4, 0);
    pthread_mutex_unlock(&sockmtx); //Denne bør du kalle hver gang elevio_floorSensor() returnerer noe annet enn -1
}


void elevio_doorOpenLamp(int value){ //Styrer lampen som indikerer at døren er åpen
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){4, value}, 4, 0); //Når heisen står stille og skal slippe ut folk, setter du denne til 1
    pthread_mutex_unlock(&sockmtx); //setter den til 0 før du kjører videre.
}


void elevio_stopLamp(int value){ //Om value = 1 slås lyset på, om den er 0 skrus den av
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){5, value}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}




int elevio_callButton(int floor, ButtonType button){
    pthread_mutex_lock(&sockmtx); //bruker pthread_mutex_lock for å sikre at ikke to deler av programmet prøver å sende data over nettverket samtidig.
    send(sockfd, (char[4]){6, button, floor}, 4, 0); 
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1]; //Den returnerer 1 hvis knappen er trykket inn, og 0 hvis ikke.
}


int elevio_floorSensor(void){ //Denne funksjonen sjekker om heisen befinner seg i en etasje akkurat nå.
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){7}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0); //en sjekker buf[1]. Hvis denne er "sann" (heisen er i en etasje), returnerer den etasjenummeret (buf[2]).
    pthread_mutex_unlock(&sockmtx); //Hvis heisen er mellom to etasjer, returnerer den -1
    return buf[1] ? buf[2] : -1; //Etasjenummer (0,1,2,3) eller −1
} 


int elevio_stopButton(void){ //sjekker om den fysiske stoppknappen i heisen holdes inne akkurat nå
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){8}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1]; //Returnerer 1 hvis knappen er trykket inn, og 0 hvis ikke.
}


int elevio_obstruction(void){ //simulerer fotocellen eller klembeskyttelsen i døren
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){9}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1]; //Returnerer 1 hvis noe blokkerer døren, og 0 hvis banen er klar
}
