#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

#define MAX 100

int n; // number of philosophers

sem_t chopstick[MAX]; // Each chopstick is a semaphore initialized to 1 (available)
sem_t room; //Controls how many philosophers try to eat (preventing deadlock)
pthread_mutex_t print_mutex; //mutex variable to synchronize print statements

void* philosopher(void* num)
{
    int id = *(int*)num; // Philosopher ID (0 to n-1)
    
    for (int i = 0; i < 5; i++) // limited iterations for demonstration purpose.
    {
        pthread_mutex_lock(&print_mutex); //locking the mutex
        cout << "Philosopher " << id << " is thinking...\n";
        pthread_mutex_unlock(&print_mutex); // releasing the mutex lock

        sleep(1); // to Simulate thinking time (pausing the thread for a sec)

        sem_wait(&room); //decresing the room semaphore by 1. If the value is 0, it will block until a philosopher leaves the room (finishes eating).

        sem_wait(&chopstick[id]); //picking the left chopstick
        sem_wait(&chopstick[(id + 1) % n]); //picking the. right chopstick.

        pthread_mutex_lock(&print_mutex);
        cout << "Philosopher " << id << " is eating...\n";
        pthread_mutex_unlock(&print_mutex);

        sleep(2);

        sem_post(&chopstick[id]); //releasing left chopstick
        sem_post(&chopstick[(id + 1) % n]); //releasing right chopstick

        sem_post(&room); //increasing room by 1, to allow other philisophers to enter.

        pthread_mutex_lock(&print_mutex);
        cout << "Philosopher " << id << " finished eating.\n";
        pthread_mutex_unlock(&print_mutex);
    }

    pthread_exit(NULL);
}

int main()
{
    cout << "Enter number of philosophers: ";
if (!(cin >> n))
{
    cout << "Error: Invalid input! Please enter an integer.\n";
    cin.clear();
    cin.ignore(1000, '\n');
    return 1;
}

if (n <= 0)
{
    cout << "Error: Number of philosophers must be greater than 0.\n";
    return 1;
}

if (n == 1)
{
    cout << "Error: Single philosopher cannot eat (needs 2 chopsticks).\n";
    return 1;
}

if (n > MAX)
{
    cout << "Error: Maximum allowed philosophers is " << MAX << ".\n";
    return 1;
}

    pthread_t philo[n]; //array to hold philosopher threads
    int ids[n]; //stores ids of philosophers

    // Initialize semaphores
    if (sem_init(&room, 0, n - 1) != 0)
{
    cout << "Error: Failed to initialize room semaphore.\n";
    return 1;
}

    for (int i = 0; i < n; i++)
{
    if (sem_init(&chopstick[i], 0, 1) != 0)
    {
        cout << "Error: Failed to initialize chopstick " << i << endl;
        return 1;
    }
}

    // Initialize mutex
   if (pthread_mutex_init(&print_mutex, NULL) != 0)
{
    cout << "Error: Mutex initialization failed.\n";
    return 1;
}

    // Create threads
  for (int i = 0; i < n; i++)
{
    ids[i] = i;

    if (pthread_create(&philo[i], NULL, philosopher, &ids[i]) != 0)
    {
        cout << "Error: Thread creation failed for philosopher " << i << endl;
        return 1;
    }
}

    // Join threads
    for (int i = 0; i < n; i++)
        pthread_join(philo[i], NULL);

        // Cleanup resources
for (int i = 0; i < n; i++)
{
    sem_destroy(&chopstick[i]);
}
sem_destroy(&room);
pthread_mutex_destroy(&print_mutex);
    return 0;
}