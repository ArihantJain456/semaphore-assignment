#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include<random>
#include <unistd.h>

using namespace std;

int seats;
int empty;

// semaphores

// for barber availability
sem_t s_barber;

// for customers
sem_t s_customer;

// for seats
sem_t s_seats;

// barber code
void *barber(void *arg){
  while(1){
    sem_wait(&s_customer);
    sem_wait(&s_seats);
    empty++;
    sem_post(&s_barber);
    sem_post(&s_seats);
    cout <<"\nBarber cutting hair"<< endl;
    sleep(rand()%5);
    // barber takes some time for cutting (here it is simulated by random number from 0 to 4)
  }
}

// customer code
void *customer(void *arg){
  while(1){
    sem_wait(&s_seats);
    if(empty > 0){
      // when seat is available
      empty--;
      sem_post(&s_customer);
      sem_post(&s_seats);
      sem_wait(&s_barber);
      cout<<"\nCustomer haircut Complete";
    }
    else{
      // when no waiting seat is available, customer has no option but to leave
      sem_post(&s_seats);
      cout << "\nNo empty seats for the customer." << endl;
    }
    sleep(rand()%2);
  }
}

int main(){
  cout<<"Enter no. of customer chairs\n";
  cin>>seats;
  empty = seats;

  pthread_t thread_barber;
  pthread_t thread_customers;

  sem_init(&s_barber, 0, 0);
  sem_init(&s_seats, 0, 1);
  sem_init(&s_customer, 0, 0);

  pthread_create(&thread_barber, NULL, barber, NULL);
  pthread_create(&thread_customers, NULL, customer, NULL);

  pthread_exit(NULL);
}
