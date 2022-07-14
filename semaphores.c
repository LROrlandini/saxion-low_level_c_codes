/* Luciano Regis Orlandini - 460952
   ETI2v.IA

   Semaphores programme in C. */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define B 3 // Number of booths available
#define R 6 // Waiting room capacity
#define V 5 // Time taken to vaccinate
#define P 50 // Population to be vaccinated

// Declaration of semaphores
sem_t WaitRoom;
sem_t Corridor;
sem_t Booth;

// Global Variables
int boothCap = B;
int waitRoomCap = R;

// Function about patients trying to get vaccinated
void *tpatient(void *ptr) {
    int patient = *((int *) ptr) + 1; // Assigns number to patient

    printf("%d tries to enter room...\n", patient);

    // Checks if there space in the room, otherwise leaves for 5 seconds
    while(sem_trywait(&WaitRoom) == -1) {
	printf("ROOM FULL FOR %d\n", patient);
	sleep((rand() % 3) + 4);
    }
    waitRoomCap -= 1;
    printf("R = %d\n", waitRoomCap);
    printf("%d in waiting room...\n", patient);

    sem_wait(&Booth); // Checks if there is an available booth
    boothCap -= 1;
    printf("B = %d\n", boothCap);

    sem_wait(&Corridor); // Checks if the corridor is free
    printf("%d Crossing...\n", patient);
    sem_post(&WaitRoom); // One extra space in the room
    waitRoomCap += 1;
    printf("R = %d\n", waitRoomCap);
    sleep((rand() & 3) + 1); // Crosses corridor
    sem_post(&Corridor); // Corridor is now free

    printf("%d is getting vaccinated...\n", patient);
    sleep(V); // Patient being vaccinated
    printf("%d READY!!!\n", patient);
    sem_post(&Booth); // Booth is now free
    boothCap += 1;
    printf("B = %d\n", boothCap);
    assert(waitRoomCap >= 0 && waitRoomCap <= R);
    assert(boothCap >= 0 && boothCap <= B);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    int index, pat[P]; // Creates array with number of patients
    pthread_t thread[P]; // Creates 1 new thread per patient

    // Initialises semaphores
    sem_init(&WaitRoom, 0, R);
    sem_init(&Corridor, 0, 1);
    sem_init(&Booth, 0, B);

    srand(time(NULL));
    printf("R Capacity = %d\n", waitRoomCap);
    printf("B Capacity = %d\n\n", boothCap);

    // Initialises threads
    for (index = 0; index < P; index++) {
	pat[index] = index;
        sleep((rand() % 3) + 1);
	pthread_create(&thread[index], NULL, &tpatient, (void *) &pat[index]);
    }

    // Waits for thread to terminate
    for (index = 0; index < P; index++) {
	pthread_join(thread[index], NULL);
    }

    // Terminates semaphores
    sem_destroy(&WaitRoom);
    sem_destroy(&Corridor);
    sem_destroy(&Booth);

    return 0;
}

