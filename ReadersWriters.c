#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 16

int buffer[BUFFER_SIZE];
int counter, readerCount;

sem_t database, mutex;
pthread_t readerThread[50], writerThread[50];

void init() {
sem_init(&mutex, 0, 1);
sem_init(&database, 0, 1);
counter = 0;
readerCount = 0;
}

void *writer(void *param) {
int id = *(int *)param;
sem_wait(&database);

int item = rand() % 100;
buffer[counter] = item;
printf("Data written by writer %d is %d\n", id, buffer[counter]);
counter++;

sleep(1);
sem_post(&database);
pthread_exit(0);
}

void *reader(void *param) {
int id = *(int *)param;

sem_wait(&mutex);
readerCount++;
if (readerCount == 1) {
sem_wait(&database);
}
sem_post(&mutex);

if(counter > 0) {
printf("Data read by reader %d is %d\n", id, buffer[counter - 1]);
} else {
printf("Buffer empty, reader %d reads nothing\n", id);
}
sleep(1);

sem_wait(&mutex);
readerCount--;
if (readerCount == 0) {
sem_post(&database);
}
sem_post(&mutex);

pthread_exit(0);
}

int main() {
init();
int no_of_writers, no_of_readers;
printf("Enter number of readers: ");
scanf("%d", &no_of_readers);
printf("Enter number of writers: ");
scanf("%d", &no_of_writers);

int i;
int writer_ids[no_of_writers];
int reader_ids[no_of_readers];

for(i = 0; i < no_of_writers; i++) {
writer_ids[i] = i + 1;
pthread_create(&writerThread[i], NULL, writer, &writer_ids[i]);
}

for(i = 0; i < no_of_readers; i++) {
reader_ids[i] = i + 1;
pthread_create(&readerThread[i], NULL, reader, &reader_ids[i]);
}

for(i = 0; i < no_of_writers; i++) {
pthread_join(writerThread[i], NULL);
}

for(i = 0; i < no_of_readers; i++) {
pthread_join(readerThread[i], NULL);
}

return 0;
}

