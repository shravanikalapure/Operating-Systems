#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define MAX 100

int n;

// Semaphores
sem_t *chopstick[MAX];
sem_t *room;

// Mutex for printing + counters
pthread_mutex_t print_mutex;

// Counters for tracking
int in_room_count = 0;
int eating_count = 0;

// Philosopher function
void* philosopher(void* num)
{
    int id = *(int*)num;

    for (int i = 0; i < 5; i++)
    {
        // THINKING
        pthread_mutex_lock(&print_mutex);
        cout << "Philosopher " << id << " is THINKING\n";
        pthread_mutex_unlock(&print_mutex);

        sleep(1);

        // HUNGRY → trying to enter room
        sem_wait(room);

        pthread_mutex_lock(&print_mutex);
        in_room_count++;
        cout << "Philosopher " << id << " ENTERED room | Inside = " << in_room_count << endl;
        pthread_mutex_unlock(&print_mutex);

        // Pick chopsticks
        sem_wait(chopstick[id]);
        sem_wait(chopstick[(id + 1) % n]);

        // CRITICAL SECTION (EATING)
        pthread_mutex_lock(&print_mutex);
        eating_count++;
        cout << "Philosopher " << id << " is EATING (Critical Section) | Eating count = " << eating_count << endl;
        pthread_mutex_unlock(&print_mutex);

        sleep(2);

        // Release chopsticks
        sem_post(chopstick[id]);
        sem_post(chopstick[(id + 1) % n]);

        pthread_mutex_lock(&print_mutex);
        eating_count--;
        cout << "Philosopher " << id << " FINISHED eating | Eating count = " << eating_count << endl;
        pthread_mutex_unlock(&print_mutex);

        // Leave room
        sem_post(room);

        pthread_mutex_lock(&print_mutex);
        in_room_count--;
        cout << "Philosopher " << id << " LEFT room | Inside = " << in_room_count << endl;
        pthread_mutex_unlock(&print_mutex);
    }

    pthread_exit(NULL);
}

int main()
{
    cout << "Enter number of philosophers: ";
    cin >> n;

    if (n <= 1 || n > MAX)
    {
        cout << "Invalid input!\n";
        return 1;
    }

    pthread_t philo[n];
    int ids[n];

    // Room semaphore
    room = sem_open("/room", O_CREAT, 0644, n - 1);

    // Chopsticks
    for (int i = 0; i < n; i++)
    {
        string name = "/chopstick" + to_string(i);
        chopstick[i] = sem_open(name.c_str(), O_CREAT, 0644, 1);
    }

    pthread_mutex_init(&print_mutex, NULL);

    // Create threads
    for (int i = 0; i < n; i++)
    {
        ids[i] = i;
        pthread_create(&philo[i], NULL, philosopher, &ids[i]);
    }

    // Join
    for (int i = 0; i < n; i++)
        pthread_join(philo[i], NULL);

    // Cleanup
    sem_unlink("/room");
    for (int i = 0; i < n; i++)
    {
        string name = "/chopstick" + to_string(i);
        sem_unlink(name.c_str());
    }

    pthread_mutex_destroy(&print_mutex);

    return 0;
}