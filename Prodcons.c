#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 5

int count = 0, in = 0, out = 0, a[BUFFER_SIZE];
sem_t full;
sem_t empty;
pthread_mutex_t mutex;

void *producer(void *thread);
void *consumer(void *thread);

int main() {
int i, p, c;
pthread_t pid[10], cid[10];

pthread_mutex_init(&mutex, NULL);
sem_init(&full, 0, 0);
sem_init(&empty, 0, BUFFER_SIZE);

printf("\nEnter number of producers: ");
scanf("%d", &p);
printf("\nEnter number of consumers: ");
scanf("%d", &c);

int producer_indices[p];
int consumer_indices[c];

// Create producer threads
for (i = 0; i < p; i++) {
producer_indices[i] = i + 1;
pthread_create(&pid[i], NULL, producer, &producer_indices[i]);
}

// Create consumer threads
for (i = 0; i < c; i++) {
consumer_indices[i] = i + 1;
pthread_create(&cid[i], NULL, consumer, &consumer_indices[i]);
}

// Join producer threads
for (i = 0; i < p; i++) {
pthread_join(pid[i], NULL);
}

// Join consumer threads
for (i = 0; i < c; i++) {
pthread_join(cid[i], NULL);
}

sem_destroy(&full);
sem_destroy(&empty);
pthread_mutex_destroy(&mutex);

return 0;
}

void *producer(void *thread) {
int t = *(int *)thread;
while (1) {
int item = rand() % 100; // Produce random item

sem_wait(&empty); // Wait if buffer is full
pthread_mutex_lock(&mutex); // Lock buffer

a[in] = item;
printf("\nProducer %d produced: %d", t, item);

in = (in + 1) % BUFFER_SIZE;
count++;

pthread_mutex_unlock(&mutex); // Unlock buffer
sem_post(&full); // Signal consumer

sleep(1);
}
pthread_exit(0);
}

void *consumer(void *thread) {
int t = *(int *)thread;
while (1) {
sem_wait(&full); // Wait if buffer is empty
pthread_mutex_lock(&mutex); // Lock buffer

int item = a[out];
printf("\nConsumer %d consumed: %d", t, item);

out = (out + 1) % BUFFER_SIZE;
count--;

pthread_mutex_unlock(&mutex); // Unlock buffer
sem_post(&empty); // Signal producer

sleep(1);
}
pthread_exit(0);
}

