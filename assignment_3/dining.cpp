#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>   // for sleep()

using namespace std;

#define N 5   // Number of philosophers

sem_t chopstick[N];  // Semaphore for chopsticks
sem_t room;          // To prevent deadlock

// Function for each philosopher
void* philosopher(void* num) {
    int id = *(int*)num;

    while (true) {
        // Thinking
        cout << "Philosopher " << id << " is thinking..." << endl;
        sleep(1);

        // Try to enter room (at most N-1 philosophers allowed)
        sem_wait(&room);

        // Pick left chopstick
        sem_wait(&chopstick[id]);

        // Pick right chopstick
        sem_wait(&chopstick[(id + 1) % N]);

        // Eating
        cout << "Philosopher " << id << " is eating..." << endl;
        sleep(2);

        // Put down left chopstick
        sem_post(&chopstick[id]);

        // Put down right chopstick
        sem_post(&chopstick[(id + 1) % N]);

        // Leave room
        sem_post(&room);
    }
}

int main() {
    pthread_t ph[N];
    int ids[N];

    // Initialize room semaphore (N-1 philosophers allowed)
    sem_init(&room, 0, N - 1);

    // Initialize all chopsticks (1 means available)
    for (int i = 0; i < N; i++) {
        sem_init(&chopstick[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&ph[i], NULL, philosopher, &ids[i]);
    }

    // Join threads (will run indefinitely)
    for (int i = 0; i < N; i++) {
        pthread_join(ph[i], NULL);
    }

    return 0;
}