#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> 
using namespace std;

#define SEATS 20

sem_t s_barber;
sem_t s_seats;
sem_t s_customer;
int empty = SEATS;

void *barber(void *arg){
    while(true){
        sem_wait(&s_customer);
        sem_wait(&s_seats);
        empty++;
        sem_post(&s_barber);
        sem_post(&s_seats);
        cout <<"\nCustomer hair cuts"<< endl;
        sleep(3);
    }
}

void *customer(void *arg){
    while(true){
        sem_wait(&s_seats);
        if(empty > 0){
            empty--;
            sem_post(&s_customer);
            sem_post(&s_seats);
            sem_wait(&s_barber);
        }
        else{
            sem_post(&s_seats);
            cout << "\nNo empty seats for the customer." << endl;
        }
        sleep(1);
    }
}

int main(){
    pthread_t barber_thread;
    pthread_t customers_thread;

    sem_init(&s_barber, 0, 0);
    sem_init(&s_seats, 0, 1);
    sem_init(&s_customer, 0, 0);

    pthread_create(&barber_thread, NULL, barber, NULL);
    pthread_create(&customers_thread, NULL, customer, NULL);

    pthread_exit(NULL);
}
